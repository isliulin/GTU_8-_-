/*******************************************************************************
*文件名：sys_init.c
*文件功能：给水硬件初始化
*创建日期：2014-04-28
*文件作者：xue
*修改记录：
*******************************************************************************/
#include "include.h"

/*全局变量*/
vu16 CCR1_Val = 0x05A0;	  /*定时器2 1440   100Hz	   10ms*/

//u8 IP[20] = {"\"112.124.055.158\""};		/*服务器IP*/
//u8 PORT[10] = {"\"08018\""};				/*服务器端口*/


u8 GPRSCmd0[20] = {"AT+IPR=115200&W\r"};    /*M35波特率设置 ：115200*/
u8 GPRSCmd1[20] = {"AT+CGATT?\r"};          /*查询GPRS网络注册是否成功*/
u8 GPRSCmd2[20] = {"AT+CPMS=\"SM\"\r"};     /*短信息优先存储位置*/
u8 GPRSCmd3[20] = {"AT+CMGF=0\r\n"};          /*PDU格式短信息*/
u8 GPRSCmd4[20] = {"AT+CMGS="};             /*短信息发送*/
u8 GPRSCmd5[20] = {"AT+CMGD=1,4\r"};        /*删除所有短信息*/
u8 GPRSCmd6[50] = {"AT+QIOPEN=\"TCP\","};   /*TCP连接，后接“IP”,“PORT”\r*/
u8 GPRSCmd7[10]	= {"ATE0\r"};				/*关闭回显*/
u8 GPRSCmd8[20] = {"AT+CMGF=1\r"};          /*text格式短信息,用于删除短信*/
u8 GPRSCmd9[15] = {"AT+QISEND="};           /*TCP发送*/
u8 GPRSCmd10[15] = {"AT+QPOWD=1\r"};        /*M35模块关机命令*/
u8 GPRSCmd11[15] = {"AT+CSQ\r"};            /*查询信号质量*/
u8 GPRSCmd12[15] = {"AT+QIACT\r"};  		/*激活场景*/
u8 GPRSCmd13[15] = {"AT+QIDEACT\r"};  		/*注销场景*/
u8 GPRSCmd14[15] = {"AT+CMGR=1\r"};         /*读取第一条短信*/
u8 GPRSCmd15[25] = {"AT+QICSGP=1,\"CMNET\"\r"}; /*设置GPRS的APN*/

u8 GPRSPDU[300] = {"0011000D9168"};          /*短信息发送开始数据*/

u8 GPRS_InitFlag = 0;

u8 Canerror = 0;  /*CAN总线上次出错代码*/

/*两个参数，保证CAN总线在一定时间内初始化次数不会太多*/
u16 Count_CanReInitNum = 0;   /*统计一定时间内CAN总线初始化次数*/
u16 Count_CanReInitTime = 0;  /*统计一定时间内CAN总线初始化时间*/ 
/*函数定义*/
/*******************************************************************************
*函数名称：Sys_Init
*函数功能：给水初始化模块主函数
*函数调用：硬件初始化函数
*函数输入：无
*函数输出：无
*函数返回：无
*备注：
*******************************************************************************/
void SYS_Init(void)
{
	/*CPU内部控制器初始化*/
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
	/*铁电存储器初始化*/
	FM25_Init();
	RX8025_Init();
//	CAN_Configuration();

}

