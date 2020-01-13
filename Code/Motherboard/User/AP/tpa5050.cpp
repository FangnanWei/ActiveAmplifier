/**************************************************************************
	File		: tpa5050.c
	Author		: 
	Date		: 2018-8-18
***************************************************************************/
	
/**************************************************************************
    								include
***************************************************************************/
#include "tpa5050.h"
#include <stdlib.h>
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
void TPA_Init(uint8_t slaveAddr)
{
	uint8_t value;

	value = 0x00;			/* both active , I2S */
	I2C_Write(I2C2, slaveAddr, CONTROL, &value, 1);
}

void TPA_SetDelay(uint8_t slaveAddr, uint16_t delay, uint8_t channel)
{
	uint8_t value = 0x01;
	uint8_t delayValue[2] = {0};

	delayValue[0] = delay>>8;
	delayValue[1] = delay&0xff;

	I2C_Write(I2C2, slaveAddr, channel, delayValue, 2);
	I2C_Write(I2C2, slaveAddr, COMPLETE_UPDATE, &value, 1);
}

uint16_t TPA_GetDelay(uint8_t channel)
{
	uint16_t value = 0;
	uint8_t delayValue[2] = {0};

	I2C_Read(I2C2, TPA_5050_Address, channel, delayValue, 2);
	value = (delayValue[0]<<8) + delayValue[1];
	return value;
}



static void Tpa5050_SetDelay(struct Tpa5050 *self, uint16_t delay, uint8_t channel)
{
	uint8_t value = 0x01;
	uint8_t delayValue[2] = {0};

	struct I2c *i2c = self->pp.i2c;

	delayValue[0] = delay>>8;
	delayValue[1] = delay&0xff;

	i2c->Write(i2c, self->pp.slaveAddr, channel, delayValue, 2);
	i2c->Write(i2c, self->pp.slaveAddr, COMPLETE_UPDATE, &value, 1);
}

static uint16_t Tpa5050_GetDelay(struct Tpa5050 *self, uint8_t channel)
{
	uint16_t value = 0;
	uint8_t delayValue[2] = {0};
	struct I2c *i2c = self->pp.i2c;

	i2c->Read(i2c, self->pp.slaveAddr, channel, delayValue, 2);
	value = (delayValue[0]<<8) + delayValue[1];
	return value;
}

struct Tpa5050 *Tpa5050_Open(struct I2c *i2c)
{
	uint8_t value;
	struct Tpa5050 *tpa5050 = (struct Tpa5050 *)malloc(sizeof(struct Tpa5050));

	tpa5050->pp.i2c = i2c;
	tpa5050->pp.slaveAddr = TPA_5050_Address;

	tpa5050->SetDelay = Tpa5050_SetDelay;
	tpa5050->GetDelay = Tpa5050_GetDelay;

	value = 0x00;			/* both active , I2S */
	i2c->Write(i2c, tpa5050->pp.slaveAddr, CONTROL, &value, 1);

	return tpa5050;
}


