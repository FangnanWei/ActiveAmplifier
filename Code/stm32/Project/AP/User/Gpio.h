#ifndef _Gpio_H_
#define _Gpio_H_

#include "stm32f10x.h"

struct Gpio_Property{
	GPIO_TypeDef* GPIOx;
	GPIO_InitTypeDef initStruct;
	
	FunctionalState highEnable;
};

struct Gpio{
	struct Gpio_Property pp;
	void (*Set)(struct Gpio *self);
	void (*Reset)(struct Gpio *self);
	void (*Reverse)(struct Gpio *self);

	uint8_t (*Read)(struct Gpio *self);
};

struct Gpio *Gpio_Open(GPIO_TypeDef* GPIOx, uint16_t pin, GPIOSpeed_TypeDef speed, GPIOMode_TypeDef mode, FunctionalState highEnable);

#endif

