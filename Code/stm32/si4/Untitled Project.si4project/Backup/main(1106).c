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
#include "absacc.h"
#include "stdlib.h"
#include "stdio.h"

#include "common_def.h"
#include "dsp.h"
#include "uart.h"
#include "comfifo.h"
#include "eeprom.h"
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


const char SoftVersion[] __at(USER_FLASH_FIRST_PAGE_ADDRESS+1024) = {" 1.0.0 Build @ "__DATE__" "__TIME__};
const char *_BuildDate		= __DATE__;
const char *_BuildTime		= __TIME__;
const char *_SoftVersion	= SoftVersion;

#pragma message("Compiling Date:"__DATE__" Time:"__TIME__" version:"SoftVersion)

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
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x10000);

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
	//int ret = 0;
	McuMsg_t msg_rx; 
	//uint32_t runTime = 0;
	uint32_t keyTime = 0;
	

	SystemInit();
	NVIC_Configuration();
	SEI();
	
	Time_Init(SYSTEMTICK_PERIOD_MS);
	
	LED_Init();
	SEI();
	 
	I2C_Configuration();
	ResetInit();
	InitComFifo(&g_ComFifo);
	InitComFifo(&g_ComFifo_2);
	USART1_Configuration();
	USART2_Configuration();
	KeyInit();
	TPA_Init(TPA_5050_Address);
	//TPA_SetDelay(TPA_5050_Address, 0x30, LEFT_DELAY);
	//TPA_SetDelay(TPA_5050_Address, 0x30, RIGHT_DELAY);
	//TPA_GetDelay(RIGHT_DELAY);
	default_download_IC_1();
	Time_SleepMs(50);
	LoadScene(DSP_MODE_Default);
#if 0
	Delay(1000);
	memset(g_TestBuf, 0xff, 8);
	Read24C256(EEPROM_SLAVE_Addr, 0, g_TestBuf, 8);
	Delay(10);
	g_TestBuf[0] = 10;
	g_TestBuf[1] = 11;
	g_TestBuf[2] = 12;
	g_TestBuf[3] = 13;
	Write24C256(EEPROM_SLAVE_Addr, 0, g_TestBuf, 8);
	memset(g_TestBuf, 0xff, 8);
	Read24C256(EEPROM_SLAVE_Addr, 0, g_TestBuf, 8);
	memset(g_TestBuf, 0xff, 8);
	I2C_Read(I2C2, TPA_5050_Address, 1, g_TestBuf, 8);
	memset(g_TestBuf, 0xff, 8);
	ReadDSP(0, g_TestBuf, 16);
