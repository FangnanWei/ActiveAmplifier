#include "app_tim.h"

uint32_t g_time_cnt = 0;

void TIM3_BSP_Init(TIM3_InitTypeDef *hTIM3)
{
	TIM3_TimeBaseInit(hTIM3->Prescaler, hTIM3->Period);
	TIM3_ARRPreloadConfig(hTIM3->ARRPreload);
	TIM3_ITConfig(hTIM3->IT, hTIM3->ITState);
	TIM3_Cmd(hTIM3->Cmd);
}

void DelayNop(uint32_t cnt) {
	while (cnt--)
	{
		nop();
	}
}

void DelayNopMs(uint32_t timeMs)
{
	timeMs *= 190;
	while (timeMs--) 
	{
		nop();
	}
}

