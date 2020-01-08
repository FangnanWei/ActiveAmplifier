#ifndef _APP_BEEP_H_
#define _APP_BEEP_H_
#include "stm8s.h"

void BEEP_BSP_Init(BEEP_InitTypeDef *hBeep);
void BEEP_BSP_SetOn(void);
void BEEP_BSP_SetOff(void);
void BEEP_BSP_SetOnAwhile(uint32_t timeMs);
#endif
