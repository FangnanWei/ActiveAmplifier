#include "app_clk.h"

void CLK_Init(CLK_HandlerTypeDef *hCLK)
{
	CLK_DeInit();

	CLK_SYSCLKConfig(hCLK->CLK_Init.CPUDIV_Prescaler);

	if (hCLK->CLK_Init.NewClock == CLK_SOURCE_HSI) {
		CLK_SYSCLKConfig(hCLK->CLK_Init.HSIDIV_Prescaler);
	}

	CLK_ClockSwitchConfig(hCLK->CLK_Init.SwitchMode, hCLK->CLK_Init.NewClock, hCLK->CLK_Init.ITState, hCLK->CLK_Init.CurrentClockState);
	CLK_ITConfig(hCLK->CLK_Init.IT, hCLK->CLK_Init.ITState);

}

bool CLK_CheckSwitchSuccessed(CLK_HandlerTypeDef *hCLK)
{
	return hCLK->SwitchSuccess;
}

