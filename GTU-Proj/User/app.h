#ifndef _APP_
#define _APP_

extern u8 Time_Now[6]; /*��ǰʱ��*/
extern u8 serverpassword[13];
extern u16 ADCBUF[80]; /*ADC���棬��4·ADC����20��ѭ��*/
extern const u8 LED_NUM[34];	 /*�޵�0-9 A,b,C,d,E,F,��*/
//extern const u8 LED_NUMPOT[16]; /*�е�0-9 A,b,C,d,E,F*/
//const u8 LED_WORD[6] = {0x77,0xF8,0x72,0xBC,0xF2,0xE2}; /*A,b,C,d,E,F*/

extern u8 Server_IP[4];  /*��IPΪ255.255.255.255 �洢Ϊ0xFF,0xFF,0xFF,0xFF,��������*/
extern u16 Server_PORT;           /*��PORTΪ65535���洢Ϊ0xFFFF,��������*/
extern u8 Server_WWW[12];/*�����������洢���12�ֽڣ��϶�ʱ��'\0'������ֻ���������������*/
extern u8 Flag_ServerMod; /*��������¼ģʽ��0ʱΪIP��¼��0xAAʱ��������¼*/

extern u8 Telphone[5][14]; /*��Ԥ��13λ����λ�ã�11λ����ǰ���ֽ�Ϊ'0',ÿ���������һλ��ʶ�ú����Ƿ����*//*���洢5���ֻ�����*/ 
extern u8 Flag_20mA_Power;   /*�Ƿ���Ҫ��ͨ20mA��������Դ*/
extern u8 Flag_Switch_Power; /*�Ƿ���Ҫ��ͨ�������ɼ���Դ*/
extern u8 Flag_20mA_Get;     /*�Ƿ���Ҫ����20mA�������ɼ�*/
extern u8 Flag_Switch_Get;   /*�Ƿ���Ҫ���п������ɼ�*/

extern u8 Flag_Error_20mA;  /*0x11 ģ����1�ɼ�����0x22 ģ����2�ɼ����� 0x33 ģ����1\2�ɼ�������*/
extern u16 Analog_20mA[2]; /*20mAģ��������ֵ��0xFFFF��ʾ������Ч,����ֵ��λ0.01mA*/
extern u8 Switch_Station[2];           /*��ʾ������״̬��0xAA ͨ 0x55 ��*/
extern u8 Output_Station[2];           /*��ʾ���������״̬��0x55 �ض� 0xAA ��ͨ*/
extern u16 Power_Voltage;                    /*��ʾ��Դ��ѹ����λ0.001V,����ʾ��ѹ65.535V*/  
extern u8 Signal_Quality;      /*GPRS�ź�ǿ��*/
extern u8 Flag_ConnGPRSOK;     /*GPRS�����Ƿ�����*/
extern u32 Count_GPRSRestart;   /*ÿ��GPRS��������*/
extern u16 Count_TimGPRSRe;    /*GPRS ��С������� 50s*/ 
extern u8 Flag_SendingMSG;     /*�Ƿ����ڷ��Ͷ���*/
extern u8 Flag_SendingGPRS;    /*�Ƿ����ڷ���GPRS��Ϣ*/
extern u8 Flag_RecvingGPRS;    /*�Ƿ�GPRSģ�����ڽ���*/
extern u32 Flag_GPRSReStep;     /*GPRS�����������*/
extern u8 Flag_GprsGetClose;   /*���GPRS�Ƿ��յ�CLOSED*/
extern u16 Count_CloseRestart;  /*���յ�CLOSED,����ʱ���ݷ��ͼ�ʱ,��λs*/
extern u16 Flag_CountClose;     /*��ǽ��յ�CLOSE�Ĵ�����ÿ5������*/
extern u16 Count_TimConnGPRS;    /*GPRSͨ���жϼ�ʱ,��λs*/
extern u8 Flag_NeedGPRSRecv;	 /*�Ƿ���Ҫ����GPRS���մ���*/
extern u32 Flag_NeedGPRSSend;	 /*�Ƿ���Ҫ����GPRS���ͣ�ÿһλ��ʾһ�ַ���Ҫ��1Ϊ��Ҫ��0Ϊ����Ҫ��0bit��ʾ��ʱ״̬���ͣ�*/

extern u16 Count_TimGPRSSend;   /*GTU״̬��ʱ���ͼ�ʱ����λs*/
extern u16 Count_TimGPRSBeat;   /*GTU���������ͼ����ʱ����λs*/
extern u16 Count_TimGPRSSendInt; /*GPRS������С����¼���ʱ����λ100ms*/
extern u16 Count_TimGPRSRecv;        /*GPRS����ʱ����ʱ����*/

extern u16 Count_GPRSRecordTim;      /*�¼���¼�ϴ����ظ���ʱ��ʱ����λs*/

