/*******************************************************************************
*文件名：basic_func.c
*文件功能：给水基本应用函数
*创建时间：2014-04-28
*作者：xue
*修改记录：
*******************************************************************************/
#include "include.h"

/*全局变量*/


u16 TimingDelay = 0;   /*延时计数*/

/*串口接收中断数据定义*/

u8 GPRS_ConnectFlag = 0; /*服务器连接标志*/
u8 GPRSComSynFlag = NO;   /*服务器通信同步标记*/
u16 GPRS_RxCount = 0;    /*GPRS缓冲接受计数*/
u8 GPRS_RxBuf[256] = {0};   /*M35模块接收缓冲区*/
u8 GPRS_Return[50] = {0};  /*AT命令响应缓存*/
u8 GPRS_ReturnCount = 0;   /*回显计数*/

u8 PCSet_ConnectFlag = 0; /*PCSet连接标志*/
u8 PCSetComSynFlag = NO;   /*PCSet通信同步标记*/
u16 PCSet_RxCount = 0;    /*PCSet缓冲接受计数*/
u8 PCSet_RxBuf[256] = {0};   /*PCSet模块接收缓冲区*/
u8 PCSet_ValueBuf[256] = {0};   /*PCSet有效数据缓冲区*/

u8 UART1TX_Buf[TXBUFMAX] = {0};   /*串口1发送中断缓存区*/
u16 UART1TX_Head = 0;         /*串口1发送中断发送起始位置*/
u16 UART1TX_Tail = 0;         /*串口1发送中断结束位置*/

u8 UART2TX_Buf[TXBUFMAX] = {0};   /*串口2发送中断缓存区*/
u16 UART2TX_Head = 0;         /*串口2发送中断发送起始位置*/
u16 UART2TX_Tail = 0;         /*串口2发送中断结束位置*/

u8 UART3_LEN = 0;             /*串口3帧长度*/
u8 UART3ComSyn1Flag = NO;	  /*串口3第一同步导码*/
u8 UART3ComSyn2Flag = NO;	  /*串口3第二同步导码*/
u8 UART3ComSyn3Flag = NO;	  /*串口3第三同步导码*/	/*第三、四导码根据帧长度决定是否启用*/
u8 UART3ComSyn4Flag = NO;	  /*串口3第四同步导码*/
u8 UART3_Buf[256] = {0};      /*串口3缓存区*/
u8 UART3_Count = 0;           /*串口3接收计数*/



/*******************************************************************************
*函数名称：UART5_Handle
*函数功能：UART5接收
*函数调用：
*函数输入：串口接收数据
*函数输出：处理后串口缓冲区
*函数返回：无
*备注：
*******************************************************************************/
void UART5_Handle(u8 sData)
{	
}


/*******************************************************************************
*函数名称：PCValueSet_Handle
*函数功能：PC参数设置接收
*函数调用：
*函数输入：串口接收数据
*函数输出：处理后串口缓冲区
*函数返回：无
*备注：
*******************************************************************************/
void PCValueSet_Handle(u8 sData)
{
	u16	 checksum = 0, x = 0;
	if(sData==PCSETSYN1)
	 {         
       if(PCSetComSynFlag==NO)	 /*搜索IPORTSYN1字符.*/
	   { 
           PCSet_RxBuf[0]=sData;
           return;
        }
     }	

	 if( (sData==PCSETSYN2)&&(PCSet_RxBuf[0]==PCSETSYN1)&&(PCSetComSynFlag==NO) )	   /*搜索PCSETSYN2字符，开始计数.*/
	 { 
         PCSet_RxBuf[1]=sData;
         PCSet_RxCount=2;
         PCSetComSynFlag=YES;  /*同步建立*/
         return;
     }

	 if(PCSetComSynFlag==NO)
	 {
         PCSet_RxCount=0;   /*其他非正常状态，重新搜索帧同步*/
         PCSet_RxBuf[0]=0x00;
         PCSet_RxBuf[1]=0x00;
		 return;
     }

	 /*同步已经建立*/
	 PCSet_RxBuf[PCSet_RxCount]=sData;
	 PCSet_RxCount++;
	 if(PCSet_RxCount<3)	   /*尚未接收到帧长度码，退出。*/
	 {   
	     return;
	 }	 
	 x=(PCSet_RxBuf[2]+5); /*计算帧字节数*/
	 if( PCSet_RxCount>=x )		  /*一帧接收完毕*/
	 {
//		 checksum1=PCSet_RxBuf[x-2];      /*crc校验*/
//		 checksum1*=256;
//		 checksum1+=PCSet_RxBuf[x-1];    	    
		 checksum = crc16_ccitt(PCSet_RxBuf,PCSet_RxBuf[2]+5);	  
		 if( 0==checksum)		  /*帧校验匹配*/
		 { 
			/*代码区*/
			if(0x80 > PCSet_RxBuf[3])
			{
				memset(PCSet_ValueBuf,0,256);
				memcpy(PCSet_ValueBuf,PCSet_RxBuf,256);
				Flag_GetPCSet = YES;
				memset(PCSet_RxBuf,0,256);
			}

		 }
		 PCSet_RxCount=0;
		 PCSetComSynFlag=NO;
	 }	
}

