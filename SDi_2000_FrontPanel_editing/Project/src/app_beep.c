#include "app_beep.h"
#include "app_tim.h"

void BEEP_BSP_Init(BEEP_InitTypeDef *hBeep)
{
	BEEP_LSICalibrationConfig(hBeep->LSIFreqHz);
	BEEP_Init(hBeep->Frequency);
	
	BEEP_BSP_SetOff();
}

void BEEP_BSP_SetOn(void)
{
	BEEP_Cmd(ENABLE);
}

void BEEP_BSP_SetOff(void)
{
	BEEP_Cmd(DISABLE);
}

void BEEP_BSP_SetOnAwhile(uint32_t timeMs)
{
	BEEP_Cmd(ENABLE);
	DelayNopMs(timeMs);
	BEEP_Cmd(DISABLE);
}


