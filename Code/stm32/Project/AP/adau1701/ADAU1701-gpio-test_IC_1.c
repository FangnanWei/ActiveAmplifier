/*
 * File:           D:\AL2-1701-V1.0\+IC 1_AL2-ADAU1701-V1.0-bk\IC 1_ADAU1701-gpio-test-1\ADAU1701-gpio-test_IC_1.h
 *
 * Created:        Wednesday, November 28, 2018 9:45:49 PM
 * Description:    ADAU1701-gpio-test:IC 1 program data.
 *
 * This software is distributed in the hope that it will be useful,
 * but is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * This software may only be used to program products purchased from
 * Analog Devices for incorporation by you into audio products that
 * are intended for resale to audio product end users. This software
 * may not be distributed whole or in any part to third parties.
 *
 * Copyright ©2018 Analog Devices, Inc. All rights reserved.
 */

#include "ADAU1701-gpio-test_IC_1_REG.h"
#include "ADAU1701-gpio-test_IC_1.h"
#include "DI_TypesPublic.h"
#include "dsp.h"
#include "eeprom.h"
#include "flash_if.h"
#include "iap.h"
#include "common_def.h"
#include "Scene.h"


#define DEVICE_ARCHITECTURE_IC_1                  "ADAU1701"
#define DEVICE_ADDR_IC_1                          0x68

/* DSP Program Data */
#define PROGRAM_SIZE_IC_1 5120
#define PROGRAM_ADDR_IC_1 1024

/* DSP Parameter (Coefficient) Data */
#define PARAM_SIZE_IC_1 4096
#define PARAM_ADDR_IC_1 0

#define R3_HWCONFIGURATION_IC_1_SIZE 24

#define DSP_TOPO_HEADER_ADDRESS					(DSP_FIRST_PAGE_ADDRESS)
#define DSP_PROGRAM_DATA_ADDRESS				(DSP_FIRST_PAGE_ADDRESS+64)
#define DSP_PARAM_DATA_ADDRESS					(DSP_PROGRAM_DATA_ADDRESS+PROGRAM_SIZE_IC_1)
#define DSP_R0_COREREGISTER_IC_1_Default		(DSP_PARAM_DATA_ADDRESS+PARAM_SIZE_IC_1)
#define DSP_R3_HWCONFIGURATION_IC_1_Default		(DSP_R0_COREREGISTER_IC_1_Default+REG_COREREGISTER_IC_1_BYTE)
#define DSP_R4_COREREGISTER_IC_1_Default		(DSP_R3_HWCONFIGURATION_IC_1_Default+R3_HWCONFIGURATION_IC_1_SIZE)


static const DSP_TopoHeader_t* g_pTopoHeader = (DSP_TopoHeader_t*)DSP_FIRST_PAGE_ADDRESS;
static const uint8_t *Program_Data_IC_1 = (const uint8_t*)DSP_PROGRAM_DATA_ADDRESS;
static const uint8_t *Param_Data_IC_1 = (const uint8_t*)DSP_PARAM_DATA_ADDRESS;

/* Register Default - IC 1.CoreRegister */
static const uint8_t *R0_COREREGISTER_IC_1_Default = (const uint8_t*)DSP_R0_COREREGISTER_IC_1_Default;

/* Register Default - IC 1.HWConFiguration */
static const uint8_t *R3_HWCONFIGURATION_IC_1_Default = (const uint8_t*)DSP_R3_HWCONFIGURATION_IC_1_Default;

/* Register Default - IC 1.CoreRegister */
static const uint8_t *R4_COREREGISTER_IC_1_Default = (const uint8_t*)DSP_R4_COREREGISTER_IC_1_Default;

/*
 * Default Download
 */

Bool IsDspTopoValid()
{
	uint32_t DSPIAPFlag = *(uint32_t*)UPDATE_FLASH_FLAG_ADDRESS;
	if (DSPIAPFlag == IAP_SUCCESS)
	{
		return (g_pTopoHeader->magic == TOPO_MAGIC) ;
	}
	else
	{
		return FALSE;
	}
}

DSP_TopoHeader_t *GetDSPTopoHeader(void)
{
	return (DSP_TopoHeader_t*)g_pTopoHeader;
}

void default_download_IC_1(void) 
{
	if (!IsDspTopoValid())
	{
		return ;
	}
	
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1, REG_COREREGISTER_IC_1_ADDR, REG_COREREGISTER_IC_1_BYTE, (uint8_t*)R0_COREREGISTER_IC_1_Default );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1, PROGRAM_ADDR_IC_1, PROGRAM_SIZE_IC_1, (uint8_t*)Program_Data_IC_1 );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1, PARAM_ADDR_IC_1, PARAM_SIZE_IC_1, (uint8_t*)Param_Data_IC_1 );
	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1, REG_COREREGISTER_IC_1_ADDR , R3_HWCONFIGURATION_IC_1_SIZE, (uint8_t*)R3_HWCONFIGURATION_IC_1_Default );

#if 0
	if (g_Scene.audioInputType == ANALOG_Input)
	{
		SIGMA_WRITE_REGISTER_BLOCK(DSP_SLAVE_Addr, 0 ,4, data);
	
	}
	else
	{
		data[3] = 1;
		SIGMA_WRITE_REGISTER_BLOCK(DSP_SLAVE_Addr, 0 ,4, data);
	}
#endif

	SIGMA_WRITE_REGISTER_BLOCK( DEVICE_ADDR_IC_1, REG_COREREGISTER_IC_1_ADDR, REG_COREREGISTER_IC_1_BYTE, (uint8_t*)R4_COREREGISTER_IC_1_Default );
}

