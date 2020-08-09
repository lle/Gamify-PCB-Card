/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */


#include <stdio.h>
#include <stdlib.h>


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
#define TSC_UP_Pin GPIO_PIN_1
#define TSC_UP_GPIO_Port GPIOA
#define TSC_DN_Pin GPIO_PIN_2
#define TSC_DN_GPIO_Port GPIOA
#define TSC_L_Pin GPIO_PIN_3
#define TSC_L_GPIO_Port GPIOA
#define TSC_R_Pin GPIO_PIN_5
#define TSC_R_GPIO_Port GPIOA
#define TSC_A_Pin GPIO_PIN_6
#define TSC_A_GPIO_Port GPIOA
#define TSC_B_Pin GPIO_PIN_7
#define TSC_B_GPIO_Port GPIOA
#define LED_P_0_Pin GPIO_PIN_0
#define LED_P_0_GPIO_Port GPIOB
#define LED_P_1_Pin GPIO_PIN_1
#define LED_P_1_GPIO_Port GPIOB
#define LED_P_2_Pin GPIO_PIN_2
#define LED_P_2_GPIO_Port GPIOB
#define LED_N_2_Pin GPIO_PIN_10
#define LED_N_2_GPIO_Port GPIOB
#define LED_N_3_Pin GPIO_PIN_11
#define LED_N_3_GPIO_Port GPIOB
#define LED_N_4_Pin GPIO_PIN_12
#define LED_N_4_GPIO_Port GPIOB
#define LED_N_5_Pin GPIO_PIN_13
#define LED_N_5_GPIO_Port GPIOB
#define LED_N_6_Pin GPIO_PIN_14
#define LED_N_6_GPIO_Port GPIOB
#define LED_N_7_Pin GPIO_PIN_15
#define LED_N_7_GPIO_Port GPIOB
#define LED_P_3_Pin GPIO_PIN_3
#define LED_P_3_GPIO_Port GPIOB
#define LED_P_4_Pin GPIO_PIN_4
#define LED_P_4_GPIO_Port GPIOB
#define LED_P_5_Pin GPIO_PIN_5
#define LED_P_5_GPIO_Port GPIOB
#define LED_P_6_Pin GPIO_PIN_6
#define LED_P_6_GPIO_Port GPIOB
#define LED_P_7_Pin GPIO_PIN_7
#define LED_P_7_GPIO_Port GPIOB
#define LED_N_0_Pin GPIO_PIN_8
#define LED_N_0_GPIO_Port GPIOB
#define LED_N_1_Pin GPIO_PIN_9
#define LED_N_1_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
