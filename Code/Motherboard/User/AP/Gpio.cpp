#include "Gpio.h"
#include <stdlib.h>
#include <string.h>

void Gpio_Set(struct Gpio *self)
{
	GPIO_SetBits(self->pp.GPIOx, self->pp.initStruct.GPIO_Pin);
}

void Gpio_Reset(struct Gpio *self)
{
	GPIO_ResetBits(self->pp.GPIOx, self->pp.initStruct.GPIO_Pin);
}

void Gpio_Reverse(struct Gpio *self)
{
	GPIO_WriteBit(self->pp.GPIOx, self->pp.initStruct.GPIO_Pin, (BitAction)!GPIO_ReadOutputDataBit(self->pp.GPIOx, self->pp.initStruct.GPIO_Pin));
}

uint8_t Gpio_Read(struct Gpio *self)
{
	return GPIO_ReadInputDataBit(self->pp.GPIOx, self->pp.initStruct.GPIO_Pin);
}

struct Gpio *Gpio_Open(GPIO_TypeDef* GPIOx, uint16_t pin, GPIOSpeed_TypeDef speed, GPIOMode_TypeDef mode, FunctionalState highEnable)
{
	struct Gpio *gpio = (struct Gpio *)malloc(sizeof(struct Gpio));

	gpio->pp.GPIOx = GPIOx;
	gpio->pp.initStruct.GPIO_Pin = pin;
	gpio->pp.initStruct.GPIO_Speed = speed;
	gpio->pp.initStruct.GPIO_Mode = mode;

	gpio->pp.highEnable = highEnable;
	gpio->Set = Gpio_Set;
	gpio->Reset = Gpio_Reset;
	gpio->Reverse = Gpio_Reverse;
	gpio->Read = Gpio_Read;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_Init(gpio->pp.GPIOx, &gpio->pp.initStruct);

	return gpio;
}
