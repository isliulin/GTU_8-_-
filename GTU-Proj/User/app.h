#ifndef _APP_
#define _APP_

extern u8 Time_Now[6]; /*当前时间*/
extern u8 serverpassword[13];
extern u16 ADCBUF[80]; /*ADC缓存，共4路ADC缓存20个循环*/
extern const u8 LED_NUM[34];	 /*无点0-9 A,b,C,d,E,F,空*/
//extern const u8 LED_NUMPOT[16]; /*有点0-9 A,b,C,d,E,F*/
//const u8 LED_WORD[6] = {0x77,0xF8,0x72,0xBC,0xF2,0xE2}; /*A,b,C,d,E,F*/

extern u8 Server_IP[4];  /*如IP为255.255.255.255 存储为0xFF,0xFF,0xFF,0xFF,依此类推*/
extern u16 Server_PORT;           /*如PORT为65535，存储为0xFFFF,依次类推*/
extern u8 Server_WWW[12];/*服务器域名存储，最长12字节，较短时以'\0'结束，只保存二、三级域名*/
extern u8 Flag_ServerMod; /*服务器登录模式，0时为IP登录，0xAA时以域名登录*/

extern u8 Telphone[5][14]; /*，预留13位号码位置，11位号码前两字节为'0',每个号码最后一位标识该号码是否可用*//*最多存储5个手机号码*/ 
extern u8 Flag_20mA_Power;   /*是否需要接通20mA变送器电源*/
extern u8 Flag_Switch_Power; /*是否需要接通开关量采集电源*/
extern u8 Flag_20mA_Get;     /*是否需要进行20mA变送器采集*/
extern u8 Flag_Switch_Get;   /*是否需要进行开关量采集*/

extern u8 Flag_Error_20mA;  /*0x11 模拟量1采集出错，0x22 模拟量2采集出错 0x33 模拟量1\2采集均出错*/
extern u16 Analog_20mA[2]; /*20mA模拟量最终值，0xFFFF表示数据无效,电流值单位0.01mA*/
extern u8 Switch_Station[2];           /*表示开关量状态，0xAA 通 0x55 断*/
extern u8 Output_Station[2];           /*表示开关量输出状态，0x55 关断 0xAA 接通*/
extern u16 Power_Voltage;                    /*表示电源电压，单位0.001V,最大表示电压65.535V*/  
extern u8 Signal_Quality;      /*GPRS信号强度*/
extern u8 Flag_ConnGPRSOK;     /*GPRS连接是否正常*/
extern u32 Count_GPRSRestart;   /*每天GPRS重启次数*/
extern u16 Count_TimGPRSRe;    /*GPRS 最小重启间隔 50s*/ 
extern u8 Flag_SendingMSG;     /*是否正在发送短信*/
extern u8 Flag_SendingGPRS;    /*是否正在发送GPRS信息*/
extern u8 Flag_RecvingGPRS;    /*是否GPRS模块正在接收*/
extern u32 Flag_GPRSReStep;     /*GPRS重启步骤计数*/
extern u8 Flag_GprsGetClose;   /*标记GPRS是否收到CLOSED*/
extern u16 Count_CloseRestart;  /*接收到CLOSED,后延时数据发送计时,单位s*/
extern u16 Flag_CountClose;     /*标记接收到CLOSE的次数，每5次清零*/
extern u16 Count_TimConnGPRS;    /*GPRS通信中断计时,单位s*/
extern u8 Flag_NeedGPRSRecv;	 /*是否需要进行GPRS接收处理*/
extern u32 Flag_NeedGPRSSend;	 /*是否需要进行GPRS发送，每一位表示一种发送要求，1为需要，0为不需要，0bit表示定时状态发送，*/

extern u16 Count_TimGPRSSend;   /*GTU状态定时发送计时，单位s*/
extern u16 Count_TimGPRSBeat;   /*GTU心跳包发送间隔计时，单位s*/
extern u16 Count_TimGPRSSendInt; /*GPRS发送最小间隔事件计时，单位100ms*/
extern u16 Count_TimGPRSRecv;        /*GPRS接收时，延时发送*/