/*******************************************************************************
*函数名称：GPIO_Configuration
*函数功能：GPIO初始化
*函数调用：相关库函数
*函数输入：无
*函数输出：无
*函数返回：无
*备注：
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
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;		   /*串口接收上拉*/
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
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;		   /*串口接收上拉*/
	GPIO_Init(GPIOA, &GPIO_InitStructure);

		/*USART3*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;		   /*串口接收上拉*/
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/*C2DIR/FM25CS*/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_4;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	

	/*配置I2C1引脚：SCL和SDA*/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
		
	/*FM25 管脚配置*/
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
*函数名称：USART_Configuration
*函数功能：USART初始化
*函数调用：相关库函数
*函数输入：无
*函数输出：无
*函数返回：无
*备注：
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
    USART_InitStructure.USART_StopBits = USART_StopBits_1;			  /*1个停止位*/
    USART_InitStructure.USART_Parity = USART_Parity_No ;			  /*没有校验位*/
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; /*硬件*/
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	/*USART1*/
	USART_InitStructure.USART_BaudRate = 115200;/*256000;					  波特率*/
	USART_Init(USART1, &USART_InitStructure);
    USART_ITConfig(USART1, USART_IT_RXNE,ENABLE);
    USART_Cmd(USART1, ENABLE);
	
	/*USART3,GPRS*/
	USART_InitStructure.USART_BaudRate = 115200;/*256000;					  波特率*/
	USART_Init(USART3, &USART_InitStructure);
    USART_ITConfig(USART3, USART_IT_RXNE,ENABLE);
    USART_Cmd(USART3, ENABLE);
	
	/*USART2,485*/
	USART_InitStructure.USART_BaudRate =38400;/* 115200;					  波特率*/
	USART_Init(USART2, &USART_InitStructure);
    USART_ITConfig(USART2, USART_IT_RXNE,ENABLE);
    USART_Cmd(USART2, ENABLE);
	
}

/*******************************************************************************
*函数名称：NVIC_Configuration
*函数功能：NVIC初始化
*函数调用：相关库函数
*函数输入：无
*函数输出：无
*函数返回：无
*备注：
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
*函数名称：RCC_Configuration
*函数功能：RCC初始化
*函数调用：相关库函数
*函数输入：无
*函数输出：无
*函数返回：无
*备注：
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
*函数名称：SPI_Configuration
*函数功能：SPI初始化
*函数调用：相关库函数
*函数输入：无
*函数输出：无
*函数返回：无
*备注：
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
*函数名称：CAN_Configuration
*函数功能：CAN初始化
*函数调用：相关库函数
*函数输入：无
*函数输出：无
*函数返回：无
*备注：
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
//	CAN_InitStructure.CAN_TTCM = DISABLE;;/*DISABLE:禁止时间触发通信模式 ENABLE:允x许时间触发通信模式*/
//	CAN_InitStructure.CAN_ABOM = DISABLE;/*DISABLE:软件对CAN_MCR寄存器的INRQ位进行置1随后清0后，一旦硬件检测到128次11位连续的隐性位，就退出离线状态。*/ 
//										 /*ENABLE:一旦硬件检测到128次11位连续的隐性位，就自动退出离线状态*/
//
//	CAN_InitStructure.CAN_AWUM = DISABLE;/*DISABLE:睡眠模式通过清除CAN_MCR寄存器的SLEEP位，由软件唤醒*/
//										 /*ENABLE:睡眠模式通过检测CAN报文,由硬件自动唤醒*/
//
//	CAN_InitStructure.CAN_NART = DISABLE;/*ENABLE;CAN报文只被发送1次，不管发送的结果如何（成功、出错或仲裁丢失）*/
//	                                    /*DISABLE:CAN硬件在发送报文失败时会一直自动重传直到发送成功*/
//
//	CAN_InitStructure.CAN_RFLM = DISABLE;/*DISABLE:在接收溢出时FIFO未被锁定，当接收FIFO的报文未被读出，下一个收到的报文会覆盖原有的报文*/ 
//	                                     /*ENABLE:在接收溢出时FIFO被锁定，当接收FIFO的报文未被读出，下一个收到的报文被丢失*/
//
//	CAN_InitStructure.CAN_TXFP = DISABLE;/*DISABLE:发送FIFO优先级由报文的标识符来决定*/ 
//										 /*ENABLE:送FIFO优先级由发送请求的顺序来决定*/
//
//	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;/*CAN_Mode_LoopBack;//CAN_Mode_Normal;// CAN_Mode_Normal;//CAN硬件工作在正常模式*/  
//	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;/*重新同步跳跃宽度1个时间单位*/ 
//	CAN_InitStructure.CAN_BS1 = CAN_BS1_2tq;/*时间段1为3个时间单位*/ 
//	CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;/*时间段2为4个时间单位*/ 
//	CAN_InitStructure.CAN_Prescaler = 12;/*(pclk1/((1+2+3)*12)) = 36Mhz/6/12 = 500Kbits*/ 
//	
//	/*Initializes the CAN1  */
//	CAN_Init(&CAN_InitStructure);
//	
//	/* CAN1 filter init */
//	CAN_FilterInitStructure.CAN_FilterNumber = 0;/*指定了待初始化的过滤器0*/ 
//	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;;/*指定了过滤器将被初始化到的模式为标识符屏蔽位模式*/ 
//	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;/*给出了过滤器位宽1个32位过滤器*/ 
//	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;/*用来设定过滤器标识符（32位位宽时为其高段位，16位位宽时为第一个）*/ 
//	CAN_FilterInitStructure.CAN_FilterIdLow = (CAN_ID_EXT|CAN_RTR_DATA)&0xffff;/*用来设定过滤器标识符（32位位宽时为其低段位，16位位宽时为第二个）*/
//	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0;/*用来设定过滤器屏蔽标识符或者过滤器标识符（32位位宽时为其高段位，16位位宽时为第一个）*/ 
//	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x007;/*用来设定过滤器屏蔽标识符或者过滤器标识符（32位位宽时为其低段位，16位位宽时为第二个）*/ 
//	
//	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;/*设定了指向过滤器的FIFO0*/
//	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;/*使能过滤器*/ 
//	CAN_FilterInit(&CAN_FilterInitStructure);
//	
////	GPIO_PinRemapConfig(GPIO_Remap1_CAN,ENABLE);
//	CAN_ITConfig( CAN_IT_FMP0|CAN_IT_EWG|CAN_IT_EPV|CAN_IT_BOF|CAN_IT_ERR, ENABLE);
}

