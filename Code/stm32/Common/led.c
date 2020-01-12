/**************************************************************************
	File		: led.c
	Author		: TaoLei Zhang
	Date		: 2018-2-5
***************************************************************************/
	
/**************************************************************************
    								include
***************************************************************************/
#include "common_def.h"
#ifndef __IAP__
#include "eeprom.h"
#include "Scene.h"
#endif
#include "led.h"	
#include "stm32f10x.h"
#include "Time.h"

struct Led *ledSys;
struct Led *ledSceneLED21;
struct Led *led22;
struct Led *ledSceneLED11;
struct Led *led12;
struct Led *led13;

/**************************************************************************
    								macro
***************************************************************************/
	
/**************************************************************************
    								typedef
***************************************************************************/
#ifndef __IAP__
void LedsProcess(AP_Inst_t *inst)
{
	static uint32_t runTime = 0;
	static uint32_t audioInputTime = 0;
	static uint32_t sceneTime = 0;
	static uint32_t sceneLoadingTime = 0;
	static uint32_t sceneLoadingTimeCnt = 0;
	struct Scene *scene = inst->scene;
	
	if (Time_GetSysMs() - runTime >= 500)
    {
        runTime = Time_GetSysMs();
        ledSys->Reverse(ledSys);
    }

	if (scene->pp.loadingLed) 
	{
		if (sceneLoadingTimeCnt == 0) {
			sceneLoadingTimeCnt = Time_GetSysMs();
			//保持一致
			ledSceneLED11->Off(ledSceneLED11);
			ledSceneLED21->Off(ledSceneLED21);
		}

		if (Time_GetSysMs() - sceneTime >= 3000)
		{
			scene->pp.loadingLed = 0;
			sceneLoadingTimeCnt = 0;
		}

		if (Time_GetSysMs() - sceneLoadingTime >= 100){
			sceneLoadingTime = Time_GetSysMs();
			ledSceneLED11->Reverse(ledSceneLED11);
			ledSceneLED21->Reverse(ledSceneLED21);
		}
	}
	else {
		if (Time_GetSysMs() - sceneTime >= 500)
	    {
	    	DSP_Scene_e mode = scene->pp.mode;
			
	        sceneTime = Time_GetSysMs();
			if (mode == DSP_SCENE_A)
			{
				ledSceneLED11->On(ledSceneLED11);
				ledSceneLED21->Off(ledSceneLED21);
			}
			else if (mode == DSP_SCENE_B)
			{
				ledSceneLED11->Off(ledSceneLED11);
				ledSceneLED21->On(ledSceneLED21);
			}
			else
			{
				ledSceneLED11->On(ledSceneLED11);
				ledSceneLED21->On(ledSceneLED21);
			}
	    }
	}	
}
#endif

#include "Gpio.h"
void LedsInit(void)
{
	ledSys = Led_Open(Gpio_Open(GPIOB, GPIO_Pin_0, GPIO_Speed_50MHz, GPIO_Mode_Out_PP, ENABLE));
	ledSceneLED21 = Led_Open(Gpio_Open(GPIOC, GPIO_Pin_7, GPIO_Speed_50MHz, GPIO_Mode_Out_PP, ENABLE));//ED12=LED21
	led22 = Led_Open(Gpio_Open(GPIOC, GPIO_Pin_6, GPIO_Speed_50MHz, GPIO_Mode_Out_PP, ENABLE));
	ledSceneLED11 = Led_Open(Gpio_Open(GPIOB, GPIO_Pin_14, GPIO_Speed_50MHz, GPIO_Mode_Out_PP, ENABLE));//ED11=LED11
	led12 = Led_Open(Gpio_Open(GPIOB, GPIO_Pin_13, GPIO_Speed_50MHz, GPIO_Mode_Out_PP, ENABLE));
	led13 = Led_Open(Gpio_Open(GPIOB, GPIO_Pin_12, GPIO_Speed_50MHz, GPIO_Mode_Out_PP, ENABLE));
}

void Led_On(struct Led *self)
{
	struct Gpio *gpio = self->pp.gpio;
	gpio->Set(gpio);
}

void Led_Off(struct Led *self)
{
	struct Gpio *gpio = self->pp.gpio;
	gpio->Reset(gpio);
}

void Led_Reverse(struct Led *self)
{
	struct Gpio *gpio = self->pp.gpio;
	gpio->Reverse(gpio);
}

struct Led *Led_Open(struct Gpio *gpio)
{
	struct Led *led = (struct Led *)malloc(sizeof(struct Led));

	led->pp.gpio = gpio;
	
	led->On = Led_On;
	led->Off = Led_Off;
	led->Reverse = Led_Reverse;

	return led;
}

