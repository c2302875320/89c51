/*
硬件层
支持多主多从，400pF
仲裁方式
传输模式：100kbit/s 、400kbit/s 、3-4kbit/s


协议层

1.数据有效性规定：SCL高电平时SDA必须稳定
2.起始信号：SCL为高，SDA由高电平变成低电平 
3.终止信号：SCL为高，SDA由低电平变成高电平
4.应答信号：从机 ACK信号，NACK信号


总线寻址
7位寻址  一位R/W
10位寻址

主机向从机写数据：

1.主机首先产生START信号
2.然后紧跟着发送一个从机地址，这个地址共有7位，紧接着的第8位是数据方 向位(R/W)，0表示主机发送数据(写)，1表示主机接收数据(读)
3.主机发送地址时，总线上的每个从机都将这7位地址码与自己的地址进行比较，若相同，则认为自己正在被主机寻址，根据R/T位将自己确定为发送器和接收器
4.这时候主机等待从机的应答信号(A)
5.当主机收到应答信号时，发送要访问从机的那个地址， 继续等待从机的应答信号
6.当主机收到应答信号时，发送N个字节的数据，继续等待从机的N次应答信号，
7.主机产生停止信号，结束传送过程。


主机要从从机读数据时
1.主机首先产生START信号
2.然后紧跟着发送一个从机地址，注意此时该地址的第8位为0，表明是向从机写命令，
3.这时候主机等待从机的应答信号(ACK)
4.当主机收到应答信号时，发送要访问的地址，继续等待从机的应答信号，
5.当主机收到应答信号后，主机要改变通信模式(主机将由发送变为接收，从机将由接收变为发送)所以主机重新发送一个开始start信号，然后紧跟着发送一个从机地址，注意此时该地址的第8位为1，表明将主机设 置成接收模式开始读取数据，
6.这时候主机等待从机的应答信号，当主机收到应答信号时，就可以接收1个字节的数据，当接收完成后，主机发送非应答信号，表示不在接收数据
7.主机进而产生停止信号，结束传送过程。




*/

//软件I2C
#include "IIC.h"

/**
   *@breaf	 I2C起始信号
   *@param	无
   *@retval	无
   
   SCL:  ____|_
             | \
             |  \____
   SDA:  ____|
             |\
             | \_____
              
*/
void IIC_Start(void)
{
    C51_IIC_SDA = 1;
    C51_IIC_SCL = 1;
    
    _nop_();_nop_();
    C51_IIC_SDA = 0;
    
    _nop_();_nop_();
    C51_IIC_SCL = 0;
}

 /**
   *@breaf	 I2C结束信号
   *@param	无
   *@retval	无
   
   SCL:     _____
           / 
     ____ /
   SDA:       ____
             /
        ____/ 
              
*/
void IIC_Stop(void)
{
    C51_IIC_SDA = 0;
    C51_IIC_SCL = 1;
    
    _nop_();_nop_();
    C51_IIC_SDA = 1;
    _nop_();_nop_();
    C51_IIC_SCL = 0;
}


void IIC_IDLE(void)
{
    C51_IIC_SDA = 1;
    C51_IIC_SCL = 1; 
}



/**
   *@breaf  I2C发送字节函数
   *@param Byte要发送的字节内容
   *@retval 无
SCL低电平期间，主机将数据位依次放在SDA线上（高位在前），
然后拉高SCL，从机将在SCL高位期间读取数据位，所以SCL高位期间不允许SDA数据发生变化，
依次进行循环8次，实现一个字节数据的传输。 
   */
void IIC_SendByte(u8 Byte)
{
    u8 i;
    
    C51_IIC_SCL=0;
    for(i=0;i<8;i++)
    {
        C51_IIC_SDA = Byte&0x80;
        C51_IIC_SCL=1;
        _nop_();_nop_();
        C51_IIC_SCL=0;
        _nop_();_nop_();
        
        Byte=Byte<<1;
    }
}


/**
*@breaf  I2C接收字节函数
*@param 无
*@retval Byte接收来自外设的数据
SCL低电平期间，从机将数据位依次放在SDA线上，（高位在前），
然后拉高SCL，主机将在SCL高电平期间读取数据，所以SCL高电平期间，不允许SDA数据发生变化，
依次循环8次，即可接受接收一个字节（主机在前接受前，需要释放(SDA）。
*/


u8 IIC_ReceiveByte()
{
    u8 Byte=0,i;
    
    C51_IIC_SDA = 1;//释放SDA
    C51_IIC_SCL = 0;
    for(i=0;i<8;i++)
    {

        C51_IIC_SCL=1;//
        _nop_();_nop_();
        
        Byte=Byte<<1|C51_IIC_SDA;
        
        C51_IIC_SCL=0;
        _nop_();_nop_();
     }
    
     return Byte;
}



/**
*@breaf I2C发送应答
*@param AckBit 应答位，0为应答，1为非应答
*@retval 无
*/
void IIC_SendAck(u8 AckBit)
{
        C51_IIC_SDA=AckBit;
	
        C51_IIC_SCL=1;
        _nop_();_nop_();
        C51_IIC_SCL=0;
        _nop_();_nop_();
}


/**
*@breaf I2C接收应答
*@param无
*@retval AckBit  应答位，0为应答，1为非应答
*/
u8 IIC_ReceiveAck()
{
        u8 AckBit;
    
        C51_IIC_SDA=1;
        C51_IIC_SCL=0;
    
        C51_IIC_SCL=1;
        _nop_();_nop_();
        AckBit=C51_IIC_SDA;
        C51_IIC_SCL=0;
        _nop_();_nop_();
    
        return AckBit;
}



 


