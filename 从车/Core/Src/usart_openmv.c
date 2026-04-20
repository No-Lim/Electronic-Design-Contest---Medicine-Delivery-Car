#include "usart_openmv.h"
#include "usart.h"

/**串口通讯文件**/


usartstructdef Usart1_rec={0};

usartstructdef Usart2_rec={0};


//串口数据解析
void usart_anl()
{
	/**可以自行添加数据  bufs=Usart1_rec.buf[Usart1_rec.out++] 获取数据，或者直接解析数据**/
	if(Usart1_rec.in!=Usart1_rec.out)
	{
		Usart1_rec.buf[Usart1_rec.out++];
		
	}
	else
	{
		Usart1_rec.out=0;
	}
	
	if(Usart2_rec.in!=Usart2_rec.out)
	{
		Usart2_rec.buf[Usart2_rec.out++];
	}
	else
	{
	
		Usart2_rec.out=0;
	}
}

