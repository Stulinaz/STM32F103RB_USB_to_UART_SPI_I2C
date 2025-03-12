/*******************************************************************************
 * Included files
 *****************************************************************************/
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "led_manager.h"
#include "usb_device.h"
#include "sys_init.h"
#include "app_usb.h"
#include "app.h"
#include "utility.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BLinkTask(void *pvParameters);

/*******************************************************************************
 * Variables
 ******************************************************************************/
TaskHandle_t blink_task_handle;

/*******************************************************************************
 * Code
 ******************************************************************************/
void vApplicationIdleHook(void)
{
	/* User adds here LOW POWER features */
	__WFI();
}

void BLinkTask(void *pvParameters)
{
	const TickType_t xDelay = pdMS_TO_TICKS(100);
	for(;;)
	{
		HAL_GPIO_WritePin(ERROR_LED_GPIO_Port, ERROR_LED_Pin, GPIO_PIN_RESET);
		vTaskDelay(xDelay);
		HAL_GPIO_WritePin(ERROR_LED_GPIO_Port, ERROR_LED_Pin, GPIO_PIN_SET);
		vTaskDelay(xDelay);
	}
	(void)vTaskDelete(NULL);
}

int main(void)
{
  (void)HAL_Init();
  (void)SYS_Init();
  while(1)
  {
	(void)xTaskCreate(BLinkTask,"BLinkTask",100, NULL,  1, &blink_task_handle);
	(void)vTaskStartScheduler();
	for(;;);
  }
}
