#include "UART.h"

char RxBuffer[RxBuffer_Len] = {0};
bit USART_Busy;

void String_Analysis()
{

}

void USART_Init(u8 USART_Mode, bit Rx_Flag, u8 Priority, u8 SYSclk, u8 Baud_Rate, u8 Double_Baud_Flag, bit USART_Timer)
{
	u16 THLx;
	Priority &= 3;
	USART_Mode &= 3;
	PCON = 0;
	PCON |= (u8)Double_Baud_Flag << 7;			//波特率倍速控制
	SCON = 0;
	SCON |= (USART_Mode << 6);						//设置工作模式
	REN = Rx_Flag;									//允许/禁止串行接收控制位

	IPH &= ~(1 << 4);								//设置中断优先级
	IPH |= ((2 & Priority) << 3);					//设置中断优先级
	PS  = (1 & Priority);							//设置中断优先级

	if (USART_Mode == USART_MODE_1 || USART_Mode == USART_MODE_3) 
	{
		if (USART_Timer)
		{
			T2MOD = 0;								//初始化定时器2模式寄存器
			T2CON = 0;								//初始化定时器2控制寄存器
			THLx = 65536 - SYSclk / 32 / Baud_Rate;	//定时器2 16位自动重装载
			TL2 = THLx;								//设置定时初始值
			TH2 = THLx >> 8;						//设置定时初始值
			RCAP2L = THLx;							//设置定时初始值
			RCAP2H = THLx >> 8;						//设置定时初始值
			TR2 = 1;								//定时器开启
		}
		else
		{
			TMOD &= 0x0F;							//初始化定时器1寄存器
			TMOD |= 1 << 5;							//定时器1 8位自动重装载
			THLx = 256 - (1 << (u8)Double_Baud_Flag) * SYSclk / 12 / 32 / Baud_Rate;	
			TH1 = THLx;								//设置定时初始值
			TL1 = THLx;								//设置定时初始值
			TR1 = 1;								//定时器开启
		}
		RCLK = USART_Timer;
		TCLK = USART_Timer;
	}
	ES = 1;											//允许串行口中断
	EA = 1;											//允许总中断
}

/*----------------------------
UART interrupt service routine
----------------------------*/
void Usart_Isr() interrupt 4
{
	static u16 Str_Count = 0;
    if (RI)
    {
        RI = 0;             //Clear receive interrupt flag

		RxBuffer[Str_Count] = SBUF; 
		if(RxBuffer[Str_Count] == '\n')
		{
			for(++Str_Count; Str_Count < RxBuffer_Len; ++Str_Count)
				RxBuffer[Str_Count] = 0;
			Str_Count = 0;
			String_Analysis();
		}
		else
			++Str_Count;
    }
    if (TI)
    {
        TI = 0;             //Clear transmit interrupt flag
        USART_Busy = 0;		//Clear transmit USART_Busy flag
    }
}

/*----------------------------
Send a byte data to UART
Input: dat (data to be sent)
Output:None
----------------------------*/
void SendData(char dat)
{
    while (USART_Busy);		//Wait for the completion of the previous data is sent
    ACC = dat;              //Calculate the even parity bit P (PSW.0)
    if (P)                  //Set the parity bit according to P
    {
#if (PARITYBIT == ODD_PARITY)
        TB8 = 0;            //Set parity bit to 0
#elif (PARITYBIT == EVEN_PARITY)
        TB8 = 1;            //Set parity bit to 1
#endif
    }
    else
    {
#if (PARITYBIT == ODD_PARITY)
        TB8 = 1;            //Set parity bit to 1
#elif (PARITYBIT == EVEN_PARITY)
        TB8 = 0;            //Set parity bit to 0
#endif
    }
    USART_Busy = 1;
    SBUF = ACC;             //Send data to UART buffer
}

/*----------------------------
Send a string to UART
Input: s (address of string)
Output:None
----------------------------*/
void SendString(char *s)
{
    while (*s)              //Check the end of the string
    {
        SendData(*s++);     //Send current char and increment string ptr
    }
}

char putchar(char Char)
{
    SendData(Char);
    return Char;
}

//适配vofa+串口示波器的JustFloat格式 浮点为大端 
void SendFloat(unsigned char *Data) //发送一个字符
{
	char i;
	for(i=0;i<4;++i)
		SendData(Data[3-i]);
}

