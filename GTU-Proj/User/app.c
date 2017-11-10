#include "include.h"

u8 Time_Now[6] = {0}; /*��ǰʱ��*/
//const u8 LED_NUM[10] = {0x3F,0x0C,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F}; 
const u8 LED_NUM[34] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C, 0x39 ,0x5E ,0x79 ,0x71,0x00,	
					   0xBF,0x86,0xDB,0xCF,0xE6,0xED,0xFD,0x87,0xFF,0xEF,0xF7,0xFC,0xB9,0xDE,0xF9,0xF1,0x80}; /*�е�0-9 A,b,C,d,E,F����*/	 /*�޵�0-9 A,b,C,d,E,F,��*/
//const u8 LED_WORD[6] = {0x77,0xF8,0x72,0xBC,0xF2,0xE2}; /*A,b,C,d,E,F*/

u8 Server_IP[4] = {111,39,237,163};  /*��IPΪ255.255.255.255 �洢Ϊ0xFF,0xFF,0xFF,0xFF,��������*/
u16 Server_PORT = 8022;           /*��PORTΪ65535���洢Ϊ0xFFFF,��������*/
u8 Server_WWW[12] = {"server.com"};/*�����������洢���12�ֽڣ��϶�ʱ��'\0'������ֻ���������������*/
u8 Flag_ServerMod = 0; /*��������¼ģʽ��0ʱΪIP��¼��0xAAʱ��������¼*/
u8 PUMPHOUSENAME[40] = {"4E0A6D77514965B08DEF7AD9000A000D"};   /*�������ƣ�����ַ�����40��unicode��,���10������*/

u8 Telphone[5][14] = {{0x30,0x30,0x31,0x38,0x33,0x30,0x31,0x38,0x31,0x33,0x34,0x36,0x39,0xee},
					   {0x00,},}; /*��Ԥ��13λ����λ�ã�11λ����ǰ���ֽ�Ϊ'0',ÿ���������һλ��ʶ�ú����Ƿ����*//*���洢5���ֻ�����*/ 
					   
u8 serverpassword[] = {0xAA,0x55,0x06,0x48,0x92,0x27,0x77,0x03,0x44,0x55,0x66,0x33,0xCC}; /*��������¼����18217025502����Ϊ13λ���룬��һλ'1'ʡ�ԣ�֮��12λ��䵽6�ֽ���*/

u16 ADCBUF[80] = {0}; /*ADC���棬��4·ADC����20��ѭ��*/
u16 ADC_Value[4] = {0}; /*��·ADC�˲����ֵ���ֱ�Ϊ[0]��Դ��ѹ��[1]��һ·4-20mAģ������[2]�ڶ�·4-20mAģ������[3]2.5V��׼��ѹ*/
float D1[3] = {0.0,0.0,0.0};	/*3·ADC���Է��̽ؾ�*/
float K1[3] = {5.12,0.488,0.488};	/*3·ADC���Է���б��*/

u8 Flag_20mA_Power = NO;   /*�Ƿ���Ҫ��ͨ20mA��������Դ*/
u8 Flag_Switch_Power = NO; /*�Ƿ���Ҫ��ͨ�������ɼ���Դ*/
u8 Flag_20mA_Get = NO;     /*�Ƿ���Ҫ����20mA�������ɼ�*/
u8 Flag_Switch_Get = NO;   /*�Ƿ���Ҫ���п������ɼ�*/

u8 Flag_Error_20mA = 0;  /*0x11 ģ����1�ɼ�����0x22 ģ����2�ɼ����� 0x33 ģ����1\2�ɼ�������*/
u16 Analog_20mA[2] = {0xFFFF,0xFFFF}; /*20mAģ��������ֵ��0xFFFF��ʾ������Ч,����ֵ��λ0.01mA*/
u8 Switch_Station[2] = {0xFF,0xFF};           /*��ʾ�������ɼ�״̬��0xAA ͨ 0x55 ��*/
u8 Output_Station[2] = {0x55,0x55};           /*��ʾ���������״̬��0x55 �ض� 0xAA ��ͨ*/
u16 Power_Voltage = 12567;                    /*��ʾ��Դ��ѹ����λ0.001V,����ʾ��ѹ65.535V*/ 
u8 Signal_Quality = 0;      /*GPRS�ź�ǿ��*/
u8 Flag_ConnGPRSOK = YES;     /*GPRS�����Ƿ�����*/
u32 Count_GPRSRestart = 0;   /*ÿ��GPRS��������*/
u16 Count_TimGPRSRe = 50;    /*GPRS ��С������� 50s*/ 
u8 Flag_SendingMSG = NO;     /*�Ƿ����ڷ��Ͷ���*/
u8 Flag_SendingGPRS = NO;    /*�Ƿ����ڷ���GPRS��Ϣ*/
u8 Flag_RecvingGPRS = NO;    /*�Ƿ�GPRSģ�����ڽ���*/
u32 Flag_GPRSReStep = 0;     /*GPRS�����������*/
u8 Flag_GprsGetClose = NO;   /*���GPRS�Ƿ��յ�CLOSED*/
u16 Count_CloseRestart = COUNT_CLOSERESTART;  /*���յ�CLOSED,����ʱ���ݷ��ͼ�ʱ,��λs*/
u16 Flag_CountClose = 0;     /*��ǽ��յ�CLOSE�Ĵ�����ÿ5������*/
u16 Count_TimConnGPRS = COUNT_TIMCONNGPRS;    /*GPRSͨ���жϼ�ʱ,��λs*/
u8 Flag_NeedGPRSRecv = NO;	 /*�Ƿ���Ҫ����GPRS���մ���*/
u32 Flag_NeedGPRSSend = NO;	 /*�Ƿ���Ҫ����GPRS���ͣ�ÿһλ��ʾһ�ַ���Ҫ��1Ϊ��Ҫ��0Ϊ����Ҫ��0bit��ʾ��ʱ״̬���ͣ�*/
u16 Count_TimGPRSSend = COUNT_TIMGPRSSEND;   /*GTU״̬��ʱ���ͼ�ʱ����λs*/
u16 Count_TimGPRSBeat = COUNT_TIMGPRSBEAT;   /*GTU���������ͼ����ʱ����λs*/
u16 Count_TimGPRSSendInt = COUNT_TIMGPRSSENDINT; /*GPRS������С�����ʱ����λ100ms*/
u16 Count_TimGPRSRecv = 0;        /*GPRS����ʱ����ʱ����*/
u16 Count_GPRSRecordTim = COUNT_GPRSRECORDTIM;      /*�¼���¼�ϴ����ظ���ʱ��ʱ����λs*/

u8 Flag_EventNowRe = 0;           /*����Ƿ��յ�ʵʱ�¼��ϴ��ظ�*/
u8 Flag_EventPastRe = 0;      	  /*����Ƿ��յ��¼���¼�ϴ��ظ�*/

u8 GetMsg_Buf[400] = {0};         /*���Ž��յĻ�����*/
u8 GetMsg_Num[20] = {0};          /*���յ����ŵķ��ͺ���*/
u8 Flag_GetMsg = NO;              /*����Ƿ���յ�����*/
u8 Flag_GetWrongMsg = NO;         /*����Ƿ���յ��������*/
u8 Flag_GetMsgOver = NO;          /*��Ƕ����Ƿ�������*/
u16 Count_GetMsgStep = 0;         /*���Ž��ղ������*/
u16 Count_GetMsgBuf = 0;          /*���Ž��ջ������*/

u16 Count_TimRemote = 0;          /*Զ�̸��������ʱ*/
u8 Flag_RemoteCommond = NO;       /*���Զ�̸��������Ƿ���Ч*/
u16 Count_RemoteCommond = 0;      /*Զ�̸���������ռ���*/

u16 Count_RunLed = COUNT_RUNLED;  /*����ָʾ����˸�������λ100ms*/

//u16 Count_ADCInterval = COUNT_ADCINTERVAL; /*ADC��ȡ�������λs*/
//u16 Count_ADCDelay = COUNT_ADCDELAY;       /*ADCת����ʱ����λ100ms*/


u16 Event_RecordAddr = 0; /*�¼���¼����ַ*/
u16 Event_RecordNum = 0;  /*�¼���¼����*/
//u16 Event_UpdateAddr = 0; /*�¼��ϴ���ַ*/


u16 EventNowGPRS_Addr = 0;  /*�ϴ�ʵʱ�¼���ַ*/
u16 EventPastGPRS_Addr = 0; /*�ϴ��¼���¼��ַ*/
u16 EventGPRS_Addr = 0;	 /*���͵�GPRS���¼�������ַ*/

u16 Recv_SerCom = 0;      /*���յ��ķ�����������ڻظ�������*/
u16 Reply_Servalue = 0;   /*�ظ������������Ӧ�Ĳ��������ڻظ�������*/

u16 Count_TimADCGet = COUNT_TIMADCGET; /*ADC�ɼ��������ʱ����λs*/
u16 Count_TimADCOffset = COUNT_TIMADCOFFSET; /*ADC��ͨ����ٽ��вɼ���ʱ��������λ100ms*/
u8  Flag_ADCGet = NO; /*����Ƿ��ڽ���ADC�ɼ�*/

u16 Count_TimSWGet = COUNT_TIMSWGET; /*�������ɼ��������ʱ����λs*/
u16 Count_TimSWOffset = COUNT_TIMSWOFFSET; /*��������ͨ����ٽ��вɼ���ʱ��������λ100ms*/
u8  Flag_SWGet = NO; /*����Ƿ��ڽ��п������ɼ�*/

u8 Flag_ErrorShow[4] = {0x00,0x00,0x00,0x00};  /*������ʾ��������ִ���*/ /*01��Դ��ѹ������02��Դ��ѹ������03 1��4-20mA�ɼ�ͨ������ 04 2��4-20mA�ɼ�ͨ������*/
 
u16 Count_Gather1SW = 0; /*������1״̬�ɼ���ʱ*/
u16 Count_Gather2SW = 0; /*������2״̬�ɼ���ʱ*/

u8 Flag_ServerSend[32] = {0x00}; /*��������Ӧ����ͱ�ǣ�0xAA��ʾ��Ӧ������Ҫ���ͣ�����ֵ���ã�[0] ��ʱ����״̬��Ϣ��[1]����������ò�����[2]ʵʱ�¼�����*/
                                 /*[31] �ظ�����������*/

u8 GPRS_RecvCommondBuf[3][256] = {0}; /*GPRS���ջ������򣬹����Ի���3������*/
u8 Flag_RecvCommondBuf[3] = {0x00};   /*��ʶ�����������Ƿ���Ч��0xAA������Ч������ֵ������Ч*/

u8 Flag_OUT1OpenNeed = NO;	/*���������1�Ƿ���Ҫ��ͨ*/
u8 Flag_OUT1CloseNeed = NO;	/*���������1�Ƿ���Ҫ�ر�*/
u8 Flag_OUT2OpenNeed = NO;	/*���������2�Ƿ���Ҫ��ͨ*/
u8 Flag_OUT2CloseNeed = NO;	/*���������2�Ƿ���Ҫ�ر�*/

u16 ADC_AlarmHighRange[2] = {0};   /*��·4-20mAģ������������,��λ0.01mA*/
u16 ADC_AlarmLowRange[2] = {0};    /*��·4-20mAģ������������,��λ0.01mA*/
u8 Flag_ADCInUse[2] = {0};		   /*��·4-20mAģ�����Ƿ�����*/ /*0xAA ����  0x55 ������*/
u8 Flag_SWInUse[2] = {0};          /*��·�������ɼ��Ƿ�ʹ��*/   /*0xAA ����  0x55 ������*/
u8 Flag_OUT1Model = 0x00;          /*0x00 �����ÿ���������Զ�����*//*����1���ģʽ*/
								   /*0x11  1�ſ��������룬������(1������Ϊͨ�����Ϊͨ��1������Ϊ�ϣ����Ϊ��)*/
								   /*0x22  1�ſ��������룬������(1������Ϊͨ�����Ϊ�ϣ�1������Ϊ�ϣ����Ϊͨ)*/
								   /*0x33  2�ſ��������룬������(2������Ϊͨ�����Ϊͨ��2������Ϊ�ϣ����Ϊ��)*/
								   /*0x44  2�ſ��������룬������(2������Ϊͨ�����Ϊ�ϣ�2������Ϊ�ϣ����Ϊͨ)*/
								   /*0x55  1��ģ�����ɼ���������(1��ģ�����ɼ��������ޣ����Ϊ����1��ģ�����ɼ��������ޣ����Ϊ�ϣ�δ���ޱ���֮ǰ�����״̬)*/
								   /*0x66  1��ģ�����ɼ���������(1��ģ�����ɼ��������ޣ����Ϊ����1��ģ�����ɼ��������ޣ����Ϊ�ϣ�δ���ޱ���֮ǰ�����״̬)*/
								   /*0x77  2��ģ�����ɼ���������(2��ģ�����ɼ��������ޣ����Ϊͨ��2��ģ�����ɼ��������ޣ����Ϊ�ϣ�δ���ޱ���֮ǰ�����״̬)*/
								   /*0x88  2��ģ�����ɼ���������(2��ģ�����ɼ��������ޣ����Ϊͨ��2��ģ�����ɼ��������ޣ����Ϊ�ϣ�δ���ޱ���֮ǰ�����״̬)*/
u8 Flag_OUT2Model = 0x00;          /*��������ͬ��*/					/*����2���ģʽ*/

u8 Flag_I2COperation = NO;  /*I2Cͨ�ű��*/
u8 Flag_I2CError = YES;		/*���I2C�����Ƿ����*/
u16 Count_I2CError = COUNT_I2CERROR; /*I2C���ߴ���ȴ���ʱ*/

u16 Flag_SendMsg = 0;    /*�Ƿ���Ҫ���Ͷ��ţ�ÿһλ����һ������ѡ��*/
						 /*1bit ��Դ��ѹ�������� 2bit ��Դ��ѹ�������� 3bit 1��ģ�����ɼ����� 4bit 2��4-20mA�ɼ����� */
						 /*5bit �ɼ����������ޣ�6bit �ɼ����������ޣ�7bit �ڶ�·�ɼ����������ޣ�8bit �ڶ�·�ɼ�����������*/
u16 Flag_AlreadyMsg = 0;
u8 TelNum = 2;  /*���Ͷ��ŵ绰��Ŀ*/
u16 MsgLen = 0; /*���ų���*/
u16 Flag_SendMsgNum = 0;        /*���ŷ������̼�¼*/
u8 PDUCmd[16][100] = {"75356e907535538b4f4e4e8e4e0b9650000A000D",          /*���ž�������,unicode ���� ����Դ��ѹ��������*/
					"75356e907535538b9ad84e8e4e0a9650000A000D",    /*��Դ��ѹ��������*/
					"003153f76a2162df91cf91c796c66545969c0000A000D",            /*1��ģ�����ɼ�����*/
					"003253f76a2162df91cf91c796c66545969c000A000D",          /*2��ģ�����ɼ�����*/
					"91C796C691CF8D858FC74E0A9650000A000D",					/*�ɼ�����������*/
					"91c796c691cf4f4e4e8e4e0b9650000A000D",};				/*�ɼ�����������*/

u8 Flag_ButtonPress = NO;        /*��Ǵ�����ť�Ƿ񱻰��£�����ʱͨ�������ѭ����ʾ�豸״̬*/

