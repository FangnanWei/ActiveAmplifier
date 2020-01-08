#include "app_gpio.h"

#define ENC_UNCHANGE_CNT  10

#define KEY_LONG_PRESS_TIMECNT 5000
#define KEY_SHORT_PRESS_TIMECNT 500

void Power_Init(Power_HandlerTypeDef *hPower)
{
	GPIO_Init(hPower->GpioOn.GPIOx, hPower->GpioOn.Pin, hPower->GpioOn.Mode);
	GPIO_Init(hPower->GpioOff.GPIOx, hPower->GpioOff.Pin, hPower->GpioOff.Mode);
}

void Power_SetLedStatus(Power_HandlerTypeDef *hPower)
{
	if (hPower->IsOn != FALSE)
	{
		GPIO_WriteLow(hPower->GpioOff.GPIOx, hPower->GpioOff.Pin);
		GPIO_WriteHigh(hPower->GpioOn.GPIOx, hPower->GpioOn.Pin);
	}
	else
	{
		GPIO_WriteHigh(hPower->GpioOff.GPIOx, hPower->GpioOff.Pin);
		GPIO_WriteLow(hPower->GpioOn.GPIOx, hPower->GpioOn.Pin);
	}
}

void Encoder_Init(Encoder_HandlerTypeDef *hEncoder)
{
	GPIO_Init(hEncoder->GpioA.GPIOx, hEncoder->GpioA.Pin, hEncoder->GpioA.Mode);
	GPIO_Init(hEncoder->GpioB.GPIOx, hEncoder->GpioB.Pin, hEncoder->GpioB.Mode);

}

void Encoder_Scan(Encoder_HandlerTypeDef *hEncoder)
{
	hEncoder->APinLevel = (GPIO_ReadInputPin(hEncoder->GpioA.GPIOx, hEncoder->GpioA.Pin) != RESET)? TRUE : FALSE;
	hEncoder->BPinLevel = (GPIO_ReadInputPin(hEncoder->GpioB.GPIOx, hEncoder->GpioB.Pin) != RESET)? TRUE : FALSE;

#if 0
	if (hEncoder->NewAction != TRUE)
	{
		if (hEncoder->A_Status != hEncoder->B_Status)
		{
			hEncoder->NewAction = TRUE;
		}
	}
	else
	{
		if (hEncoder->A_Status != hEncoder->B_Status)
		{
			hEncoder->ChangeCnt++;
			hEncoder->UnchangeCnt = 0;
			hEncoder->A_ChangeCnt += hEncoder->A_Status? 1 : -1;
		}
		else
		{
			hEncoder->UnchangeCnt++;
			hEncoder->A_UnChangeCnt += hEncoder->A_Status? 1 : -1; //并非连续  如果 有中断 也应该清零
			if (hEncoder->UnchangeCnt == ENC_UNCHANGE_CNT) //确保引脚处于稳定状态
			{
				if (hEncoder->ChangeCnt > ENC_UNCHANGE_CNT)  //找出最短允许的改变次数
				{
					if (((hEncoder->A_ChangeCnt > 0) && (hEncoder->A_UnChangeCnt > 0)) ||
						((hEncoder->A_ChangeCnt < 0) && (hEncoder->A_UnChangeCnt < 0))) 
					{//A一直不变 说明A先变化
						hEncoder->LeftTurn = TRUE;
					}
					else 
					{
						hEncoder->RightTurn = TRUE;
					}
				}
				hEncoder->UnchangeCnt = 0;
				hEncoder->ChangeCnt = 0;
				hEncoder->A_ChangeCnt = 0;
				hEncoder->A_UnChangeCnt = 0;
				hEncoder->NewAction = FALSE;
			}
		}
	}	
#endif	
	//encoder 扫描逻辑

	/**  第一步：获取稳定的电平状态 记录两线电平**/
	if (hEncoder->APinLevel == hEncoder->BPinLevel)
	{
		hEncoder->PinsStableCnt++;
		if (hEncoder->PinsStableCnt >= 10)
		{
			hEncoder->StableLevel = hEncoder->APinLevel;
			hEncoder->PinsStable = TRUE;
		}
	}

	/**  第二步：获取跳变的状态    比较稳定电平和跳变电平获取方向**/
	if ((hEncoder->PinsStable != FALSE) && (hEncoder->APinLevel != hEncoder->BPinLevel))
	{
		hEncoder->PinsStableCnt = 0;
		hEncoder->PinsStable = FALSE;
		if (hEncoder->StableLevel == hEncoder->APinLevel)
		{
			hEncoder->RightTurn = TRUE;
		}
		else
		{
			hEncoder->LeftTurn = TRUE;
		}
	}
}

void Key_Init(Key_HandlerTypeDef *hKey)
{
	GPIO_Init(hKey->GpioKey.GPIOx, hKey->GpioKey.Pin, hKey->GpioKey.Mode);
}

///////设计一个单按键扫描通用程序
void Key_Scan(Key_HandlerTypeDef *hKey)
{
	if (GPIO_ReadInputPin(hKey->GpioKey.GPIOx, hKey->GpioKey.Pin) == RESET) //按住  SET的值每次是不一样的!!!!
	{ 
		hKey->PressCnt++;

		if (hKey->PressCnt == KEY_LONG_PRESS_TIMECNT) 
		{
			hKey->LongPress = TRUE;
		}
	}
	else 
	{ 
		if ((hKey->PressCnt > KEY_SHORT_PRESS_TIMECNT) && (hKey->PressCnt < KEY_LONG_PRESS_TIMECNT))  //如果按住次数没达到长安，但至少达到短按，算短按
		{
			hKey->ShortPress = TRUE;
		}
		hKey->PressCnt = 0;//释放清零
	}
}

void Key_GeneralSwit_Scan(Keys_TypeDef *hKeys)
{
	uint8_t GenoralKeyIndex = 0;
	for (GenoralKeyIndex = 0; GenoralKeyIndex < GENERAL_SWITCH_KEY_CNT; GenoralKeyIndex++)
	{
		Key_Scan(&hKeys->GeneralSwit[GenoralKeyIndex]);
	}
}
