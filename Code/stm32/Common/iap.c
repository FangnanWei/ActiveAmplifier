/****************************************
                                iap.c
                                zhangtaolei     2015-10-22
*****************************************/

/****************************************
                                include
*****************************************/
#include "common_def.h"
#include "comfifo.h"
#include "flash_if.h"
#include "iap.h"
#include "led.h"
#include "string.h"
#include "packet.h"
#include "Time.h"

/****************************************
                                macro
*****************************************/

/****************************************
                                typedef
*****************************************/
typedef  void (*pFunction)(void);

/****************************************
                                variables
*****************************************/
extern struct Led *ledSys;
McuMsg_t g_msg;

pFunction Jump_To_Bootloader;
pFunction Jump_To_User_Application;
uint32_t Jump_Address;
uint32_t Jump_User_Address;
/****************************************
                                functions
*****************************************/
void Jmp2Boot(void)
{
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x00);
	Jump_Address = *(__IO uint32_t*) (Bootloader_Addr + 4);
	Jump_To_Bootloader = (pFunction) Jump_Address;
	__set_MSP(*(__IO uint32_t*) Bootloader_Addr);
	Jump_To_Bootloader();
}

void Jmp2User()
{
	CLI();			
	/* Jump to user application */
	Jump_User_Address = *(__IO uint32_t*) (USER_FLASH_FIRST_PAGE_ADDRESS + 4);
	Jump_To_User_Application = (pFunction) Jump_User_Address;
	/* Initialize user application's Stack Pointer */
	__set_MSP(*(__IO uint32_t*) USER_FLASH_FIRST_PAGE_ADDRESS);
	Jump_To_User_Application();
}

void SoftReset(void)
{
	CLI();			
	Jump_Address = *(__IO uint32_t*) (Bootloader_Addr + 4);
	Jump_To_Bootloader = (pFunction) Jump_Address;
	__set_MSP(*(__IO uint32_t*) Bootloader_Addr);
	Jump_To_Bootloader();
}

/*
        擦除用户代码
*/
void EraseUserCode(void)
{
    FLASH_If_Init();
    FLASH_If_Erase(USER_FLASH_FIRST_PAGE_ADDRESS, USER_FLASH_LAST_PAGE_ADDRESS, FLASH_PAGE_SIZE);
}

void EraseDSP_Topo(void)
{
	FLASH_If_Init();
	FLASH_If_Erase(DSP_FIRST_PAGE_ADDRESS, DSP_LAST_PAGE_ADDRESS, FLASH_PAGE_SIZE);
}


int BeginUpgrade(ComFifo_t * pComFifo)
{
	static uint32_t runTime = 0;
	uint32_t beginTime = Time_GetSysMs();
	UpgradeState_e state = UPGRADE_Ready;
	__IO unsigned int flashAddr = USER_FLASH_FIRST_PAGE_ADDRESS;

	while(1)
	{
		if (Time_GetSysMs() - runTime >= 100)
        {
            runTime = Time_GetSysMs();
            //LedReverse(LED_Sys);
            ledSys->Reverse(ledSys);
        }

		/* 判断超时 */
		if (UPGRADE_RecvData == state)
		{
			if (Time_GetSysMs() - beginTime > UPGRADE_TIMEOUT)
            {
                SoftReset();
            }
		}

		memset(&g_msg, 0, sizeof(g_msg));

		if (!GetMcuMsg(pComFifo, &g_msg))
		{
			continue;
		}

		if (UPGRADE_Ready == state)
		{
			if (g_msg.header.msgType == MSG_UPGRADE_Begin){
				state = UPGRADE_RecvData;
				beginTime = Time_GetSysMs();
				flashAddr = USER_FLASH_FIRST_PAGE_ADDRESS;
				SendUpgradeAck(g_msg.header.reqId);
			}
			else
			{
				continue;
			}
		}

		if (UPGRADE_RecvData == state){
			beginTime = Time_GetSysMs();		/* 更新时间，不至于超时 */
			switch(g_msg.header.msgType)
			{
				case MSG_UPGRADE_Data:
					{
						uint16_t len = g_msg.header.len;

						len = len + 3;
						len = len/4;
						WriteUserCode(&flashAddr, (uint32_t*)g_msg.content.data, len);
						SendUpgradeAck(g_msg.header.reqId);
					}
					break;

				case MSG_UPGRADE_End:
					{
						uint32_t IAPFlag = IAP_SUCCESS;
						FLASH_If_Init();
						FLASH_CFG_Erase((uint32_t)UPDATE_FLASH_FLAG_ADDRESS);
						FLASH_CFG_Write((int32_t)UPDATE_FLASH_FLAG_ADDRESS, &IAPFlag, sizeof(uint32_t));
						FLASH_if_UnInit();
						SendUpgradeAck(g_msg.header.reqId);
						Jmp2User();
					}
					break;
			}
		}
	}
}

int DSP_TopoUpgrade(ComFifo_t * pComFifo)
{
	static uint32_t runTime = 0;
	uint32_t beginTime = Time_GetSysMs();
	UpgradeState_e state = UPGRADE_RecvData;
	__IO unsigned int flashAddr = DSP_FIRST_PAGE_ADDRESS;

	while(1)
	{
		if (Time_GetSysMs() - runTime >= 100)
        {
            runTime = Time_GetSysMs();
            //LedReverse(LED_Sys);
			ledSys->Reverse(ledSys);
        }

		/* 判断超时 */
		if (UPGRADE_RecvData == state)
		{
			if (Time_GetSysMs() - beginTime > UPGRADE_TIMEOUT)
            {
                SoftReset();
            }
		}

		memset(&g_msg, 0, sizeof(g_msg));

		if (!GetMcuMsg(pComFifo, &g_msg))
		{
			continue;
		}

		if (UPGRADE_RecvData == state)
		{
			beginTime = Time_GetSysMs();		/* 更新时间，不至于超时 */
			switch(g_msg.header.msgType)
			{
				case MSG_UPGRADE_Data:
					{
						uint16_t len = g_msg.header.len;

						len = len + 3;
						len = len/4;
						WriteUserCode(&flashAddr, (uint32_t*)g_msg.content.data, len);
						SendUpgradeAck(g_msg.header.reqId);
					}
					break;

				case MSG_UPGRADE_End:
					{
						uint32_t DSPIAPFlag = IAP_SUCCESS;
						FLASH_If_Init();
						FLASH_CFG_Erase((uint32_t)UPDATE_DSP_TOPO_FLASH_FLAG_ADDRESS);
						FLASH_CFG_Write((int32_t)UPDATE_DSP_TOPO_FLASH_FLAG_ADDRESS, &DSPIAPFlag, sizeof(uint32_t));
						FLASH_if_UnInit();
						SendUpgradeAck(g_msg.header.reqId);
						SoftReset();
					}
					break;
			}
		}
	}
}


