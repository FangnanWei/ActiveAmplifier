#include "I2c.h"
#include "Sys.h"

I2c::I2c(uint8_t slaveAddress) {
  slaveAddress_ = slaveAddress;
}

void I2c::Receive(uint8_t* pBuffer, uint16_t readAddr, uint16_t numByteToRead) {
#if 0
  uint16_t i = 0;

  I2cCtrl::Start();
  I2cCtrl::WriteByte(slaveAddress_);
  I2cCtrl::WriteByte(readAddr);
  I2cCtrl::Restart();
  I2cCtrl::WriteByte(slaveAddress_ + 1);

  for (i = 0; i < numByteToRead; i++)
  {
      pBuffer[i] = I2cCtrl::ReadByte(i == (numByteToRead - 1));
  }
  I2cCtrl::Stop();
  
#else  
  /* While the bus is busy */
  while(I2C_GetFlagStatus( I2C_FLAG_BUSBUSY))
  {
    ;//TimeOut
  }

  /* Send START condition */
  I2C_GenerateSTART(ENABLE);

  /* Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
  while(!I2C_CheckEvent( I2C_EVENT_MASTER_MODE_SELECT))
  {
    ;//TimeOut
  }

  /* Send EEPROM address for write */
  I2C_Send7bitAddress(slaveAddress_, I2C_DIRECTION_TX);

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent( I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    ;//TimeOut
  }

  /* Send the EEPROM's internal address to read from: MSB of the address first */
  I2C_SendData( readAddr);

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent( I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    ;//TimeOut
  }

  //是否需要？？再判断
  /* Test on EV8 and clear it */
  while(I2C_GetFlagStatus(I2C_FLAG_TRANSFERFINISHED) == RESET)
  {
    ;//TimeOut
  }

  /* Send START condition a second time */
  I2C_GenerateSTART( ENABLE);

  /* Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
  while(!I2C_CheckEvent( I2C_EVENT_MASTER_MODE_SELECT))
  {
    ;//TimeOut
  }

  /* Send EEPROM address for read */
  I2C_Send7bitAddress(slaveAddress_, I2C_DIRECTION_RX);

  /* Read data from first byte until byte N-3 */
  if (numByteToRead> 3)
    {
      /* Poll on BTF */
      while (I2C_GetFlagStatus( I2C_FLAG_TRANSFERFINISHED) == RESET)
      {
        ;//TimeOut
      }

      /* Read a byte from the EEPROM */
      *pBuffer = I2C_ReceiveData();

      /* Point to the next location where the byte read will be saved */
      *pBuffer++;

      /* Decrement the read bytes counter */
      numByteToRead--;
    }

  /*  Remains three data for read: data N-2, data N-1, Data N */
  /* Three Bytes Master Reception procedure (POLLING) ------------------------*/
  if (numByteToRead == 3)
  {
    /* Data N-2 in DR and data N -1 in shift register */
    /* Poll on BTF */
      while (I2C_GetFlagStatus( I2C_FLAG_TRANSFERFINISHED) == RESET)
      {
        ;//TimeOut
      }

      /* Clear ACK */
      I2C_AcknowledgeConfig(I2C_ACK_NONE);

      /* Call User callback for critical section start (should typically disable interrupts) */
      //sEE_EnterCriticalSection_UserCallback();
      enableInterrupts();

      /* Read Data N-2 */
      *pBuffer = I2C_ReceiveData();

      /* Point to the next location where the byte read will be saved */
      *pBuffer++;

      /* Program the STOP */
      I2C_GenerateSTOP(ENABLE);

      /* Read DataN-1 */
      *pBuffer = I2C_ReceiveData();

       /* Call User callback for critical section end (should typically re-enable interrupts) */
       //sEE_ExitCriticalSection_UserCallback();
      disableInterrupts();

      /* Point to the next location where the byte read will be saved */
      *pBuffer++;

      /* Poll on RxNE */
      while (I2C_GetFlagStatus( I2C_FLAG_RXNOTEMPTY) == RESET)
      {
        ;//TimeOut
      }
      /* Read DataN */
      *pBuffer = I2C_ReceiveData();

      /* Reset the number of bytes to be read from the EEPROM */
      numByteToRead = 0;

    }

  /* If number of data to be read is 2 */
  /* Tow Bytes Master Reception procedure (POLLING) ---------------------------*/
  if (numByteToRead == 2)
  {
    /* Enable acknowledgement on next byte (set POS and ACK bits)*/
    I2C_AcknowledgeConfig(I2C_ACK_NEXT);

    /* Wait on ADDR flag to be set (ADDR is still not cleared at this level */
    while(I2C_GetFlagStatus( I2C_FLAG_ADDRESSSENTMATCHED) == RESET)
    {
      ;//TimeOut
    }

    /* Clear ADDR register by reading SR1 then SR3 register (SR1 has already been read) */
     (void)I2C->SR3;

    /* Disable Acknowledgement */
    I2C_AcknowledgeConfig(I2C_ACK_NONE);

    /* Wait for BTF flag to be set */
      while (I2C_GetFlagStatus( I2C_FLAG_TRANSFERFINISHED) == RESET)
      {
        ;//TimeOut
      }

    /* Call User callback for critical section start (should typically disable interrupts) */
    //sEE_EnterCriticalSection_UserCallback();
      enableInterrupts();

    /* Program the STOP */
      I2C_GenerateSTOP(ENABLE);

    /* Read Data N-1 */
      *pBuffer = I2C_ReceiveData();

      /* Point to the next location where the byte read will be saved */
      *pBuffer++;

    /* Call User callback for critical section end (should typically re-enable interrupts) */
       //sEE_ExitCriticalSection_UserCallback();
      disableInterrupts();

    /* Read Data N */
      *pBuffer = I2C_ReceiveData();

    /* Reset the number of bytes to be read from the EEPROM */
      numByteToRead = 0;
  }

  /* If number of data to be read is 1 */
  /* One Byte Master Reception procedure (POLLING) ---------------------------*/
  if (numByteToRead < 2)
  {
    /* Wait on ADDR flag to be set (ADDR is still not cleared at this level */
    while(I2C_GetFlagStatus( I2C_FLAG_ADDRESSSENTMATCHED) == RESET)
    {
      ;//TimeOut
    }

    /* Disable Acknowledgement */
    I2C_AcknowledgeConfig(I2C_ACK_NONE);

    /* Call User callback for critical section start (should typically disable interrupts) */
    //sEE_EnterCriticalSection_UserCallback();
    enableInterrupts();

    /* Clear ADDR register by reading SR1 then SR3 register (SR1 has already been read) */
    (void)I2C->SR3;

    /* Send STOP Condition */
    I2C_GenerateSTOP( ENABLE);

    /* Call User callback for critical section end (should typically re-enable interrupts) */
    //sEE_ExitCriticalSection_UserCallback();
    enableInterrupts();
    
    /* Wait for the byte to be received */
    while(I2C_GetFlagStatus( I2C_FLAG_RXNOTEMPTY) == RESET)
    {
      ;//TimeOut
    }

    /* Read the byte received from the EEPROM */
    *pBuffer = I2C_ReceiveData();

    /* Decrement the read bytes counter */
    numByteToRead--;

    /* Wait to make sure that STOP control bit has been cleared */
    while(I2C->CR2 & I2C_CR2_STOP)
    {
      ;//TimeOut
    }

    /* Re-Enable Acknowledgement to be ready for another reception */
    I2C_AcknowledgeConfig( I2C_ACK_CURR);
  }
#endif  
}

