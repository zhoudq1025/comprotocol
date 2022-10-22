#ifndef __USART_H
#define __USART_H

#include "main.h"

void USART1_Init();
void USART_SendString(USART_TypeDef* USARTx, char *DataString);



extern u8 usart1_res;



#endif