#include "Driver/Chip/RotaryEncoder.h"

RotaryEncoderType RotaryEncoder::Scan() {
   currIoAState_ = gpioA_->GetIn();
   currIoBState_ = gpioB_->GetIn();
   
   RotaryEncoderType currEncType = RotaryEncoder_None;
  
  /**  第一步：检测稳定状态 记录两线稳定时的电平**/
  /**  第二步：检测跳变状态 比较稳定状态电平和跳变状态后电平，获取方向**/
  if (!stable_) {
    if (currIoAState_ == currIoBState_) {
        stable_= true;
        IoAStableState_ = currIoAState_;
    }
  }
  else {
    if (currIoAState_ != currIoBState_) {
      if (currIoAState_ != IoAStableState_) {
        currEncType = RotaryEncoder_RightTurn;
      }
      else {
        currEncType = RotaryEncoder_LeftTurn;
      }
      stable_= false;
    }
  }
  
  return currEncType;
}