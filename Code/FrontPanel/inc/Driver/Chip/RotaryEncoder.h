#ifndef RotaryEncoder_H_
#define RotaryEncoder_H_

extern "C" {
#include "stm8s.h"
}

#include "Driver/St/Gpio.h"

enum RotaryEncoderType{
    RotaryEncoder_LeftTurn,
    RotaryEncoder_RightTurn,
    RotaryEncoder_None
};

class RotaryEncoder{
public:
  RotaryEncoder(GpioParam &ioAParam, GpioParam &ioBParam) {
    gpioA_ = new Gpio(ioAParam);
    gpioA_->Init();
    gpioB_ = new Gpio(ioBParam);
    gpioB_->Init();
    
    stable_ = false;
  }
  
  RotaryEncoderType Scan();
private:
  Gpio *gpioA_;
  Gpio *gpioB_;
  
  bool stable_;
  bool currIoAState_;
  bool currIoBState_;
  bool IoAStableState_;
};

#endif

