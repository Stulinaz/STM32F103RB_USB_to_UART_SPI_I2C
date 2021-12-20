#include "tim4_1us_tick.h"
#include "gpio.h"

static TIM_HandleTypeDef htim4;
#ifdef TIM4_IT_ENABLED
static uint32_t tim4_us_cnt;
#endif

/****************************************************************************
Function:			HAL_StatusTypeDef TIM4Init_us(void)
Input:				none
Output:				none
PreCondition:		none
Overview:			init tim4 @ fclk = 1Mhz
****************************************************************************/
HAL_StatusTypeDef TIM4Init_us(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  /* TIM4 clock ENABLED */
  RCC->APB1ENR |= 0x00000004;
  htim4.Instance               = TIM4;
  htim4.Init.Prescaler         = 12;
  htim4.Init.CounterMode       = TIM_COUNTERMODE_UP;
  htim4.Init.Period            = 80;
  htim4.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
	  return HAL_ERROR;

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
	  return HAL_ERROR;

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
	  return HAL_ERROR;
#ifdef TIM4_IT_ENABLED
	__HAL_TIM_ENABLE_IT(&htim4, TIM_IT_UPDATE);
	HAL_NVIC_EnableIRQ(TIM4_IRQn );
#endif
  return HAL_OK;
}


/****************************************************************************
Function:			void TIM4_delay_us(uint16_t time)
Input:				time to delay (in us)
Output:				none
PreCondition:		none
Overview:			delay us
****************************************************************************/
void TIM4_delay_us(uint16_t time)
{
	if(!time)
		return;
	/* clear update flag */
	TIM4->SR &= ~TIM_SR_UIF;
	/* reset counter */
	TIM4->CNT = 0;
	TIM4->ARR = time;
	/* counter enable */
	TIM4->CR1 |=TIM_CR1_CEN;
#ifdef TIM4_IT_ENABLED
	while(!tim4_us_cnt);
#else
	while(!(TIM4->SR & TIM_SR_UIF));
#endif
	/* stop counter */
	TIM4->CR1 &= (~TIM_CR1_CEN);
	/* reset counter */
	TIM4->CNT   = 0;
#ifdef TIM4_IT_ENABLED
	tim4_us_cnt = 0;
#endif
	/* clear update flag */
	TIM4->SR &= ~TIM_SR_UIF;
}

void TIM4_IRQHandler(void)
{
#ifdef TIM4_IT_ENABLED
	tim4_us_cnt++;
	/* clear update flag */
	TIM4->SR &= ~TIM_SR_UIF;
#endif
}
