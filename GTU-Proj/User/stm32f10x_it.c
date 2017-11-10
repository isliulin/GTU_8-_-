/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : stm32f10x_it.c
* Author             : MCD Application Team
* Version            : V2.0.1
* Date               : 06/13/2008
* Description        : Main Interrupt Service Routines.
*                      This file provides template for all exceptions handler
*                      and peripherals interrupt service routine.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/*门禁状态说明：首先如果15s内继电器闭合3次，说明有人进入，之后30min之后再重新判断，*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "include.h"

u16 Count_1s = 0; /*1s定时计数*/
u16 Count_100ms = 0; /*100ms定时计数*/
u16 Count_500ms = 0; /*500ms定时计数*/
u16 Count_1min = 0; /*1min定时计数*/
u16 Count_1h = 0; /*1h定时计数*/
u16 Count_1d = 0; /*1d定时计数*/
//
//u8 Count_Access = 0;  /*记录15内,继电器启停次数*/
//u8 Flag_DoorAccess = 0; /*用于门禁判断*/
//
//u8 PumpOne_StatusCount = 0;   /*用于单水泵时水泵状态获取消抖动*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : NMIException
* Description    : This function handles NMI exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NMIException(void)
{
}

/*******************************************************************************
* Function Name  : HardFaultException
* Description    : This function handles Hard Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void HardFaultException(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : MemManageException
* Description    : This function handles Memory Manage exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void MemManageException(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : BusFaultException
* Description    : This function handles Bus Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void BusFaultException(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : UsageFaultException
* Description    : This function handles Usage Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UsageFaultException(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : DebugMonitor
* Description    : This function handles Debug Monitor exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DebugMonitor(void)
{
}

/*******************************************************************************
* Function Name  : SVCHandler
* Description    : This function handles SVCall exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SVCHandler(void)
{
}

/*******************************************************************************
* Function Name  : PendSVC
* Description    : This function handles PendSVC exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PendSVC(void)
{
}

/*******************************************************************************
* Function Name  : SysTickHandler
* Description    : This function handles SysTick Handler.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SysTickHandler(void)
{
	TimingDelay = (TimingDelay ==0)?0:(TimingDelay-1);
}

/*******************************************************************************
* Function Name  : WWDG_IRQHandler
* Description    : This function handles WWDG interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void WWDG_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : PVD_IRQHandler
* Description    : This function handles PVD interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PVD_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TAMPER_IRQHandler
* Description    : This function handles Tamper interrupt request. 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TAMPER_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : RTC_IRQHandler
* Description    : This function handles RTC global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : FLASH_IRQHandler
* Description    : This function handles Flash interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void FLASH_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : RCC_IRQHandler
* Description    : This function handles RCC interrupt request. 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXTI0_IRQHandler
* Description    : This function handles External interrupt Line 0 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI0_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXTI1_IRQHandler
* Description    : This function handles External interrupt Line 1 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXTI2_IRQHandler
* Description    : This function handles External interrupt Line 2 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI2_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXTI3_IRQHandler
* Description    : This function handles External interrupt Line 3 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI3_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : EXTI4_IRQHandler
* Description    : This function handles External interrupt Line 4 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI4_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1_Channel1_IRQHandler
* Description    : This function handles DMA1 Channel 1 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1_Channel2_IRQHandler
* Description    : This function handles DMA1 Channel 2 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel2_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1_Channel3_IRQHandler
* Description    : This function handles DMA1 Channel 3 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel3_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1_Channel4_IRQHandler
* Description    : This function handles DMA1 Channel 4 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel4_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1_Channel5_IRQHandler
* Description    : This function handles DMA1 Channel 5 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel5_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1_Channel6_IRQHandler
* Description    : This function handles DMA1 Channel 6 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel6_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA1_Channel7_IRQHandler
* Description    : This function handles DMA1 Channel 7 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA1_Channel7_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : ADC1_2_IRQHandler
* Description    : This function handles ADC1 and ADC2 global interrupts requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADC1_2_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : USB_HP_CAN_TX_IRQHandler
* Description    : This function handles USB High Priority or CAN TX interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_HP_CAN1_TX_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : USB_LP_CAN_RX0_IRQHandler
* Description    : This function handles USB Low Priority or CAN RX0 interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_LP_CAN1_RX0_IRQHandler(void)
{
//	CanRxMsg RxMessage;
//	u32 transfer_id = 0;
//	u32 source_id = 0;
//	u32 commond = 0;
//	u32 data_index = 0;
//	CAN_Receive(CAN_FIFO0, &RxMessage);
//	if((RxMessage.IDE == CAN_ID_EXT))
//	{
//		transfer_id = (RxMessage.ExtId >> 16) & 0x000003fF;
//		source_id = (RxMessage.ExtId >> 8) & 0x000000FF;
//		commond =  (RxMessage.ExtId >> 4) & 0x0000000F;
//		data_index = RxMessage.ExtId & 0x0000000F;
//		if(transfer_id == 0)	/*主控板接收*/
//		{
//		  CANRx_Msg(RxMessage, source_id,commond, data_index);
//		}
//	}
}