/*******************************************************************************
*函数名称：GPRSCom_Handle
*函数功能：GPRS接收处理
*函数调用：
*函数输入：GPRS_InitFlag,GPRS_ConnectFlag,GPRS_RxBuf,GPRS_RxCount
*函数输出：GPRS_InitFlag,GPRS_ConnectFlag,GPRS_RxBuf,GPRS_RxCount
*函数返回：无
*备注：
*******************************************************************************/
void GPRSCom_Handle(u8 sData)
{
	u16	 checksum2 = 0, x = 0;
	char * tmpchar =NULL ;
	Flag_RecvingGPRS = YES;
	Count_TimGPRSRecv = COUNT_TIMGPRSRECV;   /*延时发送*/
	if(sData==GPRSSYN1)
	 {         
       if(GPRSComSynFlag==NO)	 /*搜索GPRSSYN1字符.*/
	   { 
           GPRS_RxBuf[0]=sData;
           return;
        }
     }	

	 if( (sData==GPRSSYN2)&&(GPRS_RxBuf[0]==GPRSSYN1)&&(GPRSComSynFlag==NO) )	   /*搜索GPRSSYN2字符，开始计数.*/
	 { 
         GPRS_RxBuf[1]=sData;
         GPRS_RxCount=2;
         GPRSComSynFlag=YES;  /*同步建立*/
         return;
     }

	 if(GPRSComSynFlag==NO)
	 {
         GPRS_RxCount=0;   /*其他非正常状态，重新搜索帧同步*/
         GPRS_RxBuf[0]=0x00;
         GPRS_RxBuf[1]=0x00;
		 GPRS_Return[GPRS_ReturnCount] = sData;   /*回显接收*/
		 GPRS_ReturnCount ++;
		 if(50 <= GPRS_ReturnCount)
		 {
		 	GPRS_ReturnCount = 0;
		 }
		 if((sData == '\r')&& 4 <= GPRS_ReturnCount)		   /*命令回复*/
		 {
		 	GPRS_Return[30] = 0;
			GPRS_ReturnCount = 0;
			if((0 == Flag_GPRSReStep)&&(strstr((char *)GPRS_Return,"SEND OK"))) 		   /*重启过程中不判断*/
			{
				Count_TimConnGPRS = COUNT_TIMCONNGPRS;  /*连接存在*/
//				Count_GPRSConnectEvent = COUNT_GPRSCONNECTEVENT;
				memset(GPRS_Return,0,30);	
			}
			else if((strstr((char *)GPRS_Return,"CLOSED")))					 /*接收到CLOSED*/
			{
				Flag_GprsGetClose = YES;
				memset(GPRS_Return,0,30);
			}
			else if(NULL != (tmpchar = strstr((char *)GPRS_Return,"+CSQ:")))			/*手机信号获取*/
			{
				if(',' == tmpchar[8])
				{
					Signal_Quality = (tmpchar[6]-0x30)*10;
					Signal_Quality += (tmpchar[7] - 0x30);	
				}
				else if(',' == tmpchar[7])
				{
					Signal_Quality = (tmpchar[6]-0x30);
				}	
			}
			else if(NULL != strstr((char *)GPRS_Return,"+CMTI:"))	  /*接收到短信*/
			{
				Flag_GetMsg = YES;
				Count_TimGPRSRecv = 1000; /*为接收短信预留30s处理时间*/
				Count_GetMsgBuf = 0;   /*短信处理接收计数*/
				Flag_GetWrongMsg = NO; 
				Flag_GetMsgOver = NO;
				Count_GetMsgStep = COUNT_GETMSGSTEP;
			}
		 }
	     return;
     }

	 /*同步已经建立*/
	 GPRS_RxBuf[GPRS_RxCount]=sData;
	 GPRS_RxCount++;
	 if(GPRS_RxCount<3)	   /*尚未接收到帧长度码，退出。*/
	 {   
	     return;
	 }	 
	 x=(GPRS_RxBuf[2]+5); /*计算帧字节数*/
	 if(x > 256) /*帧长出错*/
	 {
	 	 GPRS_RxCount=0;   /*其他非正常状态，重新搜索帧同步*/
         GPRS_RxBuf[0]=0x00;
         GPRS_RxBuf[1]=0x00;
		 return;	
	 }
	 if( GPRS_RxCount>=x )		  /*一帧接收完毕*/
	 {
//		 checksum1=GPRS_RxBuf[x-2];      /*crc校验*/
//		 checksum1*=256;
//		 checksum1+=GPRS_RxBuf[x-1];    	    
		 checksum2 = crc16_ccitt(GPRS_RxBuf,GPRS_RxBuf[2]+5);	  
		 if( 0 ==checksum2)		  /*帧校验匹配*/
		 { 
		 	/*代码区*/
			if(GPRS_RxBuf[3] >= 0x80)		/*上一条命令处理完后，才接收新的命令*/
			{
				
			 Count_TimConnGPRS = COUNT_TIMCONNGPRS;  /*连接存在*/
//			 Count_TimSendStop = 0;   /*重新定时发送或接收停止*/
				

			if(0xfa == GPRS_RxBuf[3]) /*程序转跳命令，特殊命令*/
			{
			   if(0 == Count_RemoteCommond)
			   {
				Count_TimRemote = COUNT_TIMREMOTE; /*开始计时*/
			   }
			   Count_RemoteCommond ++;
			   if(3 <= Count_RemoteCommond) /*转跳命令有效*/
			   {
			   	Flag_RemoteCommond = YES;
			   }
			}
			else
			{
				for(x=0;x<3;x++)
				{
					if(0xAA != Flag_RecvCommondBuf[x])
					{
						Flag_RecvCommondBuf[x] = 0xAA;
						break;
					}
				}
				if(3 != x)	   /*如果接收缓存满，舍弃该命令*/
				{
					memset(GPRS_RecvCommondBuf[x],0,256);
					memcpy(GPRS_RecvCommondBuf[x],GPRS_RxBuf,256);
					Flag_NeedGPRSRecv = YES;
				}
//				Flag_GetGPRSValue |= (0x1<<((GPRS_ValueBuf[3]&0x1f)-1));   /*获取GPRS命令码*/  /*由于只有一个缓存区，如果有连续两条带参数指令*/
//																	 /*依然不能够全部执行，仅限于多查询指令*/
//				Flag_GetGPRS = YES;
			}
			}  
		 }
		 else
		 {
			Recv_SerCom = 0x7F;
		 	Reply_Servalue = 0x00;
		 	Flag_ServerSend[31] = 0xAA;	
		 }
		 GPRSComSynFlag=NO;
		 GPRS_RxCount=0; 			 
    }	
}

