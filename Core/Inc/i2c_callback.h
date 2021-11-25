#ifndef INC_I2C_CALLBACK_H_
#define INC_I2C_CALLBACK_H_

#include "main.h"

#define IIC_TIMEOUT 10

typedef enum{
	NO_ERROR = 0,
	I2C_NOT_ACTIVATED,
	I2C_TIMEOUT,
	I2C_GENERAL_ERROR,
	I2C_OVERRUN_UNDERRUN,
	I2C_ACKNOWLEDGE_FAILURE,
	I2C_ARBITRATION_LOST,
	I2C_BUS_ERROR,
	I2C_ADDRESS_NACK
}i2c_error_t;

void MX_I2C1_Init(void);
void I2CHardReset(void);
void Call_I2cCheckActivation   (i2c_error_t *const status_transfer);
void Call_I2cGenerateStart     (i2c_error_t *const status_transfer);
void Call_I2cAddressSend       (i2c_error_t *const status_transfer, uint8_t byte);
void Call_I2cSendData          (i2c_error_t *const status_transfer, uint8_t data);
void Call_I2cReadData          (i2c_error_t *const status_transfer, uint8_t *const data);
void Call_I2cSendStopCondition (i2c_error_t *const status_transfer);
void I2cSdaRelease(void);
#endif
