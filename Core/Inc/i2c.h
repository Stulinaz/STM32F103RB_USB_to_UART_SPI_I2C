#ifndef INC_I2C_H_
#define INC_I2C_H_

#include "main.h"

#define I2C_QUEUE_COMMAND_DIM 20
#define MASTER_TRANSMITTER    0x00
#define MASTER_RECEIVER       0x01
#define I2C_OFFSET_TRANSF     0x01

typedef enum{
NO_COMMAND_SELECTED = 0u,
BYTE_WRITE,
BYTE_READ,
ADDRESS_TRANSFER,
REPEAT_START,
ACK_EXPECTED,
NACK_EXPECTED,
}i2c_queue_t;

_bool I2cEnqeue( uint8_t data, i2c_queue_t param);
_bool I2cGetqueue( uint8_t *const data, i2c_queue_t *const param, uint8_t queue_pos);
uint8_t I2cTransfer(void);
void DBG_i2c(void);
void I2cSetSlaveAddress(uint8_t addr);
void I2cQueueRelease(void);
#endif
