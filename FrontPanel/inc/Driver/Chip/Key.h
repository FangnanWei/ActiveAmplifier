#ifndef Key_H_
#define Key_H_

extern "C" {
#include "stm8s.h"
}

#include "Driver/St/Gpio.h"

enum KeyPressType{
    KeyPressType_Short,
    KeyPressType_Long,
    KeyPressType_None
};


class Key{
public:
  Key(GpioParam &ioParam, uint16_t longPressTime) {
    gpio_ = new Gpio(ioParam);
    gpio_->Init();
    longPressTime_ = longPressTime;
    startScan_ = false;
    pressTime_ = 0;
  }
  
  KeyPressType Scan();
  
private:
    Gpio *gpio_;
    uint16_t longPressTime_;
    
    //״ֵ̬
    bool startScan_;
    
    bool prevStare_; 
    bool currStare_;
    
    bool press_;
    uint32_t pressTime_;
};

#endif