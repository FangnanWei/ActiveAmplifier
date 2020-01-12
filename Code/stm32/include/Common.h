/**************************************************************************
	File		: Common.h
	Author		: TaoLei Zhang
	Date		: 2017-12-25
***************************************************************************/
#ifndef _COMMON_H_
#define _COMMON_H_
#include <stdio.h>
#include "DI_McuIfPublic.h"
/**************************************************************************
    								include
***************************************************************************/

/**************************************************************************
    								macro
***************************************************************************/
extern PrintfFunc McuIfPrint;
#define DebugPrint(fmt, ...)            McuIfPrint(__FILE__":%d :"fmt"\n", __LINE__, ##__VA_ARGS__)

/**************************************************************************
    								typedef
***************************************************************************/
typedef enum tagThreadState_e{
	THREAD_Running,
	THREAD_Stopping,
	THREAD_Stopped
}ThreadState_e;
/**************************************************************************
    								variables
***************************************************************************/

/**************************************************************************
    								functions
***************************************************************************/
#endif

