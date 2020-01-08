#include "app_i2c.h"
#include "app_gpio.h"
#include "app_tim.h"
#include "string.h"
#include "app_beep.h"

enum {
    AT24C02_ADDR				= 0xA0,//AT24C02
    STM8_LEFT_ADDR				= 0x50,//功率模块0
    STM8_RIGHT_ADDR				= 0x52,//功率模块1
    NJU72342_ADDR				= 0x80,//NJU72342电子音量芯片
    OLED_ADDR					= 0x78, //OLED显示屏
    PCF8574_ADDR				= 0x40 //PCF8574音频选择芯片,输入源，输出模式
} i2c_addr_TypeDef;

/*****************************************
-	nju72342
-
*****************************************/
uint8_t write_stm8_slave_data(uint8_t grup_id, uint8_t cmd_sign, uint8_t *data, uint16_t length)
{
	if (grup_id == 0) 
	{
		return I2C_BSP_Write(STM8_LEFT_ADDR, cmd_sign, data, length);
	}
	else if (grup_id == 1)
	{
		return I2C_BSP_Write(STM8_RIGHT_ADDR, cmd_sign, data, length);
	}
	else 
	{
		return 1;
	}
}

uint8_t read_stm8_slave_data(uint8_t grup_id, uint8_t cmd_sign, uint8_t *data, uint16_t length)
{
	if (grup_id == 0) 
	{
		return I2C_BSP_Read(STM8_LEFT_ADDR, cmd_sign, data, length);

	} 
	else if(grup_id == 1)
	{
		return I2C_BSP_Read(STM8_RIGHT_ADDR, cmd_sign, data, length);
	} 
	else 
	{
		return 1;
	}
}

uint8_t I2C_ControlBoard_GetData(uint8_t grup_id, CtrlerBoard_DataPackTypeDef *DataPack)
{
	uint8_t ret = 1;

	//打乱数据后,读回校验
 	DataPack->DataWithCheSum[0] = 0x00;
 	DataPack->DataWithCheSum[1] = 0xFF;
	DataPack->DataWithCheSum[2] = 0x55;
	
	ret = read_stm8_slave_data(grup_id, DataPack->Cmd, DataPack->DataWithCheSum, I2C_DATA_LEN);
	if (ret != 0)
	{
		return 1;
	}

	//判断校验是否正确
	if (DataPack->DataWithCheSum[2] != (DataPack->DataWithCheSum[0] | DataPack->DataWithCheSum[1]))
	{
		return 1;
	}

	//拷贝数据
	DataPack->Data = (((uint16_t)DataPack->DataWithCheSum[0]<<8) | (uint16_t)DataPack->DataWithCheSum[1]);
	return 0;
}

uint8_t I2C_ControlBoard_SendData(uint8_t grup_id, CtrlerBoard_DataPackTypeDef *DataPack)
{
	uint8_t ret = 1;
	uint16_t DataTemp;
	
	//合成协议数据参数
	DataPack->DataWithCheSum[0] = (uint8_t)(DataPack->Data>>8);
	DataPack->DataWithCheSum[1] = (uint8_t)(DataPack->Data & 0x00ff);
	DataPack->DataWithCheSum[2] = (DataPack->DataWithCheSum[0] | DataPack->DataWithCheSum[1]);

	ret = write_stm8_slave_data(grup_id, DataPack->Cmd, DataPack->DataWithCheSum, I2C_DATA_LEN);
	if (ret != 0)
	{
		return 1;
	}

	//读回校验
	DelayNop(50);
	DataTemp = DataPack->Data;
	
	ret = I2C_ControlBoard_GetData(grup_id, DataPack);
	if (ret != 0)
	{
		return 1;
	}

	//判断数据是否正确  重试一次
	if (DataPack->Data != DataTemp)
	{
		return 1;
	}
	DelayNop(30);
	
	return 0;
}

