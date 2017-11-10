#include "include.h"

u8 Time_Now[6] = {0}; /*当前时间*/
//const u8 LED_NUM[10] = {0x3F,0x0C,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F}; 
const u8 LED_NUM[34] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C, 0x39 ,0x5E ,0x79 ,0x71,0x00,	
					   0xBF,0x86,0xDB,0xCF,0xE6,0xED,0xFD,0x87,0xFF,0xEF,0xF7,0xFC,0xB9,0xDE,0xF9,0xF1,0x80}; /*有点0-9 A,b,C,d,E,F，空*/	 /*无点0-9 A,b,C,d,E,F,空*/
//const u8 LED_WORD[6] = {0x77,0xF8,0x72,0xBC,0xF2,0xE2}; /*A,b,C,d,E,F*/

u8 Server_IP[4] = {111,39,237,163};  /*如IP为255.255.255.255 存储为0xFF,0xFF,0xFF,0xFF,依此类推*/
u16 Server_PORT = 8022;           /*如PORT为65535，存储为0xFFFF,依次类推*/
u8 Server_WWW[12] = {"server.com"};/*服务器域名存储，最长12字节，较短时以'\0'结束，只保存二、三级域名*/
u8 Flag_ServerMod = 0; /*服务器登录模式，0时为IP登录，0xAA时以域名登录*/
u8 PUMPHOUSENAME[40] = {"4E0A6D77514965B08DEF7AD9000A000D"};   /*监测点名称，最大字符长度40个unicode码,最多10个汉字*/

u8 Telphone[5][14] = {{0x30,0x30,0x31,0x38,0x33,0x30,0x31,0x38,0x31,0x33,0x34,0x36,0x39,0xee},
					   {0x00,},}; /*，预留13位号码位置，11位号码前两字节为'0',每个号码最后一位标识该号码是否可用*//*最多存储5个手机号码*/ 
					   
u8 serverpassword[] = {0xAA,0x55,0x06,0x48,0x92,0x27,0x77,0x03,0x44,0x55,0x66,0x33,0xCC}; /*服务器登录命令18217025502；如为13位号码，第一位'1'省略，之后12位填充到6字节内*/

u16 ADCBUF[80] = {0}; /*ADC缓存，共4路ADC缓存20个循环*/
u16 ADC_Value[4] = {0}; /*三路ADC滤波后的值，分别为[0]电源电压，[1]第一路4-20mA模拟量，[2]第二路4-20mA模拟量，[3]2.5V标准电压*/
float D1[3] = {0.0,0.0,0.0};	/*3路ADC线性方程截距*/
float K1[3] = {5.12,0.488,0.488};	/*3路ADC线性方程斜率*/

u8 Flag_20mA_Power = NO;   /*是否需要接通20mA变送器电源*/
u8 Flag_Switch_Power = NO; /*是否需要接通开关量采集电源*/
u8 Flag_20mA_Get = NO;     /*是否需要进行20mA变送器采集*/
u8 Flag_Switch_Get = NO;   /*是否需要进行开关量采集*/

u8 Flag_Error_20mA = 0;  /*0x11 模拟量1采集出错，0x22 模拟量2采集出错 0x33 模拟量1\2采集均出错*/
u16 Analog_20mA[2] = {0xFFFF,0xFFFF}; /*20mA模拟量最终值，0xFFFF表示数据无效,电流值单位0.01mA*/
u8 Switch_Station[2] = {0xFF,0xFF};           /*表示开关量采集状态，0xAA 通 0x55 断*/
u8 Output_Station[2] = {0x55,0x55};           /*表示开关量输出状态，0x55 关断 0xAA 接通*/
u16 Power_Voltage = 12567;                    /*表示电源电压，单位0.001V,最大表示电压65.535V*/ 
u8 Signal_Quality = 0;      /*GPRS信号强度*/
u8 Flag_ConnGPRSOK = YES;     /*GPRS连接是否正常*/
u32 Count_GPRSRestart = 0;   /*每天GPRS重启次数*/
u16 Count_TimGPRSRe = 50;    /*GPRS 最小重启间隔 50s*/ 
u8 Flag_SendingMSG = NO;     /*是否正在发送短信*/
u8 Flag_SendingGPRS = NO;    /*是否正在发送GPRS信息*/
u8 Flag_RecvingGPRS = NO;    /*是否GPRS模块正在接收*/
u32 Flag_GPRSReStep = 0;     /*GPRS重启步骤计数*/
u8 Flag_GprsGetClose = NO;   /*标记GPRS是否收到CLOSED*/
u16 Count_CloseRestart = COUNT_CLOSERESTART;  /*接收到CLOSED,后延时数据发送计时,单位s*/
u16 Flag_CountClose = 0;     /*标记接收到CLOSE的次数，每5次清零*/
u16 Count_TimConnGPRS = COUNT_TIMCONNGPRS;    /*GPRS通信中断计时,单位s*/
u8 Flag_NeedGPRSRecv = NO;	 /*是否需要进行GPRS接收处理*/
u32 Flag_NeedGPRSSend = NO;	 /*是否需要进行GPRS发送，每一位表示一种发送要求，1为需要，0为不需要，0bit表示定时状态发送，*/
u16 Count_TimGPRSSend = COUNT_TIMGPRSSEND;   /*GTU状态定时发送计时，单位s*/
u16 Count_TimGPRSBeat = COUNT_TIMGPRSBEAT;   /*GTU心跳包发送间隔计时，单位s*/
u16 Count_TimGPRSSendInt = COUNT_TIMGPRSSENDINT; /*GPRS发送最小间隔计时，单位100ms*/
u16 Count_TimGPRSRecv = 0;        /*GPRS接收时，延时发送*/
u16 Count_GPRSRecordTim = COUNT_GPRSRECORDTIM;      /*事件记录上传，回复超时计时，单位s*/

u8 Flag_EventNowRe = 0;           /*标记是否收到实时事件上传回复*/
u8 Flag_EventPastRe = 0;      	  /*标记是否收到事件记录上传回复*/

u8 GetMsg_Buf[400] = {0};         /*短信接收的缓存区*/
u8 GetMsg_Num[20] = {0};          /*接收到短信的发送号码*/
u8 Flag_GetMsg = NO;              /*标记是否接收到短信*/
u8 Flag_GetWrongMsg = NO;         /*标记是否接收到错误短信*/
u8 Flag_GetMsgOver = NO;          /*标记短信是否接收完毕*/
u16 Count_GetMsgStep = 0;         /*短信接收步骤计数*/
u16 Count_GetMsgBuf = 0;          /*短信接收缓存计数*/

u16 Count_TimRemote = 0;          /*远程更新命令计时*/
u8 Flag_RemoteCommond = NO;       /*标记远程更新命令是否有效*/
u16 Count_RemoteCommond = 0;      /*远程更新命令接收计数*/

u16 Count_RunLed = COUNT_RUNLED;  /*运行指示灯闪烁间隔，单位100ms*/

//u16 Count_ADCInterval = COUNT_ADCINTERVAL; /*ADC获取间隔，单位s*/
//u16 Count_ADCDelay = COUNT_ADCDELAY;       /*ADC转换延时，单位100ms*/


u16 Event_RecordAddr = 0; /*事件记录最后地址*/
u16 Event_RecordNum = 0;  /*事件记录条数*/
//u16 Event_UpdateAddr = 0; /*事件上传地址*/


u16 EventNowGPRS_Addr = 0;  /*上传实时事件地址*/
u16 EventPastGPRS_Addr = 0; /*上传事件记录地址*/
u16 EventGPRS_Addr = 0;	 /*发送到GPRS的事件的最后地址*/

u16 Recv_SerCom = 0;      /*接收到的服务器命令，用于回复服务器*/
u16 Reply_Servalue = 0;   /*回复服务器命令对应的参数，用于回复服务器*/

u16 Count_TimADCGet = COUNT_TIMADCGET; /*ADC采集间隔倒计时，单位s*/
u16 Count_TimADCOffset = COUNT_TIMADCOFFSET; /*ADC在通电后再进行采集的时间间隔，单位100ms*/
u8  Flag_ADCGet = NO; /*标记是否在进行ADC采集*/

u16 Count_TimSWGet = COUNT_TIMSWGET; /*开关量采集间隔倒计时，单位s*/
u16 Count_TimSWOffset = COUNT_TIMSWOFFSET; /*开关量在通电后再进行采集的时间间隔，单位100ms*/
u8  Flag_SWGet = NO; /*标记是否在进行开关量采集*/

u8 Flag_ErrorShow[4] = {0x00,0x00,0x00,0x00};  /*轮流显示，最多四种错误*/ /*01电源电压超上限02电源电压超下限03 1号4-20mA采集通道出错 04 2号4-20mA采集通道出错*/
 
u16 Count_Gather1SW = 0; /*开关量1状态采集计时*/
u16 Count_Gather2SW = 0; /*开关量2状态采集计时*/

u8 Flag_ServerSend[32] = {0x00}; /*服务器对应命令发送标记，0xAA表示对应命令需要发送，其余值不用，[0] 定时发送状态信息，[1]发送相关设置参数，[2]实时事件发送*/
                                 /*[31] 回复服务器命令*/

u8 GPRS_RecvCommondBuf[3][256] = {0}; /*GPRS接收缓存区域，共可以缓存3条命令*/
u8 Flag_RecvCommondBuf[3] = {0x00};   /*标识三个缓存区是否有效，0xAA命令有效，其余值命令无效*/

u8 Flag_OUT1OpenNeed = NO;	/*开关量输出1是否需要接通*/
u8 Flag_OUT1CloseNeed = NO;	/*开关量输出1是否需要关闭*/
u8 Flag_OUT2OpenNeed = NO;	/*开关量输出2是否需要接通*/
u8 Flag_OUT2CloseNeed = NO;	/*开关量输出2是否需要关闭*/

