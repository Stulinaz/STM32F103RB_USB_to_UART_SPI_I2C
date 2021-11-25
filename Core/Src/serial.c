#include "serial.h"
#include "gpio.h"
#include "buffers_manager.h"

UART_HandleTypeDef huart2;

/****************************************************************************
Function:			UartUSerConf(command_t spi_cmd, uint8_t param)
Input:				Command type and parameter
Output:				0 Ok - 1 parameter error
PreCondition:		none
Overview:			Check user parameter for uart configuration
****************************************************************************/
UartConf_t UartUSerConf(command_t spi_cmd, uint8_t param)
{
	UartConf_t stat = UART_PARAMETER_ERROR;
	switch(spi_cmd)
	{
		case USER_SER_SET_BAUDRATE:
		stat = UART_CONFIGURATION_OK;
		if(param == BAUDRATE_1200)
			huart2.Init.BaudRate = 1200;
		else if (param == BAUDRATE_4800)
			huart2.Init.BaudRate = 4800;
		else if (param == BAUDRATE_9600)
			huart2.Init.BaudRate = 9600;
		else if (param == BAUDRATE_19200)
			huart2.Init.BaudRate = 19200;
		else if (param == BAUDRATE_38400)
			huart2.Init.BaudRate = 38400;
		else if (param == BAUDRATE_57600)
			huart2.Init.BaudRate = 57600;
		else if (param == BAUDRATE_115200)
			huart2.Init.BaudRate = 115200;
		else
		{
			/* wrong parameter.. setting to default*/
			huart2.Init.BaudRate = 9600;
			stat = UART_PARAMETER_ERROR;
		}
		break;
		default:
			break;
	}
	return stat;
}

/****************************************************************************
Function:			void UartInitDefault(void)
Input:				none
Output:				none
PreCondition:		none
Overview:			Default parameters for uart
****************************************************************************/
void UartInitDefault(void)
{
	huart2.Instance          = USART2;
	huart2.Init.BaudRate     = SER_BAUDRATE_DEFAULT;
	huart2.Init.WordLength   = UART_WORDLENGTH_8B;
	huart2.Init.StopBits     = UART_STOPBITS_1;
	huart2.Init.Parity       = UART_PARITY_NONE;
	huart2.Init.Mode         = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
}

/****************************************************************************
Function:			UartStart(void)
Input:				none
Output:				none
PreCondition:		none
Overview:			enables clock, configures pin and initialize UART
****************************************************************************/
void UartStart(void)
{
	RCC->APB1ENR |= 0x00020000;
	ser_comm_type.tx_buff_read_index  = 0;
	ser_comm_type.tx_buff_read_index  = 0;
	ser_comm_type.rx_buff_write_index = 0;
	ser_comm_type.tx_buff_read_index  = 0;
	SerialPinConfig(COMMUNICATION_MODE);
	HAL_UART_Init(&huart2);
	__HAL_UART_ENABLE_IT(&huart2,UART_IT_RXNE);
	HAL_NVIC_EnableIRQ(USART2_IRQn);
}

/****************************************************************************
Function:			void UartStop(void)
Input:				none
Output:				none
PreCondition:		none
Overview:			DeInitializes the UART peripheral
****************************************************************************/
void UartStop(void)
{
	HAL_UART_DeInit(&huart2);
	SerialPinConfig(INPUT_MODE);
}

/****************************************************************************
Function:			void USART2_IRQHandler(void)
Input:				none
Output:				none
PreCondition:		none
Overview:			interrupt handler for uart
****************************************************************************/
void USART2_IRQHandler(void)
{
	/*Check Transmit register data empty flag & transmit it enable*/
	if ( ((USART2->SR & USART_SR_TXE) != 0 ) && ((USART2->CR1 & USART_CR1_TXEIE) != 0) )
	{
		USART2->DR = (uint8_t) ser_tx_buff[ser_comm_type.tx_buff_read_index];
		ser_comm_type.tx_buff_read_index++;
		if (ser_comm_type.tx_buff_read_index >= ser_comm_type.tx_buff_write_index)
		{
			HAL_UART_AbortTransmit_IT(&huart2);
			ser_comm_type.tx_buff_read_index  = 0;
			ser_comm_type.tx_buff_write_index = 0;
		}
	}
	else
	{
		/*Check Read data register not empty flag*/
		if((USART2->SR & USART_SR_RXNE) != 0)
		{
			if(ser_comm_type.rx_buff_write_index < (ser_comm_type.rx_max_buff_dim))
			{
				ser_rx_buff[ser_comm_type.rx_buff_write_index] = (uint8_t)(USART2->DR);
				ser_comm_type.rx_buff_write_index++;
			}
			else
				USART2->DR;
		}
	}
}
/****************************************************************************
Function:			void SerStartTransmit(void)
Input:				none
Output:				none
PreCondition:		none
Overview:			Enables uart tx interrupt
****************************************************************************/
void SerStartTransmit(void)
{
	USART2->CR1 |= USART_CR1_TXEIE;
}
