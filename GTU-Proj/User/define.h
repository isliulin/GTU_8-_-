#ifndef _DEFINE_
#define _DEFINE_

//#define ELEMETER  /*是否使用电能表，不使用注释此行*//*获取共用参数：电压、电量时的方式*/
#define ADC1_DR_Address    ((u32)0x4001244C)      /*ADC输出地址，DMA用*/


#define COUNTALARM 500
#define LOW 0				  /*开关量低电平*/
#define HIGH 1				  /*开关量高电平*/
#define TXBUFMAX 512   /*串口中断发送缓冲区最大值*/


#define PCSETUART UART5	/*PC参数设置串口*/
#define PCSETBUF PCSet_ValueBuf  /*PC参数设置接收待处理缓存区*/


#define ADMINPASSWORD 87654321   /*特殊密码*/

#define AVRBUFSIZE   20 /*参数平均缓冲区大小，必须大于1*/

#define GETMSGWORD "FEDCBA98\r\n\r\nOK\r\n"	  /*接收短信判断标准*/

#define FILENAMEADD 0x8003800  /*文件名存储位置，与IAP程序相关，请勿轻易修改*/
#define REMOTEAPPADD 0x8040000 /*远程更新程序地址*/

#define OPEN 1		   /*标记水泵开，仅标记水泵动作指示*/
#define CLOSE 0 	   /*标记水泵停，仅标记水泵动作指示*/


#define YES		 1
#define NO		 0

#define RUNLED    do{GPIO_WriteBit(GPIOC, GPIO_Pin_12,(BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_12)));}while(0);		  /*RUN LED闪烁*/

#define LEDALARMON GPIO_ResetBits(GPIOC,GPIO_Pin_5);    /*Alarm Led ON*/
#define LEDALARMING do{GPIO_WriteBit(GPIOC, GPIO_Pin_5,(BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_5)));}while(0); /*Alarm Led Twinkle*/
#define LEDALARMOFF GPIO_SetBits(GPIOC,GPIO_Pin_5);     /*Alarm Led OFF*/ 

/*数码管相关*/
#define DIG1ON	   GPIO_SetBits(GPIOD,GPIO_Pin_2);     /*数码管1亮*/
#define DIG1OFF	   GPIO_ResetBits(GPIOD,GPIO_Pin_2);   /*数码管1灭*/
#define DIG2ON	   GPIO_SetBits(GPIOB,GPIO_Pin_3);     /*数码管2亮*/
#define DIG2OFF	   GPIO_ResetBits(GPIOB,GPIO_Pin_3);   /*数码管2灭*/
#define DIG3ON 	   GPIO_SetBits(GPIOB,GPIO_Pin_4);     /*数码管3亮*/
#define DIG3OFF	   GPIO_ResetBits(GPIOB,GPIO_Pin_4);   /*数码管3灭*/
#define DIG4ON	   GPIO_SetBits(GPIOB,GPIO_Pin_5);     /*数码管4亮*/
#define DIG4OFF    GPIO_ResetBits(GPIOB,GPIO_Pin_5);   /*数码管4灭*/
#define DDAUP      GPIO_SetBits(GPIOC,GPIO_Pin_11);    /*DDA 高电平*/
#define DDADOWN    GPIO_ResetBits(GPIOC,GPIO_Pin_11);  /*DDA 低电平*/
#define DLCUP      GPIO_SetBits(GPIOC,GPIO_Pin_10);    /*DLC 高电平*/
#define DLCDOWN    GPIO_ResetBits(GPIOC,GPIO_Pin_10);  /*DLC 低电平*/
#define DCKUP      GPIO_SetBits(GPIOA,GPIO_Pin_15);    /*DCK 高电平*/
#define DCKDOWN    GPIO_ResetBits(GPIOA,GPIO_Pin_15);  /*DCK 低电平*/

/*24V电源控制*/
#define PSW1ON	   GPIO_SetBits(GPIOC,GPIO_Pin_4);		 /*24V A路电源，接通*/
#define PSW1OFF	   GPIO_ResetBits(GPIOC,GPIO_Pin_4);	 /*24V A路电源，断开*/
#define PSW1READ   GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_4) /*读取 24V A路电源 输出状态*/
#define PSW2ON	   GPIO_SetBits(GPIOC,GPIO_Pin_5);		 /*24V B路电源，接通*/
#define PSW2OFF    GPIO_ResetBits(GPIOC,GPIO_Pin_5); 	 /*24V B路电源，断开*/
#define PSW2READ   GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_5) /*读取 24V B路电源 输出状态*/

/*SWITCH OUTPUT 开关量输出*/
#define PK1OPEN	   GPIO_SetBits(GPIOC,GPIO_Pin_7);		    /*开关量输出第一路 导通*/
#define PK1CLOSE   GPIO_ResetBits(GPIOC,GPIO_Pin_7);	    /*开关量输出第一路 关闭*/
#define PK1READ    GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_7)	/*开关量输出第一路 读取*/
#define PK2OPEN	   GPIO_SetBits(GPIOC,GPIO_Pin_6);		    /*开关量输出第二路 导通*/
#define PK2CLOSE   GPIO_ResetBits(GPIOC,GPIO_Pin_6);	    /*开关量输出第二路 关闭*/
#define PK2READ    GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_6)	/*开关量输出第二路 读取*/


/*SW INPUT 开关量输入读取*/
#define SW1INREAD 	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8)	 /*开关量输入第一路 读取*/
#define SW2INREAD 	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9)	 /*开关量输入第二路 读取*/
/*按键状态读取*/													
#define KEYSTATUS GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)	/*按键状态读取*/

