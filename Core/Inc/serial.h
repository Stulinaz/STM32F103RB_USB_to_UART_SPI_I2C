#ifndef INC_SERIAL_H_
#define INC_SERIAL_H_

#include "main.h"
#include "command_list.h"

#define SER_BAUDRATE_DEFAULT 9600

typedef enum{
	UART_CONFIGURATION_OK = 0u,
	UART_PARAMETER_ERROR
}UartConf_t;

typedef enum{
	BAUDRATE_1200 = 0u,
	BAUDRATE_4800,
	BAUDRATE_9600,
	BAUDRATE_19200,
	BAUDRATE_38400,
	BAUDRATE_57600,
	BAUDRATE_115200
}ser_baudrate_t;

typedef enum{
	SER_NO_ERROR      = 0u,
	SER_OVERRUN_ERROR = 0x01,
	SER_NOISE_ERROR   = 0x02,
	SER_FRAMING_ERROR = 0x04,
	SER_PARITY_ERROR  = 0x08
}ser_error_t;
extern UART_HandleTypeDef huart2;

UartConf_t UartUSerConf(command_t spi_cmd, uint8_t param);
void UartInitDefault(void);
void UartStart(void);
void UartStop(void);
void SerStartTransmit(void);
#endif
