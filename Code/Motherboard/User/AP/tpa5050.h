/**************************************************************************
	File		: tpa5050.h
	Author		: 
	Date		: 2018-8-18
***************************************************************************/
#ifndef _TPA5050_H_
#define _TPA5050_H_
	
/**************************************************************************
    								include
***************************************************************************/
#include "stm32f10x.h"
#include "i2c.h"

/**************************************************************************
    								macro
***************************************************************************/
#define TPA_5050_Address		0xd0	
#define CONTROL					1
#define RIGHT_DELAY				2
#define LEFT_DELAY				4
#define FRAME_DELAY				6
#define RJ_PACKET_LEN			7
#define COMPLETE_UPDATE			8

/**************************************************************************
    								typedef
***************************************************************************/
	
/**************************************************************************
    								variables
***************************************************************************/
	
/**************************************************************************
    								functions
***************************************************************************/
void TPA_Init(uint8_t slaveAddr);
/* 

channel:
			LEFT_DELAY 
			RIGHT_DELAY 
*/
void TPA_SetDelay(uint8_t slaveAddr, uint16_t delay, uint8_t channel);
uint16_t TPA_GetDelay(uint8_t channel);

struct Tpa5050_Property{
	struct I2c *i2c;
	uint8_t slaveAddr;
};

struct Tpa5050{
	struct Tpa5050_Property pp;
	void (*SetDelay)(struct Tpa5050 *self, uint16_t delay, uint8_t channel);
	uint16_t (*GetDelay)(struct Tpa5050 *self, uint8_t channel);
};

struct Tpa5050 *Tpa5050_Open(struct I2c *i2c);


#endif

