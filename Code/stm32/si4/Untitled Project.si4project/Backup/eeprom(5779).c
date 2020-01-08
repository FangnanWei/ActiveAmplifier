 
 /**************************************************************************
	File		: eeprom.c
	Date		: 2018-7-11
***************************************************************************/

	
/**************************************************************************
    								include
***************************************************************************/
#include "dsp.h"
#include "eeprom.h"
#include "i2c.h"
#include "DI_TypesPublic.h"
#include "tpa5050.h"
#include "Time.h"

/**************************************************************************
    								macro
***************************************************************************/

/**************************************************************************
    								typedef
***************************************************************************/	

/**************************************************************************
    								variables
***************************************************************************/
DSP_Scene_t g_Scene;
MsgDspExt_t g_MsgDspExt;
DSP_ParamHeader_t g_DSP_ParamHeader;

	
/**************************************************************************
    								functions
***************************************************************************/
/* 成功返回0 */
uint8_t Read24C256(uint8_t slaveAddr, uint16_t addr, uint8_t *buf, uint16_t len)
{
	return I2C_Read_16(I2C2, slaveAddr, addr, buf, len);
}

/* 成功返回0 */
uint8_t Write24C256(uint8_t slaveAddr, uint16_t addr, uint8_t *buf, uint16_t len)
{
	uint16_t i = 0;
	uint8_t *pBuf = buf;
	int wlen = (uint16_t)len;

#if 1
	//for page align
	if (addr % EEPROM_PAGE_Size)
	{
		I2C_Write_16(I2C2, slaveAddr, addr, buf, EEPROM_PAGE_Size-(addr%EEPROM_PAGE_Size));
		addr = addr + EEPROM_PAGE_Size-(addr%EEPROM_PAGE_Size);
		pBuf = &buf[EEPROM_PAGE_Size-(addr%EEPROM_PAGE_Size)];
		wlen = wlen - (EEPROM_PAGE_Size-(addr%EEPROM_PAGE_Size)); 
	}
#endif

	while(wlen > 0)
	{
		if (wlen > EEPROM_PAGE_Size)
		{
			I2C_Write_16(I2C2, slaveAddr, addr + i*EEPROM_PAGE_Size, &pBuf[i*EEPROM_PAGE_Size], EEPROM_PAGE_Size);
		}
		else
		{
			I2C_Write_16(I2C2, slaveAddr, addr + i*EEPROM_PAGE_Size, &pBuf[i*EEPROM_PAGE_Size], wlen);
		}
		wlen = wlen - EEPROM_PAGE_Size;
		i++;
		Time_SleepMs(20);
	}
	return 0;
}

static __inline uint16_t GetSceneAddress(DSP_Scene_e mode)
{
	switch(mode)
	{
		case DSP_MODE_A:		return MODE_A_Addr;
		case DSP_MODE_B:		return MODE_B_Addr;
		case DSP_MODE_C:		return MODE_C_Addr;
	}
}

DSP_Scene_e ReadDSP_ModeFromEEPROM()
{
	uint8_t mode = 0;

	Read24C256(EEPROM_SLAVE_Addr, MODE_SEL_Addr, &mode, 1);
	return mode;
}

bool WriteDSP_ModeToEEPROM(DSP_Scene_e mode)
{
	uint8_t val = mode;

	if (0 != Write24C256(EEPROM_SLAVE_Addr, MODE_SEL_Addr, &val, 1))
	{
		return FALSE;
	}
	return TRUE;
}


/* 错误是返回0xffff */
uint16_t ReadDelayFromEEPROM()
{
	uint16_t delay = 0xffff;
	
	return delay;
}

bool WriteDelayToEEPROM(uint16_t delay)
{

	return TRUE;
}

