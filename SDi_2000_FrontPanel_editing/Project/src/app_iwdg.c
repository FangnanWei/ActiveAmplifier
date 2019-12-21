#include "app_iwdg.h"

void IWDG_Init(IWDG_InitTypeDef *hIWDG)
{
	if (RST_GetFlagStatus(RST_FLAG_IWDGF) != RESET)
	{
	  RST_ClearFlag(RST_FLAG_IWDGF);
	}

	IWDG_Enable();
	IWDG_WriteAccessCmd(hIWDG->WriteAccess);
	IWDG_SetPrescaler(hIWDG->Prescaler); //128K/256  500Hz
	IWDG_SetReload(hIWDG->Reload); 
	IWDG_ReloadCounter();
}

void IWDG_Feed(void)
{
	IWDG_ReloadCounter();
}

