/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern PCD_HandleTypeDef hpcd_USB_FS;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define SERIAL_LED_Pin GPIO_PIN_1
#define SERIAL_LED_GPIO_Port GPIOB
#define SPI_LED_Pin GPIO_PIN_2
#define SPI_LED_GPIO_Port GPIOB
#define I2C_LED_Pin GPIO_PIN_10
#define I2C_LED_GPIO_Port GPIOB
#define USB_LED_Pin GPIO_PIN_11
#define USB_LED_GPIO_Port GPIOB
#define STANDBY_LED_Pin GPIO_PIN_15
#define STANDBY_LED_GPIO_Port GPIOB
#define TX_LED_Pin GPIO_PIN_6
#define TX_LED_GPIO_Port GPIOC
#define RX_LED_Pin GPIO_PIN_7
#define RX_LED_GPIO_Port GPIOC
#define ERROR_LED_Pin GPIO_PIN_8
#define ERROR_LED_GPIO_Port GPIOC
#define TIMEOUT_LED_Pin GPIO_PIN_9
#define TIMEOUT_LED_GPIO_Port GPIOC
#define USB_ENABLE_Pin GPIO_PIN_10
#define USB_ENABLE_GPIO_Port GPIOA
#define DRIVER_CH1_Pin GPIO_PIN_10
#define DRIVER_CH1_GPIO_Port GPIOC
#define DRIVER_CH2_Pin GPIO_PIN_11
#define DRIVER_CH2_GPIO_Port GPIOC
void   MX_GPIO_Init(void);
void   MX_I2C1_Init(void);
void   MX_SPI1_Init(void);
void   MX_USART2_UART_Init(void);
void   MX_ADC1_Init(void);
/* USER CODE BEGIN Private defines */
#define UART_TX_PIN GPIO_PIN_2
#define UART_TX_PORT GPIOA

#define UART_RX_PIN GPIO_PIN_3
#define UART_RX_PORT GPIOA

#define UART_CTS_PIN GPIO_PIN_0
#define UART_CTS_PORT GPIOA

#define UART_RTS_PIN GPIO_PIN_1
#define UART_RTS_PORT GPIOA

#define SPI_PORT GPIOA
#define SPI_NSS_PIN GPIO_PIN_4
#define SPI_MISO_PIN GPIO_PIN_6
#define SPI_MOSI_PIN GPIO_PIN_7
#define SPI_SCK_PIN GPIO_PIN_5
#define IIC_PORT GPIOB
#define IIC_SDA_PIN GPIO_PIN_7
#define IIC_SCL_PIN GPIO_PIN_6
#define NUL 0
#define TIMEOUT 2
#define PASS    1
#define FAIL    0

typedef struct{
	uint16_t tx_buff_write_index;
	uint16_t tx_buff_read_index;
	uint16_t rx_buff_write_index;
	uint16_t rx_buff_read_index;
	uint16_t tx_max_buff_dim;
	uint16_t rx_max_buff_dim;
}comm_index_t;

typedef enum{
	FALSE = 0,
	TRUE = 1
}_bool;

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
