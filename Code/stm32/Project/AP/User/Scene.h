#ifndef _Scene_H_
#define _Scene_H_

#include "I2C.h"
#include "ADAU1701-gpio-test_IC_1.h"
#include "McuMsg.h"

/**************************************************************************
    								macro
***************************************************************************/

#ifndef NULL
#define NULL ((void*)0)
#endif

//三种场景存储在EEPROM的位置
#define SCENE_A_Addr				(0*1024)
#define SCENE_B_Addr				(8*1024)
#define SCENE_C_Addr				(16*1024)

//模式大小
#define SCENE_Size				(8*1024)

//当前模式选择
#define SCENE_SEL_Addr			(24*1024)
#define SCENE_SEL_Size			1

#define AUDIO_INPUT_AlgType		0
#define AUDIO_INPUT_Addr		0
#define HARDWARE_DELAY1_Addr	0x3fe
#define HARDWARE_DELAY2_Addr	0x3ff

#define MAX_DSP_MSG_EXT_NUM		127						/* MODE_Size/sizeof(MsgDspExt_t)-1 */

#define AUDIO_INPUT1_MUTE_Addr	1
#define AUDIO_INPUT2_MUTE_Addr	3


#define AUDIO_INPUT1_GAIN_Addr	5
#define AUDIO_INPUT2_GAIN_Addr	6


/**************************************************************************
    								typedef
***************************************************************************/
typedef enum tagDSP_Scene_e
{
	DSP_SCENE_A,
	DSP_SCENE_B,
	DSP_SCENE_C,
	DSP_SCENE_Default			/* 加载默认场景 */
}DSP_Scene_e;

typedef enum tagAudioInput_e
{
	ANALOG_Input = 0,
	DANTE_Input = 1,
}AudioInput_e;						

struct Dsp_Params{
	DSP_ParamHeader_t header;						//场景头部
	MsgDspExt_t msgDspExt[MAX_DSP_MSG_EXT_NUM];		//场景数据数组
}__attribute__((aligned(1)));

struct Scene_Property{
	uint8_t audioInputType;							/* AudioInput_e */
	uint8_t mode;									/* DSP_Scene_e */
	struct Dsp_Params dspParams;

	uint8_t loadingLed;
};

struct Scene{
	struct Scene_Property pp;
	void (*Load)(struct Scene *self, DSP_Scene_e mode);
	void (*LoadMsgDspExt)(struct Scene *self, MsgDspExt_t *pMsgDspExt, Bool updateEeprom);//给DSP加载一条参数，以及控制关联外设
	void (*FindMsgDspExt)(struct Scene *self, MsgDspExt_t *pMsgDspExt, uint16_t dspAddr, uint16_t *pEepromAddr);
	void (*SetCurrentMode)(struct Scene *self, DSP_Scene_e mode);
};

struct Scene *Scene_Open(void);

#endif