u8 LED_ShowStep[12][4]= {0};     /*�������ʾ���裬�̶�Ϊ12����ÿ��4���������ʾ���ݶ�Ӧ��Ӧ�������±�*/
								 /*��һ����ʾ ADC1 ����ֵ����A11.0*/
								 /*�ڶ�����ʾ ADC2 ����ֵ����A.11.0*/
								 /*��������ʾ SW1  ������״̬����C 0E*/
								 /*���Ĳ���ʾ SW2  ������״̬����C.0FF*/
								 /*���岽��ʾ OUT1 ���״̬����O 0E*/
								 /*��������ʾ OUT2 ���״̬����O.0FF*/
								 /*���߲���ʾ SQ   �ź������� F 29*/
								 /*�ڰ˲���ʾ ERROR������Ϣ1��:E1 01*/ /*��ʾEx 00 û�д�����Ϣ*/
								 /*�ھŲ���ʾ ERROR������Ϣ2��:E2 02*/
								 /*��ʮ����ʾ ERROR������Ϣ3��:E3 03*/
								 /*��ʮһ����ʾERROR������Ϣ4��:E4 04*/
								 /*��ʮ������ʾ ��Դ��ѹ�� d 12.5 */

u16 Show_StepNum = 0;   /*����������ʾ����*/
u16 Count_ShowTimLast = COUNT_SHOWTIMLAST;   /*ÿһ����ʾ����ʱ���������λ100ms*/
u8 Flag_KeyDown = NO;   /*��ǰ�ť�Ƿ񱸰���*/
u16 Count_KeyDownLast = 0;  /*��ť����ʱ�䣬������*/
u8 Flag_LEDShowing = NO;  /*����Ƿ����ڽ���LED��ʾ*/

u8 Flag_EventRecord[20] = {0x00}; /*��Ƕ�Ӧ�¼��Ƿ���Ҫ��¼��0xAA ��Ҫ��¼��0x00 �����¼*/
								  /*[0] ��ѹ��������*/
								  /*[1] ��ѹ��������*/
								  /*[2] ADC1�ɼ�ͨ������*/
								  /*[3] ADC2�ɼ�ͨ������*/
u8 Flag_EventAlreadyRecord[20] = {0x00}; /*��Ӧ���ʱ���Ƿ��Ѿ���¼��0xAA �Ѿ���¼ 0x00 δ��¼*/
u8 LEDSHOWNUM =0;  /*����LED��̬ˢ��*/
u8 Flag_PCSETING = NO;  /*����Ƿ����ڽ���PC�������ú�ADCУ׼*//*��������ʱ����Դ���Ƴ��գ������нڵ����*/ 
u8 Flag_GetPCSet = NO;  /*�Ƿ��ȡPC���ö˿�����*/
u8 Flag_ADCSetAble = NO;  /*���ADC���ò����Ƿ���Ч*/
u16 ADC_HandleBUF[2][10] = {0}; /*ADC����������棬���������˲�*/
u8 Flag_FirstGetADC = NO;    /*����Ƿ��һ�λ�ȡADC*/ 

u16 Count_GetSignal = COUNT_GETSIGNAL;  /*��ʱ��ȡ�ź�ǿ��*/   

/******************************************************************************
*�������ƣ�GPRS_Relink
*�������ܣ�M35ģ������������
*�������룺��
*�����������
*�������أ���
*��ע��
******************************************************************************/
void GPRS_Relink(void)
{
	/*���յ�M35 CLOSE��Ĵ���*/
	if(YES == Flag_GprsGetClose)
	{
		if(0 == Count_CloseRestart)
		{	
			Flag_RecvingGPRS = YES;
			if(3 >= Flag_CountClose)  /*����CLOSE�ڣ��ж�Ϊ���Ӵ���*/
			{
				Count_TimConnGPRS = COUNT_TIMCONNGPRS;  /*���Ӵ���*/
			}
			else if(5 == Flag_CountClose)
			{
				Flag_CountClose = 0;
			}
			Flag_CountClose ++;
			Count_CloseRestart = COUNT_CLOSERESTART;
			USART_SendNData(USART3,GPRSCmd13,strlen((char *)GPRSCmd13));	 /*�رճ���*/
		}
		else if(((COUNT_CLOSERESTART)/2) == Count_CloseRestart)	    /*ATE0*/
		{
			Count_CloseRestart --;
			USART_SendNData(USART3,GPRSCmd7,strlen((char *)GPRSCmd7));	
		}
		else if(((COUNT_CLOSERESTART)/2-2) == Count_CloseRestart)	    /*����GPRS��APN*/
		{
			Count_CloseRestart --;
			USART_SendNData(USART3,GPRSCmd15,strlen((char *)GPRSCmd15));	
		}
		else if(((COUNT_CLOSERESTART/2)-4) == Count_CloseRestart)	    /*ɾ������*/
		{
			Count_CloseRestart --;
			USART_SendNData(USART3,GPRSCmd5,strlen((char *)GPRSCmd5)); /*ɾ�����ж���*/	
		}
		else if(((COUNT_CLOSERESTART*3)/4) == Count_CloseRestart)
		{
			Count_CloseRestart --;
			sprintf((char *)(GPRSCmd6+16),"\"%03d.%03d.%03d.%03d\",\"%d\"\r\0",Server_IP[0],Server_IP[1],Server_IP[2],Server_IP[3],Server_PORT);
			USART_SendNData(USART3,GPRSCmd6,strlen((char *)GPRSCmd6));
		}
		else if((COUNT_CLOSERESTART/4) == Count_CloseRestart)
		{
			Flag_GprsGetClose = NO;
			Count_CloseRestart --;
			Send_To_Server(serverpassword,13);				/*��¼������*/
		}	
	}
	/*GPRS��������*/
	else if(NO == Flag_ConnGPRSOK && COUNT_GPRSRESTART >= Count_GPRSRestart && 0 == Count_TimGPRSRe  &&NO == Flag_SendingMSG && NO == Flag_RecvingGPRS)   /*�����ж�*/  /*�ź������ϲ�ʱ����������*/
	{
		if(1 == Count_GPRSRestart%2)	/*ÿ������������һ���豸*/
		{
			if(0 == Flag_GPRSReStep)/*��һ��������ػ�����*/
			{
				USART_SendNData(USART3,GPRSCmd10,strlen((char *)GPRSCmd10));   /*�������ػ�*/
				Flag_GPRSReStep = COUNT_GPRSRESTEP;	
			}
			else if((COUNT_GPRSRESTEP - 5) == Flag_GPRSReStep) /*�ڶ�������*/
			{
				PWRKEYUP;    /*GPRS POW����*/
				Flag_GPRSReStep --;
			}
			else if((COUNT_GPRSRESTEP - 9) == Flag_GPRSReStep)/*������GPRSPOWOFF*/
			{
				PWRKEYDOWN;   /*GPRS POW����*/
				Flag_GPRSReStep --;	
			}
			else if((COUNT_GPRSRESTEP - 13) == Flag_GPRSReStep) /*���Ĳ�����*/
			{
				PWRKEYUP;    /*GPRS POW����*/
				Flag_GPRSReStep --;			  /*����14s*/
			}
		}
		else
		{
			if(0 == Flag_GPRSReStep)
			{
				
				USART_SendNData(USART3,GPRSCmd13,strlen((char *)GPRSCmd13));	 /*�رճ���*/
				Flag_GPRSReStep = COUNT_GPRSRESTEP - 25;
			}	
		}
		if((COUNT_GPRSRESTEP - 24) == Flag_GPRSReStep)      /*�رջ���*/
		{
			USART_SendNData(USART3,GPRSCmd7,strlen((char *)GPRSCmd7));  /*�رջ���*/
			Flag_GPRSReStep --;			  /*����25s*/
		}
		if((COUNT_GPRSRESTEP - 27) == Flag_GPRSReStep)      /*����GPRS��APN*/
		{
			USART_SendNData(USART3,GPRSCmd15,strlen((char *)GPRSCmd15));  /*����GPRS��APN*/
			Flag_GPRSReStep --;			  /*����25s*/
		}
		else if((COUNT_GPRSRESTEP - 30) == Flag_GPRSReStep)
		{
			sprintf((char *)(GPRSCmd6+16),"\"%03d.%03d.%03d.%03d\",\"%d\"\r\0",Server_IP[0],Server_IP[1],Server_IP[2],Server_IP[3],Server_PORT);
			USART_SendNData(USART3,GPRSCmd6,strlen((char *)GPRSCmd6));		 	 /*��������ͳ��*/
			Flag_GPRSReStep --;			  /*����31s*/
											
		}
		else if((COUNT_GPRSRESTEP - 35) == Flag_GPRSReStep)	  /*35-40ɾ������*/
		{
			USART_SendNData(USART3,GPRSCmd8,strlen((char *)GPRSCmd8)); /*text��ʽ�������������*/
			Flag_GPRSReStep --;	
			
		}
		else if((COUNT_GPRSRESTEP - 37) == Flag_GPRSReStep)	  /*35-40ɾ������*/
		{
			USART_SendNData(USART3,GPRSCmd2,strlen((char *)GPRSCmd2)); /*�������ȴ洢λ��*/
			Flag_GPRSReStep --;	
		}
		else if((COUNT_GPRSRESTEP - 39) == Flag_GPRSReStep)	  /*35-40ɾ������*/
		{
			USART_SendNData(USART3,GPRSCmd5,strlen((char *)GPRSCmd5)); /*ɾ�����ж���*/
			Flag_GPRSReStep --;	
		}
		else if ((COUNT_GPRSRESTEP - 44) == Flag_GPRSReStep)					   /*��¼������*/
		{
			Send_To_Server(serverpassword,13);
			Flag_GPRSReStep --;			  /*����45s*/
		} 
		else if( 2 == Flag_GPRSReStep)   /*��ʱ*/
		{
			Count_TimGPRSRe = 50;   /*�������50s*/
			Count_GPRSRestart ++;
			Count_TimConnGPRS = COUNT_TIMCONNGPRS;  /*���Ӵ���*/
		}  
	}
	else if (YES == Flag_ConnGPRSOK || COUNT_GPRSRESTART < Count_GPRSRestart)
	{
		Flag_GPRSReStep = 0;   /*��һ������*/	
	}
}
/******************************************************************************
*�������ƣ�Tim_Interrupt
*�������ܣ���ʱ�ж���ش���
*�������룺
*�����������
*�������أ���
*��ע��
******************************************************************************/
void Tim_Interrupt(void)
{
	if(0 == Count_TimGPRSSend && YES == Flag_ConnGPRSOK)   /*����ʱΪ0������������*/
	{
		Flag_ServerSend[0] = 0xAA;              /*GPRS��ʱ������λ*/
		Count_TimGPRSSend = COUNT_TIMGPRSSEND;  /*���¼�ʱ*/	
	}
	if(0 == Count_TimGPRSSendInt)		/*GPRS������С�����ʱ*/
	{
		Flag_SendingGPRS = NO;
	}
	if(0 == Count_GPRSRecordTim)    /*GPRS�¼��ظ��ȴ�ʱ��*/
	{
		Flag_EventNowRe = 0;
		Flag_EventPastRe = 0;
	}

	/*����״̬*/
	if(COUNT_GATHERIO == Count_KeyDownLast) /*�͵�ƽ*/
	{
		Flag_KeyDown = YES;
	}
	else
	{
		Flag_KeyDown = NO;
	}

	/*RUN LED*/
	if(0 == Count_RunLed)
	{
		RUNLED;
		Count_RunLed = COUNT_RUNLED;
	}

	if(0 == Count_TimGPRSRecv)
	{
		Flag_RecvingGPRS = NO;
	}

	/*��������ȡ���*/
	if(0 == Count_TimSWGet)
	{
		Count_TimSWGet = COUNT_TIMSWGET;
		Flag_Switch_Power = YES;
	}
	
	if(YES == Flag_Switch_Power && 0 == Count_TimSWOffset)
	{
		Flag_Switch_Get = YES;
	} 
	

	/*ģ�����ɼ����*/
	if(0 == Count_TimADCGet)
	{
		Count_TimADCGet = COUNT_TIMADCGET;
		Flag_20mA_Power = YES;
	}
	if(YES == Flag_20mA_Power && 0 == Count_TimADCOffset)
	{
		Flag_20mA_Get = YES;
	}

	/*�Ƿ�TCP���������ж�*/
	if(0 == Count_TimConnGPRS && YES == Flag_ConnGPRSOK)
	{
		Flag_ConnGPRSOK = NO;
	}
	else if(0 != Count_TimConnGPRS && NO == Flag_ConnGPRSOK)
	{
		Flag_ConnGPRSOK = YES;
	}
	/*�������������*/
	Relay_Output();
//	if(0 == )
}
/******************************************************************************
*�������ƣ�Get_Value
*�������ܣ���ȡ��ز���
*�������룺
*�����������
*�������أ���
*��ע��
******************************************************************************/
void Get_Value(void)
{
//	u16 Tmp_Adcbuf[4] = {0xFFFF,0xFFFF,0xFFFF,0xFFFF}; /*4·ADC���棬��һ· Vref2.5 �ڶ�· ��Դ��ѹ ����· 20mA ����· 20mA*/
//	u8 Tmp_Switchbuf[2] = {0xFF,0xFF}; /*���������*/
//	u16 j=0;i=0;

	/*get ADC*/
	if(YES == Flag_20mA_Power && LOW == PSW1READ)  /*�״��迪����Դ���ȴ�ת��*/
	{
		PSW1ON;
//		ADC_Cmd(ADC1, ENABLE);
//		DMA_Cmd(DMA1_Channel1, ENABLE);
		ADC_Configuration();
//		ADC_DMACmd(ADC1, ENABLE);
		Count_TimADCOffset = COUNT_TIMADCOFFSET;
		Flag_20mA_Get = NO;	
	}
	else if(YES == Flag_20mA_Power && HIGH == PSW1READ && YES == Flag_20mA_Get)	/*��ȡת��ֵ*/
	{
		ADC_ValueAvg(ADCBUF);	 /*adc �ɼ����ݻ�ȡƽ��ֵ*/
		Flag_20mA_Power = NO;
		Flag_20mA_Get = NO;
//		ADC_DMACmd(ADC1, DISABLE); 
//		DMA_Cmd(DMA1_Channel1, DISABLE);
//		DMA_Configuration();
		PSW1OFF;
		ADC_Cmd(ADC1, DISABLE);		
	}
	else  /*���������������*/
	{
	
	}
	/*get Switch*/
	if(YES == Flag_Switch_Power && LOW == PSW2READ)   /*�״��迪����Դ���ȴ��ɼ�*/
	{
		PSW2ON;
		Count_TimSWOffset = COUNT_TIMSWOFFSET;
		Flag_Switch_Get = NO;
	}
	else if(YES == Flag_Switch_Power && HIGH == PSW2READ && YES == Flag_Switch_Get)	/*��ȡ������״̬*/
	{
		if(Count_Gather1SW == GATHERIO)   /*�ȶ��ڵ͵�ƽ������ͨ״̬*/
		{
			Switch_Station[0] = 0xAA;
		}
		else                              /*����״̬�ж�Ϊ�ߵ�ƽ�����ض�״̬*/
		{
			Switch_Station[0] = 0x55;
		}

		if(Count_Gather2SW == GATHERIO)  /*�ȶ��ڵ͵�ƽ������ͨ״̬*/
		{
			Switch_Station[1] = 0xAA;
		}
		else                             /*����״̬�ж�Ϊ�ߵ�ƽ���ض�״̬*/
		{
			Switch_Station[1] = 0x55;
		}
		Flag_Switch_Power = NO;
		Flag_Switch_Get = NO;
		PSW2OFF;
	}

	/*get output*/
	if(LOW == PK1READ)   /*�������1���ڵ͵�ƽ*/
	{
		Output_Station[0] = 0x55;
	}
	else if(HIGH == PK1READ)
	{
		Output_Station[0] = 0xAA;	
	}
	if(LOW == PK2READ)  /*�������2���ڵ͵�ƽ*/
	{
		Output_Station[1] = 0x55;
	}
	else if(HIGH == PK2READ)
	{
		Output_Station[1] = 0xAA;
	}

	/*����adcֵ*/
	ADC_Value[0] =(0 == ADC_Value[0])?1:ADC_Value[0];		/*��ֹΪ0*/
	Power_Voltage = (u16)((ADC_Value[1]*(3413.33/ADC_Value[0])*K1[0]+D1[0]));  
	Analog_20mA[0] = (u16)((ADC_Value[2]*(3413.33/ADC_Value[0])*K1[1]+D1[1]));  
	Analog_20mA[1] = (u16)((ADC_Value[3]*(3413.33/ADC_Value[0])*K1[2]+D1[2]));  

	if(Power_Voltage > (u16)(POWERMAX*1.05))
	{
		Flag_EventRecord[0] = 0xAA;
		Flag_ErrorShow[0] = 0x01;
		Flag_SendMsg |= (0x1<<1);
	}
	else if(Power_Voltage <= POWERMAX) /*��ֹ����*/
	{
		Flag_EventRecord[0] = 0x00;
		Flag_ErrorShow[0] = 0x00;
		Flag_SendMsg &= ~(0x1<<1);
	}

	if(Power_Voltage < (u16)(POWERMIN*0.95))
	{
		Flag_EventRecord[1] = 0xAA;
		Flag_ErrorShow[1] = 0x02;
		Flag_SendMsg |= (0x1);
	}
	else if(Power_Voltage >= POWERMIN) /*��ֹ����*/
	{
		Flag_EventRecord[1] = 0x00;
		Flag_ErrorShow[1] = 0x00;
		Flag_SendMsg &= ~(0x1);
	}

	if((Analog_20mA[0] > (u16)(ADCMAX*1.05))||(Analog_20mA[0] < (u16)(ADCMIN*0.95)))
	{
		Flag_EventRecord[2] = 0xAA;
		Flag_ErrorShow[2] = 0x03;
		Flag_SendMsg |= (0x1<<2);
	}
	else if((Analog_20mA[0] >= ADCMIN)&&(Analog_20mA[0] <= ADCMAX))
	{
		Flag_EventRecord[2] = 0x00;
		Flag_ErrorShow[2] = 0x00;
		Flag_SendMsg &= ~(0x1<<2);
	}

	if((Analog_20mA[1] > (u16)(ADCMAX*1.05))||(Analog_20mA[1] < (u16)(ADCMIN*0.95)))
	{
		Flag_EventRecord[3] = 0xAA;
		Flag_ErrorShow[3] = 0x04;
		Flag_SendMsg |= (0x1<<3);
	}
	else if((Analog_20mA[1] >= ADCMIN)&&(Analog_20mA[1] <= ADCMAX))
	{
		Flag_EventRecord[3] = 0x00;
		Flag_ErrorShow[3] = 0x00;
		Flag_SendMsg &= ~(0x1<<3); 
	}
	if((u16)(ADC_AlarmHighRange[0]*1.05) < Analog_20mA[0] && 0xAA == Flag_ADCInUse[0])
	{
		Flag_SendMsg |= (0x1<<4);
	}
	else
	{
		Flag_SendMsg &= ~(0x1<<4);
	}
	
	if((u16)(ADC_AlarmLowRange[0]*0.95) >= Analog_20mA[0] && 0xAA == Flag_ADCInUse[0])
	{
		Flag_SendMsg |= (0x1<<5);
	}
	else
	{
		Flag_SendMsg &= ~(0x1<<5);
	}
   	
	if((u16)(ADC_AlarmHighRange[1]*1.05) < Analog_20mA[1] && 0xAA == Flag_ADCInUse[1])
	{
		Flag_SendMsg |= (0x1<<6);
	}
	else
	{
		Flag_SendMsg &= ~(0x1<<6);
	}
	
	if((u16)(ADC_AlarmLowRange[1]*0.95) >= Analog_20mA[1] && 0xAA == Flag_ADCInUse[1])
	{
		Flag_SendMsg |= (0x1<<7);
	}
	else
	{
		Flag_SendMsg &= ~(0x1<<7);
	}
	Event_RecordToFM();
}

