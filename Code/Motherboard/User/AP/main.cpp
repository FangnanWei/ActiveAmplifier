/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
//#include "absacc.h"
#include "stdlib.h"
#include "stdio.h"

#include "common_def.h"
#include "dsp.h"
#include "uart.h"
#include "comfifo.h"
#include "eeprom.h"
#include "Scene.h"
#include "Adc.h"
#include "iap.h"
#include "I2C.h"
#include "led.h"
#include "flash_if.h"
#include "key.h"
#include "McuMsg.h"
#include "packet.h"
#include "reset.h"
#include "tpa5050.h"
#include "uart.h"
#include "ADAU1701-gpio-test_IC_1.h"
#include "Time.h"


const char SoftVersion[]  = {" 1.0.1 Build @ "__DATE__" "__TIME__};
const char *_BuildDate		= __DATE__;
const char *_BuildTime		= __TIME__;
const char *_SoftVersion	= SoftVersion;

//#pragma message("Compiling Date:"__DATE__" Time:"__TIME__" version:"SoftVersion)

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
	 	

#define SYSTEMTICK_PERIOD_MS  10

/* Private variables ---------------------------------------------------------*/
uint8_t g_TestBuf[32] = {0};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/**
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
  /* Set the Vector Table base location at 0x08010000 */
	//由于指定了中断向量表的地址，为10000，导致中断向量无法触发，引发问题。可能在IAP/APP结合时候才有作用
 // NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x10000);

  /* 2 bit for pre-emption priority, 2 bits for subpriority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
}

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	AP_Inst_t *inst;
	
	McuMsg_t msg_rx; 
	uint32_t keyTime = 0;
	uint32_t adcTime = 0;

	SystemInit();
	NVIC_Configuration();
	SEI();//开中断
	
	//Time_Init(SYSTEMTICK_PERIOD_MS);
	SysTime::Init(SYSTEMTICK_PERIOD_MS);
	//LED好像必须在此初始化
	
	SEI();//开中断

	inst = AP_Init();
	LedsInit();
	
	Reset_Init();
	InitComFifo(&g_ComFifo);
	InitComFifo(&g_ComFifo_2);
	USART1_Configuration();
	USART2_Configuration();
	Key_Init();
	default_download_IC_1();
	SysTime::SleepMs(50);

	//场景初始化
	inst->scene = Scene_Open();
	inst->scene->Load(inst->scene, DSP_SCENE_Default);
	while(1)
	{
		//uint16_t len;
		/* 上报处理 */
#if 0				
		/* 接收处理      UI通信的是UART1 */
		while (GetMcuMsg(&g_ComFifo, &msg_rx)) 
		{
			if(inst->msg[msg_rx.header.msgType])
				inst->msg[msg_rx.header.msgType](inst, &msg_rx);
		}

		//与STM8通信的是 UART2，
		while (GetMcuMsg(&g_ComFifo_2, &msg_rx)) 
		{
			if(inst->msg[msg_rx.header.msgType])
				inst->msg[msg_rx.header.msgType](inst, &msg_rx);
		}
#endif

		if (SysTime::GetMs() - keyTime >= KEY_SCAN_Time)
		{
			keyTime = SysTime::GetMs();
			Key_Deal(inst);
			
		}
		if (SysTime::GetMs() - adcTime >= ADC_SCAN_Time)
		{
			adcTime = SysTime::GetMs();
			Adc_Deal(inst);
		}
		
		LedsProcess(inst);
		
	}

}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
