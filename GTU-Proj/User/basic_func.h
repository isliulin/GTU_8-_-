#ifndef _BASIC_FUNC_
#define _BASIC_FUNC_

#define Index1 0xF5
#define Index2 0xFA

#define FM25DIS 		   GPIO_SetBits(GPIOA,GPIO_Pin_4)     /*FM25cs ����*/
#define FM25EN			   GPIO_ResetBits(GPIOA,GPIO_Pin_4)   /*FM25cs ����*/

#define GPRSSYN1 0xf5
#define GPRSSYN2 0xfa

#define PCSETSYN1 0xf5
#define PCSETSYN2 0xfa

extern u16 TimingDelay;   /*��ʱ����*/

extern u8 GPRS_ConnectFlag; /*���������ӱ�־*/
extern u16 GPRS_RxCount;    /*GPRS������ܼ���*/
extern u8 GPRS_RxBuf[256];   /*M35ģ����ջ�����*/
extern u8 GPRS_Return[50];  /*AT������Ӧ����*/
extern u8 GPRS_ReturnCount;   /*���Լ���*/

extern u8 PCSet_ConnectFlag; /*Iport���ӱ�־*/
extern u8 PCSetComSynFlag;   /*Iportͨ��ͬ�����*/
extern u16 PCSet_RxCount;    /*Iport������ܼ���*/
extern u8 PCSet_RxBuf[256];   /*Iportģ����ջ�����*/
extern u8 PCSet_ValueBuf[256];   /*Iport��Ч���ݻ�����*/

extern u8 UART1TX_Buf[TXBUFMAX];   /*����1�����жϻ�����*/
extern u16 UART1TX_Head;         /*����1�����жϷ�����ʼλ��*/
extern u16 UART1TX_Tail;         /*����1�����жϽ���λ��*/

extern u8 UART2TX_Buf[TXBUFMAX];   /*����2�����жϻ�����*/
extern u16 UART2TX_Head;         /*����2�����жϷ�����ʼλ��*/
extern u16 UART2TX_Tail;         /*����2�����жϽ���λ��*/


extern u8 UART3_LEN;             /*����3֡����*/
extern u8 UART3ComSyn1Flag;	  /*����3��һͬ������*/
extern u8 UART3ComSyn2Flag;	  /*����3�ڶ�ͬ������*/
extern u8 UART3ComSyn3Flag;	  /*����3����ͬ������*/	/*�������ĵ������֡���Ⱦ����Ƿ�����*/
extern u8 UART3ComSyn4Flag;	  /*����3����ͬ������*/
extern u8 UART3_Buf[256];      /*����3������*/
extern u8 UART3_Count;           /*����3���ռ���*/


void FM25_WR_NBYTE(u32 add,u8 *pt,u32 num);	   /*512kb����д����*/
void FM25_RE_NBYTE(u32 add,u8 *pt,u32 num);	   /*512kb���������*/
void USART_SendOneData(USART_TypeDef * USART,u8 sData);
void USART_SendNData(USART_TypeDef * USART,u8* buf,u32 len);
u8 SPI1_SendOne(u8 sData);
void GPRSCom_Handle(u8 sData);
u32 I2C8025_Write1Byte(u8 add,u8 comd);	/*8025д����*/
void I2C8025_Read6Byte(u8 add,u8 *str);	/*8025������*/
void Set_Time(u8 *T); /*ʱ������*/
u16 CRC16(u8 *tmp,u8 num);	 /*16λ CRCУ�����*/
void UART5_Handle(u8 sData);/*UART5�����жϴ�����*/
void Delay(u16 nms);    /*��ʱ����*/
void Send_To_Server(u8*buf,u16 len); /*GPRS���ͺ���*/
u16 crc16_ccitt(u8 *tmp,u8 num);   /*GPRSͨ�ţ�crcУ��*/

void USART3_Handle(u8);           /*����3���ݴ�����*/

u16	Get_AvrValue(u16 * buf,u16 num);  /*��ƽ��ֵ*/
void I2C1_Free_Bus(void); 
u32 I2C_AcknowPolling(void);
void GetMsgCom_Handle(u8 sData);
void Value_Judge(void);
void PCValueSet_Handle(u8 sData);
void Get_Time(u8* tmp);
 u16 Get_Adc(u8 ch);
#endif
