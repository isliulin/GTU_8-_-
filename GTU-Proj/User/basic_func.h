#ifndef _BASIC_FUNC_
#define _BASIC_FUNC_

#define Index1 0xF5
#define Index2 0xFA

#define FM25DIS 		   GPIO_SetBits(GPIOA,GPIO_Pin_4)     /*FM25cs 拉高*/
#define FM25EN			   GPIO_ResetBits(GPIOA,GPIO_Pin_4)   /*FM25cs 拉低*/

#define GPRSSYN1 0xf5
#define GPRSSYN2 0xfa

#define PCSETSYN1 0xf5
#define PCSETSYN2 0xfa

extern u16 TimingDelay;   /*延时计数*/

extern u8 GPRS_ConnectFlag; /*服务器连接标志*/
extern u16 GPRS_RxCount;    /*GPRS缓冲接受计数*/
extern u8 GPRS_RxBuf[256];   /*M35模块接收缓冲区*/
extern u8 GPRS_Return[50];  /*AT命令响应缓存*/
extern u8 GPRS_ReturnCount;   /*回显计数*/

extern u8 PCSet_ConnectFlag; /*Iport连接标志*/
extern u8 PCSetComSynFlag;   /*Iport通信同步标记*/
extern u16 PCSet_RxCount;    /*Iport缓冲接受计数*/
extern u8 PCSet_RxBuf[256];   /*Iport模块接收缓冲区*/
extern u8 PCSet_ValueBuf[256];   /*Iport有效数据缓冲区*/

extern u8 UART1TX_Buf[TXBUFMAX];   /*串口1发送中断缓存区*/
extern u16 UART1TX_Head;         /*串口1发送中断发送起始位置*/
extern u16 UART1TX_Tail;         /*串口1发送中断结束位置*/

extern u8 UART2TX_Buf[TXBUFMAX];   /*串口2发送中断缓存区*/
extern u16 UART2TX_Head;         /*串口2发送中断发送起始位置*/
extern u16 UART2TX_Tail;         /*串口2发送中断结束位置*/


extern u8 UART3_LEN;             /*串口3帧长度*/
extern u8 UART3ComSyn1Flag;	  /*串口3第一同步导码*/
extern u8 UART3ComSyn2Flag;	  /*串口3第二同步导码*/
extern u8 UART3ComSyn3Flag;	  /*串口3第三同步导码*/	/*第三、四导码根据帧长度决定是否启用*/
extern u8 UART3ComSyn4Flag;	  /*串口3第四同步导码*/
extern u8 UART3_Buf[256];      /*串口3缓存区*/
extern u8 UART3_Count;           /*串口3接收计数*/


void FM25_WR_NBYTE(u32 add,u8 *pt,u32 num);	   /*512kb铁电写函数*/
void FM25_RE_NBYTE(u32 add,u8 *pt,u32 num);	   /*512kb铁电读函数*/
void USART_SendOneData(USART_TypeDef * USART,u8 sData);
void USART_SendNData(USART_TypeDef * USART,u8* buf,u32 len);
u8 SPI1_SendOne(u8 sData);
void GPRSCom_Handle(u8 sData);
u32 I2C8025_Write1Byte(u8 add,u8 comd);	/*8025写函数*/
void I2C8025_Read6Byte(u8 add,u8 *str);	/*8025读函数*/
void Set_Time(u8 *T); /*时间设置*/
u16 CRC16(u8 *tmp,u8 num);	 /*16位 CRC校验计算*/
void UART5_Handle(u8 sData);/*UART5串口中断处理函数*/
void Delay(u16 nms);    /*延时函数*/
void Send_To_Server(u8*buf,u16 len); /*GPRS发送函数*/
u16 crc16_ccitt(u8 *tmp,u8 num);   /*GPRS通信，crc校验*/

void USART3_Handle(u8);           /*串口3数据处理函数*/

u16	Get_AvrValue(u16 * buf,u16 num);  /*求平均值*/
void I2C1_Free_Bus(void); 
u32 I2C_AcknowPolling(void);
void GetMsgCom_Handle(u8 sData);
void Value_Judge(void);
void PCValueSet_Handle(u8 sData);
void Get_Time(u8* tmp);
 u16 Get_Adc(u8 ch);
#endif
