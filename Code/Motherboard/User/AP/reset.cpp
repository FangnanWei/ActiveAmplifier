/**************************************************************************
	File		: reset.c
	Author		: 
	Date		: 2018-7-4
***************************************************************************/
	
/**************************************************************************
    								include
***************************************************************************/
#include "reset.h"
#include "Time.h"
#include <stdlib.h>
#include <string.h>

/**************************************************************************
    								macro
***************************************************************************/
	
/**************************************************************************
    								typedef
***************************************************************************/
	
/**************************************************************************
    								variables
***************************************************************************/
	
/**************************************************************************
    								functions
***************************************************************************/
#if 0
void NsdOutput()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = NSD_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(NSD_Port, &GPIO_InitStructure);
}

void NsdInput()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = NSD_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(NSD_Port, &GPIO_InitStructure);
}

void ResetInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = DSP_RST_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(DSP_RST_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = ADC_RST_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(ADC_RST_Port, &GPIO_InitStructure);
	
	NSD_Output();

	DspResetPinLow();
	AdcResetPinLow();
	NsdResetPinLow();

	Time_SleepMs(10);
	DspResetPinHigh();
	Time_SleepMs(30);
	AdcResetPinHigh();
	Time_SleepMs(260);
	NsdResetPinHigh();
	NSD_Input();
}
#endif

struct Reset *resetDsp;
struct Reset *resetAdc;
struct Reset *resetNsd;

void Reset_Init(void)
{
	resetDsp = Reset_Open(Gpio_Open(GPIOC, GPIO_Pin_9, GPIO_Speed_50MHz, GPIO_Mode_Out_PP, ENABLE));
	resetAdc = Reset_Open(Gpio_Open(GPIOA, GPIO_Pin_8, GPIO_Speed_50MHz, GPIO_Mode_Out_PP, ENABLE));
	resetNsd = Reset_Open(Gpio_Open(GPIOA, GPIO_Pin_7, GPIO_Speed_50MHz, GPIO_Mode_Out_OD, ENABLE));

	resetDsp->Enable(resetDsp);
	resetAdc->Enable(resetAdc);
	resetNsd->Enable(resetNsd);

	SysTime::SleepMs(10);
	resetDsp->Disable(resetDsp);
	SysTime::SleepMs(30);
	resetAdc->Disable(resetAdc);
	SysTime::SleepMs(260);
	resetNsd->Disable(resetNsd);

	resetNsd->TurnToIn(resetNsd);
}

void Reset_Enable(struct Reset *self)
{
	struct Gpio *gpio = self->gpio;
	gpio->Reset(gpio);
}

void Reset_Disable(struct Reset *self)
{
	struct Gpio *gpio = self->gpio;
	gpio->Set(gpio);
}

void Reset_TurnToIn(struct Reset *self)
{
	struct Gpio *newGpio;
	struct Gpio *gpio = self->gpio;
	gpio->pp.initStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;

	newGpio = Gpio_Open(gpio->pp.GPIOx, gpio->pp.initStruct.GPIO_Pin, gpio->pp.initStruct.GPIO_Speed, gpio->pp.initStruct.GPIO_Mode, ENABLE);
	memcpy(gpio, newGpio, sizeof(struct Gpio));
	free(newGpio);
}

struct Reset *Reset_Open(struct Gpio *gpio)
{
	struct Reset *reset = (struct Reset *)malloc(sizeof(struct Reset));

	reset->gpio = gpio;
	reset->Enable = Reset_Enable;
	reset->Disable = Reset_Disable;
	reset->TurnToIn = Reset_TurnToIn;

	return reset;
}