u16 ADC_AlarmHighRange[2] = {0};   /*两路4-20mA模拟量报警上限,单位0.01mA*/
u16 ADC_AlarmLowRange[2] = {0};    /*两路4-20mA模拟量报警下限,单位0.01mA*/
u8 Flag_ADCInUse[2] = {0};		   /*两路4-20mA模拟量是否启用*/ /*0xAA 启用  0x55 不启用*/
u8 Flag_SWInUse[2] = {0};          /*两路开关量采集是否使用*/   /*0xAA 启用  0x55 不启用*/
u8 Flag_OUT1Model = 0x00;          /*0x00 不启用开关量输出自动控制*//*开关1输出模式*/
								   /*0x11  1号开关量输入，正反馈(1号输入为通，输出为通；1号输入为断，输出为断)*/
								   /*0x22  1号开关量输入，负反馈(1号输入为通，输出为断；1号输入为断，输出为通)*/
								   /*0x33  2号开关量输入，正反馈(2号输入为通，输出为通；2号输入为断，输出为断)*/
								   /*0x44  2号开关量输入，负反馈(2号输入为通，输出为断；2号输入为断，输出为通)*/
								   /*0x55  1号模拟量采集，正反馈(1号模拟量采集超过上限，输出为开；1号模拟量采集超过下限，输出为断；未超限保持之前的输出状态)*/
								   /*0x66  1号模拟量采集，负反馈(1号模拟量采集超过下限，输出为开；1号模拟量采集超过上限，输出为断；未超限保持之前的输出状态)*/
								   /*0x77  2号模拟量采集，正反馈(2号模拟量采集超过上限，输出为通；2号模拟量采集超过下限，输出为断；未超限保持之前的输出状态)*/
								   /*0x88  2号模拟量采集，负反馈(2号模拟量采集超过下限，输出为通；2号模拟量采集超过上限，输出为断；未超限保持之前的输出状态)*/
u8 Flag_OUT2Model = 0x00;          /*参数定义同上*/					/*开关2输出模式*/

u8 Flag_I2COperation = NO;  /*I2C通信标记*/
u8 Flag_I2CError = YES;		/*标记I2C总线是否出错*/
u16 Count_I2CError = COUNT_I2CERROR; /*I2C总线错误等待计时*/

u16 Flag_SendMsg = 0;    /*是否需要发送短信，每一位代表一个短信选项*/
						 /*1bit 电源电压低于下限 2bit 电源电压高于上限 3bit 1号模拟量采集故障 4bit 2号4-20mA采集故障 */
						 /*5bit 采集量超过上限，6bit 采集量低于下限，7bit 第二路采集量超过上限，8bit 第二路采集量低于下限*/
u16 Flag_AlreadyMsg = 0;
u8 TelNum = 2;  /*发送短信电话数目*/
u16 MsgLen = 0; /*短信长度*/
u16 Flag_SendMsgNum = 0;        /*短信发送流程记录*/
u8 PDUCmd[16][100] = {"75356e907535538b4f4e4e8e4e0b9650000A000D",          /*短信具体内容,unicode 编码 ，电源电压低于下限*/
					"75356e907535538b9ad84e8e4e0a9650000A000D",    /*电源电压高于上限*/
					"003153f76a2162df91cf91c796c66545969c0000A000D",            /*1号模拟量采集故障*/
					"003253f76a2162df91cf91c796c66545969c000A000D",          /*2号模拟量采集故障*/
					"91C796C691CF8D858FC74E0A9650000A000D",					/*采集量超过上限*/
					"91c796c691cf4f4e4e8e4e0b9650000A000D",};				/*采集量低于下限*/

u8 Flag_ButtonPress = NO;        /*标记触发按钮是否被按下，按下时通过数码管循环显示设备状态*/

u8 LED_ShowStep[12][4]= {0};     /*数码管显示步骤，固定为12步，每步4个数码管显示内容对应对应的数组下标*/
								 /*第一步显示 ADC1 电流值例：A11.0*/
								 /*第二步显示 ADC2 电流值例：A.11.0*/
								 /*第三步显示 SW1  开关量状态例：C 0E*/
								 /*第四步显示 SW2  开关量状态例：C.0FF*/
								 /*第五步显示 OUT1 输出状态例：O 0E*/
								 /*第六步显示 OUT2 输出状态例：O.0FF*/
								 /*第七步显示 SQ   信号质量： F 29*/
								 /*第八步显示 ERROR错误信息1例:E1 01*/ /*显示Ex 00 没有错误信息*/
								 /*第九步显示 ERROR错误信息2例:E2 02*/
								 /*第十步显示 ERROR错误信息3例:E3 03*/
								 /*第十一步显示ERROR错误信息4例:E4 04*/
								 /*第十二步显示 电源电压： d 12.5 */

u16 Show_StepNum = 0;   /*用来计数显示步数*/
u16 Count_ShowTimLast = COUNT_SHOWTIMLAST;   /*每一步显示持续时间计数，单位100ms*/
u8 Flag_KeyDown = NO;   /*标记按钮是否备按下*/
u16 Count_KeyDownLast = 0;  /*按钮按下时间，防抖动*/
u8 Flag_LEDShowing = NO;  /*标记是否正在进行LED显示*/

u8 Flag_EventRecord[20] = {0x00}; /*标记对应事件是否需要记录，0xAA 需要记录，0x00 不需记录*/
								  /*[0] 电压超过上限*/
								  /*[1] 电压超过下限*/
								  /*[2] ADC1采集通道故障*/
								  /*[3] ADC2采集通道故障*/
u8 Flag_EventAlreadyRecord[20] = {0x00}; /*对应标记时间是否已经记录，0xAA 已经记录 0x00 未记录*/
u8 LEDSHOWNUM =0;  /*用于LED动态刷新*/
u8 Flag_PCSETING = NO;  /*标记是否正在进行PC参数设置和ADC校准*//*参数设置时，电源控制常闭，不进行节电控制*/ 
u8 Flag_GetPCSet = NO;  /*是否获取PC设置端口数据*/
u8 Flag_ADCSetAble = NO;  /*标记ADC设置参数是否有效*/
u16 ADC_HandleBUF[2][10] = {0}; /*ADC数据最后处理缓存，用于排序滤波*/
u8 Flag_FirstGetADC = NO;    /*标记是否第一次获取ADC*/ 

u16 Count_GetSignal = COUNT_GETSIGNAL;  /*定时获取信号强度*/   

