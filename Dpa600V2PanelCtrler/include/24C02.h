#ifndef __24C02_H
#define __24C02_H
#include "stm8s.h"

#define At24c02WritePageSize 8
//#define At24c02ReadPageSize 256

/* Includes ------------------------------------------------------------------*/
int Read24c02(uint16_t addr,uint8_t *buf,uint16_t num);
int Write24c02(uint16_t addr,uint8_t *buf,uint16_t num);
void test24c02(void);

#endif