extern u8 Flag_EventNowRe;           /*����Ƿ��յ�ʵʱ�¼��ϴ��ظ�*/
extern u8 Flag_EventPastRe;      	  /*����Ƿ��յ��¼���¼�ϴ��ظ�*/

extern u8 GetMsg_Buf[400];          /*���Ž��յĻ�����*/
extern u8 GetMsg_Num[20] ;          /*���յ����ŵķ��ͺ���*/
extern u8 Flag_GetMsg;              /*����Ƿ���յ�����*/
extern u8 Flag_GetWrongMsg;         /*����Ƿ���յ��������*/
extern u8 Flag_GetMsgOver;          /*��Ƕ����Ƿ�������*/
extern u16 Count_GetMsgStep;         /*���Ž��ղ������*/
extern u16 Count_GetMsgBuf;          /*���Ž��մ������*/

extern u16 Count_TimRemote;          /*Զ�̸��������ʱ*/
extern u8 Flag_RemoteCommond;       /*���Զ�̸��������Ƿ���Ч*/
extern u16 Count_RemoteCommond;      /*Զ�̸���������ռ���*/

extern u16 Count_RunLed;  /*����ָʾ����˸�������λ100ms*/



extern u16 Event_RecordAddr; /*�¼���¼����ַ*/
extern u16 Event_RecordNum;  /*�¼���¼����*/
//extern u16 Event_UpdateAddr; /*�¼��ϴ���ַ*/

extern u16 EventNowGPRS_Addr;  /*�ϴ�ʵʱ�¼���ַ*/
extern u16 EventPastGPRS_Addr; /*�ϴ��¼���¼��ַ*/
extern u16 EventGPRS_Addr;	 /*���͵�GPRS���¼�������ַ*/

extern u16 Recv_SerCom;      /*���յ��ķ�����������ڻظ�������*/
extern u16 Reply_Servalue;   /*�ظ������������Ӧ�Ĳ��������ڻظ�������*/

extern u16 Count_TimADCGet; /*ADC�ɼ��������ʱ����λs*/
extern u16 Count_TimADCOffset; /*ADC��ͨ����ٽ��вɼ���ʱ��������λ100ms*/
extern u8  Flag_ADCGet; /*����Ƿ��ڽ���ADC�ɼ�*/

extern u16 Count_TimSWGet; /*�������ɼ��������ʱ����λs*/
extern u16 Count_TimSWOffset; /*��������ͨ����ٽ��вɼ���ʱ��������λ100ms*/
extern u8  Flag_SWGet; /*����Ƿ��ڽ��п������ɼ�*/

extern u8 Flag_ErrorShow[4];  /*������ʾ��������ִ���*/ /*01��Դ��ѹ������02��Դ��ѹ������03 1��4-20mA�ɼ�ͨ������ 2��4-20mA�ɼ�ͨ������*/
 
extern u16 Count_Gather1SW; /*������1״̬�ɼ���ʱ*/
extern u16 Count_Gather2SW; /*������2״̬�ɼ���ʱ*/

extern u8 Flag_ServerSend[32];	/*��������Ӧ����ͱ�ǣ�0xAA��ʾ��Ӧ������Ҫ���ͣ�����ֵ���ã�[0] ��ʱ����״̬��Ϣ��[1]����������ò�����[2]ʵʱ�¼�����*/
                                 /*[31] �ظ�����������*/
extern u8 GPRS_RecvCommondBuf[3][256]; /*GPRS���ջ������򣬹����Ի���3������*/
extern u8 Flag_RecvCommondBuf[3];   /*��ʶ�����������Ƿ���Ч��0xAA������Ч������ֵ������Ч*/

extern u8 Flag_OUT1OpenNeed;	/*���������1�Ƿ���Ҫ��ͨ*/
extern u8 Flag_OUT1CloseNeed;	/*���������1�Ƿ���Ҫ�ر�*/
extern u8 Flag_OUT2OpenNeed;	/*���������2�Ƿ���Ҫ��ͨ*/
extern u8 Flag_OUT2CloseNeed;	/*���������2�Ƿ���Ҫ�ر�*/

