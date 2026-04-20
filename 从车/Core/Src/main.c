/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"
#include "motor.h"
#include "encoder.h"
#include "pid.h"
#include "follow_line.h"
#include "esp8266.h"
#include "Sonar.h"
//#include "dht11.h"
#include "delay.h"
#include "esp8266.h"
#include "onenet.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ESP8266_ONENET_INFO		"AT+CIPSTART=\"TCP\",\"mqtts.heclouds.com\",1883\r\n"
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

//Main car and follow car each has 5 states,1-Car Start,2-Car Go,3-Car Arrive,4-Car Back,5-Car Quit

uint8_t temp=0,humi=0;
uint8_t LoadCount,NotLoadCount;
uint8_t Load_flag = 0; 
uint8_t Rec_Count = 0; 


uint8_t Middle_Cooperate_Flag = 0; 
uint8_t BL_Rec_Flag = 1; 

typedef enum
{
	left_90,
	left_90_1,
	right_90,
	right_90_1,
	back_180_l,
	back_180_r,
	straight,
	fianl,
	ojbk
}spin_dir_t;	//转弯函数的输入参数

uint8_t Spin_start_flag, Spin_succeed_flag, Stop_Flag;//旋转开始标志位  旋转成功标志位  路口停止标志位
uint8_t Do_count = 0, Do2_count = 0,Do_count_1 = 0;	//任务计数
__IO uint16_t SendTime = 0;						//电机PID计算时间调整
float Velocity,Turn;									//速度  转向状态
float car_speed_l=0,car_speed_r=0;	//设置电机的速度
char show[10];												//储存OLED需要显示的字符串
extern uint8_t intersection;					//路口识别标志位
extern uint8_t LoadCount,NotLoadCount;//装载药品时间计数  拿走药品时间计数
extern uint16_t lukou_straight; 
extern uint8_t Num;
extern char BL_Num;

uint8_t BL_Flag=1;

void Find(void);											//巡线函数
void spin_Turn(spin_dir_t zhuanxiang);//转弯函数
char Load_mode;												//药品装载标志位
uint8_t key_num=0;										//按键扫描标志位

//调参用：
uint8_t sendBuf_test[4];					//串口发送的数据


void LoadOrNot();
void spin_Turn(spin_dir_t zhuanxiang);

int OUTPUT1=0,OUTPUT2=0;//OUTPUT1 和OUTPUT2 是控制小车左右轮速度的PWM输出值
int PID_cs;//小车差速PID控制器的PWM输出值
int PID_jc1,PID_jc2;//小车基础速度PID控制器的PWM输出值
//uint8_t Track_Dates[7]={0};
int8_t Flag_BroadCast = 0;
int8_t error;
int8_t timeCount=0;
int8_t i;
int8_t ok = 0;
int8_t ok_2 = 1;
int8_t Far_Cooperate_Flag = 0;
int8_t Far_Back_Flag = 0;
uint8_t Far_Flag = 0;
uint8_t Go_Flag = 0;
char Load_mode;												//药品装载标志位
uint8_t LoadCount,NotLoadCount;//放下药品的计数值		拿走药品的计数值
extern uint32_t high_time,time;
extern uint8_t recv_buf[5];
extern int8_t active_sensors;
extern uint8_t Screen_Information;
extern uint8_t Rx1_Temp;
extern uint8_t Track_Dates[8];
extern uint8_t Rx1_Temp;
//int16_t Cnt=0;

//int8_t Log[4]={0};//路线日志，回城用。当为0时，说明没有经过这个路口
//// 1--直走 2--左拐 3--右拐

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
///////////////////////////////Variable///
int16_t True_Speed_L,True_Speed_R;
int8_t Rcv_Num=3;

extern int8_t LoR,TargetRoom,TASK,TargetNum,RoomNum;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	
  unsigned char *dataPtr = NULL;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
	
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM4_Init();
  MX_TIM3_Init();
  MX_TIM5_Init();
  MX_TIM2_Init();
  MX_USART3_UART_Init();
  MX_USART2_UART_Init();
  MX_USART6_UART_Init();
  MX_TIM9_Init();
  MX_UART5_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	//////////////////////////////////�����ʼ������/////
	OLED_Init();
	Encoder_Init();
	Motor_Init();
	HAL_UART_Receive_IT(&huart3, &Openmv_Receive_Buffer[0], 1);
	HAL_UART_Receive_IT(&huart2,(uint8_t *)&BL_Num, 1);
	HAL_UART_Receive_IT(&huart1,&Rx1_Temp,1);
	HAL_UART_Receive_IT(&huart5, &Screen_Information, 2);
//	HAL_Delay(3000);
//	Write_Data(0x67);
	
	ESP8266_Init();

//	HAL_Delay(200000);

//  //printf("Connect MQTTs Server...\r\n");
	OLED_Clear(); OLED_ShowString(1,1,"Connecting...");
	while(ESP8266_SendCmd(ESP8266_ONENET_INFO, "CONNECT"))
		delay_ms(500);
//	//printf("Connect MQTT Server Success\r\n");
	OLED_ShowString(2,1,"MQTT Success!"); delay_ms(500);

	OLED_Clear(); OLED_ShowString(3,1,"Device login ...");
	while(OneNet_DevLink())			//接入OneNET
	{
		ESP8266_SendCmd(ESP8266_ONENET_INFO, "CONNECT");
		delay_ms(500);
	}

  OneNET_Subscribe();
	humi=1;
	OneNet_SendData();
	HAL_Delay(1500);
//	Write_Data_1(0x03);
	OLED_Clear();
	Write_Data(0x67);
//	HAL_GPIO_WritePin(Green_LED_GPIO_Port,Green_LED_Pin,GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(Red_LED_GPIO_Port,Red_LED_Pin,GPIO_PIN_RESET);
	HAL_Delay(500);