/******************************************************************************
*函数名称：GPRS_Relink
*函数功能：M35模块重启、重连
*函数输入：无
*函数输出：无
*函数返回：无
*备注：
******************************************************************************/
void GPRS_Relink(void)
{
	/*接收到M35 CLOSE后的处理*/
	if(YES == Flag_GprsGetClose)
	{
		if(0 == Count_CloseRestart)
		{	
			Flag_RecvingGPRS = YES;
			if(3 >= Flag_CountClose)  /*三次CLOSE内，判断为连接存在*/
			{
				Count_TimConnGPRS = COUNT_TIMCONNGPRS;  /*连接存在*/
			}
			else if(5 == Flag_CountClose)
			{
				Flag_CountClose = 0;
			}
			Flag_CountClose ++;
			Count_CloseRestart = COUNT_CLOSERESTART;
			USART_SendNData(USART3,GPRSCmd13,strlen((char *)GPRSCmd13));	 /*关闭场景*/
		}
		else if(((COUNT_CLOSERESTART)/2) == Count_CloseRestart)	    /*ATE0*/
		{
			Count_CloseRestart --;
			USART_SendNData(USART3,GPRSCmd7,strlen((char *)GPRSCmd7));	
		}
		else if(((COUNT_CLOSERESTART)/2-2) == Count_CloseRestart)	    /*设置GPRS的APN*/
		{
			Count_CloseRestart --;
			USART_SendNData(USART3,GPRSCmd15,strlen((char *)GPRSCmd15));	
		}
		else if(((COUNT_CLOSERESTART/2)-4) == Count_CloseRestart)	    /*删除短信*/
		{
			Count_CloseRestart --;
			USART_SendNData(USART3,GPRSCmd5,strlen((char *)GPRSCmd5)); /*删除所有短信*/	
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
			Send_To_Server(serverpassword,13);				/*登录服务器*/
		}	
	}
	/*GPRS重启步骤*/
	else if(NO == Flag_ConnGPRSOK && COUNT_GPRSRESTART >= Count_GPRSRestart && 0 == Count_TimGPRSRe  &&NO == Flag_SendingMSG && NO == Flag_RecvingGPRS)   /*连接中断*/  /*信号质量较差时不进行重连*/
	{
		if(1 == Count_GPRSRestart%2)	/*每两次重连重启一次设备*/
		{
			if(0 == Flag_GPRSReStep)/*第一步，软件关机命令*/
			{
				USART_SendNData(USART3,GPRSCmd10,strlen((char *)GPRSCmd10));   /*软件命令关机*/
				Flag_GPRSReStep = COUNT_GPRSRESTEP;	
			}
			else if((COUNT_GPRSRESTEP - 5) == Flag_GPRSReStep) /*第二步拉高*/
			{
				PWRKEYUP;    /*GPRS POW拉高*/
				Flag_GPRSReStep --;
			}
			else if((COUNT_GPRSRESTEP - 9) == Flag_GPRSReStep)/*第三步GPRSPOWOFF*/
			{
				PWRKEYDOWN;   /*GPRS POW拉低*/
				Flag_GPRSReStep --;	
			}
			else if((COUNT_GPRSRESTEP - 13) == Flag_GPRSReStep) /*第四步拉高*/
			{
				PWRKEYUP;    /*GPRS POW拉高*/
				Flag_GPRSReStep --;			  /*共用14s*/
			}
		}
		else
		{
			if(0 == Flag_GPRSReStep)
			{
				
				USART_SendNData(USART3,GPRSCmd13,strlen((char *)GPRSCmd13));	 /*关闭场景*/
				Flag_GPRSReStep = COUNT_GPRSRESTEP - 25;
			}	
		}
		if((COUNT_GPRSRESTEP - 24) == Flag_GPRSReStep)      /*关闭回显*/
		{
			USART_SendNData(USART3,GPRSCmd7,strlen((char *)GPRSCmd7));  /*关闭回显*/
			Flag_GPRSReStep --;			  /*共用25s*/
		}
		if((COUNT_GPRSRESTEP - 27) == Flag_GPRSReStep)      /*设置GPRS的APN*/
		{
			USART_SendNData(USART3,GPRSCmd15,strlen((char *)GPRSCmd15));  /*设置GPRS的APN*/
			Flag_GPRSReStep --;			  /*共用25s*/
		}
		else if((COUNT_GPRSRESTEP - 30) == Flag_GPRSReStep)
		{
			sprintf((char *)(GPRSCmd6+16),"\"%03d.%03d.%03d.%03d\",\"%d\"\r\0",Server_IP[0],Server_IP[1],Server_IP[2],Server_IP[3],Server_PORT);
			USART_SendNData(USART3,GPRSCmd6,strlen((char *)GPRSCmd6));		 	 /*重启次数统计*/
			Flag_GPRSReStep --;			  /*共用31s*/
											
		}
		else if((COUNT_GPRSRESTEP - 35) == Flag_GPRSReStep)	  /*35-40删除短信*/
		{
			USART_SendNData(USART3,GPRSCmd8,strlen((char *)GPRSCmd8)); /*text格式，用于命令操作*/
			Flag_GPRSReStep --;	
			
		}
		else if((COUNT_GPRSRESTEP - 37) == Flag_GPRSReStep)	  /*35-40删除短信*/
		{
			USART_SendNData(USART3,GPRSCmd2,strlen((char *)GPRSCmd2)); /*短信优先存储位置*/
			Flag_GPRSReStep --;	
		}
		else if((COUNT_GPRSRESTEP - 39) == Flag_GPRSReStep)	  /*35-40删除短信*/
		{
			USART_SendNData(USART3,GPRSCmd5,strlen((char *)GPRSCmd5)); /*删除所有短信*/
			Flag_GPRSReStep --;	
		}
		else if ((COUNT_GPRSRESTEP - 44) == Flag_GPRSReStep)					   /*登录服务器*/
		{
			Send_To_Server(serverpassword,13);
			Flag_GPRSReStep --;			  /*共用45s*/
		} 
		else if( 2 == Flag_GPRSReStep)   /*延时*/
		{
			Count_TimGPRSRe = 50;   /*重启间隔50s*/
			Count_GPRSRestart ++;
			Count_TimConnGPRS = COUNT_TIMCONNGPRS;  /*连接存在*/
		}  
	}
	else if (YES == Flag_ConnGPRSOK || COUNT_GPRSRESTART < Count_GPRSRestart)
	{
		Flag_GPRSReStep = 0;   /*下一次重启*/	
	}
}
/******************************************************************************
*函数名称：Tim_Interrupt
*函数功能：定时中断相关处理
*函数输入：
*函数输出：无
*函数返回：无
*备注：
******************************************************************************/
void Tim_Interrupt(void)
{
	if(0 == Count_TimGPRSSend && YES == Flag_ConnGPRSOK)   /*倒计时为0，且连接正常*/
	{
		Flag_ServerSend[0] = 0xAA;              /*GPRS定时发送置位*/
		Count_TimGPRSSend = COUNT_TIMGPRSSEND;  /*重新计时*/	
	}
	if(0 == Count_TimGPRSSendInt)		/*GPRS发送最小间隔计时*/
	{
		Flag_SendingGPRS = NO;
	}
	if(0 == Count_GPRSRecordTim)    /*GPRS事件回复等待时间*/
	{
		Flag_EventNowRe = 0;
		Flag_EventPastRe = 0;
	}

	/*按键状态*/
	if(COUNT_GATHERIO == Count_KeyDownLast) /*低电平*/
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

	/*开关量获取间隔*/
	if(0 == Count_TimSWGet)
	{
		Count_TimSWGet = COUNT_TIMSWGET;
		Flag_Switch_Power = YES;
	}
	
	if(YES == Flag_Switch_Power && 0 == Count_TimSWOffset)
	{
		Flag_Switch_Get = YES;
	} 
	

	/*模拟量采集间隔*/
	if(0 == Count_TimADCGet)
	{
		Count_TimADCGet = COUNT_TIMADCGET;
		Flag_20mA_Power = YES;
	}
	if(YES == Flag_20mA_Power && 0 == Count_TimADCOffset)
	{
		Flag_20mA_Get = YES;
	}

	/*是否TCP连接正常判断*/
	if(0 == Count_TimConnGPRS && YES == Flag_ConnGPRSOK)
	{
		Flag_ConnGPRSOK = NO;
	}
	else if(0 != Count_TimConnGPRS && NO == Flag_ConnGPRSOK)
	{
		Flag_ConnGPRSOK = YES;
	}
	/*开关量输出控制*/
	Relay_Output();
//	if(0 == )
}
/******************************************************************************
*函数名称：Get_Value
*函数功能：获取相关参数
*函数输入：
*函数输出：无
*函数返回：无
*备注：
******************************************************************************/
void Get_Value(void)
{
//	u16 Tmp_Adcbuf[4] = {0xFFFF,0xFFFF,0xFFFF,0xFFFF}; /*4路ADC缓存，第一路 Vref2.5 第二路 电源电压 第三路 20mA 第四路 20mA*/
//	u8 Tmp_Switchbuf[2] = {0xFF,0xFF}; /*开关量输出*/
//	u16 j=0;i=0;

	/*get ADC*/
	if(YES == Flag_20mA_Power && LOW == PSW1READ)  /*首次需开启电源，等待转化*/
	{
		PSW1ON;
//		ADC_Cmd(ADC1, ENABLE);
//		DMA_Cmd(DMA1_Channel1, ENABLE);
		ADC_Configuration();
//		ADC_DMACmd(ADC1, ENABLE);
		Count_TimADCOffset = COUNT_TIMADCOFFSET;
		Flag_20mA_Get = NO;	
	}
	else if(YES == Flag_20mA_Power && HIGH == PSW1READ && YES == Flag_20mA_Get)	/*获取转化值*/
	{
		ADC_ValueAvg(ADCBUF);	 /*adc 采集数据获取平均值*/
		Flag_20mA_Power = NO;
		Flag_20mA_Get = NO;
//		ADC_DMACmd(ADC1, DISABLE); 
//		DMA_Cmd(DMA1_Channel1, DISABLE);
//		DMA_Configuration();
		PSW1OFF;
		ADC_Cmd(ADC1, DISABLE);		
	}
	else  /*其他情况不作处理*/
	{
	
	}
	/*get Switch*/
	if(YES == Flag_Switch_Power && LOW == PSW2READ)   /*首次需开启电源，等待采集*/
	{
		PSW2ON;
		Count_TimSWOffset = COUNT_TIMSWOFFSET;
		Flag_Switch_Get = NO;
	}
	else if(YES == Flag_Switch_Power && HIGH == PSW2READ && YES == Flag_Switch_Get)	/*获取开关量状态*/
	{
		if(Count_Gather1SW == GATHERIO)   /*稳定在低电平，即接通状态*/
		{
			Switch_Station[0] = 0xAA;
		}
		else                              /*其余状态判断为高电平，即关断状态*/
		{
			Switch_Station[0] = 0x55;
		}

		if(Count_Gather2SW == GATHERIO)  /*稳定在低电平，即接通状态*/
		{
			Switch_Station[1] = 0xAA;
		}
		else                             /*其余状态判断为高电平，关断状态*/
		{
			Switch_Station[1] = 0x55;
		}
		Flag_Switch_Power = NO;
		Flag_Switch_Get = NO;
		PSW2OFF;
	}

	/*get output*/
	if(LOW == PK1READ)   /*开关输出1处于低电平*/
	{
		Output_Station[0] = 0x55;
	}
	else if(HIGH == PK1READ)
	{
		Output_Station[0] = 0xAA;	
	}
	if(LOW == PK2READ)  /*开关输出2处于低电平*/
	{
		Output_Station[1] = 0x55;
	}
	else if(HIGH == PK2READ)
	{
		Output_Station[1] = 0xAA;
	}

	/*计算adc值*/
	ADC_Value[0] =(0 == ADC_Value[0])?1:ADC_Value[0];		/*防止为0*/
	Power_Voltage = (u16)((ADC_Value[1]*(3413.33/ADC_Value[0])*K1[0]+D1[0]));  
	Analog_20mA[0] = (u16)((ADC_Value[2]*(3413.33/ADC_Value[0])*K1[1]+D1[1]));  
	Analog_20mA[1] = (u16)((ADC_Value[3]*(3413.33/ADC_Value[0])*K1[2]+D1[2]));  

	if(Power_Voltage > (u16)(POWERMAX*1.05))
	{
		Flag_EventRecord[0] = 0xAA;
		Flag_ErrorShow[0] = 0x01;
		Flag_SendMsg |= (0x1<<1);
	}
	else if(Power_Voltage <= POWERMAX) /*防止跳动*/
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
	else if(Power_Voltage >= POWERMIN) /*防止跳动*/
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
*函数名称：ADC_ValueAvg
*函数功能: adc采集数据获取
*函数输入：
*函数输出：无
*函数返回：无
*备注：ADC通道 9、11、12、13分别对应1、2、3、4
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

   ADC_Value[0]=(u32)(((averagevaluetmp[0]-maxvalue[0]-minvalue[0])/18))&0x0000ffff;  /*2.5V标准电压*/
   ADC_Value[1]=(u32)(((averagevaluetmp[1]-maxvalue[1]-minvalue[1])/18))&0x0000ffff;  /*12V电源电压*/
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
	ADC_Value[2] = ADC_HandlePoj(ADC_HandleBUF[0],10); /*最大值前两位取平均返回*/
	ADC_Value[3] = ADC_HandlePoj(ADC_HandleBUF[1],10); /*最大值前两位取平均返回*/
}