/******************************************************************************
*�������ƣ�ADC_ValueAvg
*��������: adc�ɼ����ݻ�ȡ
*�������룺
*�����������
*�������أ���
*��ע��ADCͨ�� 9��11��12��13�ֱ��Ӧ1��2��3��4
******************************************************************************/
void ADC_ValueAvg(u16* ADCDataTab)
{
   u32 averagevaluetmp[4] = {0};
   u32 maxvalue[4], minvalue[4], i;
   u8 k=0;
   u16 tmpvalue[4] = {0};

   for(i=0;i<20;i++)
	{
		ADCBUF[i*4] = Get_Adc(ADC_Channel_9);
		ADCBUF[i*4+1] = Get_Adc(ADC_Channel_11);
		ADCBUF[i*4+2] = Get_Adc(ADC_Channel_12);
		ADCBUF[i*4+3] = Get_Adc(ADC_Channel_13);
	}

   for(i=0;i<4;i++)
   {
		averagevaluetmp[i]=0;
		maxvalue[i]=0;
		minvalue[i]=0xffff;		
   }

   for (i=0;i<20;i++) 
   { 
     for(k=0;k<4;k++)
	 {
	 	averagevaluetmp[k] += *(ADCDataTab+k+i*4);

  	 	if(*(ADCDataTab+k+i*4)>maxvalue[k])   
  	 		maxvalue[k]=*(ADCDataTab+k+i*4);
    
  	 	if(*(ADCDataTab+k+i*4)<minvalue[k])
  	 		minvalue[k]=*(ADCDataTab+k+i*4);
	 }
   }

   ADC_Value[0]=(u32)(((averagevaluetmp[0]-maxvalue[0]-minvalue[0])/18))&0x0000ffff;  /*2.5V��׼��ѹ*/
   ADC_Value[1]=(u32)(((averagevaluetmp[1]-maxvalue[1]-minvalue[1])/18))&0x0000ffff;  /*12V��Դ��ѹ*/
//   ADC_Value[2]=(u32)(((averagevaluetmp[2]-maxvalue[2]-minvalue[2])/18))&0x0000ffff;
//   ADC_Value[3]=(u32)(((averagevaluetmp[3]-maxvalue[3]-minvalue[3])/18))&0x0000ffff;
	tmpvalue[2] = (u32)(((averagevaluetmp[2]-maxvalue[2]-minvalue[2])/18))&0x0000ffff;
	tmpvalue[3] = (u32)(((averagevaluetmp[3]-maxvalue[3]-minvalue[3])/18))&0x0000ffff;
	if(NO == Flag_FirstGetADC)
	{
		Flag_FirstGetADC = YES;
		for(i=0;i<10;i++)
		{
			ADC_HandleBUF[0][i] = tmpvalue[2];
			ADC_HandleBUF[1][i] = tmpvalue[3];
		}
	}
	else
	{
		for(i=0;i<9;i++)
		{
			ADC_HandleBUF[0][i] = ADC_HandleBUF[0][i+1];
			ADC_HandleBUF[1][i] = ADC_HandleBUF[1][i+1];
		}
		ADC_HandleBUF[0][9] = tmpvalue[2];
		ADC_HandleBUF[1][9] = tmpvalue[3];
	}
	ADC_Value[2] = ADC_HandlePoj(ADC_HandleBUF[0],10); /*���ֵǰ��λȡƽ������*/
	ADC_Value[3] = ADC_HandlePoj(ADC_HandleBUF[1],10); /*���ֵǰ��λȡƽ������*/
}


u16 ADC_HandlePoj(u16*tmpbuf,u16 num)
{
	u16 tmpmaxmax = 0;  /*���ֵ*/
	u16 tmpmaxmin = 0;  /*�δ�ֵ*/
	u16 i=0;
	for(i=0;i<num;i++)
	{
		if(tmpbuf[i] > tmpmaxmax)
		{
			tmpmaxmin = tmpmaxmax;
			tmpmaxmax = tmpbuf[i];
		}
		else if(tmpbuf[i] > tmpmaxmin)
		{
			tmpmaxmin = tmpbuf[i];
		}
	}

	return ((tmpmaxmax+tmpmaxmin)/2);
}

/******************************************************************************
*�������ƣ�Communication
*��������: ����GPRSͨ��
*�������룺
*�����������
*�������أ���
*��ע��
******************************************************************************/
void Communication(void)
{
	if(YES == Flag_NeedGPRSRecv)
	{
		GPRS_Recv();
		Flag_NeedGPRSRecv = NO;
	}
	
	if((Flag_SendMsg) && (Flag_SendMsg != ( Flag_SendMsg & Flag_AlreadyMsg))) /*��Ҫ���Ͷ��ţ���û�����Ҷ��ű�ǲ�Ϊ��*/
	{
		Send_Msg();			/*���Ͷ���*/
		if(300+300*TelNum == Flag_SendMsgNum)	   /*�������*/
		{
			Flag_AlreadyMsg = Flag_SendMsg;
			Flag_SendingMSG =NO;
			Flag_SendMsgNum = 0;
		}
	}
	else if(!Flag_SendMsg)			 /*���跢��*/
	{
		Flag_AlreadyMsg = 0;
		Flag_SendingMSG = NO;
		Flag_SendMsgNum = 0;
	}
	else
	{
		Flag_SendingMSG = NO;
		Flag_SendMsgNum = 0;
	}
	
	if(NO == Flag_SendingGPRS && YES == Flag_ConnGPRSOK && NO == Flag_SendingMSG && NO == Flag_RecvingGPRS)	 /*��������û�з��͡�����*/
	{
		GPRS_Send();	
	}
	else 
	{
		if(0 == Count_GetSignal)
		{
			Count_GetSignal = COUNT_GETSIGNAL;
			USART_SendNData(USART3,GPRSCmd11,7);
		}
	}	
}

///******************************************************************************
//*�������ƣ�LED_Display
//*��������: LED��ʾ
//*�������룺
//*�����������
//*�������أ���
//*��ע��
//******************************************************************************/
//void LED_Display(void)
//{
//	
//}

