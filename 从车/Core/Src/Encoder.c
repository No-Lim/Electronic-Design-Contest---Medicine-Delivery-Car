#include "gpio.h"
#include "tim.h"
#include "motor.h"
#include "pid.h"
#include "oled.h"
#include "Sonar.h"
#include "usart.h"

extern int16_t True_Speed_L,True_Speed_R;
extern int8_t OUTPUT1,OUTPUT2;
extern uint32_t time;
extern uint32_t high_time;

int16_t My_abs(int16_t x);

void Encoder_Init()
{
	MX_TIM3_Init();
	MX_TIM5_Init();
	HAL_TIM_Base_Start_IT(&htim2);//定时中断
	HAL_TIM_Base_Start_IT(&htim9);//定时中断
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_ALL);
	
}

int16_t Speed_L_Get()
{
	int16_t Temp;
	Temp = -__HAL_TIM_GET_COUNTER(&htim3);
	__HAL_TIM_SET_COUNTER(&htim3,0);
	return Temp;
}

int16_t Speed_R_Get()
{
	int16_t Temp;
	Temp = __HAL_TIM_GET_COUNTER(&htim5);
	__HAL_TIM_SET_COUNTER(&htim5,0);
	return Temp;
}

extern __IO uint16_t SendTime;
extern float car_speed_l,car_speed_r;
uint8_t count=0;
uint16_t lukou_straight = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
	{
		
		if(htim == &htim9)
		{
			time++;
		}
		
		
		if(htim==&htim2)
		{
			SendTime++;
		
			if(lukou_straight>=1) lukou_straight+=1;

				True_Speed_L=Speed_L_Get();
				True_Speed_R=Speed_R_Get();
//			OLED_ShowSignedNum(3,3,True_Speed_L,3);
//			OLED_ShowSignedNum(3,8,True_Speed_R,3);
//			OLED_ShowSignedNum(3,3,car_speed_l,3);
//			OLED_ShowSignedNum(3,8,car_speed_r,3);

			OUTPUT1=Position_PI(My_abs(True_Speed_L),My_abs(car_speed_l));
			OUTPUT2=Position_PI(My_abs(True_Speed_R),My_abs(car_speed_r));	
			SetSpeed_L(My_abs(OUTPUT1));
			SetSpeed_R(My_abs(OUTPUT2));
			

		}
	}
	
int16_t My_abs(int16_t x)
	{
		if(x<0)
			return (-x);
		else
			return x;
	}
	


