#include "can1.h"

void CAN1_Config(void)
{
	CAN_GPIO_Init();
	CAN_NVIC_Init();
	CAN1_Init();
}




//GPIO_CONFIG
void CAN_GPIO_Init(void)
{
	GPIO_InitTypeDef        GPIO_InitStructure;
	
	//open clock signal
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap2_CAN1,ENABLE);
	
	//GPIO_Init - TX - CAN_L
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = CANL_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CAN_GPIO_PORT,&GPIO_InitStructure);
	//GPIO_Init - RX - CAN_H
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = CANH_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CAN_GPIO_PORT,&GPIO_InitStructure);
	
}

//NVIC_CONFIG
void CAN_NVIC_Init(void)
{
	NVIC_InitTypeDef        NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
//	
//	#ifdef VECT_TAB_RAM
//		NVIC_SetVectorTable(NVIC_VectTab_RAM,0x0);
//	#else
//		NVIC_SetVectorTable(NVIC_VectTab_FLASH,0x0);
//	#endif
//	
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
}

//CAN_CONFIG

void CAN1_Init(void)
{
	CAN_InitTypeDef             CAN_InitStructure;
	CAN_FilterInitTypeDef       CANFilter_InitStructure;
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
	
//	CAN_DeInit(CAN1);
//	CAN_StructInit(&CAN_InitStructure);
	
	CAN_InitStructure.CAN_TTCM = DISABLE;                   				  //TTCMʱ�䴥������
	CAN_InitStructure.CAN_ABOM = ENABLE;                  				    //ABOM�Զ����߹�����
	CAN_InitStructure.CAN_AWUM = ENABLE;                  				 	  //AWUM�Զ����ѹ���
	CAN_InitStructure.CAN_NART = DISABLE;                				  	  //NART�Զ��ش�����
	CAN_InitStructure.CAN_RFLM = DISABLE;                  				    //RFLM����FIFO����
	CAN_InitStructure.CAN_TXFP = DISABLE;               		          //TXFP�������ȼ��ж�����
	CAN_InitStructure.CAN_Mode = CAN_Mode_LoopBack;     		  		    //CAN����ģʽ
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;            				      //SJW����ֵ
	CAN_InitStructure.CAN_BS1 = CAN_BS1_5tq;           		  		      //BS1�γ���
	CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;            				      //BS2�γ���
	CAN_InitStructure.CAN_Prescaler = 4;               				        //CAN����ʱ�ӷ�Ƶ1-1024
	CAN_Init(CAN1,&CAN_InitStructure);
	
	CANFilter_InitStructure.CAN_FilterNumber = 0;                     //ɸѡ�����0-27
	CANFilter_InitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;   //ɸѡ��ģʽ
	CANFilter_InitStructure.CAN_FilterScale = CAN_FilterScale_32bit;  //ɸѡ���߶�
	CANFilter_InitStructure.CAN_FilterIdHigh = ((PASS_ID<<3|CAN_Id_Extended|CAN_RTR_Data)&0xFFFF0000)>>16;        //CAN_FXR1�Ĵ����ĸ�16λ
	CANFilter_InitStructure.CAN_FilterIdLow = ((PASS_ID<<3|CAN_Id_Extended|CAN_RTR_Data)&0xFFFF);                 //CAN_FXR1�Ĵ����ĵ�16λ
	CANFilter_InitStructure.CAN_FilterMaskIdHigh = 0xFFFF;            //CAN_FXR2�Ĵ����ĸ�16λ
	CANFilter_InitStructure.CAN_FilterMaskIdLow = 0xFFFF;             //CAN_FXR2�Ĵ����ĵ�16λ
	CANFilter_InitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;     //���þ���ɸѡ�������ݴ����ĸ�FIFO
	CANFilter_InitStructure.CAN_FilterActivation = ENABLE;            //�Ƿ�ʹ�ܸ�ɸѡ��
	CAN_FilterInit(&CANFilter_InitStructure);
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
}

//CAN SEND MESSAGE
u8 CAN_Send_Msg(u8* msg, u8 len)
{
	CanTxMsg TxMessage;
	u8 mbox;
	u16 i;
	
	TxMessage.ExtId = PASS_ID;
	TxMessage.IDE = CAN_Id_Extended;
	TxMessage.RTR = CAN_RTR_Data;
	TxMessage.DLC = len;
	for(i=0;i<len;i++) TxMessage.Data[i] = msg[i];
	mbox = CAN_Transmit(CAN1,&TxMessage);
	return mbox;
}


//CAN_IRQHANDLER
extern CanRxMsg Rx_Message;
extern u8 flag;



/**
  * @brief  ��ʼ�� Rx Message���ݽṹ��
  * @param  RxMessage: ָ��Ҫ��ʼ�������ݽṹ��
  * @retval None
  */
void Init_RxMes(CanRxMsg *RxMessage)
{
  uint8_t ubCounter = 0;

	/*�ѽ��սṹ������*/
  RxMessage->StdId = 0x00;
  RxMessage->ExtId = 0x00;
  RxMessage->IDE = CAN_ID_STD;
  RxMessage->DLC = 0;
  RxMessage->FMI = 0;
  for (ubCounter = 0; ubCounter < 8; ubCounter++)
  {
    RxMessage->Data[ubCounter] = 0x00;
  }
}


void USB_LP_CAN1_RX0_IRQHandler(void)
{
	Init_RxMes(&Rx_Message);
	CAN_Receive(CAN1,CAN_FIFO0,&Rx_Message);
	flag=1;
}