//	//播报队伍信息
//	Write_Data_2(0x8D);
//	HAL_Delay(2000);
//	Write_Data_2(0x92);
//	HAL_Delay(6000);
//	Write_Data_2(0x8F);
//	HAL_Delay(2000);
//	Write_Data_2(0x8E);
//	HAL_Delay(5000);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (0)
  {
//		True_Speed_L=Speed_L_Get();
//		True_Speed_R=Speed_R_Get();
//		OLED_ShowSignedNum(2,1,True_Speed_L,3);
//		OLED_ShowSignedNum(2,5,True_Speed_R,3);
		OLED_ShowSignedNum(1,5,Load_flag,1);
//		Get_Track_Dates(Track_Dates);
//		error=Get_Track_Errors(Track_Dates);
//		
//		for(i=0;i<7;i++)
//			OLED_ShowNum(3,i+1,Track_Dates[i],1);
		
		PID_jc1=Position_PI (True_Speed_L,0);
		PID_jc2=Position_PI (True_Speed_R,0);
		PID_cs=Position_PD(error,0);
		Xianfu_PID();
		
		SetSpeed_L(OUTPUT1);
		SetSpeed_R(OUTPUT2);
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
	
	while(0)
	{
		OLED_ShowHexNum(1,1,recv_buf[3],2); //显示语音信息的ID号
			OLED_ShowHexNum(3,1,Rx1_Temp,2);  
    OLED_ShowHexNum(4,1,Screen_Information,4);  //显示接收到的屏幕信息
//		HAL_GPIO_WritePin(Green_LED_GPIO_Port,Green_LED_Pin,GPIO_PIN_RESET);
//		HAL_GPIO_WritePin(Red_LED_GPIO_Port,Red_LED_Pin,GPIO_PIN_SET);
//		HAL_Delay(1500);
//		HAL_GPIO_WritePin(Red_LED_GPIO_Port,Red_LED_Pin,GPIO_PIN_RESET);
//		HAL_GPIO_WritePin(Green_LED_GPIO_Port,Green_LED_Pin,GPIO_PIN_SET);
//		HAL_Delay(1500);
//		hhSerialSendByte('C');
		Get_Track_Dates(Track_Dates);
		for(i=7;i>=0;i--)
	{
		OLED_ShowNum(2,8-i,Track_Dates[i],1);
	}

	}
		
	while(1)
	{
		OLED_ShowString(1,8,"BL:");
//		OLED_ShowNum(4,6,high_time,3);
//		OLED_ShowSignedNum(1,5,Stop_Flag,1);
	  OLED_ShowNum(3,6,Load_flag,2);
		//OLED_ShowNum(3,1,recv_buf[3],3);
		OLED_ShowNum(3,1,Middle_Cooperate_Flag,3);
//		OLED_ShowNum(2,1,SendTime,3);
//		SR04_GetData();
		
		if(Screen_Information == 0x31)
		{
			Flag_BroadCast = 1;
			Screen_Information = 0;
			//	//播报队伍信息
		}
		
		if(Flag_BroadCast == 1)
		{
			Write_Data_2(0x8D);
			HAL_Delay(2000);
			Write_Data_2(0x92);
			HAL_Delay(5500);
			Write_Data_2(0x8F);
			HAL_Delay(2000);
			Write_Data_2(0x8E);
			HAL_Delay(5000);
			Flag_BroadCast = 0;
		}
		
		if(SendTime >= 10)	//SendTime每10ms +1
		{
			timeCount++;
			SendTime = 0;
			SendDataToOpenmv();						//不能太快，否则会超过openmv的串口接收数据缓冲区
			if(Load_mode==0) LoadOrNot();	//判断药物是否装载
			//OLED_Clear();									//显示屏打开
		}
		
//		if(timeCount >= 30)									//发送间隔5s
//		{
////			OneNet_SendData();									//发送数据
////			OLED_ShowNum(2,1,timeCount,3);
//			timeCount = 0;
//		}
		
		if(Load_flag == 1 && Go_Flag == 1)
			Far_Back_Flag = 1;
		
		if((Load_flag == 1||Go_Flag == 1)&&TASK == 1) 	//选择任务一 || 药物已经放上
		{
//			OneNet_SendData();
//			HAL_Delay(3000);
			SetTargetRoom();LoR=0;							//设置识别到的房间号
		}
		
		if(BL_Num=='S')//as in started
		{
			HAL_GPIO_WritePin(User_Led_GPIO_Port,User_Led_Pin,GPIO_PIN_SET);
			temp=1;
//			OneNet_SendData();
//			HAL_Delay(1500);
			Rec_Count+=1;
		}
		else if(BL_Num=='P')//as in process
		{
			HAL_GPIO_WritePin(User_Led_GPIO_Port,User_Led_Pin,GPIO_PIN_RESET);
			temp=2;
//			OneNet_SendData();
//			HAL_Delay(1000);
			Rec_Count+=1;
		}
		
		else if(BL_Num=='G' )//follow car Go,as main car arrive at room
		{
			HAL_GPIO_WritePin(User_Led_GPIO_Port,User_Led_Pin,GPIO_PIN_SET);
			//hhSerialSendByte('B');
			BL_Flag=1;
			temp=3;
//			OneNet_SendData();
			Rec_Count+=1;
//			Do_count=0;
		}
		
		else if(BL_Num=='B')//as in back
		{
			HAL_GPIO_WritePin(User_Led_GPIO_Port,User_Led_Pin,GPIO_PIN_RESET);
			temp=4;
			OneNet_SendData();
			Rec_Count+=1;
			BL_Flag = 3;
			
		}
		
		else if(BL_Num=='Q')//main car Quit
		{
			HAL_GPIO_WritePin(User_Led_GPIO_Port,User_Led_Pin,GPIO_PIN_SET);
			//hhSerialSendByte('A');
//			BL_Flag=2;
			//Do_count=0;
			ok_2=0;
			temp=5;
			Middle_Cooperate_Flag = 1;
			BL_Rec_Flag = 0;
			if(Far_Flag==1)
			{
				Go_Flag = 1;
				Far_Flag = 0;
			}
		}
		
		else if(BL_Num=='F')//main car Quit
		{
			Far_Flag = 1;
			Far_Cooperate_Flag = 1;
		}
		
		//OLED_ShowNum(2,8, Num,1);
		
		if(Far_Cooperate_Flag == 1) 	//锟斤拷锟轿伙拷锟斤拷锟斤拷为A锟斤拷锟斤拷锟斤拷锟斤拷为1
				{
					switch(Do_count_1)
					{
						case 0:
						{
							
							HAL_GPIO_WritePin(Red_LED_GPIO_Port,Red_LED_Pin,GPIO_PIN_RESET);
							HAL_GPIO_WritePin(Green_LED_GPIO_Port,Green_LED_Pin,GPIO_PIN_RESET);
							HAL_Delay(200);
							Stop_Flag = 0;							//巡锟斤拷停锟斤拷锟斤拷志位
							Spin_succeed_flag = 0;			//转锟斤拷晒锟斤拷锟街疚?
							while(Stop_Flag==0)Find();  //巡锟斤拷(锟斤拷时 Stop_Flag ==0  锟斤拷珊锟斤拷远锟斤拷锟?)
							car_speed_l=car_speed_r=0;
							//Moto_stop();
//							HAL_Delay(200);
							Do_count_1++;	
						}
						break;
						
						case 1:
						{
							if(Stop_Flag == 1)		//确锟斤拷锟斤拷时巡锟竭关憋拷
							{
								spin_Turn(left_90);	//锟斤拷转  锟斤拷时Spin_succeed_flag== 0 ,锟斤拷珊锟斤拷远锟斤拷锟?
								Moto_stop();
								//Spin_succeed_flag = 1;
								HAL_Delay(200);
								Do_count_1++;
							}
						}
						break;
						
						case 2:
						{
							if(Spin_succeed_flag == 1)	//锟斤拷一锟斤拷锟斤拷转锟斤拷锟絊pin_succeed_flag锟斤拷锟斤拷1
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
								while(Stop_Flag==0) Find();
								Moto_stop();
								car_speed_l=car_speed_r=0;
								HAL_Delay(500);
								Do_count_1++;
							}
						}
						break;
							
						case 3:
						{
							if(Stop_Flag == 1)	//锟斤拷锟秸碉拷
							{
								Far_Cooperate_Flag = 0;
								car_speed_l=car_speed_r=0;
								Moto_stop();
								spin_Turn(back_180_l);	//前锟斤拷一锟轿撅拷锟斤拷
								Moto_stop();
								HAL_Delay(200);
								humi=2;
							OneNet_SendData();
							HAL_Delay(1500);
//								Go_Flag = 1;
//								Far_Cooperate_Flag = 1;
							}
							
						}
						break;
					}
				}

		if(TASK == 2 )    
	 	{
			OLED_ShowNum(1,1,Do_count,2);
			
			//oled_show();							//OLED刷新
			if((Load_flag == 1) || ((Go_Flag == 1)&&((TargetRoom == 'E' )||(TargetRoom == 'F') ||(TargetRoom == 'G')||(TargetRoom == 'H'))))    		//运送药物
			{
				//中端病房和远端病房需要发送特定标志位
					
				if((Load_flag==1) ||((Go_Flag == 1)&&( TargetRoom == 'E' || TargetRoom == 'F' || TargetRoom == 'G' || TargetRoom == 'H')))
				{
					//OLED_ShowNum(3,1, Do_count,2);
					switch(Do_count)	//直走到第一个路口
					{
						case 0: 
						{
							
							Spin_succeed_flag = 0;
							Stop_Flag = 0;
							while(Stop_Flag == 0) Find();
							car_speed_l=car_speed_r=0;
							//Moto_stop();
							Do_count++;
							//SendDataToOpenmv();  //打开数字识别
						}
						break;
						
						case 1:
						{
							if(Stop_Flag ==1)				//遇到第一个路口
							{
								if(Go_Flag == 1)
									spin_Turn(left_90);
								else
									spin_Turn(straight);	//此时Spin_succeed_flag== 0 ,完成后自动置1
								
								Do_count++;
								//SendDataToOpenmv();	//打开模板匹配数字
							}
						}
						
						case 2:
						{
							if(Spin_succeed_flag == 1)	//第一个路口往第二个路口直走
							{
								Spin_succeed_flag = 0;
								lukou_straight=1;
								Stop_Flag = 0;
								car_speed_l=car_speed_r=0;
								while(lukou_straight <= 160) Find();//lukou_straight 每10ms +1
								Moto_stop();
								//巡线1.2s后停下1.5s识别数字,再继续巡线到下一个路口
								lukou_straight=0;
								Turn=0;
								Moto_stop();
								car_speed_l=car_speed_r=0;
								//HAL_Delay(1500);
								HAL_Delay(500);
								Moto_forward_run();
								
								intersection = 0;
								Stop_Flag = 0;
								Spin_succeed_flag = 0;
								while(Stop_Flag == 0) Find();
								car_speed_l=car_speed_r=0;
								Moto_stop();
								//HAL_Delay(200);
								Do_count++;
							 }
						 }
						
						case 3:	//第二个路口		判定是否识别到数字，且该数字的位置偏左还是偏右
						{
							if(Stop_Flag == 1)
							{
								if(RoomNum == TargetNum && LoR == 1)	//识别到了目标数字, 且其位置偏左
								{
									Do_count = 8; 											//后续直至左转并直行到病房门口的函数
									TargetRoom = 'C';
									
								}
								else if(RoomNum == TargetNum && LoR == 2)		//识别到目标数字, 且其位置偏右
								{
									Do_count = 8;			//后续直至右转并直行到病房门口的函数
									TargetRoom = 'D';
									
								}	
								if(LoR == 0)		//没识别到就直走到第三个路口停止
								{
									spin_Turn(straight);
									lukou_straight=1;
									HAL_Delay(200);
									car_speed_l=car_speed_r=0;
									while(lukou_straight <= 180) Find();
									car_speed_l=car_speed_r=0;
									Moto_stop();
									//HAL_Delay(200);
									
									lukou_straight=0;
									Moto_stop();
									car_speed_l=car_speed_r=0;
//									HAL_Delay(1500);
									HAL_Delay(500);
									Moto_forward_run();
									
									intersection = 0;
									Stop_Flag = 0;
									Spin_succeed_flag = 0;
									car_speed_l=car_speed_r=0;
									while(Stop_Flag == 0) Find();
									car_speed_l=car_speed_r=0;
									Moto_stop();
									car_speed_l=car_speed_r=0;
									//HAL_Delay(200);
									Do_count++;
								}	
							}
						}
						break;
								
						case 4:	//第三个路口准备转弯
						{
							if(Stop_Flag == 1)
							{
								if(RoomNum == TargetNum && LoR == 1)   //识别到了目标数字, 且其位置偏左
								{
									Do_count++; 
									TargetRoom = 'E';		//先假定是E
									SendDataToOpenmv(); //关闭模板匹配数字
								}
								else if(RoomNum == TargetNum && LoR == 2 )  //识别到目标数字, 且其位置偏右
								{
									Do_count++;
									TargetRoom = 'G';		//先假定是G
									SendDataToOpenmv(); //关闭模板匹配数字
								}
							}
						}
						break;
								
						case 5://转弯
						{
							if(Stop_Flag == 1)
							{
								Do_count++;
								if(TargetRoom == 'E')
								{
									spin_Turn(left_90_1);
									car_speed_l=car_speed_r=0;
									Moto_stop();
									//HAL_Delay(200);
									LoR = 0;						//清空预转弯方向
									SendDataToOpenmv(); //打开模板匹配数字
								}
								else if(TargetRoom == 'G')
								{
									spin_Turn(right_90);
									car_speed_l=car_speed_r=0;
									Moto_stop();
									//HAL_Delay(200);
									LoR = 0;
									SendDataToOpenmv();
								}
							}
						}
						break;
						
						case 6: //直走到第四个路口
						{
							if(Spin_succeed_flag == 1)
							{
								Do_count++;
								Stop_Flag = 0;	
								lukou_straight=1;
								car_speed_l=car_speed_r=0;
								while(lukou_straight <= 150) Find(); 
								car_speed_l=car_speed_r=0;
								Moto_stop();
								//HAL_Delay(200);
								
								lukou_straight=0;
								Moto_stop();
								car_speed_l=car_speed_r=0;
//								HAL_Delay(1500);
								HAL_Delay(500);
								Moto_forward_run();
								
								intersection = 0;
								Stop_Flag = 0;
								Spin_succeed_flag = 0;
								car_speed_l=car_speed_r=0;
								while(Stop_Flag == 0) Find();
								car_speed_l=car_speed_r=0;
								Moto_stop();
								//HAL_Delay(200);
							}
						}
						break;
						
						case 7: //第四个路口准备转弯
						{
							car_speed_l=car_speed_r=0;
							Moto_stop();
							if(Stop_Flag == 1)
							{
								if(TargetRoom == 'E')
								{
									if(RoomNum == TargetNum && LoR == 1)    //一定时间内识别到RoomNum, 且其位置偏左
									{
										Do_count ++;  
										TargetRoom = 'E';
										SendDataToOpenmv();	//关闭模板匹配数字	
									}										 
									else if(RoomNum == TargetNum && LoR == 2)//一定时间内识别到RoomNum, 且其位置偏右
									{
										Do_count++;  
										TargetRoom = 'F';
										SendDataToOpenmv();	//关闭模板匹配数字	
									}
									else
									{
										Turn=0;
									}
								}
								else if(TargetRoom == 'G')
								{
									if(RoomNum == TargetNum && LoR == 1)    //一定时间内识别到RoomNum, 且其位置偏左
									{
										Do_count++; 
										TargetRoom = 'G';
										SendDataToOpenmv(); //关闭模板匹配数字	
									}										 											 
									else if(RoomNum == TargetNum && LoR == 2)   //一定时间内识别到RoomNum, 且其位置偏右
									{
										Do_count++;  
										TargetRoom = 'H';
										SendDataToOpenmv(); //关闭模板匹配数字	
									}
									else
									{
										Turn=0;
									}
								}	
							}
						}
						break;
								
						case 8://最后路口转弯
						{
							if(Stop_Flag == 1)
							{
								if(TargetRoom == 'C')
								{
									spin_Turn(right_90);
									
										car_speed_l=car_speed_r=0;
									Moto_stop();
									//HAL_Delay(200);
									//HAL_Delay(200);
									Spin_succeed_flag = 0;
								Stop_Flag = 0;
								car_speed_l=car_speed_r=0;
								while(Stop_Flag == 0) Find();
									Moto_stop();
										car_speed_l=car_speed_r=0;
									HAL_Delay(500);
								Moto_stop();
									spin_Turn(back_180_l);
									Moto_stop();
										car_speed_l=car_speed_r=0;
									HAL_GPIO_WritePin(Red_LED_GPIO_Port,Red_LED_Pin,GPIO_PIN_SET);
									HAL_GPIO_WritePin(Green_LED_GPIO_Port,Green_LED_Pin,GPIO_PIN_SET);
									Do_count = 11;
									
										//HAL_Delay(100);	//越长时间就前进越多
								}
								else if(TargetRoom == 'E' || TargetRoom == 'G')
									{
										spin_Turn(fianl);
										spin_Turn(left_90);
									Moto_stop();
										car_speed_l=car_speed_r=0;
									//HAL_Delay(200);
									//HAL_Delay(200);
									Do_count++;
										//HAL_Delay(100);	//越长时间就前进越多
									}
								else if(TargetRoom == 'D')
								{
									
									spin_Turn(left_90);
									Moto_stop();
										car_speed_l=car_speed_r=0;
									Spin_succeed_flag = 0;
								Stop_Flag = 0;
								car_speed_l=car_speed_r=0;
								while(Stop_Flag == 0) Find();
									Moto_stop();
										car_speed_l=car_speed_r=0;
									HAL_Delay(500);
								Moto_stop();
									spin_Turn(back_180_l);
									Moto_stop();
										car_speed_l=car_speed_r=0;
									//HAL_Delay(200);
									//HAL_Delay(200);
									HAL_GPIO_WritePin(Red_LED_GPIO_Port,Red_LED_Pin,GPIO_PIN_SET);
									HAL_GPIO_WritePin(Green_LED_GPIO_Port,Green_LED_Pin,GPIO_PIN_SET);
									Do_count = 11;
								}
								else if(TargetRoom == 'F' || TargetRoom == 'H')
									{
										spin_Turn(fianl);
										spin_Turn(right_90_1);
									Moto_stop();
										car_speed_l=car_speed_r=0;
									//HAL_Delay(200);
									//HAL_Delay(200);
									Do_count++;
										//HAL_Delay(100);	//越长时间就前进越多
									}
							}
						}
						break;
								
						case 11:
						{
							if(Middle_Cooperate_Flag == 1)
							{
								//Moto_stop();
								
								Do_count = 9;
//								Spin_succeed_flag = 0;
//								Stop_Flag = 0;
//								car_speed_l=car_speed_r=0;
//								while(Stop_Flag == 0) Find();
//								Moto_stop();
							//	OLED_ShowNum(2,1,777,3);
							}	
							else{
								Moto_stop();
					//			OLED_Clear();
								OLED_ShowNum(4,1,666,3);
							}
						}
						break;
						
						case 9: //直走到终点
						{
								HAL_GPIO_WritePin(Red_LED_GPIO_Port,Red_LED_Pin,GPIO_PIN_RESET);
								HAL_GPIO_WritePin(Green_LED_GPIO_Port,Green_LED_Pin,GPIO_PIN_RESET);
								HAL_GPIO_WritePin(Red_LED_GPIO_Port,Red_LED_Pin,GPIO_PIN_SET);
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
								car_speed_l=car_speed_r=0;
								while(Stop_Flag == 0) Find();
							car_speed_l=car_speed_r=0;
								Moto_stop();
								//HAL_Delay(200);
								spin_Turn(fianl);
								//HAL_Delay(200);
								Do_count++;
						}
						break;
						
						case 10:
						{
							if(Spin_succeed_flag == 1)//送完药
							{
								humi=3;
								OneNet_SendData();
								HAL_Delay(1500);
								Turn=0;
								BL_Flag=3;
								Write_Data_2(0x91);
								LoadOrNot();
								Go_Flag = 0;
								Far_Back_Flag = 1;
								Load_flag = 2;
							}
						}
						break;
												
					}
				}
			}
			/**********************************送药完毕返回**********************************/
			else if(Load_flag == 2 || Far_Back_Flag == 1)		// 全部房间都可返回了，就是远端的三叉路口那可能有时候会出现问题。
			{
				if(TargetRoom == 'A')
				{
					switch(Do2_count)
					{
						case 0:
						{
							HAL_GPIO_WritePin(Red_LED_GPIO_Port,Red_LED_Pin,GPIO_PIN_RESET);
							humi=4;
							OneNet_SendData();
							HAL_Delay(1500);
							car_speed_l=car_speed_r=0;
							HAL_Delay(200);
							spin_Turn(back_180_l);
							HAL_Delay(200);
							Do2_count++;
						}
						break;
						
						case 1:
						{
							if(Spin_succeed_flag == 1)//转弯成功
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
								while(Stop_Flag == 0) 
								{
									
									//HAL_Delay(200);
									Find();
									
								}
								//HAL_Delay(200);
								Do2_count++;	
							}
						}
						break;
							
						case 2:
						{
							if(Stop_Flag ==1)
							{
								spin_Turn(right_90);
								HAL_Delay(200);
								Do2_count++;
							}
						}
						break;
							 
						case 3:
						{
							if(Spin_succeed_flag == 1)
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
								while(Stop_Flag == 0) Find();
							//	HAL_Delay(200);
								spin_Turn(ojbk);
								Do2_count++;
							}
						}
						break;
						
						case 4:
						{
							
							HAL_GPIO_WritePin(Green_LED_GPIO_Port,Green_LED_Pin,GPIO_PIN_SET);
							if(Spin_succeed_flag == 1)
							{
								//清空标志位,停下小车
								Turn=0;
								Moto_stop();
								car_speed_l=car_speed_r=0;
							}
						}
						break;
					}
				}
				else if(TargetRoom == 'B')
				{
					switch(Do2_count)
					{
						case 0:
						{
							HAL_GPIO_WritePin(Red_LED_GPIO_Port,Red_LED_Pin,GPIO_PIN_RESET);
							humi=4;
							OneNet_SendData();
							HAL_Delay(1500);
							car_speed_l=car_speed_r=0;
							HAL_Delay(200);
							spin_Turn(back_180_l);
							HAL_Delay(200);
							Do2_count++;
						}
						break;
						
						case 1:
						{
							if(Spin_succeed_flag == 1)//转弯成功
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
								while(Stop_Flag == 0) 
								{
									
									//HAL_Delay(200);
									Find();
									
								}
								//HAL_Delay(200);
								Do2_count++;	
							}
						}
						break;
							
						case 2:
						{
							if(Stop_Flag ==1)
							{
								spin_Turn(left_90);
								HAL_Delay(200);
								Do2_count++;
							}
						}
						break;
							 
						case 3:
						{
							if(Spin_succeed_flag == 1)
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
								while(Stop_Flag == 0) Find();
							//	HAL_Delay(200);
								spin_Turn(ojbk);
								Do2_count++;
							}
						}
						break;
						
						case 4:
						{
							HAL_GPIO_WritePin(Green_LED_GPIO_Port,Green_LED_Pin,GPIO_PIN_SET);
							if(Spin_succeed_flag == 1)
							{
								//清空标志位,停下小车
								Turn=0;
								Moto_stop();
								car_speed_l=car_speed_r=0;
							}
						}
						break;
					}
				}
				else if(TargetRoom == 'C')  //中端病房   这两个与近端病房的差别只是在最后直走的时候走多了几十cm而已
				{
					switch(Do2_count)
					{
						
						case 0:
						{
							HAL_GPIO_WritePin(Red_LED_GPIO_Port,Red_LED_Pin,GPIO_PIN_SET);
							humi=4;
							OneNet_SendData();
							HAL_Delay(1500);
							car_speed_l=car_speed_r=0;
							HAL_Delay(300);
							spin_Turn(back_180_l);
							HAL_Delay(200);
							Do2_count++;
						}
						break;
						
						case 1:
						{
							if(Spin_succeed_flag == 1)
							{		 
								Stop_Flag = 0;	
								while(Stop_Flag==0)Find();
								HAL_Delay(200);
								Do2_count++;
							}
						}
						break;	
						
						case 2:
						{
							if(Stop_Flag ==1)
							{
								spin_Turn(right_90 );
								HAL_Delay(200);
								Spin_succeed_flag=1;
								Do2_count++;
							}
						}
						break;
						
						case 3:
						{
							if(Spin_succeed_flag == 1)
							{
								Stop_Flag = 0;	
								while(Stop_Flag == 0) Find();
								
								//HAL_Delay(200);
								spin_Turn(straight);
								//HAL_Delay(200);
								Do2_count+=2;
								Stop_Flag = 1;	
							}
						}
						break;	
						 
						case 5:
						{
							if(Stop_Flag ==1)
							{
								Stop_Flag = 0;	
								while(Stop_Flag == 0) Find();
								HAL_Delay(200);
								Do2_count++;
							}
						}
						break;

						case 6:
						{
							HAL_GPIO_WritePin(Green_LED_GPIO_Port,Green_LED_Pin,GPIO_PIN_SET);
							if(Stop_Flag ==1)
							{
								spin_Turn(ojbk);
								Moto_stop();
								car_speed_l=car_speed_r=0;
										humi=5;
							OneNet_SendData();
							}
						}
						break;
					}
				}
				else if(TargetRoom == 'D')  
				{
					switch(Do2_count)
					{
						case 0:
						{			
							HAL_GPIO_WritePin(Red_LED_GPIO_Port,Red_LED_Pin,GPIO_PIN_RESET);
							humi=4;
							OneNet_SendData();
							HAL_Delay(1500);
							car_speed_l=car_speed_r=0;
							HAL_Delay(200);
							spin_Turn(back_180_r);
							HAL_Delay(300);
							Do2_count++;
						}
						break;
						
						case 1:
						{
							if(Spin_succeed_flag == 1)
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
								HAL_Delay(200);
								while(Stop_Flag == 0) Find();  
								HAL_Delay(200);  
								Do2_count++;
							}
						}
						break;
							
						case 2:
						{
							if(Stop_Flag ==1)
							{
								spin_Turn(left_90);
								HAL_Delay(200);
								Do2_count++;
							}
						}
						break;

						case 3:
						{
							if(Spin_succeed_flag == 1)
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
								HAL_Delay(200);
								while(Stop_Flag == 0) Find();
								//HAL_Delay(200);
								Do2_count+=2;
							}
						}
						break;
							
						case 5:
						{
							if(Stop_Flag ==1)
							{
								Stop_Flag = 0;	
								while(Stop_Flag == 0) Find();
								//HAL_Delay(200);
								Do2_count++;
							}
						}
						break;
							
						case 6:
						{
							HAL_GPIO_WritePin(Green_LED_GPIO_Port,Green_LED_Pin,GPIO_PIN_SET);
					
							if(Stop_Flag ==1)
							{
								spin_Turn(ojbk);
								Moto_stop();
								car_speed_l=car_speed_r=0;
								humi=5;
							OneNet_SendData();
							}
						}
						break;	
					}
				}
				else if(TargetRoom == 'E')   //远端病房   四个只是在两个路口的转向不同而已
				{
					switch(Do2_count)
					{
						case 0:
						{
							HAL_GPIO_WritePin(Red_LED_GPIO_Port,Red_LED_Pin,GPIO_PIN_RESET);
							humi=4;
							OneNet_SendData();
							HAL_Delay(1500);
							car_speed_l=car_speed_r=0;
							//HAL_Delay(200);
							spin_Turn(back_180_r);
							HAL_Delay(300);
							Do2_count++;
						}
						break;
						
						case 1:
						{
							if(Spin_succeed_flag == 1)
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
								while(Stop_Flag == 0) Find();
								Moto_stop();
								//HAL_Delay(200);
								Do2_count++;	
							}
						}
						break;
						
						case 2:
						{
							if(Stop_Flag ==1)
							{
								spin_Turn(right_90);
								//HAL_Delay(200);
								Do2_count++;
							}
						}
						break;
						
						case 3:
						{
							if(Spin_succeed_flag == 1)
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
								while(Stop_Flag == 0) Find();
								Moto_stop();
								//HAL_Delay(200);
								Do2_count++;
							}
						}
						break;
						
						case 4:
						{
							if(Stop_Flag == 1)
							{
								spin_Turn(right_90);
								//HAL_Delay(200);
								Do2_count++;
							}
						}
						break;
							
						case 5:
						{
							if(Spin_succeed_flag == 1)
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;	
								while(Stop_Flag == 0) Find();
								Moto_stop();
								//HAL_Delay(200);
								
								spin_Turn(straight);
								Stop_Flag = 1;
								HAL_Delay(200);
								Do2_count++;
							}
						}
						break;
							
						case 6:
						{
							HAL_GPIO_WritePin(Green_LED_GPIO_Port,Green_LED_Pin,GPIO_PIN_SET);
							humi=5;
							OneNet_SendData();
							Turn=0;
							Moto_stop();
							car_speed_l=car_speed_r=0;
						}
						break;	
					}
				}
				
				else if(TargetRoom == 'F')
				{
					switch(Do2_count)
					{
						case 0:
						{
							HAL_GPIO_WritePin(Red_LED_GPIO_Port,Red_LED_Pin,GPIO_PIN_RESET);
							humi=4;
							OneNet_SendData();
							HAL_Delay(1500);
							car_speed_l=car_speed_r=0;
							HAL_Delay(200);
							spin_Turn(back_180_r);
							HAL_Delay(300);
							Do2_count++;
						}
						break;
						
						case 1:
						{
							if(Spin_succeed_flag == 1)
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
								while(Stop_Flag == 0) Find();
								//HAL_Delay(200);
								Do2_count++;   
							}
						}
						break;
							
						case 2:
						{
							if(Stop_Flag ==1)
							{
								spin_Turn(left_90);
								//HAL_Delay(200);
								Do2_count++;
							}
						}
						break;
							
						case 3:
						{
							if(Spin_succeed_flag == 1)
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
								while(Stop_Flag == 0) Find();
								//HAL_Delay(200);
								Do2_count++;
							}
						}
						break;
							
						case 4:
						{
							if(Stop_Flag == 1)
							{
								spin_Turn(right_90);
								//HAL_Delay(200);
								Do2_count++;
							}
						}
						break;
							
						case 5:
						{
							if(Spin_succeed_flag == 1)
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;	
								while(Stop_Flag == 0) Find();
								//HAL_Delay(200);
								
								spin_Turn(straight);
								Stop_Flag = 1;
								HAL_Delay(200);
								Do2_count++;
							}
						}
						break;
						
						case 6:
						{
							HAL_GPIO_WritePin(Green_LED_GPIO_Port,Green_LED_Pin,GPIO_PIN_SET);
							humi=5;
							OneNet_SendData();
							Turn=0;
							Moto_stop();
							car_speed_l=car_speed_r=0;
						}
						break;
					}
				}
				
				else if(TargetRoom == 'G')
				{
					switch(Do2_count)
					{
						case 0:
						{
							HAL_GPIO_WritePin(Red_LED_GPIO_Port,Red_LED_Pin,GPIO_PIN_RESET);
							humi=4;
							OneNet_SendData();
							HAL_Delay(1500);
							car_speed_l=car_speed_r=0;
							HAL_Delay(200);
							spin_Turn(back_180_r);
							HAL_Delay(300);
							Do2_count++;
						}
						break;
						
						case 1:
						{
							if(Spin_succeed_flag == 1)
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
								car_speed_l=car_speed_r=0;
								while(Stop_Flag == 0) Find();
								//HAL_Delay(200);
								Do2_count++;	  
							}
						}
						break;
							
						case 2:
						{
							if(Stop_Flag == 1)
							{
								spin_Turn(right_90);
								Moto_stop();
								//HAL_Delay(200);
								Do2_count++;
							}
						}
						break;
							 
						case 3:
						{
							if(Spin_succeed_flag == 1)
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;	
								while(Stop_Flag == 0) Find();
								//HAL_Delay(200);
								Do2_count++;
							}
						}
						break;
							
						case 4:
						{
							if(Stop_Flag == 1)
							{
								spin_Turn(left_90);
								Moto_stop();
								//HAL_Delay(200);
								Do2_count++;
							}
						}
						break;
							
						case 5:
						{
							if(Spin_succeed_flag == 1)
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
								while(Stop_Flag == 0) Find();
								//HAL_Delay(200);
								
								spin_Turn(straight);
								Stop_Flag = 1;
								HAL_Delay(200);
								Do2_count++;
							}
						}
						break;
							
						case 6:
						{
							HAL_GPIO_WritePin(Green_LED_GPIO_Port,Green_LED_Pin,GPIO_PIN_SET);
							humi=5;
							OneNet_SendData();
							Turn=0;
							Moto_stop();
							car_speed_l=car_speed_r=0;
						}
						break;
					}
				}
				
				else if(TargetRoom == 'H')
				{
					switch(Do2_count)
					{
						case 0:
						{
							HAL_GPIO_WritePin(Red_LED_GPIO_Port,Red_LED_Pin,GPIO_PIN_RESET);
							humi=4;
							OneNet_SendData();
							HAL_Delay(1500);
							car_speed_l=car_speed_r=0;
							HAL_Delay(200);
							spin_Turn(back_180_r);
							HAL_Delay(300);
							Do2_count++;
						}
						break;
						
						case 1:
						{
							if(Spin_succeed_flag == 1)
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
								while(Stop_Flag == 0) Find();
								//HAL_Delay(200);
								Do2_count++;
							}
						}
						break;
							
						case 2:
						{
							if(Stop_Flag == 1)
							{
								spin_Turn(left_90);    //此时Spin_succeed_flag== 0 ,完成后自动置1
								Moto_stop();
								//HAL_Delay(200);
								Do2_count++;
							}
						}
						break;
							 
						case 3:
						{
							if(Spin_succeed_flag == 1)
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;	
								while(Stop_Flag == 0) Find();
								//HAL_Delay(200);
								Do2_count++;
							}
						}
						break;
							
						case 4:
						{
							if(Stop_Flag == 1)
							{
								spin_Turn(left_90);
								Moto_stop();
								//HAL_Delay(200);
								Do2_count++;
							}
						}
						break;
							
						case 5:
						{
							if(Spin_succeed_flag == 1)
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
								while(Stop_Flag == 0) Find();
								//HAL_Delay(200);
								
								spin_Turn(straight);
								HAL_Delay(200);
								Do2_count++;
								Stop_Flag = 1;	
							}
						}
						break;
						
						case 6:
						{
							HAL_GPIO_WritePin(Green_LED_GPIO_Port,Green_LED_Pin,GPIO_PIN_SET);
							humi=5;
							Turn=0;
							Moto_stop();
							car_speed_l=car_speed_r=0;
							OneNet_SendData();
							HAL_Delay(1500);
						}
						break;	
					}
				}
			}
		}
	
	}
			

  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

