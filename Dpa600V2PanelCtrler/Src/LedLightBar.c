#include "LedLightBar.h"
#include "stm8s.h"
#include "I2c.h"
#include "Delay.h"

typedef enum Ht16k33_t {
	Ht16k33_StartCh = 7,
	Ht16k33_EndCh = 4
}Ht16k33;

void LedLightBarInit() {
    unsigned char regAddr = 0x21;
    unsigned char value = 0x1;

	//Enable internal system clock
    I2C_Write(I2cAddr_LedCtrler, regAddr, &value, 0);

	//set ROW/INT pin as ROW
	regAddr = 0xA0;
	value = 0x00;
	I2C_Write(I2cAddr_LedCtrler, regAddr, &value, 0);

	//Dimming set
	regAddr = 0xE1;
	value = 0x01;
	I2C_Write(I2cAddr_LedCtrler, regAddr, &value, 0);

	//Blinking set, no blinking
	regAddr = 0x81;
	value = 0x01;
	I2C_Write(I2cAddr_LedCtrler, regAddr, &value, 0);
	LedLightBarSetAll(0x0);
}

#if 0
uint8_t TestData[6];
void LedLightBarRead() {
	I2C_Read(I2cAddr_LedCtrler, 0x40, TestData, 1);
}
#endif

void LedLightBarSetAll(u8 isAllLight) {
	int i;
    unsigned char regAddr = 0;
	unsigned char value = isAllLight ? 0xFF : 0;
	for (i = Ht16k33_StartCh; i >= Ht16k33_EndCh; i--) {
		regAddr = 2 * i;
		I2C_Write(I2cAddr_LedCtrler, regAddr, &value, 1);
	}
}
#if 0
void LedLightBarClear(u8 channel) {
    unsigned char regAddr = 0;
    unsigned char value = 0;
	regAddr = 2 * (Ht16k33_StartCh - channel);
	I2C_Write(I2cAddr_LedCtrler, regAddr, &value, 1);
}
#endif

void LedLightBarDisplay(u8 channel, signed char level) {
	u8 regAddr = 0;
	u8 value = 0;
	if (level >= -10) {
		value = 0x1F;
	}
	else if (level >= -20 && level < -10) {
		value = 0xF;
	}
	else if (level >= -30 && level < -20) {
		value = 0x7;
	}
	else if (level >= -40 && level < -30) {
		value = 0x3;
	}
	else if (level >= -50 && level < -40) {
		value = 0x1;
	}
	else if (level < -50) {
		value = 0x0;
	}
#if 0
	switch (level) {
	case -6:
		value = 0x1;
		break;
	case -5:
	case -4:
		value = 0x3;
		break;
	case -3:
	case -2:
		value = 0x7;
		break;
	case -1:
		value = 0xF;
		break;
	default:
		if (level < -6) {
			value = 0;
		}
		else {
			value = 0x1F;
		}
		break;
	}
#endif
	switch (channel) {
	case 0:
		regAddr = 0x0E;
		break;
	case 1:
		regAddr = 0x0C;
		break;
	case 2:
		regAddr = 0x0A;
		break;
	case 3:
		regAddr = 0x08;
		break;
	default:
		break;
	}
	if (regAddr)
		I2C_Write(I2cAddr_LedCtrler, regAddr, &value, 1);
}

void LedLevelDisplay(u8 channel, signed char dB) {
	//LedLightBarClear(channel);
	LedLightBarDisplay(channel, dB);
}

void LedLightBarSelfTest(uint16_t delay) {
	signed char i;
#if 0
	for (i = 0; i < 4; i++) {
		LedLightBarDisplay(i, -6);
	}
	Delay(delay);
	for (i = 0; i < 4; i++) {
		LedLightBarDisplay(i, -4);
	}
	Delay(delay);
	for (i = 0; i < 4; i++) {
		LedLightBarDisplay(i, -2);
	}
	Delay(delay);
	for (i = 0; i < 4; i++) {
		LedLightBarDisplay(i, -1);
	}
	Delay(delay);
	for (i = 0; i < 4; i++) {
		LedLightBarDisplay(i, 0);
	}
	Delay(delay);
	for (i = 0; i < 4; i++) {
		LedLightBarDisplay(i, -7);
	}
#endif
	for (i = 0; i < 4; i++) {
		LedLightBarDisplay(i, 0);
	}
}