DSP_Scene_e GetCurrentScene(void)
{
	static uint8_t hasReaded = 0;
 
	if (!hasReaded)
	{
		hasReaded = 1;
		g_Scene.audioInputType = ANALOG_Input;
		g_Scene.header.magic = 0;
		g_Scene.header.paramNums = 0;
		g_Scene.mode = ReadDSP_ModeFromEEPROM();
		if (g_Scene.mode > DSP_MODE_C)
		{
			/* 默认采用A模式 */
			g_Scene.mode = DSP_MODE_A;
			SetCurrentScene(DSP_MODE_A);
		}
	}
	return g_Scene.mode;
}

void SetCurrentScene(DSP_Scene_e mode)
{
	g_Scene.mode = mode;
	WriteDSP_ModeToEEPROM(mode);
}

MsgDspExt_t* FindMsgDspExtByDspAddr(uint16_t dspAddr, uint16_t *pEepromAddr)
{
	uint16_t addr = MODE_A_Addr;
	
	switch(GetCurrentScene())
	{
		case DSP_MODE_A:
			{
				addr = MODE_A_Addr;
			}
			break;

		case DSP_MODE_B:
			{
				addr = MODE_B_Addr;
			}
			break;

		case DSP_MODE_C:
			{
				addr = MODE_C_Addr;
			}
			break;
	}

	addr += sizeof(DSP_ParamHeader_t);
	if (g_Scene.header.magic == TOPO_MAGIC)
	{
		uint16_t i = 0;

		for (i = 0; i<g_Scene.header.paramNums; i++)
		{
			if (g_Scene.msgDspExt[i].address == dspAddr)
			{
				*pEepromAddr = addr+i*sizeof(MsgDspExt_t);
				return &g_Scene.msgDspExt[i];
			}
		}
	}
	return NULL;
}

void LoadMsgDspExt(MsgDspExt_t *pMsgDspExt, Bool updateEeprom)
{
	Bool needWriteDsp = 1;
	
	if (AUDIO_INPUT_Addr == pMsgDspExt->address)
	{
		g_Scene.audioInputType = pMsgDspExt->dspParam[3];
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

	if (updateEeprom)
	{
		MsgDspExt_t *pMsgExt;
		uint16_t eepromAddr;
		
		pMsgExt = FindMsgDspExtByDspAddr(pMsgDspExt->address, &eepromAddr);
		if (pMsgExt)
		{
			memcpy(pMsgExt, pMsgDspExt, sizeof(MsgDspExt_t));
			Write24C256(EEPROM_SLAVE_Addr, eepromAddr, (uint8_t*)pMsgDspExt, sizeof(MsgDspExt_t));
		}
	}
}

void LoadScene(DSP_Scene_e mode)
{
	uint16_t addr = MODE_A_Addr;
	switch(mode)
	{
		case DSP_MODE_A:
			{
				addr = MODE_A_Addr;
			}
			break;

		case DSP_MODE_B:
			{
				addr = MODE_B_Addr;
			}
			break;

		case DSP_MODE_C:
			{
				addr = MODE_C_Addr;
			}
			break;

		case DSP_MODE_Default:
			{
				LoadScene(GetCurrentScene());
			}
			break;
	}

	if (mode != DSP_MODE_Default)
	{
		SetCurrentScene(mode);
	}

	Read24C256(EEPROM_SLAVE_Addr, addr, (uint8_t*)&(g_Scene.header), sizeof(DSP_ParamHeader_t));
	addr += sizeof(DSP_ParamHeader_t);
	if (g_Scene.header.magic == TOPO_MAGIC)
	{
		uint16_t i = 0;

		if (g_Scene.header.paramNums > MAX_DSP_MSG_EXT_NUM)
		{
			return;			/* 超过最大数量，无效topo参数 */
		}

		Read24C256(EEPROM_SLAVE_Addr, addr, (uint8_t*)&(g_Scene.msgDspExt[i]), g_Scene.header.paramNums*sizeof(MsgDspExt_t));
		for (i = 0; i<g_Scene.header.paramNums; i++)
		{
			LoadMsgDspExt(&(g_Scene.msgDspExt[i]), False);
			addr += sizeof(MsgDspExt_t);
		}
	}
}