void I2c::Send(uint8_t* pBuffer, uint16_t writeAddr, uint16_t numByteToWrite) {
#if 0    
    uint16_t i = 0;

    I2cCtrl::Start();
    I2cCtrl::WriteByte(slaveAddress_);
    I2cCtrl::WriteByte(writeAddr);
    for (i = 0; i < numByteToWrite; i++)
    {
        I2cCtrl::WriteByte(pBuffer[i]);
    }
    I2cCtrl::Stop();
#else
  
/* Set the pointer to the Number of data to be written.
     User should check on this variable in order to know if the
      data transfer has been completed or not. */

  /* While the bus is busy */
  while(I2C_GetFlagStatus(I2C_FLAG_BUSBUSY))
  {
    ;//TimeOut
  }

  /* Send START condition */
  I2C_GenerateSTART(ENABLE);

  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT))
  {
    ;//TimeOut
  }

  /* Send address for write */
  I2C_Send7bitAddress(slaveAddress_, I2C_DIRECTION_TX);

  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    ;//TimeOut
  }

  /* Send the EEPROM's internal address to write to : only one byte Address */
  I2C_SendData(writeAddr);

  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    ;//TimeOut
  }

  /* While there is data to be written */
  while(numByteToWrite > 0)
  {
    /* Send the byte to be written */
    I2C_SendData(*pBuffer);

    /* Test on EV8 and clear it */
    /* Wait till all data have been physically transferred on the bus */
    while(!I2C_GetFlagStatus(I2C_FLAG_TRANSFERFINISHED))
    {
      ;//TimeOut
    }
    numByteToWrite--;
  }

  /* Send STOP condition */
  I2C_GenerateSTOP(ENABLE);

  /* Perform a read on SR1 and SR3 register to clear eventually pending flags */
  (void)I2C->SR1;
  (void)I2C->SR3;
#endif  
}

const  uint16_t AT24C02_WRITE_PAGE_SIZE = 8;	//叶宽
//const  uint16_t EEPROM_AT24C02_READ_RANG_SIZE = 256;	//2kBIT

