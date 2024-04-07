#include "24C02.h"



/**
*@breaf I2Cд��������
*@param��
*@retval 
*/
void IIC_Write(u8 IC_ADDR,u8 ADDR,u8 Byte)	 //д���ݺ���
{
    IIC_Start();
     
    IIC_SendByte(IC_ADDR);		//��Ƭ��Ѱ��оƬ
    IIC_ReceiveAck();		//���Ӧ���ź�
     
    IIC_SendByte(ADDR);		//ȷ�������洢λ��ַ
    IIC_ReceiveAck();
     
    IIC_SendByte(num);		//д����
    IIC_ReceiveAck();
     
    IIC_Stop();
     
    delay_ms(10);
}
 


/**
   *@breaf I2C������������
   *@param WordAddress��ȡ�ֽڵĵ�ַ
   *@retval Byte��ȡ�ֽڵ�����
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