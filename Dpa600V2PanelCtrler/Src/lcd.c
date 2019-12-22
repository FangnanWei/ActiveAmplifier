#include "lcd.h"
#include "SpiLcd.h"
#include "timer.h"
#include "stdio.h"
#include <string.h>
#include "stm8s.h"
#include "GlobalVal.h"
#include "LedLightBar.h"
#include "Menu.h"
#include "Delay.h"
#include "Key.h"
#include "AlgCfg.h"

extern const unsigned char TendZoneLogo[][16];
extern const unsigned char Mute_32x32[][16];
extern const unsigned char CH1_32x32[][16];
extern const unsigned char CH2_32x32[][16];
extern const unsigned char CH3_32x32[][16];
extern const unsigned char CH4_32x32[][16];
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

extern const unsigned char UnitA_12x32[][16];
extern const unsigned char UnitC_12x32[][16];
extern const unsigned char UnitF_12x32[][16];
extern const unsigned char UnitV_12x32[][16];
extern const unsigned char UnitDegCs_12x32[][16];
extern const unsigned char UnitDot_12x32[][16];
extern const unsigned char UnitSpace_12x32[][16];
extern const unsigned char SelFlag_8x16[][16];

extern const unsigned char Minus_8x32[][16];
extern const u8 Atari8x16[][16];
#if 0
extern const u8 Atari8x16LowerCase[][16];
extern const u8 Atari8x16UpperCase[][16];
extern const u8 Atari8x16Num[][16];
extern const u8 Atari8x16Brackets[][16];
extern const u8 Atari8x16Space[16];
extern const u8 Atari8x16HashKey[16];
extern const u8 Atari8x16PlusSign[16];
extern const u8 Atari8x16Minus[16];
extern const u8 Atari8x16Dot[16];
extern const u8 Atari8x16Slash[16];
extern const u8 Atari8x16Colon[16];
extern const u8 Atari8x16LessThan[16];
extern const u8 Atari8x16EqualSign[16];
extern const u8 Atari8x16GreaterThan[16];
#endif
static void lcdCtrlGpio_Init()
{
	GPIO_Init(GPIOC, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_MODE_OUT_PP_HIGH_FAST);
	//GPIO_Init(GPIOC, GPIO_PIN_2, GPIO_MODE_OUT_PP_HIGH_FAST);
	//GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_OUT_PP_HIGH_FAST);
}

void WriteLcdOneByte(unsigned char data, LcdOp_e type)
{
    if (LcdOp_Cmd == type)
    {
        WriteCmdMode();
    }
    else
    {
        WriteDataMode();
    }

    SPI_CS_LOW();
    SpiLcdWrite(data);
    SPI_CS_HIGH();
}

#if 0
void WriteLcd(unsigned char *data, unsigned char size, LcdOp_e type)
{
    int i = 0;
    if (LcdOp_Cmd == type)
    {
        WriteCmdMode();
    }
    else
    {
        WriteDataMode();
    }

    SPI_CS_LOW();
    for (i = 0; i < size; i++)
    {
        SpiLcdWrite(data[i]);
    }
    SPI_CS_HIGH();
}
#endif

void LCD_ClearDisplay()
{
	#if 0
    unsigned char i, n;

    for (i = 0; i < 8; i++)
    {
        LCD_SetPos(0, i);
        for(n = 0; n <128; n++)
        {
            WriteLcdOneByte(0, LcdOp_Data);
        }
    }
	#endif
	LcdFillArea(0, 0, 128, 8, FALSE);
}

/* x : 0 ~ 127       y : 0 ~ 7 */
void LCD_SetPos(unsigned char x, unsigned char y)
{
    LCD_SetPageAddr(y);
    LCD_SetColumnAddr(x + 0);
}

/* level : 0~255 */
void LCD_SetContrast(unsigned char level)
{
    LCD_EnterSetContrastMode();       /*enter set contrat mode*/
    WriteLcdOneByte(level, LcdOp_Cmd);
}

/* level : 0~0x3f */
/* 图像的上下位置,上部分没有 */
void LCD_SetMultiplexRatio(unsigned char level)
{
    LCD_EnterSetMultiplexRatioMode();
    WriteLcdOneByte(level, LcdOp_Cmd);
}

/* level : 0~3f */
/* 图像上下位置，上下程补齐全图样式 */
void LCD_SetDisplayOffset(unsigned char level)
{
    LCD_EnterSetDisplayMode();
    WriteLcdOneByte(level, LcdOp_Cmd);
}

