#ifndef INC_TIM4_1US_TICK_H_
#define INC_TIM4_1US_TICK_H_

#include "stm32f1xx_hal.h"

HAL_StatusTypeDef TIM4Init_us(void);
void TIM4_delay_us(uint16_t time);

#endif
