#include "Scene.h"
#include "dsp.h"
#include "eeprom.h"
#include "i2c.h"
#include "DI_TypesPublic.h"
#include "tpa5050.h"
#include "Time.h"

void Scene_Load(struct Scene *self, DSP_Scene_e mode)
{
	uint16_t addr = SCENE_A_Addr;
	struct Dsp_Params *dspParams = &self->pp.dspParams;
	switch(mode)
	{
		case DSP_SCENE_A:
			{
				addr = SCENE_A_Addr;
			}
			break;

		case DSP_SCENE_B:
			{
				addr = SCENE_B_Addr;
			}
			break;

		case DSP_SCENE_C:
			{
				addr = SCENE_C_Addr;
			}
			break;

		case DSP_SCENE_Default:
			{
				//LoadScene(self->pp.mode);
				self->Load(self, self->pp.mode);
			}
			break;
	}

	if (mode != DSP_SCENE_Default)
	{
		//SetCurrentScene(mode);
		self->SetCurrentMode(self, mode);
	}

	//��ͷ��ȡ
	Read24C256(EEPROM_SLAVE_Addr, addr, (uint8_t*)&(dspParams->header), sizeof(DSP_ParamHeader_t));
	addr += sizeof(DSP_ParamHeader_t);
	if (dspParams->header.magic == TOPO_MAGIC)
	{
		uint16_t i = 0;

		if (dspParams->header.paramNums > MAX_DSP_MSG_EXT_NUM)
		{
			return;			/* ���������������Чtopo���� */
		}

		//��ʾ��Ҫ���س���������LED��ʾ
		self->pp.loadingLed = 1;
		//������������ȡ
		Read24C256(EEPROM_SLAVE_Addr, addr, (uint8_t*)&(dspParams->msgDspExt[i]), dspParams->header.paramNums*sizeof(MsgDspExt_t));
		//һ��һ������
		for (i = 0; i<dspParams->header.paramNums; i++)
		{
			self->LoadMsgDspExt(self, &(dspParams->msgDspExt[i]), False);
			addr += sizeof(MsgDspExt_t);
		}
	}
}

void Scene_FindMsgDspExt(struct Scene *self, MsgDspExt_t *pMsgDspExt, uint16_t dspAddr, uint16_t *pEepromAddr)
{
	uint16_t addr = SCENE_A_Addr;
	struct Dsp_Params *dspParams = &self->pp.dspParams;
	switch(self->pp.mode)
	{
		case DSP_SCENE_A:
			{
				addr = SCENE_A_Addr;
			}
			break;

		case DSP_SCENE_B:
			{
				addr = SCENE_B_Addr;
			}
			break;

		case DSP_SCENE_C:
			{
				addr = SCENE_C_Addr;
			}
			break;
	}

	addr += sizeof(DSP_ParamHeader_t);
	if (dspParams->header.magic == TOPO_MAGIC)
	{
		uint16_t i = 0;

		for (i = 0; i<dspParams->header.paramNums; i++)
		{
			if (dspParams->msgDspExt[i].address == dspAddr)
			{
				*pEepromAddr = addr+i*sizeof(MsgDspExt_t);
				pMsgDspExt = &dspParams->msgDspExt[i];
				return;
			}
		}
	}
	pMsgDspExt = NULL;
}

void Scene_LoadMsgDspExt(struct Scene *self, MsgDspExt_t *pMsgDspExt, Bool updateEeprom)
{

	Bool needWriteDsp = 1;

	//����address�����������ͣ�����TPA��Ӧֵ
	if (AUDIO_INPUT_Addr == pMsgDspExt->address)
	{
		self->pp.audioInputType = pMsgDspExt->dspParam[3];
	}
	else if (HARDWARE_DELAY1_Addr == pMsgDspExt->address)
	{
		TPA_SetDelay(TPA_5050_Address, (pMsgDspExt->dspParam[2]<<8) + pMsgDspExt->dspParam[3], LEFT_DELAY);
		needWriteDsp = 0;
	}
	else if (HARDWARE_DELAY2_Addr == pMsgDspExt->address)
	{
		TPA_SetDelay(TPA_5050_Address, (pMsgDspExt->dspParam[2]<<8) + pMsgDspExt->dspParam[3], RIGHT_DELAY);
		needWriteDsp = 0;
	}

	//������д��DSP
	if (needWriteDsp)
	{
		if (pMsgDspExt->len > 4)
		{
			SIGMA_SAFELOAD_WRITE_REGISTER(DSP_SLAVE_Addr, 
										pMsgDspExt->address,
										pMsgDspExt->len, 
										pMsgDspExt->dspParam);
		}
		else
		{
			SIGMA_SAFELOAD_WRITE_REGISTER(DSP_SLAVE_Addr, 
										pMsgDspExt->address,
										pMsgDspExt->len, 
										pMsgDspExt->dspParam);
		}
	}

	//�Ķ���ǰ������ĳһ����������ֵ
	if (updateEeprom)
	{
		MsgDspExt_t *pMsgExt = NULL;
		uint16_t eepromAddr;

		//����һ��ȫ�ֳ������Ƿ���ڸ���,�������洢
		self->FindMsgDspExt(self, pMsgExt, pMsgDspExt->address, &eepromAddr);
		if (pMsgExt)
		{
			memcpy(pMsgExt, pMsgDspExt, sizeof(MsgDspExt_t));
			Write24C256(EEPROM_SLAVE_Addr, eepromAddr, (uint8_t*)pMsgDspExt, sizeof(MsgDspExt_t));
		}
	}
}

void Scene_SetCurrentMode(struct Scene *self, DSP_Scene_e mode)
{
	self->pp.mode = mode;
	Write24C256(EEPROM_SLAVE_Addr, SCENE_SEL_Addr, &self->pp.mode, SCENE_SEL_Size);
}

struct Scene *Scene_Open(void)
{
	struct Scene *scene = (struct Scene *)malloc(sizeof(struct Scene));
	
	scene->pp.audioInputType = ANALOG_Input;
	scene->pp.loadingLed = 0;
	
	scene->pp.dspParams.header.magic = 0;
	scene->pp.dspParams.header.paramNums = 0;

	scene->Load = Scene_Load;
	scene->LoadMsgDspExt = Scene_LoadMsgDspExt;
	scene->FindMsgDspExt = Scene_FindMsgDspExt;
	scene->SetCurrentMode = Scene_SetCurrentMode;
	
	//��һ�γ�ʼ��dspģʽ ֻ��ȡһ�Σ�֮�󶼱��浽������
	Read24C256(EEPROM_SLAVE_Addr, SCENE_SEL_Addr, &scene->pp.mode, SCENE_SEL_Size);
	if (scene->pp.mode > DSP_SCENE_C)
	{
		/* Ĭ�ϲ���Aģʽ */
		scene->pp.mode = DSP_SCENE_A;
		scene->SetCurrentMode(scene, DSP_SCENE_A);
	}

	return scene;
}

