/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "encoder.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

typedef enum
{
	left_90,
	right_90,
	back_180_l,
	back_180_r,
	straight,
	fianl,
	ojbk
}spin_dir_t;	//ת�亯�����������

uint8_t Spin_start_flag, Spin_succeed_flag, Stop_Flag;//��ת��ʼ��־λ  ��ת�ɹ���־λ  ·��ֹͣ��־λ
uint8_t Do_count = 0, Do2_count = 0;	//�������
__IO uint16_t SendTime = 0;						//���PID����ʱ�����
float Velocity,Turn;									//�ٶ�  ת��״̬
uint8_t car_speed_l=0,car_speed_r=0;	//���õ�����ٶ�
char show[10];												//����OLED��Ҫ��ʾ���ַ���
extern uint8_t intersection;					//·��ʶ���־λ
extern uint8_t LoadCount,NotLoadCount;//װ��ҩƷʱ�����  ����ҩƷʱ�����
extern uint16_t lukou_straight; 
void Find(void);											//Ѳ�ߺ���
void spin_Turn(spin_dir_t zhuanxiang);//ת�亯��
char Load_mode;												//ҩƷװ�ر�־λ
uint8_t key_num=0;										//����ɨ���־λ
uint8_t Time_temp=0;
uint8_t BL_Flag=0;
uint8_t Far_Flag = 0;
uint8_t Far_Back_Flag = 1;
extern char BL_Num;

//�����ã�
uint8_t sendBuf_test[4];					//���ڷ��͵�����

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

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
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART6_UART_Init();
  MX_TIM10_Init();
  MX_TIM9_Init();
  /* USER CODE BEGIN 2 */

	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);	
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);	
	TIM1->CCR1 = 200;
	HAL_UART_Receive_IT(&huart1, &Openmv_Receive_Buffer[0], 1);
	HAL_UART_Receive_IT(&huart6, &Bluetooth_Receive_Buffer[0], 1);

	HAL_Delay(500);//�ȴ�ϵͳ��ʼ��
	OLED_Init();
	HAL_Delay(500);
	
	HAL_UART_Receive_IT(&huart2,(uint8_t *)&BL_Num, 1);
	hhSerialSendByte('S');
