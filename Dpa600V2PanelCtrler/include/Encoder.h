/******************************************
                    gpio.h
                    2015-09-08          zhangfei
*******************************************/
#ifndef __GPIO_H__
#define __GPIO_H__
#include "stm8s.h"

#define ENCODER_A     GPIO_PIN_0
#define ENCODER_B     GPIO_PIN_2
#define ENCODER_K     GPIO_PIN_7
#define ENCODER_Port    GPIOD

#define LED_ON(channelPort, channelItem)        GPIO_WriteLow(channelPort,channelItem)
#define LED_OFF(channelPort, channelItem)       GPIO_WriteHigh(channelPort,channelItem)

typedef enum tagTurnDir_e
{
    TURN_Dir_L,
    TURN_Dir_R
}TurnDir_e;

void EncoderEc11Init();
void IncEncoderTurn(TurnDir_e dir);
void DecEncoderTurn(TurnDir_e dir);
unsigned char IsEncoderTurnValid(TurnDir_e dir);
void ScanEncoder();
void EncoderProc();
void EncoderRightPro();
void EncoderLeftPro();
void LedLightBarSelfTest(uint16_t delay);

extern bool EncoderAdjust[4];

#endif
