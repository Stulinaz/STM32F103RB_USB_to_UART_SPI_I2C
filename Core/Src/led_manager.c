#include "led_manager.h"
#include "buffers_manager.h"
#include "usbd_conf.h"

typedef enum
{
	ERR_NO_ERROR = 0u,
	ERR_STATUS_ON,
	ERR_STATUS_OFF
}led_error_states;

typedef struct{
	uint32_t led_time;
	uint8_t  err_num;
	uint8_t  curr_cnt;
	led_error_states led_stat;
}led_error_manager;

static void LedErrorManager(void);
static void CommunicationLedSetTime(void);

static led_error_manager display_error = {0, 0, 0, ERR_NO_ERROR};
static comm_inerface_t visualize_transfer = USB_INTERFACE;
static _bool TXRXLedEnable = TRUE;
static uint32_t tx_led_countdown = 0;
static uint32_t rx_led_countdown = 0;

/****************************************************************************
Function:			LedManager
Input:				none
Output:				none
PreCondition:		Timer reference os systick
Overview:			Time reference should tick every 1ms
****************************************************************************/
void LedManager(void)
{
	CommunicationLedSetTime();
	LedErrorManager();
}

/****************************************************************************
Function:			USBStatusLed
Input:				none
Output:				none
PreCondition:
Overview:
****************************************************************************/
void USBStatusLed(GPIO_PinState status)
{
	 HAL_GPIO_WritePin(USB_LED_GPIO_Port, USB_LED_Pin,  status);
}

