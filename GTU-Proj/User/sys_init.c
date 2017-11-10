/*******************************************************************************
*�ļ�����sys_init.c
*�ļ����ܣ���ˮӲ����ʼ��
*�������ڣ�2014-04-28
*�ļ����ߣ�xue
*�޸ļ�¼��
*******************************************************************************/
#include "include.h"

/*ȫ�ֱ���*/
vu16 CCR1_Val = 0x05A0;	  /*��ʱ��2 1440   100Hz	   10ms*/

//u8 IP[20] = {"\"112.124.055.158\""};		/*������IP*/
//u8 PORT[10] = {"\"08018\""};				/*�������˿�*/


u8 GPRSCmd0[20] = {"AT+IPR=115200&W\r"};    /*M35���������� ��115200*/
u8 GPRSCmd1[20] = {"AT+CGATT?\r"};          /*��ѯGPRS����ע���Ƿ�ɹ�*/
u8 GPRSCmd2[20] = {"AT+CPMS=\"SM\"\r"};     /*����Ϣ���ȴ洢λ��*/
u8 GPRSCmd3[20] = {"AT+CMGF=0\r\n"};          /*PDU��ʽ����Ϣ*/
u8 GPRSCmd4[20] = {"AT+CMGS="};             /*����Ϣ����*/
u8 GPRSCmd5[20] = {"AT+CMGD=1,4\r"};        /*ɾ�����ж���Ϣ*/
u8 GPRSCmd6[50] = {"AT+QIOPEN=\"TCP\","};   /*TCP���ӣ���ӡ�IP��,��PORT��\r*/
u8 GPRSCmd7[10]	= {"ATE0\r"};				/*�رջ���*/
u8 GPRSCmd8[20] = {"AT+CMGF=1\r"};          /*text��ʽ����Ϣ,����ɾ������*/
u8 GPRSCmd9[15] = {"AT+QISEND="};           /*TCP����*/
u8 GPRSCmd10[15] = {"AT+QPOWD=1\r"};        /*M35ģ��ػ�����*/
u8 GPRSCmd11[15] = {"AT+CSQ\r"};            /*��ѯ�ź�����*/
u8 GPRSCmd12[15] = {"AT+QIACT\r"};  		/*�����*/
u8 GPRSCmd13[15] = {"AT+QIDEACT\r"};  		/*ע������*/
u8 GPRSCmd14[15] = {"AT+CMGR=1\r"};         /*��ȡ��һ������*/
u8 GPRSCmd15[25] = {"AT+QICSGP=1,\"CMNET\"\r"}; /*����GPRS��APN*/

u8 GPRSPDU[300] = {"0011000D9168"};          /*����Ϣ���Ϳ�ʼ����*/

u8 GPRS_InitFlag = 0;

u8 Canerror = 0;  /*CAN�����ϴγ������*/

/*������������֤CAN������һ��ʱ���ڳ�ʼ����������̫��*/
u16 Count_CanReInitNum = 0;   /*ͳ��һ��ʱ����CAN���߳�ʼ������*/
u16 Count_CanReInitTime = 0;  /*ͳ��һ��ʱ����CAN���߳�ʼ��ʱ��*/ 
/*��������*/
/*******************************************************************************
*�������ƣ�Sys_Init
*�������ܣ���ˮ��ʼ��ģ��������
*�������ã�Ӳ����ʼ������
*�������룺��
*�����������
*�������أ���
*��ע��
*******************************************************************************/
void SYS_Init(void)
{
	/*CPU�ڲ���������ʼ��*/
	RCC_Configuration();
	GPIO_Configuration();
	NVIC_Configuration();
	TIM_Configuration();
	USART_Configuration();
	I2C_Configuration();
	SPI_Configuration();
	ADC_Configuration();
	DMA_Configuration();
//	IWDG_Configuration();
//	EXIT_Configuration();
	SysTick_Configuration();
	/*����洢����ʼ��*/
	FM25_Init();
	RX8025_Init();
//	CAN_Configuration();

}

