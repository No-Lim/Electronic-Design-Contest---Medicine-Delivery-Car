#ifndef USART_OPENMV_H
#define USART_OPENMV_H
#include "../../Core/Inc/main.h"


#define USART_SIZE	8
typedef struct
{
	uint8_t in;//输入下标
	uint8_t out;//输出下标
	uint8_t buf[USART_SIZE];//数据缓存区
	

}usartstructdef;



extern usartstructdef Usart1_rec;
extern usartstructdef Usart2_rec;



#endif

