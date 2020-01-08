#ifndef _APP_I2C_H_
#define _APP_I2C_H_
#include "stm8s.h"

#define  AUDIO_CHANNEL_CNT 4
#define  CONTROL_BOARD_CNT 2

#define MAX_SHOW_LINE 				4
#define MAX_SHOW_LENGTH             15	

#define MIN_GAIN_VAL -80
#define MAX_GAIN_VAL 0
#define I2C_DATA_LEN 3

//定义一组I2C读写命令 必须有命令 I2C的从设备操作才能确定
typedef enum{
	I2C_DATA_POWER_VOLTAGE = (uint8_t)0x01,
	I2C_DATA_POWER_CURRENT = (uint8_t)0x02,
	
	I2C_DATA_DEVICE_TRMPERATURE = (uint8_t)0x03,
	I2C_DATA_AUDIO_CH1_VOLTAGE = (uint8_t)0x04,
	I2C_DATA_AUDIO_CH1_CURRENT = (uint8_t)0x05,
	I2C_DATA_AUDIO_CH1_IMPEDANCE = (uint8_t)0x06,
	I2C_DATA_AUDIO_CH2_VOLTAGE = (uint8_t)0x07,
	I2C_DATA_AUDIO_CH2_CURRENT = (uint8_t)0x08,
	I2C_DATA_AUDIO_CH2_IMPEDANCE = (uint8_t)0x09,
	
	//错误数据记录
	I2C_DATA_EXTI_HFP_CNT = (uint8_t)0x0a,
	I2C_DATA_EXTI_UVP_CNT = (uint8_t)0x0b,
	I2C_DATA_EXTI_DCP_CNT = (uint8_t)0x0c,
	I2C_DATA_EXTI_OCP1_CNT = (uint8_t)0x0d,
	I2C_DATA_EXTI_OCP2_CNT = (uint8_t)0x0e,

	//从板状态
	I2C_DATA_DEVICE_STATUS = (uint8_t)0xf,

	//配置控制版限制参数
	I2C_DATA_TEMP_GATE = (uint8_t)0x11,
	I2C_DATA_OVV_LIMIT = (uint8_t)0x12,
	I2C_DATA_OCV_LIMIT = (uint8_t)0x13,

	I2C_DATA_LIMIT_STDDT = (uint8_t)0x14,
	I2C_DATA_LIMIT_STPDT = (uint8_t)0x15,
	I2C_DATA_LIMIT_RESTM = (uint8_t)0x16,

	//主机命令
	I2C_CMD_MASTER = (uint8_t)0x21,								//可拓展
	I2C_CMD_NONE				= (uint8_t)0x00,					//无效空数据

}I2C_Slave_CmdTypeDef;

typedef struct {
	uint16_t PowerVoltage;
	uint16_t PowerCurrent;
	uint16_t DeviceTemperature;
	uint16_t AudioCh1Voltage;
	uint16_t AudioCh1Current;
	uint16_t AudioCh1Impedence;

	uint16_t AudioCh2Voltage;
	uint16_t AudioCh2Current;
	uint16_t AudioCh2Impedence;

	uint16_t ExtiHFPCnt;
	uint16_t ExtiUVPCnt;
	uint16_t ExtiDCPCnt;
	uint16_t ExtiOCP1Cnt;
	uint16_t ExtiOCP2Cnt;

	uint16_t DeviceStatus;
}CtrlBoard_Meters_TypeDef;

typedef struct {
	uint16_t TemperatureGate;
	uint16_t OVVLimit;
	uint16_t OCVLimit;
	uint16_t STDDTVal;
	uint16_t STPDTVal;
	uint16_t RESTMVal;
}CtrlBoard_Config_TypeDef;

typedef struct {
	CtrlBoard_Meters_TypeDef Meters;
	CtrlBoard_Config_TypeDef Config;
}CtrlBoard_Data_TypeDef;


typedef struct {
	I2C_Slave_CmdTypeDef Cmd; 							 					//通过I2C接收到的 主要用于标志
	uint16_t Data;																		//纯数据
	uint8_t DataWithCheSum[I2C_DATA_LEN];							//两字节数据和一字节位校验  发送和接收时候合成用及判断用
}CtrlerBoard_DataPackTypeDef;

