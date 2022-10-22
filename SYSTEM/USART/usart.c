#include "usart.h"

char Rx_Buff[200];
int  Rx_count=0;   //用于ESP8266判断接受数据的多少
unsigned char Message_length;
char Message_Buf[20];  //用于存贮操作指令

/******************串口1配置******************/
//IO对应：TX-PA9  RX-PA10
//IO时钟：RCC_APB2Periph
//IO配置：PA9-复用推挽输出；PA10-浮空输入
//USART配置：波特率；设置硬件流；设置模式；设置奇偶校验；设置停止位；设置数据位
//中断配置：中断通道；使能；响应优先级；抢占优先升级；
/*********************结束********************/
void USART1_Init()
{
	GPIO_InitTypeDef        GPIO_InitStruct;
	USART_InitTypeDef       USART_InitStruct;
	NVIC_InitTypeDef        NVIC_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	//配置IO
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//配置串口
	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_HardwareFlowControl  = USART_HardwareFlowControl_None;//无硬件控制流
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_Parity = USART_Parity_No;//无奇偶校验
	USART_InitStruct.USART_StopBits = USART_StopBits_1;//1位停止位
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//8位数据位
	USART_Init(USART1, &USART_InitStruct);
	USART_Cmd(USART1, ENABLE);
	
	//中断配置
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;//1位响应优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;//1位抢占优先级
	NVIC_Init(&NVIC_InitStruct);
}

void Receive_Normal_Data()
{
	  static int i;        //静态变量i
	
		if(Rx_count==150)                   //最多接收150个数据，数据溢出 清空数组
		{
			Rx_count=0;                 	
			memset(Rx_Buff, 0, sizeof(Rx_Buff));  //清空当前数组所有数据
		}

		if(Message_length>0)                   //表示可以开始存贮操作指令
		{
			Message_Buf[i]=Rx_Buff[Rx_count];   //存贮操作指令数据
			i++;
			Message_length--;                  //存一个指令，剩余数量减一,判断操作指令是否存贮完成
		}
		 
		if(Rx_count>3&&Rx_Buff[Rx_count-2]==0&&Rx_Buff[Rx_count-1]==0&&Rx_Buff[Rx_count]>0)   
		//如果当前接收到的数据大于0，并且这个数据的前两个数据为00 代表当前数据就是操作指令的长度。
		{
			 memset(Message_Buf, 0, sizeof(Message_Buf)); //清空存贮操作指令的数组，准备存贮新的操作指令
			
			 Message_length=Rx_Buff[Rx_count];      //将接收到的数据存为操作指令长度。
			 i=0;                                   //清空i
		}
		Rx_count++;                               //准备存储下一个数据 
						
}

//USART1中断服务函数
u8 usart1_res;
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1,USART_IT_RXNE))
	{
		usart1_res = USART_ReceiveData(USART1);	
		Rx_Buff[Rx_count] = usart1_res;
	}
}

int fputc(int ch, FILE *f )
{
   USART_SendData(USART1,(u8) ch );
   while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
   return ch;
}

void Send_data(USART_TypeDef * USARTx,u8 *s)
{
	while(*s!= '0')
	{
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC )==RESET);
		USART_SendData(USARTx,*s);
		s++;
	}
}

void USART_SendString(USART_TypeDef* USARTx, char *DataString)
{
	int i = 0;
	USART_ClearFlag(USARTx,USART_FLAG_TC);										//发送字符前清空标志位（否则缺失字符串的第一个字符）
	while(DataString[i] != '\0')												//字符串结束符
	{
		USART_SendData(USARTx,DataString[i]);									//每次发送字符串的一个字符
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC) == 0);					//等待数据发送成功
		USART_ClearFlag(USARTx,USART_FLAG_TC);									//发送字符后清空标志位
		i++;
	}
}