/*******************************************************************************
* Function Name  : CAN_RX1_IRQHandler
* Description    : This function handles CAN RX1 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CAN_RX1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : CAN_SCE_IRQHandler
* Description    : This function handles CAN SCE interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CAN_SCE_IRQHandler(void)    /*CAN总线出错*/
{
}

/*******************************************************************************
* Function Name  : EXTI9_5_IRQHandler
* Description    : This function handles External lines 9 to 5 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI9_5_IRQHandler(void)
{

}

/*******************************************************************************
* Function Name  : TIM1_BRK_IRQHandler
* Description    : This function handles TIM1 Break interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM1_BRK_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM1_UP_IRQHandler
* Description    : This function handles TIM1 overflow and update interrupt 
*                  request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM1_UP_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM1_TRG_COM_IRQHandler
* Description    : This function handles TIM1 Trigger and commutation interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM1_TRG_COM_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM1_CC_IRQHandler
* Description    : This function handles TIM1 capture compare interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM1_CC_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM2_IRQHandler
* Description    : This function handles TIM2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM2_IRQHandler(void)
{
//	u8 i = 0;
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)			/*10ms*/
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

		if(LOW == KEYSTATUS)
		{
		   Count_KeyDownLast = (Count_KeyDownLast >= COUNT_GATHERIO)?COUNT_GATHERIO:(Count_KeyDownLast+1);
		}
		else
		{
			Count_KeyDownLast = (Count_KeyDownLast > 0)?(Count_KeyDownLast-1):0;
		}
		Count_Gather1SW = (LOW == SW1INREAD)?((Count_Gather1SW <= GATHERIO)?(GATHERIO):(Count_Gather1SW + 1)):((Count_Gather1SW == 0)?(0):(Count_Gather1SW - 1));
		Count_Gather2SW = (LOW == SW2INREAD)?((Count_Gather2SW <= GATHERIO)?(GATHERIO):(Count_Gather2SW + 1)):((Count_Gather2SW == 0)?(0):(Count_Gather2SW - 1));
		Count_100ms ++;
		if(10 <= Count_100ms)  /*100ms*/
		{
			Count_100ms = 0;
			Count_TimADCOffset = (0 < Count_TimADCOffset)?(Count_TimADCOffset - 1):0;  /*ADC获取延时*/
			Count_TimSWOffset = (0 < Count_TimSWOffset)?(Count_TimSWOffset - 1):0;  /*ADC获取延时*/
			Count_TimGPRSSendInt = (0 < Count_TimGPRSSendInt)?(Count_TimGPRSSendInt - 1):0;   /*GPRS发送最小间隔*/
			Count_ShowTimLast = (0 < Count_ShowTimLast)?(Count_ShowTimLast - 1):0;     /*LED分步显示，每步的持续时间*/
			Count_RunLed = (0 < Count_RunLed)?(Count_RunLed-1):0;                  /*RUNLED 指示灯闪烁*/
			Count_TimGPRSRecv = (0 < Count_TimGPRSRecv)?(Count_TimGPRSRecv-1):0;   /*GPRS接收计时*/
		}
		Count_500ms++;
		if(50 <= Count_500ms)  /*500ms*/
		{
			Count_500ms = 0;
		}
		Count_1s ++;
		if(100 <= Count_1s)	   /*1s*/
		{
			Count_1s = 0;
			if(0 < Count_TimGPRSRe)	 /*M35模块重启间隔计时*/
			{
				Count_TimGPRSRe --;
			}
			if(0 < Count_CloseRestart) /*接收到CLOSED,后延时数据发送计时*/
			{
				Count_CloseRestart --;	
			}

			Count_TimConnGPRS = (0 < Count_TimConnGPRS)?(Count_TimConnGPRS - 1):0;		/*GPRS通信中断计时，尝试三元运算，效率更高?*/
			Count_TimADCGet = (0 < Count_TimADCGet)?(Count_TimADCGet - 1):0;      /*ADC获取间隔*/
			Count_TimSWGet = (0 < Count_TimSWGet)?(Count_TimSWGet - 1):0;         /*开关量获取间隔*/
			Count_TimGPRSSend = (0 < Count_TimGPRSSend)?(Count_TimGPRSSend - 1):0; /*GPRS定时状态信息发送计时*/
			Count_TimGPRSBeat = (0 < Count_TimGPRSBeat)?(Count_TimGPRSBeat - 1):0; /*GPRS心跳包间隔计时*/ 
			Count_GetMsgStep = (0 < Count_GetMsgStep)?(Count_GetMsgStep - 1):0;    /*短信处理步骤计数*/
			Count_GPRSRecordTim = (0 < Count_GPRSRecordTim)?(Count_GPRSRecordTim-1):0;	 /* GPRS事件回复等待时间*/
			Flag_GPRSReStep = (0 < Flag_GPRSReStep)?(Flag_GPRSReStep-1):0;         /*GPRS重启步骤*/

			Count_GetSignal = (0 < Count_GetSignal)?(Count_GetSignal-1):0;
			if(0 < Count_I2CError)
			{
				Count_I2CError --;
				if(0 == Count_I2CError)
				{
					Flag_I2CError = NO;
				}
			}
			Get_Time(Time_Now);   /*每s获取一次时间*/
			Count_1min ++;
			if(60 <= Count_1min)
			{
				Count_1min = 0;
				Count_1h ++;
				if(60 <= Count_1h)
				{
					Count_1h = 0;
					Count_1d ++;
					if(24 <= Count_1d)
					{
						Count_GPRSRestart =0;
						Count_1d = 0;
					}
				}
			}
		}
	}
}


