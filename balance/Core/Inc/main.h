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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "pid.h"
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
extern char data_string[80];
extern PID Lspeed;
extern PID Rspeed;
extern PID Angle;
extern PID up;
extern PID turn;

extern float L_target;
extern float R_target;
extern int Key;

typedef enum 
{
	go_on = 0,
	find_black = 1,
	stop = 2,
	control
}car_state;

extern car_state state;

extern float Temp;
extern int Temp2;
extern float cnt;
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define AIN1_Pin GPIO_PIN_4
#define AIN1_GPIO_Port GPIOA
#define AIN2_Pin GPIO_PIN_5
#define AIN2_GPIO_Port GPIOA
#define BIN1_Pin GPIO_PIN_6
#define BIN1_GPIO_Port GPIOA
#define BIN2_Pin GPIO_PIN_7
#define BIN2_GPIO_Port GPIOA
#define L1_Pin GPIO_PIN_4
#define L1_GPIO_Port GPIOC
#define L2_Pin GPIO_PIN_0
#define L2_GPIO_Port GPIOB
#define L3_Pin GPIO_PIN_7
#define L3_GPIO_Port GPIOE
#define L4_Pin GPIO_PIN_9
#define L4_GPIO_Port GPIOE
#define L5_Pin GPIO_PIN_11
#define L5_GPIO_Port GPIOE
#define L6_Pin GPIO_PIN_13
#define L6_GPIO_Port GPIOE
#define L7_Pin GPIO_PIN_15
#define L7_GPIO_Port GPIOE
#define L8_Pin GPIO_PIN_11
#define L8_GPIO_Port GPIOB
#define L9_Pin GPIO_PIN_13
#define L9_GPIO_Port GPIOB
#define L10_Pin GPIO_PIN_15
#define L10_GPIO_Port GPIOB
#define L11_Pin GPIO_PIN_9
#define L11_GPIO_Port GPIOD
#define L12_Pin GPIO_PIN_11
#define L12_GPIO_Port GPIOD
#define LED_Pin GPIO_PIN_15
#define LED_GPIO_Port GPIOA
#define Tigger_Pin GPIO_PIN_11
#define Tigger_GPIO_Port GPIOC

/* USER CODE BEGIN Private defines */
#define LED_ON (HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET))
#define LED_OFF (HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET))

#define IS_L1_DETECTED()    (HAL_GPIO_ReadPin(L1_GPIO_Port, L1_Pin))
#define IS_L2_DETECTED()    (HAL_GPIO_ReadPin(L2_GPIO_Port, L2_Pin))
#define IS_L3_DETECTED()    (HAL_GPIO_ReadPin(L3_GPIO_Port, L3_Pin))
#define IS_L4_DETECTED()    (HAL_GPIO_ReadPin(L4_GPIO_Port, L4_Pin))
#define IS_L5_DETECTED()    (HAL_GPIO_ReadPin(L5_GPIO_Port, L5_Pin))
#define IS_L6_DETECTED()    (HAL_GPIO_ReadPin(L6_GPIO_Port, L6_Pin))

#define IS_L7_DETECTED()    (HAL_GPIO_ReadPin(L7_GPIO_Port, L7_Pin))
#define IS_L8_DETECTED()    (HAL_GPIO_ReadPin(L8_GPIO_Port, L8_Pin))
#define IS_L9_DETECTED()    (HAL_GPIO_ReadPin(L9_GPIO_Port, L9_Pin))
#define IS_L10_DETECTED()   (HAL_GPIO_ReadPin(L10_GPIO_Port, L10_Pin))
#define IS_L11_DETECTED()   (HAL_GPIO_ReadPin(L11_GPIO_Port, L11_Pin))
#define IS_L12_DETECTED()   (HAL_GPIO_ReadPin(L12_GPIO_Port, L12_Pin))

#define GET_LINE_SENSORS() (\
    HAL_GPIO_ReadPin(L2_GPIO_Port, L2_Pin)   || \
    HAL_GPIO_ReadPin(L3_GPIO_Port, L3_Pin)   || \
    HAL_GPIO_ReadPin(L4_GPIO_Port, L4_Pin)   || \
    HAL_GPIO_ReadPin(L5_GPIO_Port, L5_Pin)   || \
    HAL_GPIO_ReadPin(L6_GPIO_Port, L6_Pin)   || \
    HAL_GPIO_ReadPin(L7_GPIO_Port, L7_Pin)   || \
    HAL_GPIO_ReadPin(L8_GPIO_Port, L8_Pin)   || \
    HAL_GPIO_ReadPin(L9_GPIO_Port, L9_Pin)   || \
    HAL_GPIO_ReadPin(L10_GPIO_Port, L10_Pin) || \
    HAL_GPIO_ReadPin(L11_GPIO_Port, L11_Pin))

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
