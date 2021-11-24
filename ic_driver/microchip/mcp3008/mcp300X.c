#include "stm32f1xx_hal.h"
#include "spi.h"
#include "gpio.h"
#include "mcp300X.h"

SPI_HandleTypeDef mcp300x_hspi;

static void Mcp300X_SPIInit(void);
static SpiError_t Mcp300XSpiSend(uint8_t *tx_buff, uint8_t *rx_buff, uint8_t tx_len);
static SpiError_t Mcp300XSpiCheckErrors(void);

static void Mcp300X_SPIInit(void)
{
	/* Enable clock */
	RCC->APB2ENR |= 0x00001000;
	mcp300x_hspi.Instance               = SPI1;
	mcp300x_hspi.Init.Mode              = SPI_MODE_MASTER;
	mcp300x_hspi.Init.Direction         = SPI_DIRECTION_2LINES;
	mcp300x_hspi.Init.DataSize          = SPI_DATASIZE_8BIT;
	mcp300x_hspi.Init.CLKPolarity       = SPI_POLARITY_LOW;
	mcp300x_hspi.Init.CLKPhase          = SPI_PHASE_1EDGE;
	mcp300x_hspi.Init.NSS               = SPI_NSS_SOFT;
	mcp300x_hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
	mcp300x_hspi.Init.FirstBit          = SPI_FIRSTBIT_MSB;
	mcp300x_hspi.Init.TIMode            = SPI_TIMODE_DISABLE;
	mcp300x_hspi.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
	mcp300x_hspi.Init.CRCPolynomial     = 10;
	/* Configure gpio*/
	SpiPinConfig(COMMUNICATION_MODE);
	HAL_GPIO_WritePin(SPI_PORT, SPI_NSS_PIN, GPIO_PIN_SET);
	HAL_SPI_Init(&mcp300x_hspi);
	SPI1->CR1 |= SPI_CR1_SPE;
}

static SpiError_t Mcp300XSpiSend(uint8_t *tx_buff, uint8_t *rx_buff, uint8_t tx_len)
{
	uint8_t trasmitted_byte = 0;
	uint8_t received_byte   = 0;
	uint8_t i;
	SPiDataHandler_t handler    = SPI_SEND_DATA;
	SpiError_t stat = MCP300XSpiCheckErrors();

	if(stat)
		return stat;

	HAL_GPIO_WritePin(SPI_PORT, SPI_NSS_PIN, GPIO_PIN_RESET);

	for(i=0; i<10; i++)
		;

	while(1)
	{
		switch(handler)
		{
			case SPI_SEND_DATA:
			if( (SPI1->SR & SPI_SR_TXE) != 0)
			{
				SPI1->DR = ((uint16_t) tx_buff[trasmitted_byte]) & 0x00FF;
				trasmitted_byte++;
				handler = SPI_READ_DATA;
			}
			break;

			case SPI_READ_DATA:
			if( (SPI1->SR & SPI_SR_RXNE) != 0)
			{
				rx_buff[received_byte] = (uint8_t) SPI1->DR;
				received_byte++;
				handler = SPI_SEND_DATA;
			}
			break;
		}
		if(received_byte >= tx_len)
			break;
	}

	HAL_GPIO_WritePin(SPI_PORT, SPI_NSS_PIN, GPIO_PIN_SET);

	return SPI_NO_ERROR;
}

static SpiError_t Mcp300XSpiCheckErrors(void)
{
	if( (SPI1->CR1 & SPI_CR1_SPE) == 0)
		return SPI_ENABLE_ERROR;
	if( (SPI1->SR & SPI_SR_MODF) != 0)
		return SPI_FAULT_ERROR;
	if( (SPI1->SR & SPI_SR_UDR) != 0)
		return SPI_UNDERRUN_ERROR;
	if( (SPI1->SR & SPI_SR_BSY) != 0)
		return SPI_BUSY_ERROR;
	return SPI_NO_ERROR;
}
