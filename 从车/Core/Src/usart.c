/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
#include "usart.h"

/* USER CODE BEGIN 0 */
#include "oled.h"
char BL_Num;
extern unsigned char esp8266_buf[512];
extern uint8_t rev_ok;
extern int8_t ok_2;
extern uint8_t BL_Rec_Flag; 
/* USER CODE END 0 */

UART_HandleTypeDef huart5;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart6;

/* UART5 init function */
void MX_UART5_Init(void)
{

  /* USER CODE BEGIN UART5_Init 0 */

  /* USER CODE END UART5_Init 0 */

  /* USER CODE BEGIN UART5_Init 1 */

  /* USER CODE END UART5_Init 1 */
  huart5.Instance = UART5;
  huart5.Init.BaudRate = 9600;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART5_Init 2 */

  /* USER CODE END UART5_Init 2 */

}
/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}
/* USART2 init function */

void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}
/* USART3 init function */

void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}
/* USART6 init function */

void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */
	HAL_UARTEx_ReceiveToIdle_IT(&huart6, esp8266_buf, 512);
  /* USER CODE END USART6_Init 2 */

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==UART5)
  {
  /* USER CODE BEGIN UART5_MspInit 0 */

  /* USER CODE END UART5_MspInit 0 */
    /* UART5 clock enable */
    __HAL_RCC_UART5_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**UART5 GPIO Configuration
    PC12     ------> UART5_TX
    PD2     ------> UART5_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* UART5 interrupt Init */
    HAL_NVIC_SetPriority(UART5_IRQn, 4, 0);
    HAL_NVIC_EnableIRQ(UART5_IRQn);
  /* USER CODE BEGIN UART5_MspInit 1 */

  /* USER CODE END UART5_MspInit 1 */
  }
  else if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PB6     ------> USART1_TX
    PB7     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PD5     ------> USART2_TX
    PD6     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */
    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**USART3 GPIO Configuration
    PC10     ------> USART3_TX
    PC11     ------> USART3_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* USART3 interrupt Init */
    HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspInit 1 */

  /* USER CODE END USART3_MspInit 1 */
  }
  else if(uartHandle->Instance==USART6)
  {
  /* USER CODE BEGIN USART6_MspInit 0 */

  /* USER CODE END USART6_MspInit 0 */
    /* USART6 clock enable */
    __HAL_RCC_USART6_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**USART6 GPIO Configuration
    PC6     ------> USART6_TX
    PC7     ------> USART6_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* USART6 interrupt Init */
    HAL_NVIC_SetPriority(USART6_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART6_IRQn);
  /* USER CODE BEGIN USART6_MspInit 1 */

  /* USER CODE END USART6_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==UART5)
  {
  /* USER CODE BEGIN UART5_MspDeInit 0 */

  /* USER CODE END UART5_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_UART5_CLK_DISABLE();

    /**UART5 GPIO Configuration
    PC12     ------> UART5_TX
    PD2     ------> UART5_RX
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_12);

    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_2);

    /* UART5 interrupt Deinit */
    HAL_NVIC_DisableIRQ(UART5_IRQn);
  /* USER CODE BEGIN UART5_MspDeInit 1 */

  /* USER CODE END UART5_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PB6     ------> USART1_TX
    PB7     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6|GPIO_PIN_7);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PD5     ------> USART2_TX
    PD6     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_5|GPIO_PIN_6);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */

  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();

    /**USART3 GPIO Configuration
    PC10     ------> USART3_TX
    PC11     ------> USART3_RX
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_10|GPIO_PIN_11);

    /* USART3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART3_IRQn);
  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART6)
  {
  /* USER CODE BEGIN USART6_MspDeInit 0 */

  /* USER CODE END USART6_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART6_CLK_DISABLE();

    /**USART6 GPIO Configuration
    PC6     ------> USART6_TX
    PC7     ------> USART6_RX
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_6|GPIO_PIN_7);

    /* USART6 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART6_IRQn);
  /* USER CODE BEGIN USART6_MspDeInit 1 */

  /* USER CODE END USART6_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

//Thru change 122-Finded_Flag,Num,130-TargetRoom,161-Num to 1,to test the code

int GetOpenmvDataCount  = 0;															//OpenMV���ݶ�ȡλ��
uint8_t  Num = 0, LoR = 0, Finded_flag = 0, FindTask = 0;	//ʶ�������  ʶ�����ֵ����ҷ���  �Ƿ�ʶ�𵽵�����  ʶ�𵽵�����
uint8_t LastNum;
uint8_t NumCount = 0;
uint8_t sendBuf[4];					//���ڷ��͵�����
uint8_t uart6_rxbuff;
uint8_t RoomNum, TargetNum;	//������  ������
uint8_t TASK=1;    					//���TASK���Դ����openmv����ֵopenmv�ϵ�FindTask������openmvģ��ƥ��Ĳ�ͬģʽ

char TargetRoom ='0';  //A, B, C, D, E, F, G, H;    //��˸��ַ���Ӧ�ŵ�ͼʵ�ʷ��䣬���������3��8�������ӦC-H

void Processing_Data(uint8_t RXdata);

void SendDataToOpenmv(void)
{
	uint8_t i;
	if(LoR==0)	
	//���Ϸ��͸�openmv �����ݵĴ��� (֡ͷ�� ģ��ƥ��ģʽѡ���־λ��ģʽ2����Ҫƥ������֣�֡β)   //����Ҫ�ܸߵķ���Ƶ��			
	{
		for(i = 0; i <= 4; i++)	//��TASK��TargetNum���һ���Է��͸�openmv
		{ 
			//sprintf((char *)sendBuf, "*%d%d&", TASK, TargetNum);
			HAL_UART_Transmit(&huart3, (uint8_t*)sendBuf, 4, 4);
		}
	}
	else
	{
		for(i = 0; i <= 4; i++)	//��TASK��TargetNum���һ���Է��͸�openmv
		{ 
			//sprintf((char *)sendBuf, "*%d%d&", 0, TargetNum);
			HAL_UART_Transmit(&huart3, (uint8_t*)sendBuf, 4, 4);
		}
	}
}

//�ȴ�openmvʶ��ָ������,������Ŀ�귿�䡣 ֻ���ڸ�λ��
void SetTargetRoom(void)
{
		//��ѰĿ�겡���ŵĻ��������ݣ���ת����ʼ����   //�������һ�δ�openmv���������ּ�ΪĿ�귿��
		//һ��ʼʶ��Ŀ�귿���*/  
		if(Finded_flag == 1)
		{
			 RoomNum = Num;
		}
		
		
		else if(Finded_flag == 0)
		{
			RoomNum = 0;   
			LastNum = 0;
			NumCount = 0;
		}
	
		
	 if(RoomNum ==  1) 
	 {
		 TargetRoom = 'A';
		 TASK = 2;
	 }
	 else if(RoomNum == 2)
	 {
		 TargetRoom = 'B';
		 TASK = 2;
			
	 }
	 else if(RoomNum >= 3)	//����else if(3 <= Num <= 8)
	 {
		 TargetRoom = 'G';
		 TASK = 2;
	 }

   switch(RoomNum)
		{
			case 1:
				TargetNum = 1;//���淿���,��ʶ������ʱ�Ż����
			break;
			case 2:
				TargetNum = 2;
			break;
			case 3:
				TargetNum = 3;
			break;
			case 4:
				TargetNum = 4;
			break;
			case 5:
				TargetNum = 5;
			break;
			case 6:
				TargetNum = 6;
			break;
			case 7:
				TargetNum = 7;
			break;
			case 8:
				TargetNum = 8;
			break;	 	 		
		}
	 //ʶ�𵽵�������3-8�� Ĭ���ȸ�  TargetRoom = RoomH
	 //����openmvʶ�𵽵�����,����ҩ�ĺ����������Ŀ��ֵ��ʵʱ����
}