/*******************************************************************************
*函数名称：GetMsgCom_Handle
*函数功能：短信接收处理
*函数调用：
*函数输入：
*函数输出：
*函数返回：无
*备注：
*******************************************************************************/
void GetMsgCom_Handle(u8 sData)
{
	GetMsg_Buf[Count_GetMsgBuf++] = sData;
	if(Count_GetMsgBuf >= 390)
	{
		Count_GetMsgBuf	= 0;
		Flag_GetWrongMsg = YES; /*接收到错误短信*/
		memset(GetMsg_Buf,0,400);
	}
	else if(Count_GetMsgBuf > 30 && '\n' == sData)
	{
		if(NULL != (strstr((char *)GetMsg_Buf,GETMSGWORD)))
		{
			Flag_GetMsgOver = YES; /*短信接受完成*/
		}
		Count_GetMsgBuf = 0;
		memset(GetMsg_Buf,0,400);
//		else if(NULL != (strstr((char*)GetMsg_Buf,"\r\nOK\r\n"))) /*其余接收清零重新接收*/
//		{
//			Count_GetMsgBuf = 0;
//			memset(GetMsg_Buf,0,400);
//		}	
	}	
}

/*******************************************************************************
*函数名称：FM25_WR_NBYTE
*函数功能：向FM25指向地址，写入指定数目字符
*函数调用：SPI2_SendOne
*函数输入：u32 add,FM25地址，u8 *pt 写入数据起始地址，u32 num写入数目
*函数输出：无
*函数返回：无
*备注：
*******************************************************************************/
void FM25_WR_NBYTE(u32 add,u8 *pt,u32 num)
{
	int i=0;
	//LEDON;
	FM25DIS;
	for(i=0;i<800;i++);
	FM25EN;
	for(i=0;i<800;i++);
	SPI1_SendOne(0x06);
	FM25DIS;
	for(i=0;i<800;i++);
	FM25EN;
	for(i=0;i<800;i++);
	SPI1_SendOne(0x02);
	SPI1_SendOne(add/256);
	SPI1_SendOne(add&0x00ff);
	for(i=0;i<num;i++)
	{
		SPI1_SendOne(*pt);
		pt++;
	}
	FM25DIS;
	for(i=0;i<1000;i++);
	//LEDOFF;
}


/*******************************************************************************
*函数名称：FM25_RE_NBYTE
*函数功能：从FM25指向地址，读出指定数目字符
*函数调用：SPI2_SendOne
*函数输入：u32 add,FM25地址，u8 *pt 读出数据存放地址，u32 num读出数目
*函数输出：无
*函数返回：无
*备注：
*******************************************************************************/
void FM25_RE_NBYTE(u32 add,u8 *pt,u32 num)
{
	int i=0;
	FM25DIS;
	for(i=0;i<800;i++);
	FM25EN;
	for(i=0;i<800;i++);
	SPI1_SendOne(0x03);
	SPI1_SendOne(add/256);
	SPI1_SendOne(add&0x00ff);
	for(i=0;i<num;i++)
	{
		*pt=SPI1_SendOne(0x00);
		pt++;	
	}
	for(i=0;i<1000;i++);
	FM25DIS;
}
/*******************************************************************************
*函数名称：SPI2_SendOne
*函数功能：向SPI2写入一个字符
*函数调用：相关库函数
*函数输入：u8 sData,写入的数据
*函数输出：无
*函数返回：读出的字符（主模式读时）
*备注：
*******************************************************************************/
u8 SPI1_SendOne(u8 sData)
{
	u16 i=2000U;
	SPI_I2S_SendData(SPI1,sData);
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET&&i!=0U)
	{
		i--;
	}
	i=2000U;
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET&&i!=0U)
	{
		i--;
	}
	return SPI_I2S_ReceiveData(SPI1);
}

/*******************************************************************************
*函数名称：USART_SendOneData
*函数功能：向USART写入一个字符
*函数调用：相关库函数
*函数输入：u8 sData,写入的数据;USART_TypeDef * USART,相应串口号
*函数输出：无
*函数返回：无
*备注：
*******************************************************************************/
void USART_SendOneData(USART_TypeDef * USART,u8 sData)
{
	u32 i=40000U;
//	if(USART1 == USART)		/*串口1采用中断发送*/
//	{
//		i = 500000; /*约0.5s*/
//		while(UART1TX_Tail+1 > TXBUFMAX && i != 0U){i --;};
//		UART1TX_Buf[UART1TX_Tail++] = sData;
//		USART_ITConfig(USART1,USART_IT_TXE,ENABLE);
//
//	}
//	else if(USART2 == USART)  /*串口2采用中断发送*/
//	{
//		i = 500000;
//		while(UART2TX_Tail+1 > TXBUFMAX && i != 0U){i --;};
//		UART2TX_Buf[UART2TX_Tail++] = sData;
//		USART_ITConfig(USART2,USART_IT_TXE,ENABLE);
//
//	}
//	else
//	{
		i = 40000U;
		USART_SendData(USART, sData);          /*发送字符*/
		while(USART_GetFlagStatus(USART, USART_FLAG_TXE) == RESET && i!=0U){i--;}/*等待发送结束*/
//	}
}