/******************************************************************************
*�������ƣ�Relay_Output
*��������: ���������
*�������룺
*�����������
*�������أ���
*��ע��
******************************************************************************/
void Relay_Output(void)
{

	/*��һ·�������*/

	/*�Ƿ���Ҫ�Զ�����*/
	switch(Flag_OUT1Model)
	{
		case 0x00:break;     /*����Ҫ�Զ�����*/
		case 0x11:if(0xAA == Switch_Station[0] && 0xAA == Flag_SWInUse[0])  /*������1������*/
				  {
					  Flag_OUT1OpenNeed = YES;
					  Flag_OUT1CloseNeed = NO;	
				  }
				  else if(0x55 == Switch_Station[0] && 0xAA == Flag_SWInUse[0]) 
				  {
					  Flag_OUT1OpenNeed = NO;
					  Flag_OUT1CloseNeed = YES;
				  }
				  break;
		case 0x22:if(0xAA == Switch_Station[0] && 0xAA == Flag_SWInUse[0])  /*������1������*/
				  {
					  Flag_OUT1OpenNeed = NO;
					  Flag_OUT1CloseNeed = YES;	
				  }
				  else if(0x55 == Switch_Station[0] && 0xAA == Flag_SWInUse[0]) 
				  {
					  Flag_OUT1OpenNeed = YES;
					  Flag_OUT1CloseNeed = NO;
				  }
				  break;
		case 0x33:if(0xAA == Switch_Station[1] && 0xAA == Flag_SWInUse[1])  /*������2������*/
				  {
					  Flag_OUT1OpenNeed = YES;
					  Flag_OUT1CloseNeed = NO;	
				  }
				  else if(0x55 == Switch_Station[1] && 0xAA == Flag_SWInUse[1]) 
				  {
					  Flag_OUT1OpenNeed = NO;
					  Flag_OUT1CloseNeed = YES;
				  }
				  break;
		case 0x44:if(0xAA == Switch_Station[1] && 0xAA == Flag_SWInUse[1])  /*������2������*/
				  {
					  Flag_OUT1OpenNeed = NO;
					  Flag_OUT1CloseNeed = YES;	
				  }
				  else if(0x55 == Switch_Station[1] && 0xAA == Flag_SWInUse[1]) 
				  {
					  Flag_OUT1OpenNeed = YES;
					  Flag_OUT1CloseNeed = NO;
				  }
				  break;
		case 0x55:if((0xFFFF != Analog_20mA[0]) && (ADC_AlarmHighRange[0] <= Analog_20mA[0]) 
					 && 0xAA == Flag_ADCInUse[0] && (ADC_AlarmHighRange[0] > ADC_AlarmLowRange[0])) /*ģ����1������*/
				  {
				      Flag_OUT1OpenNeed = YES;
					  Flag_OUT1CloseNeed = NO;
				  }
				  else if((0xFFFF != Analog_20mA[0]) && (ADC_AlarmLowRange[0] >= Analog_20mA[0]) 
				          && 0xAA == Flag_ADCInUse[0] && (ADC_AlarmHighRange[0] > ADC_AlarmLowRange[0]))
				  {
				  	  Flag_OUT1OpenNeed = NO;
					  Flag_OUT1CloseNeed = YES;
				  }
				  break;
		case 0x66:if((0xFFFF != Analog_20mA[0]) && (ADC_AlarmHighRange[0] <= Analog_20mA[0]) 
		             && 0xAA == Flag_ADCInUse[0] && (ADC_AlarmHighRange[0] > ADC_AlarmLowRange[0])) /*ģ����1������*/
				  {
				      Flag_OUT1CloseNeed = YES;
					  Flag_OUT1OpenNeed = NO;
				  }
				  else if((0xFFFF != Analog_20mA[0]) && (ADC_AlarmLowRange[0] >= Analog_20mA[0]) 
				          && 0xAA == Flag_ADCInUse[0] && (ADC_AlarmHighRange[0] > ADC_AlarmLowRange[0]))
				  {
				  	   Flag_OUT1OpenNeed = YES;
					   Flag_OUT1CloseNeed = NO;
				  }
				  break;
		case 0x77:if((0xFFFF != Analog_20mA[1]) && (ADC_AlarmHighRange[1] <= Analog_20mA[1]) 
		             && 0xAA == Flag_ADCInUse[1] && (ADC_AlarmHighRange[1] > ADC_AlarmLowRange[1])) /*ģ����2������*/
				  {
				      Flag_OUT1OpenNeed = YES;
					  Flag_OUT1CloseNeed = NO;
				  }
				  else if((0xFFFF != Analog_20mA[1]) && (ADC_AlarmLowRange[1] >= Analog_20mA[1]) 
				          && 0xAA == Flag_ADCInUse[1] && (ADC_AlarmHighRange[1] > ADC_AlarmLowRange[1]))
				  {
				  	  Flag_OUT1OpenNeed = NO;
					  Flag_OUT1CloseNeed = YES;
				  }
				  break;
		case 0x88:if((0xFFFF != Analog_20mA[1]) && (ADC_AlarmHighRange[1] <= Analog_20mA[1]) 
		             && 0xAA == Flag_ADCInUse[1] && (ADC_AlarmHighRange[1] > ADC_AlarmLowRange[1])) /*ģ����2������*/
				  {
				      Flag_OUT1CloseNeed = YES;
					  Flag_OUT1OpenNeed = NO;
				  }
				  else if((0xFFFF != Analog_20mA[1]) && (ADC_AlarmLowRange[1] >= Analog_20mA[1]) 
				          && 0xAA == Flag_ADCInUse[1] && (ADC_AlarmHighRange[1] > ADC_AlarmLowRange[1]))
				  {
				  	   Flag_OUT1OpenNeed = YES;
					   Flag_OUT1CloseNeed = NO;
				  }
				  break;
		default:break;
	}

	/*�ڶ�·�������*/

	/*�Ƿ���Ҫ�Զ�����*/
	switch(Flag_OUT1Model)
	{
		case 0x00:break;     /*����Ҫ�Զ�����*/
		case 0x11:if(0xAA == Switch_Station[0] && 0xAA == Flag_SWInUse[0])  /*������1������*/
				  {
					  Flag_OUT2OpenNeed = YES;
					  Flag_OUT2CloseNeed = NO;	
				  }
				  else if(0x55 == Switch_Station[0] && 0xAA == Flag_SWInUse[0]) 
				  {
					  Flag_OUT2OpenNeed = NO;
					  Flag_OUT2CloseNeed = YES;
				  }
				  break;
		case 0x22:if(0xAA == Switch_Station[0] && 0xAA == Flag_SWInUse[0])  /*������1������*/
				  {
					  Flag_OUT2OpenNeed = NO;
					  Flag_OUT2CloseNeed = YES;	
				  }
				  else if(0x55 == Switch_Station[0] && 0xAA == Flag_SWInUse[0]) 
				  {
					  Flag_OUT2OpenNeed = YES;
					  Flag_OUT2CloseNeed = NO;
				  }
				  break;
		case 0x33:if(0xAA == Switch_Station[1] && 0xAA == Flag_SWInUse[1])  /*������2������*/
				  {
					  Flag_OUT2OpenNeed = YES;
					  Flag_OUT2CloseNeed = NO;	
				  }
				  else if(0x55 == Switch_Station[1] && 0xAA == Flag_SWInUse[1]) 
				  {
					  Flag_OUT2OpenNeed = NO;
					  Flag_OUT2CloseNeed = YES;
				  }
				  break;
		case 0x44:if(0xAA == Switch_Station[1] && 0xAA == Flag_SWInUse[1])  /*������2������*/
				  {
					  Flag_OUT2OpenNeed = NO;
					  Flag_OUT2CloseNeed = YES;	
				  }
				  else if(0x55 == Switch_Station[1] && 0xAA == Flag_SWInUse[1]) 
				  {
					  Flag_OUT2OpenNeed = YES;
					  Flag_OUT2CloseNeed = NO;
				  }
				  break;
		case 0x55:if((0xFFFF != Analog_20mA[0]) && (ADC_AlarmHighRange[0] <= Analog_20mA[0]) && 0xAA == Flag_ADCInUse[0]) /*ģ����1������*/
				  {
				      Flag_OUT2OpenNeed = YES;
					  Flag_OUT2CloseNeed = NO;
				  }
				  else if((0xFFFF != Analog_20mA[0]) && (ADC_AlarmLowRange[0] >= Analog_20mA[0]) && 0xAA == Flag_ADCInUse[0])
				  {
				  	  Flag_OUT2OpenNeed = NO;
					  Flag_OUT2CloseNeed = YES;
				  }
				  break;
		case 0x66:if((0xFFFF != Analog_20mA[0]) && (ADC_AlarmHighRange[0] <= Analog_20mA[0]) && 0xAA == Flag_ADCInUse[0]) /*ģ����1������*/
				  {
				      Flag_OUT2CloseNeed = YES;
					  Flag_OUT2OpenNeed = NO;
				  }
				  else if((0xFFFF != Analog_20mA[0]) && (ADC_AlarmLowRange[0] >= Analog_20mA[0]) && 0xAA == Flag_ADCInUse[0])
				  {
				  	   Flag_OUT2OpenNeed = YES;
					   Flag_OUT2CloseNeed = NO;
				  }
				  break;
		case 0x77:if((0xFFFF != Analog_20mA[1]) && (ADC_AlarmHighRange[1] <= Analog_20mA[1]) && 0xAA == Flag_ADCInUse[1]) /*ģ����2������*/
				  {
				      Flag_OUT2OpenNeed = YES;
					  Flag_OUT2CloseNeed = NO;
				  }
				  else if((0xFFFF != Analog_20mA[1]) && (ADC_AlarmLowRange[1] >= Analog_20mA[1]) && 0xAA == Flag_ADCInUse[1])
				  {
				  	  Flag_OUT2OpenNeed = NO;
					  Flag_OUT2CloseNeed = YES;
				  }
				  break;
		case 0x88:if((0xFFFF != Analog_20mA[1]) && (ADC_AlarmHighRange[1] <= Analog_20mA[1]) && 0xAA == Flag_ADCInUse[1]) /*ģ����2������*/
				  {
				      Flag_OUT2CloseNeed = YES;
					  Flag_OUT2OpenNeed = NO;
				  }
				  else if((0xFFFF != Analog_20mA[1]) && (ADC_AlarmLowRange[1] >= Analog_20mA[1]) && 0xAA == Flag_ADCInUse[1])
				  {
				  	   Flag_OUT2OpenNeed = YES;
					   Flag_OUT2CloseNeed = NO;
				  }
				  break;
		default:break;
	}

	/*�������*/
	if(YES == Flag_OUT1OpenNeed)   /*��һ·��Ҫ��ͨ*/
	{
		if(0xAA == Output_Station[0])   /*�Ѿ��ڿ�״̬*/
		{
			
		}
		else if(0x55 == Output_Station[0]) /*�ڹ�״̬*/
		{
			PK1OPEN;
		}
		Flag_OUT1CloseNeed = NO;  
	}
	else if(YES == Flag_OUT1CloseNeed)  /*��һ·��Ҫ�ر�*/
	{
		if(0xAA == Output_Station[0])   /*�Ѿ��ڿ�״̬*/
		{
			PK1CLOSE;
		}
		else if(0x55 == Output_Station[0]) /*�ڹ�״̬*/
		{
			
		}
		Flag_OUT1OpenNeed = NO;
	}

	if(YES == Flag_OUT2OpenNeed)   /*�ڶ�·��Ҫ��ͨ*/
	{
		if(0xAA == Output_Station[1])   /*�Ѿ��ڿ�״̬*/
		{
			
		}
		else if(0x55 == Output_Station[1]) /*�ڹ�״̬*/
		{
			PK2OPEN;
		}
		Flag_OUT2CloseNeed = NO;  
	}
	else if(YES == Flag_OUT2CloseNeed)  /*��һ·��Ҫ�ر�*/
	{
		if(0xAA == Output_Station[1])   /*�Ѿ��ڿ�״̬*/
		{
			PK2CLOSE;
		}
		else if(0x55 == Output_Station[1]) /*�ڹ�״̬*/
		{
			
		}
		Flag_OUT2OpenNeed = NO;
	}
	
}

/******************************************************************************
*�������ƣ�GPRS_Send
*��������: GPRS���͹���
*�������룺
*�����������
*�������أ���
*��ע��
******************************************************************************/
void GPRS_Send(void)
{
	u16 i = 0,j = 0;
	u8 tmpbuf[150] = {0xF5,0xFA};
	u16 tmpcrc = 0;
	u32 tmpaddr = 0;
	if(0xAA == Flag_ServerSend[2] && (0 == Flag_EventNowRe))
	{
		tmpbuf[2] = 0x0c;  /*�¼����������򳤶�12B*/
		tmpbuf[3] = 0x03;  /*�¼�������0x03*/
		tmpbuf[4] = 0x00;	 /*��ַ���ȱ*/
		Flag_ServerSend[2] = 0x00;      /*�ȴ���һ�η���*/

		if(Event_RecordAddr > EVENTRECORDBASE)
		{
			tmpaddr = Event_RecordAddr-EVENTRECORDLEN;
		}
		else	 /*��¼�۷���ȡ*/
		{
			tmpaddr = EVENTRECORDBASE*EVENTRECORDMAX-EVENTRECORDLEN;
		}
		Flag_EventNowRe = 1;             /*�ȴ��¼����ͻظ�*/
		Count_GPRSRecordTim = COUNT_GPRSRECORDTIM;        /*�ȴ�10s�ظ�*/
		EventNowGPRS_Addr = tmpaddr;	 /*�ȴ�ȷ��*/
		FM25_RE_NBYTE(tmpaddr,tmpbuf+5,EVENTRECORDLEN);/*��ȡ���һ���¼���¼*/
		if(0 == tmpbuf[15])   /*��δ�ϴ�*/
		{
			tmpcrc = 0;   /*����*/
			tmpcrc = crc16_ccitt(tmpbuf,15);
			tmpbuf[15] = (tmpcrc>>8)&0xff;
			tmpbuf[16] = tmpcrc&0xff;
	
			Send_To_Server(tmpbuf,17);       /*���͵�������*/
		}	
	}
	else if(0xAA == Flag_ServerSend[0])	   /*��ʱ����*/
	{
		tmpbuf[2] = 37; /*�����򳤶�*/
		tmpbuf[3] = 0xE1;  /*������*/
		tmpbuf[4] = 0x00;
		for(i=0;i<6;i++)
		{
			tmpbuf[5+i] = Time_Now[i];   /*ʱ���*/
		}	
		tmpbuf[11] = (Analog_20mA[0]>>8) & 0xFF;	/*��һ·4-20mA����ֵ*/
		tmpbuf[12] = Analog_20mA[0]& 0xFF;
		tmpbuf[13] = (Analog_20mA[1]>>8) & 0xFF;	/*�ڶ�·4-20mA����ֵ*/
		tmpbuf[14] = Analog_20mA[1]& 0xFF;
		tmpbuf[15] = Switch_Station[0];             /*������1�ɼ�״̬*/
		tmpbuf[16] = Switch_Station[1];             /*������2�ɼ�״̬*/
		for(i=0;i<4;i++)							/*Ԥ��0x00*/
		{
			tmpbuf[17+i] = 0x00;
		}
		tmpbuf[21] = Output_Station[0];             /*���������1״̬*/
		tmpbuf[22] = Output_Station[1];             /*���������2״̬*/
		for(i=0;i<4;i++)							/*Ԥ��0x00*/
		{
			tmpbuf[23+i] = 0x00;
		}
		tmpbuf[27] = (Power_Voltage>>8) & 0xFF;      /*��Դ��ѹ*/
		tmpbuf[28] = Power_Voltage & 0xFF;
		for(i=0;i<11;i++)							 /*Ԥ���ռ�*/
		{
			tmpbuf[29+i] = 0x00;
		}
		tmpcrc = crc16_ccitt(tmpbuf,40);
		tmpbuf[40] = (tmpcrc >>8) & 0xFF;
		tmpbuf[41] = tmpcrc & 0xFF;
		Send_To_Server(tmpbuf,42);                   /*���͵�������*/
		Flag_ServerSend[0] = 0x00;
	}
	else if(0xAA == Flag_ServerSend[1])	             /*���ò����ϴ�*/
	{
		tmpbuf[2] = 102;							 /*�����򳤶�*/
		tmpbuf[3] = 0xE2;							 /*������*/
		tmpbuf[4] = 0x00;							 /*��ַ��*/
		for(i=0;i<2;i++)							 /*4-20mAģ��������������*/
		{
			tmpbuf[5+4*i] = (ADC_AlarmHighRange[0+i]>>8) & 0xFF;
			tmpbuf[6+4*i] = ADC_AlarmHighRange[0+i] & 0xFF;
			tmpbuf[7+4*i] = (ADC_AlarmLowRange[0+i]>>8) & 0xFF;
			tmpbuf[8+4*i] = ADC_AlarmLowRange[0+i] & 0xFF;
		}
		for(i=0;i<8;i++)		                     /*Ԥ��*/
		{
			tmpbuf[13+i] = 0x00;
		}
		tmpbuf[20] = Flag_OUT1Model;				 /*����1���ģʽ*/
		tmpbuf[21] = Flag_OUT2Model; 				 /*����2���ģʽ*/
		tmpbuf[22] = 0x00;                           /*Ԥ��*/
		tmpbuf[23] = 0x00;                           /*Ԥ��*/
		for(i=0;i<2;i++)
		{
			tmpbuf[24+i] = Flag_ADCInUse[i];		 /*��·4-20mAģ�����Ƿ�����*/
		}
		for(i=0;i<2;i++)
		{
			tmpbuf[26+i] = Flag_SWInUse[i];          /*��·�������ɼ��Ƿ�����*/
		}
		for(i=0;i<5;i++)
		{
			for(j=0;j<12;j++)						 /*Э���ݶ�Ϊ11λ����*/
			{
				tmpbuf[28+12*i+j] = Telphone[i][2+j];    
			}
		}
		for(i=0;i<16;i++)
		{
			tmpbuf[88+i] = 0x00;                     /*Ԥ��*/
		}
		tmpcrc = crc16_ccitt(tmpbuf,105);
		tmpbuf[105] = (tmpcrc >>8) & 0xFF;
		tmpbuf[106] = tmpcrc & 0xFF;
		Send_To_Server(tmpbuf,107);                   /*���͵�������*/
		Flag_ServerSend[1] = 0x00;         
	}
	else if (Count_GPRSRecordTim == 0 && EventGPRS_Addr != Event_RecordAddr && 0 == Flag_EventPastRe)  /*�����¼���¼�Ƿ���Ҫ����*/  /*��һ�������Ѿ��ָ�*/
	{
		tmpbuf[2] = 0x0c;  /*�����򳤶�*/
		tmpbuf[3] = 0x04;	 /*������*/
		tmpbuf[4] = 0x00;	 /*��ַ��*/
		for(i=0;i<5;i++)
		{
			if((EventGPRS_Addr < EVENTRECORDBASE)
			  || (EventGPRS_Addr >= (EVENTRECORDBASE+ EVENTRECORDLEN*EVENTRECORDMAX)))
			{
				EventGPRS_Addr = EVENTRECORDBASE;
			}
			if(EventGPRS_Addr == Event_RecordAddr)
			{
				break;
			}
			FM25_RE_NBYTE(EventGPRS_Addr,tmpbuf+5,EVENTRECORDLEN);
			if(tmpbuf[15] != 1)	/*��δ����*/
			{
				tmpcrc = crc16_ccitt(tmpbuf,15);
				tmpbuf[15] = (tmpcrc>>8)&0xff;
				tmpbuf[16] = tmpcrc&0xff;
				Send_To_Server(tmpbuf,17);
				Flag_EventPastRe = 1;   /*�ȴ��ظ�*/
				Count_GPRSRecordTim = COUNT_GPRSRECORDTIM;        /*�ȴ�30s�ظ�*/
				EventPastGPRS_Addr = EventGPRS_Addr;
				break;
			}
			EventGPRS_Addr += EVENTRECORDLEN;
		}
	}
	
	if (Count_TimGPRSBeat == 0)	   /* �ϴ����´������жϳ���ʱ�ޣ�����������*/
	{
		tmpbuf[0] = 0xAA;   /*������*/
		Send_To_Server(tmpbuf,1);
	}

}

/******************************************************************************
*�������ƣ�Event_RecordToFM
*��������: �洢�¼�
*�������룺
*�����������
*�������أ���
*��ע��
******************************************************************************/
void Event_RecordToFM(void)
{
	u16 i = 0;
	for(i=0;i<20;i++)
	{
		if(0xAA ==  Flag_EventRecord[i])
		{
			if(0xAA ==  Flag_EventAlreadyRecord[i])
			{}
			else
			{
				Record_Event(i,0);
				Flag_EventAlreadyRecord[i] = 0xAA;
			}
		}
		else
		{
			if(0xAA ==  Flag_EventAlreadyRecord[i])
			{
				Flag_EventAlreadyRecord[i] = 0x00;
			}
			else
			{
				
			}
		}
	}
}

