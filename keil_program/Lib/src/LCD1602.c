/*

LCD1602分为读操作和写操作
其中读操作可以分为读状态和读数据，写操作可以分为写指令和写数据。

读状态：读取LCD引脚状态，返回为状态字，D0-D6为当前LCD数据指针的地址 
D7为是否允许读写操作(即检查LCD是否处于忙状态)
引脚电平：RS=L,RW=H,E=H
输出：D0~D7=状态字

读数据：读取D0-07内的数据
引脚电平：RS=H,RW=H,E=H
输出：D0~D7的数据

写指令： 写入LCD的控制指令，比如清屏，显示开关等
引脚电平：RS=L,RW=L,D0~D7=指令码，E=高脉冲


写数据： 写入需要显示的数据，比方说要显示字符a，就写入0100 0001（41H）
引脚电平：RS=H,RW=L,D0~D7=数据，E=高脉冲

RS    R/W    DB7    DB6    DB5    DB4    DB3    DB2    DB1    DB0     执行时间

11条指令
指令1：清显示，指令码01H
0    0    0    0    0    0    0    0    0    1    1.64ms
光标复位到地址00H位置，
LCD显示DDRAM的内容全部写入” “的ASCII码20H

指令2：光标复位，光标返回到地址00H
0    0    0    0    0    0    0    0    1    X    1.64ms
光标复位到地址00H位置
LCD显示DDRAM的内容不变

指令3：光标和显示位置设置
0    0    0    0    0    0    0    1    I/D    S    40us
I/D，写入新数据后光标移动方向，高电平右移，低电平左移，
S：写入新数据后显示屏字符是否整体左移或右移一个字符，高电平表示有效，低电平表示无效

指令4：显示开关控制
0    0    0    0    0    0    1    D    C    B    40us
D(Dispaly)：控制整体的显示开与关，高电平表示开显示屏，低电平表示关显示屏
C(Cursor)：控制光标的开与关，高电平表示有光标，低电平表示无光标
B(Blink)：控制光标是否闪烁，高电平闪烁，低电平不闪烁

指令5：光标或显示移位
0    0    0    0    0    1    S/C    R/L    X     X   40us
SC=0，RL=0 ：光标左移
SC=0，RL=1 : 光标右移
SC=1，RL=0时，字符和光标都左移
SC=1，RL=1时字符和光标都右移

指令6：功能设置命令
0    0    0    0    1    DL    N    F    X     X   40us
DL：DL=1代表数据长度为8位，DL=0代表数据长度为4位
N：低电平时只有一行可以显示，高电平时两行都可以显示，
F：低电平时一个字符大小为5X7的点阵字符，高电平时一个字符大小为5X10的点阵字符。

指令7：CGRAM地址设置
0    0    0    1    CGRAM的地址（6位）   40us
LCD1602的CGRAM可以设置存储自定义字符，它共有6位，一共可以表示64个地址，即64个字节。
一个5×8点阵字符共占用8个字节，那么这64个字节一共可以自定义8个字符。

指令8：DDRAM地址设置
0    0    1    DDRAM的地址（6位）   40us
指定DDRAM地址

指令9：读忙信号和光标地址
0    1    BF   AC内容（7位）   40us
BF：忙标志位，高电平表示忙，此时模块不能接收命令或数据，如果为低电平表示不忙

指令10：写数据到CGRAM或DDRAM指令
1    0    要写入的数据（D7-D0）   40us
写入DDRAM或CGRAM中的内容

指令11·：从CGRAM或DDRAM读数据指令
1    1    要读出的数据（D7-D0）   40us
读取DDRAM或CGRAM中的内容
*/ 


#include "LCD1602.h"


/**
  * @brief  LCD1602写命令
  * @param  cmd 要写入的命令
  * @retval 无
  */

void LCD_WriteCmd(u8 cmd)
{
	LCD1602_RS = 0;  //选择写指令
	LCD1602_RW = 0; 	//选择写
	LCD1602_EN = 0;  //E使能拉低
	LCD1602_DATA_PIN=cmd;  //把指令送入P0
	delay_ms(2); //延时一小会儿，让1602准备接收数据
	LCD1602_EN = 1;  //使能线电平变化上升沿，命令送入1602的8位数据口
	delay_ms(2);//延时，让数据发送
	LCD1602_EN = 0;	//使能线拉低	
}



/**
  * @brief  LCD1602写数据
  * @param  Dat 要写入的数据
  * @retval 无
  */

void LCD_WriteData(u8 dat)
{
	LCD1602_RS = 1;  //选择写数据
	LCD1602_RW = 0; //选择写
	LCD1602_EN = 0; //E使能拉低
	LCD1602_DATA_PIN=dat;   //把数据送入P0
	delay_ms(2); //延时一小会儿，让1602准备接收数据
	LCD1602_EN = 1;  //使能线电平变化上升沿，命令送入1602的8位数据口    
	delay_ms(2);//延时，让数据发送
	LCD1602_EN = 0;//使能线拉低
}


