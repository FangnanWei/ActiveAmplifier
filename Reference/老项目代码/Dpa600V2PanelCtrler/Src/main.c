/*****************************************
                        main.c
                        zhangtaolei     2016-1-27
******************************************/

/*****************************************
                        include
******************************************/
#include "led.h"
#include "gpio.h"
#include "stm8s_it.h"
#include "Lcd.h"
#include "LedLightBar.h"
#include "Timer.h"
#include "I2c.h"
#include "Encoder.h"
#include "Key.h"
#include "stdlib.h"
#include "Menu.h"
#include "Uart.h"
#include "24C02.h"
#include "Packet.h"
#include "AlgCfg.h"

void SelfTest();

bool IsRunning = FALSE;
bool IsHomePage = TRUE;
bool IsChSelect[4]	= {FALSE, FALSE, FALSE, FALSE};
bool IsChMute[4]	= {FALSE, FALSE, FALSE, FALSE};
signed char OutGainVals[4] = {0, 0, 0, 0};
#if 0
const GpioId SwitchGpioIds[MaxSwitchNum] = {
	{GPIOB, GPIO_PIN_0},
	{GPIOB, GPIO_PIN_1},
	{GPIOB, GPIO_PIN_2},
	{GPIOB, GPIO_PIN_3},
	{GPIOF, GPIO_PIN_4}
};

const GpioId EncoderKeyGpioId = {ENCODER_Port, ENCODER_K};
#endif

const KeyPrmt SwiKeyPrmts[MaxSwitchNum] = {
	{{GPIOB, GPIO_PIN_0}, KeyId_Sw1, SwiKeyPressFunc, SwiKeyPressLongFunc},
	{{GPIOB, GPIO_PIN_1}, KeyId_Sw2, SwiKeyPressFunc, SwiKeyPressLongFunc},
	{{GPIOB, GPIO_PIN_2}, KeyId_Sw3, SwiKeyPressFunc, SwiKeyPressLongFunc},
	{{GPIOB, GPIO_PIN_3}, KeyId_Sw4, SwiKeyPressFunc, SwiKeyPressLongFunc},
	{{GPIOF, GPIO_PIN_4}, KeyId_Psw, NULL, PannelPsw}
};

const KeyPrmt EncoderKeyPrmts = {{ENCODER_Port, ENCODER_K}, KeyId_EncKey, EncoderKeyPressFunc, EncoderKeyPressLongFunc};

Menu *_Menu;

unsigned long scanTime = 0;
unsigned long flashTime = 0;
#if 1
#define TimeCnt 10
int main( void )
{
	int i, outGainTimeCnt = 0, encoderTimeCnt = 0;
	Key *swiKeys[MaxSwitchNum];
	Key *encoderKey = (Key *)malloc(sizeof(Key));
	_Menu = (Menu *)malloc(sizeof(Menu));
	_Menu->curList = NULL;
//    CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSE, ENABLE,CLK_CURRENTCLOCKSTATE_DISABLE);
    CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);
    InitTimer();
	BeepInit();
	Uart2Init();
    enableInterrupts();
	I2cInit();
	LedLightBarInit();
	LedInit();
	LCD_Init();
	EncoderEc11Init();
	PacketInit();
	AlgCfgInit();
	for (i = 0; i < MaxSwitchNum; i++) {
		swiKeys[i] = (Key *)malloc(sizeof(Key));
		KeyInit(swiKeys[i], &SwiKeyPrmts[i]);
	}

	KeyInit(encoderKey, &EncoderKeyPrmts);
	SelfTest();
#if 1
	//LcdMenuDisplay(menu);
	PannelPsw(KeyId_Psw);
	//EncoderKeyPressLongFunc(KeyId_EncKey);
	BeepEnableForMs(BeepRingMs);
	#endif
    while(1)
    {
		if (g_TimeCnt - scanTime >= 5) {
			//encoderTimeCnt++;
			scanTime = g_TimeCnt;
			if (IsRunning) {
#if 1
				if (IsHomePage) {
					for (i = 0; i< 4; i++) {
						KeyScan(swiKeys[i], 1);
					}
				}
#endif
				KeyScan(encoderKey, 1);
				//if (encoderTimeCnt >= 20) {
				//	encoderTimeCnt = 0;
				//}
			}
			KeyScan(swiKeys[4], 1);
		}
		if (IsRunning) {
			ScanEncoder();
			EncoderProc();
		}
#if 1
		if (IsRunning) {
			if (g_TimeCnt - flashTime >= 50) {
				flashTime = g_TimeCnt;
				outGainTimeCnt++;
				if (IsHomePage) {
					for (i = 0; i < 4; i++) {
						LcdGainValDisplay(i, OutGainVals[i]);
						if (outGainTimeCnt >= TimeCnt && EncoderAdjust[i]) {
							PacketTxSubCmd(CmdList_OutGain, i, &OutGainVals[i], 1);
							EncoderAdjust[i] = FALSE;
						}
					}
					if (outGainTimeCnt >= TimeCnt)
						outGainTimeCnt = 0;
				}
				else {
					if (_Menu->curList)
						LcdMenuRefresh(_Menu->curList);
					//LcdMenuDisplay(_Menu);
				}
			}
		}
#endif
 		PacketRx();
   }
}
#endif

void PannelPower(bool isOn) {
	IsHomePage = TRUE;
	LedEnable(isOn);
	//LedEnable(TRUE);
	if (!isOn) {
	    LCD_ClearDisplay();
		LedLightBarSetAll(0);
	}
	else {
		SelfTest();
		LcdHomePageDisplay();
	}
	PacketTx(CmdList_PowerOn, &isOn, 1);
}

void SelfTest() {
	//LedLightBarRead();
	//test24c02();
	//LedLightBarSelfTest(1000);
#if 0
	LcdSelfTest();
#endif

}

#if 0
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