/*****************************************
-	nju72342
-
*****************************************/
#define MUTE_VAL            0x00
#define ZERO_CROSS_DETECT   0x80
#define ATTEN_0_DB          0x77

//#define ATTEN_TOTAL_Level     80
//#define ADC_MAX_Value       0x3ff
//#define ADC_MIN_Value       0x0
//#define ATTEN_MAX_Value     79
//#define ATTEN_MIN_Value     0
//#define ATTEN_CLP_Value     3

uint8_t I2C_NJU_WriteData(uint8_t channel, uint8_t value)
{
	return I2C_BSP_Write(NJU72342_ADDR, channel + 1, &value, 1);
}

/*****************************************
-	PCF8574
-
*****************************************/
void MusicCtrl_SetInputSourceAndOutputMode(MusicData_TypeDef *hMusicCtrl)
{
	//根据输入源和输出模式参数 合成一个8位的数据 
	uint8_t PinOffset = 0x00;
	uint8_t PcfChipData = 0xff;
	for (uint8_t index = 0; index < CONTROL_BOARD_CNT; index++)
	{
		PinOffset = (index == 0)? 0 : 1;
		PcfChipData &= (~(1 << PinOffset));
		if (hMusicCtrl->InputSource[index] != DANTE)
		{
			PcfChipData |= (0<<PinOffset);
		}
		else
		{
			PcfChipData |= (1<<PinOffset);
		}
	}

	for (uint8_t index = 0; index < CONTROL_BOARD_CNT; index++)
	{
		PinOffset = (index == 0)? 6 : 7;
		PcfChipData &= (~(1 << PinOffset));
		if (hMusicCtrl->OutputMode[index] != BRIDGE)
		{
			PcfChipData |=	(0<<PinOffset);
		}
		else
		{
			PcfChipData |=	(1<<PinOffset);
		}
	}

	// 写入到芯片中即可
	I2C_BSP_Write(PCF8574_ADDR, PcfChipData, NULL, 0);
}

//传递通道只是告知要控制哪个通道
void MusicCtrl_SetChannelMute(uint8_t channel)
{
	uint8_t value;

	value = MUTE_VAL | ZERO_CROSS_DETECT;

	I2C_NJU_WriteData(channel, value);
}

void MusicCtrl_SetChannelGain(uint8_t channel, int8_t CurrGain)
{
	uint8_t value = (ATTEN_0_DB + CurrGain) | ZERO_CROSS_DETECT;

	I2C_NJU_WriteData(channel, value);
}

void MusicCtrl_SetChannelFadein(uint8_t channel, int8_t CurrGain)
{
  int8_t gain = MIN_GAIN_VAL;

	for (; gain <= CurrGain; gain++) {
		MusicCtrl_SetChannelGain(channel, gain);
		DelayNopMs(8);
	}
}

void MusicCtrl_SetChannelFadeout(uint8_t channel, int8_t CurrGain)
{
	for (; CurrGain >= MIN_GAIN_VAL; CurrGain--) {
		MusicCtrl_SetChannelGain(channel, CurrGain);
		DelayNopMs(8);
	}
}

/*****************************************
-	OLED
-
*****************************************/
//基本操作方法
void I2C_OLED_WriteACmd(uint8_t cmd)
{
    I2C_BSP_Write(OLED_ADDR, 0x00, &cmd, 1);
}

void I2C_OLED_WriteAData(uint8_t data)
{
    I2C_BSP_Write(OLED_ADDR, 0x40, &data, 1);
}

void I2C_OLED_WriteArrayDatas(uint8_t *data, uint8_t length)
{
	I2C_BSP_Write(OLED_ADDR, 0x40, data, length);
}

//OLED命令
void OLED_SetColumnAddress4LowerBits(uint8_t addr)
{
	I2C_OLED_WriteACmd(addr&0x0f);
}

void OLED_SetColumnAddress4HigherBits(uint8_t addr)
{
	I2C_OLED_WriteACmd((addr>>4)|0x10);
}

