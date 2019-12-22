/*****************************************
                                led.h
                                zhangtaolei     2016-1-27
******************************************/
#ifndef _LED_H_
#define _LED_H_

/*****************************************
                                include
******************************************/
#include "stm8s_conf.h"

#define LedGpioPort		GPIOD
#define LedPowerOffGpioPin	GPIO_PIN_4
#define LedPowerOnGpioPin	GPIO_PIN_3

void LedInit(void);
void LedEnable(bool isPowerOn);
#endif
