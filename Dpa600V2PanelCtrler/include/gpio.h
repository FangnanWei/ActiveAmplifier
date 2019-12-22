/*****************************************
                                gpio.h
                                zhangtaolei     2016-10-17
******************************************/
#ifndef _GPIO_H_
#define _GPIO_H_

#include "stm8s_conf.h"

typedef struct GpioId_t {
	GPIO_TypeDef *port;
	GPIO_Pin_TypeDef pinNum;
}GpioId;

#define BeepGpioGroup GPIOC
#define BeepGpioPinOffset GPIO_PIN_7
#define BeepRingMs 100

#define BeepEnable() GPIO_WriteLow(BeepGpioGroup, BeepGpioPinOffset);
#define BeepDisable() GPIO_WriteHigh(BeepGpioGroup, BeepGpioPinOffset);

void BeepInit();
void BeepEnableForMs(uint16_t time);
#endif
