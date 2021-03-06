#include "gpio.h"
#include "clock.h"
#include "spi.h"
#include "serial.h"
#include "usb_device.h"
#include "tim4_1us_tick.h"
#include "i2c_eeprom.h"
#include "pcb_revision.h"

/****************************************************************************
Function:			void SYS_Init(void)
Input:				none
Output:				none
PreCondition:		none
Overview:			initialize microcontroller
****************************************************************************/
void SYS_Init(void)
{
	SystemClock_Config();
	GPIO_Init();
	MX_USB_DEVICE_Init();
	UsbEnable();
	SPIInitDefault();
	UartInitDefault();
	TIM4Init_us();
#if (PCB_REV == PCB_REV2)
	EepromIicInit();
#endif
}