void OLED_SetPumpVoltageValue(uint8_t voltage)
{
	I2C_OLED_WriteACmd((voltage&0x03) | 0x30);
}

void OLED_SetDisplayStartLine(uint8_t addr)
{
	I2C_OLED_WriteACmd((addr&0x3f) | 0x40);
}

void OLED_SetContrastControl(uint8_t contrast)
{
	I2C_OLED_WriteACmd(0x81);
	I2C_OLED_WriteACmd(contrast);
}

void OLED_SetSegmentRemap(bool isright)
{
	I2C_OLED_WriteACmd(0xa0 | (isright ? 0 : 1));
}

void OLED_SetEntireDisplayOnOrOff(bool ison)
{
	I2C_OLED_WriteACmd(0xa4 | (ison ? 0 : 1));
}

void OLED_SetNormalOrReverseDisplayer(bool isnormal)
{
	I2C_OLED_WriteACmd(0xa6 | (isnormal ? 0 : 1));
}

void  OLED_SetMultiplexRation(uint8_t ration)
{
	I2C_OLED_WriteACmd(0xa8);
	I2C_OLED_WriteACmd(ration&0x3f);
}

void OLED_SetDcDcControlOnOrOff(bool ison)
{
	I2C_OLED_WriteACmd(0xad);
	I2C_OLED_WriteACmd(0x8a | (ison ? 1 : 0));
}

void OLED_SetDisplayOnOrOff(bool ison)
{
	I2C_OLED_WriteACmd(0xae | (ison ? 1 : 0));
}

void OLED_SetPageAddress(uint8_t addr)
{
	I2C_OLED_WriteACmd((addr&0x0f) | 0xb0);
}

void OLED_SetCommonOutputScanDirection(bool islowerfirst)
{
	I2C_OLED_WriteACmd(0xc0 | ((islowerfirst ? 0 : 1)<<3));
}

void OLED_SetDisplayOffset(uint8_t offset)
{
	I2C_OLED_WriteACmd(0xd3);
	I2C_OLED_WriteACmd(offset&0x3f);
}

void OLED_SetDivideRatioOrOscillatorFrequency(uint8_t ratio, uint8_t frequency)
{
	I2C_OLED_WriteACmd(0xd5);
	I2C_OLED_WriteACmd((ratio&0x0f) | ((frequency&0x0f) << 4));
}

void OLED_SetChargeOrPreChargePeriod(uint8_t dis_period, uint8_t pre_period)
{
	I2C_OLED_WriteACmd(0xd9);
	I2C_OLED_WriteACmd((pre_period&0x0f) | ((dis_period&0x0f) << 4));
}

void OLED_SetCommonPadsHardwareConfiguration(bool issequential)
{
	I2C_OLED_WriteACmd(0xda);
	I2C_OLED_WriteACmd(0x02 | ((issequential ? 0 : 1) << 4));
}

void OLED_SetDeselectLevel(uint8_t level)
{
	I2C_OLED_WriteACmd(0xdb);
	I2C_OLED_WriteACmd(level);
}

void OLED_ResetCmd(Screen_InitTypeDef *hScreen, bool reset)
{
	if (reset != FALSE)
	{
		GPIO_WriteHigh(hScreen->GpioRest.GPIOx, hScreen->GpioRest.Pin);
	}
	else
	{
		GPIO_WriteLow(hScreen->GpioRest.GPIOx, hScreen->GpioRest.Pin);
	}
}

void OLED_EnableCmd(Screen_InitTypeDef *hScreen, bool enable)
{
	if (enable != FALSE)
	{
		GPIO_WriteLow(hScreen->GpioEnable.GPIOx, hScreen->GpioEnable.Pin);
	}
	else
	{
		GPIO_WriteHigh(hScreen->GpioEnable.GPIOx, hScreen->GpioEnable.Pin);
	}
}