u16 ADC_HandlePoj(u16*tmpbuf,u16 num)
{
	u16 tmpmaxmax = 0;  /*最大值*/
	u16 tmpmaxmin = 0;  /*次大值*/
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
*函数名称：Communication
*函数功能: 负责GPRS通信
*函数输入：
*函数输出：无
*函数返回：无
*备注：
******************************************************************************/
void Communication(void)
{
	if(YES == Flag_NeedGPRSRecv)
	{
		GPRS_Recv();
		Flag_NeedGPRSRecv = NO;
	}
	
	if((Flag_SendMsg) && (Flag_SendMsg != ( Flag_SendMsg & Flag_AlreadyMsg))) /*需要发送短信，且没发送且短信标记不为空*/
	{
		Send_Msg();			/*发送短信*/
		if(300+300*TelNum == Flag_SendMsgNum)	   /*发送完毕*/
		{
			Flag_AlreadyMsg = Flag_SendMsg;
			Flag_SendingMSG =NO;
			Flag_SendMsgNum = 0;
		}
	}
	else if(!Flag_SendMsg)			 /*不需发送*/
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
	
	if(NO == Flag_SendingGPRS && YES == Flag_ConnGPRSOK && NO == Flag_SendingMSG && NO == Flag_RecvingGPRS)	 /*连接正常没有发送、接收*/
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
//*函数名称：LED_Display
//*函数功能: LED显示
//*函数输入：
//*函数输出：无
//*函数返回：无
//*备注：
//******************************************************************************/
//void LED_Display(void)
//{
//	
//}

/******************************************************************************
*函数名称：Relay_Output
*函数功能: 开关量输出
*函数输入：
*函数输出：无
*函数返回：无
*备注：
******************************************************************************/
void Relay_Output(void)
{

	/*第一路输出控制*/

	/*是否需要自动控制*/
	switch(Flag_OUT1Model)
	{
		case 0x00:break;     /*不需要自动控制*/
		case 0x11:if(0xAA == Switch_Station[0] && 0xAA == Flag_SWInUse[0])  /*开关量1正反馈*/
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
		case 0x22:if(0xAA == Switch_Station[0] && 0xAA == Flag_SWInUse[0])  /*开关量1负反馈*/
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
		case 0x33:if(0xAA == Switch_Station[1] && 0xAA == Flag_SWInUse[1])  /*开关量2正反馈*/
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
		case 0x44:if(0xAA == Switch_Station[1] && 0xAA == Flag_SWInUse[1])  /*开关量2负反馈*/
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
					 && 0xAA == Flag_ADCInUse[0] && (ADC_AlarmHighRange[0] > ADC_AlarmLowRange[0])) /*模拟量1正反馈*/
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
		             && 0xAA == Flag_ADCInUse[0] && (ADC_AlarmHighRange[0] > ADC_AlarmLowRange[0])) /*模拟量1负反馈*/
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
		             && 0xAA == Flag_ADCInUse[1] && (ADC_AlarmHighRange[1] > ADC_AlarmLowRange[1])) /*模拟量2正反馈*/
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
		             && 0xAA == Flag_ADCInUse[1] && (ADC_AlarmHighRange[1] > ADC_AlarmLowRange[1])) /*模拟量2负反馈*/
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

	/*第二路输出控制*/

	/*是否需要自动控制*/
	switch(Flag_OUT1Model)
	{
		case 0x00:break;     /*不需要自动控制*/
		case 0x11:if(0xAA == Switch_Station[0] && 0xAA == Flag_SWInUse[0])  /*开关量1正反馈*/
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
		case 0x22:if(0xAA == Switch_Station[0] && 0xAA == Flag_SWInUse[0])  /*开关量1负反馈*/
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
		case 0x33:if(0xAA == Switch_Station[1] && 0xAA == Flag_SWInUse[1])  /*开关量2正反馈*/
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
		case 0x44:if(0xAA == Switch_Station[1] && 0xAA == Flag_SWInUse[1])  /*开关量2负反馈*/
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
		case 0x55:if((0xFFFF != Analog_20mA[0]) && (ADC_AlarmHighRange[0] <= Analog_20mA[0]) && 0xAA == Flag_ADCInUse[0]) /*模拟量1正反馈*/
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
		case 0x66:if((0xFFFF != Analog_20mA[0]) && (ADC_AlarmHighRange[0] <= Analog_20mA[0]) && 0xAA == Flag_ADCInUse[0]) /*模拟量1负反馈*/
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
		case 0x77:if((0xFFFF != Analog_20mA[1]) && (ADC_AlarmHighRange[1] <= Analog_20mA[1]) && 0xAA == Flag_ADCInUse[1]) /*模拟量2正反馈*/
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
		case 0x88:if((0xFFFF != Analog_20mA[1]) && (ADC_AlarmHighRange[1] <= Analog_20mA[1]) && 0xAA == Flag_ADCInUse[1]) /*模拟量2负反馈*/
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

	/*输出控制*/
	if(YES == Flag_OUT1OpenNeed)   /*第一路需要接通*/
	{
		if(0xAA == Output_Station[0])   /*已经在开状态*/
		{
			
		}
		else if(0x55 == Output_Station[0]) /*在关状态*/
		{
			PK1OPEN;
		}
		Flag_OUT1CloseNeed = NO;  
	}
	else if(YES == Flag_OUT1CloseNeed)  /*第一路需要关闭*/
	{
		if(0xAA == Output_Station[0])   /*已经在开状态*/
		{
			PK1CLOSE;
		}
		else if(0x55 == Output_Station[0]) /*在关状态*/
		{
			
		}
		Flag_OUT1OpenNeed = NO;
	}

	if(YES == Flag_OUT2OpenNeed)   /*第二路需要接通*/
	{
		if(0xAA == Output_Station[1])   /*已经在开状态*/
		{
			
		}
		else if(0x55 == Output_Station[1]) /*在关状态*/
		{
			PK2OPEN;
		}
		Flag_OUT2CloseNeed = NO;  
	}
	else if(YES == Flag_OUT2CloseNeed)  /*第一路需要关闭*/
	{
		if(0xAA == Output_Station[1])   /*已经在开状态*/
		{
			PK2CLOSE;
		}
		else if(0x55 == Output_Station[1]) /*在关状态*/
		{
			
		}
		Flag_OUT2OpenNeed = NO;
	}
	
}

/******************************************************************************
*函数名称：GPRS_Send
*函数功能: GPRS发送功能
*函数输入：
*函数输出：无
*函数返回：无
*备注：
******************************************************************************/
void GPRS_Send(void)
{
	u16 i = 0,j = 0;
	u8 tmpbuf[150] = {0xF5,0xFA};
	u16 tmpcrc = 0;
	u32 tmpaddr = 0;
	if(0xAA == Flag_ServerSend[2] && (0 == Flag_EventNowRe))
	{
		tmpbuf[2] = 0x0c;  /*事件发送数据域长度12B*/
		tmpbuf[3] = 0x03;  /*事件命令码0x03*/
		tmpbuf[4] = 0x00;	 /*地址码空缺*/
		Flag_ServerSend[2] = 0x00;      /*等待下一次发送*/

		if(Event_RecordAddr > EVENTRECORDBASE)
		{
			tmpaddr = Event_RecordAddr-EVENTRECORDLEN;
		}
		else	 /*记录折返读取*/
		{
			tmpaddr = EVENTRECORDBASE*EVENTRECORDMAX-EVENTRECORDLEN;
		}
		Flag_EventNowRe = 1;             /*等待事件发送回复*/
		Count_GPRSRecordTim = COUNT_GPRSRECORDTIM;        /*等待10s回复*/
		EventNowGPRS_Addr = tmpaddr;	 /*等待确认*/
		FM25_RE_NBYTE(tmpaddr,tmpbuf+5,EVENTRECORDLEN);/*读取最后一条事件记录*/
		if(0 == tmpbuf[15])   /*还未上传*/
		{
			tmpcrc = 0;   /*清零*/
			tmpcrc = crc16_ccitt(tmpbuf,15);
			tmpbuf[15] = (tmpcrc>>8)&0xff;
			tmpbuf[16] = tmpcrc&0xff;
	
			Send_To_Server(tmpbuf,17);       /*发送到服务器*/
		}	
	}
	else if(0xAA == Flag_ServerSend[0])	   /*定时发送*/
	{
		tmpbuf[2] = 37; /*数据域长度*/
		tmpbuf[3] = 0xE1;  /*命令码*/
		tmpbuf[4] = 0x00;
		for(i=0;i<6;i++)
		{
			tmpbuf[5+i] = Time_Now[i];   /*时间戳*/
		}	
		tmpbuf[11] = (Analog_20mA[0]>>8) & 0xFF;	/*第一路4-20mA电流值*/
		tmpbuf[12] = Analog_20mA[0]& 0xFF;
		tmpbuf[13] = (Analog_20mA[1]>>8) & 0xFF;	/*第二路4-20mA电流值*/
		tmpbuf[14] = Analog_20mA[1]& 0xFF;
		tmpbuf[15] = Switch_Station[0];             /*开关量1采集状态*/
		tmpbuf[16] = Switch_Station[1];             /*开关量2采集状态*/
		for(i=0;i<4;i++)							/*预留0x00*/
		{
			tmpbuf[17+i] = 0x00;
		}
		tmpbuf[21] = Output_Station[0];             /*开关量输出1状态*/
		tmpbuf[22] = Output_Station[1];             /*开关量输出2状态*/
		for(i=0;i<4;i++)							/*预留0x00*/
		{
			tmpbuf[23+i] = 0x00;
		}
		tmpbuf[27] = (Power_Voltage>>8) & 0xFF;      /*电源电压*/
		tmpbuf[28] = Power_Voltage & 0xFF;
		for(i=0;i<11;i++)							 /*预留空间*/
		{
			tmpbuf[29+i] = 0x00;
		}
		tmpcrc = crc16_ccitt(tmpbuf,40);
		tmpbuf[40] = (tmpcrc >>8) & 0xFF;
		tmpbuf[41] = tmpcrc & 0xFF;
		Send_To_Server(tmpbuf,42);                   /*发送到服务器*/
		Flag_ServerSend[0] = 0x00;
	}
	else if(0xAA == Flag_ServerSend[1])	             /*设置参数上传*/
	{
		tmpbuf[2] = 102;							 /*数据域长度*/
		tmpbuf[3] = 0xE2;							 /*命令码*/
		tmpbuf[4] = 0x00;							 /*地址码*/
		for(i=0;i<2;i++)							 /*4-20mA模拟量报警上下限*/
		{
			tmpbuf[5+4*i] = (ADC_AlarmHighRange[0+i]>>8) & 0xFF;
			tmpbuf[6+4*i] = ADC_AlarmHighRange[0+i] & 0xFF;
			tmpbuf[7+4*i] = (ADC_AlarmLowRange[0+i]>>8) & 0xFF;
			tmpbuf[8+4*i] = ADC_AlarmLowRange[0+i] & 0xFF;
		}
		for(i=0;i<8;i++)		                     /*预留*/
		{
			tmpbuf[13+i] = 0x00;
		}
		tmpbuf[20] = Flag_OUT1Model;				 /*开关1输出模式*/
		tmpbuf[21] = Flag_OUT2Model; 				 /*开关2输出模式*/
		tmpbuf[22] = 0x00;                           /*预留*/
		tmpbuf[23] = 0x00;                           /*预留*/
		for(i=0;i<2;i++)
		{
			tmpbuf[24+i] = Flag_ADCInUse[i];		 /*两路4-20mA模拟量是否启用*/
		}
		for(i=0;i<2;i++)
		{
			tmpbuf[26+i] = Flag_SWInUse[i];          /*两路开关量采集是否启用*/
		}
		for(i=0;i<5;i++)
		{
			for(j=0;j<12;j++)						 /*协议暂定为11位号码*/
			{
				tmpbuf[28+12*i+j] = Telphone[i][2+j];    
			}
		}
		for(i=0;i<16;i++)
		{
			tmpbuf[88+i] = 0x00;                     /*预留*/
		}
		tmpcrc = crc16_ccitt(tmpbuf,105);
		tmpbuf[105] = (tmpcrc >>8) & 0xFF;
		tmpbuf[106] = tmpcrc & 0xFF;
		Send_To_Server(tmpbuf,107);                   /*发送到服务器*/
		Flag_ServerSend[1] = 0x00;         
	}
	else if (Count_GPRSRecordTim == 0 && EventGPRS_Addr != Event_RecordAddr && 0 == Flag_EventPastRe)  /*需检查事件记录是否需要发送*/  /*上一条发送已经恢复*/
	{
		tmpbuf[2] = 0x0c;  /*数据域长度*/
		tmpbuf[3] = 0x04;	 /*命令码*/
		tmpbuf[4] = 0x00;	 /*地址码*/
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
			if(tmpbuf[15] != 1)	/*尚未发送*/
			{
				tmpcrc = crc16_ccitt(tmpbuf,15);
				tmpbuf[15] = (tmpcrc>>8)&0xff;
				tmpbuf[16] = tmpcrc&0xff;
				Send_To_Server(tmpbuf,17);
				Flag_EventPastRe = 1;   /*等待回复*/
				Count_GPRSRecordTim = COUNT_GPRSRECORDTIM;        /*等待30s回复*/
				EventPastGPRS_Addr = EventGPRS_Addr;
				break;
			}
			EventGPRS_Addr += EVENTRECORDLEN;
		}
	}
	
	if (Count_TimGPRSBeat == 0)	   /* 上传或下传数据中断超过时限，发送心跳包*/
	{
		tmpbuf[0] = 0xAA;   /*心跳包*/
		Send_To_Server(tmpbuf,1);
	}

}

