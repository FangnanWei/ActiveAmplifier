#ifndef Gpio_H_
#define Gpio_H_

extern "C" {
#include "stm8s.h"
#include "stm8s_gpio.h"
}

struct GpioParam{
  GPIO_TypeDef* bank_;
  GPIO_Pin_TypeDef pin_;
  GPIO_Mode_TypeDef mode_;
  
  bool highEnable_;
  
  GpioParam(GPIO_TypeDef* bank, GPIO_Pin_TypeDef pin, GPIO_Mode_TypeDef mode, bool highEnable){
    this->bank_ = bank;
    this->pin_ = pin;
    this->mode_ = mode;

    this->highEnable_ = highEnable;
  }
  
};

class Gpio{
public:
  Gpio(GpioParam &param) {
    this->bank_ = param.bank_;
    this->pin_  = param.pin_;
    this->mode_ = param.mode_;
    this->highEnable_ = param.highEnable_;
  }
  void Init();
  void ReInit(GPIO_Mode_TypeDef newMode, bool newHighEnable);
  
  bool GetIn();
  void SetOut(bool isHigh);
  
  void Enable();
  void EnableForMs(uint32_t ms);
  void Disable();
  void DisableForMs(uint32_t ms);
  
  //输入是否被使能
  bool GetInState();
  
private: 
  GPIO_TypeDef* bank_;
  GPIO_Pin_TypeDef pin_;
  GPIO_Mode_TypeDef mode_;
  
  bool highEnable_;
};


#endif