/*******************************************************************************
*函数名称：FM25_Init
*函数功能：FM25初始化
*函数调用：相关库函数
*函数输入：无
*函数输出：无
*函数返回：无
*备注：
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
*函数名称：TIM_Configuration
*函数功能：TIM初始化
*函数调用：相关库函数
*函数输入：无
*函数输出：无
*函数返回：无
*备注：
*******************************************************************************/
void TIM_Configuration(void)/*TIM2的CAP初始化函数*/
{ 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;/*定时器初始化结构*/
//	TIM_OCInitTypeDef  TIM_OCInitStructure;
	/*TIM_ICInitTypeDef TIM_ICInitStructure;         //通道输入初始化结构*/
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
	TIM_TimeBaseStructure.TIM_Prescaler =7199;          /*时钟分频 72000000/(7199+1)=10kHz*/
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    	/*时钟分割*/
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);			 /*基本初始化*/
	
	
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
	TIM_TimeBaseStructure.TIM_Prescaler =1799;          /*时钟分频 72000000/(719+1)=800kHz*/
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    	/*时钟分割*/
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
//   	TIM_TimeBaseStructure.TIM_Period = 99;          	  /*10KHz/(99+1) = 100Hz 10ms*/
//	TIM_TimeBaseStructure.TIM_Prescaler =7199;          /*时钟分频 72000000/(7199+1)=10kHz*/
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    	/*时钟分割*/
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);	 /*用于数码管显示*/
	TIM_ITConfig(TIM3, TIM_IT_Update , ENABLE);	
}

/*******************************************************************************
*函数名称：IWDG_Configuration
*函数功能：IWDG初始化
*函数调用：相关库函数
*函数输入：无
*函数输出：无
*函数返回：无
*备注：
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
*函数名称：I2C_Configuration
*函数功能：I2C初始化
*函数调用：相关库函数
*函数输入：无
*函数输出：无
*函数返回：无
*备注：
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
*函数名称：EXIT_Configuration
*函数功能：EXIT初始化
*函数调用：相关库函数
*函数输入：无
*函数输出：无
*函数返回：无
*备注：
*******************************************************************************/
void EXIT_Configuration(void)
{
//	EXTI_InitTypeDef EXTI_InitStructure;
//	EXTI_DeInit();
//
//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource3);								   
//	/*配置EXIT Line3上升沿触发*/
//	EXTI_ClearITPendingBit(EXTI_Line3);
//	EXTI_InitStructure.EXTI_Line=EXTI_Line3;
//	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
//	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;
//	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
//	EXTI_Init(&EXTI_InitStructure);
}