void Screen_Init(Screen_InitTypeDef *hScreen)
{
	GPIO_Init(hScreen->GpioRest.GPIOx, hScreen->GpioRest.Pin, hScreen->GpioRest.Mode);
	GPIO_Init(hScreen->GpioEnable.GPIOx, hScreen->GpioEnable.Pin, hScreen->GpioEnable.Mode);
	
	OLED_ResetCmd(hScreen, FALSE);
	OLED_EnableCmd(hScreen, FALSE);
	DelayNopMs(100);
	
	/*V DD /V CC off State*/
	OLED_ResetCmd(hScreen, FALSE);
	OLED_EnableCmd(hScreen, TRUE);
	DelayNopMs(1);
	OLED_ResetCmd(hScreen, TRUE);
	DelayNopMs(1);

	OLED_SetDisplayOnOrOff(FALSE);

	/*Initial Settings Configuration*/
	OLED_SetDivideRatioOrOscillatorFrequency(0x00, 0x08);//0x00, 0x0F
	OLED_SetMultiplexRation(0x3F);
	OLED_SetDisplayOffset(0x00);
	OLED_SetDisplayStartLine(0x00);
	OLED_SetDcDcControlOnOrOff(TRUE);
	OLED_SetSegmentRemap(TRUE);
	OLED_SetCommonOutputScanDirection(TRUE);
	OLED_SetCommonPadsHardwareConfiguration(FALSE);
	OLED_SetContrastControl(0xff);						//对比度0x80
	OLED_SetChargeOrPreChargePeriod(0x0f,0x01); //0x02,0x02
	OLED_SetDeselectLevel(0x40);
	OLED_SetPumpVoltageValue(0x03);
	OLED_SetNormalOrReverseDisplayer(TRUE);

	/*Clear Screen*/
	Display_CleanWholeAreaToBlack();
	//Power up VCC & Stabilized (Delay Recommended)  //Power up VCC不确定如何操作
	DelayNopMs(1);

	OLED_SetDisplayOnOrOff(TRUE);

	DelayNopMs(100);
}

void Screen_SetInToSleepMode(void)//_screen_entering_sleep(void)
{
	OLED_SetDisplayOnOrOff(FALSE);
	OLED_SetDcDcControlOnOrOff(FALSE);
	//Power down V BAT
}

void Screen_QuitOutSleepMode(void)
{
	//Power up V BAT
	DelayNopMs(100);
	OLED_SetDcDcControlOnOrOff(TRUE);
	OLED_SetDisplayOnOrOff(TRUE);
	DelayNopMs(100);
}

void Screen_SetCoordinate(uint8_t x_point, uint8_t y_point)
{
  OLED_SetPageAddress(y_point);
  OLED_SetColumnAddress4LowerBits(x_point + 2);//为何需要加2
	OLED_SetColumnAddress4HigherBits(x_point + 2);
}

//鏄剧ず灞忔槸8琛岋紝鑰岃彍鍗曟槸4琛岋紝姣忚鑿滃崟鍗犵敤涓よ鏄剧ず灞? 鎵�浠ヤ紶閫掓椂鍊檡琛屽彿閮借涔樹互2
void Screen_DrawAreaToWhiteOrBlack(uint8_t x_start, uint8_t y_start, uint8_t x_size, uint8_t y_size, bool ToWhite)
{
	uint8_t x, y;
	uint8_t Data = (ToWhite != FALSE)? 0xff : 0x00;
	
	for (y = 0; y < y_size; y++) 
	{
			Screen_SetCoordinate(x_start, y_start + y);
			for (x = 0; x < x_size; x++) 
			{
					I2C_OLED_WriteAData(Data);
			}
	}
}