typedef struct
{
	GPIO_InitTypeDef GpioRest;
	GPIO_InitTypeDef GpioEnable;
}Screen_InitTypeDef;

typedef struct {
	uint8_t HorizontalStart;
	uint8_t Line;
	char *Str;
	bool Reverse;
}Display_HandlerTypeDef;

//读取控制板参数表 //发送控制板配置表
uint8_t I2C_ControlBoard_GetData(uint8_t grup_id, CtrlerBoard_DataPackTypeDef *DataPack);
uint8_t I2C_ControlBoard_SendData(uint8_t grup_id, CtrlerBoard_DataPackTypeDef *DataPack);

//音乐控制器  --除了通道以外要包含的参数  控制时候只需要知道通道就可    主屏幕界面包含音量控制器即可
//pcf8574 和电子音量芯片的音乐控制方法
//输入模式和输出选择一次性设置生效 不单独设置

//输入源  输出模式类型定义
typedef enum {DANTE = 0x00, XLR = 0x01} Music_InputSource_TypeDef;
typedef enum {BRIDGE = 0x10, STEREO = 0x11} Music_OutputMode_TypeDef;

typedef struct {
	//由于四个按键和存储的参数修改   由存储的参数和菜单的设置修改
	int8_t CurrGain[AUDIO_CHANNEL_CNT];
	bool Mute[AUDIO_CHANNEL_CNT];

	//分类定义并包含在主参数中
	Music_InputSource_TypeDef InputSource[CONTROL_BOARD_CNT];
	Music_OutputMode_TypeDef  OutputMode[CONTROL_BOARD_CNT];
}MusicData_TypeDef;

void MusicCtrl_SetInputSourceAndOutputMode(MusicData_TypeDef *hMusicCtrl);
void MusicCtrl_SetChannelMute(uint8_t channel);   //直接设静音 不需要淡出操作了
void MusicCtrl_SetChannelGain(uint8_t channel, int8_t CurrGain);
void MusicCtrl_SetChannelFadein(uint8_t channel, int8_t CurrGain);
void MusicCtrl_SetChannelFadeout(uint8_t channel, int8_t CurrGain);

//准守一个原则----设置生效的数据不要再底层进行修改 获取读取扫描更新的数据再底层修改
//eeprom操作方法
void save_dev_parameter_table(uint8_t *prmt, uint16_t length);
void get_dev_parameter_table(uint8_t *prmt, uint16_t length);

/**********************OLED Start********************************/
void Screen_Init(Screen_InitTypeDef *hScreen);
void Screen_SetInToSleepMode();
void Screen_QuitOutSleepMode();

//直接写OLED的函数 。需要做出错判断 //在操作屏幕的两个函数中控制操作是否越界
void Screen_DrawAreaToWhiteOrBlack(uint8_t x_start, uint8_t y_start, uint8_t x_size, uint8_t y_size, bool ToWhite);
void Screen_DrawBmpDatas(uint8_t x_start, uint8_t y_start, uint8_t x_size, uint8_t y_size, uint8_t BMP[], uint32_t draw_speed, bool reverse);

//菜单基本操作函数
void Display_ShowSelectSymbol(Display_HandlerTypeDef *hDisplay);
void Display_ShowPointSymbol(Display_HandlerTypeDef *hDisplay);
void Display_Show12x32BigString(Display_HandlerTypeDef *hDisplay);
void Display_ShowString(Display_HandlerTypeDef *hDisplay);

//特殊显示内容
void Display_CleanWholeAreaToBlack(void);
void Display_ShowTendZoneLogo(void);

//主屏幕界面可以做到音量控制当中，实现单独控制
void Display_ShowHomepageGainValue(char channel, signed char gainValue);
void Display_ShowHomepageChannel(char channel, bool isSelect);
void Display_ShowHomepageMute(char channel);
/**********************OLED End********************************/

//I2C基本操作方法
void I2C_BSP_Init(I2C_InitTypeDef *hI2C);
uint8_t I2C_BSP_Read(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data, uint16_t length);
uint8_t I2C_BSP_Write(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data, uint16_t length);

#endif

