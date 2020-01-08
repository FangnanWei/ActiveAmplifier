/*****************************************
                                ad.h
                                zhangtaolei     2016-1-27
******************************************/
#ifndef _AD_H_
#define _AD_H_

/*****************************************
                                include
******************************************/
#include "stm8s_conf.h"

/*****************************************
                                macro
******************************************/
#define AD_Port     GPIOB
#define AD_A_Pin    GPIO_PIN_0
#define AD_B_Pin    GPIO_PIN_1

#define ADC_CHANNEL_A       0
#define ADC_CHANNEL_B       1

#define ADC_SAMPLE_CNT      6

/*****************************************
                                typedef
******************************************/

/*****************************************
                                variables
******************************************/

/*****************************************
                                function
******************************************/
void AD_Init();
unsigned short GetADC_Value(unsigned char channelId);

#endif