/**************************************************************************
	File		: uart.h
	Author		: TaoLei Zhang
	Date		: 2018-2-5
***************************************************************************/
#ifndef _UART_H_
#define _UART_H_

/**************************************************************************
    								include
***************************************************************************/
#include "stm32f10x.h"
#include "comfifo.h"
#include "common_def.h"
#include "DI_McuTypesPublic.h"
#include "DI_TypesPublic.h"
/**************************************************************************
    								macro
***************************************************************************/
#define USART_REV_BUF_SIZE      2048

/**************************************************************************
    								typedef
***************************************************************************/

/**************************************************************************
    								variables
***************************************************************************/
extern ComFifo_t g_ComFifo;
extern ComFifo_t g_ComFifo_2;
extern DI_UartConfig_t g_Usart2Cfg;
/**************************************************************************
    								functions
***************************************************************************/
void USART1_Configuration(void);
void USART2_Configuration(void);

void Usart1SendData(unsigned char *send_buff,unsigned short length);
void Usart2SendData(unsigned char *send_buff,unsigned short length);
int Usart2Cfg(DI_UartConfig_t cfg);

#endif

