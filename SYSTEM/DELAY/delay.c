#include "delay.h"
 

void delay_1us(void)
{
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();
}

/****************************
�������ƣ�delay_us
�������ã���ʱn΢��
����������time 	��ʱʱ��
��������ֵ����
�������ߣ�ZDQ
����ʱ�䣺2022.04.25
�޸�ʱ�䣺2022.04.25
****************************/
void delay_us(uint32_t time)
{
	while(time--)
		delay_1us();
}

/****************************
�������ƣ�delay_ms
�������ã���ʱn����
����������time 	��ʱʱ��
��������ֵ����
�������ߣ�ZDQ
����ʱ�䣺2022.04.25
�޸�ʱ�䣺2022.04.25
****************************/
void delay_ms(uint32_t time)
{
	while(time--)
		delay_us(1000);
}



