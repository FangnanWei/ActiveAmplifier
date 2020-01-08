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
struct Dsp_property{
	unsigned char slaveAddr;
};

struct Dsp{
	struct I2c *i2c;
	struct Dsp_property pp;

	unsigned char (*Read)(struct Dsp *self, unsigned short addr, unsigned char *buf, unsigned short len);
	void (*WriteRegisterBlock)(struct Dsp *self, unsigned short addr, unsigned char *buf, unsigned short len);
	void (*SaftLoadWriteRegister)(struct Dsp *self, unsigned short addr, unsigned char *buf, unsigned short len);
};

struct Dsp *Dsp_Open(struct I2c *i2c);


uint8_t ReadDSP(uint16_t addr, uint8_t *buf, uint16_t len);
void SIGMA_WRITE_REGISTER_BLOCK(uint8_t devAddress, uint16_t address, uint16_t length, uint8_t *pData);
void SIGMA_SAFELOAD_WRITE_REGISTER(uint8_t devAddress, uint16_t address, uint16_t length, uint8_t *pData);


#endif