extern uint8_t intersection;
void Find(void)
{
	SR04_GetData();
//	OLED_ShowString(4,1,"Dist:");
//	OLED_ShowNum(4,6,high_time,3);
	
	Spin_start_flag = 0;			//旋转开始标志位
	Spin_succeed_flag = 0;		//旋转成功标志位
	Find_Line_Begins();				//开始巡线
	if (intersection == 1)
	{
		Write_Data_2(0x90);
		Stop_Flag = 1;					//遇到路口停止
		intersection = 0;
		OUTPUT1 = OUTPUT2 = 0;
	}
	else if(high_time<=100)
	{
		car_speed_r=car_speed_l=0;
		Moto_stop();
		OUTPUT1 = OUTPUT2 = 0;
		HAL_Delay(100);
	}
		
}

extern char Load_mode;
void LoadOrNot()
{
	if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)==0)
	{
		NotLoadCount = 0;
		if(Load_flag==0)
		{
			LoadCount++;
			if(LoadCount > 3) //3次程序执行到这之后再次判断
			{
				Load_flag = 1;
				Load_mode=1;
			}
		}
	}
	else if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)==1)
	{
		LoadCount = 0;
		
		if(Load_flag == 1)			//必须先装载过药品的情况下才能判断是否拿走药品
		{
			NotLoadCount++;
			if(NotLoadCount > 3)	//3次程序执行到这之后再次判断
			{
				Load_flag = 2;
			}
		}
	}
}

