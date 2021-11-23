#ifndef INC_GPIO_H_
#define INC_GPIO_H_

#include "main.h"

void GPIO_Init(void);
void USB_Set_System(void);
void SerialPinConfig(void);
void I2cPinConfig(void);
void I2cSdaRelease(void);
#endif
