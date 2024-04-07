/*
1.8位分辨率，逐次逼近
2.双通道A/D转换
3.输入输出电平与TTL/CMOS相兼容
4.5V单电源供电，输入模拟电压在0-5V之间
5.工作频率为250KHZ，转换时间32us
6.功耗低，一般为15mW
7.8P-DIP（双列直插）、14P-SOP两种封装
8.商用芯片温度为0℃ ~ 70℃，工业级为-40℃ ~ 85℃

时序表：
CLK :
        1       2       3       4       5       6       .................      10      11      12     ...............  19      20      21      22
       _ _     _ _     _ _     _ _     _ _     _ _     _ _     _ _     _ _     _ _     _ _     _ _     _ _     _ _     _ _     _ _     _ _     _ _
	    |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |	  |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
   _ _|   |_ _|   |_ _|   |_ _|   |_ _|   |_ _|   |_ _|   |_ _|   |_ _|   |_ _|   |_ _|   |_ _|   |_ _|   |_ _|   |_ _|   |_ _|   |_ _|   |_ _|   |_
	 
	 
CS :
_ _                                                                                                                                                 _ _
   |                                                                                                                                               |
   |_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _| 
	 
	 
DI:

前三个CLK脉冲设置工作模式


DO:

第四个CLK脉冲下降沿开始	 MSB-->LSB-->MSB

工作过程：
在第一个时钟脉冲的下沉之前DI端必须是高电平，表示启始信号。
在第二、三个脉冲下沉之前DI端应输入两位数据用于选择通道功能。
当两位数据为“1”、“0”时，只对CH0进行单通道转换。
当两位数据为“1”、“1”时，只对CH1进行单通道转换。
当两位数据为“0”、“0”时，将CH0作为正输入端IN+，CH1作为负输入端IN-进行输入。
当两位数据为“0”、“1”时，将CH0作为负输入端IN-，CH1 作为正输入端IN+进行输入。
到第三个脉冲的下降之后DI端的输入电平就失去输入作用，此后DO/DI端则开始利用数据输出DO进行转换数据的读取。

从第4个脉冲下降沿开始由DO端输出转换数据最高位Data7，
随后每一个脉冲的下降沿DO端输出下一位数据。直到第11个脉冲时发出最低位数据Data0，一个字节的数据输出完成。
也正是从此位开始输出下一个相反字节的数据，即从第11个字节的下降沿输出Data0。
随后输出8位数据，到第19 个脉冲时数据输出完成，也标志着一次A/D转换的结束。最后将CS置高电平禁用芯片.


*/

#include "ADC0832.h"

u8 ADC0832_Read(bit channel){
	
   u8 i,ADC0832_MSB =0 ,ADC0832_LSB = 0;

   ADC0832_CS_PIN = 0;//片选置为低电平，启动AD转换芯片
   ADC0832_CLK_PIN = 0;//时钟低电平

   ADC0832_DI_PIN = 1;
   _nop_();_nop_();
   ADC0832_CLK_PIN = 1;//第一个脉冲上升沿
   _nop_();_nop_();
   ADC0832_CLK_PIN = 0;//第一个脉冲下降沿
   ADC0832_DI_PIN = 1;
   _nop_();_nop_();
	
   ADC0832_CLK_PIN = 1;//第二个脉冲上升沿
   _nop_();_nop_();
	
   ADC0832_CLK_PIN = 0;//第二个脉冲下降沿
	 ADC0832_DI_PIN = channel;
   _nop_();_nop_();
   ADC0832_CLK_PIN = 1;//第三个脉冲上升沿
   _nop_();_nop_();
	 
	 
   ADC0832_CLK_PIN = 0;
	 ADC0832_DO_PIN = 1;
   _nop_();_nop_();
	 
   for ( i = 0; i < 8; i++)
   {
      ADC0832_CLK_PIN = 1;
      _nop_();_nop_();
      ADC0832_CLK_PIN = 0;
      _nop_();_nop_();

      ADC0832_MSB = ADC0832_MSB<<1|ADC0832_DO_PIN;
   }

   for ( i = 0; i < 8; i++)
   {
	  ADC0832_LSB = ADC0832_LSB|((u8)(ADC0832_DO_PIN)<<i);
      ADC0832_CLK_PIN = 1;
      _nop_();_nop_();
      ADC0832_CLK_PIN = 0;
      _nop_();_nop_();
   }

   ADC0832_CS_PIN = 1;//取消片选
	 return (ADC0832_MSB==ADC0832_LSB)?ADC0832_LSB:12;

}
