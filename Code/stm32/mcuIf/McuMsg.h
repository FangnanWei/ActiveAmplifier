/*************************************************
				McuMsg.h
*************************************************/
#ifndef __MCU_MSG_H__
#define __MCU_MSG_H__

/*************************************************
				include
*************************************************/
#include "DI_TypesPublic.h"
#include "DI_McuTypesPublic.h"
#include "comfifo.h"
/*************************************************
				macro
*************************************************/
#define MCU_MSG_MAGIC				(0xA55A)
#define MCU_MSG_DATA_MAX_LEN		1024
#define MCU_IAP_MSG_DATA_MAX_LEN	256		//兼容iap数据包长度

/*************************************************
				typedef
*************************************************/
/* 为了版本兼容性，只可在后端增加，不可修改值 */
typedef enum tagMcuMsgType_e
{
	MSG_Butt					= 0,	
	MSG_MCU_Reboot				= 10,			/* 擦除升级标记位并重启MCU */
	MSG_DSP_TOPO_UpdateMode		= 11,			/* 进入DSP TOPO升级模式 */
	MSG_VERSION_Get				= 20,

	MSG_EEPROM_Read				= 51,
	MSG_EEPROM_Write			= 52,
	MSG_DSP_Read				= 53,
	MSG_DSP_Write				= 54,
	MSG_DSP_Safeload			= 55,
	MSG_DSP_TopoHeader			= 56,
	MSG_DSP_WriteWithExt		= 57,			/* 除了写入dsp寄存器数据外还有额外数据 */
	MSG_DSP_SafeloadWithExt		= 58,			/* 除了写入dsp寄存器数据外还有额外数据 */

	MSG_DSP_AUDIO_INPUT_Change	= 70,
	MSG_DSP_SCENE_Change		= 71,

	/* 升级 */
	MSG_UPGRADE_Begin			= 80,
	MSG_UPGRADE_Data			= 81,
	MSG_UPGRADE_End				= 82,
	MSG_UPGRADE_Ack				= 83,
	MSG_UPGRADE_Nack			= 84,

}McuMsgType_e;

#pragma pack(1)
typedef struct tagMcuMsgHeader_t{
	uint16_t magic;
	uint16_t len;
	uint16_t msgType;
	uint16_t reqId;
	uint16_t errCode;
	uint16_t resv;
	uint32_t crc;
} McuMsgHeader_t;

typedef struct tagMcuMsgContent_t
{
	uint8_t data[MCU_MSG_DATA_MAX_LEN];
} McuMsgContent_t;

typedef struct tagMcuMsg_t
{
	McuMsgHeader_t header;				/* 头 */
	McuMsgContent_t content;			/* 内容 */
} McuMsg_t;

typedef struct tagMsgE2prom_t
{
	uint16_t address;
	uint16_t len;
}MsgE2prom_t;

typedef struct tagMsgDsp_t
{
	uint16_t address;
	uint16_t len;
}MsgDsp_t;

typedef struct tagMsgDspExt_t
{
	uint8_t type;
	uint16_t address;				/* dsp寄存器地址 */
	uint8_t len;					/* dsp寄存器对应参数长度 */
	uint8_t dspParam[20];			/* dsp寄存器参数的值 */
	uint8_t extData[40];			/* 附加数据的值 */
}MsgDspExt_t;
#pragma pack()



/*************************************************
				variables
*************************************************/

/*************************************************
				function
*************************************************/
uint32_t Calcrc32(const uint8_t *buf, uint32_t size);
uint32_t CalcMsgCrc(McuMsg_t *pMsg);
Bool CheckMsgCrc(McuMsg_t *pMsg);
void MakeMcuMsg(McuMsg_t *pMsg, uint8_t *buf, uint16_t len, DI_RET_e code, McuMsgType_e msgType, uint16_t reqId);
Bool GetMcuMsg(ComFifo_t *pComFifo, McuMsg_t *pMsg);


struct AP_BaseDriver{
	struct I2c * i2c;
};

struct AP_Driver{
	struct AP_BaseDriver base;

	struct Tpa5050 *tpa;
	struct Eeprom *eeprom;
	struct Dsp *dsp;

	struct Adc *adc;
};


typedef struct AP_Instance AP_Inst_t;
typedef int32_t (*AP_NetMsg_t)(AP_Inst_t *inst, McuMsg_t *msg);

struct AP_Instance{
	AP_NetMsg_t msg[100];
	struct AP_Driver drivers;
	struct Scene *scene;
};

AP_Inst_t *AP_Init(void);


#endif
