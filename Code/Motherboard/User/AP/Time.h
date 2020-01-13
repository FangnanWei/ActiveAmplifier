#ifndef _Time_H_
#define _Time_H_

#include "stm32f10x.h"

class SysTime{
public:
	static void Init(uint32_t periodMs);
	static uint32_t GetMs();
	static void SleepMs(uint32_t ms);
	static void Update();

private:
	static __IO uint32_t sysTimeMs_;
	static __IO uint32_t periodMs_;
};

#endif
