#include "Time.h"

//µ¥Àý
static struct Time time;

void Time_Init(int periodMs) 
{
	RCC_ClocksTypeDef RCC_Clocks;

	time.periodMs = periodMs;
	time.sysTimeMs = 0;

	//³õÊ¼»¯...
	/* SystTick configuration: an interrupt every 10ms */
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.SYSCLK_Frequency / 100);
	NVIC_SetPriority (SysTick_IRQn, 1); 
}

unsigned int Time_GetSysMs(void) 
{
	return time.sysTimeMs;
}

void Time_SleepMs(unsigned int ms) 
{
	  __IO unsigned int timeSleepMs;
	  
	timeSleepMs = time.sysTimeMs + ms;  
	 
	while(timeSleepMs > time.sysTimeMs)
	{	  
	}
}

void Time_Update(void) 
{
	time.sysTimeMs += time.periodMs;
}

