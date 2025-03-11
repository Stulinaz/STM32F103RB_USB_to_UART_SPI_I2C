#include "utility.h"
#include "i2c_eeprom.h"
#include "led_manager.h"

typedef struct{
	uint32_t time;
	uint32_t partial_time;
	utility_uptime_s handler;
}uptime_t;

uptime_t device_uptime = {0, 0, UPTIME_READ_DATA};

/****************************************************************************
Function:			utility_uptime_s UtilityUptime(void)
Input:				none
Output:				none
PreCondition:		rev 2 pcb with external 24lc64 eeprom
Overview:			read and stores activity time (in seconds) of the device
****************************************************************************/
utility_uptime_s UtilityUptime(void)
{
	uint8_t data[4];
	uint8_t i;

	switch(device_uptime.handler)
	{
		case UPTIME_READ_DATA:
		if( EepromRead(data, 4, 0x0000, EEPROM_SEQUENTIAL_READ) == EEPROM_NO_ERROR)
		{
			device_uptime.time = 0;
			for(i=0;i<4;i++)
				device_uptime.time += data[i] << (8*i);
			device_uptime.handler = UPTIME_GET_TICK;
		}
		else
		{
			ErrorCodeSet(5);
			device_uptime.handler = UPTIME_EEPROM_ERR;
		}
		break;

		case UPTIME_GET_TICK:
		device_uptime.partial_time	= HAL_GetTick();
		device_uptime.handler = UPTIME_CHECK_AND_STORE;
		break;

		case UPTIME_CHECK_AND_STORE:
		if ( (HAL_GetTick() - device_uptime.partial_time) >= EEPROM_UPTIME_STORE_TIME )
		{
			/* seconds */
			device_uptime.time += EEPROM_UPTIME_STORE_TIME/1000;

			data[0] = (uint8_t)(device_uptime.time);
			data[1] = (uint8_t)(device_uptime.time >> 8);
			data[2] = (uint8_t)(device_uptime.time >> 16);
			data[3] = (uint8_t)(device_uptime.time >> 24);

			if( EepromWrite(data, 4, 0x0000, EEPROM_PAGE_WRITE) == EEPROM_NO_ERROR)
				device_uptime.handler = UPTIME_GET_TICK;
			else
			{
				ErrorCodeSet(5);
				device_uptime.handler = UPTIME_EEPROM_ERR;
			}
		}
		break;

		case UPTIME_EEPROM_ERR:
		default:
		break;
	}
	return device_uptime.handler;
}

/****************************************************************************
Function:			uptime_time_t UptimeGetTime(void)
Input:				device_uptime.time) in seconds
Output:				convert seconds in the format (days,hours,minute and seconds)
PreCondition:		/
Overview:			device uptime since the first plug
****************************************************************************/
uptime_time_t UptimeGet(void)
{
	uptime_time_t curr_uptime = {0,0,0,0};
	uint32_t seconds = device_uptime.time;
	if(device_uptime.handler != UPTIME_EEPROM_ERR)
	{
		curr_uptime.days = seconds/DAY_IS_SECONDS;
		seconds -= curr_uptime.days*DAY_IS_SECONDS;
		curr_uptime.hours = seconds/HOUR_IS_SECONDS;
		seconds -= curr_uptime.hours*HOUR_IS_SECONDS;
		curr_uptime.min = seconds/MIN_IS_SECONDS;
		seconds -= curr_uptime.min*MIN_IS_SECONDS;
		curr_uptime.sec = seconds;
	}
	return curr_uptime;
}