#endif

	while(1)
	{
		//uint16_t len;
		/* 上报处理 */
				
		/* 接收处理 */
		while (GetMcuMsg(&g_ComFifo, &msg_rx)) 
		{
			switch(msg_rx.header.msgType)
			{
				case MSG_EEPROM_Read:
					{
						MsgE2prom_t *pMsgE2prom = (MsgE2prom_t*)(msg_rx.content.data);
						Read24C256(EEPROM_SLAVE_Addr, 
									pMsgE2prom->address, 
									&msg_rx.content.data[sizeof(MsgE2prom_t)], 
									pMsgE2prom->len);
						SendPacket((uint8_t*)pMsgE2prom, sizeof(MsgE2prom_t)+pMsgE2prom->len, DI_Ok, MSG_EEPROM_Read, msg_rx.header.reqId);
					}
					break;
				case MSG_EEPROM_Write:
					{
						MsgE2prom_t *pMsgE2prom = (MsgE2prom_t*)(msg_rx.content.data);
						Write24C256(EEPROM_SLAVE_Addr, 
									pMsgE2prom->address, 
									&msg_rx.content.data[sizeof(MsgE2prom_t)], 
									pMsgE2prom->len);
						SendPacket((uint8_t*)pMsgE2prom, sizeof(MsgE2prom_t), DI_Ok, MSG_EEPROM_Write, msg_rx.header.reqId);
					}
					break;
				case MSG_DSP_Read:
					{
						MsgDsp_t *pMsgDsp = (MsgDsp_t*)(msg_rx.content.data);
						ReadDSP(pMsgDsp->address, 
								&msg_rx.content.data[sizeof(MsgDsp_t)], 
								pMsgDsp->len);
						SendPacket((uint8_t*)pMsgDsp, sizeof(MsgDsp_t)+pMsgDsp->len, DI_Ok, MSG_DSP_Read, msg_rx.header.reqId);
					}
					break;
					
				case MSG_DSP_Write:
					{
						MsgDsp_t *pMsgDsp = (MsgDsp_t*)(msg_rx.content.data);
						SIGMA_WRITE_REGISTER_BLOCK(DSP_SLAVE_Addr, 
												pMsgDsp->address,
												pMsgDsp->len,
												&msg_rx.content.data[sizeof(MsgDsp_t)]);
						SendPacket((uint8_t*)pMsgDsp, sizeof(MsgDsp_t), DI_Ok, MSG_DSP_Write, msg_rx.header.reqId);
					}
					break;
					
				case MSG_DSP_Safeload:
					{
						MsgDsp_t *pMsgDsp = (MsgDsp_t*)(msg_rx.content.data);
						SIGMA_SAFELOAD_WRITE_REGISTER(DSP_SLAVE_Addr, 
												pMsgDsp->address,
												pMsgDsp->len,
												&msg_rx.content.data[sizeof(MsgDsp_t)]);
						SendPacket((uint8_t*)pMsgDsp, sizeof(MsgDsp_t), DI_Ok, MSG_DSP_Safeload, msg_rx.header.reqId);
					}
					break;

				case MSG_DSP_TopoHeader:
					{
						DSP_TopoHeader_t *pHeader = GetDSPTopoHeader();
						SendPacket((uint8_t*)pHeader, sizeof(DSP_TopoHeader_t), DI_Ok, MSG_DSP_TopoHeader, msg_rx.header.reqId);
					}
					break;

				case MSG_DSP_WriteWithExt:
					{
						MsgDspExt_t *pMsgDspExt = (MsgDspExt_t*)(msg_rx.content.data);
						#if 0
						SIGMA_WRITE_REGISTER_BLOCK(DSP_SLAVE_Addr, 
												pMsgDspExt->address,
												pMsgDspExt->len,
												&msg_rx.content.data[sizeof(pMsgDspExt)]);
						#endif
						LoadMsgDspExt(pMsgDspExt, True);
						SendPacket((uint8_t*)pMsgDspExt, sizeof(MsgDspExt_t), DI_Ok, MSG_DSP_WriteWithExt, msg_rx.header.reqId);
					}
					break;

				case MSG_DSP_SafeloadWithExt:
					{
						MsgDspExt_t *pMsgDspExt = (MsgDspExt_t*)(msg_rx.content.data);
						LoadMsgDspExt(pMsgDspExt, True);
						#if 0
						SIGMA_SAFELOAD_WRITE_REGISTER(DSP_SLAVE_Addr, 
												pMsgDspExt->address,
												pMsgDspExt->len,
												&msg_rx.content.data[sizeof(pMsgDspExt)]);
						#endif
						SendPacket((uint8_t*)pMsgDspExt, sizeof(MsgDspExt_t), DI_Ok, MSG_DSP_SafeloadWithExt, msg_rx.header.reqId);
					}
					break;

				case MSG_DSP_SCENE_Change:
					{
						uint8_t mode = msg_rx.content.data[0];
						LoadScene(mode);
						SendPacket(&mode, 1, DI_Ok, MSG_DSP_SCENE_Change, msg_rx.header.reqId);
					}
					break;
					
				case MSG_MCU_Reboot:
					{
						uint32_t IAPFlag = IAP_FAIL;

						SendPacket(NULL, 0, DI_Ok, MSG_MCU_Reboot, msg_rx.header.reqId);
						FLASH_If_Init();
						FLASH_CFG_Erase((uint32_t)UPDATE_FLASH_FLAG_ADDRESS);
						FLASH_CFG_Write((int32_t)UPDATE_FLASH_FLAG_ADDRESS, &IAPFlag, sizeof(uint32_t));
						FLASH_if_UnInit();
						I2C_DeInit(I2C1);
						USART_DeInit(USART1);
						USART_DeInit(USART2);
						CLI();
						Jmp2Boot();
					}
					break;

				case MSG_DSP_TOPO_UpdateMode:
					{
						uint32_t DSPIAPFlag = IAP_FAIL;

						SendPacket(NULL, 0, DI_Ok, MSG_DSP_TOPO_UpdateMode, msg_rx.header.reqId);
						FLASH_If_Init();
						FLASH_CFG_Erase((uint32_t)UPDATE_DSP_TOPO_FLASH_FLAG_ADDRESS);
						FLASH_CFG_Write((int32_t)UPDATE_DSP_TOPO_FLASH_FLAG_ADDRESS, &DSPIAPFlag, sizeof(uint32_t));
						FLASH_if_UnInit();
						
						while(1)
						{
							EraseDSP_Topo();
							DSP_TopoUpgrade(&g_ComFifo);
						}
						
					}
					break;

				case MSG_VERSION_Get:
					{
						SendPacket((uint8_t*)_SoftVersion, strlen(_SoftVersion) + 1, DI_Ok, MSG_VERSION_Get, msg_rx.header.reqId);
					}
					break;	

			}
			
		}

		if (Time_GetSysMs() - keyTime >= KEY_SCAN_Time)
		{
			keyTime = Time_GetSysMs();
			KeyScan();
		}
		LedProcess();
		
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