/**
  * @brief  LCD1602初始化函数
  * @param  无
  * @retval 无
  */

void LCD_Init(void)
{
	LCD_WriteCmd(0x38);        // 指令6，功能设置：8位数据口，16*2显示，5*7点阵，
	LCD_WriteCmd(0x0C);        // 指令4，开显示，不显示光标
	LCD_WriteCmd(0x06);        // 指令3，地址加1，当写入数据后光标右移
	LCD_WriteCmd(0x01);        // 指令1，清屏
}


/**
  * @brief  LCD1602设置光标位置
  * @param  Line 行位置，范围：1~2
  * @param  Column 列位置，范围：1~16
  * @retval 无
  */
void LCD_SetCursor(unsigned char Line,unsigned char Column)
{
	if(Line==1)
	{
		LCD_WriteCmd(0x80|(Column-1));
	}
	else if(Line==2)
	{
		LCD_WriteCmd(0x80|(Column-1+0x40));
	}
}


/**
  * @brief  在LCD1602指定位置上显示一个字符
  * @param  Line 行位置，范围：1~2
  * @param  Column 列位置，范围：1~16
  * @param  Char 要显示的字符
  * @retval 无
  */
void LCD_ShowChar(unsigned char Line,unsigned char Column,char Char)
{
	LCD_SetCursor(Line,Column);
	LCD_WriteData(Char);
}



/**
  * @brief  在LCD1602指定位置开始显示所给字符串
  * @param  Line 起始行位置，范围：1~2
  * @param  Column 起始列位置，范围：1~16
  * @param  String 要显示的字符串
  * @retval 无
  */
void LCD_ShowString(unsigned char Line,unsigned char Column,char *String)
{
	unsigned char i;
	LCD_SetCursor(Line,Column);
	for(i=0;String[i]!='\0';i++)
	{
		LCD_WriteData(String[i]);
	}
}



/**
  * @brief  返回值=X的Y次方
  */
int LCD_Pow(int X,int Y)
{
	unsigned char i;
	int Result=1;
	for(i=0;i<Y;i++)
	{
		Result*=X;
	}
	return Result;
}


/**
  * @brief  在LCD1602指定位置开始显示所给数字
  * @param  Line 起始行位置，范围：1~2
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~65535
  * @param  Length 要显示数字的长度，范围：1~5
  * @retval 无
*/
void LCD_ShowNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length)
{
	unsigned char i;
	LCD_SetCursor(Line,Column);
	for(i=Length;i>0;i--)
	{
		LCD_WriteData(Number/LCD_Pow(10,i-1)%10+'0');
	}
}
 
/**
  * @brief  在LCD1602指定位置开始以有符号十进制显示所给数字
  * @param  Line 起始行位置，范围：1~2
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：-32768~32767
  * @param  Length 要显示数字的长度，范围：1~5
  * @retval 无
  */
void LCD_ShowSignedNum(unsigned char Line,unsigned char Column,int Number,unsigned char Length)
{
	unsigned char i;
	unsigned int Number1;
	LCD_SetCursor(Line,Column);
	if(Number>=0)
	{
		LCD_WriteData('+');
		Number1=Number;
	}
	else
	{
		LCD_WriteData('-');
		Number1=-Number;
	}
	for(i=Length;i>0;i--)
	{
		LCD_WriteData(Number1/LCD_Pow(10,i-1)%10+'0');
	}
}
 
/**
  * @brief  在LCD1602指定位置开始以十六进制显示所给数字
  * @param  Line 起始行位置，范围：1~2
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~0xFFFF
  * @param  Length 要显示数字的长度，范围：1~4
  * @retval 无
  */
void LCD_ShowHexNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length)
{
	unsigned char i,SingleNumber;
	LCD_SetCursor(Line,Column);
	for(i=Length;i>0;i--)
	{
		SingleNumber=Number/LCD_Pow(16,i-1)%16;
		if(SingleNumber<10)
		{
			LCD_WriteData(SingleNumber+'0');
		}
		else
		{
			LCD_WriteData(SingleNumber-10+'A');
		}
	}
}
 
/**
  * @brief  在LCD1602指定位置开始以二进制显示所给数字
  * @param  Line 起始行位置，范围：1~2
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~1111 1111 1111 1111
  * @param  Length 要显示数字的长度，范围：1~16
  * @retval 无
  */
void LCD_ShowBinNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length)
{
	unsigned char i;
	LCD_SetCursor(Line,Column);
	for(i=Length;i>0;i--)
	{
		LCD_WriteData(Number/LCD_Pow(2,i-1)%2+'0');
	}
}
