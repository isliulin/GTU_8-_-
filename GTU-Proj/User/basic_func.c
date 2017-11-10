/*******************************************************************************
*�ļ�����basic_func.c
*�ļ����ܣ���ˮ����Ӧ�ú���
*����ʱ�䣺2014-04-28
*���ߣ�xue
*�޸ļ�¼��
*******************************************************************************/
#include "include.h"

/*ȫ�ֱ���*/


u16 TimingDelay = 0;   /*��ʱ����*/

/*���ڽ����ж����ݶ���*/

u8 GPRS_ConnectFlag = 0; /*���������ӱ�־*/
u8 GPRSComSynFlag = NO;   /*������ͨ��ͬ�����*/
u16 GPRS_RxCount = 0;    /*GPRS������ܼ���*/
u8 GPRS_RxBuf[256] = {0};   /*M35ģ����ջ�����*/
u8 GPRS_Return[50] = {0};  /*AT������Ӧ����*/
u8 GPRS_ReturnCount = 0;   /*���Լ���*/

u8 PCSet_ConnectFlag = 0; /*PCSet���ӱ�־*/
u8 PCSetComSynFlag = NO;   /*PCSetͨ��ͬ�����*/
u16 PCSet_RxCount = 0;    /*PCSet������ܼ���*/
u8 PCSet_RxBuf[256] = {0};   /*PCSetģ����ջ�����*/
u8 PCSet_ValueBuf[256] = {0};   /*PCSet��Ч���ݻ�����*/

u8 UART1TX_Buf[TXBUFMAX] = {0};   /*����1�����жϻ�����*/
u16 UART1TX_Head = 0;         /*����1�����жϷ�����ʼλ��*/
u16 UART1TX_Tail = 0;         /*����1�����жϽ���λ��*/

u8 UART2TX_Buf[TXBUFMAX] = {0};   /*����2�����жϻ�����*/
u16 UART2TX_Head = 0;         /*����2�����жϷ�����ʼλ��*/
u16 UART2TX_Tail = 0;         /*����2�����жϽ���λ��*/

u8 UART3_LEN = 0;             /*����3֡����*/
u8 UART3ComSyn1Flag = NO;	  /*����3��һͬ������*/
u8 UART3ComSyn2Flag = NO;	  /*����3�ڶ�ͬ������*/
u8 UART3ComSyn3Flag = NO;	  /*����3����ͬ������*/	/*�������ĵ������֡���Ⱦ����Ƿ�����*/
u8 UART3ComSyn4Flag = NO;	  /*����3����ͬ������*/
u8 UART3_Buf[256] = {0};      /*����3������*/
u8 UART3_Count = 0;           /*����3���ռ���*/



/*******************************************************************************
*�������ƣ�UART5_Handle
*�������ܣ�UART5����
*�������ã�
*�������룺���ڽ�������
*�������������󴮿ڻ�����
*�������أ���
*��ע��
*******************************************************************************/
void UART5_Handle(u8 sData)
{	
}


