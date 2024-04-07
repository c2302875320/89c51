#ifndef __ADC0832_H__
#define __ADC0832_H__

#include "Lib_config.h"

//ADC0832的各个引脚定义
sbit ADC0832_CS_PIN = P3^5;
sbit ADC0832_CLK_PIN = P3^4;
sbit ADC0832_DI_PIN = P3^3;
sbit ADC0832_DO_PIN = P3^3;


u8 ADC0832_Read(bit channel);

#endif
