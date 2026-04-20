#include "Sonar.h"
#include "gpio.h"
#include "oled.h"
#include "tim.h"
 
float distant=0;      //测量距离
uint32_t measure_Buf[3] = {0};   //存放定时器计数值的数组
uint8_t  measure_Cnt = 0;    //状态标志位
uint32_t time;
uint32_t high_time;   //超声波模块返回的高电平时间
 
 
//===============================================读取距离
void SR04_GetData(void)
{
  TRIG_H;
  delay_us(15);
	TRIG_L;
	while(HAL_GPIO_ReadPin(Echo_GPIO_Port,Echo_Pin)==0);
	time=0;
	while(HAL_GPIO_ReadPin(Echo_GPIO_Port,Echo_Pin)==1);
	high_time = time;
}
 
 
//===============================================us延时函数
    void delay_us(uint32_t us)//主频72M
{
    uint32_t delay = (HAL_RCC_GetHCLKFreq() / 4000000 * us);
    while (delay--)
	{
		;
	}
}
 
//===============================================中断回调函数

 