/*******************************************************************************
*�������ƣ�GPIO_Configuration
*�������ܣ�GPIO��ʼ��
*�������ã���ؿ⺯��
*�������룺��
*�����������
*�������أ���
*��ע��
*******************************************************************************/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);	
	GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);
	/*USART1*/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;		   /*���ڽ�������*/
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	/*RUN LED/PSW1/PSW2/PK1/PK2/DLC/DDA*/ /*OUTPUT*/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	/*DIG2/DIG3/DIG4*/ /*OUTPUT*/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	/*PWRKEY/DCK*//*OUTPUT*/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	/*DIG1*//*OUTPUT*/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_InitStructure);

	/*DK1/DK2*//*Input*/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	/*KEY*//*Input*/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	/*M35STATUS*//*Input*/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	/*USART2*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;		   /*���ڽ�������*/
	GPIO_Init(GPIOA, &GPIO_InitStructure);

		/*USART3*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;		   /*���ڽ�������*/
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/*C2DIR/FM25CS*/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_4;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	

	/*����I2C1���ţ�SCL��SDA*/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
		
	/*FM25 �ܽ�����*/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

		/*ADC0-3*/
	 /* Configure PC.00 (ADC Channel10) as analog input -------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Configure PC.01 (ADC Channel11) as analog input -------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Configure PC.02 (ADC Channel12) as analog input -------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	   
	/* Configure PC.03 (ADC Channel13) as analog input -------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

}

/*******************************************************************************
*�������ƣ�USART_Configuration
*�������ܣ�USART��ʼ��
*�������ã���ؿ⺯��
*�������룺��
*�����������
*�������أ���
*��ע��
*******************************************************************************/
void USART_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
    USART_ClockInitTypeDef  USART_ClockInitStructure;
 
    USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
    USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
    USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
    USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
    
  	/* Configure the USART3 synchronous paramters */
	USART_ClockInit(UART5, &USART_ClockInitStructure);
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;		  /*8bit*/
    USART_InitStructure.USART_StopBits = USART_StopBits_1;			  /*1��ֹͣλ*/
    USART_InitStructure.USART_Parity = USART_Parity_No ;			  /*û��У��λ*/
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; /*Ӳ��*/
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	/*USART1*/
	USART_InitStructure.USART_BaudRate = 115200;/*256000;					  ������*/
	USART_Init(USART1, &USART_InitStructure);
    USART_ITConfig(USART1, USART_IT_RXNE,ENABLE);
    USART_Cmd(USART1, ENABLE);
	
	/*USART3,GPRS*/
	USART_InitStructure.USART_BaudRate = 115200;/*256000;					  ������*/
	USART_Init(USART3, &USART_InitStructure);
    USART_ITConfig(USART3, USART_IT_RXNE,ENABLE);
    USART_Cmd(USART3, ENABLE);
	
	/*USART2,485*/
	USART_InitStructure.USART_BaudRate =38400;/* 115200;					  ������*/
	USART_Init(USART2, &USART_InitStructure);
    USART_ITConfig(USART2, USART_IT_RXNE,ENABLE);
    USART_Cmd(USART2, ENABLE);
	
}

/*******************************************************************************
*�������ƣ�NVIC_Configuration
*�������ܣ�NVIC��ʼ��
*�������ã���ؿ⺯��
*�������룺��
*�����������
*�������أ���
*��ע��
*******************************************************************************/
void NVIC_Configuration(void)
{
 NVIC_InitTypeDef NVIC_InitStructure;
  
#ifdef  VECT_TAB_RAM  
  /* Set the Vector Table base location at 0x20000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);   
#endif

  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  /*USART2*/
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /*USART1*/
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

    /*USART3*/
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /*TIM2*/
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

    /*TIM3*/
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
 
}

/*******************************************************************************
*�������ƣ�RCC_Configuration
*�������ܣ�RCC��ʼ��
*�������ã���ؿ⺯��
*�������룺��
*�����������
*�������أ���
*��ע��
*******************************************************************************/
void RCC_Configuration(void)
{
  /* RCC system reset(for debug purpose) */
  ErrorStatus HSEStartUpStatus;
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

	if(HSEStartUpStatus == SUCCESS)
	{
		/* HCLK = SYSCLK */
		RCC_HCLKConfig(RCC_SYSCLK_Div1); 
		
		/* PCLK2 = HCLK */
		RCC_PCLK2Config(RCC_HCLK_Div1); 
		
		/* PCLK1 = HCLK/2 */
		RCC_PCLK1Config(RCC_HCLK_Div2);
		RCC_ADCCLKConfig(RCC_PCLK2_Div6);
		/* Flash 2 wait state */
		FLASH_SetLatency(FLASH_Latency_2);
		/* Enable Prefetch Buffer */
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
		
		/* PLLCLK = 8MHz * 9 = 72 MHz */
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
		
		/* Enable PLL */ 
		RCC_PLLCmd(ENABLE);
		
		/* Wait till PLL is ready */
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
		{
		}
		
		/* Select PLL as system clock source */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		
		/* Wait till PLL is used as system clock source */
		while(RCC_GetSYSCLKSource() != 0x08)
		{
		}
	}
   
  /* Enable GPIOB, GPIOC and AFIO clocks */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | 
							RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA|
							RCC_APB2Periph_USART1|RCC_APB2Periph_SPI1|RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB1Periph_USART3,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);  
}

