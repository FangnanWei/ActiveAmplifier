/**
  ******************************************************************************
  * @file    Project/main.c 
  * @author  MCD Application Team
  * @version V2.3.0
  * @date    16-June-2017
  * @brief   Main program body
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 


/* Includes ------------------------------------------------------------------*/
extern "C" {
#include "stm8s.h"
#include "stdlib.h"
}
#include "St/Gpio.h"
#include "St/Sys.h"
#include "Chip/Eeprom.h"
#include "Chip/Oled.h"
#include "Chip/Key.h"
#include "Chip/RotaryEncoder.h"

/* Private defines -----------------------------------------------------------*/
const uint8_t EepronAddress = 0xA8;
const uint16_t EepronPageSize = 64;
const uint16_t EepronPages = 512;
/*
   24C256资料
   容量：32768Byte
         256Kbit
         32KByte
         512Page
         64Byte per Page     
*/

GpioParam GpioLed01Param(GPIOC, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST, false);
GpioParam GpioLed02Param(GPIOC, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_FAST, false);
GpioParam GpioLed03Param(GPIOC, GPIO_PIN_2, GPIO_MODE_OUT_PP_LOW_FAST, false);

GpioParam GpioRestKeyIoParam(GPIOE, GPIO_PIN_5, GPIO_MODE_IN_PU_NO_IT, false);
GpioParam GpioEcKeyIoParam(GPIOC, GPIO_PIN_7, GPIO_MODE_IN_PU_NO_IT, false);

GpioParam GpioEcAIoParam(GPIOC, GPIO_PIN_5, GPIO_MODE_IN_PU_NO_IT, false);
GpioParam GpioEcBIoParam(GPIOC, GPIO_PIN_6, GPIO_MODE_IN_PU_NO_IT, false);

OledParam OledSpdParam(GPIOD, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_FAST, true, GPIOD, GPIO_PIN_2, GPIO_MODE_OUT_PP_LOW_FAST, true, 0x78);                          
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void main(void)
{
	Sys::CpuInit();

	enableInterrupts();

	Gpio *led01_ =  new Gpio(GpioLed01Param);
	led01_->Init();

	Gpio *led02_ = new Gpio(GpioLed02Param);
	led02_->Init();

	Gpio *led03_ = new Gpio(GpioLed03Param);
	led03_->Init();

	Oled *oled = new Oled(OledSpdParam);
	oled->Init();
	#if 1 
	Eeprom *eeprom = new Eeprom(EepronAddress, true, EepronPageSize, EepronPageSize * EepronPages);

	eeprom->Test();
	#endif

	Key *resetKey = new Key(GpioEcKeyIoParam, 10000);
	RotaryEncoder *enc = new RotaryEncoder(GpioEcAIoParam, GpioEcBIoParam);
	RotaryEncoderType encType = RotaryEncoder_None;


	/* Infinite loop */
	Sys::BeepRingForMs(100);

	const uint8_t LedCnt = 3;
	int i = 0;
	bool ledState[LedCnt] = {true, false, false};
	bool tempLedState[LedCnt]; 

	while (1)
	{
#if 0    
		led01_->Enable();
		Sys::DelaySecond(1);
		led01_->Disable();
		Sys::DelaySecond(1);
#endif
		resetKey->Scan();
		encType = enc->Scan();
		if (encType == RotaryEncoder_LeftTurn) {
			//左滚动
			for (i = 0; i < LedCnt; i++) {
				tempLedState[i] = ledState[(i + 1)%LedCnt];
			}
			for (i = 0; i < LedCnt; i++) {
			   ledState[i] = tempLedState[i];
			   if (i == 0) {
					led01_->SetOut(!ledState[i]);
			   }
			   else if (i == 1) {
					led02_->SetOut(!ledState[i]);
			   }
			   else if (i == 2) {
					led03_->SetOut(!ledState[i]);
			   }
			}
		}
		if (encType == RotaryEncoder_RightTurn) {
		//右边滚动
			for (i = 0; i < LedCnt; i++) {
			   tempLedState[(i + 1)%LedCnt] = ledState[i];
			}
			for (i = 0; i < LedCnt; i++) {
			   ledState[i] = tempLedState[i];
			   if (i == 0) {
					led01_->SetOut(!ledState[i]);
			   }
			   else if (i == 1) {
					led02_->SetOut(!ledState[i]);
			   }
			   else if (i == 2) {
					led03_->SetOut(!ledState[i]);
			   }
			} 
		}
	}
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