/******************************************************************************
*函数名称：Event_RecordToFM
*函数功能: 存储事件
*函数输入：
*函数输出：无
*函数返回：无
*备注：
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
*函数名称：Record_Delete
*函数功能：删除事件记录
*函数调用：
*函数输入：无
*函数输出：读取的记录相关参数
*函数返回：无
*备注：
*******************************************************************************/
void Record_Delate(void)
{
	u8 tmp[8] = {0};
	u16 i = 0;
	memset(tmp,0,8);
	Event_RecordNum = 0;/*事件记录条数*/
	Event_RecordAddr = EVENTRECORDBASE;/*当前事件记录地址*/
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
*函数名称：Record_Event
*函数功能：记录事件
*函数调用：
*函数输入：事件记录最后地址（全局）、事件已记录总个数（全局）、事件号、事件参数
*函数输出：地址更新、个数更新
*函数返回：无
*备注：
*******************************************************************************/
void Record_Event(u16 num,u16 tmpval)
{
	u8 tmp[12] = {0};
	u8 i = 0;
	for(i=0;i<6;i++)
	{
		tmp[i] = Time_Now[i];   /*获取当前时间*/
	}
	tmp[6] = (num/256)&0xff;		   /*事件号扩充为2Byte*/
	tmp[7] = (num%256)&0xff;
	tmp[8] = tmpval/256;			   /*参数值 2Bytes*/
	tmp[9] = tmpval%256;
	tmp[10] = 0;                       /*标记是否已发送到服务器*/
	tmp[11] = 0;	                   /*预留*/
	if(EVENTRECORDBASE + EVENTRECORDLEN*EVENTRECORDMAX <= Event_RecordAddr)	  /*最后地址*/
	{
		Event_RecordAddr = EVENTRECORDBASE;
	}
	if(EVENTRECORDMAX > Event_RecordNum) /*如果未达到记录条数上限，记录数加一*/
	{
		Event_RecordNum ++;
	}
	FM25_WR_NBYTE(Event_RecordAddr,tmp,12);   /*记录*/
	Event_RecordAddr += EVENTRECORDLEN;
	tmp[0] = (Event_RecordNum >> 8)&0xff;
	tmp[1] = Event_RecordNum&0xff;
	tmp[2] = (Event_RecordAddr >> 8)&0xff;
	tmp[3] = Event_RecordAddr&0xff;
	tmp[4] = 0xee;
	
	FM25_WR_NBYTE(EVENTRECORDFBASE,tmp,5);   /*更新记录数*/
	FM25_WR_NBYTE(EVENTRECORDSBASE,tmp,5);   /*更新记录数*/
	FM25_WR_NBYTE(EVENTRECORDTBASE,tmp,5);   /*更新记录数*/
}


/******************************************************************************
*函数名称：GPRS_Recv
*函数功能: GPRS接收功能
*函数输入：
*函数输出：无
*函数返回：无
*备注：
******************************************************************************/
void GPRS_Recv(void)
{
	u8 tmp[50] = {0}; /*数据处理缓存*/
	u8 i=0,j=0;
	for(i=0;i<3;i++)	   /*三个缓存区依次判断*/
	{
		if(0xAA == Flag_RecvCommondBuf[i])
		{
			switch(GPRS_RecvCommondBuf[i][3]) /*判断命令码*/
			{
				case 0xF1:/*时间校准*/
						  memcpy(tmp,GPRS_RecvCommondBuf[i]+5,6); /*时间获取*/
						  Set_Time(tmp);  /*时间写入*/
						  Recv_SerCom = 0xF1;
						  Reply_Servalue = 0x00;
						  Flag_ServerSend[31] = 0xAA;
				          break;
				case 0xF2:/*下位机状态请求*/
				          Flag_ServerSend[0] = 0xAA; /*定时数据发送置位*/
						  Recv_SerCom = 0xF2;
						  Reply_Servalue = 0x00;
						  Flag_ServerSend[31] = 0xAA;
						  break;
				case 0xF3:/*参数设置下发*/
				          Get_SetValue(GPRS_RecvCommondBuf[i]+5); /*从数据域中获取设置参数，并写入存储*/
						  Recv_SerCom = 0xF3;
						  Reply_Servalue = 0x00;
						  Flag_ServerSend[1] = 0xAA;  /*需上传设置参数*/
						  Flag_ServerSend[31] = 0xAA;
						  break;
				case 0xF4:/*设置参数查询*/
				          Flag_ServerSend[1] = 0xAA; /*设置参数上传置位*/
						  Recv_SerCom = 0xF4;
						  Reply_Servalue = 0x00;
						  Flag_ServerSend[31] = 0xAA;
						  break;
				case 0xF5:/*1号开关量输出动作请求*/
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
				case 0xF6:/*2号开关量输出动作请求*/
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
				case 0xF7:/*事件接收回复*/
						  if(0x03 == GPRS_RecvCommondBuf[i][5])	/*当前事件发送回复*/
						  {
							  Flag_EventNowRe = 0;    /*可以发送下一事件*/
							  j = 1;
							  FM25_WR_NBYTE(EventNowGPRS_Addr+10,&j,1); /*标记已发送到服务器*/
						  }
						  else if(0x04 == GPRS_RecvCommondBuf[i][5]) /*事件记录发送回复*/
						  {
							  Flag_EventPastRe = 0;   /*可以发送下一事件记录*/
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
*函数名称：Show_LED
*函数功能：通过74HC595驱动数码管 
*函数输入：显示数字
*函数输出：无
*函数返回：无
*备注：只用来显示数字
******************************************************************************/
void Show_LED(u8 step,u8 lednum)
{
	u16 i =0,j=0;
	if(12 < step || 3 < lednum)
	{
		return;	  /*输入出错，直接返回*/
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
*函数名称：LED_SelfCheck
*函数功能：4个LED一次显示0-9，可以查看是否LED有损坏
*函数输入：显示数字
*函数输出：无
*函数返回：无
*备注：
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
*函数名称：Get_SetValue
*函数功能：从GPRS数据中获取设置参数
*函数输入：参数起始地址
*函数输出：无
*函数返回：无
*备注：
******************************************************************************/
void Get_SetValue(u8* buf)
{
//	u8 tmp[50] = {0};
	u16 i=0;
	
	ADC_AlarmHighRange[0] = (buf[0] <<8)|buf[1];   /*4-20mA第一路 报警上限电流值*/
	ADC_AlarmLowRange[0] = (buf[2] <<8)|buf[3];	   /*4-20mA第一路 报警下限电流值*/
	ADC_AlarmHighRange[1] = (buf[4] <<8)|buf[5];   /*4-20mA第二路 报警上限电流值*/
	ADC_AlarmLowRange[1] = (buf[6] <<8)|buf[7];	   /*4-20mA第二路 报警下限电流值*/

	if(((buf[16]>>4)&0xF) == (buf[16] & 0xF))	   /*数据有效*/
	{
		Flag_OUT1Model = buf[16];                      /*1号开关量*/
	}
	if(((buf[17]>>4)&0xF) == (buf[17] & 0xF))	   /*数据有效*/
	{
		Flag_OUT2Model = buf[17];                      /*2号开关量*/
	}

	Flag_ADCInUse[0] = (0xAA == buf[20])?0xAA:0x55;    /*4-20mA第一路是否启用*/
	Flag_ADCInUse[1] = (0xAA == buf[21])?0xAA:0x55;    /*4-20mA第二路是否启用*/
	Flag_SWInUse[0] = (0xAA == buf[22])?0xAA:0x55;     /*开关量采集第一路是否启用*/
	Flag_SWInUse[1] = (0xAA == buf[23])?0xAA:0x55;     /*开关量采集第二路是否启用*/

	for(i=0;i<5;i++)
	{
		memcpy(Telphone[i]+2,buf+24+12*i,12);   /*电话号码存储*/
	}

	/*参数获取完毕,写入存储器*/
	Update_VIPValue();
}


/*******************************************************************************
*函数名称：GetMsg_Analysis
*函数功能：分析处理接收到的短信息
*函数输入：
*函数返回：
*函数输出：
*备注：
*******************************************************************************/
void GetMsg_Analysis(void)
{
	char * tmpaddr = NULL;   /*指示相关字符地址*/
	u16 i =0;
	u8 tmp[50] = {0};
//	u32 tmpport = 0;
//	u32 tmpip[4] = {0};
	if(58 == Count_GetMsgStep) /*接收到短信后1s再进行处理*/
	{
		Count_GetMsgStep --; /*防止重入*/
		USART_SendNData(USART2,GPRSCmd8,strlen((char *)GPRSCmd8));	/*短信解析用text格式*/
	}
	else if(56 == Count_GetMsgStep) //
	{
		Count_GetMsgStep --; /*防止重入*/
		USART_SendNData(USART2,GPRSCmd14,strlen((char *)GPRSCmd14)); /*读取短信，接收短信，只读取第一条*/
	}
	else if(YES == Flag_GetMsgOver && NO == Flag_GetWrongMsg && 50 < Count_GetMsgStep)	 /*短信接收完成且未出错*/
	{
		if(NULL != (tmpaddr = strstr((char*)GetMsg_Buf,"+CMGR:"))) /*查找接受开头*/
		{
			if(NULL != (tmpaddr = strstr((char *)GetMsg_Buf,"\",\"")))	/*查找到号码开头*/
			{
				GetMsg_Num[0] = '\"';
				i=3;
				while(tmpaddr[i] != '\"')
				{
					GetMsg_Num[i-2] = tmpaddr[i];	  /*号码获取*/
					i++;	
				}
				GetMsg_Num[i-2] = '\"';
				GetMsg_Num[i-1] = '\r';  /*号码获取完成*/
			}	
		}
		
		if(NULL != (tmpaddr = strstr((char*)GetMsg_Buf,"\r\n12345678")))  /*短信固定格式开头,登录号码，IP，Port设置*/
		{
			
			i=0;
			for(i=0;i<32;i++)
			{
				if((tmpaddr[10+i]< 0x30 || tmpaddr[10+i] > 0x39) && ('.' != tmpaddr[10+i])) /*格式错误*/
				{
					break;
				}
			}
			if(i == 32)
			{
				for(i=0;i<12;i++)	   /*号码12位，第一位为0*/
				{
					tmp[i] = tmpaddr[10+i];   /*短信号码获取*/
				}	
				for(i=0;i<6;i++)		/*ascall码转换*/
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
				if(((tmp[20] >>4)&0xF) != 0x00) /*13位号码*/
				{
					tmp[0] = 0x01;
				}
				else
				{
					tmp[0] = 0x00;
				}
				FM25_WR_NBYTE(SPECIALVALUE1ADDBASE,tmp,8);   /*登录号码更新*/
				FM25_WR_NBYTE(SPECIALVALUE2ADDBASE,tmp,8);   /*登录号码更新*/
				FM25_WR_NBYTE(SPECIALVALUE3ADDBASE,tmp,8);   /*登录号码更新*/
//				for(i=0;i<15;i++)
//				{
//					Server_IP[1+i] = tmpaddr[22+i];  /*IP地址获取*/
//				}	
//				for(i=0;i<5;i++)
//				{
//					Server_PORT[1+i] = tmpaddr[37+i]; /*PORT获取*/
//				}
//				memset(tmp,0,10); /*清空*/
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
//					Count_GetMsgStep = 28; /*跳过短信恢复*/
//				}
//				else
//				{
//					for(i=0;i<4;i++)
//					{
//						tmp[i] = tmpip[i]&0xff;
//					}
//					tmp[4] = (tmpport >>8)	& 0xff;
//					tmp[5] = tmpport & 0xff;
//					tmp[6] = 0xee; /*有效标识*/
//					#if 64 == FMMEMORY
//					FM25_WR_NBYTE(64020,tmp,7);
//					#else 
//					FM25_WR_NBYTE(260020,tmp,7);
//					#endif
//					Count_GetMsgStep = 50; /*跳到短信回复*/
//				}	
			}
			else  /*格式不对*/
			{
				Count_GetMsgStep = 28; /*跳过短信恢复*/	
			}

		}
		else if(NULL != (tmpaddr = strstr((char*)GetMsg_Buf,"\r\n87654321012345678")))  /*短信固定格式开头软件重启*/
		{
			NVIC_GenerateSystemReset();  /*重启*/	
		}
		else
		{
			Count_GetMsgStep = 28; /*跳过短信恢复*/	
		}	
	}
	else if(48 == Count_GetMsgStep && YES == Flag_GetMsgOver && NO == Flag_GetWrongMsg )	 /*短信回复*/
	{
		Count_GetMsgStep --;
		USART_SendNData(USART2,GPRSCmd4,strlen((char *)GPRSCmd4));	/*短信发送*/	
	}
	else if(46 == Count_GetMsgStep && YES == Flag_GetMsgOver && NO == Flag_GetWrongMsg )	 /*短信回复*/
	{
		Count_GetMsgStep --;
		USART_SendNData(USART2,GetMsg_Num,strlen((char *)GetMsg_Num));	/*短信发送号码*/	
	}
	else if(44 == Count_GetMsgStep && YES == Flag_GetMsgOver && NO == Flag_GetWrongMsg )	 /*短信回复*/
	{
		Count_GetMsgStep --;
		USART_SendNData(USART2,"set OK\r\n",8);	/*短信发送内容*/	
	}
	else if(42 == Count_GetMsgStep && YES == Flag_GetMsgOver && NO == Flag_GetWrongMsg )	 /*短信回复*/
	{
		Count_GetMsgStep --;
		USART_SendOneData(USART2,0x1a);	/*短信发送内容 Ctrl+Z*/	
	}
	else if(25 == Count_GetMsgStep) /*处理完成，恢复*/
	{
		Count_GetMsgStep --;
		USART_SendNData(USART2,GPRSCmd5,strlen((char *)GPRSCmd5));	/*删除所有短信*/	
	}
	else if(22 == Count_GetMsgStep) /*处理完成，恢复*/
	{
		Count_GetMsgStep --;
		USART_SendNData(USART2,GPRSCmd3,strlen((char *)GPRSCmd3));	/*恢复PDU格式*/
	}
	else if(20 > Count_GetMsgStep)
	{
		Flag_GetMsgOver = NO;
		Flag_GetMsg = NO;
		Flag_GetWrongMsg = NO;
		Count_GetMsgBuf = 0;
		Flag_GprsGetClose = YES; /*标记为接收到Close*/
		Count_TimGPRSRecv = COUNT_TIMGPRSRECV*3;
	}

}

/*******************************************************************************
*函数名称：Send_Msg
*函数功能：发送PDU短信
*函数调用：
*函数输入：短信内容、号码
*函数输出：无
*函数返回：无
*备注：
*******************************************************************************/
void Send_Msg(void)
{
	u16 i=0;
	if(YES == Flag_RecvingGPRS)	/*如果正在发送GPRS信息，延时发送MSG*/
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
		USART_SendNData(USART2,GPRSCmd8,strlen((char*)GPRSCmd8));		   /*删除短信*/
	}
	else if(100 == Flag_SendMsgNum)
	{
		USART_SendNData(USART2,GPRSCmd5,strlen((char*)GPRSCmd5));        /*删除短信，保证能够发送成功*/
	}
	else if(200 == Flag_SendMsgNum)
	{
		USART_SendNData(USART2,GPRSCmd3,strlen((char*)GPRSCmd3));		   /*PDU格式*/
	}
	else if(300 +300*TelNum > Flag_SendMsgNum  && Flag_SendMsgNum > 300)
	{
		if(1 == ((Flag_SendMsgNum-300)%300))
		{
			Msg_Create(Flag_SendMsgNum);										/*制作短信内容*/
			if(0 == MsgLen)	/*短信制作失败*/
			{
				Flag_SendMsgNum = 300 +300*TelNum;
				return;	
			}								   
			USART_SendNData(USART2,GPRSCmd4,strlen((char*)GPRSCmd4));			/*发送AT短信发送命令*/
			USART_SendOneData(USART2,(u8)(((MsgLen)%1000)/100 + 0x30));             /*短信长度百位*/
			USART_SendOneData(USART2,(u8)(((MsgLen)%100)/10 + 0x30));             /*短信长度十位*/
			USART_SendOneData(USART2,(u8)((MsgLen)%10 + 0x30));             /*短信长度个位*/
			USART_SendOneData(USART2,'\r');                               /*回车*/
		}
		else if(140 == ((Flag_SendMsgNum-300)%300))
		{
			USART_SendNData(USART2,GPRSPDU,(MsgLen+1)*2);//strlen((char*)GPRSPDU));    /*发送短信内容*/
			for(i=0;i<2000;i++);
			USART_SendOneData(USART2,0x1a);                            /*ctrl+z*/
		}
		else       /*等待一个循环，延时*/
		{
		
		}
	}	
}

/*******************************************************************************
*函数名称：Msg_Create
*函数功能：短信内容整合
*函数调用：
*函数输入：短信发送步骤计数
*函数输出：短信内容、短信长度
*函数返回：无
*备注：
*******************************************************************************/
void Msg_Create(u16 Flag)
{
	u8 i =0 ,j = 0;
	u8 tmp_phone[14] = {0};  /*号码缓存*/ 
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
		return;   /*错误计数*/
	}
	if(0x30 == Telphone[i][0] && 0x30 == Telphone[i][1]) /*11位号码*/
	{
		for(j=0;j<6;j++)							/*号码前后颠倒*/
		{
			tmp_phone[2*j] = Telphone[i][2*j+3];
			tmp_phone[2*j+1]=Telphone[i][2*j+2];
		}
		tmp_phone[10] = 'F';/*最后一位为F代替*/
		sprintf((char *)(GPRSPDU+12),"%12s",tmp_phone);		  /*合成电话号码*/
		sprintf((char*)(GPRSPDU+24),"000800");				  /*固定格式*/
		tmplen = 0;
		sprintf((char*)(GPRSPDU+32+tmplen*2),"%s",PUMPHOUSENAME);  /*泵房名称*/
		tmplen += strlen((char*)PUMPHOUSENAME)/2;
	}
	else
	{
		for(j=0;j<7;j++)							/*号码前后颠倒*/
		{
			tmp_phone[2*j] = Telphone[i][2*j+1];
			tmp_phone[2*j+1]=Telphone[i][2*j];
		}
		tmp_phone[12] = 'F';/*最后一位为F代替*/
		sprintf((char *)(GPRSPDU+12),"%14s",tmp_phone);		  /*合成电话号码*/
		sprintf((char*)(GPRSPDU+26),"000800");				  /*固定格式*/
		tmplen = 0;
		sprintf((char*)(GPRSPDU+34+tmplen*2),"%s",PUMPHOUSENAME);  /*泵房名称*/
		tmplen += strlen((char*)PUMPHOUSENAME)/2;
		tmplen +=1;   /*13位号码长度增加两字节*/
	}
	
	for(i=0;i<16;i++)
	{
		if(((Flag_SendMsg&(0x1<<i)) && (!(Flag_AlreadyMsg & (0x1<<i))))&&(tmplen<240))
		{
			sprintf((char*)(GPRSPDU+32+tmplen*2),"%s",PDUCmd[i]);
			tmplen += strlen((char*)PDUCmd[i])/2;
			tmpneedvalue ++;	
		}
		else if(tmplen>=240) /*超过最大长度*/
		{
			tmplen = 240;
			break;
		}
	}
	if(0 != tmpneedvalue) /*有报警事件*/
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
	else		    /*防止发送短信时，事件恢复发送空短信*/
	{
		MsgLen = 0;
	}
}

/*******************************************************************************
*函数名称：Read_VIPValue
*函数功能：从铁电读取重要参数
*函数调用：
*函数输入：
*函数输出：
*函数返回：无
*备注：
*******************************************************************************/
void Read_VIPValue(void)
{
	u8 tmp[3][50] = {0};   /*参数读取缓存*/
	u16 i=0,j=0;
	u16 tmpvalue = 0;
	float tmpcalib[4]  ={0.0};

	FM25_RE_NBYTE(SPECIALVALUE1ADDBASE,tmp[0],25);   /*读取IP,PORT,ID等*/
	FM25_RE_NBYTE(SPECIALVALUE2ADDBASE,tmp[1],25); /*第二份*/
	FM25_RE_NBYTE(SPECIALVALUE3ADDBASE,tmp[2],25); /*第三份*/
	if(0 == memcmp(tmp[0],tmp[1],25) && 0 == memcmp(tmp[0],tmp[2],25))  /*三份全部相同*/
	{
		i=0;
	}
	else if(0 == memcmp(tmp[0],tmp[1],25)) /*第三份不同*/
	{
		i=3;
	}
	else if(0 == memcmp(tmp[0],tmp[2],25))	/*第二份不同*/
	{
		i=2;
	}
	else if(0 == memcmp(tmp[1],tmp[2],25)) /*第一份不同*/
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
	if(0xEE == tmp[0][7])	/*数据有效*/
	{
//		memcpy(serverpassword+2,tmp[0]+1,6);   /*登录ID赋值*/
	}
	if(0x11 == tmp[0][10] || 0x22 == tmp[0][10])
	{
		Flag_ServerMod = (0x11 == tmp[0][10])?0x00:0xAA;	/*登录方式*/
		Server_PORT = (tmp[0][11]<<8)&0xFF00;
		Server_PORT |= (tmp[0][12] & 0xFF);					/*登录端口*/
		if(0x11 == tmp[0][10])
		{
			for(i=0;i<4;i++)
			{
				Server_IP[i] = tmp[0][13+i];
			}
		}
		else
		{
			memcpy(Server_WWW,tmp[0]+13,12);              /*域名，二、三级域名*/
		}
	}
	memset(tmp[0],0,25); /*清空*/
	memset(tmp[1],0,25);
	memset(tmp[2],0,25);
	FM25_RE_NBYTE(SPECIALVALUE1ADDBASE+25,tmp[0],40);   /*读取报警号码等*/
	FM25_RE_NBYTE(SPECIALVALUE2ADDBASE+25,tmp[1],40); /*第二份*/
	FM25_RE_NBYTE(SPECIALVALUE3ADDBASE+25,tmp[2],40); /*第三份*/
	if(0 == memcmp(tmp[0],tmp[1],40) && 0 == memcmp(tmp[0],tmp[2],40))  /*三份全部相同*/
	{
		i=0;
	}
	else if(0 == memcmp(tmp[0],tmp[1],40)) /*第三份不同*/
	{
		i=3;
	}
	else if(0 == memcmp(tmp[0],tmp[2],40))	/*第二份不同*/
	{
		i=2;
	}
	else if(0 == memcmp(tmp[1],tmp[2],40)) /*第一份不同*/
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

	for(i=0;i<5;i++)				  /*最多存储5个号码*/
	{
		if(0xEE == tmp[0][8*i+7])	  /*该号码是否有效*/
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

	memset(tmp[0],0,40); /*清空*/
	memset(tmp[1],0,40);
	memset(tmp[2],0,40);
	FM25_RE_NBYTE(SPECIALVALUE1ADDBASE+65,tmp[0],25);   /*读取模拟量、开关量是否启用 模拟量报警上下限，自动控制参数、事件记录等*/
	FM25_RE_NBYTE(SPECIALVALUE2ADDBASE+65,tmp[1],25); /*第二份*/
	FM25_RE_NBYTE(SPECIALVALUE3ADDBASE+65,tmp[2],25); /*第三份*/
	if(0 == memcmp(tmp[0],tmp[1],25) && 0 == memcmp(tmp[0],tmp[2],25))  /*三份全部相同*/
	{
		i=0;
	}
	else if(0 == memcmp(tmp[0],tmp[1],25)) /*第三份不同*/
	{
		i=3;
	}
	else if(0 == memcmp(tmp[0],tmp[2],25))	/*第二份不同*/
	{
		i=2;
	}
	else if(0 == memcmp(tmp[1],tmp[2],25)) /*第一份不同*/
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

	if(0xee == tmp[0][13])	 /*启用报警*/
	{
		for(i=0;i<2;i++)
		{
			ADC_AlarmHighRange[i] = ((tmp[0][5+4*i]<<8)&0xFF00)|(tmp[0][6+4*i] & 0xFF);
			ADC_AlarmLowRange[i] = ((tmp[0][7+4*i]<<8)&0xFF00)|(tmp[0][8+4*i] & 0xFF);
		}
	}
	if(0xee == tmp[0][17])	/*数据有效*/
	{
		Flag_OUT1Model = tmp[0][15];
		Flag_OUT2Model = tmp[0][16];	
	}

	if(0xee == tmp[0][24]) /*数据有效*/
	{
		Event_RecordNum = ((tmp[0][20]<<8) & 0xFF00)|(tmp[0][21] & 0xFF);
		Event_RecordAddr = ((tmp[0][22]<<8) & 0xFF00)|(tmp[0][23] & 0xFF);
	}

	memset(tmp[0],0,25); /*清空*/
	memset(tmp[1],0,25);
	memset(tmp[2],0,25);
	FM25_RE_NBYTE(SPECIALVALUE1ADDBASE+90,tmp[0],25);   /*读取ADC参数校准记录*/
	FM25_RE_NBYTE(SPECIALVALUE2ADDBASE+90,tmp[1],25); /*第二份*/
	FM25_RE_NBYTE(SPECIALVALUE3ADDBASE+90,tmp[2],25); /*第三份*/
	if(0 == memcmp(tmp[0],tmp[1],25) && 0 == memcmp(tmp[0],tmp[2],25))  /*三份全部相同*/
	{
		i=0;
	}
	else if(0 == memcmp(tmp[0],tmp[1],25)) /*第三份不同*/
	{
		i=3;
	}
	else if(0 == memcmp(tmp[0],tmp[2],25))	/*第二份不同*/
	{
		i=2;
	}
	else if(0 == memcmp(tmp[1],tmp[2],25)) /*第一份不同*/
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

	if(0xEE == tmp[0][24])  /*数据有效*/
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
*函数名称：Update_VIPValue
*函数功能：向铁电更新重要参数
*函数调用：
*函数输入：
*函数输出：
*函数返回：无
*备注：不包含ADC校准相关记录
*******************************************************************************/
void Update_VIPValue(void)
{
	u8 tmp[50] = {0};   /*参数写入缓存*/
	u16 i=0,j=0;
	
	/*登录ID，IP,Port等信息*/
	memcpy(tmp+1,serverpassword+2,6); /*只运用12位数字，如果为13位号码，第一位默认为1*/
	tmp[7] = 0xEE;
	tmp[10] = (0x00 == Flag_ServerMod)?0x11:0x22; /*登录方式*/
	tmp[11] = (Server_PORT >> 8) & 0xFF;
	tmp[12] = Server_PORT & 0xFF;
	if(0x00 == Flag_ServerMod)
	{
		memcpy(tmp+13,Server_IP,4);  /*地址IP*/
	}
	else
	{
		memcpy(tmp+13,Server_WWW,12);              /*域名，二、三级域名*/
	}
	FM25_WR_NBYTE(SPECIALVALUE1ADDBASE,tmp,25);	 /*写入三次*/
	FM25_WR_NBYTE(SPECIALVALUE2ADDBASE,tmp,25);
	FM25_WR_NBYTE(SPECIALVALUE3ADDBASE,tmp,25);
	
	/*报警号码存储*/
	memset(tmp,0,25);
	for(i=0;i<5;i++)				  /*最多存储5个号码*/
	{
		if(0xEE == Telphone[i][13])	  /*号码有效*/
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
	FM25_WR_NBYTE(SPECIALVALUE1ADDBASE+25,tmp,40);	 /*写入三次*/
	FM25_WR_NBYTE(SPECIALVALUE2ADDBASE+25,tmp,40);
	FM25_WR_NBYTE(SPECIALVALUE3ADDBASE+25,tmp,40);

	/*报警上下限、参数相关*/
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

	FM25_WR_NBYTE(SPECIALVALUE1ADDBASE+65,tmp,25);	 /*写入三次*/
	FM25_WR_NBYTE(SPECIALVALUE2ADDBASE+65,tmp,25);
	FM25_WR_NBYTE(SPECIALVALUE3ADDBASE+65,tmp,25);		
}

/*******************************************************************************
*函数名称：Show_Update
*函数功能：数码管显示内容更新
*函数调用：
*函数输入：
*函数输出：
*函数返回：无
*备注：
*******************************************************************************/
void Show_Update(void)
{
	LED_ShowStep[0][0] = 10;     /*'A'*/
	LED_ShowStep[0][1] = (Analog_20mA[0]%10000)/1000;  /*电流值十位*/
	LED_ShowStep[0][1] = (0 == LED_ShowStep[0][1])?16:LED_ShowStep[0][1];  /*十位为0 空*/
	LED_ShowStep[0][2] = ((Analog_20mA[0]%1000)/100)+17; /*电流值个位，带点*/
	LED_ShowStep[0][3] = (Analog_20mA[0]%100)/10;      /*电流小数点后一位*/

	LED_ShowStep[1][0] = 10+17;     /* 'A.' */
	LED_ShowStep[1][1] = (Analog_20mA[1]%10000)/1000;  /*电流值十位*/
	LED_ShowStep[1][1] = (0 == LED_ShowStep[1][1])?16:LED_ShowStep[1][1];  /*十位为0 空*/
	LED_ShowStep[1][2] = ((Analog_20mA[1]%1000)/100)+17; /*电流值个位，带点*/
	LED_ShowStep[1][3] = (Analog_20mA[1]%100)/10;      /*电流小数点后一位*/

	LED_ShowStep[2][0] = 12;     /*'C'*/
	if(0xAA == Switch_Station[0])/*通*/
	{
		LED_ShowStep[2][1] = 16;  /*空*/
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
	if(0xAA == Switch_Station[1])/*通*/
	{
		LED_ShowStep[3][1] = 16;  /*空*/
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
	if(0xAA == Output_Station[0])/*通*/
	{
		LED_ShowStep[4][1] = 16;  /*空*/
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
	if(0xAA == Output_Station[1])/*通*/
	{
		LED_ShowStep[5][1] = 16;  /*空*/
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
	LED_ShowStep[6][1] = (Signal_Quality/100)%1000; /*信号质量百位*/
	LED_ShowStep[6][1] = (0 == LED_ShowStep[6][1])?16:LED_ShowStep[6][1];  /*十位为0 空*/
	LED_ShowStep[6][2] = (Signal_Quality/10)%100; /*信号质量十位*/
	LED_ShowStep[6][3] = Signal_Quality%10; /*信号质量个位*/

	LED_ShowStep[7][0] = 14;	 /*'E'*/
	LED_ShowStep[7][1] = 1;	     /*1*/
	LED_ShowStep[7][2] = 0;	     /*0*/
	LED_ShowStep[7][3] = Flag_ErrorShow[0]%10;	     /*错误码*/

	LED_ShowStep[8][0] = 14;	 /*'E'*/
	LED_ShowStep[8][1] = 2;	     /*2*/
	LED_ShowStep[8][2] = 0;	     /*0*/
	LED_ShowStep[8][3] = Flag_ErrorShow[1]%10;	     /*错误码*/

	LED_ShowStep[9][0] = 14;	 /*'E'*/
	LED_ShowStep[9][1] = 3;	     /*3*/
	LED_ShowStep[9][2] = 0;	     /*0*/
	LED_ShowStep[9][3] = Flag_ErrorShow[2]%10;	     /*错误码*/

	LED_ShowStep[10][0] = 14;	 /*'E'*/
	LED_ShowStep[10][1] = 4;	     /*4*/
	LED_ShowStep[10][2] = 0;	     /*0*/
	LED_ShowStep[10][3] = Flag_ErrorShow[3]%10;	     /*错误码*/

	/*电源电压*/
	LED_ShowStep[11][0] = 13;	 /*‘d’*/
	LED_ShowStep[11][1] = Power_Voltage/10000;	 /*电压十位*/
	LED_ShowStep[11][1] = (0 == LED_ShowStep[11][1])?16:LED_ShowStep[11][1]; /*十位为0，显示为空*/
	LED_ShowStep[11][2] = ((Power_Voltage%10000)/1000)+17;	 /*电压个位,带小数点*/
	LED_ShowStep[11][3] = (Power_Voltage%1000)/100;	 /*电压小数点后一位*/
}

/*******************************************************************************
*函数名称：PCSet_Handler
*函数功能：数码管显示内容更新
*函数调用：
*函数输入：
*函数输出：
*函数返回：无
*备注：
*******************************************************************************/
void PCSet_Handler(void)
{
	u8 tmpre[20] = {0};
	u8 tmpvalue[10] = {0};
	u16 tmpcrc = 0;
	u16 i = 0;
	if(YES == Flag_GetPCSet)	/*接收到待处理命令*/
	{
		switch(PCSet_ValueBuf[3])
		{
			case 0x01:							/*开始PC参数设置，和ADC校验*/
					  Flag_PCSETING = YES;
					  ADC_Configuration();
					  PSW1ON;
					  PSW2ON;
					  tmpre[0] = 0xF5;
					  tmpre[1] = 0xFA;
					  tmpre[2] = 10;         /*帧长*/
					  tmpre[3] = 0x81;       /*回复相应命令*/
					  memcpy(tmpre+4,PCSet_ValueBuf+4,9);
					  tmpcrc = crc16_ccitt(tmpre,13);
					  tmpre[13] = (tmpcrc >>8) & 0xFF;
					  tmpre[14] = tmpcrc & 0xFF;
					  USART_SendNData(USART1,tmpre,15);
					  break;
			case 0x02:						/*ADC校准命令*/
					  if(0x1 == PCSet_ValueBuf[5]) /*电源电压校准*/
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
						 else if(0x22 == PCSet_ValueBuf[6])		/*第二次*/
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
					  else if(0x02 == PCSet_ValueBuf[5])   /*4-20MA第一路校准*/
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
						 else if(0x22 == PCSet_ValueBuf[6])		/*第二次*/
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
					  else if(0x02 == PCSet_ValueBuf[5])   /*4-20MA第三路校准*/
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
						 else if(0x22 == PCSet_ValueBuf[6])		/*第二次*/
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
					  tmpre[2] = 10;         /*帧长*/
					  tmpre[3] = 0x82;       /*回复相应命令*/
					  memcpy(tmpre+4,PCSet_ValueBuf+4,9);
					  tmpcrc = crc16_ccitt(tmpre,13);
					  tmpre[13] = (tmpcrc >>8) & 0xFF;
					  tmpre[14] = tmpcrc & 0xFF;
					  USART_SendNData(USART1,tmpre,15);
					  break;
			case 0x03:				 /*校时命令*/
					  memcpy(tmpvalue,PCSet_ValueBuf+5,6);
					  Set_Time(tmpvalue);   /*时间校准*/
					  tmpre[0] = 0xF5;
					  tmpre[1] = 0xFA;
					  tmpre[2] = 10;         /*帧长*/
					  tmpre[3] = 0x83;       /*回复相应命令*/
					  memcpy(tmpre+4,PCSet_ValueBuf+4,9);
					  tmpcrc = crc16_ccitt(tmpre,13);
					  tmpre[13] = (tmpcrc >>8) & 0xFF;
					  tmpre[14] = tmpcrc & 0xFF;
					  USART_SendNData(USART1,tmpre,15);
					  break;
			case 0x04:              /*记录清除*/
					  Record_Delate();
					  tmpre[0] = 0xF5;
					  tmpre[1] = 0xFA;
					  tmpre[2] = 10;         /*帧长*/
					  tmpre[3] = 0x84;       /*回复相应命令*/
					  memcpy(tmpre+4,PCSet_ValueBuf+4,9);
					  tmpcrc = crc16_ccitt(tmpre,13);
					  tmpre[13] = (tmpcrc >>8) & 0xFF;
					  tmpre[14] = tmpcrc & 0xFF;
					  USART_SendNData(USART1,tmpre,15);
					  break;
			case 0x05:				/*铁电检测，会丢失数据*/
					  tmpvalue[0] = 0xAA;  /*写入数据*/
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
					  tmpre[2] = 10;         /*帧长*/
					  tmpre[3] = 0x85;       /*回复相应命令*/
					  tmpre[4] = 0x00;
					  tmpre[5] = (2048 == i)?0xAA:0x55;
					  tmpre[6] = tmpre[5];
					  memset(tmpre+7,0,6);
					  tmpcrc = crc16_ccitt(tmpre,13);
					  tmpre[13] = (tmpcrc >>8) & 0xFF;
					  tmpre[14] = tmpcrc & 0xFF;
					  USART_SendNData(USART1,tmpre,15);
					  break;
			case 0x06:						  /*ADC采集值查询*/
					  Read_VIPValue();    /*读取参数，防止校准后参数修改*/
//					  ADC_ValueAvg(ADCBUF);	 /*采集ADC转换值*/
					  ADC_Value[0] =(0 == ADC_Value[0])?1:ADC_Value[0];		/*防止为0*/	 	/*计算adc值*/
					  Power_Voltage = (u16)((ADC_Value[1]*(3413.33/ADC_Value[0])*K1[0]+D1[0]));  
					  Analog_20mA[0] = (u16)((ADC_Value[2]*(3413.33/ADC_Value[0])*K1[1]+D1[1]));  
					  Analog_20mA[1] = (u16)((ADC_Value[3]*(3413.33/ADC_Value[0])*K1[2]+D1[2])); 
					  tmpre[0] = 0xF5;
					  tmpre[1] = 0xFA;
					  tmpre[2] = 10;         /*帧长*/
					  tmpre[3] = 0x86;       /*回复相应命令*/
					  tmpre[4] = 0x00;       /*地址码*/
					  tmpre[5] = (Power_Voltage >> 8) & 0xFF;  /*电源电压*/
					  tmpre[6] = Power_Voltage & 0xFF;
					  tmpre[7] = (Analog_20mA[0] >> 8) & 0xFF; /*第一路 4-20mA 模拟量采集*/
					  tmpre[8] = Analog_20mA[0] & 0xFF;
					  tmpre[9] = (Analog_20mA[1] >> 8) & 0xFF; /*第二路 4-20mA 模拟量采集*/
					  tmpre[10] = Analog_20mA[1] & 0xFF;
					  tmpre[11] = 0x00;
					  tmpre[12] = 0x00;
					  tmpcrc = crc16_ccitt(tmpre,13);
					  tmpre[13] = (tmpcrc >>8) & 0xFF;
					  tmpre[14] = tmpcrc & 0xFF;
					  USART_SendNData(USART1,tmpre,15);
					  break;
			case 0x07:					 /*结束ADC校准和参数设置*/
					  Flag_PCSETING = NO;
					  tmpre[0] = 0xF5;
					  tmpre[1] = 0xFA;
					  tmpre[2] = 10;         /*帧长*/
					  tmpre[3] = 0x87;       /*回复相应命令*/
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