/*******************************************************************************
*�������ƣ�SPI_Configuration
*�������ܣ�SPI��ʼ��
*�������ã���ؿ⺯��
*�������룺��
*�����������
*�������أ���
*��ע��
*******************************************************************************/
void SPI_Configuration(void)
{
	SPI_InitTypeDef SPI_InitStructure;
	SPI_I2S_DeInit(SPI1);
	SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL=SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA=SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_16;
	SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial=7;
	SPI_Init(SPI1,&SPI_InitStructure);
	SPI_Cmd(SPI1,ENABLE);
}

/*******************************************************************************
*�������ƣ�CAN_Configuration
*�������ܣ�CAN��ʼ��
*�������ã���ؿ⺯��
*�������룺��
*�����������
*�������أ���
*��ע��
*******************************************************************************/
void CAN_Configuration(void)
{
//	CAN_InitTypeDef CAN_InitStructure;
//	CAN_FilterInitTypeDef CAN_FilterInitStructure;
//	
//	/* CAN1  register init */  
//	CAN_DeInit();
//	
//	CAN_StructInit(&CAN_InitStructure);
//	
//	/* CAN1 cell init */
//	CAN_InitStructure.CAN_TTCM = DISABLE;;/*DISABLE:��ֹʱ�䴥��ͨ��ģʽ ENABLE:��x��ʱ�䴥��ͨ��ģʽ*/
//	CAN_InitStructure.CAN_ABOM = DISABLE;/*DISABLE:�����CAN_MCR�Ĵ�����INRQλ������1�����0��һ��Ӳ����⵽128��11λ����������λ�����˳�����״̬��*/ 
//										 /*ENABLE:һ��Ӳ����⵽128��11λ����������λ�����Զ��˳�����״̬*/
//
//	CAN_InitStructure.CAN_AWUM = DISABLE;/*DISABLE:˯��ģʽͨ�����CAN_MCR�Ĵ�����SLEEPλ�����������*/
//										 /*ENABLE:˯��ģʽͨ�����CAN����,��Ӳ���Զ�����*/
//
//	CAN_InitStructure.CAN_NART = DISABLE;/*ENABLE;CAN����ֻ������1�Σ����ܷ��͵Ľ����Σ��ɹ���������ٲö�ʧ��*/
//	                                    /*DISABLE:CANӲ���ڷ��ͱ���ʧ��ʱ��һֱ�Զ��ش�ֱ�����ͳɹ�*/
//
//	CAN_InitStructure.CAN_RFLM = DISABLE;/*DISABLE:�ڽ������ʱFIFOδ��������������FIFO�ı���δ����������һ���յ��ı��ĻḲ��ԭ�еı���*/ 
//	                                     /*ENABLE:�ڽ������ʱFIFO��������������FIFO�ı���δ����������һ���յ��ı��ı���ʧ*/
//
//	CAN_InitStructure.CAN_TXFP = DISABLE;/*DISABLE:����FIFO���ȼ��ɱ��ĵı�ʶ��������*/ 
//										 /*ENABLE:��FIFO���ȼ��ɷ��������˳��������*/
//
//	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;/*CAN_Mode_LoopBack;//CAN_Mode_Normal;// CAN_Mode_Normal;//CANӲ������������ģʽ*/  
//	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;/*����ͬ����Ծ���1��ʱ�䵥λ*/ 
//	CAN_InitStructure.CAN_BS1 = CAN_BS1_2tq;/*ʱ���1Ϊ3��ʱ�䵥λ*/ 
//	CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;/*ʱ���2Ϊ4��ʱ�䵥λ*/ 
//	CAN_InitStructure.CAN_Prescaler = 12;/*(pclk1/((1+2+3)*12)) = 36Mhz/6/12 = 500Kbits*/ 
//	
//	/*Initializes the CAN1  */
//	CAN_Init(&CAN_InitStructure);
//	
//	/* CAN1 filter init */
//	CAN_FilterInitStructure.CAN_FilterNumber = 0;/*ָ���˴���ʼ���Ĺ�����0*/ 
//	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;;/*ָ���˹�����������ʼ������ģʽΪ��ʶ������λģʽ*/ 
//	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;/*�����˹�����λ��1��32λ������*/ 
//	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;/*�����趨��������ʶ����32λλ��ʱΪ��߶�λ��16λλ��ʱΪ��һ����*/ 
//	CAN_FilterInitStructure.CAN_FilterIdLow = (CAN_ID_EXT|CAN_RTR_DATA)&0xffff;/*�����趨��������ʶ����32λλ��ʱΪ��Ͷ�λ��16λλ��ʱΪ�ڶ�����*/
//	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0;/*�����趨���������α�ʶ�����߹�������ʶ����32λλ��ʱΪ��߶�λ��16λλ��ʱΪ��һ����*/ 
//	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x007;/*�����趨���������α�ʶ�����߹�������ʶ����32λλ��ʱΪ��Ͷ�λ��16λλ��ʱΪ�ڶ�����*/ 
//	
//	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;/*�趨��ָ���������FIFO0*/
//	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;/*ʹ�ܹ�����*/ 
//	CAN_FilterInit(&CAN_FilterInitStructure);
//	
////	GPIO_PinRemapConfig(GPIO_Remap1_CAN,ENABLE);
//	CAN_ITConfig( CAN_IT_FMP0|CAN_IT_EWG|CAN_IT_EPV|CAN_IT_BOF|CAN_IT_ERR, ENABLE);
}

