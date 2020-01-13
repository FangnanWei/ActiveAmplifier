/**************************************************************************
	File		: eeprom.h
	Date		: 2018-7-11
***************************************************************************/
#ifndef _EEPROM_H_
#define _EEPROM_H_

#include "I2C.h"

/**************************************************************************
    								include
***************************************************************************/
#define EEPROM_SLAVE_Addr		0xa0
#define EEPROM_PAGE_Size		64		

/**************************************************************************
    								variables
***************************************************************************/


/**************************************************************************
    								functions
***************************************************************************/
uint8_t Read24C256(uint8_t slaveAddr, uint16_t addr, uint8_t *buf, uint16_t len);
uint8_t Write24C256(uint8_t slaveAddr, uint16_t addr, uint8_t *buf, uint16_t len);

struct Eeprom_Property{
	struct I2c *i2c;
	uint8_t slaveAddr;
};

struct Eeprom{
	struct Eeprom_Property pp;

	uint8_t (*Read)(struct Eeprom *self, uint16_t addr, uint8_t *buf, uint16_t len);
	uint8_t (*Write)(struct Eeprom *self, uint16_t addr, uint8_t *buf, uint16_t len);	
};

struct Eeprom *Eeprom_Open(struct I2c *i2c);

#endif

