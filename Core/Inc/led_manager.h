#ifndef INC_LED_MANAGER_H_
#define INC_LED_MANAGER_H_

#include "main.h"
#include "command_process.h"

#define TX_LED_ON_TIME       (uint32_t) 180
#define RX_LED_ON_TIME       (uint32_t) 180
#define LED_ERROR_BLINK_TIME (uint32_t) 250
#define LED_ERROR_DEAD_TIME  (uint32_t) 1500

void LedManager(void);
void USBStatusLed(GPIO_PinState status);
void LedInterfaceSel(command_t cmd);
void ErrorCodeSet(uint8_t err_num);
void CommunicationLedVisualize(void);
void LedTest(uint32_t time);

#endif
