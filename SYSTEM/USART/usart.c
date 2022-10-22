#include "usart.h"

char Rx_Buff[200];
int  Rx_count=0;   //����ESP8266�жϽ������ݵĶ���
unsigned char Message_length;
char Message_Buf[20];  //���ڴ�������ָ��

/******************����1����******************/
//IO��Ӧ��TX-PA9  RX-PA10
//IOʱ�ӣ�RCC_APB2Periph
//IO���ã�PA9-�������������PA10-��������
//USART���ã������ʣ�����Ӳ����������ģʽ��������żУ�飻����ֹͣλ����������λ
//�ж����ã��ж�ͨ����ʹ�ܣ���Ӧ���ȼ�����ռ����������
/*********************����********************/
void USART1_Init()
{
	GPIO_InitTypeDef        GPIO_InitStruct;
	USART_InitTypeDef       USART_InitStruct;
	NVIC_InitTypeDef        NVIC_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	//����IO
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//�����������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//���ô���
	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_HardwareFlowControl  = USART_HardwareFlowControl_None;//��Ӳ��������
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_Parity = USART_Parity_No;//����żУ��
	USART_InitStruct.USART_StopBits = USART_StopBits_1;//1λֹͣλ
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//8λ����λ
	USART_Init(USART1, &USART_InitStruct);
	USART_Cmd(USART1, ENABLE);
	
	//�ж�����
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;//1λ��Ӧ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;//1λ��ռ���ȼ�
	NVIC_Init(&NVIC_InitStruct);
}

void Receive_Normal_Data()
{
	  static int i;        //��̬����i
	
		if(Rx_count==150)                   //������150�����ݣ�������� �������
		{
			Rx_count=0;                 	
			memset(Rx_Buff, 0, sizeof(Rx_Buff));  //��յ�ǰ������������
		}

		if(Message_length>0)                   //��ʾ���Կ�ʼ��������ָ��
		{
			Message_Buf[i]=Rx_Buff[Rx_count];   //��������ָ������
			i++;
			Message_length--;                  //��һ��ָ�ʣ��������һ,�жϲ���ָ���Ƿ�������
		}
		 
		if(Rx_count>3&&Rx_Buff[Rx_count-2]==0&&Rx_Buff[Rx_count-1]==0&&Rx_Buff[Rx_count]>0)   
		//�����ǰ���յ������ݴ���0������������ݵ�ǰ��������Ϊ00 ����ǰ���ݾ��ǲ���ָ��ĳ��ȡ�
		{
			 memset(Message_Buf, 0, sizeof(Message_Buf)); //��մ�������ָ������飬׼�������µĲ���ָ��
			
			 Message_length=Rx_Buff[Rx_count];      //�����յ������ݴ�Ϊ����ָ��ȡ�
			 i=0;                                   //���i
		}
		Rx_count++;                               //׼���洢��һ������ 
						
}

//USART1�жϷ�����
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
	USART_ClearFlag(USARTx,USART_FLAG_TC);										//�����ַ�ǰ��ձ�־λ������ȱʧ�ַ����ĵ�һ���ַ���
	while(DataString[i] != '\0')												//�ַ���������
	{
		USART_SendData(USARTx,DataString[i]);									//ÿ�η����ַ�����һ���ַ�
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC) == 0);					//�ȴ����ݷ��ͳɹ�
		USART_ClearFlag(USARTx,USART_FLAG_TC);									//�����ַ�����ձ�־λ
		i++;
	}
}