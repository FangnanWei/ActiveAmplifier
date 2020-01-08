#ifndef _Time_H_
#define _Time_H_

#include "stm32f10x.h"

struct Time{
	__IO unsigned int sysTimeMs;
	__IO unsigned int periodMs;
};

void Time_Init(int periodMs);
unsigned int Time_GetSysMs(void);
void Time_SleepMs(unsigned int ms);
void Time_Update(void);//只提供给中断调用

#endif
