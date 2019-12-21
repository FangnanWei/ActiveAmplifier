#ifndef __APP_IWDG_H__
#define __APP_IWDG_H__
#include "stm8s.h"

void clear_iwdg(void);
void init_iwdg(void);

void IWDG_Init(IWDG_InitTypeDef *hIWDG);
void IWDG_Feed(void);
#endif

