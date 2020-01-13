/**************************************************************************
	File		: led.h
	Author		: TaoLei Zhang
	Date		: 2018-2-5
***************************************************************************/
#ifndef _LED_H_
#define _LED_H_
	
/**************************************************************************
    								include
***************************************************************************/
	
/**************************************************************************
    								macro
***************************************************************************/
#if 0
#define SCENE_LED				LED_11
#define AUDIO_SRC_LED			LED_21
/**************************************************************************
    								typedef
***************************************************************************/
typedef enum tagLedIndex_e
{
	LED_Sys = 0,
	LED_21,
	LED_22,
	LED_11,
	LED_12,
	LED_13,
}LedIndex_e;

/**************************************************************************
    								variables
***************************************************************************/
	
/**************************************************************************
    								functions
***************************************************************************/
void LED_Init(void);
void LedOn(LedIndex_e index);
void LedOff(LedIndex_e index);
void LedReverse(LedIndex_e index);

#endif

void LedsProcess(AP_Inst_t *inst);
void LedsInit(void);

struct Led_Property{
	struct Gpio *gpio;
};

struct Led{
	struct Led_Property pp;
	void (*On)(struct Led *self);
	void (*Off)(struct Led *self);
	void (*Reverse)(struct Led *self);
};

struct Led *Led_Open(struct Gpio *gpio);

#endif

