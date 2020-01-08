/*****************************************
                                mute.h
                                zhangtaolei     2016-1-27
******************************************/
#ifndef _MUTE_H_
#define _MUTE_H_

/*****************************************
                                include
******************************************/
#include "stm8s_conf.h"

/*****************************************
                                macro
******************************************/
#define MUTE_A_Port     GPIOA
#define MUTE_B_Port     GPIOF

#define MUTE_A_Pin      GPIO_PIN_2
#define MUTE_B_Pin      GPIO_PIN_4

#define MuteALow()		GPIO_WriteLow(MUTE_A_Port, MUTE_A_Pin);
#define MuteAHigh()   	GPIO_WriteHigh(MUTE_A_Port, MUTE_A_Pin);

#define MuteBLow()		GPIO_WriteLow(MUTE_B_Port, MUTE_B_Pin);
#define MuteBHigh()   	GPIO_WriteHigh(MUTE_B_Port, MUTE_B_Pin);


/*****************************************
                                typedef
******************************************/

/*****************************************
                                variables
******************************************/

/*****************************************
                                function
******************************************/
void MuteInit(void);

#endif
