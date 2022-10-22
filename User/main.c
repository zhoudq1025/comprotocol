#include "main.h"

CanRxMsg Rx_Message;
u8 flag;

int main(void)
{
	u8 value[8]={1,2,3,4,5,6,7,8};
	uint8_t ncount;
//	Key_GPIO_Config();
//	LED_GPIO_Config();
	USART1_Init();
	CAN1_Config();
	
	
	
	while(1)
	{
		
	}
}