void Openmv_Receive_Data(uint8_t com_data)
{
		uint8_t i;
		static uint8_t RxCounter1=0;//����
		static uint16_t RxBuffer1[10]={0};
		static uint8_t RxState = 0;	
		if(RxState==0&&com_data==0x2C)   //0x2c֡ͷ
		{			
			RxState=1;
			RxBuffer1[RxCounter1++]=com_data;  
		}
		else if(RxState==1&&com_data==0x12)  //0x12֡ͷ
		{
			RxState=2;
			RxBuffer1[RxCounter1++]=com_data;
		}		
		else if(RxState==2)
		{		 
			RxBuffer1[RxCounter1++]=com_data;
			if(RxCounter1>=10||com_data == 0x5B)  //RxBuffer1��������,�������ݽ���
			{
				RxState=3;
				 //���������,���е���RxCounter1 == 7��  7-5 = 2    openmv���͹�����һ�����ݰ���8��
				Num =          RxBuffer1[RxCounter1-5]; 
				
				OLED_ShowNum(2,8, Num,1); 
				
				
				LoR =           RxBuffer1[RxCounter1-4];	//-1���� 1���ң�0��ʾ��û��ʶ���κ�����
				Finded_flag =   RxBuffer1[RxCounter1-3];	//�Ƿ�ʶ�����ֵı�־λ
				FindTask =      RxBuffer1[RxCounter1-2];		
				//RxCounter1-1��֡β	
		  	GetOpenmvDataCount++;      
				//������1���ڳɹ�������ٸ����ݰ��� ��Ҫ��1s�ӵ���ʱ�������֡��Խ��Խ׼ȷ����λ���Ļ�ƫ��ʹ���
				//�����һ�½�����룬��openmv�����֡��ֱ�Ӵ�����
				if(have==0)have=1;				
			}
		}
		else if(RxState==3)		//����Ƿ���ܵ�������־
		{
				if(RxBuffer1[RxCounter1-1] == 0x5B)
				{
							RxCounter1 = 0;
							RxState = 0;
						
				}
				else   //���մ���
				{
							RxState = 0;
							RxCounter1=0;
							for(i=0;i<10;i++)
							{
									RxBuffer1[i]=0x00;	//�����������������
							}
				}
		} 
		else    //�����쳣
		{
				RxState = 0;
				RxCounter1=0;
				for(i=0;i<10;i++)
				{
						RxBuffer1[i]=0x00;    //�����������������
				}
		}
}
uint8_t Bluetooth_Receive_Buffer[1];