void LCD_SetAddrMode(unsigned char mode)
{
    LCD_EnterSetAddressingMode();
    WriteLcdOneByte(mode, LcdOp_Cmd);
}

/* data : see data sheet */
void LCD_SetDisplayClock(unsigned char data)
{
    LCD_EnterSetClockMode();
    WriteLcdOneByte(data, LcdOp_Cmd);
}

#if 1
void LCD_SetDcDcMode(unsigned char data)
{
    LCD_EnterSetDcDcMode();
    WriteLcdOneByte(data, LcdOp_Cmd);
}
#endif

/* data : see datasheet */
void LCD_SetCharge(unsigned char data)
{
    LCD_EnterSetChargeMode();
    WriteLcdOneByte(data, LcdOp_Cmd);
}

/* Common Pads Hardware Configuration
     data : 02h 0r 12h                          */
void LCD_SetCPHC(unsigned char data)
{
    LCD_EnterSetCPHCMode();
    WriteLcdOneByte(data, LcdOp_Cmd);
}

void LCD_SetVcomDesLevel(unsigned char level)
{
    LCD_EnterSetVcomDesMode();
    WriteLcdOneByte(level, LcdOp_Cmd);
}

void LCD_Setup()
{
#if 1
    LCD_DisplayOff();
#if 0
	WriteLcdOneByte(0x2C, LcdOp_Cmd);
	Delay(5);
	WriteLcdOneByte(0x2E, LcdOp_Cmd);
	Delay(5);
	WriteLcdOneByte(0x2F, LcdOp_Cmd);
	Delay(5);
#endif
    //LCD_SetDisplayClock(0xF1);  /* div = 1 freq = +15% */
    LCD_SetDisplayClock(0xF0);  /* div = 1 freq = +50% */
    LCD_SetMultiplexRatio(0x3f);
    LCD_SetDisplayOffset(0);
    LCD_DisplayStartLine(0);
	LCD_SetDcDcMode(0x14);
	//LCD_SetDcDcMode(0x8a);
	LCD_SetAddrMode(0x02);
	LCD_SegmentRemap();
	//LCD_NormalScanOrder();
    LCD_ReverseScanOrder();
    LCD_SetCPHC(0x12);
    LCD_SetContrast(0x80);
    LCD_SetCharge(0x22);        /* pre charge = 1, dis charge = 15 */
    LCD_SetVcomDesLevel(0x40);
    LCD_EntireDisplayOff();
    LCD_NormalDisplay();
    LCD_ClearDisplay();

    LCD_DisplayOn();
#endif
#if 0
	Set_Display_On_Off(0xAE);		// Display Off (0xAE/0xAF)
	Set_Display_Clock(0x80);		// Set Clock as 100 Frames/Sec
	Set_Multiplex_Ratio(0x3F);		// 1/64 Duty (0x0F~0x3F)
	Set_Display_Offset(0x00);		// Shift Mapping RAM Counter (0x00~0x3F)
	Set_Start_Line(0x00);			// Set Mapping RAM Display Start Line (0x00~0x3F)
	Set_Charge_Pump(0x14);			// Disable Embedded DC/DC Converter (0x10/0x14)
	Set_Addressing_Mode(0x02);		// Set Page Addressing Mode (0x00/0x01/0x02)
	Set_Segment_Remap(0xA1);		// Set SEG/Column Mapping (0xA0/0xA1)
	Set_Common_Remap(0xC8);			// Set COM/Row Scan Direction (0xC0/0xC8)
	Set_Common_Config(0x12);		// Set Sequential Configuration (0x02/0x12)
	Set_Contrast_Control(Brightness);	// Set SEG Output Current
	Set_Precharge_Period(0x22);		// Set Pre-Charge as 2 Clocks & Discharge as 2 Clocks
	Set_VCOMH(0x40);			// Set VCOM Deselect Level
	Set_Entire_Display(0xA4);		// Disable Entire Display On (0xA4/0xA5)
	Set_Inverse_Display(0xA6);		// Disable Inverse Display On (0xA6/0xA7)

	Fill_RAM(0x00);				// Clear Screen

	Set_Display_On_Off(0xAF);		// Display On (0xAE/0xAF)
#endif
}

/* xStart : x轴起点
     yStart : y轴起点
     xSize  : 图像x轴大小
     ySize*8  : 图像y轴大小
     drawSpeed : 越小越快*/
