#ifndef _APP_H_
#define _APP_H_
#include "stm8s.h"
#include "app_i2c.h"
#include "app_gpio.h"

#define BOARD_BEING_CONFIG 0xffaa
#define BOARD_RUNNING      0xaa55

//全局设备状态信息
struct device_global_prmt {
	bool IsHomePage;
	bool IsPowerOn;
	bool is_leaf_select;
	bool refresh_menu;
	bool reset_dev;
	bool need_save_prmt;
	bool iwdg_inited; //看门狗初始化完毕
};

void DeviceParameter_InitAndConfig(void);
void Interface_InitAndDisplay(void);
void updata_info(void);

void Encoder_PressorTurnAction(Encoder_HandlerTypeDef *hEncoder);
void EnckSwit_PressorPressAction(Key_HandlerTypeDef *EnckSwit);
void PowerSwit_PressorPressAction(Key_HandlerTypeDef *PowerSwit);
void GeneralSwit_PressorPressAction(Keys_TypeDef *hKeys);

uint8_t CtrlerBoard_ConfigWithNewData(uint8_t group_id, I2C_Slave_CmdTypeDef Cmd, uint16_t Data);
void CtrlerBoard_ReadNewData(uint8_t group_id, I2C_Slave_CmdTypeDef Cmd, uint16_t *Data);
#endif

