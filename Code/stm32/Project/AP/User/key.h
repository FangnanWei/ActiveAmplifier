/**********************************************
                key.h
                zhangtaolei   2016-6-18
***********************************************/
#ifndef _KEY_H_
#define _KEY_H_

/**********************************************
                include
***********************************************/
#include "stm32f10x.h"
#include "DI_TypesPublic.h"
#include "Gpio.h"
#include "McuMsg.h"

/**********************************************
                macro
***********************************************/
#define KEY_SCAN_Time       10

#define DIM(x)				(sizeof(x)/sizeof(x[0]))

typedef void(*KeyProc_f)(AP_Inst_t *inst);

struct Key_Property{
	//保持不变
	struct Gpio *gpio;
	bool isLowValid;
	uint16_t longPressTime;
	KeyProc_f proc_f;    //按键处理函数

	bool longPressEvent;	//长按事件
	bool shortPressEvent;	//短按事件


	//可变参数
	bool isKeyPressed;		//是否按住
	bool pressEvent;		//按下事件
	bool releaseEvent;		//抬起事件
	uint16_t pressTime;		//按住总时间
	bool isLongPress;
	bool isShortPress;
};

struct Key{
	struct Key_Property pp;
	void (*Scan)(struct Key *self);
	void (*Proc)(struct Key *self, AP_Inst_t *inst);
};

void Key_Init(void);
void Key_Deal(AP_Inst_t *inst);

struct Key *Key_Open(struct Gpio *gpio, bool isLowValid, uint16_t longPressTime, KeyProc_f proc_f);

#endif

