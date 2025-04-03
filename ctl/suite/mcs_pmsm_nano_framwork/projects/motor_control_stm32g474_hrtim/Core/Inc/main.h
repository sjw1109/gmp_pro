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
#include "stm32g4xx_hal.h"

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

void HAL_HRTIM_MspPostInit(HRTIM_HandleTypeDef *hhrtim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define USER_BTN_Pin GPIO_PIN_13
#define USER_BTN_GPIO_Port GPIOC
#define MOTOR_IA_Pin GPIO_PIN_0
#define MOTOR_IA_GPIO_Port GPIOA
#define MOTOR_VA_Pin GPIO_PIN_1
#define MOTOR_VA_GPIO_Port GPIOA
#define USER_LED_Pin GPIO_PIN_5
#define USER_LED_GPIO_Port GPIOA
#define MOTOR_IB_Pin GPIO_PIN_6
#define MOTOR_IB_GPIO_Port GPIOA
#define MOTOR_VB_Pin GPIO_PIN_7
#define MOTOR_VB_GPIO_Port GPIOA
#define MOTOR_IC_Pin GPIO_PIN_0
#define MOTOR_IC_GPIO_Port GPIOB
#define MOTOR_VC_Pin GPIO_PIN_1
#define MOTOR_VC_GPIO_Port GPIOB
#define MOTOR_VBUS_Pin GPIO_PIN_2
#define MOTOR_VBUS_GPIO_Port GPIOB
#define SPI_ENCODER_CS_Pin GPIO_PIN_10
#define SPI_ENCODER_CS_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
