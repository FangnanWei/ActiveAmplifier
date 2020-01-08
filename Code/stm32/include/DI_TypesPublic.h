/*************************************************
				DI_TypesPublic.h
*************************************************/
#ifndef __DI_TYPES_PUBLIC_H__
#define __DI_TYPES_PUBLIC_H__

/*************************************************
				include
*************************************************/
#include <stdint.h>
/*************************************************
				macro
*************************************************/
#if 0
#ifndef int8_t
#define int8_t char
#endif

#ifndef int16_t
#define int16_t short
#endif

#ifndef int32_t
#define int32_t int
#endif

#ifndef uint8_t
#define uint8_t unsigned char
#endif

#ifndef uint16_t
#define uint16_t unsigned short
#endif

#ifndef uint32_t
#define uint32_t unsigned int
#endif
#endif

#ifndef Bool
#define Bool uint8_t
#endif

#ifndef bool
#define bool uint8_t
#endif

#ifndef UINT8
#define UINT8 uint8_t
#endif

#ifndef False
#define False 0
#endif
#ifndef FALSE
#define FALSE 0
#endif


#ifndef True
#define True 1
#endif
#ifndef TRUE
#define TRUE 1
#endif


#ifndef NULL
#define NULL ((void*)0)
#endif

/*************************************************
				typedef
*************************************************/
/* 为了版本兼容性，只可在后端增加，不可修改值 */
typedef enum tagDI_RET_e
{
	DI_Ok 				= 0,
	DI_BuffTooShort 	= 1,
	DI_NoSuchDevice		= 2,
	DI_IllegalArg		= 3,
	DI_DeviceNotOpen	= 4,
	DI_DeviceOperErr	= 5,
	DI_DeviceAckTimeout	= 6,
	DI_Busy				= 7,
	DI_NotSupport		= 8,
	DI_DataTooLarge		= 9,
	DI_DeviceI2CErr		= 10,
	DI_FileNotExist		= 11,
	DI_IllegalFile		= 12,
	
}DI_RET_e;

/*************************************************
				variables
*************************************************/

/*************************************************
				function
*************************************************/

#endif

