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
    
    //�ж��� ���»����ͷ�
    //�ͷ� �жϳ��̰���
    //���� ��ʼ��ʱ
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