void LCD_DrawBmp(unsigned char xStart, unsigned char yStart,
                        unsigned char xSize, unsigned char ySize,
                        unsigned char BMP[], unsigned int drawSpeed, bool reverse)
{
    unsigned int index = 0;
    unsigned char xPos = xStart;
    unsigned char yPos = yStart;
    unsigned char x,y;
    for (y = 0; y < ySize; y++)
    {
        LCD_SetPos(xPos, yPos + y);
        for (x = 0; x < xSize; x++)
        {
			WriteLcdOneByte(reverse ? ~BMP[index] : BMP[index], LcdOp_Data);
			index++;
            //if (index >= xSize*ySize)
           // {
           //     return ;
           // }
        }
        if (drawSpeed)
        {
            Delay(drawSpeed);
        }
    }
}

void LcdFillArea(uint8_t xStart, uint8_t yStart, uint8_t xSize, uint8_t ySize, bool isHighLight) {
    unsigned char x,y;
	char content = isHighLight ? 0xff : 0;
	//if (xStart > 120)
	//	return;
    for (y = 0; y < ySize; y++)
    {
        LCD_SetPos(xStart, yStart + y);
        for (x = 0; x < xSize; x++)
        {
	        WriteLcdOneByte(content, LcdOp_Data);
        }
    }
}

/* x
: x轴起点0 ~ 127
     y : y轴起点0 ~ 7
     str : 字符串
     字符: 8*16 */
void LcdShowStringAllLine(uint8_t x, uint8_t line, char *str, bool reverse)
{
    unsigned char xIndex = x;
	xIndex = LcdShowString(xIndex, line, str, reverse);
	//if (i < MAX_SHOW_LENGTH) {
		LcdFillArea(xIndex, line * 2, 128 - xIndex, 2, FALSE);
	//}
}

uint8_t LcdShowString(uint8_t x, uint8_t line, char *str, bool reverse)
{
    unsigned char strLen = strlen(str);
    unsigned char xIndex = x;
    unsigned char yIndex = line * 2;
    unsigned char i = 0;
	//unsigned char *atari8x16Char;
	//if (xIndex == 0)
		xIndex += 8;
	if (x == 0) {
		if (reverse && !IsLeafSel)
			LCD_DrawBmp(0, line * 2, 8, 2, (unsigned char*)SelFlag_8x16[0], 0, FALSE);
		else
			LcdFillArea(0, line * 2, 8, 2, FALSE);
	}
   if (line >= LcdMaxLine)
        /* y到达边界不足以显示字符 */
        return 0;
    for (i = 0; i < strLen; i++)
    {
        //if (xIndex > 120)
        //{
            /* x到达边界不足显示一个字符 */
            //xIndex = 0;
            //yIndex += 1;
        //    return 0;
        //}

        LCD_DrawBmp(xIndex, yIndex, 8, 2, (unsigned char*)Atari8x16[str[i] - ' '], 0, IsLeafSel ? reverse : FALSE);
        xIndex += 8;
    }
	return xIndex;
}

void LCD_Init()
{
	SpiLcdInit();
    lcdCtrlGpio_Init();
    LcdClrRst();
    LcdOn();
	Delay(1);
    LcdSetRst();
	Delay(1);
    LCD_Setup();
#if 1
    LCD_DrawBmp(0, 0, 128, 8, (unsigned char*)TendZoneLogo, 1000, FALSE);
    Delay(1000);
#endif
    LCD_ClearDisplay();
//    LcdShowStringAllLine(0, 0, "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ", FALSE);
}

void LcdSelfTest() {
#if 0
	int i;
	LCD_ReverseDisplay();
	for (i = 1; i <= 0x7f; i+=2) {
		LCD_SetContrast(i);
		Delay(24);
	}
	for (i = 0x81; i <= 0xff; i+=2) {
		LCD_SetContrast(i);
		Delay(24);
	}
	for (i = 0xff; i >= 0x81; i-=2) {
		LCD_SetContrast(i);
		Delay(16);
	}
	for (i = 0x7f; i >= 1; i-=2) {
		LCD_SetContrast(i);
		Delay(16);
	}
	LCD_NormalDisplay();
	LCD_SetContrast(0x80);
#endif
#if 0
	LCD_SetContrast(0);
	LCD_ReverseDisplay();
	LCD_SetContrast(0x7e);
	LCD_SetContrast(0);
	LCD_SetContrast(0x7f);
	LCD_SetContrast(0);
	LCD_SetContrast(2);
	LCD_SetContrast(0xfe);
	LCD_SetContrast(0x1);
#endif
}

