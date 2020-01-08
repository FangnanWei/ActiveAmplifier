#include "Delay.h"
#include "stm8s.h"
#include "stm8s_it.h"

void Delay(uint16_t nCount)
{
	unsigned long cnt = g_TimeCnt;

    while(g_TimeCnt - cnt < nCount);
}

void DelayNop() {
	char i;
	for (i = 0; i < 1; i++) {
		asm("nop");
	}
}

