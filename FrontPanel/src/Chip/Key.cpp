#include "Chip/Key.h"
#include "Sys.h"

KeyPressType Key::Scan() {
    KeyPressType newPressType = KeyPressType_None;
    
    if (false == startScan_) {
        prevStare_ = gpio_->GetInState();
        pressTime_ = Sys::GetTimeMs();
        startScan_ = true;
        return KeyPressType_None;
    }
    
    currStare_ = gpio_->GetInState();
    
    //有动作 按下或者释放
    //释放 判断长短按下
    //按下 开始计时
    if (prevStare_ != currStare_) {
      if (prevStare_) {
        if ((Sys::GetTimeMs() - pressTime_) < longPressTime_) {
            newPressType =  KeyPressType_Short;
        }
        else {
            newPressType = KeyPressType_Long;
        }
      }
      else {
        pressTime_ = Sys::GetTimeMs();
      }
    }
    prevStare_ = currStare_;
    
    return newPressType;
}