/*******************************************************************************
*�������ƣ�PCValueSet_Handle
*�������ܣ�PC�������ý���
*�������ã�
*�������룺���ڽ�������
*�������������󴮿ڻ�����
*�������أ���
*��ע��
*******************************************************************************/
void PCValueSet_Handle(u8 sData)
{
	u16	 checksum = 0, x = 0;
	if(sData==PCSETSYN1)
	 {         
       if(PCSetComSynFlag==NO)	 /*����IPORTSYN1�ַ�.*/
	   { 
           PCSet_RxBuf[0]=sData;
           return;
        }
     }	

	 if( (sData==PCSETSYN2)&&(PCSet_RxBuf[0]==PCSETSYN1)&&(PCSetComSynFlag==NO) )	   /*����PCSETSYN2�ַ�����ʼ����.*/
	 { 
         PCSet_RxBuf[1]=sData;
         PCSet_RxCount=2;
         PCSetComSynFlag=YES;  /*ͬ������*/
         return;
     }

	 if(PCSetComSynFlag==NO)
	 {
         PCSet_RxCount=0;   /*����������״̬����������֡ͬ��*/
         PCSet_RxBuf[0]=0x00;
         PCSet_RxBuf[1]=0x00;
		 return;
     }

	 /*ͬ���Ѿ�����*/
	 PCSet_RxBuf[PCSet_RxCount]=sData;
	 PCSet_RxCount++;
	 if(PCSet_RxCount<3)	   /*��δ���յ�֡�����룬�˳���*/
	 {   
	     return;
	 }	 
	 x=(PCSet_RxBuf[2]+5); /*����֡�ֽ���*/
	 if( PCSet_RxCount>=x )		  /*һ֡�������*/
	 {
//		 checksum1=PCSet_RxBuf[x-2];      /*crcУ��*/
//		 checksum1*=256;
//		 checksum1+=PCSet_RxBuf[x-1];    	    
		 checksum = crc16_ccitt(PCSet_RxBuf,PCSet_RxBuf[2]+5);	  
		 if( 0==checksum)		  /*֡У��ƥ��*/
		 { 
			/*������*/
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
*�������ƣ�GPRSCom_Handle
*�������ܣ�GPRS���մ���
*�������ã�
*�������룺GPRS_InitFlag,GPRS_ConnectFlag,GPRS_RxBuf,GPRS_RxCount
*���������GPRS_InitFlag,GPRS_ConnectFlag,GPRS_RxBuf,GPRS_RxCount
*�������أ���
*��ע��
*******************************************************************************/
void GPRSCom_Handle(u8 sData)
{
	u16	 checksum2 = 0, x = 0;
	char * tmpchar =NULL ;
	Flag_RecvingGPRS = YES;
	Count_TimGPRSRecv = COUNT_TIMGPRSRECV;   /*��ʱ����*/
	if(sData==GPRSSYN1)
	 {         
       if(GPRSComSynFlag==NO)	 /*����GPRSSYN1�ַ�.*/
	   { 
           GPRS_RxBuf[0]=sData;
           return;
        }
     }	

	 if( (sData==GPRSSYN2)&&(GPRS_RxBuf[0]==GPRSSYN1)&&(GPRSComSynFlag==NO) )	   /*����GPRSSYN2�ַ�����ʼ����.*/
	 { 
         GPRS_RxBuf[1]=sData;
         GPRS_RxCount=2;
         GPRSComSynFlag=YES;  /*ͬ������*/
         return;
     }

	 if(GPRSComSynFlag==NO)
	 {
         GPRS_RxCount=0;   /*����������״̬����������֡ͬ��*/
         GPRS_RxBuf[0]=0x00;
         GPRS_RxBuf[1]=0x00;
		 GPRS_Return[GPRS_ReturnCount] = sData;   /*���Խ���*/
		 GPRS_ReturnCount ++;
		 if(50 <= GPRS_ReturnCount)
		 {
		 	GPRS_ReturnCount = 0;
		 }
		 if((sData == '\r')&& 4 <= GPRS_ReturnCount)		   /*����ظ�*/
		 {
		 	GPRS_Return[30] = 0;
			GPRS_ReturnCount = 0;
			if((0 == Flag_GPRSReStep)&&(strstr((char *)GPRS_Return,"SEND OK"))) 		   /*���������в��ж�*/
			{
				Count_TimConnGPRS = COUNT_TIMCONNGPRS;  /*���Ӵ���*/
//				Count_GPRSConnectEvent = COUNT_GPRSCONNECTEVENT;
				memset(GPRS_Return,0,30);	
			}
			else if((strstr((char *)GPRS_Return,"CLOSED")))					 /*���յ�CLOSED*/
			{
				Flag_GprsGetClose = YES;
				memset(GPRS_Return,0,30);
			}
			else if(NULL != (tmpchar = strstr((char *)GPRS_Return,"+CSQ:")))			/*�ֻ��źŻ�ȡ*/
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
			else if(NULL != strstr((char *)GPRS_Return,"+CMTI:"))	  /*���յ�����*/
			{
				Flag_GetMsg = YES;
				Count_TimGPRSRecv = 1000; /*Ϊ���ն���Ԥ��30s����ʱ��*/
				Count_GetMsgBuf = 0;   /*���Ŵ�����ռ���*/
				Flag_GetWrongMsg = NO; 
				Flag_GetMsgOver = NO;
				Count_GetMsgStep = COUNT_GETMSGSTEP;
			}
		 }
	     return;
     }

	 /*ͬ���Ѿ�����*/
	 GPRS_RxBuf[GPRS_RxCount]=sData;
	 GPRS_RxCount++;
	 if(GPRS_RxCount<3)	   /*��δ���յ�֡�����룬�˳���*/
	 {   
	     return;
	 }	 
	 x=(GPRS_RxBuf[2]+5); /*����֡�ֽ���*/
	 if(x > 256) /*֡������*/
	 {
	 	 GPRS_RxCount=0;   /*����������״̬����������֡ͬ��*/
         GPRS_RxBuf[0]=0x00;
         GPRS_RxBuf[1]=0x00;
		 return;	
	 }
	 if( GPRS_RxCount>=x )		  /*һ֡�������*/
	 {
//		 checksum1=GPRS_RxBuf[x-2];      /*crcУ��*/
//		 checksum1*=256;
//		 checksum1+=GPRS_RxBuf[x-1];    	    
		 checksum2 = crc16_ccitt(GPRS_RxBuf,GPRS_RxBuf[2]+5);	  
		 if( 0 ==checksum2)		  /*֡У��ƥ��*/
		 { 
		 	/*������*/
			if(GPRS_RxBuf[3] >= 0x80)		/*��һ���������󣬲Ž����µ�����*/
			{
				
			 Count_TimConnGPRS = COUNT_TIMCONNGPRS;  /*���Ӵ���*/
//			 Count_TimSendStop = 0;   /*���¶�ʱ���ͻ����ֹͣ*/
				

			if(0xfa == GPRS_RxBuf[3]) /*����ת�������������*/
			{
			   if(0 == Count_RemoteCommond)
			   {
				Count_TimRemote = COUNT_TIMREMOTE; /*��ʼ��ʱ*/
			   }
			   Count_RemoteCommond ++;
			   if(3 <= Count_RemoteCommond) /*ת��������Ч*/
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
				if(3 != x)	   /*������ջ�����������������*/
				{
					memset(GPRS_RecvCommondBuf[x],0,256);
					memcpy(GPRS_RecvCommondBuf[x],GPRS_RxBuf,256);
					Flag_NeedGPRSRecv = YES;
				}
//				Flag_GetGPRSValue |= (0x1<<((GPRS_ValueBuf[3]&0x1f)-1));   /*��ȡGPRS������*/  /*����ֻ��һ�����������������������������ָ��*/
//																	 /*��Ȼ���ܹ�ȫ��ִ�У������ڶ��ѯָ��*/
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
*�������ƣ�GetMsgCom_Handle
*�������ܣ����Ž��մ���
*�������ã�
*�������룺
*���������
*�������أ���
*��ע��
*******************************************************************************/
void GetMsgCom_Handle(u8 sData)
{
	GetMsg_Buf[Count_GetMsgBuf++] = sData;
	if(Count_GetMsgBuf >= 390)
	{
		Count_GetMsgBuf	= 0;
		Flag_GetWrongMsg = YES; /*���յ��������*/
		memset(GetMsg_Buf,0,400);
	}
	else if(Count_GetMsgBuf > 30 && '\n' == sData)
	{
		if(NULL != (strstr((char *)GetMsg_Buf,GETMSGWORD)))
		{
			Flag_GetMsgOver = YES; /*���Ž������*/
		}
		Count_GetMsgBuf = 0;
		memset(GetMsg_Buf,0,400);
//		else if(NULL != (strstr((char*)GetMsg_Buf,"\r\nOK\r\n"))) /*��������������½���*/
//		{
//			Count_GetMsgBuf = 0;
//			memset(GetMsg_Buf,0,400);
//		}	
	}	
}

/*******************************************************************************
*�������ƣ�FM25_WR_NBYTE
*�������ܣ���FM25ָ���ַ��д��ָ����Ŀ�ַ�
*�������ã�SPI2_SendOne
*�������룺u32 add,FM25��ַ��u8 *pt д��������ʼ��ַ��u32 numд����Ŀ
*�����������
*�������أ���
*��ע��
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
*�������ƣ�FM25_RE_NBYTE
*�������ܣ���FM25ָ���ַ������ָ����Ŀ�ַ�
*�������ã�SPI2_SendOne
*�������룺u32 add,FM25��ַ��u8 *pt �������ݴ�ŵ�ַ��u32 num������Ŀ
*�����������
*�������أ���
*��ע��
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
*�������ƣ�SPI2_SendOne
*�������ܣ���SPI2д��һ���ַ�
*�������ã���ؿ⺯��
*�������룺u8 sData,д�������
*�����������
*�������أ��������ַ�����ģʽ��ʱ��
*��ע��
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
*�������ƣ�USART_SendOneData
*�������ܣ���USARTд��һ���ַ�
*�������ã���ؿ⺯��
*�������룺u8 sData,д�������;USART_TypeDef * USART,��Ӧ���ں�
*�����������
*�������أ���
*��ע��
*******************************************************************************/
void USART_SendOneData(USART_TypeDef * USART,u8 sData)
{
	u32 i=40000U;
//	if(USART1 == USART)		/*����1�����жϷ���*/
//	{
//		i = 500000; /*Լ0.5s*/
//		while(UART1TX_Tail+1 > TXBUFMAX && i != 0U){i --;};
//		UART1TX_Buf[UART1TX_Tail++] = sData;
//		USART_ITConfig(USART1,USART_IT_TXE,ENABLE);
//
//	}
//	else if(USART2 == USART)  /*����2�����жϷ���*/
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
		USART_SendData(USART, sData);          /*�����ַ�*/
		while(USART_GetFlagStatus(USART, USART_FLAG_TXE) == RESET && i!=0U){i--;}/*�ȴ����ͽ���*/
//	}
}

/*******************************************************************************
*�������ƣ�USART_SendNData
*�������ܣ���USARTд��N���ַ�
*�������ã���ؿ⺯��
*�������룺u8 *buf,д���������ʼ��ַ;USART_TypeDef * USART,��Ӧ���ں� ��u32 len д�볤��
*�����������
*�������أ���
*��ע��
*******************************************************************************/
void USART_SendNData(USART_TypeDef * USART,u8* buf,u32 len)
{
	u32 i=0;
	u16 tmplen = 0;
//	if(USART1 == USART)	 /*����1�����жϷ���*/
//	{
//		USART_ITConfig(USART1,USART_IT_TXE,DISABLE);
//		i = 1000; /*�ȴ���һ���ֽڷ������*/   
//		while(USART_GetFlagStatus(USART, USART_FLAG_TXE) == RESET && i!=0U){i--;}/*�ȴ����ͽ���*/
//		if(len + UART1TX_Tail > TXBUFMAX)  /*�����ͳ��ȴ�����󻺳���*/
//		{
//			tmplen = TXBUFMAX-UART1TX_Tail;
//			memcpy(UART1TX_Buf+UART1TX_Tail,buf,tmplen);
//			UART1TX_Tail = TXBUFMAX;
//			USART_ITConfig(USART1,USART_IT_TXE,ENABLE);
//			i = 1000000; /*Լ1s*/
//			while(len - tmplen + UART1TX_Tail > TXBUFMAX && i != 0U){i--;};
//			USART_ITConfig(USART1,USART_IT_TXE,DISABLE);
//			i = 1000; /*�ȴ���һ���ֽڷ������*/   
//			while(USART_GetFlagStatus(USART, USART_FLAG_TXE) == RESET && i!=0U){i--;}/*�ȴ����ͽ���*/
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
//	else if(USART2 == USART)	 /*����2�����жϷ���*/
//	{
//		USART_ITConfig(USART2,USART_IT_TXE,DISABLE);
//		i = 1000; /*�ȴ���һ���ֽڷ������*/   
//		while(USART_GetFlagStatus(USART, USART_FLAG_TXE) == RESET && i!=0U){i--;}/*�ȴ����ͽ���*/
//		if(len + UART2TX_Tail > TXBUFMAX)  /*�����ͳ��ȴ�����󻺳���*/
//		{
//			tmplen = TXBUFMAX-UART2TX_Tail;
//			memcpy(UART2TX_Buf+UART2TX_Tail,buf,tmplen);
//			UART2TX_Tail = TXBUFMAX;
//			USART_ITConfig(USART2,USART_IT_TXE,ENABLE);
//			i = 1000000;
//			while(len - tmplen + UART2TX_Tail > TXBUFMAX && i != 0U){i--;};
//			USART_ITConfig(USART2,USART_IT_TXE,DISABLE);
//			i = 1000; /*�ȴ���һ���ֽڷ������*/   
//			while(USART_GetFlagStatus(USART, USART_FLAG_TXE) == RESET && i!=0U){i--;}/*�ȴ����ͽ���*/
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
*�������ƣ�I2C8025_Write1Byte
*�������ܣ�ͨ��I2C��8025д��һ���ֽ�
*�������ã�
*�������룺��ַadd������comd
*�����������
*�������أ�i(==0д��ʧ�ܣ�!=0д��ɹ���
*��ע��
*******************************************************************************/
u32 I2C8025_Write1Byte(u8 add,u8 comd)
{
	u32 i=1;
//	u8 k=0;
//	i=I2C_AcknowPolling();
	I2C_AcknowledgeConfig(I2C1, ENABLE);//ʹ�ܻ���ʧ��ָ�� I2C��Ӧ����
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
*�������ƣ�I2C8025_Read6Byte
*�������ܣ�ͨ��I2C��8025����6���ֽ�
*�������ã�
*�������룺��ַadd����ŵ�ַ
*���������ʵʱʱ��
*�������أ���
*��ע��
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
		I2C_AcknowledgeConfig(I2C1, ENABLE);//ʹ�ܻ���ʧ��ָ�� I2C��Ӧ����
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
									I2C_AcknowledgeConfig(I2C1, DISABLE);//ʹ�ܻ���ʧ��ָ�� I2C��Ӧ����
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
*�������ƣ�CRC16
*�������ܣ�16λcrc���㣬����ͬ���ܱ�ͨ�ţ�����ʽΪA001
*�������ã�
*�������룺u8*,��ҪУ�����ݻ����ַ��u8 У����Ŀ���255��
*�����������
*�������أ�Crc������
*��ע��
*******************************************************************************/
u16 CRC16(u8 *tmp,u8 num)
{
	u16 crc = 0xFFFF;   /*Ԥ��ֵ*/
	u8 i=0,j=0;
	u8 tmpcrc = 0;
	for(i=0;i<num;i++)
	{
		tmpcrc = crc&0xff;
		tmpcrc ^= tmp[i];
		crc &=0xff00;
		crc |=(tmpcrc|0x0);     /*��8λ���*/

		for(j=0;j<8;j++)
		{
			if(crc&0x1)   /*���λΪ1*/
			{
				crc >>= 1;  /*����һλ*/
				crc &=~(0x8000); /*���λ��0*/
				crc ^= 0xA001;   /*��A001H���*/
			}
			else		    /*Ϊ0*/
			{			   
				crc >>= 1;		/*����1λ*/
			}
		}
	}
	return crc;
}

/*******************************************************************************
*�������ƣ�Delay
*�������ܣ���ʱnms
*�������ã�
*�������룺��ʱʱ��,��λms
*�����������
*�������أ���
*��ע��
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
*�������ƣ�Send_To_Server
*�������ܣ������ݷ��͵�������
*�������ã�
*�������룺���ݻ����ַ�����ݳ��ȡ�Flag_SendingGPRS
*���������Flag_SendingGPRS
*�������أ���
*��ע��	������ֽ�999�ֽ�	������Ҫ���޸�
*******************************************************************************/

//#if YES == FLAGIPORT && 0xaa == IPORTFUNCTION  /*ͨ��Iport���ӷ�����*/
void Send_To_Server(u8*buf,u16 len)
{
	u32 i = 0;
//	Count_TimSendStop = 0;   /*���¶�ʱ���ͻ����ֹͣ*/
	Flag_SendingGPRS = YES;
	Count_TimGPRSSendInt = COUNT_TIMGPRSSENDINT;
	Count_TimGPRSBeat = COUNT_TIMGPRSBEAT;
	GPRSCmd9[10] = ((len%1000)/100)+0x30;  /*������ֽ�999�ֽ�*/
	GPRSCmd9[11] = ((len%100)/10)+0x30;
	GPRSCmd9[12] = len%10 + 0x30;
	GPRSCmd9[13] = '\n';
	USART_SendNData(USART3,GPRSCmd9,14);	 
	Delay(20);
	USART_SendNData(USART3,buf,len);
//	USART_SendOneData(USART2,0x1a);
	for(i=0;i<15500;i++);   /*�ȴ���Ӧ���ݶ�*/
}



/*******************************************************************************
*�������ƣ�crc16_ccitt
*�������ܣ�16λCRCУ�飬����ͬ������ͨ�ţ�����ʽΪ1021
*�������ã���
*�������룺u8*tmp У����ʼ��ַ��u8 num У�鳤��
*�������룺16CRCУ����
*�����������
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
		CrcTmp ^= tmpcrc;			 /*��8λ���*/
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
*�������ƣ�USART3_Handle    
*�������ܣ�����3���ݴ�����
*�������룺����������
*�����������
*�������أ���
*��ע���ɼ������ֲ�ͬˮλ��
*******************************************************************************/
void USART3_Handle(u8 tmp)
{
//	u16 tmpsum = 0;
//	u16 sum	 = 0;
//	u16 i = 0;
//	/*ͬ����1*/
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
//	/*ͬ����2*/
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
//	/*ͬ����3*/
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
//	else if( NO == UART3ComSyn1Flag || NO == UART3ComSyn2Flag)  /*֡������ȷ*/
//	{
//		UART3ComSyn1Flag = NO;
//		UART3ComSyn2Flag = NO;
//		UART3_Buf[0] = 0x00;
//		UART3_Buf[1] = 0x00;
//		return;	
//	}
//
//	/*ͬ����4*/
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
//		if(UART3_LEN == UART3_Count) /*һ֡���ݽ������*/
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
//			if(sum == tmpsum)  /*У��ɹ�*/
//			{  
//				Count_GetWaterLevel = COUNT_GETWATERLEVEL; /*����ˮ��ˮλ��ȡ����λmin*/
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
//					UART3_Buf[200+i] = UART3_Buf[2+i]; /*����3��������56�ֽ���Ϊ��ʱ�洢*/
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
//		if(UART3_LEN == UART3_Count) /*һ֡���ݽ������*/
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
//			if(sum == tmpsum)	/*У��ɹ�*/
//			{
//				Count_GetWaterLevel = COUNT_GETWATERLEVEL; /*����ˮ��ˮλ��ȡ����λmin*/
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
//					UART3_Buf[200+i] = UART3_Buf[4+i]; /*����3��������56�ֽ���Ϊ��ʱ�洢*/
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
*�������ƣ�Get_AvrValue   
*�������ܣ���ȡƽ��ֵ
*�������룺ƽ����������ƽ��ֵ����
*������������ƽ��ֵ
*�������أ���
*��ע��16λ��ƽ������󲻳���65535���� ,ƽ��������ȥ�����Сƽ��
*******************************************************************************/
u16	Get_AvrValue(u16 * buf,u16 num)
{
	u32 tmpsum = 0;
	u16 tmpmin = 0xffff;
	u16 tmpmax = 0;
	u16 i = 0;
	u16 tmpvalue;
	if(num <= 2)  /*��������ֱ��ƽ��*/
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
		I2C_ReadRegister(I2C1,I2C_Register_SR1); //���ADDRλ
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
*�������ƣ�Get_Time
*�������ܣ���ȡʵʱʱ��
*�������ã�
*�������룺ʱ�Ӵ�ŵ�ַ
*���������yymmddhhmmss,��6�ֽ�
*�������أ���
*��ע��
*******************************************************************************/
void Get_Time(u8* tmp)
{
	if(NO == Flag_I2COperation)
	{
		Flag_I2COperation = YES;  /*���ڲ���*/
		I2C8025_Read6Byte(0x00,tmp+5);
		Flag_I2COperation = NO;	  /*��������*/
		if((0x00 == tmp[5]&&0x00 == tmp[6]&&0x00 == tmp[7]&&0x00 == tmp[8]&&0x00 == tmp[9]&&0x00 == tmp[10])  /*���߳���*/
		 ||(0xFF == tmp[5]&&0xFF == tmp[6]&&0xFF == tmp[7]&&0xFF == tmp[8]&&0xFF == tmp[9]&&0xFF == tmp[10]))
		 {
		 	I2C_Configuration();
			Delay(10);
			RX8025_Init();
		 }
	}
}

/*******************************************************************************
*�������ƣ�S_Time
*�������ܣ�����ʵʱʱ��
*�������ã�
*�������룺ʱ�Ӵ�ŵ�ַ
*�����������
*�������أ���
*��ע��
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

		if(0 == i) /*I2C���߳���*/
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
*�������ƣ�Get_Adc
*�������ܣ�ADC1���������ȡ
*�������ã�
*�������룺
*�����������
*�������أ���
*��ע��
*******************************************************************************/
 u16 Get_Adc(u8 ch)   
{
  	u32 i=0;
	//����?????��ADC��?1??�����������̨���?����???��??��?��a?��?3D��o��2��?������??
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC�����̨�3,1??��2��?��?3D��?��?a1,2��?������???a239.5?��?��	  			    

	ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);		//��1?��???����?ADC1��?����?t��a?????��1|?��	
	 
	i=1000;
	while((!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC )) && i !=0)
	{
		i --;
	}//�̨���y��a???����?

	//	 ADC_SoftwareStartConvCmd(ADC1, DISABLE);	
	return ADC_GetConversionValue(ADC1);	//����??��??����?��?ADC11??��������?��a???��1?
}
