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
函数名称：delay_us
函数作用：延时n微妙
函数参数：time 	延时时间
函数返回值：无
函数作者：ZDQ
创建时间：2022.04.25
修改时间：2022.04.25
****************************/
void delay_us(uint32_t time)
{
	while(time--)
		delay_1us();
}

/****************************
函数名称：delay_ms
函数作用：延时n毫秒
函数参数：time 	延时时间
函数返回值：无
函数作者：ZDQ
创建时间：2022.04.25
修改时间：2022.04.25
****************************/
void delay_ms(uint32_t time)
{
	while(time--)
		delay_us(1000);
}