extern u16 Count_GPRSRecordTim;      /*事件记录上传，回复超时计时，单位s*/

extern u8 Flag_EventNowRe;           /*标记是否收到实时事件上传回复*/
extern u8 Flag_EventPastRe;      	  /*标记是否收到事件记录上传回复*/

extern u8 GetMsg_Buf[400];          /*短信接收的缓存区*/
extern u8 GetMsg_Num[20] ;          /*接收到短信的发送号码*/
extern u8 Flag_GetMsg;              /*标记是否接收到短信*/
extern u8 Flag_GetWrongMsg;         /*标记是否接收到错误短信*/
extern u8 Flag_GetMsgOver;          /*标记短信是否接收完毕*/
extern u16 Count_GetMsgStep;         /*短信接收步骤计数*/
extern u16 Count_GetMsgBuf;          /*短信接收处理计数*/

extern u16 Count_TimRemote;          /*远程更新命令计时*/
extern u8 Flag_RemoteCommond;       /*标记远程更新命令是否有效*/
extern u16 Count_RemoteCommond;      /*远程更新命令接收计数*/

extern u16 Count_RunLed;  /*运行指示灯闪烁间隔，单位100ms*/



extern u16 Event_RecordAddr; /*事件记录最后地址*/
extern u16 Event_RecordNum;  /*事件记录条数*/
//extern u16 Event_UpdateAddr; /*事件上传地址*/

extern u16 EventNowGPRS_Addr;  /*上传实时事件地址*/
extern u16 EventPastGPRS_Addr; /*上传事件记录地址*/
extern u16 EventGPRS_Addr;	 /*发送到GPRS的事件的最后地址*/

extern u16 Recv_SerCom;      /*接收到的服务器命令，用于回复服务器*/
extern u16 Reply_Servalue;   /*回复服务器命令对应的参数，用于回复服务器*/

extern u16 Count_TimADCGet; /*ADC采集间隔倒计时，单位s*/
extern u16 Count_TimADCOffset; /*ADC在通电后再进行采集的时间间隔，单位100ms*/
extern u8  Flag_ADCGet; /*标记是否在进行ADC采集*/

extern u16 Count_TimSWGet; /*开关量采集间隔倒计时，单位s*/
extern u16 Count_TimSWOffset; /*开关量在通电后再进行采集的时间间隔，单位100ms*/
extern u8  Flag_SWGet; /*标记是否在进行开关量采集*/

extern u8 Flag_ErrorShow[4];  /*轮流显示，最多四种错误*/ /*01电源电压超上限02电源电压超下限03 1号4-20mA采集通道出错 2号4-20mA采集通道出错*/
 
extern u16 Count_Gather1SW; /*开关量1状态采集计时*/
extern u16 Count_Gather2SW; /*开关量2状态采集计时*/

extern u8 Flag_ServerSend[32];	/*服务器对应命令发送标记，0xAA表示对应命令需要发送，其余值不用，[0] 定时发送状态信息，[1]发送相关设置参数，[2]实时事件发送*/
                                 /*[31] 回复服务器命令*/
extern u8 GPRS_RecvCommondBuf[3][256]; /*GPRS接收缓存区域，共可以缓存3条命令*/
extern u8 Flag_RecvCommondBuf[3];   /*标识三个缓存区是否有效，0xAA命令有效，其余值命令无效*/

extern u8 Flag_OUT1OpenNeed;	/*开关量输出1是否需要接通*/
extern u8 Flag_OUT1CloseNeed;	/*开关量输出1是否需要关闭*/
extern u8 Flag_OUT2OpenNeed;	/*开关量输出2是否需要接通*/
extern u8 Flag_OUT2CloseNeed;	/*开关量输出2是否需要关闭*/

