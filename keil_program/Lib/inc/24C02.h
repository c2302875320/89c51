#ifndef __24C02_H__
#define __24C02_H__

#include "Lib_config.h"

//24C02的各个引脚定义
//sbit ADC0832_CS_PIN = P3^5;
//sbit ADC0832_CLK_PIN = P3^4;

void IIC_Write(u8 IC_ADDR,u8 ADDR,u8 Byte);
u8  IIC_Read(u8 IC_ADDR,u8 ADDR);
#endif
