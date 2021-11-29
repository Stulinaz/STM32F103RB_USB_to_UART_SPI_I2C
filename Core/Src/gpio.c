#include "sys_definitions.h"
#include "gpio.h"

void GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	/*Configure GPIO pin Output Level for leds*/
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

	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

	/*Configure GPIO pin: USB_ENABLE*/
	GPIO_InitStruct.Pin   = USB_ENABLE_Pin;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_NOPULL;
	HAL_GPIO_Init(USB_ENABLE_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : SERIAL_Pin SPI_Pin I2C_Pin USB_Pin STANDBY_Pin */
	GPIO_InitStruct.Pin = SERIAL_LED_Pin| SPI_LED_Pin| I2C_LED_Pin| USB_LED_Pin|STANDBY_LED_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : TX_Pin RX_Pin ERROR_Pin TIMEOUT_Pin DRIVER_CH1_Pin DRIVER_CH2_Pin */
	GPIO_InitStruct.Pin = TX_LED_Pin |RX_LED_Pin|ERROR_LED_Pin|TIMEOUT_LED_Pin|DRIVER_CH1_Pin|DRIVER_CH2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	SerialPinConfig(INPUT_MODE);
	I2cPinConfig(INPUT_MODE);
	SpiPinConfig(INPUT_MODE);
}

/****************************************************************************
Function:			SerialPinConfig(void)
Input:				mode = INPUT_MODE  or  mode = COMMUNICATION_MODE
Output:				Configure USART pins
PreCondition:		/
Overview:			Configure UART pins for data trasnfer(COMMUNICATION_MODE) or imput mode(INPUT_MODE)
****************************************************************************/
void SerialPinConfig(DataPInConfig_t mode)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if(mode == INPUT_MODE)
	{
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;

		GPIO_InitStruct.Pin  = UART_CTS_PIN;
		HAL_GPIO_Init(UART_CTS_PORT, &GPIO_InitStruct);

		GPIO_InitStruct.Pin  = UART_RTS_PIN;
		HAL_GPIO_Init(UART_RTS_PORT, &GPIO_InitStruct);

		GPIO_InitStruct.Pin  = UART_TX_PIN;
		HAL_GPIO_Init(UART_TX_PORT, &GPIO_InitStruct);

		GPIO_InitStruct.Pin  = UART_RX_PIN;
		HAL_GPIO_Init(UART_RX_PORT, &GPIO_InitStruct);
	}
	else if(mode == COMMUNICATION_MODE)
	{
		HAL_GPIO_WritePin(UART_RTS_PORT, UART_RTS_PIN, GPIO_PIN_RESET);
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

		GPIO_InitStruct.Pin   = UART_CTS_PIN;
		GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull  = GPIO_PULLUP;
		HAL_GPIO_Init(UART_CTS_PORT, &GPIO_InitStruct);

		GPIO_InitStruct.Pin   = UART_RTS_PIN;
		GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull  = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin   = UART_TX_PIN;
		GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull  = GPIO_NOPULL;
		HAL_GPIO_Init(UART_TX_PORT, &GPIO_InitStruct);

		GPIO_InitStruct.Pin  = UART_RX_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		HAL_GPIO_Init(UART_RX_PORT, &GPIO_InitStruct);
	}
}

/****************************************************************************
Function:			void I2cPinConfig(DataPInConfig_t mode)
Input:				mode = INPUT_MODE  or  mode = COMMUNICATION_MODE
Output:				Configure IIC pins
PreCondition:		/
Overview:			Configure IIC pins for data trasnfer(COMMUNICATION_MODE) or imput mode(INPUT_MODE)
****************************************************************************/
void I2cPinConfig(DataPInConfig_t mode)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if(mode == INPUT_MODE)
	{
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;

		GPIO_InitStruct.Pin  = IIC_SDA_PIN;
		HAL_GPIO_Init(IIC_PORT, &GPIO_InitStruct);

		GPIO_InitStruct.Pin  = IIC_SCL_PIN;
		HAL_GPIO_Init(IIC_PORT, &GPIO_InitStruct);
	}
	else if (mode == COMMUNICATION_MODE)
	{
	    GPIO_InitStruct.Pin   = IIC_SDA_PIN;
	    GPIO_InitStruct.Mode  = GPIO_MODE_AF_OD;
	    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
#ifdef IIC_PULLUP_EXTERNAL
	    GPIO_InitStruct.Pull  = GPIO_NOPULL;
#else
	    GPIO_InitStruct.Pull  = GPIO_PULLUP;
#endif
	    HAL_GPIO_Init(IIC_PORT, &GPIO_InitStruct);

	    GPIO_InitStruct.Pin   = IIC_SCL_PIN;
	    GPIO_InitStruct.Mode  = GPIO_MODE_AF_OD;
	    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
#ifdef IIC_PULLUP_EXTERNAL
	    GPIO_InitStruct.Pull  = GPIO_NOPULL;
#else
	    GPIO_InitStruct.Pull  = GPIO_PULLUP;
#endif
	    HAL_GPIO_Init(IIC_PORT, &GPIO_InitStruct);
	}
}

