#ifndef __TIMER_H__
#define __TIMER_H__

#include "Lib_config.h"

#define USE_TIMER0 0
#define USE_TIMER1 1
#define USE_TIMER2 2


//TMOD �Ĵ�������
#define STC_TIMER0_MODE_0   (u8)0  //ģʽ0��13λ������/��ʱ��
#define STC_TIMER0_MODE_1   (u8)1  //ģʽ1��16λ������/��ʱ����һ�������ϼ���
#define STC_TIMER0_MODE_2   (u8)2  //ģʽ2��8λ�Զ���װ
#define STC_TIMER0_COUTE    (u8)3 //�ⲿ��������
#define STC_TIMER0_TIMER	(u8)0 //�ڲ�ʱ������
#define STC_TIMER0_GATE_ON  (u8)8 //�ſؿ�������Ҫ��TFx��INTx���ܿ�����ʱ��
#define STC_TIMER0_GATE_OFF (u8)0 //�ſعرգ���TFx���ɿ�����ʱ��

#define STC_TIMER1_MODE_0   (u8)0  //ģʽ0��13λ������/��ʱ��
#define STC_TIMER1_MODE_1   (u8)16  //ģʽ1��16λ������/��ʱ����һ�������ϼ���
#define STC_TIMER1_MODE_2   (u8)32  //ģʽ2��8λ�Զ���װ
#define STC_TIMER1_COUTE	(u8)64 //�ⲿ��������
#define STC_TIMER1_TIMER	(u8)0 //�ڲ�ʱ������
#define STC_TIMER1_GATE_ON  (u8)128 //�ſؿ�������Ҫ��TFx��INTx���ܿ�����ʱ��
#define STC_TIMER1_GATE_OFF (u8)0 //�ſعرգ���TFx���ɿ�����ʱ��

//T2MOD �Ĵ�������



#define STC_TIMER0_MODE_3 (u8)3  //ģʽ3������8λ��ʱ��,����T1


void TIMER_Init(u8 SET_TMOD,u8 USE_TIMER,u16 TIMER_PULSE);
void TIMER2_Init(u16 TIMER_PULSE);


//SMOD �Ĵ�������


	
#endif
