#ifndef _APP_GPIO_H_
#define _APP_GPIO_H_
#include "stm8s.h"

#define GENERAL_SWITCH_KEY_CNT   4

typedef enum {
    TURN_LEFT,
    TURN_RIGHT,
    TURN_NONE
} encoder_dir_TypeDef;


typedef struct {
	GPIO_InitTypeDef GpioOn;
	GPIO_InitTypeDef GpioOff;
	//���ǰѵ�Դ�����������
	bool IsOn;
}Power_HandlerTypeDef;

typedef struct {
	GPIO_InitTypeDef GpioKey;
	bool ShortPress;
	bool LongPress;
	uint16_t PressCnt;
}Key_HandlerTypeDef;

typedef struct {
	Key_HandlerTypeDef GeneralSwit[GENERAL_SWITCH_KEY_CNT];
	Key_HandlerTypeDef EnckSwit;
	Key_HandlerTypeDef PowerSwit;
}Keys_TypeDef;

typedef struct {
	GPIO_InitTypeDef GpioA;
	GPIO_InitTypeDef GpioB;

	//����λ״̬
	bool APinLevel;
	bool BPinLevel;

	//�ȶ�ʱ���״̬
	bool PinsStable;
	bool StableLevel;
	int16_t PinsStableCnt;

	//��¼���յķ���
	bool LeftTurn; 						//����ת��
	bool RightTurn;						//����ת��



	int16_t A_ChangeCnt;
	int16_t A_UnChangeCnt;
	bool NewAction;		//�жϼ�⵽�������仯
	uint16_t ChangeCnt;
	uint16_t UnchangeCnt;
}Encoder_HandlerTypeDef;

void Encoder_Init(Encoder_HandlerTypeDef *hEncoder);
void Encoder_Scan(Encoder_HandlerTypeDef *hEncoder);

void Power_Init(Power_HandlerTypeDef *hPower);
void Power_SetLedStatus(Power_HandlerTypeDef *hPower);

void Key_Init(Key_HandlerTypeDef *hKey);
void Key_Scan(Key_HandlerTypeDef *hKey);
void Key_GeneralSwit_Scan(Keys_TypeDef *hKeys);

#endif