//	OLED_Clear();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
	Moto_forward_run();
	HAL_GPIO_WritePin(Red_LED_GPIO_Port,Red_LED_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(Green_LED_GPIO_Port,Green_LED_Pin,GPIO_PIN_SET);
	HAL_Delay(300);
	HAL_GPIO_WritePin(Red_LED_GPIO_Port,Red_LED_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Green_LED_GPIO_Port,Green_LED_Pin,GPIO_PIN_RESET);
	HAL_Delay(300);
	HAL_GPIO_WritePin(Red_LED_GPIO_Port,Red_LED_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(Green_LED_GPIO_Port,Green_LED_Pin,GPIO_PIN_SET);
	HAL_Delay(300);
	HAL_GPIO_WritePin(Red_LED_GPIO_Port,Red_LED_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Green_LED_GPIO_Port,Green_LED_Pin,GPIO_PIN_RESET);
	HAL_Delay(300);

	while (0)
	{
		//Find_Line_Begins();
//		HAL_GPIO_WritePin(Red_LED_GPIO_Port,Red_LED_Pin,GPIO_PIN_SET);
//		HAL_GPIO_WritePin(Green_LED_GPIO_Port,Green_LED_Pin,GPIO_PIN_RESET);
//		HAL_Delay(1000);
//		HAL_GPIO_WritePin(Red_LED_GPIO_Port,Red_LED_Pin,GPIO_PIN_RESET);
//		HAL_GPIO_WritePin(Green_LED_GPIO_Port,Green_LED_Pin,GPIO_PIN_SET);
//		HAL_Delay(1000);
		//sprintf((char *)sendBuf_test, "{B%f:%f:%f}$", 15.0, encoder_mlist[LEFT_REAR].speed,encoder_mlist[RIGHT_REAR].speed);
		//HAL_UART_Transmit(&huart1, (uint8_t*)sendBuf_test, 36, 4);
		if(BL_Num == 'C')
		{
			HAL_GPIO_WritePin(Red_LED_GPIO_Port,Red_LED_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(Green_LED_GPIO_Port,Green_LED_Pin,GPIO_PIN_SET);
		}
	}

  while (1)
	{
		OLED_ShowNum(16,2,Spin_succeed_flag,2,8);
		OLED_ShowChar(16,4,BL_Num,12);
		OLED_ShowNum(16,3,car_speed_r,2,8);
		OLED_ShowNum(32,3,car_speed_l,2,8);
		
//		hhSerialSendByte('B');
		
		if(SendTime >= 10)	//SendTimeÿ10ms +1
		{
			Time_temp++;
			if(Time_temp>=100)
			{
				if(Load_flag==0)
				hhSerialSendByte('S');
			}
			
			SendTime = 0;
			SendDataToOpenmv();						//����̫�죬����ᳬ��openmv�Ĵ��ڽ������ݻ�����
			if(Load_mode==0) LoadOrNot();	//�ж�ҩ���Ƿ�װ��
			oled_show();									//��ʾ����
		}
		if(Load_flag == 1&&TASK == 1) 	//ѡ������һ || ҩ���Ѿ�����
		{
			SetTargetRoom();LoR=0;							//����ʶ�𵽵ķ����
		}
		
		if(BL_Num == 'C')
		{
			HAL_GPIO_WritePin(Red_LED_GPIO_Port,Red_LED_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(Green_LED_GPIO_Port,Green_LED_Pin,GPIO_PIN_SET);
			Far_Back_Flag = 0;
		}
		if(BL_Num == 'F')
		{
			Far_Back_Flag = 1;
		}
		
		if(TASK == 2)    
	 	{
			
			oled_show();							//OLEDˢ��
			if(Load_flag == 1)    		//����ҩ��
			{
				
				if(TargetRoom == 'A') 	//���λ�����ΪA��������Ϊ1
				{
					switch(Do_count)
					{
						case 0:
						{
							HAL_GPIO_WritePin(Red_LED_GPIO_Port,Red_LED_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(Green_LED_GPIO_Port,Green_LED_Pin,GPIO_PIN_RESET);
							hhSerialSendByte('P');
							HAL_Delay(200);
							Stop_Flag = 0;							//Ѳ��ͣ����־λ
							Spin_succeed_flag = 0;			//ת��ɹ���־λ
							while(Stop_Flag==0)Find();  //Ѳ��(��ʱ Stop_Flag ==0  ��ɺ��Զ���1)
							HAL_Delay(200);
							Do_count++;	
						}
						break;
						
						case 1:
						{
							if(Stop_Flag == 1)		//ȷ����ʱѲ�߹ر�
							{
								spin_Turn(left_90);	//��ת  ��ʱSpin_succeed_flag== 0 ,��ɺ��Զ���1
								//Spin_succeed_flag = 1;
								HAL_Delay(200);
								Do_count++;
							}
						}
						break;
						
						case 2:
						{
							if(Spin_succeed_flag == 1)	//��һ����ת���Spin_succeed_flag����1
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
								while(Stop_Flag==0) Find();
								HAL_Delay(200);
								Do_count++;
							}
						}
						break;
							
						case 3:
						{
							if(Stop_Flag == 1)	//���յ�
							{							
								spin_Turn(fianl);	//ǰ��һ�ξ���
							}
							HAL_Delay(200);
						}
						break;
					}
				}
				else if(TargetRoom == 'B')	//B�ŷ�
				{
					switch(Do_count)
					{
						case 0:
						{
							HAL_GPIO_WritePin(Red_LED_GPIO_Port,Red_LED_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(Green_LED_GPIO_Port,Green_LED_Pin,GPIO_PIN_RESET);
							Spin_succeed_flag = 0;
							Stop_Flag = 0;
							while(Stop_Flag == 0) Find();
							HAL_Delay(200);				
							Do_count++;	
						}
						break;
						
						case 1:
						{
							if(Stop_Flag ==1)
								spin_Turn(right_90);
							HAL_Delay(200);			
							
							if(Spin_succeed_flag == 1)
							{
								HAL_Delay(200);
								Do_count++;
							}
						}
						break;
							
						case 2:
						{
							if(Spin_succeed_flag == 1)
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
								while(Stop_Flag == 0) Find();
								HAL_Delay(200);			
								spin_Turn(fianl);
								HAL_Delay(200);
								Do_count++;
							}
						}
						break;
						
						case 3:
						{
							if(Spin_succeed_flag ==1)
							{
								car_speed_l = car_speed_r = 0;
							}
						}
						break;
					}			 
				}
				//�ж˲�����Զ�˲�����Ҫ�����ض���־λ					
				else      //TargetRoom == 'C' || TargetRoom == 'D' || TargetRoom == 'E' || TargetRoom == 'F' || TargetRoom == 'G' || TargetRoom == 'H'
				{
					OLED_ShowNumber(10,3, Do_count,3,4);
					switch(Do_count)	//ֱ�ߵ���һ��·��
					{
						case 0: 
						{
							hhSerialSendByte('P');
							hhSerialSendByte('P');
							HAL_GPIO_WritePin(Red_LED_GPIO_Port,Red_LED_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(Green_LED_GPIO_Port,Green_LED_Pin,GPIO_PIN_RESET);
							Spin_succeed_flag = 0;
							Stop_Flag = 0;
							while(Stop_Flag == 0) Find();
							
							Do_count++;
							SendDataToOpenmv();  //������ʶ��
						}
						break;
						
						case 1:
						{
							if(Stop_Flag ==1)				//������һ��·��
							{
								hhSerialSendByte('P');
								hhSerialSendByte('P');
								spin_Turn(straight);	//��ʱSpin_succeed_flag== 0 ,��ɺ��Զ���1
								Do_count++;
								SendDataToOpenmv();	//��ģ��ƥ������
							}
						}
						
						case 2:
						{
							if(Spin_succeed_flag == 1)	//��һ��·�����ڶ���·��ֱ��
							{
								Spin_succeed_flag = 0;
								lukou_straight=1;
								Stop_Flag = 0;
								car_speed_l=car_speed_r=0;
								while(lukou_straight <= 150) Find();//lukou_straight ÿ10ms +1
								
								//Ѳ��1.2s��ͣ��1.5sʶ������,�ټ���Ѳ�ߵ���һ��·��
								lukou_straight=0;
								Turn=0;
								Moto_stop();
								car_speed_l=car_speed_r=0;
								//HAL_Delay(1000);
								HAL_Delay(500);
								Moto_forward_run();
								
								intersection = 0;
								Stop_Flag = 0;
								Spin_succeed_flag = 0;
								while(Stop_Flag == 0) Find();
								HAL_Delay(200);
								Do_count++;
							 }
						 }
						
						case 3:	//�ڶ���·��		�ж��Ƿ�ʶ�����֣��Ҹ����ֵ�λ��ƫ����ƫ��
						{
							if(Stop_Flag == 1)
							{
								if(RoomNum == TargetNum && LoR == 1)	//ʶ����Ŀ������, ����λ��ƫ��
								{
									Do_count = 8; 											//����ֱ����ת��ֱ�е������ſڵĺ���
									TargetRoom = 'C';
								}
								else if(RoomNum == TargetNum && LoR == 2)		//ʶ��Ŀ������, ����λ��ƫ��
								{
									Do_count = 8;			//����ֱ����ת��ֱ�е������ſڵĺ���
									TargetRoom = 'D';
								}	
								if(LoR == 0)		//ûʶ�𵽾�ֱ�ߵ�������·��ֹͣ
								{
									spin_Turn(straight);
									lukou_straight=1;
									HAL_Delay(200);
									car_speed_l=car_speed_r=0;
									while(lukou_straight <= 150) Find();
									//HAL_Delay(200);
									
									lukou_straight=0;
									Moto_stop();
									car_speed_l=car_speed_r=0;
									//HAL_Delay(1000);
									HAL_Delay(500);
									Moto_forward_run();
									
									intersection = 0;
									Stop_Flag = 0;
									Spin_succeed_flag = 0;
									car_speed_l=car_speed_r=0;
									while(Stop_Flag == 0) Find();
									HAL_Delay(200);
									Do_count++;
								}	
							}
						}
						break;
								
						case 4:	//������·��׼��ת��
						{
							if(Stop_Flag == 1)
							{
								if(RoomNum == TargetNum && LoR == 1)   //ʶ����Ŀ������, ����λ��ƫ��
								{
									Do_count++; 
									TargetRoom = 'E';		//�ȼٶ���E
									SendDataToOpenmv(); //�ر�ģ��ƥ������
									
								}
								else if(RoomNum == TargetNum && LoR == 2 )  //ʶ��Ŀ������, ����λ��ƫ��
								{
									Do_count++;
									TargetRoom = 'G';		//�ȼٶ���G
									SendDataToOpenmv(); //�ر�ģ��ƥ������
								}
							}
						}
						break;
								
						case 5://ת��
						{
							if(Stop_Flag == 1)
							{
								Do_count++;
								if(TargetRoom == 'E')
								{
									spin_Turn(left_90);
									HAL_Delay(200);
									LoR = 0;						//���Ԥת�䷽��
									SendDataToOpenmv(); //��ģ��ƥ������
								}
								else if(TargetRoom == 'G')
								{
									spin_Turn(right_90);
									HAL_Delay(200);
									LoR = 0;
									SendDataToOpenmv();
								}
							}
						}
						break;
						
						case 6: //ֱ�ߵ����ĸ�·��
						{
							if(Spin_succeed_flag == 1)
							{
								Do_count++;
								Stop_Flag = 0;	
								lukou_straight=1;
								car_speed_l=car_speed_r=0;
								while(lukou_straight <= 110) Find(); 
								//HAL_Delay(200);
								
								lukou_straight=0;
								Moto_stop();
								car_speed_l=car_speed_r=0;
								//HAL_Delay(1000);
								HAL_Delay(500);
								Moto_forward_run();
								
								intersection = 0;
								Stop_Flag = 0;
								Spin_succeed_flag = 0;
								car_speed_l=car_speed_r=0;
								while(Stop_Flag == 0) Find();
								HAL_Delay(200);
							}
						}
						break;
						
						case 7: //���ĸ�·��׼��ת��
						{
							if(Stop_Flag == 1)
							{
								if(TargetRoom == 'E')
								{
									if(RoomNum == TargetNum && LoR == 1)    //һ��ʱ����ʶ��RoomNum, ����λ��ƫ��
									{
										Do_count ++;  
										TargetRoom = 'E';
										SendDataToOpenmv();	//�ر�ģ��ƥ������	
									}										 
									else if(RoomNum == TargetNum && LoR == 2)//һ��ʱ����ʶ��RoomNum, ����λ��ƫ��
									{
										Do_count++;  
										TargetRoom = 'F';
										SendDataToOpenmv();	//�ر�ģ��ƥ������	
									}
									else
									{
										Turn=0;
									}
								}
								else if(TargetRoom == 'G')
								{
									if(RoomNum == TargetNum && LoR == 1)    //һ��ʱ����ʶ��RoomNum, ����λ��ƫ��
									{
										Do_count++; 
										TargetRoom = 'G';
										SendDataToOpenmv(); //�ر�ģ��ƥ������	
									}										 											 
									else if(RoomNum == TargetNum && LoR == 2)   //һ��ʱ����ʶ��RoomNum, ����λ��ƫ��
									{
										Do_count++;  
										TargetRoom = 'H';
										SendDataToOpenmv(); //�ر�ģ��ƥ������	
									}
									else
									{
										Turn=0;
									}
								}	
							}
						}
						break;
								
						case 8://���·��ת��
						{
							if(Stop_Flag == 1)
							{
								if(TargetRoom == 'C' || TargetRoom == 'E' || TargetRoom == 'G')
								{
									if(TargetRoom == 'E' || TargetRoom == 'G')
									{
										Moto_forward_run();
										car_speed_l=car_speed_r=15;
										HAL_Delay(100);	//Խ��ʱ���ǰ��Խ��
									}
									
									spin_Turn(left_90);
									HAL_Delay(200);
									HAL_Delay(200);
									Do_count++;
								}
								else if(TargetRoom == 'D' || TargetRoom == 'F' || TargetRoom == 'H')
								{
									if(TargetRoom == 'F' || TargetRoom == 'H')
									{
										Moto_forward_run();
										car_speed_l=car_speed_r=15;
										HAL_Delay(100);	//Խ��ʱ���ǰ��Խ��
									}
									spin_Turn(right_90);
									HAL_Delay(200);
									HAL_Delay(200);
									Do_count++;
								}
							}
						}
						break;
								
						case 9: //ֱ�ߵ��յ�
						{
							HAL_GPIO_WritePin(Red_LED_GPIO_Port,Red_LED_Pin,GPIO_PIN_SET);
							if(Spin_succeed_flag == 1)
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
								car_speed_l=car_speed_r=0;
								while(Stop_Flag == 0) Find();
								HAL_Delay(200);
								
								spin_Turn(fianl);
								HAL_Delay(200);
								Do_count++;
							}
							hhSerialSendByte('G');
							hhSerialSendByte('G');
							hhSerialSendByte('G');
						}
						break;
						
						case 10:
						{
							if(Spin_succeed_flag == 1)//����ҩ
							{
								Turn=0;
							}
						}
						break;								
					}
				}
			}
			/**********************************��ҩ��Ϸ���**********************************/
			else if(Load_flag == 2 )		// ȫ�����䶼�ɷ����ˣ�����Զ�˵�����·���ǿ�����ʱ���������⡣
			{
				HAL_GPIO_WritePin(Red_LED_GPIO_Port,Red_LED_Pin,GPIO_PIN_RESET);
				if(TargetRoom == 'A')
				{
					switch(Do2_count)
					{
						case 0:
						{
							spin_Turn(back_180_l);
							hhSerialSendByte('B');
							hhSerialSendByte('B');
							hhSerialSendByte('B');
							hhSerialSendByte('B');
							HAL_Delay(200);
							Do2_count++;
						}
						break;
						
						case 1:
						{
							if(Spin_succeed_flag == 1)//ת��ɹ�
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
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
								HAL_Delay(200);
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
								//��ձ�־λ,ͣ��С��
								Turn=0;
								Moto_stop();
								car_speed_l=car_speed_r=0;
							}
							//hhSerialSendByte('A');
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
							spin_Turn(back_180_r);
							HAL_Delay(200);
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
								while(Stop_Flag == 0) Find();
								HAL_Delay(200);			
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
								Turn=0;
								Moto_stop();
								car_speed_l=car_speed_r=0;
							}
						}
						break;
					}
				}
				else if(TargetRoom == 'C')  //�ж˲���   ����������˲����Ĳ��ֻ�������ֱ�ߵ�ʱ���߶��˼�ʮcm����
				{
					switch(Do2_count)
					{
						case 0:
						{
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
								hhSerialSendByte('Q');
								hhSerialSendByte('Q');
							}
						}
						break;
						
						case 3:
						{
							if(Spin_succeed_flag == 1)
							{
								hhSerialSendByte('Q');
								Stop_Flag = 0;	
								while(Stop_Flag == 0) Find();
								
								HAL_Delay(200);
								spin_Turn(straight);
								HAL_Delay(200);
								Do2_count++;
								Stop_Flag = 1;	
							}
						}
						break;	
						 
						case 4:
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

						case 5:
						{
							HAL_GPIO_WritePin(Green_LED_GPIO_Port,Green_LED_Pin,GPIO_PIN_SET);
							if(Stop_Flag ==1)
							{
								spin_Turn(ojbk);
								Moto_stop();
								car_speed_l=car_speed_r=0;
								hhSerialSendByte('Q');
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
							spin_Turn(back_180_r);
							HAL_Delay(200);
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
								hhSerialSendByte('Q');
								hhSerialSendByte('Q');
							}
						}
						break;

						case 3:
						{
							if(Spin_succeed_flag == 1)
							{
								hhSerialSendByte('Q');
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
								HAL_Delay(200);
								while(Stop_Flag == 0) Find();
								HAL_Delay(200);
								Do2_count++;
							}
						}
						break;
							
						case 4:
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
							
						case 5:
						{
							HAL_GPIO_WritePin(Green_LED_GPIO_Port,Green_LED_Pin,GPIO_PIN_SET);
							if(Stop_Flag ==1)
							{
								spin_Turn(ojbk);
								Moto_stop();
								car_speed_l=car_speed_r=0;
								hhSerialSendByte('Q');
							}
							
						}
						break;	
					}
				}
				else if(TargetRoom == 'E')   //Զ�˲���   �ĸ�ֻ��������·�ڵ�ת��ͬ����
				{
					switch(Do2_count)
					{
						case 0:
						{
							hhSerialSendByte('F');
							hhSerialSendByte('F');
							hhSerialSendByte('F');
							hhSerialSendByte('F');
							spin_Turn(back_180_r);
							car_speed_l=car_speed_r=0;
							HAL_Delay(400);
							Do2_count++;
						}
						break;
						
						case 1:
						{
							
							if(Spin_succeed_flag == 1)
							{
								hhSerialSendByte('B');
							hhSerialSendByte('B');
							hhSerialSendByte('B');
							hhSerialSendByte('B');
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
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
								HAL_Delay(200);
								Do2_count++;
							}
						}
						break;
						
						case 4:
						{
							if(Stop_Flag == 1)
							{
								spin_Turn(right_90);
								HAL_Delay(200);
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
								HAL_Delay(200);
								
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
							Turn=0;
							Moto_stop();
							car_speed_l=car_speed_r=0;
							hhSerialSendByte('Q');
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
							hhSerialSendByte('F');
							hhSerialSendByte('F');
							hhSerialSendByte('F');
							hhSerialSendByte('F');
							spin_Turn(back_180_l);
							car_speed_l=car_speed_r=0;
							HAL_Delay(400);
							Do2_count++;
						}
						break;
						
						case 1:
						{
							if(Spin_succeed_flag == 1)
							{
								hhSerialSendByte('B');
							hhSerialSendByte('B');
							hhSerialSendByte('B');
							hhSerialSendByte('B');
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
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
								while(Stop_Flag == 0) Find();
								HAL_Delay(200);
								Do2_count++;
							}
						}
						break;
							
						case 4:
						{
							if(Stop_Flag == 1)
							{
								spin_Turn(right_90);
								HAL_Delay(200);
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
								HAL_Delay(200);
								
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
							Turn=0;
							Moto_stop();
							car_speed_l=car_speed_r=0;
							hhSerialSendByte('Q');
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
							hhSerialSendByte('F');
							hhSerialSendByte('F');
							hhSerialSendByte('F');
							hhSerialSendByte('F');
							spin_Turn(back_180_r);
							car_speed_l=car_speed_r=0;
							HAL_Delay(400);
							Do2_count++;
						}
						break;
						
						case 1:
						{
							hhSerialSendByte('B');
							hhSerialSendByte('B');
							hhSerialSendByte('B');
							hhSerialSendByte('B');
							if(Spin_succeed_flag == 1)
							{
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
								car_speed_l=car_speed_r=0;
								while(Stop_Flag == 0) Find();
								HAL_Delay(200);
								Do2_count++;	  
							}
						}
						break;
							
						case 2:
						{
							if(Stop_Flag == 1)
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
								HAL_Delay(200);
								Do2_count++;
							}
						}
						break;
							
						case 4:
						{
							if(Stop_Flag == 1)
							{
								spin_Turn(left_90);
								HAL_Delay(200);
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
								HAL_Delay(200);
								
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
							Turn=0;
							Moto_stop();
							car_speed_l=car_speed_r=0;
							hhSerialSendByte('Q');
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
							hhSerialSendByte('F');
							hhSerialSendByte('F');
							hhSerialSendByte('F');
							hhSerialSendByte('F');
							spin_Turn(back_180_l);
							car_speed_l=car_speed_r=0;
							HAL_Delay(400);
							Do2_count++;
						}
						break;
						
						case 1:
						{
							if(Spin_succeed_flag == 1)
							{
								hhSerialSendByte('B');
							hhSerialSendByte('B');
							hhSerialSendByte('B');
							hhSerialSendByte('B');
								Spin_succeed_flag = 0;
								Stop_Flag = 0;
								while(Stop_Flag == 0) Find();
								HAL_Delay(200);
								Do2_count++;
							}
						}
						break;
							
						case 2:
						{
							if(Stop_Flag == 1)
							{
								spin_Turn(left_90);    //��ʱSpin_succeed_flag== 0 ,��ɺ��Զ���1
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
								HAL_Delay(200);
								Do2_count++;
							}
						}
						break;
							
						case 4:
						{
							if(Stop_Flag == 1)
							{
								spin_Turn(left_90);
								HAL_Delay(200);
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
								HAL_Delay(200);
								
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
							Turn=0;
							Moto_stop();
							car_speed_l=car_speed_r=0;
							hhSerialSendByte('Q');
						}
						break;	
					}
				}
			}
		}
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 84;
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
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
extern uint8_t intersection;
void Find(void)
{
	Spin_start_flag = 0;			//��ת��ʼ��־λ
	Spin_succeed_flag = 0;		//��ת�ɹ���־λ
	Find_Line_Begins();				//��ʼѲ��
	if (intersection == 1)
	{
		Stop_Flag = 1;					//����·��ֹͣ
		intersection = 0;
		car_speed_l = car_speed_r = 0;
	}
}

/******ת�����ʱ  Spin_start_flag == 0 && Spin_succeed_flag == 1  ********/
/*ת������ת90����ת90����ת180���������*/
void spin_Turn(spin_dir_t zhuanxiang) //��ʵת�򻷲�����Ҫ�ܾ�׼��ת���ֱ����ֱ��ʱ��Ѳ�ߺ�����������ͺ�
{
	Stop_Flag = 0;  //ִ��ת��ʱ����ֱ����ɵı�־λ����. �������һ����ֱ�У������ת�䣬������ҵ��������ֶ���λ

	Spin_start_flag = 1;
	Spin_succeed_flag = 0;
	/**************Ҫ��ת����ܻص����ϣ����Ը������ת��ϵ��**************/
	if (zhuanxiang == left_90)  //openmvʶ����Ҫ����߲�����
	{
		Moto_forward_run();
		car_speed_l=car_speed_r=15;
		HAL_Delay(1000);		//Խ��ʱ���ǰ��Խ��
		Moto_forward_run();
		Moto_turn_left();
		HAL_Delay(810);		//Խ��ʱ���תԽ��
		car_speed_l=car_speed_r=0;
	}
	else if (zhuanxiang == right_90)  //openmvʶ����Ҫ���ұ߲�����
	{
		Moto_forward_run();
		car_speed_l=car_speed_r=15;
		HAL_Delay(1000);
		Moto_forward_run();
		Moto_turn_right();
		HAL_Delay(810);
		car_speed_l=car_speed_r=0;
	}
	else if (zhuanxiang == back_180_r)
	{
		Moto_forward_run();
		car_speed_l=car_speed_r=15;
		HAL_Delay(300);
		Moto_turn_right();
		HAL_Delay(1600);
		Moto_forward_run();
		car_speed_l=car_speed_r=0;
		HAL_Delay(100);
	}

	else if (zhuanxiang == back_180_l)
	{
		Moto_forward_run();
		car_speed_l=car_speed_r=15;
		HAL_Delay(300);
		Moto_turn_left();
		HAL_Delay(1600);
		Moto_forward_run();
		car_speed_l=car_speed_r=0;
		HAL_Delay(100);
	}
	else if (zhuanxiang == straight)
	{
		Moto_forward_run();
		car_speed_l=15;
		car_speed_r=15;
		HAL_Delay(500);
	}
	else if (zhuanxiang == fianl)
	{
		Moto_forward_run();
		HAL_Delay(200);
//		car_speed_l=car_speed_r=15;
//		HAL_Delay(800);
		car_speed_l=car_speed_r=0;
		Load_mode=0;
	}
	else if (zhuanxiang == ojbk)
	{
		Moto_forward_run();
		car_speed_l=car_speed_r=15;
		HAL_Delay(600);
//		Moto_turn_right();
//		HAL_Delay(1600);
//		Moto_forward_run();
//		HAL_Delay(300);
	}
	Spin_succeed_flag = 1;
}

uint8_t LoadCount,NotLoadCount;//����ҩƷ�ļ���ֵ		����ҩƷ�ļ���ֵ
uint8_t Load_flag = 0; 				 //0��ʾ��ûװ��ҩƷ��1��ʾװ����ϣ�2��ʾҩƷ����
extern char Load_mode;
void LoadOrNot()
{
	if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5)==0)
	{
		NotLoadCount = 0;
		if(Load_flag==0)
		{
			LoadCount++;
			if(LoadCount > 3) //3�γ���ִ�е���֮���ٴ��ж�
			{
				Load_flag = 1;
				Load_mode=1;
			}
		}
	}
	else if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5)==1)
	{
		LoadCount = 0;
		
		if(Load_flag == 1)			//������װ�ع�ҩƷ������²����ж��Ƿ�����ҩƷ
		{
			NotLoadCount++;
			if(NotLoadCount > 3)	//3�γ���ִ�е���֮���ٴ��ж�
			{
				Load_flag = 2;
			}
		}
	}
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
