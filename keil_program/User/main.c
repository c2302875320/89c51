#include "main.h"


void delay_5us(void)//一进一出，延迟5us时间
{
	_nop_();
}


void delay_us(unsigned char us)	//us级别延迟，最大输入255，us--1次用6.5us，进入1次函数需要11.95us
{
	while (us--);
}


void delay_ms(unsigned char ms)//设置毫秒级别延迟函数，ms最大输入255
{
	unsigned char x;
	for (ms; ms > 0; ms--)
		for (x = 114; x > 0; x--);
}



//主函数
int main(void){
		
		uchar adc = 0;
		uint voltage = 0;
		uchar LED_buffer1[16] = {""};//ADC显示缓冲
		uchar LED_buffer2[16] = {""};//电压显示缓冲
		LCD_Init();//1602初始化
		
		while(1)
		{
			adc = ADC0832_Read(0);//读取adc值
//			voltage = adc * 500.0 / 255;//将adc值换算成电压
			
//			LED_buffer1[9] = adc / 100 + '0';		//取adc百位
//			LED_buffer1[10] = adc % 100 / 10 + '0';	//取adc十位
//			LED_buffer1[11] = adc % 10 + '0';		//取adc个位
//	 
//			LED_buffer2[9] = voltage / 100 + '0';		//取电压个位
//			LED_buffer2[10] = '.';//小数点
//			LED_buffer2[11] = voltage / 10 % 10 + '0';	//取电压十分位
//			LED_buffer2[12] = voltage % 10 + '0';	//取电压百分位
			
			
//			LCD_ShowString(1, 1, &LED_buffer1[0]);	//显示字符串
			LCD_ShowNum(2, 1, ADC0832_Read(0),3);	//显示字符串
		}

		return 0;
}