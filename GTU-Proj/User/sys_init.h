#ifndef _SYS_INIT_
#define _SYS_INIT_


//#define RX485 do{GPIO_ResetBits(GPIOC,GPIO_Pin_13);}while(0);		/*485��������˫������*/
//#define TX485 do{GPIO_SetBits(GPIOC,GPIO_Pin_13);}while(0);			/*485��������˫������*/
//#define GPRSPOWON do{GPIO_ResetBits(GPIOC,GPIO_Pin_8);}while(0);
//#define GPRSPOWOFF do{GPIO_SetBits(GPIOC,GPIO_Pin_8);}while(0);	
//
//#define CANREINITTIMEMIN 60 /*CAN���߳�ʼ������ͳ������,��λmin,���65535*/
//#define CANREINITNUMMAX 30 /*CAN���߳�ʼ��ͳ������������ʼ������,���65535*/

//extern u8 IP[20];		/*������IP*/
//extern u8 PORT[10];				/*�������˿�*/


extern u8 GPRSCmd0[20];    /*M35���������� ��115200*/
extern u8 GPRSCmd1[20];          /*��ѯGPRS����ע���Ƿ�ɹ�*/
extern u8 GPRSCmd2[20];     /*����Ϣ���ȴ洢λ��*/
extern u8 GPRSCmd3[20];          /*PDU��ʽ����Ϣ*/
extern u8 GPRSCmd4[20];             /*����Ϣ����*/
extern u8 GPRSCmd5[20];        /*ɾ�����ж���Ϣ*/
extern u8 GPRSCmd6[50];   /*TCP���ӣ���ӡ�IP��,��PORT��\r*/
extern u8 GPRSCmd7[10];				/*�رջ���*/
extern u8 GPRSCmd8[20];     /*text��ʽ�����ڶ���ɾ��*/
extern u8 GPRSCmd9[15];           /*TCP����*/
extern u8 GPRSCmd10[15];          /*M35ģ��ػ�����*/
extern u8 GPRSCmd11[15];            /*��ѯ�ź�����*/ 
extern u8 GPRSCmd12[15];  		    /*�����*/
extern u8 GPRSCmd13[15];  		/*ע������*/
extern u8 GPRSCmd14[15];         /*��ȡ��һ������*/
extern u8 GPRSCmd15[25];         /*����GPRS��APN*/

extern u8 GPRSPDU[300];          /*����Ϣ���Ϳ�ʼ����*/

extern u8 GPRS_InitFlag;

extern u8 Canerror;  /*CAN�����ϴγ������*/
/*������������֤CAN������һ��ʱ���ڳ�ʼ����������̫��*/
extern u16 Count_CanReInitNum;   /*ͳ��һ��ʱ����CAN���߳�ʼ������*/
extern u16 Count_CanReInitTime;  /*ͳ��һ��ʱ����CAN���߳�ʼ��ʱ��*/ 



/*��������*/
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
void ADC_Configuration(void);//ADC��ʼ������
void DMA_Configuration(void);
void WG_Reload(void);
void GPRS_Init(void);
void FM25_Init(void);
void SysTick_Configuration(void);
void RX8025_Init(void);
void CAN_Configuration(void);

#endif
