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

//����һ��I2C��д���� ���������� I2C�Ĵ��豸��������ȷ��
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
	
	//�������ݼ�¼
	I2C_DATA_EXTI_HFP_CNT = (uint8_t)0x0a,
	I2C_DATA_EXTI_UVP_CNT = (uint8_t)0x0b,
	I2C_DATA_EXTI_DCP_CNT = (uint8_t)0x0c,
	I2C_DATA_EXTI_OCP1_CNT = (uint8_t)0x0d,
	I2C_DATA_EXTI_OCP2_CNT = (uint8_t)0x0e,

	//�Ӱ�״̬
	I2C_DATA_DEVICE_STATUS = (uint8_t)0xf,

	//���ÿ��ư����Ʋ���
	I2C_DATA_TEMP_GATE = (uint8_t)0x11,
	I2C_DATA_OVV_LIMIT = (uint8_t)0x12,
	I2C_DATA_OCV_LIMIT = (uint8_t)0x13,

	I2C_DATA_LIMIT_STDDT = (uint8_t)0x14,
	I2C_DATA_LIMIT_STPDT = (uint8_t)0x15,
	I2C_DATA_LIMIT_RESTM = (uint8_t)0x16,

	//��������
	I2C_CMD_MASTER = (uint8_t)0x21,								//����չ
	I2C_CMD_NONE				= (uint8_t)0x00,					//��Ч������

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
	I2C_Slave_CmdTypeDef Cmd; 							 					//ͨ��I2C���յ��� ��Ҫ���ڱ�־
	uint16_t Data;																		//������
	uint8_t DataWithCheSum[I2C_DATA_LEN];							//���ֽ����ݺ�һ�ֽ�λУ��  ���ͺͽ���ʱ��ϳ��ü��ж���
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

//��ȡ���ư������ //���Ϳ��ư����ñ�
uint8_t I2C_ControlBoard_GetData(uint8_t grup_id, CtrlerBoard_DataPackTypeDef *DataPack);
uint8_t I2C_ControlBoard_SendData(uint8_t grup_id, CtrlerBoard_DataPackTypeDef *DataPack);

//���ֿ�����  --����ͨ������Ҫ�����Ĳ���  ����ʱ��ֻ��Ҫ֪��ͨ���Ϳ�    ����Ļ���������������������
//pcf8574 �͵�������оƬ�����ֿ��Ʒ���
//����ģʽ�����ѡ��һ����������Ч ����������

//����Դ  ���ģʽ���Ͷ���
typedef enum {DANTE = 0x00, XLR = 0x01} Music_InputSource_TypeDef;
typedef enum {BRIDGE = 0x10, STEREO = 0x11} Music_OutputMode_TypeDef;

typedef struct {
	//�����ĸ������ʹ洢�Ĳ����޸�   �ɴ洢�Ĳ����Ͳ˵��������޸�
	int8_t CurrGain[AUDIO_CHANNEL_CNT];
	bool Mute[AUDIO_CHANNEL_CNT];

	//���ඨ�岢��������������
	Music_InputSource_TypeDef InputSource[CONTROL_BOARD_CNT];
	Music_OutputMode_TypeDef  OutputMode[CONTROL_BOARD_CNT];
}MusicData_TypeDef;

void MusicCtrl_SetInputSourceAndOutputMode(MusicData_TypeDef *hMusicCtrl);
void MusicCtrl_SetChannelMute(uint8_t channel);   //ֱ���農�� ����Ҫ����������
void MusicCtrl_SetChannelGain(uint8_t channel, int8_t CurrGain);
void MusicCtrl_SetChannelFadein(uint8_t channel, int8_t CurrGain);
void MusicCtrl_SetChannelFadeout(uint8_t channel, int8_t CurrGain);

//׼��һ��ԭ��----������Ч�����ݲ�Ҫ�ٵײ�����޸� ��ȡ��ȡɨ����µ������ٵײ��޸�
//eeprom��������
void save_dev_parameter_table(uint8_t *prmt, uint16_t length);
void get_dev_parameter_table(uint8_t *prmt, uint16_t length);

/**********************OLED Start********************************/
void Screen_Init(Screen_InitTypeDef *hScreen);
void Screen_SetInToSleepMode();
void Screen_QuitOutSleepMode();

//ֱ��дOLED�ĺ��� ����Ҫ�������ж� //�ڲ�����Ļ�����������п��Ʋ����Ƿ�Խ��
void Screen_DrawAreaToWhiteOrBlack(uint8_t x_start, uint8_t y_start, uint8_t x_size, uint8_t y_size, bool ToWhite);
void Screen_DrawBmpDatas(uint8_t x_start, uint8_t y_start, uint8_t x_size, uint8_t y_size, uint8_t BMP[], uint32_t draw_speed, bool reverse);

//�˵�������������
void Display_ShowSelectSymbol(Display_HandlerTypeDef *hDisplay);
void Display_ShowPointSymbol(Display_HandlerTypeDef *hDisplay);
void Display_Show12x32BigString(Display_HandlerTypeDef *hDisplay);
void Display_ShowString(Display_HandlerTypeDef *hDisplay);

//������ʾ����
void Display_CleanWholeAreaToBlack(void);
void Display_ShowTendZoneLogo(void);

//����Ļ������������������Ƶ��У�ʵ�ֵ�������
void Display_ShowHomepageGainValue(char channel, signed char gainValue);
void Display_ShowHomepageChannel(char channel, bool isSelect);
void Display_ShowHomepageMute(char channel);
/**********************OLED End********************************/

//I2C������������
void I2C_BSP_Init(I2C_InitTypeDef *hI2C);
uint8_t I2C_BSP_Read(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data, uint16_t length);
uint8_t I2C_BSP_Write(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data, uint16_t length);

#endif

