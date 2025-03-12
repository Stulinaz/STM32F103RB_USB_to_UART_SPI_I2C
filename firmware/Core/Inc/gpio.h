#ifndef INC_GPIO_H_
#define INC_GPIO_H_

/*******************************************************************************
 * Included files
 *****************************************************************************/
#include "main.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef enum{
	INPUT_MODE = 0,
	COMMUNICATION_MODE,
}DataPInConfig_t;

/*******************************************************************************
 * API
 ******************************************************************************/
void GPIO_Init(void);
void USBPinConfig(void);
void SerialPinConfig(DataPInConfig_t mode);
void I2cPinConfig(DataPInConfig_t mode);
void I2cSdaRelease(void);
void SpiPinConfig(DataPInConfig_t mode);
void EepromPinInit(void);
void UsbEnable(void);
void UsbDisable(void);

#endif
