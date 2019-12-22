/******************************************
                    spi.h
                    2015-09-08          zhangfei
*******************************************/
#ifndef __SPI_H__
#define __SPI_H__

/******************************************
                    include
*******************************************/
#include "stm8s.h"

/******************************************
                    macro
*******************************************/
#define SPI_CS				GPIOE
#define SPI_Pin_CS			GPIO_PIN_5

#define SpiGpioGroup		GPIOC

#define SpiClkPin			GPIO_PIN_5
#define SpiSdoPin			GPIO_PIN_6

#define SpiClkLow()		GPIO_WriteLow(SpiGpioGroup, SpiClkPin)
#define SpiClkHigh()	GPIO_WriteHigh(SpiGpioGroup, SpiClkPin)

#define  SpiSdoLow()	GPIO_WriteLow(SpiGpioGroup, SpiSdoPin)
#define SpiSdoHigh()	GPIO_WriteHigh(SpiGpioGroup, SpiSdoPin)

#define SPI_CS_LOW()       GPIO_WriteLow(SPI_CS, SPI_Pin_CS)
#define SPI_CS_HIGH()      GPIO_WriteHigh(SPI_CS, SPI_Pin_CS)

#define SPI_WaiteAckTime    20000

char SpiLcdWrite(u8 data);
void SpiLcdInit();
#endif
