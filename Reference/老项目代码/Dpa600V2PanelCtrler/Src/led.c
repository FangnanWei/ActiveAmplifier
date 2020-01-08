/*****************************************
                                led.c
                                zhangtaolei     2016-1-27
******************************************/

#include "led.h"

void LedInit(void)
{
#if 1
    GPIO_Init(LedGpioPort, LedPowerOffGpioPin | LedPowerOnGpioPin, GPIO_MODE_OUT_PP_HIGH_FAST);

    LedEnable(FALSE);
#endif
}

void LedEnable(bool isPowerOn) {
	if (isPowerOn) {
	    GPIO_WriteHigh(LedGpioPort, LedPowerOffGpioPin);
	    GPIO_WriteLow(LedGpioPort, LedPowerOnGpioPin);
	}
	else {
	    GPIO_WriteHigh(LedGpioPort, LedPowerOnGpioPin);
	    GPIO_WriteLow(LedGpioPort, LedPowerOffGpioPin);
	}
}
