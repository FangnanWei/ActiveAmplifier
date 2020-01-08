/* Includes ------------------------------------------------------------------*/
#include "I2C.h"
/*******************************************************************************
* Function Name  : I2C_Configuration
* Description    : 
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void I2C_Configuration(void)
{
   I2C_InitTypeDef  I2C_InitStructure;
   GPIO_InitTypeDef  GPIO_InitStructure; 

   RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO , ENABLE);

   GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
   GPIO_Init(GPIOB, &GPIO_InitStructure);
   
   I2C_DeInit(I2C2);
   I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
   I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
   I2C_InitStructure.I2C_OwnAddress1 = 0x30;
   I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
   I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
   I2C_InitStructure.I2C_ClockSpeed = 100000;  /* 100K速度 */
    
   I2C_Cmd(I2C2, ENABLE);
   I2C_Init(I2C2, &I2C_InitStructure);
   /*允许1字节1应答模式*/
   I2C_AcknowledgeConfig(I2C2, ENABLE);
}
 
/*******************************************************************************
* Function Name  : I2C_delay
* Description    : 延迟时间
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void I2C_delay(uint16_t cnt)
{
	while(cnt--);
}

/*******************************************************************************
* Function Name  : I2C_AcknowledgePolling
* Description    : 等待获取I2C总线控制权 判断忙状态
* Input          : - I2Cx:I2C寄存器基址
*                  - I2C_Addr:从器件地址
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void I2C_AcknowledgePolling(I2C_TypeDef *I2Cx,uint8_t I2C_Addr)
{
  vu16 SR1_Tmp;
  do
  {   
    I2C_GenerateSTART(I2Cx, ENABLE); /*起始位*/
    /*读SR1*/
    SR1_Tmp = I2C_ReadRegister(I2Cx, I2C_Register_SR1);
    /*器件地址(写)*/

	I2C_Send7bitAddress(I2Cx, I2C_Addr, I2C_Direction_Transmitter);

  }while(!(I2C_ReadRegister(I2Cx, I2C_Register_SR1) & 0x0002));
  
  I2C_ClearFlag(I2Cx, I2C_FLAG_AF);
    
  I2C_GenerateSTOP(I2Cx, ENABLE);  /*停止位*/  
}

int I2C_Read(I2C_TypeDef *I2Cx,uint8_t I2C_Addr,uint8_t addr,uint8_t *buf,uint16_t num)
{
	int timeout = 0;
    if(num==0)
	return 1;
	
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2C_Configuration();
            return -1;
        }
    }
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
		
	/*允许1字节1应答模式*/
	I2C_AcknowledgeConfig(I2Cx, ENABLE);


	/* 发送起始位 */
    I2C_GenerateSTART(I2Cx, ENABLE);
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2C_Configuration();
            return -1;
        }
    }
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/

    /*发送器件地址(写)*/
    I2C_Send7bitAddress(I2Cx,  I2C_Addr, I2C_Direction_Transmitter);
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2C_Configuration();
            return -1;
        }
    }
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	/*发送地址*/
	I2C_SendData(I2Cx, addr);
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2C_Configuration();
            return -1;
        }
    }
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*数据已发送*/
		
	/*起始位*/
	I2C_GenerateSTART(I2Cx, ENABLE);
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2C_Configuration();
            return -1;
        }
    }
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
	
	/*器件读*/
	I2C_Send7bitAddress(I2Cx, I2C_Addr, I2C_Direction_Receiver);
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2C_Configuration();
            return -1;
        }
    }
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	
    while (num)
    {
		if(num==1)
		{
     		I2C_AcknowledgeConfig(I2Cx, DISABLE);	/* 最后一位后要关闭应答的 */
    		I2C_GenerateSTOP(I2Cx, ENABLE);			/* 发送停止位 */
		}
	    timeout = I2C_WaiteAckTime;
        do{
            if (timeout-- < 1)
            {
                I2C_Configuration();
                return -1;
            }
        }
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));  /* EV7 */
	    *buf = I2C_ReceiveData(I2Cx);
	    buf++;
	    /* Decrement the read bytes counter */
	    num--;
    }
	/* 再次允许应答模式 */
	I2C_AcknowledgeConfig(I2Cx, ENABLE);

	return 0;
}	

