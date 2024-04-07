#ifndef __LIB_CONFIG_H__
#define __LIB_CONFIG_H__


#define uchar unsigned char
#define uint unsigned int
#define u8 unsigned char
#define u16 unsigned int



//调用库函数声明
#include "stdio.h"
#include "math.h"
#include "intrins.h"
#include "reg52.h"

#include "LCD1602.h"
#include "ADC0832.h"
#include "24C02.h"

extern void delay_5us(void);//一进一出，延迟5us时间
extern void delay_us(unsigned char us);//us级别延迟，最大输入255，us--1次用6.5us，进入1次函数需要11.95us
extern void delay_ms(unsigned char ms);//设置毫秒级别延迟函数，ms最大输入255

	

#endif
