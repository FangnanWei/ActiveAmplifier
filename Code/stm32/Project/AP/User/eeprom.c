 
 /**************************************************************************
	File		: eeprom.c
	Date		: 2018-7-11
***************************************************************************/

	
/**************************************************************************
    								include
***************************************************************************/
#include "dsp.h"
#include "eeprom.h"
#include "i2c.h"
#include "DI_TypesPublic.h"
#include "tpa5050.h"
#include "Time.h"

/**************************************************************************
    								macro
***************************************************************************/

/**************************************************************************
    								typedef
***************************************************************************/	

/**************************************************************************
    								variables
***************************************************************************/
	
/**************************************************************************
    								functions
***************************************************************************/
/* 成功返回0 */
uint8_t Read24C256(uint8_t slaveAddr, uint16_t addr, uint8_t *buf, uint16_t len)
{
	return I2C_Read_16(I2C2, slaveAddr, addr, buf, len);
}

/* 成功返回0 */
uint8_t Write24C256(uint8_t slaveAddr, uint16_t addr, uint8_t *buf, uint16_t len)
{
	uint16_t i = 0;
	uint8_t *pBuf = buf;
	int wlen = (uint16_t)len;

#if 1
	//for page align
	if (addr % EEPROM_PAGE_Size)
	{
		I2C_Write_16(I2C2, slaveAddr, addr, buf, EEPROM_PAGE_Size-(addr%EEPROM_PAGE_Size));
		addr = addr + EEPROM_PAGE_Size-(addr%EEPROM_PAGE_Size);
		pBuf = &buf[EEPROM_PAGE_Size-(addr%EEPROM_PAGE_Size)];
		wlen = wlen - (EEPROM_PAGE_Size-(addr%EEPROM_PAGE_Size)); 
	}
#endif

	while(wlen > 0)
	{
		if (wlen > EEPROM_PAGE_Size)
		{
			I2C_Write_16(I2C2, slaveAddr, addr + i*EEPROM_PAGE_Size, &pBuf[i*EEPROM_PAGE_Size], EEPROM_PAGE_Size);
		}
		else
		{
			I2C_Write_16(I2C2, slaveAddr, addr + i*EEPROM_PAGE_Size, &pBuf[i*EEPROM_PAGE_Size], wlen);
		}
		wlen = wlen - EEPROM_PAGE_Size;
		i++;
		Time_SleepMs(20);
	}
	return 0;
}


static uint8_t Eeprom_Read(struct Eeprom *self, uint16_t addr, uint8_t *buf, uint16_t len)
{
	struct I2c *i2c = self->pp.i2c;
	return i2c->Read_16(i2c, self->pp.slaveAddr, addr, buf, len);
}

static uint8_t Eeprom_Write(struct Eeprom *self, uint16_t addr, uint8_t *buf, uint16_t len)
{

	uint16_t i = 0;
	uint8_t *pBuf = buf;
	int wlen = (uint16_t)len;
	struct I2c *i2c = self->pp.i2c;

#if 1
	//for page align
	if (addr % EEPROM_PAGE_Size)
	{
		i2c->Write_16(i2c, self->pp.slaveAddr, addr, buf, EEPROM_PAGE_Size-(addr%EEPROM_PAGE_Size));
		addr = addr + EEPROM_PAGE_Size-(addr%EEPROM_PAGE_Size);
		pBuf = &buf[EEPROM_PAGE_Size-(addr%EEPROM_PAGE_Size)];
		wlen = wlen - (EEPROM_PAGE_Size-(addr%EEPROM_PAGE_Size)); 
	}
#endif

	while(wlen > 0)
	{
		if (wlen > EEPROM_PAGE_Size)
		{
			i2c->Write_16(i2c, self->pp.slaveAddr, addr + i*EEPROM_PAGE_Size, &pBuf[i*EEPROM_PAGE_Size], EEPROM_PAGE_Size);
		}
		else
		{
			i2c->Write_16(i2c, self->pp.slaveAddr, addr + i*EEPROM_PAGE_Size, &pBuf[i*EEPROM_PAGE_Size], wlen);
		}
		wlen = wlen - EEPROM_PAGE_Size;
		i++;
		Time_SleepMs(20);
	}
	return 0;
}

struct Eeprom *Eeprom_Open(struct I2c *i2c)
{
	struct Eeprom *eeprom = (struct Eeprom *)malloc(sizeof(struct Eeprom));

	eeprom->pp.i2c = i2c;
	eeprom->pp.slaveAddr = EEPROM_SLAVE_Addr;

	eeprom->Read = Eeprom_Read;
	eeprom->Write = Eeprom_Write;

	return eeprom;
}