/*******************************************************************************
*�������ƣ�FM25_Init
*�������ܣ�FM25��ʼ��
*�������ã���ؿ⺯��
*�������룺��
*�����������
*�������أ���
*��ע��
*******************************************************************************/
void FM25_Init(void)
{
	int i;
	FM25DIS;
	for(i=0;i<1000;i++);
	FM25EN;
	for(i=0;i<1000;i++);
	SPI1_SendOne(0x06);
	FM25DIS;
	for(i=0;i<1000;i++);
	FM25EN;
	for(i=0;i<1000;i++);
	SPI1_SendOne(0x01);
	SPI1_SendOne(0x40);
	FM25DIS;
}

/*******************************************************************************
*�������ƣ�TIM_Configuration
*�������ܣ�TIM��ʼ��
*�������ã���ؿ⺯��
*�������룺��
*�����������
*�������أ���
*��ע��
*******************************************************************************/
void TIM_Configuration(void)/*TIM2��CAP��ʼ������*/
{ 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;/*��ʱ����ʼ���ṹ*/
//	TIM_OCInitTypeDef  TIM_OCInitStructure;
	/*TIM_ICInitTypeDef TIM_ICInitStructure;         //ͨ�������ʼ���ṹ*/
	/* ---------------------------------------------------------------
	TIM2 Configuration: Output Compare Toggle Mode:
	TIM2CLK = 36*2 MHz, Prescaler = 0x1f3(499), TIM2 counter clock = 0.144 MHz
	CC1 update rate = TIM2 counter clock / CCR1_Val = 200 Hz		  100Hz 10ms
	CC2 update rate = TIM2 counter clock / CCR2_Val = 100 Hz
	CC3 update rate = TIM2 counter clock / CCR3_Val = 100 Hz
	CC4 update rate = TIM2 counter clock / CCR4_Val = ?? Hz
	--------------------------------------------------------------- */
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 99;          	  /*10KHz/(99+1) = 100Hz 10ms*/
	TIM_TimeBaseStructure.TIM_Prescaler =7199;          /*ʱ�ӷ�Ƶ 72000000/(7199+1)=10kHz*/
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    	/*ʱ�ӷָ�*/
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);			 /*������ʼ��*/
	
	
	/* Output Compare Toggle Mode configuration: Channel1 */
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//	TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
//	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
//	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Disable);
	
	/*TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = CCR2_Val;
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Disable);
	*/
	
	/*
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = CCR3_Val;
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Disable);	 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 255;
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);
	*/
	
	/* TIM enable counter */
	TIM_Cmd(TIM2, ENABLE);

    /* TIM IT enable */
	TIM_ITConfig(TIM2, TIM_IT_Update , ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = 99;          	  /*800KHz/(99+1) = 8000Hz 0.125ms*/
	TIM_TimeBaseStructure.TIM_Prescaler =1799;          /*ʱ�ӷ�Ƶ 72000000/(719+1)=800kHz*/
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    	/*ʱ�ӷָ�*/
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
//   	TIM_TimeBaseStructure.TIM_Period = 99;          	  /*10KHz/(99+1) = 100Hz 10ms*/
//	TIM_TimeBaseStructure.TIM_Prescaler =7199;          /*ʱ�ӷ�Ƶ 72000000/(7199+1)=10kHz*/
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    	/*ʱ�ӷָ�*/
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);	 /*�����������ʾ*/
	TIM_ITConfig(TIM3, TIM_IT_Update , ENABLE);	
}