const uint8_t (*Num_32x32[10])[16] = {
	Num0_12x32, Num1_12x32, Num2_12x32, Num3_12x32,
	Num4_12x32, Num5_12x32, Num6_12x32, Num7_12x32,
	Num8_12x32, Num9_12x32
};

void Lcd12x32StrDisplay(char *str) {
    unsigned char strLen = strlen(str);
    unsigned char xIndex = 0;
    unsigned char yIndex = 2;
    unsigned char i = 0;
	unsigned char *atari8x16Char;
    for (i = 0; i < strLen; i++)
    {
		if (str[i] >= '0' && str[i] <= '9')
        {
            atari8x16Char = (uint8_t *)Num_32x32[str[i] - '0'];
        }
        else if (str[i] == 'A')
        {
            atari8x16Char = (unsigned char*)UnitA_12x32;
        }
        else if (str[i] == 'C')
        {
            atari8x16Char = (unsigned char*)UnitC_12x32;
        }
        else if (str[i] == 'F')
        {
            atari8x16Char = (unsigned char*)UnitF_12x32;
        }
        else if (str[i] == 'V')
        {
            atari8x16Char = (unsigned char*)UnitV_12x32;
        }
        else if (str[i] == 'o')
        {
            atari8x16Char = (unsigned char*)UnitDegCs_12x32;
        }
        else if (str[i] == '.')
        {
            atari8x16Char = (unsigned char*)UnitDot_12x32;
        }
        else if (str[i] == ' ')
        {
            atari8x16Char = (unsigned char*)UnitSpace_12x32;
        }
        LCD_DrawBmp(xIndex, yIndex, 12, 4, atari8x16Char, 0, FALSE);
        xIndex += 12;
    }
	LcdFillArea(xIndex, yIndex, 128 - xIndex, 4, FALSE);
}

void LcdGainValDisplay(char channel, signed char gainValue) {
	uint8_t offset = channel *32;
	uint8_t symbolxOffset = 8;
	uint8_t NumCharOffset = symbolxOffset + offset;
	uint8_t oneNumCharWidth = 12;
	if (gainValue < 0) {
	    LCD_DrawBmp(offset, 0, symbolxOffset, 4, (uint8_t *)Minus_8x32, 0, FALSE);
	}
	else {
		LcdFillArea(offset, 0, symbolxOffset, 4, FALSE);
	}

	if (gainValue < -99)
		gainValue = -99;
    LCD_DrawBmp(NumCharOffset, 0, oneNumCharWidth, 4, (uint8_t *)Num_32x32[(-gainValue) / 10], 0, FALSE);
	LCD_DrawBmp(NumCharOffset + oneNumCharWidth, 0, 12, 4, (uint8_t *)Num_32x32[(-gainValue) % 10], 0, FALSE);
}

const uint8_t (*Ch_32x32[4])[16] = { CH1_32x32, CH2_32x32, CH3_32x32, CH4_32x32 };
void LcdSelectChannel(char channel, bool isSelect) {
    LCD_DrawBmp(32 * channel, 4, 32, 4, (uint8_t *)Ch_32x32[channel], 0, isSelect);
}

void LcdHomePageDisplay() {
	char channel;
	for (channel = 0; channel < 4; channel++) {
		//LedLevelDisplay(channel, OutGainVals[channel]);
		LcdGainValDisplay(channel, OutGainVals[channel]);
		//LcdSelectChannel(channel, IsChSelect[channel]);
		if (IsChMute[channel])
			LcdMuteDisplay(channel);
		else
			LcdSelectChannel(channel, IsChSelect[channel]);
	}
}

void LcdMenuRefresh(ListHead *curList) {
	uint8_t startItemId;
	Item *item;
	char i = 0;

	startItemId = curList->startItemId;
	for (; i + startItemId < curList->itemNum && i < LcdMaxLine; i++) {
		item = &curList->items[i + startItemId];
		if (curList->itemNum == 1 && !BigCharDisplay) {
			i++;
		}
 		if (curList->curLine != i || !IsLeafSel) {
            if (item->encoderSpin)
                item->encoderSpin(item);
			//LcdShowString(0, i, item->data8, IsLeafSel ? FALSE : i == curList->curLine);
			LcdLineDisplay(item, i, IsLeafSel ? FALSE : i == curList->curLine, IsLeafSel ? i == curList->curLine : FALSE);
		}
	}
	if (BigCharDisplay)
		Lcd12x32StrDisplay(CapPrmtStr);
	//if (IsCharSelected)
		//LcdShowString(SelectCharPos * 8, curList->curLine, NumChar, TRUE);
}

