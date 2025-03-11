#include "app_usb.h"
#include "gpio.h"
#include "led_manager.h"
#include "buffers_manager.h"
#include "command_process.h"
#include "command_list.h"
#include "usbd_conf.h"
#include "usb_device.h"
#include "usbd_def.h"
#include "app.h"
#include "utility.h"
#include "sys_definitions.h"

static void PrintStatus(void);

typedef struct{
	command_manager_s cmd_manager;
	uint8_t           connection_status;
	uint8_t			  usb_reset_num;
	uint32_t          time_app;
	uint32_t          inactivity_time;
	uint8_t           cmd_len;
	command_t         user_cmd;
	uint8_t           user_data[10];
	uint8_t           user_data_len;
}interface_t;

interface_t user_interface = { CHECK_USB, DISCONNECTED, 0, 0, 0, 0, NO_COMMAND,
							 {0,0,0,0,0,0,0,0,0,0},0};

/****************************************************************************
Function:			DecodeFromPc
Input:				none
Output:				none
PreCondition:		USBInit()
Overview:			Handles message from user and application call
****************************************************************************/
void DecodeFromPc (void)
{
	switch(user_interface.cmd_manager)
	{
		case CHECK_USB:
		user_interface.time_app = HAL_GetTick();
		if(user_interface.time_app >= CONNECTION_TIMEOUT)
		{
			if(DeviceGetUSBState() != USBD_STATE_CONFIGURED)
			{
				user_interface.time_app = HAL_GetTick();
				UsbDisable();
				if(user_interface.usb_reset_num > 0)
					user_interface.cmd_manager = USB_FAIL;
				else
					user_interface.cmd_manager = RESET_USB;
			}
			else
			{
				user_interface.connection_status  = CONNECTED;
				user_interface.cmd_manager = NO_DATA_TO_PROCESS;
				USBStatusLed(GPIO_PIN_SET);
				ErrorCodeSet(0);
			}
		}
		break;

		case RESET_USB:
		if ((HAL_GetTick() - user_interface.inactivity_time >= USB_RESET_TIMEOUT))
		{
			UsbEnable();
			user_interface.cmd_manager = CHECK_USB;
			user_interface.usb_reset_num++;
		}
		break;

		case NO_DATA_TO_PROCESS:
		if(user_interface.inactivity_time == 0)
			user_interface.inactivity_time = HAL_GetTick();
		if (data_avail(USBVCP))
		{
			user_interface.time_app = HAL_GetTick();
			user_interface.cmd_manager = WAIT_FOR_DATA;
		}
		else
			user_interface.cmd_manager = CHECK_FOR_INACTIVITY;
		break;

		case CHECK_FOR_INACTIVITY:
		if(user_interface.connection_status ==  CONNECTED)
		{
			if ((HAL_GetTick() - user_interface.inactivity_time >= INACTIVITY_TIMEOUT))
			{
				user_interface.inactivity_time = 0;
				PrintStatus();
			}
		}
		user_interface.cmd_manager = NO_DATA_TO_PROCESS;
		break;

		case WAIT_FOR_DATA:
		if ((HAL_GetTick() - user_interface.time_app) >= MAN_TIMEOUT)
			user_interface.cmd_manager = RESTART;
		else
		{
			if (data_avail(USBVCP))
				user_interface.cmd_manager = DECODE_MESSAGE;
			else
				user_interface.cmd_manager = RESTART;
		}
		break;

		case DECODE_MESSAGE:
		{
			user_interface.user_cmd = Decode( data_avail(USBVCP), user_interface.user_data, &user_interface.user_data_len);
			Application(&user_interface.user_cmd, user_interface.user_data, user_interface.user_data_len);
			user_interface.cmd_manager = RESTART;
			break;
		}

		case RESTART:
		clear_buff(USBVCP);
		user_interface.inactivity_time = 0;
		user_interface.cmd_manager = NO_DATA_TO_PROCESS;
		break;

		case USB_FAIL:
		ErrorCodeSet(1);
		break;
	}
}

/****************************************************************************
Function:			static void Printstatus(void)
Input:				none
Output:				none
PreCondition:		none
Overview:			Print some information about the device
****************************************************************************/
static void PrintStatus(void)
{
	uint8_t buff[10]={0,0,0,0,0,0,0,0,0,0};
	uint8_t i;
	uint8_t len;

	uptime_time_t time = UptimeGet();
	if ( (time.min == 0) && (time.hours == 0) && (time.days == 0) )
		return;
	UsbPrintString(ready, PRINT_ONLY);
	UsbPrintString("- Device uptime < ", PRINT_ONLY);
	/* DAYS */
	if(time.days)
	{
		len = Uint32_ToPrintableString( (uint32_t)time.days, buff);
		for(i=len; i>0; i--)
			putbyte(USBVCP, buff[i-1]);
		UsbPrintString("d ", PRINT_ONLY);
	}
	/* HOURS */
	if(time.hours)
	{
		len = Uint32_ToPrintableString( (uint32_t)time.hours, buff);
		for(i=len; i>0; i--)
			putbyte(USBVCP, buff[i-1]);
		UsbPrintString("h ", PRINT_ONLY);
	}
	/* MIN */
	if(time.min)
	{
		len = Uint32_ToPrintableString( (uint32_t)time.min, buff);
		for(i=len; i>0; i--)
			putbyte(USBVCP, buff[i-1]);
		UsbPrintString("m ", PRINT_ONLY);
	}
	UsbPrintString("> ", PRINT_ONLY);
	putbyte(USBVCP, CR_);
}
