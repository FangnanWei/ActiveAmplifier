#include "Time.h"

__IO uint32_t SysTime::sysTimeMs_ = 0;
__IO uint32_t SysTime::periodMs_  = 0;

void SysTime::Init(uint32_t periodMs) {
	RCC_ClocksTypeDef RCC_Clocks;

	periodMs_ = periodMs;
	sysTimeMs_ = 0;

	//初始化...
	/* SystTick configuration: an interrupt every 10ms */
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.SYSCLK_Frequency / 100);
	NVIC_SetPriority (SysTick_IRQn, 1); 

}

uint32_t SysTime::GetMs() { 
	return sysTimeMs_;
}

void SysTime::SleepMs(uint32_t ms) {
	 __IO uint32_t timeSleepMs;
	  
	timeSleepMs = sysTimeMs_ + ms;  
	 
	while(timeSleepMs > sysTimeMs_)
	{	  
	}
}

void SysTime::Update() {
	sysTimeMs_ += periodMs_;
}