int I2C_Write(I2C_TypeDef *I2Cx,uint8_t I2C_Addr,uint8_t addr,uint8_t *buf,uint16_t num)
{
    int timeout = 0;
    int i = 0;

    if (0 == num)
    {
        return 0;
    }
    /* 起始位 */
  	I2C_GenerateSTART(I2Cx, ENABLE);
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2C_Configuration();
            return -1;
        }
    }
  	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));  

  	/* 发送器件地址(写)*/
  	I2C_Send7bitAddress(I2Cx, I2C_Addr, I2C_Direction_Transmitter);
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2C_Configuration();
            return -1;
        }
    }
  	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  
  	/*发送地址*/
  	I2C_SendData(I2Cx, addr);
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2C_Configuration();
            return -1;
        }
    }
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    for(i = 0; i < num; i++)
    {
        I2C_SendData(I2Cx, buf[i]); 
        timeout = I2C_WaiteAckTime;
        do{
            if (timeout-- < 1)
            {
                I2C_Configuration();
                return -1;
            }
        }
      	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    }
  	
  	/* 停止位*/
  	I2C_GenerateSTOP(I2Cx, ENABLE);
	I2C_delay(1000);
	return 0;
}

int I2C_Read_16(I2C_TypeDef *I2Cx,uint8_t I2C_Addr,uint16_t addr,uint8_t *buf,uint16_t num)
{
	int timeout = 0;
    if(num==0)
	return 1;
	
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2C_Configuration();
            return -1;
        }
    }
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));
		
	/*允许1字节1应答模式*/
	I2C_AcknowledgeConfig(I2Cx, ENABLE);


	/* 发送起始位 */
    I2C_GenerateSTART(I2Cx, ENABLE);
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2C_Configuration();
            return -1;
        }
    }
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/

    /*发送器件地址(写)*/
    I2C_Send7bitAddress(I2Cx,  I2C_Addr, I2C_Direction_Transmitter);
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2C_Configuration();
            return -1;
        }
    }
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	/*发送地址*/
	I2C_SendData(I2Cx, addr>>8);
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2C_Configuration();
            return -1;
        }
    }
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*数据已发送*/
	I2C_SendData(I2Cx, addr&0xff);
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2C_Configuration();
            return -1;
        }
    }
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*数据已发送*/
		
	/*起始位*/
	I2C_GenerateSTART(I2Cx, ENABLE);
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2C_Configuration();
            return -1;
        }
    }
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
	
	/*器件读*/
	I2C_Send7bitAddress(I2Cx, I2C_Addr, I2C_Direction_Receiver);
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2C_Configuration();
            return -1;
        }
    }
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	
    while (num)
    {
		if(num==1)
		{
     		I2C_AcknowledgeConfig(I2Cx, DISABLE);	/* 最后一位后要关闭应答的 */
    		I2C_GenerateSTOP(I2Cx, ENABLE);			/* 发送停止位 */
		}
	    timeout = I2C_WaiteAckTime;
        do{
            if (timeout-- < 1)
            {
                I2C_Configuration();
                return -1;
            }
        }
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));  /* EV7 */
	    *buf = I2C_ReceiveData(I2Cx);
	    buf++;
	    /* Decrement the read bytes counter */
	    num--;
    }
	/* 再次允许应答模式 */
	I2C_AcknowledgeConfig(I2Cx, ENABLE);

	return 0;
}	

int I2C_Write_16(I2C_TypeDef *I2Cx,uint8_t I2C_Addr,uint16_t addr,uint8_t *buf,uint16_t num)
{
    int timeout = 0;
    int i = 0;

    if (0 == num)
    {
        return 0;
    }
    /* 起始位 */
  	I2C_GenerateSTART(I2Cx, ENABLE);
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2C_Configuration();
            return -1;
        }
    }
  	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));  

  	/* 发送器件地址(写)*/
  	I2C_Send7bitAddress(I2Cx, I2C_Addr, I2C_Direction_Transmitter);
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2C_Configuration();
            return -1;
        }
    }
  	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  
  	/*发送地址*/
  	I2C_SendData(I2Cx, addr>>8);
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2C_Configuration();
            return -1;
        }
    }
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_SendData(I2Cx, addr&0xff);
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2C_Configuration();
            return -1;
        }
    }
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    for(i = 0; i < num; i++)
    {
        I2C_SendData(I2Cx, buf[i]); 
        timeout = I2C_WaiteAckTime;
        do{
            if (timeout-- < 1)
            {
                I2C_Configuration();
                return -1;
            }
        }
      	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    }
  	
  	/* 停止位*/
  	I2C_GenerateSTOP(I2Cx, ENABLE);
	I2C_delay(1000);
	return 0;
}