void Screen_DrawBmpDatas(uint8_t x_start, uint8_t y_start, uint8_t x_size, uint8_t y_size, uint8_t BMP[], uint32_t draw_speed, bool reverse)
{
	uint32_t index = 0;
	uint8_t x, y;

	for (y = 0; y < y_size; y++) 
	{
		Screen_SetCoordinate(x_start, y_start + y);

		if (reverse) 
		{
			for (x = 0; x < x_size; x++) 
			{
				I2C_OLED_WriteAData(~BMP[index]);
				index++;
			}
		} 
		else 
		{
			I2C_OLED_WriteArrayDatas(&BMP[index], x_size);
			index += x_size;
		}
	}
}

/***************************
* 通用内容显示的实际操作方法
*
******************************/
//LOGO
extern const unsigned char TendZoneLogo[][16];

//通道 CH1~CH4 MUTE
extern const unsigned char CH1_32x32[][16];
extern const unsigned char CH2_32x32[][16];
extern const unsigned char CH3_32x32[][16];
extern const unsigned char CH4_32x32[][16];
extern const unsigned char Mute_32x32[][16];

//数字 0~9
extern const unsigned char Num0_12x32[][16];
extern const unsigned char Num1_12x32[][16];
extern const unsigned char Num2_12x32[][16];
extern const unsigned char Num3_12x32[][16];
extern const unsigned char Num4_12x32[][16];
extern const unsigned char Num5_12x32[][16];
extern const unsigned char Num6_12x32[][16];
extern const unsigned char Num7_12x32[][16];
extern const unsigned char Num8_12x32[][16];
extern const unsigned char Num9_12x32[][16];

//大写字母
extern const unsigned char UnitA_12x32[][16];
extern const unsigned char UnitC_12x32[][16];
extern const unsigned char UnitF_12x32[][16];
extern const unsigned char UnitV_12x32[][16];

extern const unsigned char UnitDegCs_12x32[][16];
extern const unsigned char UnitDot_12x32[][16];
extern const unsigned char UnitSpace_12x32[][16]; //空格
extern const unsigned char SelFlag_8x16[][16];//选择标志

extern const unsigned char Minus_8x32[][16];
extern const u8 Atari8x16[][16];

void Display_CleanWholeAreaToBlack(void)
{
	Screen_DrawAreaToWhiteOrBlack(0, 0, 128, 8, FALSE);
}

///OLED--鏍规湰璁惧        Screen--灞炴�?    Display--鏄剧ず鐣岄潰

void Display_ShowTendZoneLogo(void)
{
	Screen_DrawBmpDatas(0, 0, 128, 8, (unsigned char *)TendZoneLogo, 0, FALSE);
}

//void Display_ShowString(uint8_t horizontal_start, uint8_t line, char *str, bool reverse)  //reverse 应该是旗标状态
void Display_ShowString(Display_HandlerTypeDef *hDisplay)

{
  uint8_t strLen = strlen(hDisplay->Str);
  uint8_t xIndex = hDisplay->HorizontalStart * 8;
  uint8_t yIndex = hDisplay->Line* 2;
  uint8_t i = 0;

  if (hDisplay->Line >= MAX_SHOW_LINE)
      return ;
  for (i = 0; i < strLen; i++) 
	{
    if (xIndex > 120)
        return ;
															  //ascii - 32
    Screen_DrawBmpDatas(xIndex, yIndex, 8, 2, (uint8_t *)Atari8x16[hDisplay->Str[i] - ' '], 0, hDisplay->Reverse? TRUE : FALSE);
    xIndex += 8;
  }
}

void Display_ShowPointSymbol(Display_HandlerTypeDef *hDisplay)
{
	//如果是当前行 则显示行首 否则清除
	if (hDisplay->Reverse != FALSE)
	{
		Screen_DrawBmpDatas(0, hDisplay->Line * 2, 8, 2, (unsigned char *)SelFlag_8x16[0], 0, FALSE);
	}
	else
	{
		Screen_DrawAreaToWhiteOrBlack(0, hDisplay->Line * 2, 8, 2, FALSE);
	}
}

