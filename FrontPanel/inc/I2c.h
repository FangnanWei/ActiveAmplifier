#ifndef I2c_H__
#define I2c_H__

extern "C" {
#include "stm8s.h"
}

class I2c{
public:
  I2c(uint8_t slaveAddress, bool useWordRegAddr);
  
protected:
  ErrorStatus Receive(uint8_t* pBuffer, uint16_t readAddr, uint16_t numByteToRead);
  ErrorStatus Send(uint8_t* pBuffer, uint16_t writeAddr, uint16_t numByteToWrite);
  ErrorStatus WaitSlaveStandbyState(void);
  
private:
  uint8_t slaveAddress_;
  bool useWordRegAddr_;
};


class Eeprom : public I2c{
public:
  Eeprom(uint8_t address, bool useWordRegAddr, uint16_t pageSize, uint16_t maxByte) : I2c(address, useWordRegAddr) {
    pageSize_ = pageSize;
    maxByte_ = maxByte;
  }
  void Write(uint16_t startAddr, uint8_t *buf, uint16_t length);
  void Read(uint16_t startAddr, uint8_t *buf, uint16_t length);
  
  uint16_t pageSize_;
  uint16_t maxByte_;

};

#endif