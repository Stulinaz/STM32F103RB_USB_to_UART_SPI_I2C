#include "spi.h"
#include "gpio.h"
#include "buffers_manager.h"

SPI_HandleTypeDef hspi1;
SpiNss_t spi_nss_mode;

static SpiError_t SpiCheckErrors(void);

/****************************************************************************
Function:			SpiUserConf(command_t spi_cmd, uint8_t param)
Input:				Command type and parameter
Output:				0 Ok - 1 parameter error
PreCondition:		none
Overview:			Check user parameter for spi configuration
****************************************************************************/
SpiConf_t SpiUserConf(command_t spi_cmd, uint8_t param)
{
	SpiConf_t stat = SPI_PARAMETER_ERROR;
	switch(spi_cmd)
	{
		case USER_SPI_CLK_POLARITY_LOW:
		hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
		stat = SPI_CONFIGURATION_OK;
		break;

		case USER_SPI_CLK_POLARITY_HIGH:
		hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
		stat = SPI_CONFIGURATION_OK;
		break;

		case USER_SPI_SPEED_SELECTION:
		stat = SPI_CONFIGURATION_OK;
		if(param == SPI_USER_BAUDRATEPRESCALER_2)
			hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;   /* fclk = 6 MHz @ ABP2 clk = 12Mhz */
		else if(param == SPI_USER_BAUDRATEPRESCALER_4)
			hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;   /* fclk = 3 MHz @ ABP2 clk = 12Mhz */
		else if(param == SPI_USER_BAUDRATEPRESCALER_8)
			hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;   /* fclk = 1.5 Mhz @ ABP2 clk = 12Mhz */
		else if(param == SPI_USER_BAUDRATEPRESCALER_16)
			hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;  /* fclk = 750 kHz @ ABP2 clk = 12Mhz */
		else if(param == SPI_USER_BAUDRATEPRESCALER_32)
			hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;  /* fclk = 357 kHz @ ABP2 clk = 12Mhz */
		else if(param == SPI_USER_BAUDRATEPRESCALER_64)
			hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;  /* fclk = 187.500 kHz @ ABP2 clk = 12Mhz */
		else if(param == SPI_USER_BAUDRATEPRESCALER_128)
			hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128; /* fclk = 93.750 kHz @ ABP2 clk = 12Mhz */
		else if(param == SPI_USER_BAUDRATEPRESCALER_256)
			hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256; /* fclk = 46.875 kHz @ ABP2 clk = 12Mhz */
		else
			stat = SPI_PARAMETER_ERROR;
		break;

		case USER_SPI_NSS_ACTIVE_LOW:
		spi_nss_mode = SPI_NSS_ACTIVE_LOW;
		stat = SPI_CONFIGURATION_OK;
		break;

		case USER_SPI_NSS_ACTIVE_HIGH:
		spi_nss_mode = SPI_NSS_ACTIVE_HIGH;
		stat = SPI_CONFIGURATION_OK;
		break;

		default:
		break;
	}
	return stat;
}

/****************************************************************************
Function:			SPIInitDefault(void)
Input:				none
Output:				none
PreCondition:		none
Overview:			Default parameters for spi
****************************************************************************/
void SPIInitDefault(void)
{
	hspi1.Instance               = SPI1;
	hspi1.Init.Mode              = SPI_MODE_MASTER;
	hspi1.Init.Direction         = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize          = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity       = SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase          = SPI_PHASE_1EDGE;
	hspi1.Init.NSS               = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
	hspi1.Init.FirstBit          = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode            = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial     = 10;
	spi_nss_mode                 = SPI_NSS_ACTIVE_LOW;
}

/****************************************************************************
Function:			SPIStart(void)
Input:				none
Output:				none
PreCondition:		none
Overview:			enables clock, configures pin and initialize SPI
****************************************************************************/
void SPIStart(void)
{
	/* Enable clock */
	RCC->APB2ENR |= 0x00001000;
	/* Configure gpio*/
	SpiPinConfig();
	if(spi_nss_mode == SPI_NSS_ACTIVE_LOW)
		HAL_GPIO_WritePin(SPI_PORT, SPI_NSS_PIN, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(SPI_PORT, SPI_NSS_PIN, GPIO_PIN_RESET);
	/* Init Spi */
	HAL_SPI_Init(&hspi1);
	SPI1->CR1 |= SPI_CR1_SPE;
}

/****************************************************************************
Function:			SpiSend(void)
Input:				none
Output:				0 Ok - !=0 error
PreCondition:		none
Overview:			Send data over spi in polling mode
****************************************************************************/
SpiError_t SpiSend(void)
{
	SpiError_t stat             = 0;
	SPiDataHandler_t handler    = SPI_SEND_DATA;

	stat = SpiCheckErrors();
	if(stat)
		return stat;

	if(spi_nss_mode == SPI_NSS_ACTIVE_LOW)
		HAL_GPIO_WritePin(SPI_PORT, SPI_NSS_PIN, GPIO_PIN_RESET);
	else
		HAL_GPIO_WritePin(SPI_PORT, SPI_NSS_PIN, GPIO_PIN_SET);

	while(1)
	{
		switch(handler)
		{
			case SPI_SEND_DATA:
			if( (SPI1->SR & SPI_SR_TXE) != 0)
			{
				SPI1->DR = ((uint16_t) spi_tx_buff[spi_comm_type.tx_buff_read_index]) & 0x00FF;
				spi_comm_type.tx_buff_read_index++;
				handler = SPI_READ_DATA;
			}
			break;

			case SPI_READ_DATA:
			if( (SPI1->SR & SPI_SR_RXNE) != 0)
			{
				spi_rx_buff[spi_comm_type.rx_buff_write_index] = (uint8_t) SPI1->DR;
				spi_comm_type.rx_buff_write_index++;
				handler = SPI_SEND_DATA;
			}
			break;
		}
		if(spi_comm_type.rx_buff_write_index >= spi_comm_type.tx_buff_write_index)
		{
			spi_comm_type.tx_buff_write_index = 0;
			spi_comm_type.tx_buff_read_index = 0;
			break;
		}
	}

	if(spi_nss_mode == SPI_NSS_ACTIVE_LOW)
		HAL_GPIO_WritePin(SPI_PORT, SPI_NSS_PIN, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(SPI_PORT, SPI_NSS_PIN, GPIO_PIN_RESET);

	return SPI_NO_ERROR;
}

/****************************************************************************
Function:			SpiError_t SpiCheckErrors(void)
Input:				none
Output:				0 Ok - !=0 error
PreCondition:		none
Overview:			Check input data len and spi status register
****************************************************************************/
static SpiError_t SpiCheckErrors(void)
{

	if(spi_comm_type.tx_buff_write_index == 0)
		return SPI_BUFFLEN_ERROR;

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
