#ifndef __LCD_H__
#define __LCD_H__
#include "stm8s.h"
#include "Menu.h"

#define LcdMaxLine 4

#define LCD_DisplayOn()     WriteLcdOneByte(0xaf, LcdOp_Cmd)
#define LCD_DisplayOff()    WriteLcdOneByte(0xae, LcdOp_Cmd)

//#define LCD_EnterSetDcDcMode()			    WriteLcdOneByte(0xAD, LcdOp_Cmd)//sh1106
#define LCD_EnterSetDcDcMode()			    WriteLcdOneByte(0x8D, LcdOp_Cmd) //ssd1306
#define LCD_EnterSetContrastMode()			WriteLcdOneByte(0x81, LcdOp_Cmd)
#define LCD_EnterSetMultiplexRatioMode()    WriteLcdOneByte(0xa8, LcdOp_Cmd)
#define LCD_EnterSetDisplayMode()           WriteLcdOneByte(0xd3, LcdOp_Cmd)
#define LCD_EnterSetClockMode()             WriteLcdOneByte(0xd5, LcdOp_Cmd)
#define LCD_EnterSetChargeMode()            WriteLcdOneByte(0xd9, LcdOp_Cmd)
#define LCD_EnterSetCPHCMode()				WriteLcdOneByte(0xda, LcdOp_Cmd)
#define LCD_EnterSetVcomDesMode()			WriteLcdOneByte(0xdb, LcdOp_Cmd)


#define LCD_DisplayStartLine(x)				WriteLcdOneByte(x|0x40, LcdOp_Cmd)
#define LCD_EnterSetAddressingMode(x)		WriteLcdOneByte(0x20, LcdOp_Cmd)
//#define LCD_NormalDirection()				WriteLcdOneByte(0xa1, LcdOp_Cmd)
#define LCD_SegmentRemap()					WriteLcdOneByte(0xa0, LcdOp_Cmd)
#define LCD_NormalScanOrder()               WriteLcdOneByte(0xc8, LcdOp_Cmd)
#define LCD_ReverseScanOrder()              WriteLcdOneByte(0xc0, LcdOp_Cmd)
#define LCD_EntireDisplayOn()               WriteLcdOneByte(0xa5, LcdOp_Cmd)
#define LCD_EntireDisplayOff()              WriteLcdOneByte(0xa4, LcdOp_Cmd)
#define LCD_NormalDisplay()                 WriteLcdOneByte(0xa6, LcdOp_Cmd)
#define LCD_ReverseDisplay()                WriteLcdOneByte(0xa7, LcdOp_Cmd)
#define LCD_SetPageAddr(x)                  WriteLcdOneByte(0xb0 + x, LcdOp_Cmd);
#define LCD_SetColumnAddr(x)                WriteLcdOneByte(x&0x0f, LcdOp_Cmd);WriteLcdOneByte((x>>4)|0x10,LcdOp_Cmd)
//#define LCD_SetPumpVolValue()			    WriteLcdOneByte(0x32, LcdOp_Cmd)
//#define LCD_SetEntireDisplayOn()
#define LcdOn()             GPIO_WriteLow(GPIOC, GPIO_PIN_2)
#define LcdOff()            GPIO_WriteHigh(GPIOC, GPIO_PIN_2)

#define LcdSetRst()         GPIO_WriteHigh(GPIOC, GPIO_PIN_1)
#define LcdClrRst()         GPIO_WriteLow(GPIOC, GPIO_PIN_1)

#define WriteDataMode()     GPIO_WriteHigh(GPIOC, GPIO_PIN_3)
#define WriteCmdMode()      GPIO_WriteLow(GPIOC, GPIO_PIN_3)

typedef enum tagLcdOp_e
{
    LcdOp_Cmd,
    LcdOp_Data,
    LcdOp_Butt
} LcdOp_e;

//void WriteLcd(unsigned char *data, unsigned char size, LcdOp_e type);
void LCD_Init();
void LCD_ClearDisplay();
void LCD_SetPos(unsigned char x, unsigned char y);
void LCD_SetContrast(unsigned char level);
void LCD_SetMultiplexRatio(unsigned char level);
void LCD_SetDisplayOffset(unsigned char level);
void LCD_SetDisplayClock(unsigned char data);
void LCD_SetCharge(unsigned char data);
void LCD_SetCPHC(unsigned char data);
void LCD_SetVcomDesLevel(unsigned char level);
void LCD_Setup();
void LCD_DrawBmp(unsigned char xStart, unsigned char yStart,
                        unsigned char xSize, unsigned char ySize,
                        unsigned char BMP[], unsigned int drawSpeed, bool reverse);
void LcdShowStringAllLine(uint8_t x, uint8_t line, char *str, bool reverse);
uint8_t LcdShowString(uint8_t x, uint8_t line, char *str, bool reverse);

void LcdSelfTest();
void LcdGainValDisplay(char channel, signed char gainValue);
void LcdFillArea(uint8_t xStart, uint8_t yStart, uint8_t xSize, uint8_t ySize, bool isLight);
void LcdHomePageDisplay();
void LcdSelectChannel(char channel, bool isSelect);
void LcdMuteDisplay(char channel);
void LcdMenuDisplay(Menu *menu);
void LcdBacKDisplay(uint8_t line, bool isHighlight);
void LcdLineDisplay(Item *item, uint8_t line, bool isHighlight, bool isSymHighlight);
void LcdSymbolDisplay(uint8_t itemDataLen, uint8_t line, char *symbol, bool isLineHighlight);
void LcdMenuRefresh(ListHead *curList);
void Lcd12x32StrDisplay(char *str);
#endif

