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

uint8_t Tx1_Buffer[] = " STM8S I2C Firmware Library EEPROM driver example: \
                        buffer 1 transfer into address sEE_WRITE_ADDRESS1  \
                        Example Description \
                        This firmware provides a basic example of how to use the I2C firmware library and\
                        an associate I2C EEPROM driver to communicate with an I2C EEPROM device (here the\
                        example is interfacing with M24C64 EEPROM)\
                          \
                        I2C peripheral is configured in Master transmitter during write operation and in\
                        Master receiver during read operation from I2C EEPROM. \
                          \
                        I2C peripheral speed is set to 200kHz and can be configured by \
                        modifying the relative define in stm8s_eval_i2c_ee.h file.\
                         ";
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
  
  Eeprom *eeprom = new Eeprom(EepronAddress, true, EepronPageSize, EepronPageSize * EepronPages);
  
  int i = 0;
  eeprom->Write(0xe4, Tx1_Buffer, sizeof(Tx1_Buffer));
  
  uint8_t tempBuf[sizeof(Tx1_Buffer)];
  Sys::DelayMs(2);//写完必须出来再等两毫秒，内部等还不行！！
  eeprom->Read(0xe4, tempBuf, sizeof(Tx1_Buffer));
  
  for (i = 0; i < sizeof(Tx1_Buffer); i++) {
    if (tempBuf[i] != Tx1_Buffer[i]) {
        //while(1);
        i++;
        continue;
        
    }
  }
  /* Infinite loop */
  Sys::BeepRingForMs(100);
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