/*******************************************************************************
*�������ƣ�Record_Delete
*�������ܣ�ɾ���¼���¼
*�������ã�
*�������룺��
*�����������ȡ�ļ�¼��ز���
*�������أ���
*��ע��
*******************************************************************************/
void Record_Delate(void)
{
	u8 tmp[8] = {0};
	u16 i = 0;
	memset(tmp,0,8);
	Event_RecordNum = 0;/*�¼���¼����*/
	Event_RecordAddr = EVENTRECORDBASE;/*��ǰ�¼���¼��ַ*/
	for(i=0;i<2;i++)
	{
		tmp[i] = (Event_RecordNum>>(8*(1-i)))&0xff;
	}
	for(i=0;i<2;i++)
	{
		tmp[2+i] = (Event_RecordAddr>>(8*(1-i)))&0xff;
	}
	tmp[4] = 0x00;
	FM25_WR_NBYTE(EVENTRECORDFBASE,tmp,5);
	FM25_WR_NBYTE(EVENTRECORDSBASE,tmp,5);
	FM25_WR_NBYTE(EVENTRECORDTBASE,tmp,5);
}

/*******************************************************************************
*�������ƣ�Record_Event
*�������ܣ���¼�¼�
*�������ã�
*�������룺�¼���¼����ַ��ȫ�֣����¼��Ѽ�¼�ܸ�����ȫ�֣����¼��š��¼�����
*�����������ַ���¡���������
*�������أ���
*��ע��
*******************************************************************************/
void Record_Event(u16 num,u16 tmpval)
{
	u8 tmp[12] = {0};
	u8 i = 0;
	for(i=0;i<6;i++)
	{
		tmp[i] = Time_Now[i];   /*��ȡ��ǰʱ��*/
	}
	tmp[6] = (num/256)&0xff;		   /*�¼�������Ϊ2Byte*/
	tmp[7] = (num%256)&0xff;
	tmp[8] = tmpval/256;			   /*����ֵ 2Bytes*/
	tmp[9] = tmpval%256;
	tmp[10] = 0;                       /*����Ƿ��ѷ��͵�������*/
	tmp[11] = 0;	                   /*Ԥ��*/
	if(EVENTRECORDBASE + EVENTRECORDLEN*EVENTRECORDMAX <= Event_RecordAddr)	  /*����ַ*/
	{
		Event_RecordAddr = EVENTRECORDBASE;
	}
	if(EVENTRECORDMAX > Event_RecordNum) /*���δ�ﵽ��¼�������ޣ���¼����һ*/
	{
		Event_RecordNum ++;
	}
	FM25_WR_NBYTE(Event_RecordAddr,tmp,12);   /*��¼*/
	Event_RecordAddr += EVENTRECORDLEN;
	tmp[0] = (Event_RecordNum >> 8)&0xff;
	tmp[1] = Event_RecordNum&0xff;
	tmp[2] = (Event_RecordAddr >> 8)&0xff;
	tmp[3] = Event_RecordAddr&0xff;
	tmp[4] = 0xee;
	
	FM25_WR_NBYTE(EVENTRECORDFBASE,tmp,5);   /*���¼�¼��*/
	FM25_WR_NBYTE(EVENTRECORDSBASE,tmp,5);   /*���¼�¼��*/
	FM25_WR_NBYTE(EVENTRECORDTBASE,tmp,5);   /*���¼�¼��*/
}


/******************************************************************************
*�������ƣ�GPRS_Recv
*��������: GPRS���չ���
*�������룺
*�����������
*�������أ���
*��ע��
******************************************************************************/
void GPRS_Recv(void)
{
	u8 tmp[50] = {0}; /*���ݴ�����*/
	u8 i=0,j=0;
	for(i=0;i<3;i++)	   /*���������������ж�*/
	{
		if(0xAA == Flag_RecvCommondBuf[i])
		{
			switch(GPRS_RecvCommondBuf[i][3]) /*�ж�������*/
			{
				case 0xF1:/*ʱ��У׼*/
						  memcpy(tmp,GPRS_RecvCommondBuf[i]+5,6); /*ʱ���ȡ*/
						  Set_Time(tmp);  /*ʱ��д��*/
						  Recv_SerCom = 0xF1;
						  Reply_Servalue = 0x00;
						  Flag_ServerSend[31] = 0xAA;
				          break;
				case 0xF2:/*��λ��״̬����*/
				          Flag_ServerSend[0] = 0xAA; /*��ʱ���ݷ�����λ*/
						  Recv_SerCom = 0xF2;
						  Reply_Servalue = 0x00;
						  Flag_ServerSend[31] = 0xAA;
						  break;
				case 0xF3:/*���������·�*/
				          Get_SetValue(GPRS_RecvCommondBuf[i]+5); /*���������л�ȡ���ò�������д��洢*/
						  Recv_SerCom = 0xF3;
						  Reply_Servalue = 0x00;
						  Flag_ServerSend[1] = 0xAA;  /*���ϴ����ò���*/
						  Flag_ServerSend[31] = 0xAA;
						  break;
				case 0xF4:/*���ò�����ѯ*/
				          Flag_ServerSend[1] = 0xAA; /*���ò����ϴ���λ*/
						  Recv_SerCom = 0xF4;
						  Reply_Servalue = 0x00;
						  Flag_ServerSend[31] = 0xAA;
						  break;
				case 0xF5:/*1�ſ����������������*/
						  if(0xAA == GPRS_RecvCommondBuf[i][5])
						  {
						  	Flag_OUT1OpenNeed = YES;
							Flag_OUT1CloseNeed = NO;
						  }
						  else if(0x55 == GPRS_RecvCommondBuf[i][5])
						  {
						  	Flag_OUT1OpenNeed = NO;
							Flag_OUT1CloseNeed = YES;
						  }
						  Recv_SerCom = 0xF5;
						  Reply_Servalue = 0x00;
						  Flag_ServerSend[31] = 0xAA;
						  break;
				case 0xF6:/*2�ſ����������������*/
						  if(0xAA == GPRS_RecvCommondBuf[i][5])
						  {
						  	Flag_OUT2OpenNeed = YES;
							Flag_OUT2CloseNeed = NO;
						  }
						  else if(0x55 == GPRS_RecvCommondBuf[i][5])
						  {
						  	Flag_OUT2OpenNeed = NO;
							Flag_OUT2CloseNeed = YES;
						  }
						  Recv_SerCom = 0xF6;
						  Reply_Servalue = 0x00;
						  Flag_ServerSend[31] = 0xAA;
						  break;
				case 0xF7:/*�¼����ջظ�*/
						  if(0x03 == GPRS_RecvCommondBuf[i][5])	/*��ǰ�¼����ͻظ�*/
						  {
							  Flag_EventNowRe = 0;    /*���Է�����һ�¼�*/
							  j = 1;
							  FM25_WR_NBYTE(EventNowGPRS_Addr+10,&j,1); /*����ѷ��͵�������*/
						  }
						  else if(0x04 == GPRS_RecvCommondBuf[i][5]) /*�¼���¼���ͻظ�*/
						  {
							  Flag_EventPastRe = 0;   /*���Է�����һ�¼���¼*/
							  j = 1;
							  FM25_WR_NBYTE(EventPastGPRS_Addr+10,&j,1);
						  }
						  break;
				default:break;
			}
			Flag_RecvCommondBuf[i] = 0x00;
		}
	}
	   
}

/******************************************************************************
*�������ƣ�Show_LED
*�������ܣ�ͨ��74HC595��������� 
*�������룺��ʾ����
*�����������
*�������أ���
*��ע��ֻ������ʾ����
******************************************************************************/
void Show_LED(u8 step,u8 lednum)
{
	u16 i =0,j=0;
	if(12 < step || 3 < lednum)
	{
		return;	  /*�������ֱ�ӷ���*/
	}
	switch(lednum)
	{
		case 0:
				DLCDOWN;
				DIG1OFF;
				DIG2OFF;
				DIG3OFF;
				DIG4OFF;
			
				for(i=0;i<8;i++)
				{
					DCKDOWN;
					if(LED_ShowStep[step][0] > 33)
					{
						return;
					}
					if(LED_NUM[LED_ShowStep[step][0]] & (1<<(7-i)))
					{
						DDAUP;
					}
					else
					{
						DDADOWN;
					}
					for(j=0;j<3;j++);
					DCKUP;
					for(j=0;j<3;j++);
				}
				DIG1ON;
				DLCUP;
				for(j=0;j<3;j++);
				DLCDOWN;
				break;
		case 1:	
				for(i=0;i<8;i++)
				{
					DCKDOWN;
					if(LED_ShowStep[step][1] > 33)
					{
						return;
					}
					if(LED_NUM[LED_ShowStep[step][1]] & (1<<(7-i)))
					{
						DDAUP;
					}
					else
					{
						DDADOWN;
					}
					for(j=0;j<3;j++);
					DCKUP;
					for(j=0;j<3;j++);
				}
				DIG2ON;
				DIG1OFF;
				DLCUP;
				for(j=0;j<3;j++);
				DLCDOWN;
				break;
		case 2:
				for(i=0;i<8;i++)
				{
					DCKDOWN;
					if(LED_ShowStep[step][2] > 33)
					{
						return;
					}
					if(LED_NUM[LED_ShowStep[step][2]] & (1<<(7-i)))
					{
						DDAUP;
					}
					else
					{
						DDADOWN;
					}
					for(j=0;j<3;j++);
					DCKUP;
					for(j=0;j<3;j++);
				}
				DIG3ON;
				DIG2OFF;
				DLCUP;
				for(j=0;j<3;j++);
				DLCDOWN;
				break;
		case 3:
				for(i=0;i<8;i++)
				{
					DCKDOWN;
					if(LED_ShowStep[step][3] > 33)
					{
						return;
					}
					if(LED_NUM[LED_ShowStep[step][3]] & (1<<(7-i)))
					{
						DDAUP;
					}
					else
					{
						DDADOWN;
					}
					for(j=0;j<3;j++);
					DCKUP;
					for(j=0;j<3;j++);
				}
				DIG4ON;
				DIG3OFF;
				DLCUP;
				for(j=0;j<3;j++);
				DLCDOWN;
	}
}

/******************************************************************************
*�������ƣ�LED_SelfCheck
*�������ܣ�4��LEDһ����ʾ0-9�����Բ鿴�Ƿ�LED����
*�������룺��ʾ����
*�����������
*�������أ���
*��ע��
******************************************************************************/
void LED_SelfCheck(void)
{
	u16 i =0,j=0;
	static u16 k=0;

   if(k < 34)
   {
		switch(k%4)
		{
		case 0:
		DIG4OFF;
		for(i=0;i<8;i++)
		{
			DCKDOWN;
			if(LED_NUM[k] & (1<<(7-i)))
			{
				DDAUP;
			}
			else
			{
				DDADOWN;
			}
			for(j=0;j<3;j++);
			DCKUP;
			for(j=0;j<3;j++);
		}
		DIG1ON;
		DLCDOWN;
		for(j=0;j<3;j++);
		DLCUP;
		break;
		case 1:
		for(i=0;i<8;i++)
		{
			DCKDOWN;
			if(LED_NUM[k] & (1<<(7-i)))
			{
				DDAUP;
			}
			else
			{
				DDADOWN;
			}
			for(j=0;j<3;j++);
			DCKUP;
			for(j=0;j<3;j++);
		}
		DIG2ON;
		DIG1OFF;
		DLCDOWN;
		for(j=0;j<3;j++);
		DLCUP;

		break;
		case 2:
		for(i=0;i<8;i++)
		{
			DCKDOWN;
			if(LED_NUM[k] & (1<<(7-i)))
			{
				DDAUP;
			}
			else
			{
				DDADOWN;
			}
			for(j=0;j<3;j++);
			DCKUP;
			for(j=0;j<3;j++);
		}
		DIG3ON;
		DIG2OFF;
		DLCDOWN;
		for(j=0;j<3;j++);
		DLCUP;

		break;
		case 3:
		for(i=0;i<8;i++)
		{
			DCKDOWN;
			if(LED_NUM[k] & (1<<(7-i)))
			{
				DDAUP;
			}
			else
			{
				DDADOWN;
			}
			for(j=0;j<3;j++);
			DCKUP;
			for(j=0;j<3;j++);
		}
		DIG4ON;
		DIG3OFF;
		DLCDOWN;
		for(j=0;j<3;j++);
		DLCUP;
		break;
		}
		k++;
		Delay(200);
		RUNLED;
	}
	else
	{
	   k = 0;
	}
}


/******************************************************************************
*�������ƣ�Get_SetValue
*�������ܣ���GPRS�����л�ȡ���ò���
*�������룺������ʼ��ַ
*�����������
*�������أ���
*��ע��
******************************************************************************/
void Get_SetValue(u8* buf)
{
//	u8 tmp[50] = {0};
	u16 i=0;
	
	ADC_AlarmHighRange[0] = (buf[0] <<8)|buf[1];   /*4-20mA��һ· �������޵���ֵ*/
	ADC_AlarmLowRange[0] = (buf[2] <<8)|buf[3];	   /*4-20mA��һ· �������޵���ֵ*/
	ADC_AlarmHighRange[1] = (buf[4] <<8)|buf[5];   /*4-20mA�ڶ�· �������޵���ֵ*/
	ADC_AlarmLowRange[1] = (buf[6] <<8)|buf[7];	   /*4-20mA�ڶ�· �������޵���ֵ*/

	if(((buf[16]>>4)&0xF) == (buf[16] & 0xF))	   /*������Ч*/
	{
		Flag_OUT1Model = buf[16];                      /*1�ſ�����*/
	}
	if(((buf[17]>>4)&0xF) == (buf[17] & 0xF))	   /*������Ч*/
	{
		Flag_OUT2Model = buf[17];                      /*2�ſ�����*/
	}

	Flag_ADCInUse[0] = (0xAA == buf[20])?0xAA:0x55;    /*4-20mA��һ·�Ƿ�����*/
	Flag_ADCInUse[1] = (0xAA == buf[21])?0xAA:0x55;    /*4-20mA�ڶ�·�Ƿ�����*/
	Flag_SWInUse[0] = (0xAA == buf[22])?0xAA:0x55;     /*�������ɼ���һ·�Ƿ�����*/
	Flag_SWInUse[1] = (0xAA == buf[23])?0xAA:0x55;     /*�������ɼ��ڶ�·�Ƿ�����*/

	for(i=0;i<5;i++)
	{
		memcpy(Telphone[i]+2,buf+24+12*i,12);   /*�绰����洢*/
	}

	/*������ȡ���,д��洢��*/
	Update_VIPValue();
}