/*******************************************************************************
*函数名称：GPRS_Init
*函数功能：M35模块初始化
*函数调用：
*函数输入：无
*函数输出：无
*函数返回：无
*备注：无
*******************************************************************************/
void GPRS_Init(void)
{
	u16 i = 0;
	GPRS_RxCount = 0;
	USART_SendNData(USART3,GPRSCmd0,strlen((char *)GPRSCmd0));
	//while(0 == (GPRS_InitFlag&0x1));				/*波特率设置*/
	for(i=0;i<2;i++)
	{
		LED_SelfCheck();
	}

	for(i=0;i<2;i++)
	{
		LED_SelfCheck();
	}
	USART_SendNData(USART3,GPRSCmd2,strlen((char *)GPRSCmd2)); /*短信优先存储位置*/
	for(i=0;i<2;i++)
	{
		LED_SelfCheck();
	}
	USART_SendNData(USART3,GPRSCmd5,strlen((char *)GPRSCmd5)); /*删除所有短信*/
	for(i=0;i<2;i++)
	{
		LED_SelfCheck();
	}
	GPRS_RxCount = 0;
	USART_SendNData(USART3,GPRSCmd7,strlen((char *)GPRSCmd7));  /*关闭回显*/
	for(i=0;i<5;i++)
	{
		LED_SelfCheck();
	}

	USART_SendNData(USART3,GPRSCmd15,strlen((char *)GPRSCmd15));  /*设置GPRS的APN*/
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
	GPRS_RxCount = 0;     /*重新开始接收*/
	memset(GPRS_RxBuf,0,256);  /*M35接收缓冲区清空*/	
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
*函数名称：GPIO_Configuration
*函数功能：GPIO初始化
*函数调用：相关库函数
*函数输入：无
*函数输出：无
*函数返回：无
*备注：
*******************************************************************************/
void ADC_Configuration(void)//ADC初始化函数
{
	ADC_InitTypeDef ADC_InitStructure;
	/* ADC1 configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_TempSensorVrefintCmd(ENABLE);    //失能内部参考
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
*函数名称：DMA_Configuration
*函数功能：DMA初始化
*函数调用：相关库函数
*函数输入：无
*函数输出：无
*函数返回：无
*备注：
*******************************************************************************/
void DMA_Configuration(void)
{	
	DMA_InitTypeDef DMA_InitStructure;

	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;/*外设地址*/
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)ADCBUF;/*内存地址*/
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;/*dma传输方向单向*/
	DMA_InitStructure.DMA_BufferSize = 80;/*设置DMA在传输时缓冲区的长度 word*/
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;/*设置DMA的外设递增模式，一个外设*/
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;/*设置DMA的内存递增模式，*/
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;/*外设数据字长*/
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;/*内存数据字长*/
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;/*设置DMA的传输模式：连续不断的循环模式*/
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;/*设置DMA的优先级别*/
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;/*设置DMA的2个memory中的变量互相访问*/
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	
	DMA_Cmd(DMA1_Channel1, ENABLE);
}

/*******************************************************************************
*函数名称：WG_Reload
*函数功能：看门狗刷新
*函数调用：库函数
*函数输入：
*函数输出：
*函数返回：
*备注：
*******************************************************************************/
void WG_Reload(void)
{

}
/*******************************************************************************
*函数名称：RX8025_Init
*函数功能：实时时钟初始化
*函数调用：I2C8025_Write1Byte
*函数输入：无
*函数输出：无
*函数返回：无
*备注：
*******************************************************************************/
void RX8025_Init(void)
{
	I2C8025_Write1Byte(0xe0,0x20);	
}
