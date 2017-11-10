#ifndef _DEFINE_
#define _DEFINE_

//#define ELEMETER  /*�Ƿ�ʹ�õ��ܱ���ʹ��ע�ʹ���*//*��ȡ���ò�������ѹ������ʱ�ķ�ʽ*/
#define ADC1_DR_Address    ((u32)0x4001244C)      /*ADC�����ַ��DMA��*/


#define COUNTALARM 500
#define LOW 0				  /*�������͵�ƽ*/
#define HIGH 1				  /*�������ߵ�ƽ*/
#define TXBUFMAX 512   /*�����жϷ��ͻ��������ֵ*/


#define PCSETUART UART5	/*PC�������ô���*/
#define PCSETBUF PCSet_ValueBuf  /*PC�������ý��մ���������*/


#define ADMINPASSWORD 87654321   /*��������*/

#define AVRBUFSIZE   20 /*����ƽ����������С���������1*/

#define GETMSGWORD "FEDCBA98\r\n\r\nOK\r\n"	  /*���ն����жϱ�׼*/

#define FILENAMEADD 0x8003800  /*�ļ����洢λ�ã���IAP������أ����������޸�*/
#define REMOTEAPPADD 0x8040000 /*Զ�̸��³����ַ*/

#define OPEN 1		   /*���ˮ�ÿ��������ˮ�ö���ָʾ*/
#define CLOSE 0 	   /*���ˮ��ͣ�������ˮ�ö���ָʾ*/


#define YES		 1
#define NO		 0

#define RUNLED    do{GPIO_WriteBit(GPIOC, GPIO_Pin_12,(BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_12)));}while(0);		  /*RUN LED��˸*/

#define LEDALARMON GPIO_ResetBits(GPIOC,GPIO_Pin_5);    /*Alarm Led ON*/
#define LEDALARMING do{GPIO_WriteBit(GPIOC, GPIO_Pin_5,(BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_5)));}while(0); /*Alarm Led Twinkle*/
#define LEDALARMOFF GPIO_SetBits(GPIOC,GPIO_Pin_5);     /*Alarm Led OFF*/ 

/*��������*/
#define DIG1ON	   GPIO_SetBits(GPIOD,GPIO_Pin_2);     /*�����1��*/
#define DIG1OFF	   GPIO_ResetBits(GPIOD,GPIO_Pin_2);   /*�����1��*/
#define DIG2ON	   GPIO_SetBits(GPIOB,GPIO_Pin_3);     /*�����2��*/
#define DIG2OFF	   GPIO_ResetBits(GPIOB,GPIO_Pin_3);   /*�����2��*/
#define DIG3ON 	   GPIO_SetBits(GPIOB,GPIO_Pin_4);     /*�����3��*/
#define DIG3OFF	   GPIO_ResetBits(GPIOB,GPIO_Pin_4);   /*�����3��*/
#define DIG4ON	   GPIO_SetBits(GPIOB,GPIO_Pin_5);     /*�����4��*/
#define DIG4OFF    GPIO_ResetBits(GPIOB,GPIO_Pin_5);   /*�����4��*/
#define DDAUP      GPIO_SetBits(GPIOC,GPIO_Pin_11);    /*DDA �ߵ�ƽ*/
#define DDADOWN    GPIO_ResetBits(GPIOC,GPIO_Pin_11);  /*DDA �͵�ƽ*/
#define DLCUP      GPIO_SetBits(GPIOC,GPIO_Pin_10);    /*DLC �ߵ�ƽ*/
#define DLCDOWN    GPIO_ResetBits(GPIOC,GPIO_Pin_10);  /*DLC �͵�ƽ*/
#define DCKUP      GPIO_SetBits(GPIOA,GPIO_Pin_15);    /*DCK �ߵ�ƽ*/
#define DCKDOWN    GPIO_ResetBits(GPIOA,GPIO_Pin_15);  /*DCK �͵�ƽ*/

