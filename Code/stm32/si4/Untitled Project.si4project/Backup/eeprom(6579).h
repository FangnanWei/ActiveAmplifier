/**************************************************************************
	File		: eeprom.h
	Date		: 2018-7-11
***************************************************************************/
#ifndef _EEPROM_H_
#define _EEPROM_H_

/**************************************************************************
    								include
***************************************************************************/
#include "I2C.h"
#include "ADAU1701-gpio-test_IC_1.h"
#include "McuMsg.h"

/**************************************************************************
    								macro
***************************************************************************/
#define EEPROM_SLAVE_Addr		0xa0
#define EEPROM_PAGE_Size		64

#if 0
/* Parameter RAM 1024 x 32 Direct write1 safeload write */
#define EEPROM_DSP_PARAM_ADDR			0000
#define EEPROM_DSP_PARAM_SIZE			4096

/* Program RAM 1024 x 40 Direct write */
#define EEPROM_DSP_PROGRAM_ADDR			4096
#define EEPROM_DSP_PROGRAM_SIZE			5120

/* The total size of DSP Program and Param is 9KB = 0x2400 */
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

#define offset(member, type)			(&(((type*)0)->member))

#define MODE_A_Addr				(0*1024)
#define MODE_B_Addr				(8*1024)
#define MODE_C_Addr				(16*1024)

#define MODE_Size				(8*1024)


#define MODE_SEL_Addr			(24*1024)
#define MODE_SEL_Size			1

#define AUDIO_INPUT_AlgType		0
#define AUDIO_INPUT_Addr		0
#define HARDWARE_DELAY1_Addr	0x3fe
#define HARDWARE_DELAY2_Addr	0x3ff

#define MAX_DSP_MSG_EXT_NUM		127						/* MODE_Size/sizeof(MsgDspExt_t)-1 */


/**************************************************************************
    								typedef
***************************************************************************/
typedef enum tagDSP_Scene_e
{
	DSP_MODE_A,
	DSP_MODE_B,
	DSP_MODE_C,
	DSP_MODE_Default			/* º”‘ÿƒ¨»œ≥°æ∞ */
}DSP_Scene_e;

typedef enum tagAudioInput_e
{
	ANALOG_Input = 0,
	DANTE_Input = 1,
}AudioInput_e;						

typedef struct tagDSP_Scene_t
{
	DSP_ParamHeader_t header;
	MsgDspExt_t msgDspExt[MAX_DSP_MSG_EXT_NUM];	
	uint8_t audioInputType;							/* AudioInput_e */
	uint8_t mode;									/* DSP_Scene_e */
}__attribute__((aligned(1))) DSP_Scene_t;			

/**************************************************************************
    								variables
***************************************************************************/
extern DSP_Scene_t g_Scene;

/**************************************************************************
    								functions
***************************************************************************/
uint8_t Read24C256(uint8_t slaveAddr, uint16_t addr, uint8_t *buf, uint16_t len);
uint8_t Write24C256(uint8_t slaveAddr, uint16_t addr, uint8_t *buf, uint16_t len);
DSP_Scene_e GetCurrentScene(void);
void SetCurrentScene(DSP_Scene_e mode);
void LoadMsgDspExt(MsgDspExt_t *pMsgDspExt, Bool updateEeprom);
void LoadScene(DSP_Scene_e mode);


#endif

