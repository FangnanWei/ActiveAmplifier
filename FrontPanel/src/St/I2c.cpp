#include "St/I2c.h"
#include "St/Sys.h"

const uint32_t I2C_FLAG_TIMEOUT = 0x1000;
const uint32_t I2C_LONG_TIMEOUT = 10 * I2C_FLAG_TIMEOUT;
const uint32_t MAX_TRIALS_NUMBER = 150;
static uint32_t I2CTimeout = 0;

extern "C" {
static ErrorStatus I2cErrorCallback(void)
{
    I2C_GenerateSTOP(ENABLE);
    return ERROR;
}
}

I2c::I2c(uint8_t slaveAddress, bool useWordRegAddr) {
  slaveAddress_ = slaveAddress;
  useWordRegAddr_ = useWordRegAddr;
}

ErrorStatus I2c::Send(uint8_t* pBuffer, uint16_t writeAddr, uint16_t numByteToWrite) {

    I2CTimeout = I2C_LONG_TIMEOUT;
    while(I2C_GetFlagStatus(I2C_FLAG_BUSBUSY))
    {
        if((I2CTimeout--) == 0)
            return I2cErrorCallback();
    }

    I2C_GenerateSTART( ENABLE);
    I2CTimeout = I2C_FLAG_TIMEOUT;
    while(!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT))
    {
        if((I2CTimeout--) == 0)
            return I2cErrorCallback();
    }

    I2C_Send7bitAddress((uint8_t)slaveAddress_, I2C_DIRECTION_TX);
    I2CTimeout = I2C_FLAG_TIMEOUT;
    while(!I2C_CheckEvent( I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        if((I2CTimeout--) == 0)
            return I2cErrorCallback();
    }
    if (!useWordRegAddr_) {
        I2C_SendData((uint8_t)writeAddr);
    }
    else {
        I2C_SendData( (uint8_t)((writeAddr & 0xFF00) >> 8));
        I2CTimeout = I2C_FLAG_TIMEOUT;
        while(!I2C_CheckEvent( I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        {
            if((I2CTimeout--) == 0)
                return I2cErrorCallback();
        }
        
        I2C_SendData( (uint8_t)(writeAddr & 0x00FF));
    }
    
    I2CTimeout = I2C_FLAG_TIMEOUT;
    while(!I2C_CheckEvent( I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        if((I2CTimeout--) == 0)
            return I2cErrorCallback();
    }

    while(numByteToWrite > 0)
    {
        I2C_SendData(*pBuffer);
        I2CTimeout = I2C_LONG_TIMEOUT;

        while(!I2C_CheckEvent( I2C_EVENT_MASTER_BYTE_TRANSMITTED))	 //可靠,速度变慢EV8_2
        {
            if((I2CTimeout--) == 0)
                return I2cErrorCallback();
        }
        numByteToWrite--;
        pBuffer++;
    }

    I2C_GenerateSTOP(ENABLE);
    
    return SUCCESS;
}

ErrorStatus I2c::Receive(uint8_t* pBuffer, uint16_t readAddr, uint16_t numByteToRead) {
    
    I2CTimeout = I2C_LONG_TIMEOUT;
    while(I2C_GetFlagStatus(I2C_FLAG_BUSBUSY))
    {
        if((I2CTimeout--) == 0) return I2cErrorCallback();
    }

    I2C_AcknowledgeConfig(I2C_ACK_CURR);			//必须先使能一次ACK

    I2C_GenerateSTART( ENABLE);
    I2CTimeout = I2C_FLAG_TIMEOUT;
    while(!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT))
    {
        if((I2CTimeout--) == 0) return I2cErrorCallback();
    }

    I2C_Send7bitAddress(slaveAddress_, I2C_DIRECTION_TX);
    I2CTimeout = I2C_FLAG_TIMEOUT;
    while(!I2C_CheckEvent( I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        if((I2CTimeout--) == 0) return I2cErrorCallback();
    }

    //I2C_SendData((uint8_t)readAddr);
    //I2CTimeout = I2C_FLAG_TIMEOUT;
    //while(!I2C_CheckEvent( I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    //{
    //   if((I2CTimeout--) == 0) return I2cErrorCallback();
    //}

    if (!useWordRegAddr_) {
        I2C_SendData((uint8_t)readAddr);
    }
    else {
        I2C_SendData( (uint8_t)((readAddr & 0xFF00) >> 8));
        I2CTimeout = I2C_FLAG_TIMEOUT;
        while(!I2C_CheckEvent( I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        {
            if((I2CTimeout--) == 0)
                return I2cErrorCallback();
        }
        
        I2C_SendData( (uint8_t)(readAddr & 0x00FF));
    }
    
    I2CTimeout = I2C_FLAG_TIMEOUT;
    while(I2C_GetFlagStatus(I2C_FLAG_TRANSFERFINISHED) == RESET)
    {
        if((I2CTimeout--) == 0)
            return I2cErrorCallback();
    }  

    I2C_GenerateSTART( ENABLE);
    I2CTimeout = I2C_FLAG_TIMEOUT;
    while(!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT))
    {
        if((I2CTimeout--) == 0) return I2cErrorCallback();
    }

    I2C_Send7bitAddress(slaveAddress_, I2C_DIRECTION_RX);
    I2CTimeout = I2C_FLAG_TIMEOUT;
    while(!I2C_CheckEvent( I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    {
        if((I2CTimeout--) == 0) return I2cErrorCallback();
    }

    while(numByteToRead > 1) {
        I2CTimeout = I2C_FLAG_TIMEOUT;
        while(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED))
        {
            if((I2CTimeout--) == 0) return I2cErrorCallback();
        }
        *pBuffer = I2C_ReceiveData();

        pBuffer++;
        numByteToRead--;
    }

    I2C_AcknowledgeConfig(I2C_ACK_NONE);
    I2CTimeout = I2C_FLAG_TIMEOUT;
    while(!I2C_CheckEvent(I2C_EVENT_MASTER_BYTE_RECEIVED))
    {
        if((I2CTimeout--) == 0) return I2cErrorCallback();
    }

    *pBuffer = I2C_ReceiveData();
    I2C_GenerateSTOP(ENABLE);
    
    return SUCCESS;
}

ErrorStatus I2c::WaitSlaveStandbyState(void) {

  __IO uint8_t tmpSR1 = 0;
  __IO uint32_t sEETrials = 0;

 /* While the bus is busy */
  I2CTimeout = I2C_FLAG_TIMEOUT;
  while(I2C_GetFlagStatus(I2C_FLAG_BUSBUSY))
  {
     if((I2CTimeout--) == 0) return I2cErrorCallback();
  }

  /* Keep looping till the slave acknowledges his address or the maximum number
     of trials is reached (this number is defined by sEE_MAX_TRIALS_NUMBER define
     in stm8s_eval_i2c_ee.h file) */
  while (1)
  {
    /* Send START condition */
    I2C_GenerateSTART(ENABLE);

    /* Test on EV5 and clear it */
    I2CTimeout = I2C_FLAG_TIMEOUT;
    while(!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT))
    {
      if((I2CTimeout--) == 0) return I2cErrorCallback();
    }

    /* Send EEPROM address for write */
    I2C_Send7bitAddress(slaveAddress_, I2C_DIRECTION_TX);

    /* Wait for ADDR flag to be set (Slave acknowledged his address) */
    I2CTimeout = I2C_FLAG_TIMEOUT;
    do
    {
      /* Get the current value of the SR1 register */
      tmpSR1 = I2C->SR1;

      /* Update the timeout value and exit if it reach 0 */
      if((I2CTimeout--) == 0) return I2cErrorCallback();
    }
    /* Keep looping till the Address is acknowledged or the AF flag is
       set (address not acknowledged at time) */
    while((I2C_GetFlagStatus(I2C_FLAG_ADDRESSSENTMATCHED)== RESET) &
          (I2C_GetFlagStatus(I2C_FLAG_ACKNOWLEDGEFAILURE)== RESET));
    tmpSR1 = I2C->SR1;
    /* Check if the ADDR flag has been set */
    if (tmpSR1 & I2C_SR1_ADDR)
    {
      /* Clear ADDR Flag by reading SR1 then SR3 registers (SR1 have already
         been read) */
      (void)I2C->SR3;

      /* STOP condition */
      I2C_GenerateSTOP(ENABLE);

      /* Exit the function */
      return SUCCESS;
    }
    else
    {
      /* Clear AF flag */
      I2C_ClearFlag(I2C_FLAG_ACKNOWLEDGEFAILURE);
    }

    /* Check if the maximum allowed number of trials has bee reached */
    if (sEETrials++ == MAX_TRIALS_NUMBER)
    {
      /* If the maximum number of trials has been reached, exit the function */
      return I2cErrorCallback();
    }
  }

}