/*******************************************************************************
*�������ƣ�GetMsg_Analysis
*�������ܣ�����������յ��Ķ���Ϣ
*�������룺
*�������أ�
*���������
*��ע��
*******************************************************************************/
void GetMsg_Analysis(void)
{
	char * tmpaddr = NULL;   /*ָʾ����ַ���ַ*/
	u16 i =0;
	u8 tmp[50] = {0};
//	u32 tmpport = 0;
//	u32 tmpip[4] = {0};
	if(58 == Count_GetMsgStep) /*���յ����ź�1s�ٽ��д���*/
	{
		Count_GetMsgStep --; /*��ֹ����*/
		USART_SendNData(USART2,GPRSCmd8,strlen((char *)GPRSCmd8));	/*���Ž�����text��ʽ*/
	}
	else if(56 == Count_GetMsgStep) //
	{
		Count_GetMsgStep --; /*��ֹ����*/
		USART_SendNData(USART2,GPRSCmd14,strlen((char *)GPRSCmd14)); /*��ȡ���ţ����ն��ţ�ֻ��ȡ��һ��*/
	}
	else if(YES == Flag_GetMsgOver && NO == Flag_GetWrongMsg && 50 < Count_GetMsgStep)	 /*���Ž��������δ����*/
	{
		if(NULL != (tmpaddr = strstr((char*)GetMsg_Buf,"+CMGR:"))) /*���ҽ��ܿ�ͷ*/
		{
			if(NULL != (tmpaddr = strstr((char *)GetMsg_Buf,"\",\"")))	/*���ҵ����뿪ͷ*/
			{
				GetMsg_Num[0] = '\"';
				i=3;
				while(tmpaddr[i] != '\"')
				{
					GetMsg_Num[i-2] = tmpaddr[i];	  /*�����ȡ*/
					i++;	
				}
				GetMsg_Num[i-2] = '\"';
				GetMsg_Num[i-1] = '\r';  /*�����ȡ���*/
			}	
		}
		
		if(NULL != (tmpaddr = strstr((char*)GetMsg_Buf,"\r\n12345678")))  /*���Ź̶���ʽ��ͷ,��¼���룬IP��Port����*/
		{
			
			i=0;
			for(i=0;i<32;i++)
			{
				if((tmpaddr[10+i]< 0x30 || tmpaddr[10+i] > 0x39) && ('.' != tmpaddr[10+i])) /*��ʽ����*/
				{
					break;
				}
			}
			if(i == 32)
			{
				for(i=0;i<12;i++)	   /*����12λ����һλΪ0*/
				{
					tmp[i] = tmpaddr[10+i];   /*���ź����ȡ*/
				}	
				for(i=0;i<6;i++)		/*ascall��ת��*/
				{
					tmp[20+i] = ((tmp[2*i]&0xf)<<4)|(tmp[2*i+1]&0xf);	
				}
				for(i=0;i<6;i++)
				{
					serverpassword[2+i] = tmp[20+i];
				}
				tmp[26] = 0xee;
				for(i=0;i<7;i++)
				{
					tmp[1+i] = tmp[20+i];
				}
				if(((tmp[20] >>4)&0xF) != 0x00) /*13λ����*/
				{
					tmp[0] = 0x01;
				}
				else
				{
					tmp[0] = 0x00;
				}
				FM25_WR_NBYTE(SPECIALVALUE1ADDBASE,tmp,8);   /*��¼�������*/
				FM25_WR_NBYTE(SPECIALVALUE2ADDBASE,tmp,8);   /*��¼�������*/
				FM25_WR_NBYTE(SPECIALVALUE3ADDBASE,tmp,8);   /*��¼�������*/
//				for(i=0;i<15;i++)
//				{
//					Server_IP[1+i] = tmpaddr[22+i];  /*IP��ַ��ȡ*/
//				}	
//				for(i=0;i<5;i++)
//				{
//					Server_PORT[1+i] = tmpaddr[37+i]; /*PORT��ȡ*/
//				}
//				memset(tmp,0,10); /*���*/
//				for(i=0;i<4;i++)
//				{
//					tmpip[i] = (Server_IP[4*i+1]-'0')*100;
//					tmpip[i] += (Server_IP[4*i+2]-'0')*10;
//					tmpip[i] += (Server_IP[4*i+3]-'0');
//				}
//				tmpport = (Server_PORT[1]-'0') * 10000;
//				tmpport += (Server_PORT[2]-'0') * 1000;
//				tmpport += (Server_PORT[3]-'0') * 100;
//				tmpport += (Server_PORT[4]-'0') * 10;
//				tmpport += (PORT[5]-'0');
//				if((tmpip[0]>255)||(tmpip[1]>255)||(tmpip[2]>255)||(tmpip[3]>255)||(tmpport>65535))
//				{
//					Count_GetMsgStep = 28; /*�������Żָ�*/
//				}
//				else
//				{
//					for(i=0;i<4;i++)
//					{
//						tmp[i] = tmpip[i]&0xff;
//					}
//					tmp[4] = (tmpport >>8)	& 0xff;
//					tmp[5] = tmpport & 0xff;
//					tmp[6] = 0xee; /*��Ч��ʶ*/
//					#if 64 == FMMEMORY
//					FM25_WR_NBYTE(64020,tmp,7);
//					#else 
//					FM25_WR_NBYTE(260020,tmp,7);
//					#endif
//					Count_GetMsgStep = 50; /*�������Żظ�*/
//				}	
			}
			else  /*��ʽ����*/
			{
				Count_GetMsgStep = 28; /*�������Żָ�*/	
			}

		}
		else if(NULL != (tmpaddr = strstr((char*)GetMsg_Buf,"\r\n87654321012345678")))  /*���Ź̶���ʽ��ͷ�������*/
		{
			NVIC_GenerateSystemReset();  /*����*/	
		}
		else
		{
			Count_GetMsgStep = 28; /*�������Żָ�*/	
		}	
	}
	else if(48 == Count_GetMsgStep && YES == Flag_GetMsgOver && NO == Flag_GetWrongMsg )	 /*���Żظ�*/
	{
		Count_GetMsgStep --;
		USART_SendNData(USART2,GPRSCmd4,strlen((char *)GPRSCmd4));	/*���ŷ���*/	
	}
	else if(46 == Count_GetMsgStep && YES == Flag_GetMsgOver && NO == Flag_GetWrongMsg )	 /*���Żظ�*/
	{
		Count_GetMsgStep --;
		USART_SendNData(USART2,GetMsg_Num,strlen((char *)GetMsg_Num));	/*���ŷ��ͺ���*/	
	}
	else if(44 == Count_GetMsgStep && YES == Flag_GetMsgOver && NO == Flag_GetWrongMsg )	 /*���Żظ�*/
	{
		Count_GetMsgStep --;
		USART_SendNData(USART2,"set OK\r\n",8);	/*���ŷ�������*/	
	}
	else if(42 == Count_GetMsgStep && YES == Flag_GetMsgOver && NO == Flag_GetWrongMsg )	 /*���Żظ�*/
	{
		Count_GetMsgStep --;
		USART_SendOneData(USART2,0x1a);	/*���ŷ������� Ctrl+Z*/	
	}
	else if(25 == Count_GetMsgStep) /*������ɣ��ָ�*/
	{
		Count_GetMsgStep --;
		USART_SendNData(USART2,GPRSCmd5,strlen((char *)GPRSCmd5));	/*ɾ�����ж���*/	
	}
	else if(22 == Count_GetMsgStep) /*������ɣ��ָ�*/
	{
		Count_GetMsgStep --;
		USART_SendNData(USART2,GPRSCmd3,strlen((char *)GPRSCmd3));	/*�ָ�PDU��ʽ*/
	}
	else if(20 > Count_GetMsgStep)
	{
		Flag_GetMsgOver = NO;
		Flag_GetMsg = NO;
		Flag_GetWrongMsg = NO;
		Count_GetMsgBuf = 0;
		Flag_GprsGetClose = YES; /*���Ϊ���յ�Close*/
		Count_TimGPRSRecv = COUNT_TIMGPRSRECV*3;
	}

}

/*******************************************************************************
*�������ƣ�Send_Msg
*�������ܣ�����PDU����
*�������ã�
*�������룺�������ݡ�����
*�����������
*�������أ���
*��ע��
*******************************************************************************/
void Send_Msg(void)
{
	u16 i=0;
	if(YES == Flag_RecvingGPRS)	/*������ڷ���GPRS��Ϣ����ʱ����MSG*/
	{
		return;
	}
	else
	{
		Flag_SendingMSG = YES;
	}

	Flag_SendMsgNum ++;

	if(0 == Flag_SendMsgNum)
	{
//		Flag_SendMsgNum = 1;
		USART_SendNData(USART2,GPRSCmd8,strlen((char*)GPRSCmd8));		   /*ɾ������*/
	}
	else if(100 == Flag_SendMsgNum)
	{
		USART_SendNData(USART2,GPRSCmd5,strlen((char*)GPRSCmd5));        /*ɾ�����ţ���֤�ܹ����ͳɹ�*/
	}
	else if(200 == Flag_SendMsgNum)
	{
		USART_SendNData(USART2,GPRSCmd3,strlen((char*)GPRSCmd3));		   /*PDU��ʽ*/
	}
	else if(300 +300*TelNum > Flag_SendMsgNum  && Flag_SendMsgNum > 300)
	{
		if(1 == ((Flag_SendMsgNum-300)%300))
		{
			Msg_Create(Flag_SendMsgNum);										/*������������*/
			if(0 == MsgLen)	/*��������ʧ��*/
			{
				Flag_SendMsgNum = 300 +300*TelNum;
				return;	
			}								   
			USART_SendNData(USART2,GPRSCmd4,strlen((char*)GPRSCmd4));			/*����AT���ŷ�������*/
			USART_SendOneData(USART2,(u8)(((MsgLen)%1000)/100 + 0x30));             /*���ų��Ȱ�λ*/
			USART_SendOneData(USART2,(u8)(((MsgLen)%100)/10 + 0x30));             /*���ų���ʮλ*/
			USART_SendOneData(USART2,(u8)((MsgLen)%10 + 0x30));             /*���ų��ȸ�λ*/
			USART_SendOneData(USART2,'\r');                               /*�س�*/
		}
		else if(140 == ((Flag_SendMsgNum-300)%300))
		{
			USART_SendNData(USART2,GPRSPDU,(MsgLen+1)*2);//strlen((char*)GPRSPDU));    /*���Ͷ�������*/
			for(i=0;i<2000;i++);
			USART_SendOneData(USART2,0x1a);                            /*ctrl+z*/
		}
		else       /*�ȴ�һ��ѭ������ʱ*/
		{
		
		}
	}	
}

/*******************************************************************************
*�������ƣ�Msg_Create
*�������ܣ�������������
*�������ã�
*�������룺���ŷ��Ͳ������
*����������������ݡ����ų���
*�������أ���
*��ע��
*******************************************************************************/
void Msg_Create(u16 Flag)
{
	u8 i =0 ,j = 0;
	u8 tmp_phone[14] = {0};  /*���뻺��*/ 
	u16 tmplen = 0;
	u8 tmpneedvalue = 0;
	for(i=0;i<5;i++)
	{
		if(0xee == Telphone[i][13])
		{
			j ++;
			if((j-1) == (Flag-300)/300)
			{
				break;
			}
		}
	}
	if(5 == i)
	{
		return;   /*�������*/
	}
	if(0x30 == Telphone[i][0] && 0x30 == Telphone[i][1]) /*11λ����*/
	{
		for(j=0;j<6;j++)							/*����ǰ��ߵ�*/
		{
			tmp_phone[2*j] = Telphone[i][2*j+3];
			tmp_phone[2*j+1]=Telphone[i][2*j+2];
		}
		tmp_phone[10] = 'F';/*���һλΪF����*/
		sprintf((char *)(GPRSPDU+12),"%12s",tmp_phone);		  /*�ϳɵ绰����*/
		sprintf((char*)(GPRSPDU+24),"000800");				  /*�̶���ʽ*/
		tmplen = 0;
		sprintf((char*)(GPRSPDU+32+tmplen*2),"%s",PUMPHOUSENAME);  /*�÷�����*/
		tmplen += strlen((char*)PUMPHOUSENAME)/2;
	}
	else
	{
		for(j=0;j<7;j++)							/*����ǰ��ߵ�*/
		{
			tmp_phone[2*j] = Telphone[i][2*j+1];
			tmp_phone[2*j+1]=Telphone[i][2*j];
		}
		tmp_phone[12] = 'F';/*���һλΪF����*/
		sprintf((char *)(GPRSPDU+12),"%14s",tmp_phone);		  /*�ϳɵ绰����*/
		sprintf((char*)(GPRSPDU+26),"000800");				  /*�̶���ʽ*/
		tmplen = 0;
		sprintf((char*)(GPRSPDU+34+tmplen*2),"%s",PUMPHOUSENAME);  /*�÷�����*/
		tmplen += strlen((char*)PUMPHOUSENAME)/2;
		tmplen +=1;   /*13λ���볤���������ֽ�*/
	}
	
	for(i=0;i<16;i++)
	{
		if(((Flag_SendMsg&(0x1<<i)) && (!(Flag_AlreadyMsg & (0x1<<i))))&&(tmplen<240))
		{
			sprintf((char*)(GPRSPDU+32+tmplen*2),"%s",PDUCmd[i]);
			tmplen += strlen((char*)PDUCmd[i])/2;
			tmpneedvalue ++;	
		}
		else if(tmplen>=240) /*������󳤶�*/
		{
			tmplen = 240;
			break;
		}
	}
	if(0 != tmpneedvalue) /*�б����¼�*/
	{
		GPRSPDU[30] = 0x30 + (tmplen/16);
		if((tmplen%16)>9)
		{
			GPRSPDU[31] = 'A' + ((tmplen%16)-10);
		}
		else
		{
			GPRSPDU[31] = 0x30 + (tmplen%16);
		}
		MsgLen = 15 + tmplen;
	}
	else		    /*��ֹ���Ͷ���ʱ���¼��ָ����Ϳն���*/
	{
		MsgLen = 0;
	}
}

