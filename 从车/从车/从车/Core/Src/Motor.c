#include "tim.h"
#include "main.h"

void Motor_Init()
{
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);
}

void Moto_forward_run()	//控制两个电机正转
{
	HAL_GPIO_WritePin(Motor1_1_GPIO_Port,Motor1_1_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(Motor1_2_GPIO_Port,Motor1_2_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Motor2_1_GPIO_Port,Motor2_1_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(Motor2_2_GPIO_Port,Motor2_2_Pin,GPIO_PIN_RESET);
}

void Moto_back_run()
{
	HAL_GPIO_WritePin(Motor1_1_GPIO_Port,Motor1_1_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Motor1_2_GPIO_Port,Motor1_2_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(Motor2_1_GPIO_Port,Motor2_1_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Motor2_2_GPIO_Port,Motor2_2_Pin,GPIO_PIN_SET);
}

void Moto_stop()
{
	HAL_GPIO_WritePin(Motor1_1_GPIO_Port,Motor1_1_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Motor1_2_GPIO_Port,Motor1_2_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Motor2_1_GPIO_Port,Motor2_1_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Motor2_2_GPIO_Port,Motor2_2_Pin,GPIO_PIN_RESET);
}

void Moto_turn_right()
{
		HAL_GPIO_WritePin(Motor1_1_GPIO_Port,Motor1_1_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(Motor1_2_GPIO_Port,Motor1_2_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Motor2_1_GPIO_Port,Motor2_1_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Motor2_2_GPIO_Port,Motor2_2_Pin,GPIO_PIN_SET);

}


void Moto_turn_left()
{
	HAL_GPIO_WritePin(Motor1_1_GPIO_Port,Motor1_1_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Motor1_2_GPIO_Port,Motor1_2_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(Motor2_1_GPIO_Port,Motor2_1_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(Motor2_2_GPIO_Port,Motor2_2_Pin,GPIO_PIN_RESET);
}

void SetSpeed_L(int16_t Speed)
{
	__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_3,Speed);
}
	
void SetSpeed_R(int16_t Speed)
{
	__HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_4,Speed);

}

