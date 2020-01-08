/**********************************************
                key.c
***********************************************/

/**********************************************
                include
***********************************************/
#include <stdlib.h>

#include "dsp.h"
#include "eeprom.h"
#include "key.h"
#include "uart.h"
#include "McuMsg.h"
#include "packet.h"
#include "Scene.h"

//#include "ADAU1701-gpio-test_IC_1.h"
/**********************************************
                macro
***********************************************/

 
/**********************************************
                typedef
***********************************************/
static void switchAudioInput(AP_Inst_t *inst)
{
	struct Scene *scene = inst->scene;
	MsgDspExt_t *pMsgDspExt = NULL;
	
	if (scene->pp.audioInputType == ANALOG_Input)
	{
		scene->pp.audioInputType = DANTE_Input;
	}
	else
	{
		scene->pp.audioInputType = ANALOG_Input;
	}

	//组合一条消息，发往DSP
	pMsgDspExt = (MsgDspExt_t*)malloc(sizeof(MsgDspExt_t));
	pMsgDspExt->address = AUDIO_INPUT_Addr;
	pMsgDspExt->dspParam[3] = scene->pp.audioInputType;
	pMsgDspExt->len = 4;
	pMsgDspExt->type = AUDIO_INPUT_AlgType;
	scene->LoadMsgDspExt(scene, pMsgDspExt, True);
	free(pMsgDspExt);
	SendPacket((uint8_t*)pMsgDspExt, sizeof(MsgDspExt_t), DI_Ok, MSG_DSP_AUDIO_INPUT_Change, 0);
}

static void switchScene(AP_Inst_t *inst)
{
	struct Scene *scene = inst->scene;
	uint8_t value;

	if (scene->pp.mode == DSP_SCENE_A)
	{
		scene->pp.mode = DSP_SCENE_B;
	}
	else if (scene->pp.mode == DSP_SCENE_B)
	{
		scene->pp.mode = DSP_SCENE_C;
	}
	else if (scene->pp.mode == DSP_SCENE_C)
	{
		scene->pp.mode = DSP_SCENE_A;
	}	

	scene->Load(scene, scene->pp.mode);
	value = scene->pp.mode;
	SendPacket(&value, sizeof(value), DI_Ok, MSG_DSP_SCENE_Change, 0);
}

struct Key *sceneKey;
struct Key *audioSrcKey;

void Key_Init(void)
{
	sceneKey = Key_Open(Gpio_Open(GPIOB, GPIO_Pin_15, GPIO_Speed_50MHz, GPIO_Mode_IPU, ENABLE), TRUE, 300*KEY_SCAN_Time, switchScene);
	audioSrcKey = Key_Open(Gpio_Open(GPIOC, GPIO_Pin_8, GPIO_Speed_50MHz, GPIO_Mode_IPU, ENABLE), TRUE, 300*KEY_SCAN_Time, switchAudioInput);
}

void Key_Deal(AP_Inst_t *inst)
{
	sceneKey->Scan(sceneKey);
	sceneKey->Proc(sceneKey, inst);
	
	audioSrcKey->Scan(audioSrcKey);
	audioSrcKey->Proc(audioSrcKey, inst);
}

//改为按下去到时间就执行操作，不释放的话只执行一次操作
void Key_Scan(struct Key *self)
{
	bool isKeyPressed;
	struct Gpio *keyio = self->pp.gpio;
	
	isKeyPressed = self->pp.isLowValid ? !keyio->Read(keyio) : keyio->Read(keyio);

	//之前有无按下
	//有--//这次还是按下--记录时长---根据时长判断是否达到长按事件
		  //这次抬起--根据时长--设置是长按还是短按事件

	//无--//这次是按下--设置按下标志--开始统计时长
		  //这次还是抬起--不作任何操作
	
	if (self->pp.isKeyPressed)
	{
		
		if (isKeyPressed)
		{
			self->pp.pressTime += KEY_SCAN_Time;
			//长按事件
			if (self->pp.pressTime == self->pp.longPressTime)
			{
				self->pp.longPressEvent = 1;
			}
		}
		//释放事件
		else
		{
			if ((self->pp.pressTime > 0) && (self->pp.pressTime < self->pp.longPressTime))
			{
				self->pp.shortPressEvent = 1;
			}
			self->pp.isKeyPressed = 0;
		}
	}
	else 
	{
		//按下事件
		if (isKeyPressed)
		{
			self->pp.pressTime = 0;
			self->pp.isKeyPressed = 1;
		}
	}

	
	


#if 0	
	/* 检测按键被按下或者被抬起,消抖 */
	if(KEY_SCAN_Time == self->pp.pressTime && isKeyPressed)
	{
		self->pp.pressEvent = 1;
	}
	
	if ((self->pp.pressTime > KEY_SCAN_Time) && !isKeyPressed)
	{
		self->pp.releaseEvent = 1;
	}
	
	/* 记录当前按键状态 */
	self->pp.isKeyPressed = isKeyPressed;
	
	/* 记录长短按 */
	if (self->pp.releaseEvent)
	{
		if (self->pp.pressTime > self->pp.longPressTime)
		{
			self->pp.isLongPress = 1;
			self->pp.isShortPress = 0;
		}
		else
		{
			self->pp.isLongPress = 0;
			self->pp.isShortPress = 1;
		}
	}
	
	/* 记录按键按下时长 */
	if (self->pp.isKeyPressed)
	{
		self->pp.pressTime += KEY_SCAN_Time;
	}
	else
	{
		self->pp.pressTime = 0;
	}
#endif	
}

void Key_Proc(struct Key *self, AP_Inst_t *inst)
{
	if (self->pp.longPressEvent)
	{
		/* 长按处理 */
		if (self->pp.proc_f) {
			self->pp.proc_f(inst);
		}
		self->pp.longPressEvent = 0;
	}

	if (self->pp.shortPressEvent)
	{	
		/*短按处理*/
		self->pp.shortPressEvent = 0;
	}

#if 0
	if (self->pp.isKeyPressed)
	{
	   /* 按键按下处理 */
	}

	if (self->pp.isLongPress)
	{
		/* 长按处理 */
		if (self->pp.proc_f) {
			self->pp.proc_f(inst);
		}
	 
		self->pp.isLongPress = 0;
	}

	if (self->pp.isShortPress)
	{
		/* 短按处理 */
		self->pp.isShortPress = 0;
	}

	if (self->pp.pressEvent)
	{
		/* 按下处理 */
		self->pp.pressEvent = 0;
	}

	if (self->pp.releaseEvent)
	{
		/* 释放处理 */
		self->pp.releaseEvent = 0;
	}
#endif	
}

struct Key *Key_Open(struct Gpio *gpio, bool isLowValid, uint16_t longPressTime, KeyProc_f proc_f)
{
	struct Key *key = (struct Key *)malloc(sizeof(struct Key));
	key->pp.gpio = gpio;
	key->pp.isLowValid = isLowValid;
	key->pp.longPressTime = longPressTime;
	key->pp.proc_f = proc_f;

	key->pp.longPressEvent = FALSE;
	key->pp.shortPressEvent = FALSE;

	key->pp.isKeyPressed = FALSE;
	key->pp.pressEvent = FALSE;
	key->pp.releaseEvent = FALSE;
	key->pp.pressTime = 0;
	key->pp.isLongPress = FALSE;
	key->pp.isShortPress = FALSE;

	key->Scan = Key_Scan;
	key->Proc = Key_Proc;

	return key;
}

