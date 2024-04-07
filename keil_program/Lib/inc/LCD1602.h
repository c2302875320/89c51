#ifndef __LCD1602_H__
#define __LCD1602_H__

#include "Lib_config.h"
#define LCD1602_DATA_PIN P0

sbit LCD1602_RS = P2^0;
sbit LCD1602_RW = P2^1;
sbit LCD1602_EN = P2^2;

void LCD_WriteCmd(u8 cmd);
void LCD_WriteData(u8 dat);
void LCD_Init(void);
void LCD_SetCursor(unsigned char Line,unsigned char Column);
void LCD_ShowChar(unsigned char Line,unsigned char Column,char Char);
void LCD_ShowString(unsigned char Line,unsigned char Column,char *String);
int LCD_Pow(int X,int Y);
void LCD_ShowNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length);
void LCD_ShowSignedNum(unsigned char Line,unsigned char Column,int Number,unsigned char Length);
void LCD_ShowHexNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length);
void LCD_ShowBinNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length);

#endif 