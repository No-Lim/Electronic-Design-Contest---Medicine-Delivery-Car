#include "follow_line.h"
#include "oled.h"
#include "main.h"
#include "stdio.h"
#include "motor.h"

char have_line =0;

uint8_t find_line_state = 1;
uint16_t find_time=0;	
uint8_t intersection=0;
uint8_t final_intersection=0;
uint8_t low_pin_count = 0;
uint8_t count_fw=0;
//uint8_t low_pin_time = 0;//丢失目标时间

uint8_t Track_Dates[8]={0};

uint8_t active_sensors;
uint8_t active_sensors_R;
uint8_t active_sensors_L;

float Sensor_Weights[8]={-4,-3,-2,-1,1,2,3,4}; // 传感器位置权重

int Position_PID (int Encoder,int Target);   //差速PID控制器//这里Encoder=error，Target=0;

extern uint8_t Do_count,Do2_count,Do_count_1,Load_flag;
extern char TargetRoom;
extern uint8_t BL_Flag,Go_Flag;


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
  active_sensors = 0;
	active_sensors_R = 0;
	active_sensors_L = 0;
	for(i=0;i<8;i++)
	{
		if(Dates[i]==0)
		{
			active_sensors++;
		if(i<=3)
			active_sensors_L++;
		else if(i>=4)
			active_sensors_R++;
		}			
//		sum+=((Dates[i])*Sensor_Weights[i]);
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

char value_led[20];
char show_intersection[20];
extern float car_speed_l,car_speed_r;
int16_t PID_Turn=0;
int find_value;

int Find_Line_Begins(void)
{
	int i;
	Get_Track_Dates(Track_Dates);
	find_value = Get_Track_Errors(Track_Dates);
	
	for(i=7;i>=0;i--)
	{
		OLED_ShowNum(2,8-i,Track_Dates[i],1);
	}
	Moto_forward_run();
  //sprintf(value_led,"value_led = %d   ",get_LedFind_Scan());
  //OLED_ShowString(0,1,(unsigned char *)value_led);

	PID_Turn=Position_PID(find_value,0);
	if(Load_flag==1 || Go_Flag == 1)
	{
		if((active_sensors>=8) &&Do_count != 2 && (TargetRoom == 'A'||TargetRoom == 'B'))
		intersection=1;
	else if(active_sensors==0 && Do_count == 2 && (TargetRoom == 'A'||TargetRoom == 'B'))
		intersection=1;
	else if((active_sensors>=8) && Do_count != 9 && Do_count != 3 && Do_count != 8 && Do_count != 6  && (TargetRoom == 'C' || TargetRoom == 'D' || TargetRoom == 'E' || TargetRoom == 'F' || TargetRoom == 'G' || TargetRoom == 'H'))
		intersection=1;
	else if((active_sensors==0) && (Do_count == 9 || Do_count == 8 || Do_count == 3||Do_count == 6) && (TargetRoom == 'C' || TargetRoom == 'D' || TargetRoom == 'E' || TargetRoom == 'F' || TargetRoom == 'G' || TargetRoom == 'H'))
		intersection=1;
	}
	
//	else if((active_sensors>=7) && Load_flag==2 && Do2_count != 3 && (TargetRoom == 'A'||TargetRoom == 'B'))
//		intersection=1;
//	else if(active_sensors==0 && Load_flag==2 && Do2_count == 3 && (TargetRoom == 'A'||TargetRoom == 'B'))
//		intersection=1;
//	else if((active_sensors>=7)&& Load_flag==2  && Do2_count != 4 && (TargetRoom == 'C' || TargetRoom == 'D') )
//		intersection=1;
//	else if((active_sensors==0)&& Load_flag==2  && Do2_count == 4 && (TargetRoom == 'C' || TargetRoom == 'D') )
//		intersection=1;
////	else if((active_sensors>=3)&& Load_flag==2  && Do2_count == 3 && (TargetRoom == 'E' || TargetRoom == 'F' || TargetRoom == 'G' || TargetRoom == 'H') && count_fw<=5)
////	{
////		count_fw++;
////	}
//	else if((active_sensors>=4)&& Load_flag==2  && (Do2_count == 1 || Do2_count==3) && (TargetRoom == 'E' || TargetRoom == 'F' || TargetRoom == 'G' || TargetRoom == 'H'))
//	{
////		count_fw=0;
//		intersection=1;
//	}
	else if(Load_flag==2 || Go_Flag == 1)
	{
		if(((active_sensors_L >=4) || (active_sensors_R >= 4)) && Do2_count!=5 && (TargetRoom == 'C' ||TargetRoom == 'D'||TargetRoom == 'E' || TargetRoom == 'F' || TargetRoom == 'G' || TargetRoom == 'H'))
		intersection=1;

	else if((active_sensors==0)&& Do2_count == 5 && (TargetRoom == 'C' ||TargetRoom == 'D' ||TargetRoom == 'E' || TargetRoom == 'F' || TargetRoom == 'G' || TargetRoom == 'H'))
		intersection=1;
	}
	
	else if(Load_flag==0)
	{
		if((active_sensors >= 8) && Do_count_1 == 0)
			intersection=1;
		else if((active_sensors == 0) && Do_count_1 == 2)
			intersection=1;
	}
	
//	if(BL_Flag == 1 && (active_sensors>=7) && Do_count==0)
//  {
//	intersection=1;
//	}

//	else if(BL_Flag == 1 && (active_sensors==0) && Do_count==2)
//  {
//	intersection=1;
//	}
	
	
//	set_speed(35 + find_value,35 - find_value);
	car_speed_l = 22 + PID_Turn;
	car_speed_r = 22 - PID_Turn;

//	car_speed_l = 10 ;
//	car_speed_r = 10 ;
	
}

int Position_PID (int Encoder,int Target)   //差速PID控制器//这里Encoder=error，Target=0;
{ 	
	 float Position_KP=3.6,Position_KI=0,Position_KD=3.7;
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Encoder-Target;                                  //计算偏差
	 Integral_bias+=Bias;	                                 //求出偏差的积分
	 Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);       //位置式PID控制器
	 Last_Bias=Bias;                                       //保存上一次偏差 
	 return Pwm;                                           //返回PWM值
}
