#ifndef _SYS_INIT_
#define _SYS_INIT_


//#define RX485 do{GPIO_ResetBits(GPIOC,GPIO_Pin_13);}while(0);		/*485控制器半双工控制*/
//#define TX485 do{GPIO_SetBits(GPIOC,GPIO_Pin_13);}while(0);			/*485控制器半双工控制*/
//#define GPRSPOWON do{GPIO_ResetBits(GPIOC,GPIO_Pin_8);}while(0);
//#define GPRSPOWOFF do{GPIO_SetBits(GPIOC,GPIO_Pin_8);}while(0);	
//
//#define CANREINITTIMEMIN 60 /*CAN总线初始化次数统计周期,单位min,最大65535*/
//#define CANREINITNUMMAX 30 /*CAN总线初始化统计周期内最大初始化次数,最大65535*/

//extern u8 IP[20];		/*服务器IP*/
//extern u8 PORT[10];				/*服务器端口*/


extern u8 GPRSCmd0[20];    /*M35波特率设置 ：115200*/
extern u8 GPRSCmd1[20];          /*查询GPRS网络注册是否成功*/
extern u8 GPRSCmd2[20];     /*短信息优先存储位置*/
extern u8 GPRSCmd3[20];          /*PDU格式短信息*/
extern u8 GPRSCmd4[20];             /*短信息发送*/
extern u8 GPRSCmd5[20];        /*删除所有短信息*/
extern u8 GPRSCmd6[50];   /*TCP连接，后接“IP”,“PORT”\r*/
extern u8 GPRSCmd7[10];				/*关闭回显*/
extern u8 GPRSCmd8[20];     /*text格式，用于短信删除*/
extern u8 GPRSCmd9[15];           /*TCP发送*/
extern u8 GPRSCmd10[15];          /*M35模块关机命令*/
extern u8 GPRSCmd11[15];            /*查询信号质量*/ 
extern u8 GPRSCmd12[15];  		    /*激活场景*/
extern u8 GPRSCmd13[15];  		/*注销场景*/
extern u8 GPRSCmd14[15];         /*读取第一条短信*/
extern u8 GPRSCmd15[25];         /*设置GPRS的APN*/

extern u8 GPRSPDU[300];          /*短信息发送开始数据*/

extern u8 GPRS_InitFlag;

extern u8 Canerror;  /*CAN总线上次出错代码*/
/*两个参数，保证CAN总线在一定时间内初始化次数不会太多*/
extern u16 Count_CanReInitNum;   /*统计一定时间内CAN总线初始化次数*/
extern u16 Count_CanReInitTime;  /*统计一定时间内CAN总线初始化时间*/ 



/*函数声明*/
void SYS_Init(void);
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void TIM_Configuration(void);
void USART_Configuration(void);
void I2C_Configuration(void);
void SPI_Configuration(void);
void IWDG_Configuration(void);
void EXIT_Configuration(void);
void ADC_Configuration(void);//ADC初始化函数
void DMA_Configuration(void);
void WG_Reload(void);
void GPRS_Init(void);
void FM25_Init(void);
void SysTick_Configuration(void);
void RX8025_Init(void);
void CAN_Configuration(void);

#endif
