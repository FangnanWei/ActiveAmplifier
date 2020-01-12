#ifndef _Time_H_
#define _Time_H_

#include "stm32f10x.h"

struct Time{
	__IO uint32_t sysTimeMs;
	__IO uint32_t periodMs;
};

void Time_Init(uint32_t periodMs);
uint32_t Time_GetSysMs(void);
void Time_SleepMs(uint32_t ms);
void Time_Update(void);//只提供给中断调用

#endif
