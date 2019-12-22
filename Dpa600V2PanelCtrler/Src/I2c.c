#include "I2c.h"
#include "Delay.h"

void I2cInit()
{
    GPIO_Init(I2cGpioPort, SclGpioPin | SdaGpioPin, GPIO_MODE_OUT_PP_HIGH_FAST);
//    GPIO_Init(I2cGpioPort, , GPIO_MODE_OUT_PP_HIGH_FAST);
    SDA_High();
    SCL_High();
}

/* 执行前SCL = 1; SDA = 1;
     执行后SCL = 0; SDA = 0;*/
void I2C_Start()
{
    //DelayNop();
    SDA_Low();
    DelayNop();
    SCL_Low();
    DelayNop();
}

/* 执行前SCL = 0; SDA = ?
     执行后SCL = 1; SDA = 1*/
void I2C_Stop()
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
void I2C_WriteByte(unsigned char data)
{
    unsigned char i = 0;
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

#if 0
/* 执行前SCL = ?;SDA = ?
    执行后SCL = 0; SDA = 0;*/
void I2C_Restart()
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
unsigned char I2C_ReadByte(bool isNack)
{
    unsigned char data = 0;
    unsigned char i = 0;

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

unsigned char I2C_Read(unsigned char slaveAddr, unsigned char regAddr, unsigned char *buf, unsigned short len)
{
    unsigned short i = 0;

    I2C_Start();
    I2C_WriteByte(slaveAddr);
    I2C_WriteByte(regAddr);
    I2C_Restart();
    I2C_WriteByte(slaveAddr + 1);

    for (i = 0; i < len; i++)
    {
        buf[i] = I2C_ReadByte(i == (len - 1));
    }
    I2C_Stop();
    return 0;
}
#endif

unsigned char I2C_Write(unsigned char slaveAddr, unsigned char regAddr, unsigned char *buf, unsigned short len)
{
    unsigned short i = 0;

    I2C_Start();
    I2C_WriteByte(slaveAddr);
    I2C_WriteByte(regAddr);
    for (i = 0; i < len; i++)
    {
        I2C_WriteByte(buf[i]);
    }
    I2C_Stop();
    return 0;
}

