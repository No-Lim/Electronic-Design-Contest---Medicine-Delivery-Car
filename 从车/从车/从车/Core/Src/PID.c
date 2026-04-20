#include "gpio.h"

extern int OUTPUT1,OUTPUT2;//OUTPUT1 和OUTPUT2 是控制小车左右轮速度的PWM输出值

extern int PID_cs;//小车差速PID控制器的PWM输出值

extern int PID_jc1,PID_jc2;//小车基础速度PID控制器的PWM输出值

int Position_PI (int Encoder,int Target)   //基础速度PI控制器//这里Encoder=a，Target=0;
{  
	 float Position_KP=0.6,Position_KI=0.02,Position_KD=0; //参数整定，需要调到适合自己小车的数值大小  
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Target-Encoder;                                  //计算偏差
	 Integral_bias+=Bias;	                                 //求出偏差的积分
	 Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);       //位置式PID控制器
	 Last_Bias=Bias;                                       //保存上一次偏差 
	 return Pwm;                                           //返回PWM值
}

int Position_PD (int Encoder,int Target)   //舵机PID控制器//这里Encoder=error，Target=0;
{ 	
	 float Position_KP=7.2,Position_KI=0,Position_KD=14;
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=Encoder-Target;                                  //计算偏差
	 Integral_bias+=Bias;	                                 //求出偏差的积分
	 Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);       //位置式PID控制器
	 Last_Bias=Bias;                                       //保存上一次偏差 
	 return Pwm;                                           //返回PWM值
}

void Xianfu_PID(void)//限幅
{	
		
	
	  OUTPUT1=PID_jc1+PID_cs;//基础速度+速度偏差量 
	  OUTPUT2=PID_jc2-PID_cs;//基础速度-速度偏差量
	
	   /*控制小车电机的PWM有效值范围为0-100*/
	  if(OUTPUT1>50) OUTPUT1=50;
    if(OUTPUT1<-50) OUTPUT1=-50;
	  if(OUTPUT2>50) OUTPUT2=50;
    if(OUTPUT2<-50) OUTPUT2=-50;
		
}