/*******************************************************************************
*函数名称：USART_SendNData
*函数功能：向USART写入N个字符
*函数调用：相关库函数
*函数输入：u8 *buf,写入的数据起始地址;USART_TypeDef * USART,相应串口号 ；u32 len 写入长度
*函数输出：无
*函数返回：无
*备注：
*******************************************************************************/
void USART_SendNData(USART_TypeDef * USART,u8* buf,u32 len)
{
	u32 i=0;
	u16 tmplen = 0;
//	if(USART1 == USART)	 /*串口1采用中断发送*/
//	{
//		USART_ITConfig(USART1,USART_IT_TXE,DISABLE);
//		i = 1000; /*等待上一个字节发送完成*/   
//		while(USART_GetFlagStatus(USART, USART_FLAG_TXE) == RESET && i!=0U){i--;}/*等待发送结束*/
//		if(len + UART1TX_Tail > TXBUFMAX)  /*待发送长度大于最大缓冲区*/
//		{
//			tmplen = TXBUFMAX-UART1TX_Tail;
//			memcpy(UART1TX_Buf+UART1TX_Tail,buf,tmplen);
//			UART1TX_Tail = TXBUFMAX;
//			USART_ITConfig(USART1,USART_IT_TXE,ENABLE);
//			i = 1000000; /*约1s*/
//			while(len - tmplen + UART1TX_Tail > TXBUFMAX && i != 0U){i--;};
//			USART_ITConfig(USART1,USART_IT_TXE,DISABLE);
//			i = 1000; /*等待上一个字节发送完成*/   
//			while(USART_GetFlagStatus(USART, USART_FLAG_TXE) == RESET && i!=0U){i--;}/*等待发送结束*/
//			memcpy(UART1TX_Buf+UART1TX_Tail,buf+tmplen,len - tmplen);
//			UART1TX_Tail += (len-tmplen);
//			USART_ITConfig(USART1,USART_IT_TXE,ENABLE);
//		}
//		else
//		{
//			memcpy(UART1TX_Buf+UART1TX_Tail,buf,len);
//			UART1TX_Tail += len;
//			USART_ITConfig(USART1,USART_IT_TXE,ENABLE);	
//		}	
//	}
//	else if(USART2 == USART)	 /*串口2采用中断发送*/
//	{
//		USART_ITConfig(USART2,USART_IT_TXE,DISABLE);
//		i = 1000; /*等待上一个字节发送完成*/   
//		while(USART_GetFlagStatus(USART, USART_FLAG_TXE) == RESET && i!=0U){i--;}/*等待发送结束*/
//		if(len + UART2TX_Tail > TXBUFMAX)  /*待发送长度大于最大缓冲区*/
//		{
//			tmplen = TXBUFMAX-UART2TX_Tail;
//			memcpy(UART2TX_Buf+UART2TX_Tail,buf,tmplen);
//			UART2TX_Tail = TXBUFMAX;
//			USART_ITConfig(USART2,USART_IT_TXE,ENABLE);
//			i = 1000000;
//			while(len - tmplen + UART2TX_Tail > TXBUFMAX && i != 0U){i--;};
//			USART_ITConfig(USART2,USART_IT_TXE,DISABLE);
//			i = 1000; /*等待上一个字节发送完成*/   
//			while(USART_GetFlagStatus(USART, USART_FLAG_TXE) == RESET && i!=0U){i--;}/*等待发送结束*/
//			memcpy(UART2TX_Buf+UART2TX_Tail,buf+tmplen,len - tmplen);
//			UART2TX_Tail += (len-tmplen);
//			USART_ITConfig(USART2,USART_IT_TXE,ENABLE);
//		}
//		else
//		{
//			memcpy(UART2TX_Buf+UART2TX_Tail,buf,len);
//			UART2TX_Tail += len;
//			USART_ITConfig(USART2,USART_IT_TXE,ENABLE);	
//		}	
//	}
//	else
//	{
	for(i = 0;i<len;i++)
	{
		USART_SendOneData(USART,buf[i]);
	}
//	}
}

/*******************************************************************************
*函数名称：I2C8025_Write1Byte
*函数功能：通过I2C向8025写入一个字节
*函数调用：
*函数输入：地址add，内容comd
*函数输出：无
*函数返回：i(==0写入失败，!=0写入成功）
*备注：
*******************************************************************************/
u32 I2C8025_Write1Byte(u8 add,u8 comd)
{
	u32 i=1;
//	u8 k=0;
//	i=I2C_AcknowPolling();
	I2C_AcknowledgeConfig(I2C1, ENABLE);//使能或者失能指定 I2C的应答功能
//	if(i!=0)
//	{
		I2C_GenerateSTART(I2C1,ENABLE);
		i=I2C1->DR;
		i=20000;
		while((!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT))&&(i!=0))
			i--;
		if(i!=0)
		{
			i=40000;
			I2C_Send7bitAddress(I2C1,0x64,I2C_Direction_Transmitter);
			while((!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))&&(i!=0))
				i--;
			if(i!=0)
			{
				i=20000;
				I2C_SendData(I2C1,add);
				while((!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED))&&(i!=0))
					i--;
				if(i!=0)
				{
					i=20000;
					I2C_SendData(I2C1,comd);
					while((!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED))&&(i!=0))
						i--;
				}
			}
		}
		I2C_GenerateSTOP(I2C1,ENABLE);
		if(i==0)
		{
//			UART5_SendOneData(0xe1);
			I2C_Configuration();
		}