/*24V��Դ����*/
#define PSW1ON	   GPIO_SetBits(GPIOC,GPIO_Pin_4);		 /*24V A·��Դ����ͨ*/
#define PSW1OFF	   GPIO_ResetBits(GPIOC,GPIO_Pin_4);	 /*24V A·��Դ���Ͽ�*/
#define PSW1READ   GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_4) /*��ȡ 24V A·��Դ ���״̬*/
#define PSW2ON	   GPIO_SetBits(GPIOC,GPIO_Pin_5);		 /*24V B·��Դ����ͨ*/
#define PSW2OFF    GPIO_ResetBits(GPIOC,GPIO_Pin_5); 	 /*24V B·��Դ���Ͽ�*/
#define PSW2READ   GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_5) /*��ȡ 24V B·��Դ ���״̬*/

/*SWITCH OUTPUT ���������*/
#define PK1OPEN	   GPIO_SetBits(GPIOC,GPIO_Pin_7);		    /*�����������һ· ��ͨ*/
#define PK1CLOSE   GPIO_ResetBits(GPIOC,GPIO_Pin_7);	    /*�����������һ· �ر�*/
#define PK1READ    GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_7)	/*�����������һ· ��ȡ*/
#define PK2OPEN	   GPIO_SetBits(GPIOC,GPIO_Pin_6);		    /*����������ڶ�· ��ͨ*/
#define PK2CLOSE   GPIO_ResetBits(GPIOC,GPIO_Pin_6);	    /*����������ڶ�· �ر�*/
#define PK2READ    GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_6)	/*����������ڶ�· ��ȡ*/


/*SW INPUT �����������ȡ*/
#define SW1INREAD 	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8)	 /*�����������һ· ��ȡ*/
#define SW2INREAD 	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9)	 /*����������ڶ�· ��ȡ*/
/*����״̬��ȡ*/													
#define KEYSTATUS GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)	/*����״̬��ȡ*/

/*M35 ����*/
#define PWRKEYUP 	GPIO_ResetBits(GPIOA,GPIO_Pin_11);	 /*M35 PWR����*/
#define PWRKEYDOWN 	GPIO_SetBits(GPIOA,GPIO_Pin_11);	 /*M35 PWR����*/

/*RS485 ���*/
#define RS485TX		GPIO_SetBits(GPIOA,GPIO_Pin_1);    /*485дʹ��*/
#define RS485RX		GPIO_ResetBits(GPIOA,GPIO_Pin_1);    /*485��ʹ��*/

#define COUNT_POWER   10                   /*�жϽ������粻�����ж����ޣ�*/
#define COUNT_ALARMNUM 15                   /*�жϱ�����������*/
#define COUNT_PROTECTPHMAX  50             /*ÿСʱ��ͣ������������*/
#define COUNT_PROTECTNUM 5                 /*��ͣ�����¼����������ж�����*/
#define COUNT_GATHERIO 10                   /*IO�����ж�����*/
#define COUNT_CLOSE  500                    /*ˮ�ò������޺�ˮ�ö�����ʱ����λ20ms*/


