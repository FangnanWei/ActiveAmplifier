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
   I2C_InitStructure.I2C_ClockSpeed = 100000;  /* 100K�ٶ� */
    
   I2C_Cmd(I2C2, ENABLE);
   I2C_Init(I2C2, &I2C_InitStructure);
   /*����1�ֽ�1Ӧ��ģʽ*/
   I2C_AcknowledgeConfig(I2C2, ENABLE);
}
 
/*******************************************************************************
* Function Name  : I2C_delay
* Description    : �ӳ�ʱ��
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
* Description    : �ȴ���ȡI2C���߿���Ȩ �ж�æ״̬
* Input          : - I2Cx:I2C�Ĵ�����ַ
*                  - I2C_Addr:��������ַ
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
static void I2C_AcknowledgePolling(I2C_TypeDef *I2Cx,uint8_t I2C_Addr)
{
  vu16 SR1_Tmp;
  do
  {   
    I2C_GenerateSTART(I2Cx, ENABLE); /*��ʼλ*/
    /*��SR1*/
    SR1_Tmp = I2C_ReadRegister(I2Cx, I2C_Register_SR1);
    /*������ַ(д)*/

	I2C_Send7bitAddress(I2Cx, I2C_Addr, I2C_Direction_Transmitter);

  }while(!(I2C_ReadRegister(I2Cx, I2C_Register_SR1) & 0x0002));
  
  I2C_ClearFlag(I2Cx, I2C_FLAG_AF);
    
  I2C_GenerateSTOP(I2Cx, ENABLE);  /*ֹͣλ*/  
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
		
	/*����1�ֽ�1Ӧ��ģʽ*/
	I2C_AcknowledgeConfig(I2Cx, ENABLE);


	/* ������ʼλ */
    I2C_GenerateSTART(I2Cx, ENABLE);
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2C_Configuration();
            return -1;
        }
    }
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,��ģʽ*/

    /*����������ַ(д)*/
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

	/*���͵�ַ*/
	I2C_SendData(I2Cx, addr);
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2C_Configuration();
            return -1;
        }
    }
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*�����ѷ���*/
		
	/*��ʼλ*/
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
	
	/*������*/
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
     		I2C_AcknowledgeConfig(I2Cx, DISABLE);	/* ���һλ��Ҫ�ر�Ӧ��� */
    		I2C_GenerateSTOP(I2Cx, ENABLE);			/* ����ֹͣλ */
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
	/* �ٴ�����Ӧ��ģʽ */
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
    /* ��ʼλ */
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

  	/* ����������ַ(д)*/
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
  
  	/*���͵�ַ*/
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
  	
  	/* ֹͣλ*/
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
		
	/*����1�ֽ�1Ӧ��ģʽ*/
	I2C_AcknowledgeConfig(I2Cx, ENABLE);


	/* ������ʼλ */
    I2C_GenerateSTART(I2Cx, ENABLE);
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2C_Configuration();
            return -1;
        }
    }
    while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,��ģʽ*/

    /*����������ַ(д)*/
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

	/*���͵�ַ*/
	I2C_SendData(I2Cx, addr>>8);
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2C_Configuration();
            return -1;
        }
    }
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*�����ѷ���*/
	I2C_SendData(I2Cx, addr&0xff);
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2C_Configuration();
            return -1;
        }
    }
    while (!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*�����ѷ���*/
		
	/*��ʼλ*/
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
	
	/*������*/
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
     		I2C_AcknowledgeConfig(I2Cx, DISABLE);	/* ���һλ��Ҫ�ر�Ӧ��� */
    		I2C_GenerateSTOP(I2Cx, ENABLE);			/* ����ֹͣλ */
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
	/* �ٴ�����Ӧ��ģʽ */
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
    /* ��ʼλ */
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

  	/* ����������ַ(д)*/
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
  
  	/*���͵�ַ*/
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
  	
  	/* ֹͣλ*/
  	I2C_GenerateSTOP(I2Cx, ENABLE);
	I2C_delay(1000);
	return 0;
}

