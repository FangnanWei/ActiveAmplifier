#ifndef __APP_CLK_H__
#define __APP_CLK_H__
#include "stm8s.h"

typedef struct{
	CLK_InitTypeDef CLK_Init;
	bool SwitchSuccess;
}CLK_HandlerTypeDef;


void CLK_Init(CLK_HandlerTypeDef *hCLK);
bool CLK_CheckSwitchSuccessed(CLK_HandlerTypeDef *hCLK);

#endif
