#include "gpio.h"

void GPIO_Init(void)
{
	  GPIO_InitTypeDef GPIO_InitStruct = {0};

	  /* GPIO Ports Clock Enable */
	  __HAL_RCC_GPIOD_CLK_ENABLE();
	  __HAL_RCC_GPIOA_CLK_ENABLE();
	  __HAL_RCC_GPIOB_CLK_ENABLE();
	  __HAL_RCC_GPIOC_CLK_ENABLE();

	  /*Configure GPIO pin Output Level */
	  HAL_GPIO_WritePin(GPIOB, SERIAL_LED_Pin|SPI_LED_Pin|I2C_LED_Pin|STANDBY_LED_Pin, GPIO_PIN_RESET);

	  HAL_GPIO_WritePin(GPIOB, USB_LED_Pin, GPIO_PIN_RESET);

	  /*Configure GPIO pin Output Level */
	  HAL_GPIO_WritePin(GPIOC, TX_LED_Pin|RX_LED_Pin|ERROR_LED_Pin|TIMEOUT_LED_Pin
	                          |DRIVER_CH1_Pin|DRIVER_CH2_Pin, GPIO_PIN_RESET);

	  /*Configure GPIO pins : SERIAL_Pin SPI_Pin I2C_Pin USB_Pin
	                           STANDBY_Pin */
	  GPIO_InitStruct.Pin = SERIAL_LED_Pin| SPI_LED_Pin| I2C_LED_Pin| USB_LED_Pin
	                          |STANDBY_LED_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	  /*Configure GPIO pins : TX_Pin RX_Pin ERROR_Pin TIMEOUT_Pin
	                           DRIVER_CH1_Pin DRIVER_CH2_Pin */
	  GPIO_InitStruct.Pin = TX_LED_Pin |RX_LED_Pin|ERROR_LED_Pin|TIMEOUT_LED_Pin
	                          |DRIVER_CH1_Pin|DRIVER_CH2_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void USB_Set_System(void)
{
	  //TODO si può abilitare il pull up da qui?
	  GPIO_InitTypeDef  GPIO_InitStructure;
	  /*USB DATA - */
	  GPIO_InitStructure.Pin   = GPIO_PIN_11;
	  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	  GPIO_InitStructure.Mode  = GPIO_MODE_AF_PP;
	  GPIO_InitStructure.Pull  = GPIO_NOPULL;
	  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

	  /* USB DATA + */
	  GPIO_InitStructure.Pin   = GPIO_PIN_12;
	  GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	  GPIO_InitStructure.Mode  = GPIO_MODE_AF_PP;
	  GPIO_InitStructure.Pull  = GPIO_PULLUP;
	  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
	  HAL_NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
}

/****************************************************************************
Function:			SerialPinConfig(void)
Input:				/
Output:				Configure USART pins
PreCondition:		/
Overview:			/
****************************************************************************/
void SerialPinConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/****************************************************************************
Function:			I2cPinConfig(void)
Input:				/
Output:				Configure I2C pins
PreCondition:		/
Overview:			/
****************************************************************************/
void I2cPinConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin   = IIC_SDA_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    HAL_GPIO_Init(IIC_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin   = IIC_SCL_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    HAL_GPIO_Init(IIC_PORT, &GPIO_InitStruct);
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
    I2cPinConfig();
}

/****************************************************************************
Function:			SpiPinConfig(void)
Input:				/
Output:				Configure SPI pins
PreCondition:		/
Overview:			/
****************************************************************************/
void SpiPinConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	HAL_GPIO_WritePin(SPI_PORT, SPI_NSS_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SPI_PORT, SPI_MOSI_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SPI_PORT, SPI_SCK_PIN, GPIO_PIN_SET);

    GPIO_InitStruct.Pin   = SPI_NSS_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    HAL_GPIO_Init(SPI_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin   = SPI_MISO_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_AF_INPUT;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    HAL_GPIO_Init(SPI_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin   = SPI_MOSI_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    HAL_GPIO_Init(SPI_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin   = SPI_SCK_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    HAL_GPIO_Init(SPI_PORT, &GPIO_InitStruct);
}
