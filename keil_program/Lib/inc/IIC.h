#ifndef __IIC_H__
#define __IIC_H__

#include "Lib_config.h"

//IIC���Ŷ���
sbit C51_IIC_SCL = P3^5;//ʱ����
sbit C51_IIC_SDA = P3^4;//������


void IIC_Start(void);
void IIC_Stop(void);
void IIC_IDLE(void);
void IIC_SendByte(u8 Byte);
u8 IIC_ReceiveByte();
void IIC_SendAck(u8 AckBit);
u8 IIC_ReceiveAck();




#endif

