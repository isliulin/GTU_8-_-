/*******************************************************************************
*工程名称：GTU
*工程概述：远程监测管网压力，模拟量采集、开关量采集、开关量输出、数码管显示等功能
*创建日期：2016-04-01
*创建人：xth
*开发平台：keil MDK4.22
*最终版本号：V1.0.2(2016-08-17)
*修改记录：
*		  V1.0.1 使用RTC低功耗尝试 Sleepmode 平均电流大约减少8mA
*         V1.0.2 调整PC设置和ADC校准时对ADC的采样值获取
*******************************************************************************/
#include "include.h"

int main()
{
    u8 i=0;//j=0;
	u8 tmp[6] = {0x17,0x08,0x02,0x16,0x03,0x20};
//	u8 tmpbuf[128] = {0};
//	u32 add = 0;
	u16 tasknum = 0;  /*任务运行计数*/
	/*Hardware resource initialization硬件资源初始化*/
	SYS_Init();

	DIG1OFF;
	DIG2OFF;
	DIG3OFF;
	DIG4OFF;
	/*Interrupte Disable 中断失能*/
	Read_VIPValue();   /*读取存储参数*/

	/*GSM module initialization GSM模块初始化*/
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

	/*Digital tube self diagnosis 数码管自诊断*/ 
//	LED_SelfCheck(); 
	DIG1OFF;
	DIG2OFF;
	DIG3OFF;
	DIG4OFF; 
	/*Interrupte Enable 中断使能*/
//	NVIC_RESETFAULTMASK();
	/*主循环*/
	Count_TimConnGPRS = COUNT_TIMCONNGPRS;  /*连接存在*/
	while(1)
	{
		switch(tasknum)
		{
			case 0:tasknum=1;Get_Value();break;			  /*获取采集参数*/
			case 1:tasknum=2;Tim_Interrupt();break;		  /*定时中断处理*/
			case 2:tasknum=3;
							 if(NO == Flag_GetMsg)
							 {
							 	Communication();
							 }
							 else
							 {
							 	GetMsg_Analysis();
							 }
							 break;		  /*通信处理*/
			case 3:tasknum=4;
				   if(YES == Flag_KeyDown)	              /*按键按下*/
				   {
				       if(NO == Flag_LEDShowing)
					   {
					      Flag_LEDShowing = YES;
						  TIM_Cmd(TIM3, ENABLE);
						  Show_Update();                  /*更新显示数据*/
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
			case 4: tasknum = 5;GPRS_Relink();break;	   /*GPRS重连过程*/
			case 5: tasknum = 6;PCSet_Handler();break;	   /*参数设置处理*/
			case 6: tasknum = 0;PWR_EnterSLEEPMode(0,PWR_SLEEPEntry_WFI);break; 					   /*低功耗尝试*/
			default:tasknum = 0;break;
		}
	}
}