/*******************************************************************************
* Function Name  : TIM3_IRQHandler
* Description    : This function handles TIM3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)			/*10ms*/
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		LEDSHOWNUM +=1;
		LEDSHOWNUM %= 4;
		Show_LED(Show_StepNum,LEDSHOWNUM);
	}
}

/*******************************************************************************
* Function Name  : TIM4_IRQHandler
* Description    : This function handles TIM4 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM4_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : I2C1_EV_IRQHandler
* Description    : This function handles I2C1 Event interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C1_EV_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : I2C1_ER_IRQHandler
* Description    : This function handles I2C1 Error interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C1_ER_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : I2C2_EV_IRQHandler
* Description    : This function handles I2C2 Event interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C2_EV_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : I2C2_ER_IRQHandler
* Description    : This function handles I2C2 Error interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C2_ER_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : SPI1_IRQHandler
* Description    : This function handles SPI1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : SPI2_IRQHandler
* Description    : This function handles SPI2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI2_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : USART1_IRQHandler
* Description    : This function handles USART1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART1_IRQHandler(void)										 /*DGUS*/
{
	u8 tmp = 0 ;
	if(USART_GetFlagStatus(USART1,USART_FLAG_ORE)!=RESET)
	{
		USART_ReceiveData(USART1);
	}
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);     /*清除中断标志 */
		tmp = USART_ReceiveData(USART1);
		PCValueSet_Handle(tmp);
	}
//	else if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)  /*发送中断*/ /*优先接收*/
//	{
//		USART_SendData(USART1, UART1TX_Buf[UART1TX_Head++]);
//
//	    if(UART1TX_Head == UART1TX_Tail)
//	    {
//	      /* Disable the USART1 Transmit interrupt */
//		  UART1TX_Head = 0;
//		  UART1TX_Tail = 0;
//	      USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
//	    } 
//	}
}

/*******************************************************************************
* Function Name  : USART2_IRQHandler
* Description    : This function handles USART2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART2_IRQHandler(void)											/*GPRS*/
{
	u8 tmp = 0;
	u16 i=0;
	if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)!=RESET)
	{
		USART_ReceiveData(USART2);
	}
	else if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{  
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);     /*清除中断标志 */
		tmp = USART_ReceiveData(USART2);
		RS485TX;
		for(i=0;i<500;i++);
		USART_SendOneData(USART2,tmp);
		for(i=0;i<5000;i++);
		RS485RX;
	}
