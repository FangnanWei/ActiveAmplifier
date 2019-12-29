#include "Chip/Eeprom.h"

void Eeprom::Write(uint16_t startAddr, uint8_t *buf, uint16_t length)
{
    uint16_t first_page_size, last_page_size, next_page_base_addr, last_page_base_addr;
    uint16_t full_page_cnt;
    uint8_t *pdata = buf;

    if ((startAddr + length) > maxByte_) {
        //out of rang
        while(true);
    }
    
    first_page_size = pageSize_ - startAddr % pageSize_;
    full_page_cnt = (length - first_page_size)/pageSize_;			//ÂúÒ³ÊýÁ¿
    last_page_size  = (length - first_page_size) % pageSize_;
    next_page_base_addr = startAddr + first_page_size;
    last_page_base_addr = full_page_cnt * pageSize_ + next_page_base_addr;

    if (first_page_size > 0) {
        i2c_->Send(pdata, startAddr, first_page_size);
        i2c_->WaitSlaveStandbyState();
        //Sys::DelayMs(5);
    }

    pdata += first_page_size;

    if (full_page_cnt > 0) {
        for(int i = 0; i < full_page_cnt; i++) {
            i2c_->Send(pdata, next_page_base_addr, pageSize_);
            next_page_base_addr += pageSize_;
            pdata += pageSize_;
            i2c_->WaitSlaveStandbyState();
            //Sys::DelayMs(5);
        }
    }

    if (last_page_size > 0) {
        i2c_->Send(pdata, last_page_base_addr, last_page_size);
        i2c_->WaitSlaveStandbyState();
        //Sys::DelayMs(5);
    }
}

void Eeprom::Read(uint16_t startAddr, uint8_t *buf, uint16_t length)
{
   i2c_->Receive(buf, startAddr, length);
}

void Eeprom::Test() 
{
  uint8_t Tx1_Buffer[] = "STM8S I2C Firmware Library EEPROM driver example";
  uint8_t tempBuf[sizeof(Tx1_Buffer)];
  int i = 0;
  
  Write(0xe4, Tx1_Buffer, sizeof(Tx1_Buffer));
  Read(0xe4, tempBuf, sizeof(Tx1_Buffer));
  
  for (i = 0; i < sizeof(Tx1_Buffer); i++) {
    if (tempBuf[i] != Tx1_Buffer[i]) {
        while(1);//Failed£¡£¡
    }
  }
}