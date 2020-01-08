#include "stm8s.h"
#include "app_i2c.h"
#include "app_clk.h"
//#include "app_gpio.h"
//#include "app_exti.h"
#include "app_beep.h"
#include "app_tim.h"
#include "app_iwdg.h"
#include "app.h"

#include "stdlib.h"

const uint16_t SOFT_VERSION = 1009;//版本号  如果软件版本升级 eeprom会相应的自动擦掉一次
extern uint32_t g_time_cnt;
extern struct device_global_prmt g_dev;//涓嶈兘鏄剧ず涓枃鍚楋紝涓枃鏈夐棶棰橈紝濂藉儚鏄病鏈夊姙娉曚簡

#define I2C_SPEED							100000
#define TIME3_PERIOD						12288

CLK_HandlerTypeDef hCLK;
Power_HandlerTypeDef hPower;
Encoder_HandlerTypeDef hEncoder;
Keys_TypeDef hKeys;
IWDG_InitTypeDef hIWDG = {.Inited = FALSE};

void CLK_Config(CLK_HandlerTypeDef *hCLK)
{
	hCLK->CLK_Init.CPUDIV_Prescaler = CLK_PRESCALER_CPUDIV1;
	hCLK->CLK_Init.HSIDIV_Prescaler = CLK_PRESCALER_HSIDIV1;
	hCLK->CLK_Init.SwitchMode = CLK_SWITCHMODE_AUTO;
	hCLK->CLK_Init.NewClock = CLK_SOURCE_HSE;
	hCLK->CLK_Init.ITState = ENABLE;
	hCLK->CLK_Init.CurrentClockState = CLK_CURRENTCLOCKSTATE_DISABLE;
	hCLK->CLK_Init.IT = CLK_IT_SWIF;
	hCLK->SwitchSuccess = FALSE;
	CLK_Init(hCLK);
}

void Power_Config(Power_HandlerTypeDef *hPower)
{
	hPower->GpioOn.GPIOx = GPIOC;
	hPower->GpioOn.Pin = GPIO_PIN_4;
	hPower->GpioOn.Mode = GPIO_MODE_OUT_PP_HIGH_FAST;

	hPower->GpioOff.GPIOx = GPIOC;
	hPower->GpioOff.Pin = GPIO_PIN_3;
	hPower->GpioOff.Mode = GPIO_MODE_OUT_PP_HIGH_FAST;
	hPower->IsOn = FALSE;
	Power_Init(hPower);
}

void BEEP_Config(BEEP_InitTypeDef * hBeep)
{
	hBeep->Frequency = BEEP_FREQUENCY_4KHZ;
	hBeep->LSIFreqHz = 128000;
	BEEP_BSP_Init(hBeep);
}

void TIM3_Config(TIM3_InitTypeDef * hTIM3)
{
	hTIM3->Prescaler = TIM3_PRESCALER_1;
	hTIM3->Period = TIME3_PERIOD;
	hTIM3->ARRPreload = ENABLE;
	hTIM3->IT = TIM3_IT_UPDATE;
	hTIM3->ITState = ENABLE;
	hTIM3->Cmd = ENABLE;
	TIM3_BSP_Init(hTIM3);
}

void IWDG_Config(IWDG_InitTypeDef * hIWDG)
{
	hIWDG->WriteAccess = IWDG_WriteAccess_Enable;
	hIWDG->Prescaler = IWDG_Prescaler_256;
	hIWDG->Reload = 0xff;
	IWDG_Init(hIWDG);
}

void I2C_Config(I2C_InitTypeDef *hI2C)
{
	uint8_t InputClock;

	InputClock = CLK_GetClockFreq() / 1000000;
	
	hI2C->OutputClockFrequencyHz = I2C_SPEED;
	hI2C->OwnAddress = 0xAA;
	hI2C->DutyCycle = I2C_DUTYCYCLE_2; 
	hI2C->Ack = I2C_ACK_CURR;
	hI2C->AddMode = I2C_ADDMODE_7BIT;
	hI2C->InputClockFrequencyMHz = InputClock;
	hI2C->IT = I2C_IT_ERR;
	hI2C->ITState = ENABLE;
	hI2C->Cmd = ENABLE;
	I2C_BSP_Init(hI2C);
}

void Screen_Config(Screen_InitTypeDef *hScreen)
{
	hScreen->GpioRest.GPIOx = GPIOC;
	hScreen->GpioRest.Pin = GPIO_PIN_1;
	hScreen->GpioRest.Mode = GPIO_MODE_OUT_PP_HIGH_FAST;

	hScreen->GpioEnable.GPIOx = GPIOC;
	hScreen->GpioEnable.Pin = GPIO_PIN_2;
	hScreen->GpioEnable.Mode = GPIO_MODE_OUT_PP_HIGH_FAST;

	Screen_Init(hScreen);
}

void Encoder_Config(Encoder_HandlerTypeDef *hEncoder)
{
	hEncoder->GpioA.GPIOx = GPIOC;
	hEncoder->GpioA.Pin = GPIO_PIN_5;
	hEncoder->GpioA.Mode = GPIO_MODE_IN_FL_NO_IT;

	hEncoder->GpioB.GPIOx = GPIOC;
	hEncoder->GpioB.Pin = GPIO_PIN_6;
	hEncoder->GpioB.Mode = GPIO_MODE_IN_FL_NO_IT;
	//hEncoder->NewAction = FALSE;
	hEncoder->LeftTurn = FALSE;
	hEncoder->RightTurn = FALSE;
	//hEncoder->A_ChangeCnt = 0;
	//hEncoder->A_UnChangeCnt = 0;
	//hEncoder->ChangeCnt = 0;
	//hEncoder->UnchangeCnt = 0;
	
	Encoder_Init(hEncoder);
}