/*******************************************************************************
*�������ƣ�Read_VIPValue
*�������ܣ��������ȡ��Ҫ����
*�������ã�
*�������룺
*���������
*�������أ���
*��ע��
*******************************************************************************/
void Read_VIPValue(void)
{
	u8 tmp[3][50] = {0};   /*������ȡ����*/
	u16 i=0,j=0;
	u16 tmpvalue = 0;
	float tmpcalib[4]  ={0.0};

	FM25_RE_NBYTE(SPECIALVALUE1ADDBASE,tmp[0],25);   /*��ȡIP,PORT,ID��*/
	FM25_RE_NBYTE(SPECIALVALUE2ADDBASE,tmp[1],25); /*�ڶ���*/
	FM25_RE_NBYTE(SPECIALVALUE3ADDBASE,tmp[2],25); /*������*/
	if(0 == memcmp(tmp[0],tmp[1],25) && 0 == memcmp(tmp[0],tmp[2],25))  /*����ȫ����ͬ*/
	{
		i=0;
	}
	else if(0 == memcmp(tmp[0],tmp[1],25)) /*�����ݲ�ͬ*/
	{
		i=3;
	}
	else if(0 == memcmp(tmp[0],tmp[2],25))	/*�ڶ��ݲ�ͬ*/
	{
		i=2;
	}
	else if(0 == memcmp(tmp[1],tmp[2],25)) /*��һ�ݲ�ͬ*/
	{
		i=1;
	}
	else
	{
		i = 0xff;
	}
	RUNLED;
	switch(i)
	{
		case 0:break;
		case 1:FM25_WR_NBYTE(SPECIALVALUE1ADDBASE,tmp[1],25);memcpy(tmp[0],tmp[1],25);break;
		case 2:FM25_WR_NBYTE(SPECIALVALUE2ADDBASE,tmp[0],25);break;
		case 3:FM25_WR_NBYTE(SPECIALVALUE3ADDBASE,tmp[0],25);break;
		case 0xff:return;
		default:break;
	}
	if(0xEE == tmp[0][7])	/*������Ч*/
	{
//		memcpy(serverpassword+2,tmp[0]+1,6);   /*��¼ID��ֵ*/
	}
	if(0x11 == tmp[0][10] || 0x22 == tmp[0][10])
	{
		Flag_ServerMod = (0x11 == tmp[0][10])?0x00:0xAA;	/*��¼��ʽ*/
		Server_PORT = (tmp[0][11]<<8)&0xFF00;
		Server_PORT |= (tmp[0][12] & 0xFF);					/*��¼�˿�*/
		if(0x11 == tmp[0][10])
		{
			for(i=0;i<4;i++)
			{
				Server_IP[i] = tmp[0][13+i];
			}
		}
		else
		{
			memcpy(Server_WWW,tmp[0]+13,12);              /*������������������*/
		}
	}
	memset(tmp[0],0,25); /*���*/
	memset(tmp[1],0,25);
	memset(tmp[2],0,25);
	FM25_RE_NBYTE(SPECIALVALUE1ADDBASE+25,tmp[0],40);   /*��ȡ���������*/
	FM25_RE_NBYTE(SPECIALVALUE2ADDBASE+25,tmp[1],40); /*�ڶ���*/
	FM25_RE_NBYTE(SPECIALVALUE3ADDBASE+25,tmp[2],40); /*������*/
	if(0 == memcmp(tmp[0],tmp[1],40) && 0 == memcmp(tmp[0],tmp[2],40))  /*����ȫ����ͬ*/
	{
		i=0;
	}
	else if(0 == memcmp(tmp[0],tmp[1],40)) /*�����ݲ�ͬ*/
	{
		i=3;
	}
	else if(0 == memcmp(tmp[0],tmp[2],40))	/*�ڶ��ݲ�ͬ*/
	{
		i=2;
	}
	else if(0 == memcmp(tmp[1],tmp[2],40)) /*��һ�ݲ�ͬ*/
	{
		i=1;
	}
	else
	{
		i = 0xff;
	}
	RUNLED;
	switch(i)
	{
		case 0:break;
		case 1:FM25_WR_NBYTE(SPECIALVALUE1ADDBASE+25,tmp[1],40);memcpy(tmp[0],tmp[1],40);break;
		case 2:FM25_WR_NBYTE(SPECIALVALUE2ADDBASE+25,tmp[0],40);break;
		case 3:FM25_WR_NBYTE(SPECIALVALUE3ADDBASE+25,tmp[0],40);break;
		case 0xff:return;
		default:break;
	}

	for(i=0;i<5;i++)				  /*���洢5������*/
	{
		if(0xEE == tmp[0][8*i+7])	  /*�ú����Ƿ���Ч*/
		{
			for(j=0;j<7;i++)
			{
				tmpvalue = (tmp[0][8*i+j]>>4) & 0xF;
				Telphone[i][2*j] = 0x30 + tmpvalue;
				tmpvalue = tmp[0][8*i+j] & 0xF;
				Telphone[i][2*j+1] = 0x30 + tmpvalue;
			}
			memcpy(Telphone[i],Telphone[i]+1,13);
			Telphone[i][13] = 0xEE;
		}
		else
		{
			Telphone[i][13] = 0x00;
		}
	}

	memset(tmp[0],0,40); /*���*/
	memset(tmp[1],0,40);
	memset(tmp[2],0,40);
	FM25_RE_NBYTE(SPECIALVALUE1ADDBASE+65,tmp[0],25);   /*��ȡģ�������������Ƿ����� ģ�������������ޣ��Զ����Ʋ������¼���¼��*/
	FM25_RE_NBYTE(SPECIALVALUE2ADDBASE+65,tmp[1],25); /*�ڶ���*/
	FM25_RE_NBYTE(SPECIALVALUE3ADDBASE+65,tmp[2],25); /*������*/
	if(0 == memcmp(tmp[0],tmp[1],25) && 0 == memcmp(tmp[0],tmp[2],25))  /*����ȫ����ͬ*/
	{
		i=0;
	}
	else if(0 == memcmp(tmp[0],tmp[1],25)) /*�����ݲ�ͬ*/
	{
		i=3;
	}
	else if(0 == memcmp(tmp[0],tmp[2],25))	/*�ڶ��ݲ�ͬ*/
	{
		i=2;
	}
	else if(0 == memcmp(tmp[1],tmp[2],25)) /*��һ�ݲ�ͬ*/
	{
		i=1;
	}
	else
	{
		i = 0xff;
	}
	RUNLED;
	switch(i)
	{
		case 0:break;
		case 1:FM25_WR_NBYTE(SPECIALVALUE1ADDBASE+65,tmp[1],25);memcpy(tmp[0],tmp[1],25);break;
		case 2:FM25_WR_NBYTE(SPECIALVALUE2ADDBASE+65,tmp[0],25);break;
		case 3:FM25_WR_NBYTE(SPECIALVALUE3ADDBASE+65,tmp[0],25);break;
		case 0xff:return;
		default:break;
	}

	Flag_ADCInUse[0] = (0xEE == tmp[0][0])?0xAA:0x55;
	Flag_ADCInUse[1] = (0xEE == tmp[0][1])?0xAA:0x55;
    Flag_SWInUse[0]	 = (0xEE == tmp[0][2])?0xAA:0x55;
	Flag_SWInUse[1]	 = (0xEE == tmp[0][3])?0xAA:0x55;

	if(0xee == tmp[0][13])	 /*���ñ���*/
	{
		for(i=0;i<2;i++)
		{
			ADC_AlarmHighRange[i] = ((tmp[0][5+4*i]<<8)&0xFF00)|(tmp[0][6+4*i] & 0xFF);
			ADC_AlarmLowRange[i] = ((tmp[0][7+4*i]<<8)&0xFF00)|(tmp[0][8+4*i] & 0xFF);
		}
	}
	if(0xee == tmp[0][17])	/*������Ч*/
	{
		Flag_OUT1Model = tmp[0][15];
		Flag_OUT2Model = tmp[0][16];	
	}

	if(0xee == tmp[0][24]) /*������Ч*/
	{
		Event_RecordNum = ((tmp[0][20]<<8) & 0xFF00)|(tmp[0][21] & 0xFF);
		Event_RecordAddr = ((tmp[0][22]<<8) & 0xFF00)|(tmp[0][23] & 0xFF);
	}

	memset(tmp[0],0,25); /*���*/
	memset(tmp[1],0,25);
	memset(tmp[2],0,25);
	FM25_RE_NBYTE(SPECIALVALUE1ADDBASE+90,tmp[0],25);   /*��ȡADC����У׼��¼*/
	FM25_RE_NBYTE(SPECIALVALUE2ADDBASE+90,tmp[1],25); /*�ڶ���*/
	FM25_RE_NBYTE(SPECIALVALUE3ADDBASE+90,tmp[2],25); /*������*/
	if(0 == memcmp(tmp[0],tmp[1],25) && 0 == memcmp(tmp[0],tmp[2],25))  /*����ȫ����ͬ*/
	{
		i=0;
	}
	else if(0 == memcmp(tmp[0],tmp[1],25)) /*�����ݲ�ͬ*/
	{
		i=3;
	}
	else if(0 == memcmp(tmp[0],tmp[2],25))	/*�ڶ��ݲ�ͬ*/
	{
		i=2;
	}
	else if(0 == memcmp(tmp[1],tmp[2],25)) /*��һ�ݲ�ͬ*/
	{
		i=1;
	}
	else
	{
		i = 0xff;
	}
	RUNLED;
	switch(i)
	{
		case 0:break;
		case 1:FM25_WR_NBYTE(SPECIALVALUE1ADDBASE+90,tmp[1],25);memcpy(tmp[0],tmp[1],25);break;
		case 2:FM25_WR_NBYTE(SPECIALVALUE2ADDBASE+90,tmp[0],25);break;
		case 3:FM25_WR_NBYTE(SPECIALVALUE3ADDBASE+90,tmp[0],25);break;
		case 0xff:return;
		default:break;
	}

	if(0xEE == tmp[0][24])  /*������Ч*/
	{
		for(i=0;i<3;i++)
		{
			tmpcalib[0] = (float)(((tmp[0][0+8*i]<<8) & 0xFF00)|(tmp[0][1+8*i] & 0xFF));
			tmpcalib[1] = (float)(((tmp[0][2+8*i]<<8) & 0xFF00)|(tmp[0][3+8*i] & 0xFF));
			tmpcalib[2] = (float)(((tmp[0][4+8*i]<<8) & 0xFF00)|(tmp[0][5+8*i] & 0xFF));
			tmpcalib[3] = (float)(((tmp[0][6+8*i]<<8) & 0xFF00)|(tmp[0][7+8*i] & 0xFF));
			K1[i] = (tmpcalib[1] - tmpcalib[3])/(tmpcalib[0] - tmpcalib[2]);
			D1[i] = tmpcalib[1] - K1[i]*(tmpcalib[0] - tmpcalib[2]);	
		}
		Flag_ADCSetAble = YES;
	}
}

/*******************************************************************************
*�������ƣ�Update_VIPValue
*�������ܣ������������Ҫ����
*�������ã�
*�������룺
*���������
*�������أ���
*��ע��������ADCУ׼��ؼ�¼
*******************************************************************************/
void Update_VIPValue(void)
{
	u8 tmp[50] = {0};   /*����д�뻺��*/
	u16 i=0,j=0;
	
	/*��¼ID��IP,Port����Ϣ*/
	memcpy(tmp+1,serverpassword+2,6); /*ֻ����12λ���֣����Ϊ13λ���룬��һλĬ��Ϊ1*/
	tmp[7] = 0xEE;
	tmp[10] = (0x00 == Flag_ServerMod)?0x11:0x22; /*��¼��ʽ*/
	tmp[11] = (Server_PORT >> 8) & 0xFF;
	tmp[12] = Server_PORT & 0xFF;
	if(0x00 == Flag_ServerMod)
	{
		memcpy(tmp+13,Server_IP,4);  /*��ַIP*/
	}
	else
	{
		memcpy(tmp+13,Server_WWW,12);              /*������������������*/
	}
	FM25_WR_NBYTE(SPECIALVALUE1ADDBASE,tmp,25);	 /*д������*/
	FM25_WR_NBYTE(SPECIALVALUE2ADDBASE,tmp,25);
	FM25_WR_NBYTE(SPECIALVALUE3ADDBASE,tmp,25);
	
	/*��������洢*/
	memset(tmp,0,25);
	for(i=0;i<5;i++)				  /*���洢5������*/
	{
		if(0xEE == Telphone[i][13])	  /*������Ч*/
		{
			tmp[8*i+0] = Telphone[i][0] - 0x30;
			for(j=1;j<7;i++)
			{
				tmp[8*i+j] = ((Telphone[i][2*j-1]-0x30)<<4)&0xF;
				tmp[8*i+j] |= (Telphone[i][2*j] & 0xF);
			}
			tmp[8*i+7] = 0xEE;

		}
		else
		{
			memset(tmp+8*i,0,8);
		}
	}
	FM25_WR_NBYTE(SPECIALVALUE1ADDBASE+25,tmp,40);	 /*д������*/
	FM25_WR_NBYTE(SPECIALVALUE2ADDBASE+25,tmp,40);
	FM25_WR_NBYTE(SPECIALVALUE3ADDBASE+25,tmp,40);

	/*���������ޡ��������*/
	memset(tmp,0,40);

	tmp[0] = (0xAA == Flag_ADCInUse[0])?0xEE:0x00;
	tmp[1] = (0xAA == Flag_ADCInUse[1])?0xEE:0x00;
	tmp[2] = (0xAA == Flag_SWInUse[0])?0xEE:0x00;
	tmp[3] = (0xAA == Flag_SWInUse[1])?0xEE:0x00;

	for(i=0;i<2;i++)
	{
		tmp[5+4*i] = (ADC_AlarmHighRange[i]>>8) & 0xFF;
		tmp[5+4*i+1] = ADC_AlarmHighRange[i] & 0xFF;
		tmp[5+4*i+2] = (ADC_AlarmLowRange[i]>>8) & 0xFF;
		tmp[5+4*i+3] = ADC_AlarmLowRange[i] & 0xFF;
	}
	tmp[13] = 0xEE;

	tmp[15] = Flag_OUT1Model;
	tmp[16] = Flag_OUT2Model;
	tmp[17] = 0xEE;

	tmp[20] = (Event_RecordNum >> 8) & 0xFF;
	tmp[21] = Event_RecordNum & 0xFF;
	tmp[22] = (Event_RecordAddr>>8)&0xFF;
	tmp[23] = Event_RecordNum & 0xFF;
	tmp[24] = 0xEE;

	FM25_WR_NBYTE(SPECIALVALUE1ADDBASE+65,tmp,25);	 /*д������*/
	FM25_WR_NBYTE(SPECIALVALUE2ADDBASE+65,tmp,25);
	FM25_WR_NBYTE(SPECIALVALUE3ADDBASE+65,tmp,25);		
}

/*******************************************************************************
*�������ƣ�Show_Update
*�������ܣ��������ʾ���ݸ���
*�������ã�
*�������룺
*���������
*�������أ���
*��ע��
*******************************************************************************/
void Show_Update(void)
{
	LED_ShowStep[0][0] = 10;     /*'A'*/
	LED_ShowStep[0][1] = (Analog_20mA[0]%10000)/1000;  /*����ֵʮλ*/
	LED_ShowStep[0][1] = (0 == LED_ShowStep[0][1])?16:LED_ShowStep[0][1];  /*ʮλΪ0 ��*/
	LED_ShowStep[0][2] = ((Analog_20mA[0]%1000)/100)+17; /*����ֵ��λ������*/
	LED_ShowStep[0][3] = (Analog_20mA[0]%100)/10;      /*����С�����һλ*/

	LED_ShowStep[1][0] = 10+17;     /* 'A.' */
	LED_ShowStep[1][1] = (Analog_20mA[1]%10000)/1000;  /*����ֵʮλ*/
	LED_ShowStep[1][1] = (0 == LED_ShowStep[1][1])?16:LED_ShowStep[1][1];  /*ʮλΪ0 ��*/
	LED_ShowStep[1][2] = ((Analog_20mA[1]%1000)/100)+17; /*����ֵ��λ������*/
	LED_ShowStep[1][3] = (Analog_20mA[1]%100)/10;      /*����С�����һλ*/

	LED_ShowStep[2][0] = 12;     /*'C'*/
	if(0xAA == Switch_Station[0])/*ͨ*/
	{
		LED_ShowStep[2][1] = 16;  /*��*/
		LED_ShowStep[2][2] = 0;   /*0*/
		LED_ShowStep[2][3] = 14;  /*E*/	
	}
	else
	{
		LED_ShowStep[2][1] = 0;  /*0*/
		LED_ShowStep[2][2] = 15;   /*F*/
		LED_ShowStep[2][3] = 15;  /*F*/
	}
	
	LED_ShowStep[3][0] = 12+17;     /* 'C.' */
	if(0xAA == Switch_Station[1])/*ͨ*/
	{
		LED_ShowStep[3][1] = 16;  /*��*/
		LED_ShowStep[3][2] = 0;   /*0*/
		LED_ShowStep[3][3] = 14;  /*E*/	
	}
	else
	{
		LED_ShowStep[3][1] = 0;  /*0*/
		LED_ShowStep[3][2] = 15;   /*F*/
		LED_ShowStep[3][3] = 15;  /*F*/
	}

	LED_ShowStep[4][0] = 0;     /*'0'*/
	if(0xAA == Output_Station[0])/*ͨ*/
	{
		LED_ShowStep[4][1] = 16;  /*��*/
		LED_ShowStep[4][2] = 0;   /*0*/
		LED_ShowStep[4][3] = 14;  /*E*/	
	}
	else
	{
		LED_ShowStep[4][1] = 0;  /*0*/
		LED_ShowStep[4][2] = 15;   /*F*/
		LED_ShowStep[4][3] = 15;  /*F*/
	}

	LED_ShowStep[5][0] = 0+17;     /* '0.' */
	if(0xAA == Output_Station[1])/*ͨ*/
	{
		LED_ShowStep[5][1] = 16;  /*��*/
		LED_ShowStep[5][2] = 0;   /*0*/
		LED_ShowStep[5][3] = 14;  /*E*/	
	}
	else
	{
		LED_ShowStep[5][1] = 0;  /*0*/
		LED_ShowStep[5][2] = 15;   /*F*/
		LED_ShowStep[5][3] = 15;  /*F*/
	}

	LED_ShowStep[6][0] = 15;	 /*'F'*/
	LED_ShowStep[6][1] = (Signal_Quality/100)%1000; /*�ź�������λ*/
	LED_ShowStep[6][1] = (0 == LED_ShowStep[6][1])?16:LED_ShowStep[6][1];  /*ʮλΪ0 ��*/
	LED_ShowStep[6][2] = (Signal_Quality/10)%100; /*�ź�����ʮλ*/
	LED_ShowStep[6][3] = Signal_Quality%10; /*�ź�������λ*/

	LED_ShowStep[7][0] = 14;	 /*'E'*/
	LED_ShowStep[7][1] = 1;	     /*1*/
	LED_ShowStep[7][2] = 0;	     /*0*/
	LED_ShowStep[7][3] = Flag_ErrorShow[0]%10;	     /*������*/

	LED_ShowStep[8][0] = 14;	 /*'E'*/
	LED_ShowStep[8][1] = 2;	     /*2*/
	LED_ShowStep[8][2] = 0;	     /*0*/
	LED_ShowStep[8][3] = Flag_ErrorShow[1]%10;	     /*������*/

	LED_ShowStep[9][0] = 14;	 /*'E'*/
	LED_ShowStep[9][1] = 3;	     /*3*/
	LED_ShowStep[9][2] = 0;	     /*0*/
	LED_ShowStep[9][3] = Flag_ErrorShow[2]%10;	     /*������*/

	LED_ShowStep[10][0] = 14;	 /*'E'*/
	LED_ShowStep[10][1] = 4;	     /*4*/
	LED_ShowStep[10][2] = 0;	     /*0*/
	LED_ShowStep[10][3] = Flag_ErrorShow[3]%10;	     /*������*/

	/*��Դ��ѹ*/
	LED_ShowStep[11][0] = 13;	 /*��d��*/
	LED_ShowStep[11][1] = Power_Voltage/10000;	 /*��ѹʮλ*/
	LED_ShowStep[11][1] = (0 == LED_ShowStep[11][1])?16:LED_ShowStep[11][1]; /*ʮλΪ0����ʾΪ��*/
	LED_ShowStep[11][2] = ((Power_Voltage%10000)/1000)+17;	 /*��ѹ��λ,��С����*/
	LED_ShowStep[11][3] = (Power_Voltage%1000)/100;	 /*��ѹС�����һλ*/
}