/****************************************************************************
Function:			LedInterfaceSel
Input:				none
Output:				none
PreCondition:		none
Overview:			visual activation of woring interface (SPI - UART - I2C)
****************************************************************************/
void LedInterfaceSel(command_t cmd)
{
	switch(cmd)
	{
		case USER_SERIAL_INTERFACE_SELECTED:
		{
			visualize_transfer = SER_INTERFACE;
			HAL_GPIO_WritePin(SERIAL_LED_GPIO_Port, SERIAL_LED_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(I2C_LED_GPIO_Port, I2C_LED_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(SPI_LED_GPIO_Port, SPI_LED_Pin, GPIO_PIN_RESET);
			break;
		}
		case USER_I2C_INTERFACE_SELECTED:
		{
			visualize_transfer = I2C_INTERFACE;
			HAL_GPIO_WritePin(SERIAL_LED_GPIO_Port, SERIAL_LED_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(I2C_LED_GPIO_Port, I2C_LED_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(SPI_LED_GPIO_Port, SPI_LED_Pin, GPIO_PIN_RESET);
			break;
		}
		case USER_SPI_INTERFACE_SELECTED:
		{
			visualize_transfer = SPI_INTERFACE;
			HAL_GPIO_WritePin(SERIAL_LED_GPIO_Port, SERIAL_LED_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(I2C_LED_GPIO_Port, I2C_LED_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(SPI_LED_GPIO_Port, SPI_LED_Pin, GPIO_PIN_SET);
			break;
		}
		default:
			break;
	}
}

/****************************************************************************
Function:			CommunicationLedSetTime
Input:				none
Output:				none
PreCondition:		none
Overview:			Visualize on-goin transfer on selected interface (SPI - UART - I2C)
****************************************************************************/
static void CommunicationLedSetTime(void)
{
	switch(visualize_transfer)
	{
		case SER_INTERFACE:
		case SPI_INTERFACE:
		{
			if (data_avail(visualize_transfer) != 0)
				tx_led_countdown = TX_LED_ON_TIME;
			if (data_toprocess(visualize_transfer) != 0)
				tx_led_countdown = RX_LED_ON_TIME;
		}
		break;
		case I2C_INTERFACE:
		default:
			break;
	}
}

/****************************************************************************
Function:			CommunicationLedVisualize
Input:				none
Output:				none
PreCondition:		must be call in gpt interrupt
Overview:			Visualize on-goin transfer on selected interface (SPI - UART - I2C)
****************************************************************************/
void CommunicationLedVisualize(void)
{
	if(!TXRXLedEnable)
		return;
	if ( (tx_led_countdown == 0) && (tx_led_countdown == 0) )
	{
		HAL_GPIO_WritePin(TX_LED_GPIO_Port, TX_LED_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(RX_LED_GPIO_Port, RX_LED_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(STANDBY_LED_GPIO_Port, STANDBY_LED_Pin, GPIO_PIN_SET);
		return;
	}
	else
		HAL_GPIO_WritePin(STANDBY_LED_GPIO_Port, STANDBY_LED_Pin, GPIO_PIN_RESET);

	if(tx_led_countdown > 0)
	{
		tx_led_countdown--;
		HAL_GPIO_WritePin(TX_LED_GPIO_Port, TX_LED_Pin, GPIO_PIN_SET);
	}
	else
		HAL_GPIO_WritePin(TX_LED_GPIO_Port, TX_LED_Pin, GPIO_PIN_RESET);

	if(rx_led_countdown > 0)
	{
		HAL_GPIO_WritePin(RX_LED_GPIO_Port, RX_LED_Pin, GPIO_PIN_SET);
		rx_led_countdown--;
	}
	else
		HAL_GPIO_WritePin(RX_LED_GPIO_Port, RX_LED_Pin, GPIO_PIN_RESET);
}

/****************************************************************************
Function:			ErrorCodeSet
Input:				(uint8_t) Number of blinks to be displayed on EVK's LED Board
Output:				none
PreCondition:		none
Overview:			Debug purpose
****************************************************************************/
void ErrorCodeSet(uint8_t err_num)
{
	display_error.err_num = err_num;
}

/****************************************************************************
Function:			ErrorCodeSet
Input:				(uint8_t) Number of blinks to be displayed on EVK's LED Board
Output:				none
PreCondition:		Timer reference os systick
Overview:			Time reference should tick every 1ms
****************************************************************************/
static void LedErrorManager(void)
{
	switch(display_error.led_stat)
	{
		case ERR_NO_ERROR:
		if(display_error.curr_cnt != 0)
		{
			HAL_GPIO_WritePin(ERROR_LED_GPIO_Port, ERROR_LED_Pin, GPIO_PIN_SET);
			display_error.led_time = HAL_GetTick();
			display_error.led_stat = ERR_STATUS_ON;
		}
		else
		{
			if(display_error.err_num != 0)
				display_error.curr_cnt = display_error.err_num;
		}
		break;

		case ERR_STATUS_ON:
		if ((HAL_GetTick() - display_error.led_time) >= LED_ERROR_BLINK_TIME)
		{
			display_error.led_time = HAL_GetTick();
			HAL_GPIO_WritePin(ERROR_LED_GPIO_Port, ERROR_LED_Pin, GPIO_PIN_RESET);
			display_error.curr_cnt --;
			display_error.led_stat = ERR_STATUS_OFF;
		}
		break;

		case ERR_STATUS_OFF:
		if(display_error.curr_cnt == 0)
		{
			if ((HAL_GetTick() - display_error.led_time) >= LED_ERROR_DEAD_TIME)
				display_error.led_stat = ERR_NO_ERROR;
		}
		else
		{
			if ((HAL_GetTick() - display_error.led_time) >= LED_ERROR_BLINK_TIME)
			{
				display_error.led_time = HAL_GetTick();
				HAL_GPIO_WritePin(ERROR_LED_GPIO_Port, ERROR_LED_Pin, GPIO_PIN_SET);
				display_error.led_stat = ERR_STATUS_ON;
			}
		}
		break;
	}
}

/****************************************************************************
Function:			void LedTest(uint32_t time)
Input:				time to hold on all the onboard leds
Output:				none
PreCondition:		none
Overview:			Test onboard led after pcb mounting phase
****************************************************************************/
void LedTest(uint32_t time)
{
	TXRXLedEnable = FALSE;
	HAL_GPIO_WritePin(USB_ENABLE_GPIO_Port,   USB_ENABLE_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SERIAL_LED_GPIO_Port,   SERIAL_LED_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SPI_LED_GPIO_Port,         SPI_LED_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(I2C_LED_GPIO_Port,         I2C_LED_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(STANDBY_LED_GPIO_Port, STANDBY_LED_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(USB_LED_GPIO_Port,         USB_LED_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(TX_LED_GPIO_Port,           TX_LED_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(RX_LED_GPIO_Port,           RX_LED_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(ERROR_LED_GPIO_Port,     ERROR_LED_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(TIMEOUT_LED_GPIO_Port, TIMEOUT_LED_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(DRIVER_CH1_GPIO_Port,   DRIVER_CH1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(DRIVER_CH2_GPIO_Port,   DRIVER_CH2_Pin, GPIO_PIN_SET);
	HAL_Delay(time);
	HAL_GPIO_WritePin(USB_ENABLE_GPIO_Port,   USB_ENABLE_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SERIAL_LED_GPIO_Port,   SERIAL_LED_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SPI_LED_GPIO_Port,         SPI_LED_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(I2C_LED_GPIO_Port,         I2C_LED_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(STANDBY_LED_GPIO_Port, STANDBY_LED_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(USB_LED_GPIO_Port,         USB_LED_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(TX_LED_GPIO_Port,           TX_LED_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RX_LED_GPIO_Port,           RX_LED_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(ERROR_LED_GPIO_Port,     ERROR_LED_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(TIMEOUT_LED_GPIO_Port, TIMEOUT_LED_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DRIVER_CH1_GPIO_Port,   DRIVER_CH1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DRIVER_CH2_GPIO_Port,   DRIVER_CH2_Pin, GPIO_PIN_RESET);
	TXRXLedEnable = TRUE;
}