uint8_t Openmv_Receive_Buffer[1];
uint8_t Screen_Information;
uint8_t have;

uint8_t recv_buf[5]={0}; //����buf Receive buf
uint8_t Vocal_recv[5]={0};
uint8_t  cmd[5] = {0};
uint8_t g_index = 0;
int j = 0;
uint8_t Rx1_Temp=0;
uint8_t Flag = 0;

//void Processing_Data(uint8_t RXdata)
//{
//	recv_buf[g_index] =RXdata;
//	g_index++;
//	if (g_index >=5)
//		{
//			 if (recv_buf[0] == 0xAA && recv_buf[1] == 0x55) 
//			 {	 
//				 OLED_ShowNum(2,1,666,3);
//					g_index = 0;
//			 }
//			
//		}2
//}

void Processing_Data(uint8_t RXdata)
{
	recv_buf[g_index] =RXdata;
	
	if(RXdata == 0xAA || g_index!=0)
		g_index++;
	
	if (g_index >=5)
		{
			g_index = 0;
			//printf("Read_ID: %d\n",recv_buf[3]);
		}

}

void Write_Data(uint8_t dat)
{

	cmd[0] = 0xAA;
	cmd[1] = 0x55;
	cmd[2] = 0xFF;
	cmd[3] = dat;  // ֱ��ʹ�ô��������
	cmd[4] = 0xFB;

	for(j = 0; j<5 ;j++)
	{
		//OLED_ShowNum(2,1,666,3);
		HAL_UART_Transmit(&huart1, &cmd[j], 1, HAL_MAX_DELAY);
	}

}

void Write_Data_1(uint8_t dat)
{

	cmd[0] = 0xAA;
	cmd[1] = 0x55;
	cmd[2] = dat;
	cmd[3] = 0x00;  // ֱ��ʹ�ô��������?
	cmd[4] = 0xFB;

	for(j = 0; j<5 ;j++)
	{
		//OLED_ShowNum(2,1,666,3);
		HAL_UART_Transmit(&huart1, &cmd[j], 1, HAL_MAX_DELAY);
	}
}

void Write_Data_2(uint8_t dat)
{
	cmd[0] = 0xAA;
	cmd[1] = 0x55;
	cmd[2] = 0xFF;
	cmd[3] = dat;  // ֱ��ʹ�ô��������?
	cmd[4] = 0xFB;

	for(j = 0; j<5 ;j++)
	{
		//OLED_ShowNum(2,1,666,3);
		HAL_UART_Transmit(&huart1, &cmd[j], 1, HAL_MAX_DELAY);
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	
	if(huart->Instance == USART1)
	{
			HAL_UART_Receive_IT(&huart1, &Rx1_Temp, 1);
			Processing_Data(Rx1_Temp);
	}
	
	if(huart->Instance == USART3)
	{
		Openmv_Receive_Data(Openmv_Receive_Buffer[0]);
		HAL_UART_Receive_IT(&huart3, &Openmv_Receive_Buffer[0], 1);
	}
	
	if(huart->Instance == UART5)
	{
		HAL_UART_Receive_IT(&huart5, &Screen_Information, 1);
	}
	
	if(huart->Instance == USART2 && ok_2==1 && BL_Rec_Flag == 1)
	{
		//Openmv_Receive_Data(Openmv_Receive_Buffer[0]);
		HAL_UART_Receive_IT(&huart2,(uint8_t *)&BL_Num, 1);
		OLED_ShowChar(1,11,BL_Num);
//		OLED_Clear();
	}
	
//	if(huart->Instance == USART6)
//	{
//		HAL_UARTEx_ReceiveToIdle_IT(&huart6, esp8266_buf, 512);
//		rev_ok = 1;
//	}
	
//	if(huart == &huart6)
//	{
//		HAL_UARTEx_ReceiveToIdle_IT(&huart6, esp8266_buf, 512);
//	}
//	
		
}




void hhSerialSendByte(uint8_t Byte){
HAL_UART_Transmit(&huart2, &Byte, 1, HAL_MAX_DELAY);
}

/* USER CODE END 1 */
