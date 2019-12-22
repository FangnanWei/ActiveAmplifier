#ifndef _I2c_h_
#define _I2c_h_
#include "stm8s_conf.h"
#include "stm8s.h"

/*****************************************
                                macro
******************************************/
typedef enum I2cAddr_t {
	I2cAddr_LedCtrler	= 0xE0,
	I2cAddr_E2prom		= 0xA0
} I2cAddr;

#define I2C_TimeOut         ((unsigned long)0x10000)

#define I2cGpioPort			GPIOB
#define SclGpioPin			GPIO_PIN_4
#define SdaGpioPin			GPIO_PIN_5

#define SCL_Low()           GPIO_WriteLow(I2cGpioPort, SclGpioPin)
#define SCL_High()          GPIO_WriteHigh(I2cGpioPort, SclGpioPin)

#define SDA_Low()           GPIO_WriteLow(I2cGpioPort, SdaGpioPin)
#define SDA_High()          GPIO_WriteHigh(I2cGpioPort, SdaGpioPin)

#define SetSDA_Input()      GPIO_Init(I2cGpioPort, SdaGpioPin, GPIO_MODE_IN_PU_NO_IT)
#define SetSDA_Output()     GPIO_Init(I2cGpioPort, SdaGpioPin, GPIO_MODE_OUT_OD_HIZ_FAST)

#define Read_SDA()          GPIO_ReadInputPin(I2cGpioPort, SdaGpioPin)

//#define I2C_Delay()         asm("nop"); asm("nop");asm("nop"); asm("nop");

void I2cInit();
void I2C_Start();
void I2C_Restart();
void I2C_Stop();
void I2C_WriteByte(unsigned char data);
unsigned char I2C_ReadByte(bool isNack);

unsigned char I2C_Read(unsigned char slaveAddr, unsigned char regAddr, unsigned char *buf, unsigned short len);
unsigned char I2C_Write(unsigned char slaveAddr, unsigned char regAddr, unsigned char *buf, unsigned short len);

#endif