//	}
	return i;
}

/*******************************************************************************
*函数名称：I2C8025_Read6Byte
*函数功能：通过I2C从8025读出6个字节
*函数调用：
*函数输入：地址add，存放地址
*函数输出：实时时钟
*函数返回：无
*备注：
*******************************************************************************/
void I2C8025_Read6Byte(u8 add,u8 *str)
{
	u32 i=1;
	u8 k=0;
	u8 ch[7];
	u8 erorr=0x00;
	for(i=0;i<7;i++)
		ch[i]=0x00;
//	i=I2C_AcknowPolling();
	if(i!=0)
	{
		I2C_AcknowledgeConfig(I2C1, ENABLE);//使能或者失能指定 I2C的应答功能
		i=20000;
		I2C_GenerateSTART(I2C1,ENABLE);
		k=I2C1->DR;
		while((!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT))&&(i!=0))
			i--;
		if(i!=0)
		{
			i=20000;
			I2C_Send7bitAddress(I2C1,0x64,I2C_Direction_Transmitter);
			while((!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))&&(i!=0))
				i--;
			if(i!=0)
			{	
				i=20000;
				I2C_SendData(I2C1,add);
				while((!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED))&&(i!=0))
					i--;
				if(i!=0)
				{
					i=20000;
					I2C_GenerateSTART(I2C1,ENABLE);
					while((!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT))&&(i!=0))
						i--;
					if(i!=0)
					{
						i=20000;
						I2C_Send7bitAddress(I2C1,0x64,I2C_Direction_Receiver);
						while((!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))&&(i!=0))
							i--;
						if(i!=0)
						{
							for(k=0;k<7;k++)
							{
								i=20000;
								if(k==6)
									I2C_AcknowledgeConfig(I2C1, DISABLE);//使能或者失能指定 I2C的应答功能
								while((!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED))&&(i!=0))
									i--;
								if(i!=0)
									ch[k]=I2C1->DR;
								else
								{									
									erorr=0xa7;
									break;
								}
							}
						}
						else
							erorr=0xa6;
					}
					else
						erorr=0xa5;
				}
				else
					erorr=0xa4;
			}
			else
				erorr=0xa3;			
		}
		else
			erorr=0xa2;
	}
	else
		erorr=0xa1;
	I2C_GenerateSTOP(I2C1,ENABLE);
	if(i==0)
	{
//		Flag_I2CError = YES;
//		Count_I2CError = COUNT_I2CERROR;
		I2C_Configuration();
	}
	for(k=0;k<7;k++)
	{
		if(k<3)
			*(str-k)=ch[k];
		else if(k>3)
			*(str-k+1)=ch[k];
	}
	i = erorr;
}

/*******************************************************************************
*函数名称：CRC16
*函数功能：16位crc计算，用于同电能表通信，多项式为A001
*函数调用：
*函数输入：u8*,需要校验数据缓存地址，u8 校验数目最大255个
*函数输出：无
*函数返回：Crc计算结果
*备注：
*******************************************************************************/
u16 CRC16(u8 *tmp,u8 num)
{
	u16 crc = 0xFFFF;   /*预设值*/
	u8 i=0,j=0;
	u8 tmpcrc = 0;
	for(i=0;i<num;i++)
	{
		tmpcrc = crc&0xff;
		tmpcrc ^= tmp[i];
		crc &=0xff00;
		crc |=(tmpcrc|0x0);     /*低8位异或*/

		for(j=0;j<8;j++)
		{
			if(crc&0x1)   /*最低位为1*/
			{
				crc >>= 1;  /*右移一位*/
				crc &=~(0x8000); /*最高位补0*/
				crc ^= 0xA001;   /*与A001H异或*/
			}
			else		    /*为0*/
			{			   
				crc >>= 1;		/*右移1位*/
			}
		}
	}
	return crc;
}

/*******************************************************************************
*函数名称：Delay
*函数功能：延时nms
*函数调用：
*函数输入：延时时间,单位ms
*函数输出：无
*函数返回：无
*备注：
*******************************************************************************/
void Delay(u16 nms)
{
	  /* Enable the SysTick Counter */
  SysTick_CounterCmd(SysTick_Counter_Enable);
  
  TimingDelay = nms;

  while(TimingDelay != 0)
  {
  
  }

  /* Disable the SysTick Counter */
  SysTick_CounterCmd(SysTick_Counter_Disable);
  /* Clear the SysTick Counter */
  SysTick_CounterCmd(SysTick_Counter_Clear);
	
}

/*******************************************************************************
*函数名称：Send_To_Server
*函数功能：把数据发送到服务器
*函数调用：
*函数输入：数据缓存地址，数据长度、Flag_SendingGPRS
*函数输出：Flag_SendingGPRS
*函数返回：无
*备注：	最长发送字节999字节	，超过要做修改
*******************************************************************************/