/*******************************************************************************
*�������ƣ�PCSet_Handler
*�������ܣ��������ʾ���ݸ���
*�������ã�
*�������룺
*���������
*�������أ���
*��ע��
*******************************************************************************/
void PCSet_Handler(void)
{
	u8 tmpre[20] = {0};
	u8 tmpvalue[10] = {0};
	u16 tmpcrc = 0;
	u16 i = 0;
	if(YES == Flag_GetPCSet)	/*���յ�����������*/
	{
		switch(PCSet_ValueBuf[3])
		{
			case 0x01:							/*��ʼPC�������ã���ADCУ��*/
					  Flag_PCSETING = YES;
					  ADC_Configuration();
					  PSW1ON;
					  PSW2ON;
					  tmpre[0] = 0xF5;
					  tmpre[1] = 0xFA;
					  tmpre[2] = 10;         /*֡��*/
					  tmpre[3] = 0x81;       /*�ظ���Ӧ����*/
					  memcpy(tmpre+4,PCSet_ValueBuf+4,9);
					  tmpcrc = crc16_ccitt(tmpre,13);
					  tmpre[13] = (tmpcrc >>8) & 0xFF;
					  tmpre[14] = tmpcrc & 0xFF;
					  USART_SendNData(USART1,tmpre,15);
					  break;
			case 0x02:						/*ADCУ׼����*/
					  if(0x1 == PCSet_ValueBuf[5]) /*��Դ��ѹУ׼*/
					  {
					  	 tmpvalue[2] = PCSet_ValueBuf[7];
						 tmpvalue[3] = PCSet_ValueBuf[8];
//						 ADC_ValueAvg(ADCBUF);
						 tmpvalue[0] = (ADC_Value[1]>>8) & 0xFF;
						 tmpvalue[1] = ADC_Value[1] & 0xFF;
						 if(0x11 == PCSet_ValueBuf[6])
						 {
						 	FM25_WR_NBYTE(SPECIALVALUE1ADDBASE+90,tmpvalue,4);
							FM25_WR_NBYTE(SPECIALVALUE2ADDBASE+90,tmpvalue,4);
							FM25_WR_NBYTE(SPECIALVALUE3ADDBASE+90,tmpvalue,4);
						 }
						 else if(0x22 == PCSet_ValueBuf[6])		/*�ڶ���*/
						 {
						 	FM25_WR_NBYTE(SPECIALVALUE1ADDBASE+94,tmpvalue,4);
							FM25_WR_NBYTE(SPECIALVALUE2ADDBASE+94,tmpvalue,4);
							FM25_WR_NBYTE(SPECIALVALUE3ADDBASE+94,tmpvalue,4);
							tmpvalue[4] = 0xee;
							FM25_WR_NBYTE(SPECIALVALUE1ADDBASE+114,tmpvalue+4,1);
							FM25_WR_NBYTE(SPECIALVALUE2ADDBASE+114,tmpvalue+4,1);
							FM25_WR_NBYTE(SPECIALVALUE3ADDBASE+114,tmpvalue+4,1);
						 }
					  }
					  else if(0x02 == PCSet_ValueBuf[5])   /*4-20MA��һ·У׼*/
					  {
					  	 tmpvalue[2] = PCSet_ValueBuf[7];
						 tmpvalue[3] = PCSet_ValueBuf[8];
//						 ADC_ValueAvg(ADCBUF);
						 tmpvalue[0] = (ADC_Value[2]>>8) & 0xFF;
						 tmpvalue[1] = ADC_Value[2] & 0xFF;
						 if(0x11 == PCSet_ValueBuf[6])
						 {
						 	FM25_WR_NBYTE(SPECIALVALUE1ADDBASE+98,tmpvalue,4);
							FM25_WR_NBYTE(SPECIALVALUE2ADDBASE+98,tmpvalue,4);
							FM25_WR_NBYTE(SPECIALVALUE3ADDBASE+98,tmpvalue,4);
						 }
						 else if(0x22 == PCSet_ValueBuf[6])		/*�ڶ���*/
						 {
						 	FM25_WR_NBYTE(SPECIALVALUE1ADDBASE+102,tmpvalue,4);
							FM25_WR_NBYTE(SPECIALVALUE2ADDBASE+102,tmpvalue,4);
							FM25_WR_NBYTE(SPECIALVALUE3ADDBASE+102,tmpvalue,4);
							tmpvalue[4] = 0xee;
							FM25_WR_NBYTE(SPECIALVALUE1ADDBASE+114,tmpvalue+4,1);
							FM25_WR_NBYTE(SPECIALVALUE2ADDBASE+114,tmpvalue+4,1);
							FM25_WR_NBYTE(SPECIALVALUE3ADDBASE+114,tmpvalue+4,1);
						 }
					  }
					  else if(0x02 == PCSet_ValueBuf[5])   /*4-20MA����·У׼*/
					  {
					  	 tmpvalue[2] = PCSet_ValueBuf[7];
						 tmpvalue[3] = PCSet_ValueBuf[8];
//						 ADC_ValueAvg(ADCBUF);
						 tmpvalue[0] = (ADC_Value[3]>>8) & 0xFF;
						 tmpvalue[1] = ADC_Value[3] & 0xFF;
						 if(0x11 == PCSet_ValueBuf[6])
						 {
						 	FM25_WR_NBYTE(SPECIALVALUE1ADDBASE+106,tmpvalue,4);
							FM25_WR_NBYTE(SPECIALVALUE2ADDBASE+106,tmpvalue,4);
							FM25_WR_NBYTE(SPECIALVALUE3ADDBASE+106,tmpvalue,4);
						 }
						 else if(0x22 == PCSet_ValueBuf[6])		/*�ڶ���*/
						 {
						 	FM25_WR_NBYTE(SPECIALVALUE1ADDBASE+110,tmpvalue,4);
							FM25_WR_NBYTE(SPECIALVALUE2ADDBASE+110,tmpvalue,4);
							FM25_WR_NBYTE(SPECIALVALUE3ADDBASE+110,tmpvalue,4);
							tmpvalue[4] = 0xee;
							FM25_WR_NBYTE(SPECIALVALUE1ADDBASE+114,tmpvalue+4,1);
							FM25_WR_NBYTE(SPECIALVALUE2ADDBASE+114,tmpvalue+4,1);
							FM25_WR_NBYTE(SPECIALVALUE3ADDBASE+114,tmpvalue+4,1);
						 }
					  }
					  tmpre[0] = 0xF5;
					  tmpre[1] = 0xFA;
					  tmpre[2] = 10;         /*֡��*/
					  tmpre[3] = 0x82;       /*�ظ���Ӧ����*/
					  memcpy(tmpre+4,PCSet_ValueBuf+4,9);
					  tmpcrc = crc16_ccitt(tmpre,13);
					  tmpre[13] = (tmpcrc >>8) & 0xFF;
					  tmpre[14] = tmpcrc & 0xFF;
					  USART_SendNData(USART1,tmpre,15);
					  break;
			case 0x03:				 /*Уʱ����*/
					  memcpy(tmpvalue,PCSet_ValueBuf+5,6);
					  Set_Time(tmpvalue);   /*ʱ��У׼*/
					  tmpre[0] = 0xF5;
					  tmpre[1] = 0xFA;
					  tmpre[2] = 10;         /*֡��*/
					  tmpre[3] = 0x83;       /*�ظ���Ӧ����*/
					  memcpy(tmpre+4,PCSet_ValueBuf+4,9);
					  tmpcrc = crc16_ccitt(tmpre,13);
					  tmpre[13] = (tmpcrc >>8) & 0xFF;
					  tmpre[14] = tmpcrc & 0xFF;
					  USART_SendNData(USART1,tmpre,15);
					  break;
			case 0x04:              /*��¼���*/
					  Record_Delate();
					  tmpre[0] = 0xF5;
					  tmpre[1] = 0xFA;
					  tmpre[2] = 10;         /*֡��*/
					  tmpre[3] = 0x84;       /*�ظ���Ӧ����*/
					  memcpy(tmpre+4,PCSet_ValueBuf+4,9);
					  tmpcrc = crc16_ccitt(tmpre,13);
					  tmpre[13] = (tmpcrc >>8) & 0xFF;
					  tmpre[14] = tmpcrc & 0xFF;
					  USART_SendNData(USART1,tmpre,15);
					  break;
			case 0x05:				/*�����⣬�ᶪʧ����*/
					  tmpvalue[0] = 0xAA;  /*д������*/
					  for(i=0;i<2048;i++)
					  {
					  	FM25_WR_NBYTE(i,tmpvalue,1);
						FM25_RE_NBYTE(i,tmpvalue+1,1);
						if(0 == i%100)
						{
						  RUNLED;
						}
						if(0xAA != tmpvalue[1])
						{
							break;
						}
					  }
					  tmpre[0] = 0xF5;
					  tmpre[1] = 0xFA;
					  tmpre[2] = 10;         /*֡��*/
					  tmpre[3] = 0x85;       /*�ظ���Ӧ����*/
					  tmpre[4] = 0x00;
					  tmpre[5] = (2048 == i)?0xAA:0x55;
					  tmpre[6] = tmpre[5];
					  memset(tmpre+7,0,6);
					  tmpcrc = crc16_ccitt(tmpre,13);
					  tmpre[13] = (tmpcrc >>8) & 0xFF;
					  tmpre[14] = tmpcrc & 0xFF;
					  USART_SendNData(USART1,tmpre,15);
					  break;
			case 0x06:						  /*ADC�ɼ�ֵ��ѯ*/
					  Read_VIPValue();    /*��ȡ��������ֹУ׼������޸�*/
//					  ADC_ValueAvg(ADCBUF);	 /*�ɼ�ADCת��ֵ*/
					  ADC_Value[0] =(0 == ADC_Value[0])?1:ADC_Value[0];		/*��ֹΪ0*/	 	/*����adcֵ*/
					  Power_Voltage = (u16)((ADC_Value[1]*(3413.33/ADC_Value[0])*K1[0]+D1[0]));  
					  Analog_20mA[0] = (u16)((ADC_Value[2]*(3413.33/ADC_Value[0])*K1[1]+D1[1]));  
					  Analog_20mA[1] = (u16)((ADC_Value[3]*(3413.33/ADC_Value[0])*K1[2]+D1[2])); 
					  tmpre[0] = 0xF5;
					  tmpre[1] = 0xFA;
					  tmpre[2] = 10;         /*֡��*/
					  tmpre[3] = 0x86;       /*�ظ���Ӧ����*/
					  tmpre[4] = 0x00;       /*��ַ��*/
					  tmpre[5] = (Power_Voltage >> 8) & 0xFF;  /*��Դ��ѹ*/
					  tmpre[6] = Power_Voltage & 0xFF;
					  tmpre[7] = (Analog_20mA[0] >> 8) & 0xFF; /*��һ· 4-20mA ģ�����ɼ�*/
					  tmpre[8] = Analog_20mA[0] & 0xFF;
					  tmpre[9] = (Analog_20mA[1] >> 8) & 0xFF; /*�ڶ�· 4-20mA ģ�����ɼ�*/
					  tmpre[10] = Analog_20mA[1] & 0xFF;
					  tmpre[11] = 0x00;
					  tmpre[12] = 0x00;
					  tmpcrc = crc16_ccitt(tmpre,13);
					  tmpre[13] = (tmpcrc >>8) & 0xFF;
					  tmpre[14] = tmpcrc & 0xFF;
					  USART_SendNData(USART1,tmpre,15);
					  break;
			case 0x07:					 /*����ADCУ׼�Ͳ�������*/
					  Flag_PCSETING = NO;
					  tmpre[0] = 0xF5;
					  tmpre[1] = 0xFA;
					  tmpre[2] = 10;         /*֡��*/
					  tmpre[3] = 0x87;       /*�ظ���Ӧ����*/
					  memcpy(tmpre+4,PCSet_ValueBuf+4,9);
					  tmpcrc = crc16_ccitt(tmpre,13);
					  tmpre[13] = (tmpcrc >>8) & 0xFF;
					  tmpre[14] = tmpcrc & 0xFF;
					  USART_SendNData(USART1,tmpre,15);
					  break;
			default :break;
					  
		}	
		Flag_GetPCSet = NO;
	}
}

void PWR_EnterSLEEPMode(u32 SysCtrl_Set, u8 PWR_SLEEPEntry)
{
    if (SysCtrl_Set)
         SCB->SCR |= SysCtrl_SLEEPONEXIT_Set;    
    else
        SCB->SCR &= ~SysCtrl_SLEEPONEXIT_Set;
    SCB->SCR &= ~SysCtrl_SLEEPDEEP_Set;    
    if(PWR_SLEEPEntry == PWR_SLEEPEntry_WFI)           
        __WFI();                                        
    else
        __WFE();                                
}
