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
#include "stm32f4xx_hal.h"

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
#define Motor2_1_Pin GPIO_PIN_2
#define Motor2_1_GPIO_Port GPIOE
#define Motor1_1_Pin GPIO_PIN_3
#define Motor1_1_GPIO_Port GPIOE
#define Motor2_2_Pin GPIO_PIN_4
#define Motor2_2_GPIO_Port GPIOE
#define Motor1_2_Pin GPIO_PIN_0
#define Motor1_2_GPIO_Port GPIOC
#define Red_LED_Pin GPIO_PIN_3
#define Red_LED_GPIO_Port GPIOA
#define Green_LED_Pin GPIO_PIN_5
#define Green_LED_GPIO_Port GPIOA
#define HW_1_Pin GPIO_PIN_6
#define HW_1_GPIO_Port GPIOA
#define HW_3_Pin GPIO_PIN_4
#define HW_3_GPIO_Port GPIOC
#define HW_2_Pin GPIO_PIN_0
#define HW_2_GPIO_Port GPIOB
#define User_Led_Pin GPIO_PIN_2
#define User_Led_GPIO_Port GPIOB
#define HW_4_Pin GPIO_PIN_7
#define HW_4_GPIO_Port GPIOE
#define HW_5_Pin GPIO_PIN_9
#define HW_5_GPIO_Port GPIOE
#define HW_6_Pin GPIO_PIN_11
#define HW_6_GPIO_Port GPIOE
#define HW_8_Pin GPIO_PIN_12
#define HW_8_GPIO_Port GPIOE
#define HW_7_Pin GPIO_PIN_13
#define HW_7_GPIO_Port GPIOE
#define PB10_Pin GPIO_PIN_10
#define PB10_GPIO_Port GPIOB
#define PB11_Pin GPIO_PIN_11
#define PB11_GPIO_Port GPIOB
#define Trig_Pin GPIO_PIN_8
#define Trig_GPIO_Port GPIOA
#define Echo_Pin GPIO_PIN_15
#define Echo_GPIO_Port GPIOA
#define AO1_Pin GPIO_PIN_4
#define AO1_GPIO_Port GPIOB
#define AO2_Pin GPIO_PIN_5
#define AO2_GPIO_Port GPIOB
#define PWM_1_Pin GPIO_PIN_8
#define PWM_1_GPIO_Port GPIOB
#define PWM_2_Pin GPIO_PIN_9
#define PWM_2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
extern float car_speed_l,car_speed_r;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
