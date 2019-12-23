#ifndef I2c_H__
#define I2c_H__

extern "C" {
#include "stm8s.h"
}


//使用静态函数，写模拟i2c的控制。
class I2cCtrl{
public:  
  static void Init();
  static void Start();
  static void Stop();
  static void WriteByte(uint8_t data);
  static uint8_t ReadByte(bool isNack);
  static void Restart();

  static void DelayNop();
};



class I2c{
public:
  I2c(uint8_t slaveAddress);
  void Receive(uint8_t* pBuffer, uint16_t readAddr, uint16_t numByteToRead);
  void Send(uint8_t* pBuffer, uint16_t writeAddr, uint16_t numByteToWrite);
  
private:
  uint8_t slaveAddress_;
};


class Eeprom : public I2c{
public:
  Eeprom(uint8_t address) : I2c(address) {
  }
  void Write(uint16_t startAddr, uint8_t *buf, uint16_t length);
  void Read(uint16_t startAddr, uint8_t *buf, uint16_t length);

};

#endif