//#if YES == FLAGIPORT && 0xaa == IPORTFUNCTION  /*通过Iport连接服务器*/
void Send_To_Server(u8*buf,u16 len)
{
	u32 i = 0;
//	Count_TimSendStop = 0;   /*重新定时发送或接收停止*/
	Flag_SendingGPRS = YES;
	Count_TimGPRSSendInt = COUNT_TIMGPRSSENDINT;
	Count_TimGPRSBeat = COUNT_TIMGPRSBEAT;
	GPRSCmd9[10] = ((len%1000)/100)+0x30;  /*最长发送字节999字节*/
	GPRSCmd9[11] = ((len%100)/10)+0x30;
	GPRSCmd9[12] = len%10 + 0x30;
	GPRSCmd9[13] = '\n';
	USART_SendNData(USART3,GPRSCmd9,14);	 
	Delay(20);
	USART_SendNData(USART3,buf,len);
//	USART_SendOneData(USART2,0x1a);
	for(i=0;i<15500;i++);   /*等待响应，暂定*/
}



/*******************************************************************************
*函数名称：crc16_ccitt
*函数功能：16位CRC校验，用于同服务器通信，多项式为1021
*函数调用：无
*函数输入：u8*tmp 校验起始地址，u8 num 校验长度
*函数出入：16CRC校验结果
*函数输出：无
*
*******************************************************************************/
u16 crc16_ccitt(u8 *tmp,u8 num)
{
	u16 CrcTmp = 0x0;
	u8 i=0,j=0;
	u16 s = 0;
	u16 tmpcrc = 0;
	for(i=0;i<num;i++)
	{
		tmpcrc = tmp[i]<<8;
		tmpcrc &= 0xff00;			 
		CrcTmp ^= tmpcrc;			 /*高8位异或*/
		for(j=0;j<8;j++)
		{
			s = CrcTmp&0x8000;
			CrcTmp <<= 1;
			CrcTmp &= ~(0x0001);
			if(s)
			{
				CrcTmp ^= 0x1021;
			}
		}
	}
	return CrcTmp;
}


