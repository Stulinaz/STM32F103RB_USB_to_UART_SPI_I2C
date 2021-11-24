#ifndef INC_GPIO_H_
#define INC_GPIO_H_

#include "main.h"

typedef enum{
	INPUT_MODE = 0,
	COMMUNICATION_MODE,
}DataPInConfig_t;

void GPIO_Init(void);
void USBPinConfig(void);
void SerialPinConfig(DataPInConfig_t mode);
void I2cPinConfig(DataPInConfig_t mode);
void I2cSdaRelease(void);
void SpiPinConfig(DataPInConfig_t mode);
void UsbEnable(void);
#endif
