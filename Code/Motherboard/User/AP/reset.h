/**************************************************************************
	File		: reset.h
	Author		: 
	Date		: 2018-7-4
***************************************************************************/
#ifndef _RESET_H_
#define _RESET_H_
	
/**************************************************************************
    								include
***************************************************************************/
#include "stm32f10x.h"

/**************************************************************************
    								macro
***************************************************************************/
#if 0
#define DSP_RST_Port				GPIOC
#define DSP_RST_Pin					GPIO_Pin_9
#define DspResetPinHigh()			GPIO_SetBits(DSP_RST_Port, DSP_RST_Pin)
#define DspResetPinLow()			GPIO_ResetBits(DSP_RST_Port, DSP_RST_Pin)

#define ADC_RST_Port				GPIOA
#define ADC_RST_Pin					GPIO_Pin_8
#define AdcResetPinHigh()			GPIO_SetBits(ADC_RST_Port, ADC_RST_Pin)
#define AdcResetPinLow()			GPIO_ResetBits(ADC_RST_Port, ADC_RST_Pin)


#define NSD_Port					GPIOA
#define NSD_Pin						GPIO_Pin_7
#define NsdResetPinHigh()			GPIO_SetBits(NSD_Port, NSD_Pin)
#define NsdResetPinLow()			GPIO_ResetBits(NSD_Port, NSD_Pin)
#define NSD_Input()					NsdInput()
#define NSD_Output()				NsdOutput()
#define NSD_InputVal()				GPIO_ReadInputDataBit(NSD_Port, NSD_Pin)


/**************************************************************************
    								typedef
***************************************************************************/
	
/**************************************************************************
    								variables
***************************************************************************/
	
/**************************************************************************
    								functions
***************************************************************************/
void ResetInit(void);
#endif
#include "Gpio.h"

struct Reset{
	struct Gpio *gpio;
	void (*Enable)(struct Reset *self);
	void (*Disable)(struct Reset *self);
	void (*TurnToIn)(struct Reset *self);//转换方向为输入
};

void Reset_Init(void);
struct Reset *Reset_Open(struct Gpio *gpio);


#endif

