#ifndef	_I2C_H_
#define	_I2C_H_

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_i2c.h"

struct I2c_property{
	I2C_TypeDef *I2Cx;
};

struct I2c{
	struct I2c_property pp;

	int (*Read)(const struct I2c *self, unsigned char slaveAddr, unsigned char addr, unsigned char *buf, unsigned int len);
	int (*Write)(const struct I2c *self, unsigned char slaveAddr, unsigned char addr, unsigned char *buf, unsigned int len);
	int (*Read_16)(const struct I2c *self, unsigned char slaveAddr, unsigned char addr, unsigned char *buf, unsigned int len);
	int (*Write_16)(const struct I2c *self, unsigned char slaveAddr, unsigned char addr, unsigned char *buf, unsigned int len);
	void (*Close)(const struct I2c *self);
};

struct I2c *I2c_Open(I2C_TypeDef *I2Cx);
void I2c_ReOpen(const struct I2c *self);

struct Eeprom{
	struct I2c *adapter;
	unsigned char slaveAddr;
};

struct Tpa5050{
	struct I2c *adapter;
	unsigned char slaveAddr;
};



#define I2C_WaiteAckTime     6000



void I2C_Configuration(void);
int I2C_Read(I2C_TypeDef *I2Cx,uint8_t I2C_Addr,uint8_t addr,uint8_t *buf,uint16_t num);
int I2C_Write(I2C_TypeDef *I2Cx,uint8_t I2C_Addr,uint8_t addr,uint8_t *buf,uint16_t num);
int I2C_Read_16(I2C_TypeDef *I2Cx,uint8_t I2C_Addr,uint16_t addr,uint8_t *buf,uint16_t num);
int I2C_Write_16(I2C_TypeDef *I2Cx,uint8_t I2C_Addr,uint16_t addr,uint8_t *buf,uint16_t num);

#endif
