/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

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
#define UART_LEG4_Pin GPIO_PIN_0
#define UART_LEG4_GPIO_Port GPIOA
#define UART_LEG2_Pin GPIO_PIN_2
#define UART_LEG2_GPIO_Port GPIOA
#define UART_LEG3_Pin GPIO_PIN_10
#define UART_LEG3_GPIO_Port GPIOB
#define UART_HEAD_Pin GPIO_PIN_13
#define UART_HEAD_GPIO_Port GPIOB
#define UART_LEG1_Pin GPIO_PIN_14
#define UART_LEG1_GPIO_Port GPIOB
#define Power_out_Pin GPIO_PIN_8
#define Power_out_GPIO_Port GPIOD
#define Power_in_Pin GPIO_PIN_9
#define Power_in_GPIO_Port GPIOD
#define Servo_Power_Pin GPIO_PIN_4
#define Servo_Power_GPIO_Port GPIOD
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
