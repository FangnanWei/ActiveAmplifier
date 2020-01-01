#include "Driver/St/Gpio.h"
#include "Driver/St/Sys.h"

void Gpio::Init() {
  GPIO_Init(bank_, pin_, mode_);
  //如果设置为输出，先失能
  if ((mode_ != GPIO_MODE_IN_FL_NO_IT) || (mode_ != GPIO_MODE_IN_PU_NO_IT) || (mode_ != GPIO_MODE_IN_FL_IT) || (mode_ != GPIO_MODE_IN_PU_IT)) {
    Disable();
  }
}

void Gpio::ReInit(GPIO_Mode_TypeDef newMode, bool newHighEnable) {
    this->mode_ = newMode;
    this->highEnable_ = newHighEnable;
    Init();
}

void Gpio::Enable() {
  if (highEnable_) {
    GPIO_WriteHigh(bank_, pin_);
  }
  else {
    GPIO_WriteLow(bank_, pin_);
  }
}

void Gpio::EnableForMs(uint32_t ms) {
  Enable();
  Sys::DelayMs(ms);
  Disable();
}

void Gpio::Disable() {
  if (highEnable_) {
    GPIO_WriteLow(bank_, pin_);
  }
  else {
    GPIO_WriteHigh(bank_, pin_);
  }
}

void Gpio::DisableForMs(uint32_t ms) {
  Disable();
  Sys::DelayMs(ms);
  Enable();
}

bool Gpio::GetIn() {
  return (bool)GPIO_ReadInputPin(bank_, pin_);
}

void Gpio::SetOut(bool isHigh) {
  if (isHigh) {
    GPIO_WriteHigh(bank_, pin_);
  }
  else {
    GPIO_WriteLow(bank_, pin_);
  }
}

bool Gpio::GetInState() {
    return highEnable_ == GetIn();
}

