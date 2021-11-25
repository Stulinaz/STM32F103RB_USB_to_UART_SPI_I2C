#ifndef INC_BUFFERS_MANAGER_H_
#define INC_BUFFERS_MANAGER_H_

#include "main.h"
#include "serial.h"
#include "i2c.h"
#include "usbd_cdc_if.h"

#define SERIAL_TX_BUFF_DIM 100
#define SERIAL_RX_BUFF_DIM 100
#define I2C_TX_BUFF_DIM    0
#define I2C_RX_BUFF_DIM    0
#define SPI_TX_BUFF_DIM    100
#define SPI_RX_BUFF_DIM    100

typedef enum{
	IDLE = 0u,
	UART,
	I2C,
	SPI,
	USBVCP
}comm_inerface_t;

typedef enum{
	PRINT_ONLY = 0u,
	APPEND_CR,
	APPEND_CRCR,
}append_t;

extern volatile comm_index_t ser_comm_type;
extern uint8_t ser_tx_buff[];
extern uint8_t ser_rx_buff[];
extern volatile comm_index_t spi_comm_type;
extern uint8_t spi_tx_buff[];
extern uint8_t spi_rx_buff[];
extern volatile comm_index_t i2c_comm_type;
extern uint8_t i2c_tx_buff[];
extern uint8_t i2c_rx_buff[];

uint8_t putbyte(comm_inerface_t comm_type, uint8_t data);
uint8_t getbyte(comm_inerface_t comm_type, uint8_t * const data);
uint16_t data_avail(comm_inerface_t comm_type);
uint16_t data_toprocess(comm_inerface_t comm_type);
void clear_buff(comm_inerface_t comm_type);
void UsbPrintString(const char *buff, append_t character);
void USB_set_rx_data_len(uint16_t len);
uint16_t SetBuffer(comm_inerface_t comm_type);
void SerialTransfer(void);
void ToUsb(comm_inerface_t comm_type);

#endif