extern u16 ADC_AlarmHighRange[2];   /*两路4-20mA模拟量报警上限,单位0.01mA*/
extern u16 ADC_AlarmLowRange[2];    /*两路4-20mA模拟量报警下限,单位0.01mA*/
extern u8 Flag_ADCInUse[2];		   /*两路4-20mA模拟量是否启用*/ /*0xAA 启用  0x55 不启用*/
extern u8 Flag_SWInUse[2];          /*两路开关量采集是否使用*/   /*0xAA 启用  0x55 不启用*/
extern u8 Flag_OUT1Model;          /*0x00 不启用开关量输出自动控制*//*开关1输出模式*/
								   /*0x11  1号开关量输入，正反馈(1号输入为通，输出为通；1号输入为断，输出为断)*/
								   /*0x22  1号开关量输入，负反馈(1号输入为通，输出为断；1号输入为断，输出为通)*/
								   /*0x33  2号开关量输入，正反馈(2号输入为通，输出为通；2号输入为断，输出为断)*/
								   /*0x44  2号开关量输入，负反馈(2号输入为通，输出为断；2号输入为断，输出为通)*/
								   /*0x55  1号模拟量采集，正反馈(1号模拟量采集超过上限，输出为开；1号模拟量采集超过下限，输出为断；未超限保持之前的输出状态)*/
								   /*0x66  1号模拟量采集，负反馈(1号模拟量采集超过下限，输出为开；1号模拟量采集超过上限，输出为断；未超限保持之前的输出状态)*/
								   /*0x77  2号模拟量采集，正反馈(2号模拟量采集超过上限，输出为通；2号模拟量采集超过下限，输出为断；未超限保持之前的输出状态)*/
								   /*0x88  2号模拟量采集，负反馈(2号模拟量采集超过下限，输出为通；2号模拟量采集超过上限，输出为断；未超限保持之前的输出状态)*/
extern u8 Flag_OUT2Model;          /*参数定义同上*/	

extern u8 Flag_I2COperation;  /*I2C通信标记*/
extern u8 Flag_I2CError;
extern u16 Count_I2CError;

extern u8 LED_ShowStep[12][4];     /*数码管显示步骤，固定为12步，每步4个数码管显示内容对应对应的数组下标*/
								 /*第一步显示 ADC1 电流值例：A11.0*/
								 /*第二步显示 ADC2 电流值例：A.11.0*/
								 /*第三步显示 SW1  开关量状态例：C 0E*/
								 /*第四步显示 SW2  开关量状态例：C.0FF*/
								 /*第五步显示 OUT1 输出状态例：O 0E*/
								 /*第六步显示 OUT2 输出状态例：O.0FF*/
								 /*第七步显示 SQ   信号质量： F 29*/
								 /*第八步显示 ERROR错误信息1例:E1 01*/ /*显示E 00 没有错误信息*/
								 /*第九步显示 ERROR错误信息2例:E2 02*/
								 /*第十步显示 ERROR错误信息3例:E3 03*/
								 /*第十一步显示ERROR错误信息4例:E4 04*/
								 /*第十二步显示 空*/

extern u16 Show_StepNum;   /*用来计数显示步数*/
extern u16 Count_ShowTimLast;   /*每一步显示持续时间计数，单位100ms*/
extern u16 Count_KeyDownLast;  /*按钮按下时间，防抖动*/
extern u8 Flag_KeyDown;   /*标记按钮是否备按下*/
extern u8 Flag_LEDShowing;  /*标记是否正在进行LED显示*/
extern u8 LEDSHOWNUM;  /*用于LED动态刷新*/
extern u8 Flag_PCSETING;  /*标记是否正在进行PC参数设置和ADC校准*//*参数设置时，电源控制常闭，不进行节电控制*/
extern u8 Flag_GetPCSet;  /*是否获取PC设置端口数据*/
extern u8 Flag_ADCSetAble;  /*标记ADC设置参数是否有效*/ 

extern u16 Count_GetSignal;  /*定时获取信号强度*/      

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
