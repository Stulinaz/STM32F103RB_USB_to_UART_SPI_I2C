#ifndef INC_SPI_H_
#define INC_SPI_H_

#include "main.h"
#include "command_list.h"

typedef enum{
	SPI_NSS_ACTIVE_LOW = 0u,
	SPI_NSS_ACTIVE_HIGH
}SpiNss_t;

typedef enum{
	SPI_CONFIGURATION_OK = 0u,
	SPI_PARAMETER_ERROR
}SpiConf_t;

typedef enum{
	SPI_SEND_DATA = 0u,
	SPI_READ_DATA
}SPiDataHandler_t;

typedef enum{
	SPI_USER_BAUDRATEPRESCALER_2 = 0u,
	SPI_USER_BAUDRATEPRESCALER_4,
	SPI_USER_BAUDRATEPRESCALER_8,
	SPI_USER_BAUDRATEPRESCALER_16,
	SPI_USER_BAUDRATEPRESCALER_32,
	SPI_USER_BAUDRATEPRESCALER_64,
	SPI_USER_BAUDRATEPRESCALER_128,
	SPI_USER_BAUDRATEPRESCALER_256
}SpiPrescaler_t;

typedef enum{
	SPI_NO_ERROR = 0u,
	SPI_BUFFLEN_ERROR,
	SPI_ENABLE_ERROR,
	SPI_FAULT_ERROR,
	SPI_UNDERRUN_ERROR,
	SPI_BUSY_ERROR
}SpiError_t;

SpiConf_t SpiUserConf(command_t spi_cmd, uint8_t param);
void SPIInitDefault(void);
void SPIStart(void);
SpiError_t SpiSend(void);

#endif