void Keys_Config(Keys_TypeDef *hKeys)
{
	GPIO_Pin_TypeDef GenoralKeysPin[GENERAL_SWITCH_KEY_CNT] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3};
	uint8_t GenoralKeyIndex = 0;
	for (GenoralKeyIndex = 0; GenoralKeyIndex < GENERAL_SWITCH_KEY_CNT; GenoralKeyIndex++)
	{
		hKeys->GeneralSwit[GenoralKeyIndex].GpioKey.GPIOx = GPIOB;
		hKeys->GeneralSwit[GenoralKeyIndex].GpioKey.Pin = GenoralKeysPin[GenoralKeyIndex];
		hKeys->GeneralSwit[GenoralKeyIndex].GpioKey.Mode = GPIO_MODE_IN_FL_NO_IT;
		hKeys->GeneralSwit[GenoralKeyIndex].LongPress = FALSE;
		hKeys->GeneralSwit[GenoralKeyIndex].ShortPress = FALSE;
		hKeys->GeneralSwit[GenoralKeyIndex].PressCnt = 0;
		Key_Init(&hKeys->GeneralSwit[GenoralKeyIndex]);
	}
	hKeys->EnckSwit.GpioKey.GPIOx = GPIOC;
	hKeys->EnckSwit.GpioKey.Pin = GPIO_PIN_7;
	hKeys->EnckSwit.GpioKey.Mode = GPIO_MODE_IN_FL_NO_IT;
	hKeys->EnckSwit.LongPress = FALSE;
	hKeys->EnckSwit.ShortPress = FALSE;
	hKeys->EnckSwit.PressCnt = 0;
	Key_Init(&hKeys->EnckSwit);

	hKeys->PowerSwit.GpioKey.GPIOx = GPIOF;
	hKeys->PowerSwit.GpioKey.Pin = GPIO_PIN_4;
	hKeys->PowerSwit.GpioKey.Mode = GPIO_MODE_IN_FL_NO_IT;
	hKeys->PowerSwit.LongPress = FALSE;
	hKeys->PowerSwit.ShortPress = FALSE;
	hKeys->PowerSwit.PressCnt = 0;
	Key_Init(&hKeys->PowerSwit);
}

void main(void)
{
	uint32_t RefreshInfoTimeCnt = 0;

	static BEEP_InitTypeDef hBeep;
	static TIM3_InitTypeDef hTIM3;
	static I2C_InitTypeDef  hI2C;
	static Screen_InitTypeDef hScreen;

	/** 配置时钟 并等待切换成功*/
	CLK_Config(&hCLK);
	enableInterrupts();
	while(CLK_CheckSwitchSuccessed(&hCLK) != TRUE) {}
	disableInterrupts();

	/** 配置电源指示灯**/
	Power_Config(&hPower);
	Power_SetLedStatus(&hPower);

	/** 配置蜂鸣器 **/
	BEEP_Config(&hBeep);

	/** 配置tim3定时器**/
	TIM3_Config(&hTIM3);

	/** 配置I2C 并启动中断**/
	I2C_Config(&hI2C);
	enableInterrupts();

	/** 配置OLED显示器 完成后显示logo**/
	Screen_Config(&hScreen);
	Display_ShowTendZoneLogo();

	//读取全局参数表 初始化机器：  初始化控制板, 音频模式芯片，电子音量芯片
	DeviceParameter_InitAndConfig();

	/** 初始化菜单 并显示主屏幕界面*/
	Interface_InitAndDisplay();
	
	hPower.IsOn = TRUE;
	Power_SetLedStatus(&hPower);
	BEEP_BSP_SetOnAwhile(700);

	/** 初始化按键-------普通按键(上电且在主界面有效)  编码器按键(上电时有效)  电源按键(一直有效)**/
	/** 初始化编码器---(上电期间有效)**/
	Encoder_Config(&hEncoder);
	Keys_Config(&hKeys);

	/** 初始化iwdg  暂时不用  可用于重启或者复位*/
	IWDG_Config(&hIWDG);
	hIWDG.Inited = TRUE;
        
        int *mallocInt = (int *)malloc(sizeof(int) * 10);
        
        
	while (1)
	{
		Key_Scan(&hKeys.PowerSwit);
		PowerSwit_PressorPressAction(&hKeys.PowerSwit);

		if (g_dev.IsPowerOn != FALSE)
		{
			if (g_dev.IsHomePage != FALSE)
			{
				Key_GeneralSwit_Scan(&hKeys);
				GeneralSwit_PressorPressAction(&hKeys);
			}

			Key_Scan(&hKeys.EnckSwit);
			EnckSwit_PressorPressAction(&hKeys.EnckSwit);
			
			//Encoder_Scan(&hEncoder);   用中断定时扫描
			Encoder_PressorTurnAction(&hEncoder);

			if ((g_time_cnt - RefreshInfoTimeCnt) > 2000) 
			{
				updata_info();
				RefreshInfoTimeCnt = g_time_cnt;
			}
		}
	}
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  	BEEP_BSP_SetOn();
  }
}
#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