/*******************************************************************************
*�������ƣ�IWDG_Configuration
*�������ܣ�IWDG��ʼ��
*�������ã���ؿ⺯��
*�������룺��
*�����������
*�������أ���
*��ע��
*******************************************************************************/
void IWDG_Configuration(void)
{
  /* IWDG timeout equal to x ms (the timeout may varies due to LSI frequency
     dispersion) */
  /* Enable write access to IWDG_PR and IWDG_RLR registers */
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

  /* IWDG counter clock: 40KHz(LSI) / 32 = 1.25 KHz */
  IWDG_SetPrescaler(IWDG_Prescaler_32);

  /* Set counter reload value to 4000 */
  IWDG_SetReload(4000);

  /* Reload IWDG counter */
  IWDG_ReloadCounter();

  /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
  IWDG_Enable();
}

/*******************************************************************************
*�������ƣ�I2C_Configuration
*�������ܣ�I2C��ʼ��
*�������ã���ؿ⺯��
*�������룺��
*�����������
*�������أ���
*��ע��
*******************************************************************************/
void I2C_Configuration(void)
{
	I2C_InitTypeDef I2C_InitStructure;
	I2C_Cmd(I2C1,DISABLE);
	I2C_InitStructure.I2C_Mode=I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle=I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1=0x64;
	I2C_InitStructure.I2C_Ack=I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed=200000;
	I2C_Init(I2C1,&I2C_InitStructure);
	I2C_Cmd(I2C1,ENABLE);
}

/*******************************************************************************
*�������ƣ�EXIT_Configuration
*�������ܣ�EXIT��ʼ��
*�������ã���ؿ⺯��
*�������룺��
*�����������
*�������أ���
*��ע��
*******************************************************************************/
void EXIT_Configuration(void)
{
//	EXTI_InitTypeDef EXTI_InitStructure;
//	EXTI_DeInit();
//
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource3);								   
//	/*����EXIT Line3�����ش���*/
//	EXTI_ClearITPendingBit(EXTI_Line3);
//	EXTI_InitStructure.EXTI_Line=EXTI_Line3;
//	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;
//	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
//	EXTI_Init(&EXTI_InitStructure);
}


/*******************************************************************************
*�������ƣ�GPRS_Init
*�������ܣ�M35ģ���ʼ��
*�������ã�
*�������룺��
*�����������
*�������أ���
*��ע����
*******************************************************************************/
void GPRS_Init(void)
{
	u16 i = 0;
	GPRS_RxCount = 0;
	USART_SendNData(USART3,GPRSCmd0,strlen((char *)GPRSCmd0));
	//while(0 == (GPRS_InitFlag&0x1));				/*����������*/
	for(i=0;i<2;i++)
	{
		LED_SelfCheck();
	}

	for(i=0;i<2;i++)
	{
		LED_SelfCheck();
	}
	USART_SendNData(USART3,GPRSCmd2,strlen((char *)GPRSCmd2)); /*�������ȴ洢λ��*/
	for(i=0;i<2;i++)
	{
		LED_SelfCheck();
	}
	USART_SendNData(USART3,GPRSCmd5,strlen((char *)GPRSCmd5)); /*ɾ�����ж���*/
	for(i=0;i<2;i++)
	{
		LED_SelfCheck();
	}
	GPRS_RxCount = 0;
	USART_SendNData(USART3,GPRSCmd7,strlen((char *)GPRSCmd7));  /*�رջ���*/
	for(i=0;i<5;i++)
	{
		LED_SelfCheck();
	}

	USART_SendNData(USART3,GPRSCmd15,strlen((char *)GPRSCmd15));  /*����GPRS��APN*/
	for(i=0;i<5;i++)
	{
		LED_SelfCheck();
	}
	sprintf((char *)(GPRSCmd6+16),"\"%03d.%03d.%03d.%03d\",\"%d\"\r\0",Server_IP[0],Server_IP[1],Server_IP[2],Server_IP[3],Server_PORT);
	USART_SendNData(USART3,GPRSCmd6,strlen((char *)GPRSCmd6));
	for(i=0;i<60;i++)
	{
		LED_SelfCheck();
	}


	GPRS_ConnectFlag = 0x1;
	GPRS_RxCount = 0;     /*���¿�ʼ����*/
	memset(GPRS_RxBuf,0,256);  /*M35���ջ��������*/	
}

