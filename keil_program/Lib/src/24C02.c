#include "24C02.h"



/**
*@breaf I2C写数据流程
*@param无
*@retval 
*/
void IIC_Write(u8 IC_ADDR,u8 ADDR,u8 Byte)	 //写数据函数
{
    IIC_Start();
     
    IIC_SendByte(IC_ADDR);		//单片机寻找芯片
    IIC_ReceiveAck();		//检查应答信号
     
    IIC_SendByte(ADDR);		//确定器件存储位地址
    IIC_ReceiveAck();
     
    IIC_SendByte(num);		//写数据
    IIC_ReceiveAck();
     
    IIC_Stop();
     
    delay_ms(10);
}
 


/**
   *@breaf I2C接收数据流程
   *@param WordAddress读取字节的地址
   *@retval Byte读取字节的内容
   */
u8  IIC_Read(u8 IC_ADDR,u8 ADDR)
{
    u8 Byte;
	
    IIC_Start();
    
    IIC_SendByte(IC_ADDR);
    IIC_ReceiveAck();	
    
    IIC_SendByte(ADDR);
    IIC_ReceiveAck();
    
    IIC_Start();
    IIC_SendByte(AT24C02_ADDRESS|0x01);
    
    IIC_ReceiveAck();	
    Byte= I2C_ReceiveByte();
    IIC_SendAck(1);
    
    IIC_Stop();

    return Byte;
}