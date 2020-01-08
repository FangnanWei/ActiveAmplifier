#ifndef _Adc_H_
#define _Adc_H_

#include "Gpio.h"
#include "McuMsg.h"

#define ADC_SCAN_Time       200

struct Adc_Property{
	struct Gpio *gpio;
	ADC_InitTypeDef ADC_InitStruct;
	DMA_InitTypeDef DMA_InitStruct;
	uint16_t advalue;
};

struct Adc{
	struct Adc_Property pp;
	uint16_t (*GetValue)(struct Adc *self);
};

void Adc_Deal(AP_Inst_t *inst);
struct Adc *Adc_Open(void);

#endif