/*******************************************************************************
*函数名称：USART3_Handle    
*函数功能：串口3数据处理函数
*函数输入：待处理数据
*函数输出：无
*函数返回：无
*备注：可兼容三种不同水位仪
*******************************************************************************/
void USART3_Handle(u8 tmp)
{
//	u16 tmpsum = 0;
//	u16 sum	 = 0;
//	u16 i = 0;
//	/*同步码1*/
//	if(0x7f == tmp && NO == UART3ComSyn1Flag)
//	{
//		UART3ComSyn1Flag = YES;
//		UART3_Buf[0] = tmp;
//		return;
//	}
//	else if(0xaa == tmp && NO == UART3ComSyn1Flag)
//	{
//		UART3ComSyn1Flag = YES;
//		UART3_Buf[0] = tmp;
//		return;	
//	}
//	else if(0x1a == tmp && NO == UART3ComSyn1Flag)
//	{
//		UART3ComSyn1Flag = YES;
//		UART3_Buf[0] = tmp;
//		return;
//	}
//	else if(NO == UART3ComSyn1Flag)
//	{
//		UART3_Buf[0] = 0x00;
//		return;
//	}
//
//	/*同步码2*/
//	if((0x91 == tmp || 0x92 == tmp || 0x93 == tmp || 0x94 == tmp || 0x95 == tmp)
//		&& YES == UART3ComSyn1Flag && NO == UART3ComSyn2Flag && 0x7f == UART3_Buf[0])
//	{
//		UART3ComSyn2Flag = YES;
//		UART3_Buf[1] = tmp;
//		UART3_LEN = 6;
//		UART3_Count = 2;
//		return;
//	}
//	else if(0xbf == tmp && YES == UART3ComSyn1Flag && NO == UART3ComSyn2Flag)
//	{
//		UART3ComSyn2Flag = YES;
//		UART3_Buf[1] = tmp;
//		return;	
//	}
//	else if(0x2f == tmp && YES == UART3ComSyn1Flag && NO == UART3ComSyn2Flag && 0x1a == UART3_Buf[0])
//	{
//		UART3ComSyn2Flag = YES;
//		UART3_Buf[1] = tmp;
//		return;
//	}
//	else if(NO == UART3ComSyn2Flag)
//	{
//		UART3_Buf[0] = 0x00;
//		UART3ComSyn1Flag = NO;
//		return;
//	}
//
//	/*同步码3*/
//	if(0xcb == tmp && YES == UART3ComSyn1Flag && YES == UART3ComSyn2Flag && NO == UART3ComSyn3Flag
//	   && 0xbf == UART3_Buf[1] && 0xaa == UART3_Buf[0])
//	{
//		UART3ComSyn3Flag = YES;
//		UART3_Buf[2] = tmp;
//		return;	
//	}
//	else if(0x3b == tmp && YES == UART3ComSyn1Flag && YES == UART3ComSyn2Flag && NO == UART3ComSyn3Flag
//		    && 0x2f == UART3_Buf[1]&& 0x1a == UART3_Buf[0])
//	{
//		UART3ComSyn3Flag = YES;
//		UART3_Buf[2] = tmp;
//		return;
//	}
//	else if( NO == UART3ComSyn1Flag || NO == UART3ComSyn2Flag)  /*帧长不正确*/
//	{
//		UART3ComSyn1Flag = NO;
//		UART3ComSyn2Flag = NO;
//		UART3_Buf[0] = 0x00;
//		UART3_Buf[1] = 0x00;
//		return;	
//	}
//
//	/*同步码4*/
//	if(0xde == tmp && YES == UART3ComSyn1Flag && YES == UART3ComSyn2Flag && YES == UART3ComSyn3Flag
//	   && NO == UART3ComSyn4Flag && 0xaa == UART3_Buf[0] && 0xbf == UART3_Buf[1] && 0xcb == UART3_Buf[2])
//	{
//	   	UART3ComSyn4Flag = YES;
//		UART3_Buf[3] = tmp;
//		UART3_LEN = 8;
//		UART3_Count = 4;
//		return;
//	}
//	else if(0x4d == tmp && YES == UART3ComSyn1Flag && YES == UART3ComSyn2Flag && YES == UART3ComSyn3Flag
//	   && NO == UART3ComSyn4Flag && 0x1a == UART3_Buf[0] && 0x2f == UART3_Buf[1] && 0x3b == UART3_Buf[2])
//	{
//	   	UART3ComSyn4Flag = YES;
//		UART3_Buf[3] = tmp;
//		UART3_LEN = 8;
//		UART3_Count = 4;
//		return;
//	}
//	else if(NO == UART3ComSyn1Flag || NO == UART3ComSyn2Flag)
//	{
//		UART3ComSyn1Flag = NO;
//		UART3ComSyn2Flag = NO;
//		UART3ComSyn3Flag = NO;
//		UART3_Buf[0] = 0x00;
//		UART3_Buf[1] = 0x00;
//		UART3_Buf[2] = 0x00;
//		return ;	
//	}
//
//	if(6 == UART3_LEN && YES == UART3ComSyn1Flag && YES == UART3ComSyn2Flag)
//	{
//		UART3_Buf[UART3_Count] = tmp;
//		UART3_Count ++;
//
//		if(UART3_LEN == UART3_Count) /*一帧数据接收完毕*/
//		{
//			UART3_Count = 0;
//			UART3ComSyn1Flag = NO;
//			UART3ComSyn2Flag = NO;
//			sum = UART3_Buf[UART3_LEN-1] & 0xff;
//			tmpsum =0;
//			for(i=0;i<UART3_LEN-1;i ++)
//			{
//				tmpsum += UART3_Buf[i];
//			}
//			tmpsum &= 0xff;
//			if(sum == tmpsum)  /*校验成功*/
//			{  
//				Count_GetWaterLevel = COUNT_GETWATERLEVEL; /*监视水塔水位获取，单位min*/
//				if(0x22 == Flag_RadioSwitch)
//				{
//					Flag_GetTowerLevel = YES;
//					Flag_TowerLevl = YES;
//				}
//				else if(0x11 == Flag_RadioSwitch)
//				{
//					Flag_GetInputPool = YES;
//					Flag_PoolLevl = YES;
//				}
//				for(i=0;i<3;i++)
//				{
//					UART3_Buf[200+i] = UART3_Buf[2+i]; /*串口3缓存区后56字节作为临时存储*/
//				
//				}	
//			}	
//		}
//	}
//	else if(8 == UART3_LEN && YES == UART3ComSyn1Flag && YES == UART3ComSyn2Flag
//			&& YES == UART3ComSyn3Flag && YES == UART3ComSyn4Flag)
//	{
//		UART3_Buf[UART3_Count] = tmp;
//		UART3_Count ++;
//		if(UART3_LEN == UART3_Count) /*一帧数据接收完成*/
//		{
//			UART3_Count = 0;
//			UART3ComSyn1Flag = NO;
//			UART3ComSyn2Flag = NO;
//			UART3ComSyn3Flag = NO;
//			UART3ComSyn4Flag = NO;
//			sum = UART3_Buf[UART3_LEN-1] & 0xff;
//			tmpsum = 0;
//			for(i=0;i<UART3_LEN-1;i++)
//			{
//				tmpsum += UART3_Buf[i];
//			}
//			tmpsum &= 0xff;
//			if(sum == tmpsum)	/*校验成功*/
//			{
//				Count_GetWaterLevel = COUNT_GETWATERLEVEL; /*监视水塔水位获取，单位min*/
//				if(0x22 == Flag_RadioSwitch)
//				{
//					Flag_GetTowerLevel = YES;
//					Flag_TowerLevl = YES;
//				}
//				else if(0x11 == Flag_RadioSwitch)
//				{
//					Flag_GetInputPool = YES;
//					Flag_PoolLevl = YES;
//				}
//				for(i=0;i<3;i++)
//				{
//					UART3_Buf[200+i] = UART3_Buf[4+i]; /*串口3缓存区后56字节作为临时存储*/
//				
//				}
//			}
//		}	
//	}
//	else
//	{
//		UART3_Count = 0;
//		UART3ComSyn1Flag = NO;
//		UART3ComSyn2Flag = NO;
//		UART3ComSyn3Flag = NO;
//		UART3ComSyn4Flag = NO;
//		UART3_LEN = 0;	
//	}
} 



/*******************************************************************************
*函数名称：Get_AvrValue   
*函数功能：获取平均值
*函数输入：平均缓冲区，平均值个数
*函数输出：求得平均值
*函数返回：无
*备注：16位数平均，最大不超过65535个数 ,平均方法：去最大最小平均
*******************************************************************************/
u16	Get_AvrValue(u16 * buf,u16 num)
{
	u32 tmpsum = 0;
	u16 tmpmin = 0xffff;
	u16 tmpmax = 0;
	u16 i = 0;
	u16 tmpvalue;
	if(num <= 2)  /*两个以下直接平均*/
	{
		for(i=0;i<num;i++)
		{
			tmpsum += buf[i];
		}
		tmpvalue = (num == 0)?(0):(tmpsum/num);
	}
	else
	{
		tmpsum = 0;
		for(i=0;i<num;i++)
		{
			tmpsum	+= buf[i];
			if(buf[i] < tmpmin)
			{
				tmpmin = buf[i];
			}
			if(buf[i] > tmpmax)
			{
				tmpmax = buf[i];
			} 
		}

		tmpvalue = ((tmpsum - tmpmin - tmpmax)/ (num-2))&0xffff;
	}
	return tmpvalue;
}


