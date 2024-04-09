#ifndef __USART_H__
#define __USART_H__

#include "Lib_config.h"


#define RxBuffer_Len 20
extern char RxBuffer[];

/*Define UART parity mode*/
#define NONE_PARITY     0		//None parity
#define ODD_PARITY      1		//Odd parity
#define EVEN_PARITY     2		//Even parity
#define MARK_PARITY     3		//Mark parity
#define SPACE_PARITY    4		//Space parity

#define PARITYBIT NONE_PARITY	//Testing even parity

#define USART_MODE_0 0
#define USART_MODE_1 1
#define USART_MODE_2 2
#define USART_MODE_3 3

#define Rx_ENABLE	1
#define Rx_DISABLE	0

//STC单片机的4级优先级
#define STC_USART_Priority_Lowest 	0
#define STC_USART_Priority_Lower	1
#define STC_USART_Priority_Higher	2
#define STC_USART_Priority_Highest	3

#define DOUBLE_BAUD_ENABLE	1
#define DOUBLE_BAUD_DISABLE	0

#define USART_TIMER_1 0					//8位自动重装载
#define USART_TIMER_2 1					//16位自动重装载

void USART_Init(u8 USART_Mode, bit Rx_Flag, u8 Priority, u8 SYSclk, u8 Baud_Rate, u8 Double_Baud_Flag, bit USART_Timer);
void SendData(char dat);
void SendFloat(unsigned char *Data);	//适配vofa+串口示波器的JustFloat格式 浮点为大端
	
//适配vofa+串口示波器的JustFloat格式 结束符
#define SendEnd() {SendData(0x00);SendData(0x00);SendData(0x80);SendData(0x7f);}	
	
#endif