/*M35 启动*/
#define PWRKEYUP 	GPIO_ResetBits(GPIOA,GPIO_Pin_11);	 /*M35 PWR拉高*/
#define PWRKEYDOWN 	GPIO_SetBits(GPIOA,GPIO_Pin_11);	 /*M35 PWR拉低*/

/*RS485 相关*/
#define RS485TX		GPIO_SetBits(GPIOA,GPIO_Pin_1);    /*485写使能*/
#define RS485RX		GPIO_ResetBits(GPIOA,GPIO_Pin_1);    /*485读使能*/

#define COUNT_POWER   10                   /*判断交流供电不正常判断门限，*/
#define COUNT_ALARMNUM 15                   /*判断报警产生次数*/
#define COUNT_PROTECTPHMAX  50             /*每小时启停保护次数上限*/
#define COUNT_PROTECTNUM 5                 /*启停保护事件产生次数判断门限*/
#define COUNT_GATHERIO 10                   /*IO抖动判断门限*/
#define COUNT_CLOSE  500                    /*水泵参数超限后水泵动作延时，单位20ms*/


#define COUNT_TIMGPRSSEND  120 /*GPRS定时通信计时，单位s*/
#define COUNT_RUNLED   5   /*RUNLED 闪烁频率，单位100ms*/
#define COUNT_TIMCONNGPRS 100 /*GPRS通信中断判定,单位s*/
#define COUNT_TIMPUMPCONTROL  10 /*定时模式下，定时判断间隔，单位s*/
#define COUNT_TIMRELAYINTERVAL 10 /*继电器最短工作间隔，单位s*/
#define COUNT_TIMDISTTIME 60    /*屏幕显示时间校准，单位s*/
#define COUNT_TIMGPRSRECV 20     /*GPRS接收时，发送延时单位10ms*/
#define COUNT_TIMGPRSSENDINT 5   /*GPRS发送到服务器时，发送最小间隔，单位100ms*/
#define COUNT_GPRSRESTART 240     /*GPRS每天最大重启次数*/
#define COUNT_GETSIGNALQUALITY	4    /*获取手机信号强度，时间间隔，单位min*/
#define COUNT_CLOSERESTART 90 		/*接收到CLOSED,后延时数据发送计时,单位s*/
#define COUNT_GPRSRESTEP 60        	/*GPRS重启时间*/
#define COUNT_GETMSGSTEP 60        /*短信接收处理计数及防超时,单位s*/
#define COUNT_GPRSCONNECTEVENT 15  /*连接中断事件计数单独，单位min*/
#define COUNT_TIMREMOTE	180        /*远程更新命令有效时间计时，单位s*/
#define COUNT_I2CERROR 5 		   /*I2C总线出错复位等待时间，单位s*/
#define COUNT_TIMADCGET 30       /*ADC获取间隔，单位s*/
#define COUNT_TIMADCOFFSET    5         /*ADC转换延时，单位100ms*/	/*请勿轻易修改*/
#define COUNT_TIMGPRSBEAT 30        /*GPRS通信心跳包间隔，单位s*/
#define COUNT_TIMSWGET 3        /*开关量获取间隔，单位s*/
#define COUNT_TIMSWOFFSET 3     /*开关量采集延时，单位100ms*/
#define COUNT_GPRSRECORDTIM 30   /*事件记录上传，回复超时限制计时*/

#define COUNT_GETSIGNAL	100   /*定时获取信号强度*/


#define GATHERIO 2               /*主控板开关量采集时长,防抖动，单位10ms*/


#define EVENTRECORDLEN 12		     /*事件记录长度*/

#define EVENTRECORDBASE 200U	 /*事件记录起始地址*/

#define EVENTRECORDFBASE 85  /*事件记录参数记录地址1*/
#define EVENTRECORDSBASE 205  /*事件记录参数记录地址2*/
#define EVENTRECORDTBASE 2010  /*事件记录参数记录地址3*/

#define EVENTRECORDMAX 140			 /*事件记录最大条数*/
//#define EVENTRECORDCOUNTBASE 630     /*事件记录条数记录起始地址，每隔20记录一次共三次*/
//#define EVENTRECORDADDRBASE  635     /*事件记录最后地址记录起始地址，每隔20记录一次共三次*/
#define SPECIALVALUE1ADDBASE 0         /*特殊参数存储地址起始地址1*/
#define SPECIALVALUE2ADDBASE 120       /*特殊参数存储地址起始地址2*/
#define SPECIALVALUE3ADDBASE 1925      /*特殊参数存储地址起始地址3*/ /*三取二，每个地址块长度100Byte*/


#define CANREINITTIMEMIN 60 /*CAN总线初始化次数统计周期,单位min,最大65535*/
#define CANREINITNUMMAX 30 /*CAN总线初始化统计周期内最大初始化次数,最大65535*/

#define COUNT_SHOWTIMLAST 8  /*每一步显示持续时间计数，单位100ms*/

#define POWERMAX  15000  /*电压最大值，单位0.001V*/
#define POWERMIN  10000  /*电压最小值，单位0.001V*/
#define ADCMAX    3000   /*ADC采集最大电流值，单位0.01mA*/
#define ADCMIN    360    /*ADC采集最小电流值，单位0.01mA*/


#define PWR_SLEEPEntry_WFI         ((uint8_t)0x01)
#define PWR_SLEEPEntry_WFE         ((uint8_t)0x02)
#define SysCtrl_SLEEPONEXIT_Set    ((u16)0x0002)
#define SysCtrl_SLEEPDEEP_Set  ((u16)0x0004)

#endif