void I2C1_Free_Bus(void)
{
	u16 i=10,k=0;
	u16	temp=0;
	temp=I2C1->SR2;
	while((temp&0x0002)&&i!=0)
	{
		I2C1->CR1|=0x8000;
		for(k=0;k<10;k++);
		I2C1->CR1&=~0x8000;
		temp=I2C1->SR2;
		i--;
	}
	I2C_ClearFlag(I2C1,I2C_FLAG_AF);
}

u32 I2C_AcknowPolling(void)
{
	u8 con=0;
	u32 i=200;
	do
	{
		I2C_GenerateSTART(I2C1,ENABLE);
		I2C_ReadRegister(I2C1,I2C_Register_SR1); //清除ADDR位
		I2C_ReadRegister(I2C1,I2C_Register_SR2);
		I2C_Send7bitAddress(I2C1,0x64,I2C_Direction_Transmitter);
		for(con=0;con<10;con++);
		i--;
	}while((!(I2C_ReadRegister(I2C1,I2C_Register_SR1)&0x0002))&&(i!=0));

	I2C_ClearFlag(I2C1,I2C_FLAG_AF);
//	I2C_GenerateSTOP(I2C1,ENABLE);
	return i;
}

/*******************************************************************************
*函数名称：Get_Time
*函数功能：获取实时时钟
*函数调用：
*函数输入：时钟存放地址
*函数输出：yymmddhhmmss,共6字节
*函数返回：无
*备注：
*******************************************************************************/
void Get_Time(u8* tmp)
{
	if(NO == Flag_I2COperation)
	{
		Flag_I2COperation = YES;  /*正在操作*/
		I2C8025_Read6Byte(0x00,tmp+5);
		Flag_I2COperation = NO;	  /*操作结束*/
		if((0x00 == tmp[5]&&0x00 == tmp[6]&&0x00 == tmp[7]&&0x00 == tmp[8]&&0x00 == tmp[9]&&0x00 == tmp[10])  /*总线出错*/
		 ||(0xFF == tmp[5]&&0xFF == tmp[6]&&0xFF == tmp[7]&&0xFF == tmp[8]&&0xFF == tmp[9]&&0xFF == tmp[10]))
		 {
		 	I2C_Configuration();
			Delay(10);
			RX8025_Init();
		 }
	}
}

/*******************************************************************************
*函数名称：S_Time
*函数功能：设置实时时钟
*函数调用：
*函数输入：时钟存放地址
*函数输出：无
*函数返回：无
*备注：
*******************************************************************************/
void Set_Time(u8 *T)
{
	u32 i=0;
	u8 k=0;
	if(NO == Flag_I2COperation && NO == Flag_I2CError)
	{
		Flag_I2COperation = YES;
		i=I2C8025_Write1Byte(0x70,0x00);
		if(i!=0)
		{
			for(k=0;k<100;k++){}
			i=I2C8025_Write1Byte(0x60,*T);//year
			if(i!=0)
			{
				for(k=0;k<100;k++){}
				i=I2C8025_Write1Byte(0x50,*(T+1));//mouth
				if(i!=0)
				{
					for(k=0;k<100;k++){}
					i=I2C8025_Write1Byte(0x40,*(T+2));//date
					if(i!=0)
					{
						for(k=0;k<100;k++){}
						i=I2C8025_Write1Byte(0x20,*(T+3));//hour
						if(i!=0)
						{
							for(k=0;k<100;k++){}
							i=I2C8025_Write1Byte(0x10,*(T+4));//min
							if(i!=0)
							{
								for(k=0;k<100;k++){}
								i=I2C8025_Write1Byte(0x00,*(T+5));//sec
							}
						}
					}
				}
			}
		}

		if(0 == i) /*I2C总线出错*/
		{
			I2C_Configuration();
			Flag_I2CError = YES;
			Count_I2CError = COUNT_I2CERROR;
//			Flag_I2CError = YES;
		}
		Flag_I2COperation = NO;
	}
}

/*******************************************************************************
*函数名称：Get_Adc
*函数功能：ADC1软件启动获取
*函数调用：
*函数输入：
*函数输出：无
*函数返回：无
*备注：
*******************************************************************************/
 u16 Get_Adc(u8 ch)   
{
  	u32 i=0;
	//éè?????¨ADCμ?1??ò×éí¨μà￡?éè???ü??μ?×a?ˉ?3Dòoí2é?ùê±??
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCí¨μà3,1??ò2é?ù?3Dò?μ?a1,2é?ùê±???a239.5?ü?ú	  			    

	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);		//ê1?ü???¨μ?ADC1μ?èí?t×a?????ˉ1|?ü	
	 
	i=1000;
	while((!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC )) && i !=0)
	{
		i --;
	}//μè′y×a???áê?

	//	 ADC_SoftwareStartConvCmd(ADC1, DISABLE);	
	return ADC_GetConversionValue(ADC1);	//·μ??×??üò?′?ADC11??ò×éμ?×a???á1?
}
