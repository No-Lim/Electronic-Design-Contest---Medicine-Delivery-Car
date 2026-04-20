/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
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
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern UART_HandleTypeDef huart5;

extern UART_HandleTypeDef huart1;

extern UART_HandleTypeDef huart2;

extern UART_HandleTypeDef huart3;

extern UART_HandleTypeDef huart6;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_UART5_Init(void);
void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);
void MX_USART3_UART_Init(void);
void MX_USART6_UART_Init(void);

/* USER CODE BEGIN Prototypes */
void hhSerialSendByte(uint8_t Byte);
void SendDataToOpenmv(void);
extern void SetTargetRoom(void);
extern uint8_t Openmv_Receive_Buffer[1];
extern uint8_t Bluetooth_Receive_Buffer[1];
void Processing_Data(uint8_t RXdata);
void Write_Data(uint8_t dat);
void Write_Data_1(uint8_t dat);
void Write_Data_2(uint8_t dat);

extern uint8_t have;void SendDataToOpenmv(void);
extern void SetTargetRoom(void);
extern uint8_t Openmv_Receive_Buffer[1];
extern uint8_t Bluetooth_Receive_Buffer[1];


extern uint8_t have;
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

