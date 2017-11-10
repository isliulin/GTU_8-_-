/*******************************************************************************
*�������ƣ�GTU
*���̸�����Զ�̼�����ѹ����ģ�����ɼ����������ɼ���������������������ʾ�ȹ���
*�������ڣ�2016-04-01
*�����ˣ�xth
*����ƽ̨��keil MDK4.22
*���հ汾�ţ�V1.0.2(2016-08-17)
*�޸ļ�¼��
*		  V1.0.1 ʹ��RTC�͹��ĳ��� Sleepmode ƽ��������Լ����8mA
*         V1.0.2 ����PC���ú�ADCУ׼ʱ��ADC�Ĳ���ֵ��ȡ
*******************************************************************************/
#include "include.h"

int main()
{
    u8 i=0;//j=0;
	u8 tmp[6] = {0x17,0x08,0x02,0x16,0x03,0x20};
//	u8 tmpbuf[128] = {0};
//	u32 add = 0;
	u16 tasknum = 0;  /*�������м���*/
	/*Hardware resource initializationӲ����Դ��ʼ��*/
	SYS_Init();

	DIG1OFF;
	DIG2OFF;
	DIG3OFF;
	DIG4OFF;
	/*Interrupte Disable �ж�ʧ��*/
	Read_VIPValue();   /*��ȡ�洢����*/

	/*GSM module initialization GSMģ���ʼ��*/
	RS485RX;
	PWRKEYDOWN;
	for(i=0;i<10;i++)
	{
//		RUNLED;
		LED_SelfCheck();
	}
	PWRKEYUP;
	Set_Time(tmp);
	for(i=0;i<25;i++)
	{
//		RUNLED;
		LED_SelfCheck();
	}
	GPRS_Init();
	Send_To_Server(serverpassword,13);
	PSW1ON;
	PSW2ON;

	/*Digital tube self diagnosis ����������*/ 
//	LED_SelfCheck(); 
	DIG1OFF;
	DIG2OFF;
	DIG3OFF;
	DIG4OFF; 
	/*Interrupte Enable �ж�ʹ��*/
//	NVIC_RESETFAULTMASK();
	/*��ѭ��*/
	Count_TimConnGPRS = COUNT_TIMCONNGPRS;  /*���Ӵ���*/
	while(1)
	{
		switch(tasknum)
		{
			case 0:tasknum=1;Get_Value();break;			  /*��ȡ�ɼ�����*/
			case 1:tasknum=2;Tim_Interrupt();break;		  /*��ʱ�жϴ���*/
			case 2:tasknum=3;
							 if(NO == Flag_GetMsg)
							 {
							 	Communication();
							 }
							 else
							 {
							 	GetMsg_Analysis();
							 }
							 break;		  /*ͨ�Ŵ���*/
			case 3:tasknum=4;
				   if(YES == Flag_KeyDown)	              /*��������*/
				   {
				       if(NO == Flag_LEDShowing)
					   {
					      Flag_LEDShowing = YES;
						  TIM_Cmd(TIM3, ENABLE);
						  Show_Update();                  /*������ʾ����*/
						  Count_ShowTimLast = COUNT_SHOWTIMLAST; 	
					   }
					   if(0 == Count_ShowTimLast)
					   {
					   	Count_ShowTimLast = COUNT_SHOWTIMLAST;
						Show_StepNum ++;
					   	Show_StepNum = Show_StepNum%12;
					   }
				   }
				   else
				   {
				   	   if(YES == Flag_LEDShowing)
					   {
					       Flag_LEDShowing = NO;
						   TIM_Cmd(TIM3, DISABLE);
						   DIG1OFF;
						   DIG2OFF;
						   DIG3OFF;
						   DIG4OFF; 
					   }
					   Show_StepNum = 0;
				   }
				   break;
			case 4: tasknum = 5;GPRS_Relink();break;	   /*GPRS��������*/
			case 5: tasknum = 6;PCSet_Handler();break;	   /*�������ô���*/
			case 6: tasknum = 0;PWR_EnterSLEEPMode(0,PWR_SLEEPEntry_WFI);break; 					   /*�͹��ĳ���*/
			default:tasknum = 0;break;
		}
	}
}