#define COUNT_TIMGPRSSEND  120 /*GPRS��ʱͨ�ż�ʱ����λs*/
#define COUNT_RUNLED   5   /*RUNLED ��˸Ƶ�ʣ���λ100ms*/
#define COUNT_TIMCONNGPRS 100 /*GPRSͨ���ж��ж�,��λs*/
#define COUNT_TIMPUMPCONTROL  10 /*��ʱģʽ�£���ʱ�жϼ������λs*/
#define COUNT_TIMRELAYINTERVAL 10 /*�̵�����̹����������λs*/
#define COUNT_TIMDISTTIME 60    /*��Ļ��ʾʱ��У׼����λs*/
#define COUNT_TIMGPRSRECV 20     /*GPRS����ʱ��������ʱ��λ10ms*/
#define COUNT_TIMGPRSSENDINT 5   /*GPRS���͵�������ʱ��������С�������λ100ms*/
#define COUNT_GPRSRESTART 240     /*GPRSÿ�������������*/
#define COUNT_GETSIGNALQUALITY	4    /*��ȡ�ֻ��ź�ǿ�ȣ�ʱ��������λmin*/
#define COUNT_CLOSERESTART 90 		/*���յ�CLOSED,����ʱ���ݷ��ͼ�ʱ,��λs*/
#define COUNT_GPRSRESTEP 60        	/*GPRS����ʱ��*/
#define COUNT_GETMSGSTEP 60        /*���Ž��մ������������ʱ,��λs*/
#define COUNT_GPRSCONNECTEVENT 15  /*�����ж��¼�������������λmin*/
#define COUNT_TIMREMOTE	180        /*Զ�̸���������Чʱ���ʱ����λs*/
#define COUNT_I2CERROR 5 		   /*I2C���߳���λ�ȴ�ʱ�䣬��λs*/
#define COUNT_TIMADCGET 30       /*ADC��ȡ�������λs*/
#define COUNT_TIMADCOFFSET    5         /*ADCת����ʱ����λ100ms*/	/*���������޸�*/
#define COUNT_TIMGPRSBEAT 30        /*GPRSͨ���������������λs*/
#define COUNT_TIMSWGET 3        /*��������ȡ�������λs*/
#define COUNT_TIMSWOFFSET 3     /*�������ɼ���ʱ����λ100ms*/
#define COUNT_GPRSRECORDTIM 30   /*�¼���¼�ϴ����ظ���ʱ���Ƽ�ʱ*/

#define COUNT_GETSIGNAL	100   /*��ʱ��ȡ�ź�ǿ��*/


#define GATHERIO 2               /*���ذ忪�����ɼ�ʱ��,����������λ10ms*/


#define EVENTRECORDLEN 12		     /*�¼���¼����*/

#define EVENTRECORDBASE 200U	 /*�¼���¼��ʼ��ַ*/

#define EVENTRECORDFBASE 85  /*�¼���¼������¼��ַ1*/
#define EVENTRECORDSBASE 205  /*�¼���¼������¼��ַ2*/
#define EVENTRECORDTBASE 2010  /*�¼���¼������¼��ַ3*/

#define EVENTRECORDMAX 140			 /*�¼���¼�������*/
//#define EVENTRECORDCOUNTBASE 630     /*�¼���¼������¼��ʼ��ַ��ÿ��20��¼һ�ι�����*/
//#define EVENTRECORDADDRBASE  635     /*�¼���¼����ַ��¼��ʼ��ַ��ÿ��20��¼һ�ι�����*/
#define SPECIALVALUE1ADDBASE 0         /*��������洢��ַ��ʼ��ַ1*/
#define SPECIALVALUE2ADDBASE 120       /*��������洢��ַ��ʼ��ַ2*/
#define SPECIALVALUE3ADDBASE 1925      /*��������洢��ַ��ʼ��ַ3*/ /*��ȡ����ÿ����ַ�鳤��100Byte*/


#define CANREINITTIMEMIN 60 /*CAN���߳�ʼ������ͳ������,��λmin,���65535*/
#define CANREINITNUMMAX 30 /*CAN���߳�ʼ��ͳ������������ʼ������,���65535*/

#define COUNT_SHOWTIMLAST 8  /*ÿһ����ʾ����ʱ���������λ100ms*/

#define POWERMAX  15000  /*��ѹ���ֵ����λ0.001V*/
#define POWERMIN  10000  /*��ѹ��Сֵ����λ0.001V*/
#define ADCMAX    3000   /*ADC�ɼ�������ֵ����λ0.01mA*/
#define ADCMIN    360    /*ADC�ɼ���С����ֵ����λ0.01mA*/


#define PWR_SLEEPEntry_WFI         ((uint8_t)0x01)
#define PWR_SLEEPEntry_WFE         ((uint8_t)0x02)
#define SysCtrl_SLEEPONEXIT_Set    ((u16)0x0002)
#define SysCtrl_SLEEPDEEP_Set  ((u16)0x0004)

#endif