/*******************************************************************************
* Function Name  : SysTick_Configuration
* Description    : Configures the SysTick to generate an interrupt each 1 millisecond.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SysTick_Configuration(void)
{
  /* Select AHB clock(HCLK) as SysTick clock source */
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);

  /* Set SysTick Priority to 3 */
  NVIC_SystemHandlerPriorityConfig(SystemHandler_SysTick, 3, 0);
   
  /* SysTick interrupt each 1ms with HCLK equal to 72MHz */
  SysTick_SetReload(72000);

  /* Enable the SysTick Interrupt */
  SysTick_ITConfig(ENABLE);
}

/*******************************************************************************
*�������ƣ�GPIO_Configuration
*�������ܣ�GPIO��ʼ��
*�������ã���ؿ⺯��
*�������룺��
*�����������
*�������أ���
*��ע��
*******************************************************************************/
void ADC_Configuration(void)//ADC��ʼ������
{
	ADC_InitTypeDef ADC_InitStructure;
	/* ADC1 configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_TempSensorVrefintCmd(ENABLE);    //ʧ���ڲ��ο�
	ADC_Init(ADC1, &ADC_InitStructure);
	
//	/* ADC1 regular channel14 configuration */ 
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_7Cycles5);
//	/* ADC1 regular channel14 configuration */ 
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_7Cycles5);
//	/* ADC1 regular channel14 configuration */ 
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, ADC_SampleTime_7Cycles5);
//	/* ADC1 regular channel14 configuration */ 
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 4, ADC_SampleTime_7Cycles5);
	
	
//	/* Enable ADC1 DMA */
//	ADC_DMACmd(ADC1, ENABLE);
	
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	
	/* Enable ADC1 reset calibaration register */   
//	ADC_ResetCalibration(ADC1);
//	/* Check the end of ADC1 reset calibration register */
//	while(ADC_GetResetCalibrationStatus(ADC1));
//	
//	/* Start ADC1 calibaration */
//	ADC_StartCalibration(ADC1);
//	/* Check the end of ADC1 calibration */
//	while(ADC_GetCalibrationStatus(ADC1));
//	 
//	/* Start ADC1 Software Conversion */ 
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/*******************************************************************************
*�������ƣ�DMA_Configuration
*�������ܣ�DMA��ʼ��
*�������ã���ؿ⺯��
*�������룺��
*�����������
*�������أ���
*��ע��
*******************************************************************************/
void DMA_Configuration(void)
{	
	DMA_InitTypeDef DMA_InitStructure;

	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;/*�����ַ*/
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADCBUF;/*�ڴ��ַ*/
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;/*dma���䷽����*/
	DMA_InitStructure.DMA_BufferSize = 80;/*����DMA�ڴ���ʱ�������ĳ��� word*/
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;/*����DMA���������ģʽ��һ������*/
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;/*����DMA���ڴ����ģʽ��*/
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;/*���������ֳ�*/
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;/*�ڴ������ֳ�*/
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;/*����DMA�Ĵ���ģʽ���������ϵ�ѭ��ģʽ*/
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;/*����DMA�����ȼ���*/
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;/*����DMA��2��memory�еı����������*/
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	
	DMA_Cmd(DMA1_Channel1, ENABLE);
}

/*******************************************************************************
*�������ƣ�WG_Reload
*�������ܣ����Ź�ˢ��
*�������ã��⺯��
*�������룺
*���������
*�������أ�
*��ע��
*******************************************************************************/
void WG_Reload(void)
{

}
/*******************************************************************************
*�������ƣ�RX8025_Init
*�������ܣ�ʵʱʱ�ӳ�ʼ��
*�������ã�I2C8025_Write1Byte
*�������룺��
*�����������
*�������أ���
*��ע��
*******************************************************************************/
void RX8025_Init(void)
{
	I2C8025_Write1Byte(0xe0,0x20);	
}
