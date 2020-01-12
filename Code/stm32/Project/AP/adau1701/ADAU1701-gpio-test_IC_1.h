/**************************************************************************
	File		: ADAU1701-gpio-test_IC_1.h
	Author		: 
	Date		: 2019-3-11
***************************************************************************/
#ifndef __ADAU1701_GPIO_TEST_IC_1_H__
#define __ADAU1701_GPIO_TEST_IC_1_H__
	
/**************************************************************************
    								include
***************************************************************************/
#include <stdint.h>
/**************************************************************************
    								macro
***************************************************************************/
	
/**************************************************************************
    								typedef
***************************************************************************/
#define TOPO_MAGIC								0x55aa55aa

#pragma pack(1)
typedef struct tagDSP_TopoHeader_t
{
	uint32_t magic;
	uint16_t topoId;
	uint16_t topoVersion;
	uint16_t consumerId; 
	uint8_t resv[54];
}DSP_TopoHeader_t;

typedef struct tagDSP_ParamHeader_t
{
	uint32_t magic;				//校验魔幻字
	uint16_t topoId;
	uint16_t topoVersion;
	uint16_t paramNums; 		//场景参数的条数
	uint8_t resv[54];
}DSP_ParamHeader_t;
#pragma pack()	

/**************************************************************************
    								variables
***************************************************************************/

/**************************************************************************
    								functions
***************************************************************************/
void default_download_IC_1(void);
DSP_TopoHeader_t *GetDSPTopoHeader(void);

#endif