//void XunJi()
//{
//		OLED_ShowSignedNum(2,1,True_Speed_L,3);
//		OLED_ShowSignedNum(2,5,True_Speed_R,3);
//		Get_Track_Dates(Track_Dates);
//		error=Get_Track_Errors(Track_Dates);
//	
//		for(i=0;i<7;i++)
//			OLED_ShowNum(3,i+1,Track_Dates[i],1);
//		
//		PID_jc1=Position_PI (True_Speed_L,10);
//		PID_jc2=Position_PI (True_Speed_R,10);
//		PID_cs=Position_PD(error,0);
//		Xianfu_PID();

//}

void spin_Turn(spin_dir_t zhuanxiang) //其实转向环并不需要很精准，转弯后直接用直走时的巡线函数回正车身就好
{
	Stop_Flag = 0;  //执行转弯时，将直走完成的标志位清零. 即如果上一次是直行，这次是转弯，则不用在业务代码里手动置位

	Spin_start_flag = 1;
	Spin_succeed_flag = 0;
	/**************要是转弯后不能回到线上，可以改这里的转向系数**************/
	if (zhuanxiang == left_90)  //openmv识别到需要往左边病房走
	{
		car_speed_r=car_speed_l=0;
		Moto_stop();
		
		car_speed_r=car_speed_l=9;
		Moto_forward_run();
		HAL_Delay(720);		//越长时间就前进越多
		//Moto_forward_run();
		car_speed_r=car_speed_l=9;
		Moto_turn_left();
		HAL_Delay(750);		//越长时间就转越多
		car_speed_l=car_speed_r=0;
		//HAL_Delay(200);
	}
	
	if (zhuanxiang == left_90_1)  //openmv识别到需要往左边病房走
	{
		car_speed_r=car_speed_l=0;
		Moto_stop();
		
		car_speed_r=car_speed_l=9;
		Moto_forward_run();
		HAL_Delay(500);		//越长时间就前进越多
		//Moto_forward_run();
		car_speed_r=car_speed_l=9;
		Moto_turn_left();
		HAL_Delay(750);		//越长时间就转越多
		car_speed_l=car_speed_r=0;
		//HAL_Delay(200);
	}
	
	else if (zhuanxiang == right_90)  //openmv识别到需要往右边病房走
	{
		car_speed_l=car_speed_r=9;
		Moto_forward_run();
		HAL_Delay(580);
		car_speed_l=car_speed_r=9;
		Moto_turn_right();
		HAL_Delay(720);
		car_speed_l=car_speed_r=0;
		//HAL_Delay(200);  
	}
	
	else if (zhuanxiang == right_90_1)  //openmv识别到需要往右边病房走
	{
		car_speed_l=car_speed_r=9;
		Moto_forward_run();
		HAL_Delay(610);
		car_speed_l=car_speed_r=9;
		Moto_turn_right();
		HAL_Delay(700);
		car_speed_l=car_speed_r=0;
		//HAL_Delay(200);  
	}
	
	else if (zhuanxiang == back_180_r)
	{
		Moto_forward_run();
		car_speed_l=car_speed_r=15;
		HAL_Delay(200);
		Moto_turn_right();
		HAL_Delay(900);
		Moto_forward_run();
		car_speed_l=car_speed_r=0;
		Moto_stop();
		HAL_Delay(100);
	}

	else if (zhuanxiang == back_180_l)
	{
		car_speed_l=car_speed_r=15;
		Moto_forward_run();
		HAL_Delay(200);
		Moto_turn_left();
		HAL_Delay(974);
		car_speed_l=car_speed_r=0;
		Moto_forward_run();
		Moto_stop();
		HAL_Delay(100);
	}
	else if (zhuanxiang == straight)
	{
		Moto_forward_run();
		car_speed_l=10;
		car_speed_r=10;
		HAL_Delay(400);
		car_speed_r=car_speed_l=0;
	}
	else if (zhuanxiang == fianl)
	{
		car_speed_l=car_speed_r=10;
		Moto_forward_run();
		HAL_Delay(100);
		car_speed_l=car_speed_r=0;
		Load_mode=0;
	}
	else if (zhuanxiang == ojbk)
	{
		car_speed_l=car_speed_r=0;
		Moto_stop();
		
		HAL_Delay(10);
//		Moto_turn_right();
//		HAL_Delay(1600);
//		Moto_forward_run();
//		HAL_Delay(300);
	}
	Spin_succeed_flag = 1;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
