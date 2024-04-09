#include "TIMER.h"

/*T0/T1才有以上工作模式，称之为传统定时器，只能向上计数
当选择方式3时，定时器T0就会被分成两个独立的计数器或者定时器。
此时，TL0为8位计数器，计数溢出好置位TF0，并向CPU申请中断，之后需要软件重装初值；
TH0也被固定为8位计数器，不过TL0已经占用了TF0和TR0，因此TH0将占T1的中断请求标志TF1和定时器启动控制为TR1。

一般配置流程：
第一步：配置工作方式寄存器—TMOD
第二步：赋初值 —— THx、TLx
第三步：配置控制寄存器——TCON 
第四步：开总中断——EA = 1；
*/

/*
模式控制寄存器TMOD： GATE  C/T M1 M0 GATE C/T M1 M0
控制寄存器TCON: TF1 TR1 TF0 TR0 IE1 TT1 IE0 IT0

GATE -- 是否由外部中断引脚来开启定时器中断
C/T -- 外部时钟还是内部时钟

T2CON:     
TF2         =           T2CON^7;

EXF2        =           T2CON^6;
当EXEN2=1且T2EX（单片机的P1.1口）的负跳变产生捕捉或重装时，EXF2置位。定时器2中断使能时，
EXF2=1将使CPU进入定时器2的中断服务程序。EXF2必须用软件清0。在递增/递减计数器模式（DCEN=1）中，EXF2不会引起中断。

RCLK        =           T2CON^5;置1时将定时器2溢出脉冲作为串口方式1或者方式3的接收时钟，0则使用定时器1 
TCLK        =           T2CON^4;置1时将定时器2溢出脉冲作为串口方式1或者方式3的发送时钟，0则使用定时器1 
EXEN2       =           T2CON^3;
当EXEN2=1且定时器2未用作串行口时钟时，允许T2EX引脚的负跳变产生捕获或重装；当EXEN2=0时，T2EX引脚的负跳变对定时器2无效。

这里我们要对捕获进行一下解释，通俗地讲，捕获就是捕捉某一瞬间的值，通常它用来测量外部某个脉冲的宽度或周期，
使用捕获功能可以准确地测量出脉冲的宽度或周期。
捕获的工作原理是：单片机内部有两组寄存器，其中一组的内部数值是按固定机器周期递增或递减的，
通常这组寄存器就是单片机内部定时器的计数器寄存器（TLn，THn），当与捕获功能相关的外部某引脚有一个负跳变时，
捕获便会立即将此时第一组寄存器中的数值准确地捕获，并且存入另一组寄存器中，
这组寄存器通常被称为“陷阱寄存器”（RCAPnL，RCAPnH），同时向CPU申请中断，
软件记录两次捕获之间的数据后，便可以准确地计算出该脉冲的周期。


TR2         =           T2CON^2; 1:启动 0：关闭
C_T2        =           T2CON^1; 1:计数器（下降沿触发）选择 0:定时器
CP_RL2      =           T2CON^0; 1：捕捉 0：重装
        
    RCAP2H: 重装高八位    
    RCAP2L: 重装低八位

    T2H: 初始高八位
    T2L: 初始低八位
    
T2MOD:  T2OE             DCEN
       输出使能位      向上/向下计数位
       

工作方式3只适用于定时器0。如果使定时器0为工作方式3，则定时器1将处于关闭状态。
当T0为工作方式3时，TH0和TL0分成2个独立的8位计数器。
其中，TL0既可用作定时器，又可用作计数器，并使用原T0的所有控制位TR0及其定时器回零标志TF0和中断源。
TH0只能用作定时器，并使用T1的控制位TR1、回零标志TFl和中断源。

通常情况下，T0不运行于工作方式3，只有在T1处于工作方式2，并不要求中断的条件下才可能使用。
这时，T1往往用作串行口波特率发生器，TH0用作定时器，TL0作为定时器或计数器。
所以，方式3是为了使单片机有1个独立的定时器／计数器、1个定时器以及1个串行口波特率发生器的应用场合而特地提供的。
这时，可把定时器l用于工作方式2，把定时器0用于工作方式3。


机器周期  = 1/12Mhz/12 = 1*10^(-6) = 1us    1us * 10000 = 0.1s=100ms
*/

sbit EXAMPLE_LED0 = P1^0;
sbit EXAMPLE_LED1 = P1^1;
sbit EXAMPLE_LED2= P1^2;


void TIMER_Init(u8 SET_TMOD,u8 USE_TIMER,u16 TIMER_PULSE)
{
    u8 THx,TLx,CT,GATE,MODE;
    
    if(USE_TIMER == USE_TIMER0)
    {
        MODE = SET_TMOD&3;
        CT = SET_TMOD&4;
        GATE = SET_TMOD&8;
    }
    
    if(USE_TIMER == USE_TIMER1)
    {
        MODE = (SET_TMOD&48)>>4;
        CT = (SET_TMOD&64)>>4;
        GATE = (SET_TMOD&128)>>4;
    }
    
    
    if(MODE = 0)//13位溢出
    {
        if(TIMER_PULSE<8191 && TIMER_PULSE>0) ;//停止定时器使能
        {
            THx = (8191 - TIMER_PULSE)/256;
            THx &= 31; //只取低5位
            TLx = (8191 - TIMER_PULSE)%256;
        }
    }            
    
    if(MODE = 1)//16位溢出
    {
        if(TIMER_PULSE<65535 && TIMER_PULSE>0)
        {
            THx = (65535 - TIMER_PULSE)/256;
            TLx = (65535 - TIMER_PULSE)%256;
        }
    }  
    
    if(MODE = 2)//8位重装，TH作为重装值 ，TL作为初始值
    {
        if(TIMER_PULSE<255 && TIMER_PULSE>0)
        {
            THx = TLx = 255 - TIMER_PULSE;
        }
    }  
    
    if (USE_TIMER == USE_TIMER0)
    {
        TH0 = THx;
        TL0 = TLx;
    }
    
    if (USE_TIMER == USE_TIMER1)
    {
        TH1 = THx;
        TL1 = TLx;
    }
    

    
    TMOD = SET_TMOD;
    if (USE_TIMER == USE_TIMER1)
    {
        TR1 = 1;
        ET1 = 1;
    }
    if (USE_TIMER == USE_TIMER0)
    {
        TR0 = 1;
        ET0 = 1;  
    }
    EA = 1;
}

void TIMER2_Init(u16 TIMER_PULSE)
{
    TH2 = (65535-TIMER_PULSE)/256;
    TL2 = (65535-TIMER_PULSE)%256;
    RCAP2H =  TH2;
    RCAP2L = TL2;
    TR2 = 1;
    ET2 = 1;
    EA = 1;
}

void TIMER0() interrupt 1{
    
    if(TF0)TF0=0;
    TH0 = (65535-1000)/256;
    TL0 = (65535-1000)%256;
    EXAMPLE_LED0 = ~EXAMPLE_LED0;

}

void TIMER1() interrupt 3{
    
    if(TF1)TF1=0;
    TH1 = (65535-5000)/256;
    TL1 = (65535-5000)%256;
    EXAMPLE_LED1 = ~EXAMPLE_LED1;

}

void TIMER2() interrupt 5{
    
    if(TF2)TF2=0;
    
    EXAMPLE_LED2 = ~EXAMPLE_LED2;

}
