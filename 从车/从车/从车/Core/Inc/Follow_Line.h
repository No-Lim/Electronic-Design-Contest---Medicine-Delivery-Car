#ifndef __FOLLOW_LINE_H
#define __FOLLOW_LINE_H

#include "main.h"

#define  u8 unsigned char 
#define  u32 unsigned int 

#define READ_FIND_LINE_OUT1 HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_12)
#define READ_FIND_LINE_OUT2 HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6)
#define READ_FIND_LINE_OUT3 HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_4)
#define READ_FIND_LINE_OUT4 HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)
#define READ_FIND_LINE_OUT5 HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_7)
#define READ_FIND_LINE_OUT6 HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_9)
#define READ_FIND_LINE_OUT7 HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_11)
#define READ_FIND_LINE_OUT8 HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_13)



static uint8_t mask_line=0;

//uint8_t get_LedFind_Scan(void);
int Find_Line_Begins(void);
int Position_PID (int Encoder,int Target);
float Get_Track_Errors(uint8_t *Dates);
 void Get_Track_Dates(uint8_t *Dates);


#endif

