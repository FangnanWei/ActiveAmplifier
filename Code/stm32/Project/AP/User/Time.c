#include "Time.h"

//单例
static struct Time time;

void Time_Init(uint32_t periodMs) 
{
	RCC_ClocksTypeDef RCC_Clocks;

	time.periodMs = periodMs;
	time.sysTimeMs = 0;

	//初始化...
	/* SystTick configuration: an interrupt every 10ms */
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.SYSCLK_Frequency / 100);
	NVIC_SetPriority (SysTick_IRQn, 1); 
}

uint32_t Time_GetSysMs(void) 
{
	return time.sysTimeMs;
}

void Time_SleepMs(uint32_t ms) 
{
	  __IO uint32_t timeSleepMs;
	  
	timeSleepMs = time.sysTimeMs + ms;  
	 
	while(timeSleepMs > time.sysTimeMs)
	{	  
	}
}

void Time_Update(void) 
{
	time.sysTimeMs += time.periodMs;
}

