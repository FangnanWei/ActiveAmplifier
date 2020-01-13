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
#include "Driver/St/Gpio.h"
#include "Driver/St/Sys.h"
#include "Driver/St/Uart.h"
#include "Driver/Chip/Eeprom.h"
#include "Driver/Chip/Oled.h"
#include "Driver/Chip/Key.h"
#include "Driver/Chip/RotaryEncoder.h"

#include "App/Menu.h"

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

GpioParam GpioUartRxParam(GPIOD, GPIO_PIN_6, GPIO_MODE_IN_FL_NO_IT, false);
GpioParam GpioUartTxParam(GPIOD, GPIO_PIN_5, GPIO_MODE_OUT_PP_LOW_FAST, false);

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


void ShowPage(Item *rootItem, Oled *oled) {
	oled->Clear();
	for (uint8_t i = 0; i < rootItem->GetSize(); i++) {
		Item *son = rootItem->GetSon(i);
		if (son) {
			oled->ShowString(0, i*2, (const uint8_t *)son->GetFieldName());
		}
	}
}

//将一个页面显示出来
void ShowSon(Item *rootItem, Oled *oled){
  
  for (uint8_t i = 0; i < rootItem->GetSize(); i++) {
  	Item *son = rootItem->GetSon(i);
	if (son) {
		ShowPage(son, oled);
		ShowSon(son, oled);
	  
	  //ShowSon(son);
	}
  }
}


void main(void)
{
	Sys::CpuInit();

	Gpio *uartRx_ =  new Gpio(GpioUartRxParam);
	uartRx_->Init();
	
	Gpio *uartTx_ =  new Gpio(GpioUartTxParam);
	uartTx_->Init();
	
	Gpio *led01_ =  new Gpio(GpioLed01Param);
	led01_->Init();

	Gpio *led02_ = new Gpio(GpioLed02Param);
	led02_->Init();

	Gpio *led03_ = new Gpio(GpioLed03Param);
	led03_->Init();

	Eeprom *eeprom = new Eeprom(EepronAddress, true, EepronPageSize, EepronPageSize * EepronPages);

	Key *encKey = new Key(GpioEcKeyIoParam, 2000);
	KeyPressType encKeyType = KeyPressType_None;
	RotaryEncoder *enc = new RotaryEncoder(GpioEcAIoParam, GpioEcBIoParam);
	RotaryEncoderType encType = RotaryEncoder_None;

	Uart *uart = new Uart(115200);
	
	enableInterrupts();
	
	Oled *oled = new Oled(OledSpdParam);
	oled->Init();

	//Item *rootItem = new RootItem(NULL, "ROOT");
	//ShowSon(rootItem, oled);
	Menu *menu = new Menu(oled);
	menu->ShowSubItems();
	menu->ShowSymbol();
	
	//eeprom->Test();
	/* Infinite loop */
	Sys::BeepRingForMs(100);
	
	uint8_t uartSendBuf[] = {0x0f, 0x0f, 0x03, 0x0f, 0x0f, 0x03, 0x0f, 0x0f, 0x03, 0x0f, 0x0f, 0x03};
	uint32_t TestTime = Sys::GetTimeMs();
	
	while (1)
	{
#if 0    
		led01_->Enable();
		Sys::DelaySecond(1);
		led01_->Disable();
		Sys::DelaySecond(1);
#endif
		encKeyType = encKey->Scan();
		
		if(encKeyType != KeyPressType_None) {
			menu->Press(encKeyType);
		}
		encType = enc->Scan();
		if (encType != RotaryEncoder_None) {
			menu->Rolling(encType);
		}
		
		if ((Sys::GetTimeMs() - TestTime) > 5000) {
		  	TestTime = Sys::GetTimeMs();
			uart->SendData(uartSendBuf, sizeof(uartSendBuf));
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