static int32_t I2c_Read(const struct I2c *self, uint8_t slaveAddr, uint8_t addr, uint8_t *buf, uint16_t len)
{
	int timeout = 0;
    if(len==0)
		return 1;
	
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2c_ReOpen(self);
            return -1;
        }
    }
	while(I2C_GetFlagStatus(self->pp.I2Cx, I2C_FLAG_BUSY));
		
	/*����1�ֽ�1Ӧ��ģʽ*/
	I2C_AcknowledgeConfig(self->pp.I2Cx, ENABLE);


	/* ������ʼλ */
    I2C_GenerateSTART(self->pp.I2Cx, ENABLE);
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2c_ReOpen(self);
            return -1;
        }
    }
    while(!I2C_CheckEvent(self->pp.I2Cx, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,��ģʽ*/

    /*����������ַ(д)*/
    I2C_Send7bitAddress(self->pp.I2Cx,  slaveAddr, I2C_Direction_Transmitter);
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2c_ReOpen(self);
            return -1;
        }
    }
    while (!I2C_CheckEvent(self->pp.I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	/*���͵�ַ*/
	I2C_SendData(self->pp.I2Cx, addr);
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2c_ReOpen(self);
            return -1;
        }
    }
    while (!I2C_CheckEvent(self->pp.I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*�����ѷ���*/
		
	/*��ʼλ*/
	I2C_GenerateSTART(self->pp.I2Cx, ENABLE);
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2c_ReOpen(self);
            return -1;
        }
    }
	while(!I2C_CheckEvent(self->pp.I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
	
	/*������*/
	I2C_Send7bitAddress(self->pp.I2Cx, slaveAddr, I2C_Direction_Receiver);
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2c_ReOpen(self);
            return -1;
        }
    }
	while(!I2C_CheckEvent(self->pp.I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	
    while (len)
    {
		if(len==1)
		{
     		I2C_AcknowledgeConfig(self->pp.I2Cx, DISABLE);	/* ���һλ��Ҫ�ر�Ӧ��� */
    		I2C_GenerateSTOP(self->pp.I2Cx, ENABLE);			/* ����ֹͣλ */
		}
	    timeout = I2C_WaiteAckTime;
        do{
            if (timeout-- < 1)
            {
                I2c_ReOpen(self);
                return -1;
            }
        }
		while(!I2C_CheckEvent(self->pp.I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));  /* EV7 */
	    *buf = I2C_ReceiveData(self->pp.I2Cx);
	    buf++;
	    /* Decrement the read bytes counter */
	    len--;
    }
	/* �ٴ�����Ӧ��ģʽ */
	I2C_AcknowledgeConfig(self->pp.I2Cx, ENABLE);

	return 0;

}

static int32_t I2c_Write(const struct I2c *self, uint8_t slaveAddr, uint8_t addr, uint8_t *buf, uint16_t len)
{
    int timeout = 0;
    int i = 0;

    if (0 == len)
    	return 0;
    
    /* ��ʼλ */
  	I2C_GenerateSTART(self->pp.I2Cx, ENABLE);
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2c_ReOpen(self);
            return -1;
        }
    }
  	while(!I2C_CheckEvent(self->pp.I2Cx, I2C_EVENT_MASTER_MODE_SELECT));  

  	/* ����������ַ(д)*/
  	I2C_Send7bitAddress(self->pp.I2Cx, slaveAddr, I2C_Direction_Transmitter);
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2c_ReOpen(self);
            return -1;
        }
    }
  	while(!I2C_CheckEvent(self->pp.I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  
  	/*���͵�ַ*/
  	I2C_SendData(self->pp.I2Cx, addr);
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2c_ReOpen(self);
            return -1;
        }
    }
	while(!I2C_CheckEvent(self->pp.I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    for(i = 0; i < len; i++)
    {
        I2C_SendData(self->pp.I2Cx, buf[i]); 
        timeout = I2C_WaiteAckTime;
        do{
            if (timeout-- < 1)
            {
                I2c_ReOpen(self);
                return -1;
            }
        }
      	while(!I2C_CheckEvent(self->pp.I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    }
  	
  	/* ֹͣλ*/
  	I2C_GenerateSTOP(self->pp.I2Cx, ENABLE);
	I2C_delay(1000);
	return 0;

}

static int32_t I2c_Read_16(const struct I2c *self, uint8_t slaveAddr, uint8_t addr, uint8_t *buf, uint16_t len)
{
	int timeout = 0;
    if(0 == len)
		return 1;
	
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2c_ReOpen(self);
            return -1;
        }
    }
	while(I2C_GetFlagStatus(self->pp.I2Cx, I2C_FLAG_BUSY));
		
	/*����1�ֽ�1Ӧ��ģʽ*/
	I2C_AcknowledgeConfig(self->pp.I2Cx, ENABLE);


	/* ������ʼλ */
    I2C_GenerateSTART(self->pp.I2Cx, ENABLE);
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2c_ReOpen(self);
            return -1;
        }
    }
    while(!I2C_CheckEvent(self->pp.I2Cx, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,��ģʽ*/

    /*����������ַ(д)*/
    I2C_Send7bitAddress(self->pp.I2Cx,  slaveAddr, I2C_Direction_Transmitter);
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2c_ReOpen(self);
            return -1;
        }
    }
    while (!I2C_CheckEvent(self->pp.I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	/*���͵�ַ*/
	I2C_SendData(self->pp.I2Cx, addr>>8);
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2c_ReOpen(self);
            return -1;
        }
    }
    while (!I2C_CheckEvent(self->pp.I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*�����ѷ���*/
	I2C_SendData(self->pp.I2Cx, addr&0xff);
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2c_ReOpen(self);
            return -1;
        }
    }
    while (!I2C_CheckEvent(self->pp.I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));/*�����ѷ���*/
		
	/*��ʼλ*/
	I2C_GenerateSTART(self->pp.I2Cx, ENABLE);
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2c_ReOpen(self);
            return -1;
        }
    }
	while(!I2C_CheckEvent(self->pp.I2Cx, I2C_EVENT_MASTER_MODE_SELECT));
	
	/*������*/
	I2C_Send7bitAddress(self->pp.I2Cx, slaveAddr, I2C_Direction_Receiver);
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2c_ReOpen(self);
            return -1;
        }
    }
	while(!I2C_CheckEvent(self->pp.I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	
    while (len)
    {
		if(len==1)
		{
     		I2C_AcknowledgeConfig(self->pp.I2Cx, DISABLE);	/* ���һλ��Ҫ�ر�Ӧ��� */
    		I2C_GenerateSTOP(self->pp.I2Cx, ENABLE);			/* ����ֹͣλ */
		}
	    timeout = I2C_WaiteAckTime;
        do{
            if (timeout-- < 1)
            {
                I2c_ReOpen(self);
                return -1;
            }
        }
		while(!I2C_CheckEvent(self->pp.I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED));  /* EV7 */
	    *buf = I2C_ReceiveData(self->pp.I2Cx);
	    buf++;
	    /* Decrement the read bytes counter */
	    len--;
    }
	/* �ٴ�����Ӧ��ģʽ */
	I2C_AcknowledgeConfig(self->pp.I2Cx, ENABLE);

	return 0;

}

static int32_t I2c_Write_16(const struct I2c *self, uint8_t slaveAddr, uint8_t addr, uint8_t *buf, uint16_t len)
{
    int timeout = 0;
    int i = 0;

    if (0 == len)
    {
        return 0;
    }
    /* ��ʼλ */
  	I2C_GenerateSTART(self->pp.I2Cx, ENABLE);
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2c_ReOpen(self);
            return -1;
        }
    }
  	while(!I2C_CheckEvent(self->pp.I2Cx, I2C_EVENT_MASTER_MODE_SELECT));  

  	/* ����������ַ(д)*/
  	I2C_Send7bitAddress(self->pp.I2Cx, slaveAddr, I2C_Direction_Transmitter);
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2c_ReOpen(self);
            return -1;
        }
    }
  	while(!I2C_CheckEvent(self->pp.I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  
  	/*���͵�ַ*/
  	I2C_SendData(self->pp.I2Cx, addr>>8);
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2c_ReOpen(self);
            return -1;
        }
    }
	while(!I2C_CheckEvent(self->pp.I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_SendData(self->pp.I2Cx, addr&0xff);
    timeout = I2C_WaiteAckTime;
    do{
        if (timeout-- < 1)
        {
            I2c_ReOpen(self);
            return -1;
        }
    }
	while(!I2C_CheckEvent(self->pp.I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    for(i = 0; i < len; i++)
    {
        I2C_SendData(self->pp.I2Cx, buf[i]); 
        timeout = I2C_WaiteAckTime;
        do{
            if (timeout-- < 1)
            {
                I2c_ReOpen(self);
                return -1;
            }
        }
      	while(!I2C_CheckEvent(self->pp.I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    }
  	
  	/* ֹͣλ*/
  	I2C_GenerateSTOP(self->pp.I2Cx, ENABLE);
	I2C_delay(1000);
	return 0;

}

static void I2c_Close(const struct I2c *self)
{
	if(self) {
		I2C_DeInit(self->pp.I2Cx);
		free(self);
	}
}

struct I2c *I2c_Open(I2C_TypeDef *I2Cx) 
{	
	I2C_InitTypeDef  I2C_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure; 
	
	struct I2c *i2c = (struct I2c *)malloc(sizeof(struct I2c));

	i2c->pp.I2Cx = I2Cx;
	
	i2c->Read = I2c_Read;
	i2c->Write = I2c_Write;
	i2c->Read_16 = I2c_Read_16;
	i2c->Write_16 = I2c_Write_16;
	i2c->Close = I2c_Close;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO , ENABLE);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	I2C_DeInit(i2c->pp.I2Cx);
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0x30;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 100000;	/* 100K�ٶ� */
	 
	I2C_Cmd(i2c->pp.I2Cx, ENABLE);
	I2C_Init(i2c->pp.I2Cx, &I2C_InitStructure);
	/*����1�ֽ�1Ӧ��ģʽ*/
	I2C_AcknowledgeConfig(i2c->pp.I2Cx, ENABLE);

	return i2c;
}

//���´�
void I2c_ReOpen(const struct I2c *self)
{
	struct I2c *newI2c = I2c_Open(self->pp.I2Cx);
	memcpy(self, newI2c, sizeof(struct I2c));
	free(newI2c);
}
