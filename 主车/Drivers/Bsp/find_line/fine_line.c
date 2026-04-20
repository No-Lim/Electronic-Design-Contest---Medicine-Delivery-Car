#include "fine_line.h"

#include "../../Drivers/Bsp/oled/oled.h"
#include "stdio.h"

char have_line =0;

uint8_t find_line_state = 1;
uint16_t find_time=0;	
uint8_t intersection=0;
uint8_t final_intersection=0;
uint8_t low_pin_count = 0;
//uint8_t low_pin_time = 0;//丢失目标时间

uint8_t Track_Dates[8]={0};
int8_t L_error=0,R_error=0;
uint8_t active_sensors;

float Sensor_Weights[8]={-5,-3,-2,-1,1,2,3,5}; // 传感器位置权重

int Position_PID (int Encoder,int Target);   //差速PID控制器//这里Encoder=error，Target=0;

extern uint8_t Do_count,Do2_count;

 void Get_Track_Dates(uint8_t *Dates)
{
if(READ_FIND_LINE_OUT1==1)
Dates[0]=1;
else Dates[0]=0; 
if(READ_FIND_LINE_OUT2==1)
Dates[1]=1;
else Dates[1]=0; 
if(READ_FIND_LINE_OUT3==1)
Dates[2]=1;
else Dates[2]=0; 
if(READ_FIND_LINE_OUT4==1)
Dates[3]=1;
else Dates[3]=0; 
if(READ_FIND_LINE_OUT5==1)
Dates[4]=1;
else Dates[4]=0;
if(READ_FIND_LINE_OUT6==1)
Dates[5]=1;
else Dates[5]=0;
if(READ_FIND_LINE_OUT7==1)
Dates[6]=1;
else Dates[6]=0;
if(READ_FIND_LINE_OUT8==1)
Dates[7]=1;
else Dates[7]=0;
}



float Get_Track_Errors(uint8_t *Dates)
{	
    int i;
	float sum = 0.0f;
	L_error=0,R_error=0;
  active_sensors = 0;
	for(i=0;i<8;i++)
	{
//	sum+=((Dates[i])*Sensor_Weights[i]);
	if(Dates[i]==0)
	{
		active_sensors++;
		if(i<=3)
			L_error++;
		if(i>=4)
			R_error++;
	}
	
	}
	
		if(active_sensors >= 4)
	{
		for( i = 2 ; i<=5 ; i++)
			sum+=((Dates[i])*Sensor_Weights[i]);
	}
	
	else if(active_sensors <4 && active_sensors > 0 )
	{
		for( i = 0 ; i<=7 ; i++)
			sum+=((Dates[i])*Sensor_Weights[i]);
	}

	if (active_sensors == 0) return 0.0f; // 未检测到黑线
//    return ((sum / active_sensors)); 
	return sum;
}

//uint8_t get_LedFind_Scan(void)
//{
//	low_pin_count = 0;
//	//有一个传感器识别到色块就+1
//	if (READ_FIND_LINE_OUT1 == GPIO_PIN_SET) low_pin_count++;
//	if (READ_FIND_LINE_OUT2 == GPIO_PIN_SET) low_pin_count++;
//	if (READ_FIND_LINE_OUT3 == GPIO_PIN_SET) low_pin_count++;
//	if (READ_FIND_LINE_OUT4 == GPIO_PIN_SET) low_pin_count++;
//	if (READ_FIND_LINE_OUT5 == GPIO_PIN_SET) low_pin_count++;
//	if (READ_FIND_LINE_OUT6 == GPIO_PIN_SET) low_pin_count++;
//	if (READ_FIND_LINE_OUT7 == GPIO_PIN_SET) low_pin_count++;
//	if (READ_FIND_LINE_OUT8 == GPIO_PIN_SET) low_pin_count++;
//	
//	if (low_pin_count >= 4) final_intersection+=2;				//识别到4个以上色块就可以判断为路口
//  else if	(low_pin_count == 0)//没有识别到色块也可以判断为路口
//	{
//		final_intersection++;
//	}
//	else final_intersection=0;
//	if(final_intersection>100)
//	{
//		intersection = 1;
//		final_intersection = 0;
//		HAL_Delay(200);
//	}