extern u16 ADC_AlarmHighRange[2];   /*��·4-20mAģ������������,��λ0.01mA*/
extern u16 ADC_AlarmLowRange[2];    /*��·4-20mAģ������������,��λ0.01mA*/
extern u8 Flag_ADCInUse[2];		   /*��·4-20mAģ�����Ƿ�����*/ /*0xAA ����  0x55 ������*/
extern u8 Flag_SWInUse[2];          /*��·�������ɼ��Ƿ�ʹ��*/   /*0xAA ����  0x55 ������*/
extern u8 Flag_OUT1Model;          /*0x00 �����ÿ���������Զ�����*//*����1���ģʽ*/
								   /*0x11  1�ſ��������룬������(1������Ϊͨ�����Ϊͨ��1������Ϊ�ϣ����Ϊ��)*/
								   /*0x22  1�ſ��������룬������(1������Ϊͨ�����Ϊ�ϣ�1������Ϊ�ϣ����Ϊͨ)*/
								   /*0x33  2�ſ��������룬������(2������Ϊͨ�����Ϊͨ��2������Ϊ�ϣ����Ϊ��)*/
								   /*0x44  2�ſ��������룬������(2������Ϊͨ�����Ϊ�ϣ�2������Ϊ�ϣ����Ϊͨ)*/
								   /*0x55  1��ģ�����ɼ���������(1��ģ�����ɼ��������ޣ����Ϊ����1��ģ�����ɼ��������ޣ����Ϊ�ϣ�δ���ޱ���֮ǰ�����״̬)*/
								   /*0x66  1��ģ�����ɼ���������(1��ģ�����ɼ��������ޣ����Ϊ����1��ģ�����ɼ��������ޣ����Ϊ�ϣ�δ���ޱ���֮ǰ�����״̬)*/
								   /*0x77  2��ģ�����ɼ���������(2��ģ�����ɼ��������ޣ����Ϊͨ��2��ģ�����ɼ��������ޣ����Ϊ�ϣ�δ���ޱ���֮ǰ�����״̬)*/
								   /*0x88  2��ģ�����ɼ���������(2��ģ�����ɼ��������ޣ����Ϊͨ��2��ģ�����ɼ��������ޣ����Ϊ�ϣ�δ���ޱ���֮ǰ�����״̬)*/
extern u8 Flag_OUT2Model;          /*��������ͬ��*/	

extern u8 Flag_I2COperation;  /*I2Cͨ�ű��*/
extern u8 Flag_I2CError;
extern u16 Count_I2CError;

extern u8 LED_ShowStep[12][4];     /*�������ʾ���裬�̶�Ϊ12����ÿ��4���������ʾ���ݶ�Ӧ��Ӧ�������±�*/
								 /*��һ����ʾ ADC1 ����ֵ����A11.0*/
								 /*�ڶ�����ʾ ADC2 ����ֵ����A.11.0*/
								 /*��������ʾ SW1  ������״̬����C 0E*/
								 /*���Ĳ���ʾ SW2  ������״̬����C.0FF*/
								 /*���岽��ʾ OUT1 ���״̬����O 0E*/
								 /*��������ʾ OUT2 ���״̬����O.0FF*/
								 /*���߲���ʾ SQ   �ź������� F 29*/
								 /*�ڰ˲���ʾ ERROR������Ϣ1��:E1 01*/ /*��ʾE 00 û�д�����Ϣ*/
								 /*�ھŲ���ʾ ERROR������Ϣ2��:E2 02*/
								 /*��ʮ����ʾ ERROR������Ϣ3��:E3 03*/
								 /*��ʮһ����ʾERROR������Ϣ4��:E4 04*/
								 /*��ʮ������ʾ ��*/

extern u16 Show_StepNum;   /*����������ʾ����*/
extern u16 Count_ShowTimLast;   /*ÿһ����ʾ����ʱ���������λ100ms*/
extern u16 Count_KeyDownLast;  /*��ť����ʱ�䣬������*/
extern u8 Flag_KeyDown;   /*��ǰ�ť�Ƿ񱸰���*/
extern u8 Flag_LEDShowing;  /*����Ƿ����ڽ���LED��ʾ*/
extern u8 LEDSHOWNUM;  /*����LED��̬ˢ��*/
extern u8 Flag_PCSETING;  /*����Ƿ����ڽ���PC�������ú�ADCУ׼*//*��������ʱ����Դ���Ƴ��գ������нڵ����*/
extern u8 Flag_GetPCSet;  /*�Ƿ��ȡPC���ö˿�����*/
extern u8 Flag_ADCSetAble;  /*���ADC���ò����Ƿ���Ч*/ 

extern u16 Count_GetSignal;  /*��ʱ��ȡ�ź�ǿ��*/      

void Show_LED(u8 show,u8 lednum);
void GPRS_Send(void);
void GPRS_Recv(void);
void ADC_ValueAvg(u16* ADCDataTab);
void LED_SelfCheck(void);
void Get_SetValue(u8* buf);
void GetMsg_Analysis(void);
void Msg_Create(u16 Flag);
void Send_Msg(void);
void Get_Value(void);
void Tim_Interrupt(void);
void Communication(void);
void Show_Update(void);
void GPRS_Relink(void);
void PCSet_Handler(void);
void Relay_Output(void);
void Read_VIPValue(void);
void Update_VIPValue(void);
void PWR_EnterSLEEPMode(u32 SysCtrl_Set, u8 PWR_SLEEPEntry);
void Record_Event(u16 num,u16 tmpval);
void Event_RecordToFM(void);
void Record_Delate(void);
u16 ADC_HandlePoj(u16*tmpbuf,u16 num);

#endif