void LcdMenuDisplay(Menu *menu) {
	char i = 0;
	ListHead *curList;
	uint8_t startItemId;
	Item *item;
	curList = menu->curList;
	if (curList) {
		//itemNum = curList->itemNum;
		startItemId = curList->startItemId;
		//curLine = curList->curLine;
		//LCD_ClearDisplay();
		for (; i + startItemId < curList->itemNum && i < LcdMaxLine; i++) {
			item = &curList->items[i + startItemId];
			if (item->encoderSpin)
				item->encoderSpin(item);
			if (!BigCharDisplay) {
				if (curList->itemNum == 1) {
					LcdFillArea(0, 0 * 2, 128, 2, FALSE);
					i++;
					//curList->curLine++;
				}
			}
			if (PageNoAdjust)
				curList->curLine = 3;
			LcdLineDisplay(item, i, i == curList->curLine, i == curList->curLine);
		}
		if (BigCharDisplay) {
			Lcd12x32StrDisplay(CapPrmtStr);
		}
		else if (curList->itemNum < 3) {
			LcdFillArea(0, 2 * 2, 128, 2, FALSE);
		}
#if 0
		if (curList->curLine + startItemId < curList->itemNum) {
			//item = &curList->items[itemOffset];
			//LcdLineDisplay(item, curLine, TRUE, FALSE);
			//LcdLineDisplay(item, curLine, TRUE);
			if (i < LcdMaxLine) {
				LcdBacKDisplay(i, FALSE);
				i++;
				LcdFillArea(0, i * 2, 128, (LcdMaxLine - i) * 2, FALSE);
			}
		}
		else {
			LcdBacKDisplay(i, TRUE);
		}
#endif
        //i--;
		if ((i + startItemId >= curList->itemNum && i < LcdMaxLine) || curList->curLine + startItemId >= curList->itemNum)
			LcdBacKDisplay(3, curList->curLine == 3);
	}
}

void LcdBacKDisplay(uint8_t line, bool isHighlight) {
	LcdShowStringAllLine(0, line, "BACK", isHighlight);
}

void LcdSymbolDisplay(uint8_t itemDataLen, uint8_t line, char *symbol, bool isLineHighlight) {
	uint8_t xOffset = (itemDataLen) * 8;
	//char len = strlen(symbol);
	if (itemDataLen < MAX_SHOW_LENGTH - 1) {
		LcdShowString(xOffset, line, symbol, isLineHighlight);
		//LcdFillArea(xOffset + len, line * 2, 128 - (xOffset + len), 2, FALSE);
	}
	else if (itemDataLen == MAX_SHOW_LENGTH - 1) {
		LcdShowString(xOffset, line, symbol + 1, isLineHighlight);
	}
}

void LcdLineDisplay(Item *item, uint8_t line, bool isLineHighlight, bool isSymHighlight) {
	uint8_t displayStrLen;
	uint8_t xOffset;
	char *symbol;
    displayStrLen = strlen(item->data8);
	//LcdShowStringAllLine(0, line, item->data8, isLineHighlight);
	LcdShowString(0, line, item->data8, isLineHighlight);

    xOffset = (displayStrLen) * 8;
	if (!PageNoAdjust) {
		if (ItemPrmtDpa600V2[item->nodeId].subListItemNum == 0 && displayStrLen < MAX_SHOW_LENGTH) {
			if (displayStrLen < MAX_SHOW_LENGTH - 2) {
				LcdShowString(xOffset, line, " ", FALSE);
				displayStrLen++;
			}
			symbol = "<>";
		}
		else {
			symbol = " >";
		}
        LcdSymbolDisplay(displayStrLen, line, symbol, isSymHighlight);
	}
	else {
		symbol = "\0";
	}
	//if (IsLeafSel)
	if (displayStrLen < MAX_SHOW_LENGTH - 1) {
    	xOffset = (displayStrLen + strlen(symbol) + 1) * 8;
//		LcdFillArea(xOffset, line * 2, 128 - (xOffset), 2, IsLeafSel ? FALSE : isLineHighlight);
		LcdFillArea(xOffset, line * 2, 128 - (xOffset), 2, FALSE);
	}

}

void LcdMuteDisplay(char channel) {
	LCD_DrawBmp(32 * channel, 4, 32, 4, (unsigned char*)Mute_32x32, 0, TRUE);
}


