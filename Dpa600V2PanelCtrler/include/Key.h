#ifndef __KEY_H
#define __KEY_H

#include "stm8s.h"
#include "Gpio.h"

#ifdef __cplusplus
extern "C" {
#endif  /*  __cplusplus                 */

#define MaxSwitchNum 5
#define  LONG_PRESS_TIMEOUT		300

typedef enum {
	KeyId_Sw1,
	KeyId_Sw2,
	KeyId_Sw3,
	KeyId_Sw4,
	KeyId_Psw,
	KeyId_EncKey
} KeyId;

typedef struct KeyPrmt_t {
	GpioId gpioId;
	KeyId keyId;
	void (*keyPressFunc)(KeyId keyId);
	void (*keyPresslongFunc)(KeyId keyId);
}KeyPrmt;

typedef struct Key_t {
	GpioId *gpioId_;
	//bool isPressed_;
	bool pressEvent_;
	bool releaseEvent_;
	unsigned int pressedTimeMs_;	/* 按键按下时间 */

	void (*keyPressFunc)(KeyId keyId);
	void (*keyPresslongFunc)(KeyId keyId);
	KeyId keyId;
}Key;

void KeyInit(Key *key, const KeyPrmt *keyPrmt);
void KeyScan(Key *key, unsigned int intervalMs);
void EncoderKeyPressFunc(KeyId keyId);
void EncoderKeyPressLongFunc(KeyId keyId);
void SwiKeyPressLongFunc(KeyId keyId);
void SwiKeyPressFunc(KeyId keyId);
void PannelPsw(KeyId keyId);


extern bool IsLeafSel;

#ifdef __cplusplus
}
#endif	 /*  __cplusplus*/


#endif//__KEY_H

