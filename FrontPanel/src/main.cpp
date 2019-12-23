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
#include "Gpio.h"
#include "Sys.h"
#include "I2c.h"
/* Private defines -----------------------------------------------------------*/
const uint8_t EepronAddress = 0xA8;
GpioParam GpioLed01Param(GPIOC, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST, false);
GpioParam GpioLed02Param(GPIOC, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_FAST, false);
GpioParam GpioLed03Param(GPIOC, GPIO_PIN_2, GPIO_MODE_OUT_PP_LOW_FAST, false);
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
  
  led01_->Disable();
  led01_->Enable();
  Sys::BeepRingForMs(100);
  //Sys::DelaySecond(10);
  led01_->Disable();
  
  //I2cCtrl::Init();
  
  Eeprom *eeprom = new Eeprom(EepronAddress);
  
  uint8_t tempBuf[] = {0, 1, 2, 3, 4, 5, 6, 7};
  
  eeprom->Write(0x00, tempBuf, sizeof(tempBuf));
  
  eeprom->Read(0x00, tempBuf, sizeof(tempBuf));
  
  /* Infinite loop */
  while (1)
  {
    led01_->Enable();
    Sys::DelaySecond(1);
    led01_->Disable();
    Sys::DelaySecond(1);
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