void Display_ShowSelectSymbol(Display_HandlerTypeDef *hDisplay)
{
	char *symbol = "<>";

	hDisplay->HorizontalStart = MAX_SHOW_LENGTH - 1;
	hDisplay->Str = symbol;
	Display_ShowString(hDisplay);
}

const uint8_t (*Num_32x32[10])[16] = {
    Num0_12x32, Num1_12x32, Num2_12x32, Num3_12x32,
    Num4_12x32, Num5_12x32, Num6_12x32, Num7_12x32,
    Num8_12x32, Num9_12x32
};

void Display_Show12x32BigString(Display_HandlerTypeDef *hDisplay)
{
    unsigned char strLen = strlen(hDisplay->Str);
    unsigned char xIndex = hDisplay->HorizontalStart + 4; //多出8个分辨率 为了头尾对称 头空四个
    unsigned char yIndex = hDisplay->Line * 2;//动态行号
    unsigned char i = 0;
    unsigned char *atari8x16Char;

	Screen_DrawAreaToWhiteOrBlack(0, yIndex, 4, 4, FALSE);//清理前面四个像素

    for (i = 0; i < strLen; i++) {
        if (hDisplay->Str[i] >= '0' && hDisplay->Str[i] <= '9') {
            atari8x16Char = (uint8_t *)Num_32x32[hDisplay->Str[i] - '0'];
        } else if (hDisplay->Str[i] == 'A') {
            atari8x16Char = (unsigned char *)UnitA_12x32;
        } else if (hDisplay->Str[i] == 'C') {
            atari8x16Char = (unsigned char *)UnitC_12x32;
        } else if (hDisplay->Str[i] == 'F') {
            atari8x16Char = (unsigned char *)UnitF_12x32;
        } else if (hDisplay->Str[i] == 'V') {
            atari8x16Char = (unsigned char *)UnitV_12x32;
        } else if (hDisplay->Str[i] == 'o') {
            atari8x16Char = (unsigned char *)UnitDegCs_12x32;
        } else if (hDisplay->Str[i] == '.') {
            atari8x16Char = (unsigned char *)UnitDot_12x32;
        } else if (hDisplay->Str[i] == ' ') {
            atari8x16Char = (unsigned char *)UnitSpace_12x32;
        }
        Screen_DrawBmpDatas(xIndex, yIndex, 12, 4, atari8x16Char, 0, hDisplay->Reverse);
        xIndex += 12;
    }
	
    Screen_DrawAreaToWhiteOrBlack(xIndex, yIndex, 128 - xIndex, 4, FALSE);
}

void Display_ShowHomepageGainValue(char channel, signed char gainValue)
{
    uint8_t offset = channel * 32;
    uint8_t symbolxOffset = 8;
    uint8_t NumCharOffset = symbolxOffset + offset;
    uint8_t oneNumCharWidth = 12;
    if (gainValue < 0) {
        Screen_DrawBmpDatas(offset, 0, symbolxOffset, 4, (uint8_t *)Minus_8x32, 0, FALSE);//负号
    } else {
        Screen_DrawAreaToWhiteOrBlack(offset, 0, symbolxOffset, 4, FALSE);
    }

    Screen_DrawBmpDatas(NumCharOffset, 0, oneNumCharWidth, 4, (uint8_t *)Num_32x32[(-gainValue) / 10], 0, FALSE);
    Screen_DrawBmpDatas(NumCharOffset + oneNumCharWidth, 0, 12, 4, (uint8_t *)Num_32x32[(-gainValue) % 10], 0, FALSE);
}

const uint8_t (*Ch_32x32[4])[16] = { CH1_32x32, CH2_32x32, CH3_32x32, CH4_32x32 };
void Display_ShowHomepageChannel(char channel, bool isSelect)
{
    Screen_DrawBmpDatas(32 * channel, 4, 32, 4, (uint8_t *)Ch_32x32[channel], 0, isSelect);
}

