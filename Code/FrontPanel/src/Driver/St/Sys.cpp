#include "Driver/St/Sys.h"


const uint32_t TimeCounterPeriod = HSE_VALUE/1000;
const uint32_t I2cSpeed = 200000;//100K, EEPROM正常最高跑2.5K，过高将出错
const uint16_t I2cOwnAddress = 0xA0;

static void ClockInit() { 
    CLK_DeInit();
    
    //使能外部时钟
    CLK_HSECmd(ENABLE);
    
    //内部时钟分频设置为1
    /* Configure the Fcpu to DIV1*/
    CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
    
    //切换时钟源到HSE
    /* Configure the system clock to use HSE clock source and to run at 24Mhz */
    CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSE, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE);

    //配置使能外设时钟
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_I2C, ENABLE);
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART3, ENABLE);
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER1, ENABLE);
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER2, ENABLE);
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER3, ENABLE);
}

static void TimeCounterInit() {
    TIM3_TimeBaseInit(TIM3_PRESCALER_1, (uint16_t)TimeCounterPeriod);
    TIM3_ARRPreloadConfig(ENABLE);
    TIM3_ITConfig(TIM3_IT_UPDATE, ENABLE);
    TIM3_Cmd(ENABLE);
}

static void BeepInit() {
    BEEP_LSICalibrationConfig(LSI_VALUE);
    BEEP_Init(BEEP_FREQUENCY_4KHZ);
    
    BEEP_Cmd(DISABLE);
}


static void I2cCtrlerInit() {
  /* I2C Peripheral Enable */
  I2C_Cmd( ENABLE);
    /* I2C configuration after enabling it */
  I2C_Init(I2cSpeed, I2cOwnAddress, I2C_DUTYCYCLE_2, I2C_ACK_CURR, I2C_ADDMODE_7BIT, CLK_GetClockFreq() / 1000000);
}

void Sys::CpuInit() {
  //时钟初始化
  ClockInit();
  
  //蜂鸣器初始化
  BeepInit();
  
  //计数定时器初始化
  TimeCounterInit();
  
  //初始化i2c控制器
  I2cCtrlerInit();
}

void Sys::DelayMs(uint32_t ms) {
  uint32_t stopTimeCnt = TimeCounterCntGet() + ms;
  
  while (TimeCounterCntGet() < stopTimeCnt) {
    nop();
  }
}

void Sys::DelaySecond(uint32_t second) {
  uint32_t ms =  second * 1000;
  DelayMs(ms);
}

uint32_t Sys::GetTimeMs() {
    return TimeCounterCntGet();
}

void Sys::BeepRingForMs(uint32_t ms) {
  BEEP_Cmd(ENABLE);
  DelayMs(ms);
  BEEP_Cmd(DISABLE);
}

