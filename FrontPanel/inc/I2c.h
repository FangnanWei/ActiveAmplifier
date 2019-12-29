#ifndef I2c_H__
#define I2c_H__

extern "C" {
#include "stm8s.h"
}

class I2c{
public:
  I2c(uint8_t slaveAddress, bool useWordRegAddr);
  
  ErrorStatus Receive(uint8_t* pBuffer, uint16_t readAddr, uint16_t numByteToRead);
  ErrorStatus Send(uint8_t* pBuffer, uint16_t writeAddr, uint16_t numByteToWrite);
  ErrorStatus WaitSlaveStandbyState(void);
  
private:
  uint8_t slaveAddress_;
  bool useWordRegAddr_;
};


#endif