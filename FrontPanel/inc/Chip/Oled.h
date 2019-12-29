#ifndef Oled_H__
#define Oled_H__

extern "C" {
#include "stm8s.h"
}
#include "I2c.h"
#include "Gpio.h"

struct OledParam {
    GpioParam resetIoParam_;
    GpioParam enableIoParem_;
    uint8_t i2cAddress_;
    OledParam(GPIO_TypeDef* resetBank, GPIO_Pin_TypeDef resetPin, GPIO_Mode_TypeDef resetMode, bool resetHighEnable, GPIO_TypeDef* enableBank, GPIO_Pin_TypeDef enablePin, GPIO_Mode_TypeDef enableMode, bool enableHighEnable, uint8_t i2cAddress) : resetIoParam_(resetBank, resetPin, resetMode, resetHighEnable) , enableIoParem_(enableBank, enablePin, enableMode, enableHighEnable)
    {
        this->i2cAddress_ = i2cAddress;
    }

};



class Oled{
public:
    Oled(OledParam &param);

    void WriteOneByte(uint8_t op, uint8_t data);
    void DisplayOn();
    void DisplayOff();
    void Init();
    void Clear();
    //void DrawPoint(uint8_t x,uint8_t y,uint8_t t);
    //void Fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t dot);
    void ShowChar(uint8_t x, uint8_t y, uint8_t chr);
    void ShowNum(uint8_t x, uint8_t y,uint32_t num, uint8_t len, uint8_t size2);
    void ShowString(uint8_t x, uint8_t y, const uint8_t *chr);
    void SetPos(uint8_t x, uint8_t y);
    //void ShowCHinese(uint8_t x, uint8_t y, uint8_t no);
    void DrawBMP(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, const uint8_t BMP[]);
  
private:
    enum LcdOp{
        oldeOp_Cmd      = 0x00,
        oledOp_Data     = 0x40,
        oledOp_Null
    };
    I2c *i2c_;
    Gpio *resetIo_;
    Gpio *enableIo_;
  
};

#endif