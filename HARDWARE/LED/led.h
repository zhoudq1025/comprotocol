#ifndef __LED_H
#define __LED_H

#include "main.h"   

void LED_GPIO_Config(void);
 
 /* 使用标准的固件库控制IO*/
#define LED1(a)	if (a)	\
					GPIO_SetBits(GPIOB,GPIO_Pin_5);\
					else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_5)

#define LED2(a)	if (a)	\
					GPIO_SetBits(GPIOB,GPIO_Pin_0);\
					else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_0)

#define LED3(a)	if (a)	\
					GPIO_SetBits(GPIOB,GPIO_Pin_1);\
					else		\
					GPIO_ResetBits(GPIOB,GPIO_Pin_1)
					
#define LED_RED \
					LED1(1);\
					LED2(0);\
					LED3(0)

#define LED_GREEN \
					LED1(0);\
					LED2(1);\
					LED3(0)
					
#define LED_BLUE \
					LED1(0);\
					LED2(0);\
					LED3(1)
					
#endif