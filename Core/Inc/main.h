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
#include "stm32f2xx_hal.h"

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
ADC_HandleTypeDef* Get_HADC1_Ptr(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Buzzer_Pin GPIO_PIN_1
#define Buzzer_GPIO_Port GPIOA
#define LeftReceiver_Pin GPIO_PIN_5
#define LeftReceiver_GPIO_Port GPIOA
#define FrontLeftReceiver_Pin GPIO_PIN_6
#define FrontLeftReceiver_GPIO_Port GPIOA
#define FrontRightReceiver_Pin GPIO_PIN_7
#define FrontRightReceiver_GPIO_Port GPIOA
#define RightReceiver_Pin GPIO_PIN_4
#define RightReceiver_GPIO_Port GPIOC
#define MotorCh4_Pin GPIO_PIN_10
#define MotorCh4_GPIO_Port GPIOB
#define MotorCh3_Pin GPIO_PIN_11
#define MotorCh3_GPIO_Port GPIOB
#define LED_1_Pin GPIO_PIN_14
#define LED_1_GPIO_Port GPIOB
#define LED_2_Pin GPIO_PIN_15
#define LED_2_GPIO_Port GPIOB
#define LeftEncoderCh2_Pin GPIO_PIN_6
#define LeftEncoderCh2_GPIO_Port GPIOC
#define LeftEncoderCh1_Pin GPIO_PIN_7
#define LeftEncoderCh1_GPIO_Port GPIOC
#define RightEmitter_Pin GPIO_PIN_10
#define RightEmitter_GPIO_Port GPIOA
#define MotorCh1_Pin GPIO_PIN_15
#define MotorCh1_GPIO_Port GPIOA
#define FrontRightEmitter_Pin GPIO_PIN_11
#define FrontRightEmitter_GPIO_Port GPIOC
#define FrontLeftEmitter_Pin GPIO_PIN_2
#define FrontLeftEmitter_GPIO_Port GPIOD
#define MotorCh2_Pin GPIO_PIN_3
#define MotorCh2_GPIO_Port GPIOB
#define LeftEmitter_Pin GPIO_PIN_4
#define LeftEmitter_GPIO_Port GPIOB
#define RightEncoderCh1_Pin GPIO_PIN_6
#define RightEncoderCh1_GPIO_Port GPIOB
#define RightEncoderCh2_Pin GPIO_PIN_7
#define RightEncoderCh2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
