#ifndef __TIMER_H__
#define __TIMER_H__

#include "Lib_config.h"

#define USE_TIMER0 0
#define USE_TIMER1 1
#define USE_TIMER2 2


//TMOD 寄存器配置
#define STC_TIMER0_MODE_0   (u8)0  //模式0，13位计数器/定时器
#define STC_TIMER0_MODE_1   (u8)1  //模式1，16位计数器/定时器，一般是向上计数
#define STC_TIMER0_MODE_2   (u8)2  //模式2，8位自动重装
#define STC_TIMER0_COUTE    (u8)3 //外部脉冲输入
#define STC_TIMER0_TIMER	(u8)0 //内部时钟输入
#define STC_TIMER0_GATE_ON  (u8)8 //门控开启，需要打开TFx与INTx才能开启定时器
#define STC_TIMER0_GATE_OFF (u8)0 //门控关闭，打开TFx即可开启定时器

#define STC_TIMER1_MODE_0   (u8)0  //模式0，13位计数器/定时器
#define STC_TIMER1_MODE_1   (u8)16  //模式1，16位计数器/定时器，一般是向上计数
#define STC_TIMER1_MODE_2   (u8)32  //模式2，8位自动重装
#define STC_TIMER1_COUTE	(u8)64 //外部脉冲输入
#define STC_TIMER1_TIMER	(u8)0 //内部时钟输入
#define STC_TIMER1_GATE_ON  (u8)128 //门控开启，需要打开TFx与INTx才能开启定时器
#define STC_TIMER1_GATE_OFF (u8)0 //门控关闭，打开TFx即可开启定时器

//T2MOD 寄存器配置



#define STC_TIMER0_MODE_3 (u8)3  //模式3，两个8位定时器,禁用T1


void TIMER_Init(u8 SET_TMOD,u8 USE_TIMER,u16 TIMER_PULSE);
void TIMER2_Init(u16 TIMER_PULSE);


//SMOD 寄存器配置


	
#endif