void Display_ShowHomepageMute(char channel)
{
    Screen_DrawBmpDatas(32 * channel, 4, 32, 4, (uint8_t *)Mute_32x32, 0, TRUE);
}

/*****************************************
-	EEPROM
-
*****************************************/
#define AT24C02_WRITE_PAGE_SIZE 8	//叶宽
#define EEPROM_AT24C02_READ_RANG_SIZE 256	//2kBIT

uint8_t read_eeprom(uint16_t start_addr, uint8_t *buf, uint16_t length)
{
	if(length == 0)
		return 1;

	uint8_t ret = 0;

	ret = I2C_BSP_Read(AT24C02_ADDR, start_addr, buf, length);

	if (ret != 0) {
		for(int i = 0; i < length; i++)
			buf[i] = 0x00;
	 }

	return ret;
}

uint8_t write_eeprom(uint16_t start_addr, uint8_t *buf, uint16_t length)
{
	uint16_t first_page_size, last_page_size, next_page_base_addr, last_page_base_addr;
	uint16_t full_page_cnt;
	uint8_t *pdata = buf;

	first_page_size = AT24C02_WRITE_PAGE_SIZE - start_addr % AT24C02_WRITE_PAGE_SIZE;
	full_page_cnt = (length - first_page_size)/AT24C02_WRITE_PAGE_SIZE;			//满页数量
	last_page_size  = (length - first_page_size) % AT24C02_WRITE_PAGE_SIZE;
 	next_page_base_addr = start_addr + first_page_size;
 	last_page_base_addr = full_page_cnt * AT24C02_WRITE_PAGE_SIZE + next_page_base_addr;

	if (first_page_size > 0) {
		I2C_BSP_Write(AT24C02_ADDR, start_addr, pdata, first_page_size);
		DelayNopMs(5);
	}

	pdata += first_page_size;

	if (full_page_cnt > 0) {
		for(int i = 0; i < full_page_cnt; i++) {
			I2C_BSP_Write(AT24C02_ADDR, next_page_base_addr, pdata, AT24C02_WRITE_PAGE_SIZE);
			next_page_base_addr += AT24C02_WRITE_PAGE_SIZE;
			pdata += AT24C02_WRITE_PAGE_SIZE;
			DelayNopMs(5);
		}
	}

	if (last_page_size > 0) {
		I2C_BSP_Write(AT24C02_ADDR, last_page_base_addr, pdata, last_page_size);
		DelayNopMs(5);
	}

	return 0;
}

void save_dev_parameter_table(uint8_t *prmt, uint16_t length)
{
	write_eeprom(0x00,prmt,length);
}

void get_dev_parameter_table(uint8_t *prmt, uint16_t length)
{
	read_eeprom(0x00,prmt,length);
	DelayNopMs(0xff);
}


/****************************************
*	i2c控制器基本操作接口
*
*****************************************/
#define I2C_SPEED             	 100000		//400000速率下，读取EEPROM会有数据错误
#define I2C_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define I2C_LONG_TIMEOUT         ((uint32_t)(10 * I2C_FLAG_TIMEOUT))
#define I2C_TIMEOUT         	 ((unsigned long)0x5)

uint32_t  I2CTimeout = I2C_LONG_TIMEOUT;

uint8_t i2c_error_callback(void)
{
    I2C_GenerateSTOP(ENABLE);
	return 1;
}

void I2C_BSP_Init(I2C_InitTypeDef *hI2C)
{
	CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, ENABLE);
	I2C_Cmd(hI2C->Cmd);
	I2C_Init(hI2C->OutputClockFrequencyHz, hI2C->OwnAddress, hI2C->DutyCycle, hI2C->Ack, hI2C->AddMode, hI2C->InputClockFrequencyMHz);
	I2C_ITConfig(hI2C->IT, hI2C->ITState);
}


