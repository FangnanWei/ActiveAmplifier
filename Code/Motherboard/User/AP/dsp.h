/**************************************************************************
	File		: dsp.h
	Author		: TaoLei Zhang
	Date		: 2018-8-8
***************************************************************************/
#ifndef _DSP_H_
#define _DSP_H_
	
/**************************************************************************
    								include
***************************************************************************/
#include "I2C.h"
/**************************************************************************
    								macro
***************************************************************************/
#define DSP_SLAVE_Addr		0x68

/**************************************************************************
    								typedef
***************************************************************************/

/**************************************************************************
    								variables
***************************************************************************/
	
/**************************************************************************
    								functions
***************************************************************************/
uint8_t ReadDSP(uint16_t addr, uint8_t *buf, uint16_t len);
void SIGMA_WRITE_REGISTER_BLOCK(uint8_t devAddress, uint16_t address, uint16_t length, uint8_t *pData);
void SIGMA_SAFELOAD_WRITE_REGISTER(uint8_t devAddress, uint16_t address, uint16_t length, uint8_t *pData);

struct Dsp_Property{
	struct I2c *i2c;
	uint8_t slaveAddr;
};

struct Dsp{	
	struct Dsp_Property pp;
	uint8_t (*Read)(struct Dsp *self, uint16_t addr, uint8_t *buf, uint16_t len);
	void (*WriteRegisterBlock)(struct Dsp *self, uint16_t addr, uint8_t *buf, uint16_t len);
	void (*SaftLoadWriteRegister)(struct Dsp *self, uint16_t addr, uint8_t *buf, uint16_t len);
};

struct Dsp *Dsp_Open(struct I2c *i2c);


#endif

