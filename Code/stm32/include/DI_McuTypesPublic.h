/**************************************************************************
	File		: DI_McuTypesPublic.h
	Author		: TaoLei Zhang
	Date		: 2018-1-10
***************************************************************************/
#ifndef _DI_MCU_TYPES_PUBLIC_H_
#define _DI_MCU_TYPES_PUBLIC_H_

/**************************************************************************
    								include
***************************************************************************/

/**************************************************************************
    								macro
***************************************************************************/

/**************************************************************************
    								typedef
***************************************************************************/
typedef enum tagMcuChangeType_e
{
	MCU_CHANGE_UartReveice = 1,
	MCU_CHANGE_KeyBoardLedStatus = 2,
}DI_McuChangeType_e;

typedef enum tagMcuUartCheckType
{
	MCU_UART_CHECK_None,
	MCU_UART_CHECK_ODD,
	MCU_UART_CHECK_EVEN,
	MCU_UART_CHECK_MARK,
	MCU_UART_CHECK_SPACE,

	MCU_UART_CHECK_Butt,
}McuUartCheckType_e;

typedef enum tagMcuUartType
{
	MCU_UART_RS232,
	MCU_UART_RS485,
	MCU_UART_RS422,
	
	MCU_UART_TYPE_Butt,
}McuUartType_e;

typedef enum tagParityMode_e
{
	ParityMode_NONE,
	ParityMode_ODD,
	ParityMode_EVEN,
	ParityMode_MARK,
	ParityMode_SPACE,

	ParityMode_Butt,
}ParityMode_e;

typedef struct tagUartConfig_t
{
	uint32_t	baudrate;
	uint8_t		wordWidth;
	uint8_t		stopBits;
	uint8_t		check;		//McuUartCheckType_e类型，使用uint8_t兼容不同平台，枚举类型hisi为4字节，stm32为1字节
	uint8_t 	type;		//McuUartType_e类型，使用uint8_t兼容不同平台

}DI_UartConfig_t;

/**************************************************************************
    								variables
***************************************************************************/

/**************************************************************************
    								functions
***************************************************************************/
#endif