uint8_t I2C_BSP_Read(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data, uint16_t length)
{
	I2CTimeout = I2C_LONG_TIMEOUT;
	while(I2C_GetFlagStatus(I2C_FLAG_BUSBUSY))
	{
		if((I2CTimeout--) == 0) return i2c_error_callback();
	}

	I2C_AcknowledgeConfig(I2C_ACK_CURR);			//必须先使能一次ACK

	I2C_GenerateSTART( ENABLE);
	I2CTimeout = I2C_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((I2CTimeout--) == 0) return i2c_error_callback();
	}

	I2C_Send7bitAddress((uint8_t)slave_addr, I2C_DIRECTION_TX);
	I2CTimeout = I2C_FLAG_TIMEOUT;
	while(!I2C_CheckEvent( I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if((I2CTimeout--) == 0) return i2c_error_callback();
	}

	I2C_SendData(reg_addr);
	I2CTimeout = I2C_FLAG_TIMEOUT;
	while(!I2C_CheckEvent( I2C_EVENT_MASTER_BYTE_TRANSMITTED))
	{
		if((I2CTimeout--) == 0) return i2c_error_callback();
	}

	I2C_GenerateSTART( ENABLE);
	I2CTimeout = I2C_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((I2CTimeout--) == 0) return i2c_error_callback();
	}

	I2C_Send7bitAddress((uint8_t)slave_addr, I2C_DIRECTION_RX);
	I2CTimeout = I2C_FLAG_TIMEOUT;
	while(!I2C_CheckEvent( I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
	{
		if((I2CTimeout--) == 0) return i2c_error_callback();
	}

	while(length > 1) {
		I2CTimeout = I2C_FLAG_TIMEOUT;
		while(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED))
		{
			if((I2CTimeout--) == 0) return i2c_error_callback();
		}
		*data = I2C_ReceiveData();

		data++;
		length--;
	}

	I2C_AcknowledgeConfig(I2C_ACK_NONE);
	I2CTimeout = I2C_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED))
	{
		if((I2CTimeout--) == 0) return i2c_error_callback();
	}

	*data = I2C_ReceiveData();
	I2C_GenerateSTOP(ENABLE);

    return 0;
}

uint8_t I2C_BSP_Write(uint8_t slave_addr, uint8_t reg_addr, uint8_t *data, uint16_t length)
{
	//I2C_AcknowledgeConfig(I2C_ACK_CURR);//是否需要

		I2CTimeout = I2C_LONG_TIMEOUT;
		while(I2C_GetFlagStatus(I2C_FLAG_BUSBUSY))
		{
			if((I2CTimeout--) == 0)
				return i2c_error_callback();
		}

		I2C_GenerateSTART( ENABLE);
		I2CTimeout = I2C_FLAG_TIMEOUT;
		while(!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT))
		{
			if((I2CTimeout--) == 0)
				return i2c_error_callback();
		}

		I2C_Send7bitAddress((uint8_t)slave_addr, I2C_DIRECTION_TX);
		I2CTimeout = I2C_FLAG_TIMEOUT;
		while(!I2C_CheckEvent( I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		{
			if((I2CTimeout--) == 0)
				return i2c_error_callback();
		}

		I2C_SendData( reg_addr);
		I2CTimeout = I2C_FLAG_TIMEOUT;
		while(!I2C_CheckEvent( I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
			if((I2CTimeout--) == 0)
				return i2c_error_callback();
		}

		while(length > 0)
		{
			I2C_SendData(*data);
			I2CTimeout = I2C_LONG_TIMEOUT;

			while(!I2C_CheckEvent( I2C_EVENT_MASTER_BYTE_TRANSMITTED))	 //可靠,速度变慢EV8_2
			{
				if((I2CTimeout--) == 0)
					return i2c_error_callback();
			}
			length--;
			data++;
		}

		I2C_GenerateSTOP(ENABLE);

		//(void)I2C->SR1;
		//(void)I2C->SR3;

	  return 0;
}

