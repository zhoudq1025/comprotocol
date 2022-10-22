#ifndef __CAN_H
#define __CAN_H
#include "main.h"

/*****CAN_GPIO DEFINE****/
#define CAN_GPIO_PORT    GPIOB
#define CANH_GPIO_PIN     GPIO_Pin_8
#define CANL_GPIO_PIN     GPIO_Pin_9

/********* FUNCTION DEFINE ********/
void CAN_GPIO_Init(void);
void CAN_NVIC_Init(void);
void CAN1_Init(void);
void CAN1_Config(void);

u8 CAN_Send_Msg(u8* msg, u8 len);
u8 CAN_Receive_Msg(u8* buf);

/************FLAG DEFINE**************/
extern u8* buf;

#define PASS_ID ((uint32_t)0x1314)

#endif