//	else if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)	/*中断发送*/
//	{
//		USART_SendData(USART2, UART2TX_Buf[UART2TX_Head++]);
//	    if(UART2TX_Head == UART2TX_Tail)
//	    {
//	      /* Disable the USART1 Transmit interrupt */
//		  UART2TX_Head = 0;
//		  UART2TX_Tail = 0;
//	      USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
//	    } 
//	}	
}

/*******************************************************************************
* Function Name  : USART3_IRQHandler
* Description    : This function handles USART3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART3_IRQHandler(void)											  /*数传电台*/
{
	u8 tmp = 0;
	if(USART_GetFlagStatus(USART3,USART_FLAG_ORE)!=RESET)
	{
		USART_ReceiveData(USART3);
	}
	else if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{  
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);     /*清除中断标志 */
		tmp = USART_ReceiveData(USART3);
		GPRSCom_Handle(tmp);
//		USART3_Handle(tmp);    /*串口3数据处理函数*/
	}	
}

/*******************************************************************************
* Function Name  : EXTI15_10_IRQHandler
* Description    : This function handles External lines 15 to 10 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI15_10_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : RTCAlarm_IRQHandler
* Description    : This function handles RTC Alarm interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTCAlarm_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : USBWakeUp_IRQHandler
* Description    : This function handles USB WakeUp interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USBWakeUp_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM8_BRK_IRQHandler
* Description    : This function handles TIM8 Break interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM8_BRK_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM8_UP_IRQHandler
* Description    : This function handles TIM8 overflow and update interrupt 
*                  request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM8_UP_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM8_TRG_COM_IRQHandler
* Description    : This function handles TIM8 Trigger and commutation interrupts 
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM8_TRG_COM_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM8_CC_IRQHandler
* Description    : This function handles TIM8 capture compare interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM8_CC_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : ADC3_IRQHandler
* Description    : This function handles ADC3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADC3_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : FSMC_IRQHandler
* Description    : This function handles FSMC global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void FSMC_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : SDIO_IRQHandler
* Description    : This function handles SDIO global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SDIO_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM5_IRQHandler
* Description    : This function handles TIM5 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM5_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : SPI3_IRQHandler
* Description    : This function handles SPI3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI3_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : UART4_IRQHandler
* Description    : This function handles UART4 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UART4_IRQHandler(void)											 /*测量板*/
{
//	u8 tmp = 0;
//	if(USART_GetFlagStatus(UART4,USART_FLAG_ORE)!=RESET)
//	{
//		USART_ReceiveData(UART4);
//	}
//	else if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
//	{  
//		USART_ClearITPendingBit(UART4, USART_IT_RXNE);     /*清除中断标志 */
//		tmp = USART_ReceiveData(UART4);
//		CLCom_Handle(tmp);     /*测量板中断处理*/
//	}
}

/*******************************************************************************
* Function Name  : UART5_IRQHandler
* Description    : This function handles UART5 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UART5_IRQHandler(void)											  /*电能表*/
{
//	u8 tmp = 0;
//	if(USART_GetFlagStatus(UART5,USART_FLAG_ORE)!=RESET)
//	{
//		tmp = USART_ReceiveData(UART5);
//	}
//	else if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
//	{  
//		USART_ClearITPendingBit(UART5,  USART_IT_RXNE);     /*清除中断标志 */
//		tmp=USART_ReceiveData(UART5);
//		if(NO == Flag_PCValueSet)
//		{
//			UART5_Handle(tmp);      /*电能表中断处理*/
//		}
//		else
//		{
//			PCValueSet_Handle(tmp);
//		}
//		
//	}
}

/*******************************************************************************
* Function Name  : TIM6_IRQHandler
* Description    : This function handles TIM6 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM6_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : TIM7_IRQHandler
* Description    : This function handles TIM7 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM7_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA2_Channel1_IRQHandler
* Description    : This function handles DMA2 Channel 1 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA2_Channel1_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA2_Channel2_IRQHandler
* Description    : This function handles DMA2 Channel 2 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA2_Channel2_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA2_Channel3_IRQHandler
* Description    : This function handles DMA2 Channel 3 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA2_Channel3_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : DMA2_Channel4_5_IRQHandler
* Description    : This function handles DMA2 Channel 4 and DMA2 Channel 5
*                  interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA2_Channel4_5_IRQHandler(void)
{
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
