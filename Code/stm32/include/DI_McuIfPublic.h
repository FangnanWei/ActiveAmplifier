/**************************************************************************
	File		: DI_McuIfPublic.h
	Author		: TaoLei Zhang
	Date		: 2018-1-10
***************************************************************************/
#ifndef _DI_MCU_IF_PUBLIC_H_
#define _DI_MCU_IF_PUBLIC_H_

/**************************************************************************
    								include
***************************************************************************/
#include "DI_TypesPublic.h"
#include "DI_McuTypesPublic.h"
/**************************************************************************
    								macro
***************************************************************************/


/**************************************************************************
    								typedef
***************************************************************************/



typedef void *DI_McuIfHandle;
typedef void (*McuChangeFn_f)(DI_McuIfHandle hdl, DI_McuChangeType_e type, uint8_t channel, uint8_t *buf, uint16_t len);

/**************************************************************************
    								variables
***************************************************************************/

/**************************************************************************
    								functions
***************************************************************************/
/**
  * @brief					: ��ʼ��
  * @param 	pControlDev		: ���ڶ˿ں�COMx
  * @return value				: ���ؾ��
**/
DI_McuIfHandle DI_McuIfInit(const char *pControlDev);

/**
  * @brief					: ȥ��ʼ��
  * @param 	hdl				: �������ͬ
  * @return value				:
**/
void DI_McuIfDeInit(DI_McuIfHandle hdl);

/**
  * @brief					: ע��ص�����
  * @param 	cb				: �ص�����
  * @return value				: ��DI_RET_e, ��ͬ
**/
int DI_RegisterMcuChangeFn(DI_McuIfHandle hdl, McuChangeFn_f cb);

/**
  * @brief					: ע���ص�����
  * @param 					:
  * @return value				:
**/
int DI_UnRegisterMcuChangeFn(DI_McuIfHandle hdl);


/**
  * @brief					: ����
  * @param fw				: �̼�·��
  * @return value				:
**/
int DI_McuFirmwareUpgrade(DI_McuIfHandle hdl, const char *fw);

/**
  * @brief					: ��ȡ�̼��汾��
  * @param fw				: �̼��ļ�·��
  * @param versionBuf		: �汾�Ŵ洢buffer
  * @param versionBufLen		: buffer��С
  * @return value				:
**/
int DI_McuGetFirmwareVersion(const char *fw, char *versionBuf, int versionBufLen);

/**
  * @brief					: ��ȡ����汾��
  * @param fw				: �̼�·��
  * @param versionBuf		: �汾�Ŵ洢buffer
  * @param versionBufLen		: buffer��С
  * @return value				:
**/
int DI_McuGetSoftwareVersion(DI_McuIfHandle hdl, char *versionBuf, int versionBufLen);

/**
  * @brief					: ��ȡEeprom�е�����
  * @param hdl				: ���
  * @param buf				: �洢buffer
  * @param address			: ��ȡ��ַ
  * @param len				: ��ȡ����
  * @return value			:
**/
int DI_McuReadEeprom(DI_McuIfHandle hdl, uint8_t *buf, uint16_t address, uint16_t len);
/**
  * @brief					: д�����ݵ�Eeprom
  * @param hdl				: ���
  * @param buf				: �洢buffer
  * @param address			: д���ַ
  * @param len				: д�볤��
  * @return value			:
**/
int DI_McuWriteE2prom(DI_McuIfHandle hdl, uint8_t *buf, uint16_t address, uint16_t len);
/**
  * @brief					: д���ļ���Eeprom
  * @param hdl				: ���
  * @param address			: ��ַ
  * @return value			:
**/
int DI_McuWriteFileToE2prom(DI_McuIfHandle hdl, uint32_t address, const char *fw);

/**
  * @brief					: ��ȡdsp����
  * @param hdl				: ���
  * @param address			: ��ַ
  * @return value			:
**/
int DI_McuReadDsp(DI_McuIfHandle hdl, uint8_t *buf, uint16_t address, uint16_t len);

/**
  * @brief					: д��dsp����
  * @param hdl				: ���
  * @param address			: ��ַ
  * @return value			:
**/
int DI_McuWriteDsp(DI_McuIfHandle hdl, uint8_t *buf, uint16_t address, uint16_t len);

/**
  * @brief					: д��safeload dsp����
  * @param hdl				: ���
  * @param address			: ��ַ
  * @return value			:
**/
int DI_McuSafeloadDsp(DI_McuIfHandle hdl, uint8_t *buf, uint16_t address, uint16_t len);

typedef int (*PrintfFunc)(const char * __fmt, ...);
int DI_McuPrintFnRegister(PrintfFunc printFn);
int DI_McuPrintFnUnregister();
void HexPrint(uint8_t *buf, uint32_t len, uint16_t numPerLine);
#endif

