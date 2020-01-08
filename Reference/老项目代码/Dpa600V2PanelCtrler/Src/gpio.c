
#include "gpio.h"
#include "Encoder.h"

void BeepInit() {
	GPIO_Init(GPIOC, GPIO_PIN_7, GPIO_MODE_OUT_PP_HIGH_FAST);
	BeepDisable();
}

void BeepEnableForMs(uint16_t time) {
	BeepEnable();
	Delay(time);
	BeepDisable();
}

