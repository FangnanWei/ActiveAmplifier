#ifndef	_I2C_H_
#define	_I2C_H_

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_i2c.h"


#define I2C_WaiteAckTime     6000



void I2C_Configuration(void);
int I2C_Read(I2C_TypeDef *I2Cx,uint8_t I2C_Addr,uint8_t addr,uint8_t *buf,uint16_t num);
int I2C_Write(I2C_TypeDef *I2Cx,uint8_t I2C_Addr,uint8_t addr,uint8_t *buf,uint16_t num);
int I2C_Read_16(I2C_TypeDef *I2Cx,uint8_t I2C_Addr,uint16_t addr,uint8_t *buf,uint16_t num);
int I2C_Write_16(I2C_TypeDef *I2Cx,uint8_t I2C_Addr,uint16_t addr,uint8_t *buf,uint16_t num);

struct I2c_Property{
	I2C_TypeDef *I2Cx;
};

struct I2c{
	struct I2c_Property pp;

	int32_t (*Read)(const struct I2c *self, uint8_t slaveAddr, uint8_t addr, uint8_t *buf, uint16_t len);
	int32_t (*Write)(const struct I2c *self, uint8_t slaveAddr, uint8_t addr, uint8_t *buf, uint16_t len);
	int32_t (*Read_16)(const struct I2c *self, uint8_t slaveAddr, uint8_t addr, uint8_t *buf, uint16_t len);
	int32_t (*Write_16)(const struct I2c *self, uint8_t slaveAddr, uint8_t addr, uint8_t *buf, uint16_t len);
	void (*Close)(const struct I2c *self);
};

struct I2c *I2c_Open(I2C_TypeDef *I2Cx);
void I2c_ReOpen(const struct I2c *self);


#endif