void Eeprom::Write(uint16_t startAddr, uint8_t *buf, uint16_t length)
{
    uint16_t first_page_size, last_page_size, next_page_base_addr, last_page_base_addr;
    uint16_t full_page_cnt;
    uint8_t *pdata = buf;

    first_page_size = AT24C02_WRITE_PAGE_SIZE - startAddr % AT24C02_WRITE_PAGE_SIZE;
    full_page_cnt = (length - first_page_size)/AT24C02_WRITE_PAGE_SIZE;			//满页数量
    last_page_size  = (length - first_page_size) % AT24C02_WRITE_PAGE_SIZE;
    next_page_base_addr = startAddr + first_page_size;
    last_page_base_addr = full_page_cnt * AT24C02_WRITE_PAGE_SIZE + next_page_base_addr;

    if (first_page_size > 0) {
        Send(pdata, startAddr, first_page_size);
        Sys::DelayMs(5);
    }

    pdata += first_page_size;

    if (full_page_cnt > 0) {
        for(int i = 0; i < full_page_cnt; i++) {
            Send(pdata, next_page_base_addr, AT24C02_WRITE_PAGE_SIZE);
            next_page_base_addr += AT24C02_WRITE_PAGE_SIZE;
            pdata += AT24C02_WRITE_PAGE_SIZE;
            Sys::DelayMs(5);
        }
    }

    if (last_page_size > 0) {
        Send(pdata, last_page_base_addr, last_page_size);
        Sys::DelayMs(5);
    }
}

void Eeprom::Read(uint16_t startAddr, uint8_t *buf, uint16_t length)
{
   Receive(buf, startAddr, length);
}

#define I2C_TimeOut         ((unsigned long)0x10000)

#define I2cGpioPort			GPIOB
#define SclGpioPin			GPIO_PIN_4
#define SdaGpioPin			GPIO_PIN_5

#define SCL_Low()           GPIO_WriteLow(I2cGpioPort, SclGpioPin)
#define SCL_High()          GPIO_WriteHigh(I2cGpioPort, SclGpioPin)

#define SDA_Low()           GPIO_WriteLow(I2cGpioPort, SdaGpioPin)
#define SDA_High()          GPIO_WriteHigh(I2cGpioPort, SdaGpioPin)

#define SetSDA_Input()      GPIO_Init(I2cGpioPort, SdaGpioPin, GPIO_MODE_IN_PU_NO_IT)
#define SetSDA_Output()     GPIO_Init(I2cGpioPort, SdaGpioPin, GPIO_MODE_OUT_OD_HIZ_FAST)

#define Read_SDA()          GPIO_ReadInputPin(I2cGpioPort, SdaGpioPin)


void I2cCtrl::DelayNop() {
    char i;
    for (i = 0; i < 1; i++) {
       asm("nop");
    }
}

void I2cCtrl::Init()
{
    GPIO_Init(I2cGpioPort, (GPIO_Pin_TypeDef)(SclGpioPin | SdaGpioPin), GPIO_MODE_OUT_PP_HIGH_FAST);
//    GPIO_Init(I2cGpioPort, , GPIO_MODE_OUT_PP_HIGH_FAST);
    SDA_High();
    SCL_High();
}

/* 执行前SCL = 1; SDA = 1;
     执行后SCL = 0; SDA = 0;*/
void I2cCtrl::Start()
{
    //DelayNop();
    SDA_Low();
    DelayNop();
    SCL_Low();
    DelayNop();
}

/* 执行前SCL = 0; SDA = ?
     执行后SCL = 1; SDA = 1*/
void I2cCtrl::Stop()
{
    DelayNop();
    SDA_Low();
    DelayNop();
    SCL_High();
    DelayNop();
    SDA_High();
    DelayNop();
}

/* 执行前SCL = 0; SDA = ?
     执行后SCL = 0; SDA = ?*/
void I2cCtrl::WriteByte(uint8_t data)
{
    uint8_t i = 0;
//    unsigned char ack = 0;

    for (i = 0; i < 8; i++)
    {
        if (data & (0x80>>i))
        {
            SDA_High();
        }
        else
        {
            SDA_Low();
        }
        DelayNop();
        SCL_High();
        DelayNop();
        SCL_Low();
        DelayNop();
    }
    SetSDA_Input();
    SCL_High();
    DelayNop();
    //ack = Read_SDA();
    Read_SDA();
    SCL_Low();
    SetSDA_Output();
}

/* 执行前SCL = ?;SDA = ?
    执行后SCL = 0; SDA = 0;*/
void I2cCtrl::Restart()
{
    DelayNop();
    SCL_Low();
    DelayNop();
    SDA_High();
    DelayNop();
    SCL_High();
    DelayNop();
    SDA_Low();
    DelayNop();
    SCL_Low();
    DelayNop();
}

/* 执行前SCL = 0; SDA = ?
     执行后SCL = 0; SDA = 0*/
uint8_t I2cCtrl::ReadByte(bool isNack)
{
    uint8_t data = 0;
    uint8_t i = 0;

    SetSDA_Input();
    for (i = 0; i < 8; i++)
    {
        SCL_Low();
		DelayNop();

        SCL_High();
        //DelayNop();
        data |= (Read_SDA()<<(7-i));
        //DelayNop();
    }
    SCL_Low();
    DelayNop();
    SetSDA_Output();
    SDA_Low();
	DelayNop();
	if (isNack)
		SDA_High();
    SCL_High();
	DelayNop();
    SCL_Low();
    return data;
}
