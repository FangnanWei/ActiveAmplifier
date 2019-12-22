/******************************************
                            timer.c
                            zhangtaolei         2016-01-29
*******************************************/

/******************************************
                            include
*******************************************/
#include "timer.h"

/******************************************
                            macro
*******************************************/

/******************************************
                            typedef
*******************************************/

/******************************************
                            variables
*******************************************/

/******************************************
                            functions
*******************************************/
void InitTimer()
{
    /* 12.288MHz */
    TIM1_TimeBaseInit(16, TIM1_COUNTERMODE_UP, 768, 0);
    TIM1_ARRPreloadConfig(ENABLE);
    TIM1_ITConfig(TIM1_IT_UPDATE, ENABLE);
    TIM1_Cmd(ENABLE);

#if 0
	/* 12.288MHz/192KHz */
	TIM2_TimeBaseInit(TIM2_PRESCALER_1, 63);
	TIM2_OC1Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, 32, TIM2_OCPOLARITY_HIGH);
	TIM2_OC1PreloadConfig(ENABLE);
	TIM2_ARRPreloadConfig(ENABLE);
  	TIM2_Cmd(ENABLE);
#endif
}

