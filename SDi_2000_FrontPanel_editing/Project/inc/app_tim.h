#ifndef __APP_TIM_H__
#define __APP_TIM_H__
#include "stm8s.h"

void DelayNop(uint32_t cnt);
void DelayNopMs(uint32_t timeMs);

void TIM3_BSP_Init(TIM3_InitTypeDef *hTIM3);

#endif

