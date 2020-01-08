#include "Driver/Chip/RotaryEncoder.h"

RotaryEncoderType RotaryEncoder::Scan() {
   currIoAState_ = gpioA_->GetIn();
   currIoBState_ = gpioB_->GetIn();
   
   RotaryEncoderType currEncType = RotaryEncoder_None;
  
  /**  ��һ��������ȶ�״̬ ��¼�����ȶ�ʱ�ĵ�ƽ**/
  /**  �ڶ������������״̬ �Ƚ��ȶ�״̬��ƽ������״̬���ƽ����ȡ����**/
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