//	if(READ_FIND_LINE_OUT4 == 1)
//	{
//		return 3;
//	}
//	else if(READ_FIND_LINE_OUT5 == 1)
//	{
//		return 4;
//	}
//	else if(READ_FIND_LINE_OUT6 == 1)
//	{
//		return 5;
//	}
//	else if(READ_FIND_LINE_OUT3 == 1)
//	{
//		return 2;
//	}
//	else if(READ_FIND_LINE_OUT2 == 1)
//	{
//		return 1;
//	}
//	else if(READ_FIND_LINE_OUT7 == 1)
//	{
//		return 6;
//	}
//	else if(READ_FIND_LINE_OUT1 == 1)
//	{
//		return 0;
//	}
//	else if(READ_FIND_LINE_OUT8 == 1)
//	{
//		return 7;
//	}
//	else
//	{
//		return 3;
//	}	
////	else
////	{
////		find_time++;
////		if(find_time>0 && find_time<100)
////			find_line_state = 1; //find line is lost
////		if(find_time>=100)
////		{
////			find_time=1;
////			find_line_state = 0;	//find line is end
////			if(find_time>10000)
////				find_time=100;
////		}
////	}
////	
////	return 2;
//}

//static int Linechange_list[8]=
//{
//	-16,
//	-11,
//	-8,
//	-2,
//	2,
//	8,
//	11,
//	16,
//};
//extern void set_speed(uint16_t left,uint16_t right);
char value_led[20];
char show_intersection[20];
extern uint8_t car_speed_l,car_speed_r;
int PID_Turn=0;
int find_value;

int Find_Line_Begins(void)
{
	int i;
	Get_Track_Dates(Track_Dates);
	find_value = Get_Track_Errors(Track_Dates);
	
	for(i=7;i>=0;i--)
	{
		OLED_ShowNum(56-i*8,4,Track_Dates[i],1,8);
	}
	Moto_forward_run();
  //sprintf(value_led,"value_led = %d   ",get_LedFind_Scan());
  //OLED_ShowString(0,1,(unsigned char *)value_led);

	PID_Turn=Position_PID(find_value,0);
	
	if((active_sensors>=6) && Load_flag==1 &&Do_count != 2 && (TargetRoom == 'A'||TargetRoom == 'B'))
		intersection=1;
	else if(active_sensors==0 && Load_flag==1 && Do_count == 2 && (TargetRoom == 'A'||TargetRoom == 'B'))
		intersection=1;
	else if((active_sensors>=8)&& Load_flag==1  && Do_count != 9 && (TargetRoom == 'C' || TargetRoom == 'D' || TargetRoom == 'E' || TargetRoom == 'F' || TargetRoom == 'G' || TargetRoom == 'H'))
		intersection=1;
	else if((active_sensors==0)&& Load_flag==1  && Do_count == 9 && (TargetRoom == 'C' || TargetRoom == 'D' || TargetRoom == 'E' || TargetRoom == 'F' || TargetRoom == 'G' || TargetRoom == 'H'))
		intersection=1;
	else if((active_sensors>=6) && Load_flag==2 && Do2_count != 3 && (TargetRoom == 'A'||TargetRoom == 'B'))
		intersection=1;
	else if(active_sensors==0 && Load_flag==2 && Do2_count == 3 && (TargetRoom == 'A'||TargetRoom == 'B'))
		intersection=1;
	else if((active_sensors>=6)&& Load_flag==2  && Do2_count != 4 && (TargetRoom == 'C' || TargetRoom == 'D') )
		intersection=1;
	else if((active_sensors==0)&& Load_flag==2  && Do2_count == 4 && (TargetRoom == 'C' || TargetRoom == 'D') )
		intersection=1;
	else if(((L_error == 4) || (R_error == 4))&& Load_flag==2  && Do2_count != 5 && (TargetRoom == 'E' || TargetRoom == 'F' || TargetRoom == 'G' || TargetRoom == 'H'))
		intersection=1;
	else if((active_sensors==0)&& Load_flag==2  && Do2_count == 5 && (TargetRoom == 'E' || TargetRoom == 'F' || TargetRoom == 'G' || TargetRoom == 'H'))
		intersection=1;
	
//	set_speed(35 + find_value,35 - find_value);
	car_speed_l = 34 - PID_Turn;
	car_speed_r = 34 + PID_Turn;
	
//	car_speed_l = 0 ;
//	car_speed_r = 0 ;
	
}

int Position_PID (int Encoder,int Target)   //差速PID控制器//这里Encoder=error，Target=0;
{ 	
	 float Position_KP=1,Position_KI=0,Position_KD=3;
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Encoder-Target;                                  //计算偏差
	 Integral_bias+=Bias;	                                 //求出偏差的积分
	 Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);       //位置式PID控制器
	 Last_Bias=Bias;                                       //保存上一次偏差 
	 return Pwm;                                           //返回PWM值
}
