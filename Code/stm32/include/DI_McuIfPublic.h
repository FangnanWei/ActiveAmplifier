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
  * @brief					: 初始化
  * @param 	pControlDev		: 串口端口号COMx
  * @return value				: 返回句柄
**/
DI_McuIfHandle DI_McuIfInit(const char *pControlDev);

/**
  * @brief					: 去初始化
  * @param 	hdl				: 句柄，下同
  * @return value				:
**/
void DI_McuIfDeInit(DI_McuIfHandle hdl);

/**
  * @brief					: 注册回调函数
  * @param 	cb				: 回调函数
  * @return value				: 见DI_RET_e, 下同
**/
int DI_RegisterMcuChangeFn(DI_McuIfHandle hdl, McuChangeFn_f cb);

/**
  * @brief					: 注销回调函数
  * @param 					:
  * @return value				:
**/
int DI_UnRegisterMcuChangeFn(DI_McuIfHandle hdl);


/**
  * @brief					: 升级
  * @param fw				: 固件路径
  * @return value				:
**/
int DI_McuFirmwareUpgrade(DI_McuIfHandle hdl, const char *fw);

/**
  * @brief					: 获取固件版本号
  * @param fw				: 固件文件路径
  * @param versionBuf		: 版本号存储buffer
  * @param versionBufLen		: buffer大小
  * @return value				:
**/
int DI_McuGetFirmwareVersion(const char *fw, char *versionBuf, int versionBufLen);

/**
  * @brief					: 获取软件版本号
  * @param fw				: 固件路径
  * @param versionBuf		: 版本号存储buffer
  * @param versionBufLen		: buffer大小
  * @return value				:
**/
int DI_McuGetSoftwareVersion(DI_McuIfHandle hdl, char *versionBuf, int versionBufLen);

/**
  * @brief					: 读取Eeprom中的数据
  * @param hdl				: 句柄
  * @param buf				: 存储buffer
  * @param address			: 读取地址
  * @param len				: 读取长度
  * @return value			:
**/
int DI_McuReadEeprom(DI_McuIfHandle hdl, uint8_t *buf, uint16_t address, uint16_t len);
/**
  * @brief					: 写入数据到Eeprom
  * @param hdl				: 句柄
  * @param buf				: 存储buffer
  * @param address			: 写入地址
  * @param len				: 写入长度
  * @return value			:
**/
int DI_McuWriteE2prom(DI_McuIfHandle hdl, uint8_t *buf, uint16_t address, uint16_t len);
/**
  * @brief					: 写入文件到Eeprom
  * @param hdl				: 句柄
  * @param address			: 地址
  * @return value			:
**/
int DI_McuWriteFileToE2prom(DI_McuIfHandle hdl, uint32_t address, const char *fw);

/**
  * @brief					: 读取dsp数据
  * @param hdl				: 句柄
  * @param address			: 地址
  * @return value			:
**/
int DI_McuReadDsp(DI_McuIfHandle hdl, uint8_t *buf, uint16_t address, uint16_t len);

/**
  * @brief					: 写入dsp数据
  * @param hdl				: 句柄
  * @param address			: 地址
  * @return value			:
**/
int DI_McuWriteDsp(DI_McuIfHandle hdl, uint8_t *buf, uint16_t address, uint16_t len);

/**
  * @brief					: 写入safeload dsp数据
  * @param hdl				: 句柄
  * @param address			: 地址
  * @return value			:
**/
int DI_McuSafeloadDsp(DI_McuIfHandle hdl, uint8_t *buf, uint16_t address, uint16_t len);

typedef int (*PrintfFunc)(const char * __fmt, ...);
int DI_McuPrintFnRegister(PrintfFunc printFn);
int DI_McuPrintFnUnregister();
void HexPrint(uint8_t *buf, uint32_t len, uint16_t numPerLine);
#endif

