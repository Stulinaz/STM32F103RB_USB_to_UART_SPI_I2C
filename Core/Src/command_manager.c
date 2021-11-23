#include "command_manager.h"
#include "led_manager.h"
#include "buffers_manager.h"
#include "command_process.h"
#include "command_list.h"
#include "usbd_conf.h"
#include "usb_device.h"
#include "usbd_def.h"
#include "app.h"

typedef struct{
command_manager_s cmd_manager_states;
uint8_t           connection_status;
uint32_t          time_app;
uint32_t          inactivity_time;
uint8_t           cmd_len;
command_t         user_cmd;
}interface_t;

interface_t user_interface = { CHECK_USB, DISCONNECTED, 0, 0, 0, NO_COMMAND};

/****************************************************************************
Function:			DecodeFromPc
Input:				none
Output:				none
PreCondition:		USBInit()
Overview:			Handles message from user and application call
****************************************************************************/
void DecodeFromPc (void)
{
	switch(user_interface.cmd_manager_states)
	{
		case CHECK_USB:
		user_interface.time_app = HAL_GetTick();
		if(user_interface.time_app >= CONNECTION_TIMEOUT)
		{
			if(DeviceGetUSBState() != USBD_STATE_CONFIGURED)
			{
				USBStatusLed(GPIO_PIN_RESET);
				ErrorCodeSet(1);
			}
			else
			{
				user_interface.connection_status  = CONNECTED;
				user_interface.cmd_manager_states = NO_DATA_TO_PROCESS;
				USBStatusLed(GPIO_PIN_SET);
				ErrorCodeSet(0);
			}
		}
		break;

		case NO_DATA_TO_PROCESS:
		if(user_interface.inactivity_time == 0)
			user_interface.inactivity_time = HAL_GetTick();
		if (data_avail(USB_INTERFACE))
		{
			user_interface.time_app = HAL_GetTick();
			user_interface.cmd_manager_states = WAIT_FOR_DATA;
		}
		else
			user_interface.cmd_manager_states = CHECK_FOR_INACTIVITY;
		break;

		case CHECK_FOR_INACTIVITY:
		if(user_interface.connection_status ==  CONNECTED)
		{
			if ((HAL_GetTick() - user_interface.inactivity_time >= INACTIVITY_TIMEOUT))
			{
				user_interface.inactivity_time = 0;
				UsbPrintString(ready, TRUE);
			}
		}
		user_interface.cmd_manager_states = NO_DATA_TO_PROCESS;
		break;

		case WAIT_FOR_DATA:
		if ((HAL_GetTick() - user_interface.time_app) >= MAN_TIMEOUT)
			user_interface.cmd_manager_states = RESTART;
		else
		{
			if (data_avail(USB_INTERFACE))
				user_interface.cmd_manager_states = DECODE_MESSAGE;
			else
				user_interface.cmd_manager_states = RESTART;
		}
		break;

		case DECODE_MESSAGE:
		{
		uint8_t conf;
		user_interface.user_cmd = Decode( data_avail(USB_INTERFACE), &conf);
		Application(&user_interface.user_cmd, &conf);
		user_interface.cmd_manager_states = RESTART;
		break;
		}

		case RESTART:
		clear_buff(USB_INTERFACE);
		user_interface.inactivity_time = 0;
		user_interface.cmd_manager_states = NO_DATA_TO_PROCESS;
		break;
	}
}
