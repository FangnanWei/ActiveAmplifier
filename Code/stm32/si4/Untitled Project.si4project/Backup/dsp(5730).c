/**************************************************************************
	File		: dsp.c
	Author		: TaoLei Zhang
	Date		: 2018-8-8
***************************************************************************/

	
/**************************************************************************
    								include
***************************************************************************/
#include "dsp.h"
#include "I2C.h"
#include "Time.h"
/**************************************************************************
    								macro
***************************************************************************/


#define ADAU1701_SAFELOAD_DATA(i) (0x0810 + (i))
#define ADAU1701_SAFELOAD_ADDR(i) (0x0815 + (i))

#define ADAU1701_DSPCTRL	0x081c
#define ADAU1701_SEROCTL	0x081e
#define ADAU1701_SERICTL	0x081f

#define ADAU1701_AUXNPOW	0x0822
#define ADAU1701_PINCONF_0	0x0820
#define ADAU1701_PINCONF_1	0x0821
#define ADAU1701_AUXNPOW	0x0822

#define ADAU1701_OSCIPOW	0x0826
#define ADAU1701_DACSET		0x0827

#define ADAU1701_MAX_REGISTER	0x0828

#define ADAU1701_DSPCTRL_CR		(1 << 2)
#define ADAU1701_DSPCTRL_DAM		(1 << 3)
#define ADAU1701_DSPCTRL_ADM		(1 << 4)
#define ADAU1701_DSPCTRL_IST		(1 << 5)
#define ADAU1701_DSPCTRL_SR_48		0x00
#define ADAU1701_DSPCTRL_SR_96		0x01
#define ADAU1701_DSPCTRL_SR_192		0x02
#define ADAU1701_DSPCTRL_SR_MASK	0x03

/**************************************************************************
    								typedef
***************************************************************************/
 	
/**************************************************************************
    								variables
***************************************************************************/

/**************************************************************************
    								functions
***************************************************************************/
static __inline void put_unaligned_le16(u16 val, void *p)
{
	u8 *_p = p;
	_p[0] = val >> 8;
	_p[1] = val&0xff ;
}


static unsigned char Dsp_Read(struct Dsp *self, unsigned short addr, unsigned char *buf, unsigned short len)
{
	struct I2c *i2c = self->i2c;
	return i2c->Read_16(i2c, self->pp.slaveAddr, addr, buf, len);
}

static void Dsp_WriteRegisterBlock(struct Dsp *self, unsigned short addr, unsigned char *buf, unsigned short len)
{
	struct I2c *i2c = self->i2c;
	i2c->Write_16(i2c, self->pp.slaveAddr, addr, buf, len);
}

static void Dsp_SaftLoadWriteRegister(struct Dsp *self, unsigned short addr, unsigned char *buf, unsigned short len)
{
	uint8_t value[2] = {0};
	uint16_t val = 0;
	uint16_t i = 0;
	uint8_t data[10];

	struct I2c *i2c = self->i2c;

	i2c->Read_16(i2c, self->pp.slaveAddr, ADAU1701_DSPCTRL, value, 2);
	val = (value[0]<<8) + value[1];
	if (val & ADAU1701_DSPCTRL_IST)
	{
		Time_SleepMs(50);
	}

	for (i = 0; i < len/4; i++)
	{
		data[0] = 0;
		memcpy(&data[1], buf + i*4, 4);
		i2c->Write_16(i2c, self->pp.slaveAddr, ADAU1701_SAFELOAD_DATA(i), data, 5);
		put_unaligned_le16(addr+i, data);
		i2c->Write_16(i2c, self->pp.slaveAddr, ADAU1701_SAFELOAD_ADDR(i), data, 2);
	}
	
	val |= ADAU1701_DSPCTRL_IST;
	value[0] = val >> 8;
	value[1] = val & 0xff;
	i2c->Write_16(i2c, self->pp.slaveAddr, ADAU1701_DSPCTRL, value, 2);
}

struct Dsp *Dsp_Open(struct I2c *i2c)
{
	struct Dsp *dsp =(struct Dsp *)malloc(sizeof(struct Dsp));

	dsp->i2c = i2c;
	dsp->pp.slaveAddr = DSP_SLAVE_Addr;
	dsp->Read = Dsp_Read;
	dsp->WriteRegisterBlock = Dsp_WriteRegisterBlock;
	dsp->SaftLoadWriteRegister = Dsp_SaftLoadWriteRegister;

	return dsp;
}

uint8_t ReadDSP(uint16_t addr, uint8_t *buf, uint16_t len)
{
	return I2C_Read_16(I2C2, DSP_SLAVE_Addr, addr, buf, len);
}

void SIGMA_WRITE_REGISTER_BLOCK(uint8_t devAddress, uint16_t address, uint16_t length, uint8_t *pData)
{
	I2C_Write_16(I2C2, devAddress, address, pData, length);
}

void SIGMA_SAFELOAD_WRITE_REGISTER(uint8_t devAddress, uint16_t address, uint16_t length, uint8_t *pData)
{
	uint8_t value[2] = {0};
	uint16_t val = 0;
	uint16_t i = 0;
	uint8_t buf[10];

	I2C_Read_16(I2C2, devAddress, ADAU1701_DSPCTRL, value, 2);
	val = (value[0]<<8) + value[1];
	if (val & ADAU1701_DSPCTRL_IST)
	{
		Time_SleepMs(50);
	}

	for (i = 0; i < length/4; i++)
	{
		buf[0] = 0;
		memcpy(&buf[1], pData + i*4, 4);
		I2C_Write_16(I2C2, devAddress, ADAU1701_SAFELOAD_DATA(i), buf, 5);
		put_unaligned_le16(address+i, buf);
		I2C_Write_16(I2C2, devAddress, ADAU1701_SAFELOAD_ADDR(i), buf, 2);
	}
	
	val |= ADAU1701_DSPCTRL_IST;
	value[0] = val >> 8;
	value[1] = val & 0xff;
	I2C_Write_16(I2C2, devAddress, ADAU1701_DSPCTRL, value, 2);
}





