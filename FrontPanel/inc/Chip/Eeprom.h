#ifndef Eeprom_H__
#define Eeprom_H__

extern "C" {
#include "stm8s.h"
}
#include "I2c.h"

class Eeprom{
public:
  Eeprom(uint8_t address, bool useWordRegAddr, uint16_t pageSize, uint16_t maxByte) {
    
    i2c_ = new I2c(address, useWordRegAddr);
    pageSize_ = pageSize;
    maxByte_ = maxByte;
  }
  void Write(uint16_t startAddr, uint8_t *buf, uint16_t length);
  void Read(uint16_t startAddr, uint8_t *buf, uint16_t length);
  void Test();
private:  
  uint16_t pageSize_;
  uint16_t maxByte_;
  
  I2c *i2c_;
};

#endif