/****************************************************************************
Function:			I2cSdaRelease(void)
Input:				/
Output:				generate 8 clock pulse on SCL pin to release SDA
PreCondition:		/
Overview:			Could happen that slave device become stuck holding SDA low
****************************************************************************/
void I2cSdaRelease(void)
{
	uint8_t i;
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	HAL_GPIO_WritePin(IIC_PORT, IIC_SCL_PIN, GPIO_PIN_SET);

    GPIO_InitStruct.Pin   = IIC_SCL_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    HAL_GPIO_Init(IIC_PORT, &GPIO_InitStruct);

    for(i=0;i<8;i++)
    {
    	HAL_GPIO_WritePin(IIC_PORT, IIC_SCL_PIN, GPIO_PIN_SET);
    	HAL_GPIO_WritePin(IIC_PORT, IIC_SCL_PIN, GPIO_PIN_RESET);
    }
    I2cPinConfig(COMMUNICATION_MODE);
}

/****************************************************************************
Function:			void SpiPinConfig(DataPInConfig_t mode)
Input:				mode = INPUT_MODE  or  mode = COMMUNICATION_MODE
Output:				Configure SPI pins
PreCondition:		/
Overview:			Configure SPI pins for data trasnfer(COMMUNICATION_MODE) or imput mode(INPUT_MODE)
****************************************************************************/
void SpiPinConfig(DataPInConfig_t mode)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if(mode == INPUT_MODE)
	{
		GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull  = GPIO_NOPULL;

		GPIO_InitStruct.Pin   = SPI_NSS_PIN;
		HAL_GPIO_Init(SPI_PORT, &GPIO_InitStruct);

		GPIO_InitStruct.Pin   = SPI_MISO_PIN;
		HAL_GPIO_Init(SPI_PORT, &GPIO_InitStruct);

		GPIO_InitStruct.Pin   = SPI_MOSI_PIN;
		HAL_GPIO_Init(SPI_PORT, &GPIO_InitStruct);

		GPIO_InitStruct.Pin   = SPI_SCK_PIN;
		HAL_GPIO_Init(SPI_PORT, &GPIO_InitStruct);
	}
	else if (mode == COMMUNICATION_MODE)
	{
		HAL_GPIO_WritePin(SPI_PORT, SPI_NSS_PIN, GPIO_PIN_SET);

		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

		GPIO_InitStruct.Pin   = SPI_NSS_PIN;
		GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull  = GPIO_NOPULL;
		HAL_GPIO_Init(SPI_PORT, &GPIO_InitStruct);

		GPIO_InitStruct.Pin   = SPI_MISO_PIN;
		GPIO_InitStruct.Mode  = GPIO_MODE_AF_INPUT;
		GPIO_InitStruct.Pull  = GPIO_NOPULL;
		HAL_GPIO_Init(SPI_PORT, &GPIO_InitStruct);

		GPIO_InitStruct.Pin   = SPI_MOSI_PIN;
		GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull  = GPIO_NOPULL;
		HAL_GPIO_Init(SPI_PORT, &GPIO_InitStruct);

		GPIO_InitStruct.Pin   = SPI_SCK_PIN;
		GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull  = GPIO_NOPULL;
		HAL_GPIO_Init(SPI_PORT, &GPIO_InitStruct);
	}
}

/****************************************************************************
Function:			void UsbEnable(void)
Input:				/
Output:				/
PreCondition:		/
Overview:			Enable pull up on DATA+ ( 1.5k resistor)
****************************************************************************/
void UsbEnable(void)
{
	/*AS SOON AS THE USB IS ENABLED, USB_DM AND USB_DP ARE AUTOMATICALLY CONNECTED TO THE USB INTERNAL TRASCEIVER*/
	/* EXTERNAL PULL UP IS NEEDED */
	HAL_GPIO_WritePin(USB_ENABLE_GPIO_Port,   USB_ENABLE_Pin, GPIO_PIN_SET);
}

/****************************************************************************
Function:			void UsbDisable(void)
Input:				/
Output:				/
PreCondition:		/
Overview:			Disable pull up on DATA+ ( 1.5k resistor)
****************************************************************************/
void UsbDisable(void)
{
	HAL_GPIO_WritePin(USB_ENABLE_GPIO_Port,   USB_ENABLE_Pin, GPIO_PIN_SET);
}
