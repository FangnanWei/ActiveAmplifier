/*****************************************
                                ad.c
                                zhangtaolei     2016-1-27
******************************************/

/*****************************************
                                include
******************************************/
#include "stm8s_conf.h"
#include "ad.h"
/*****************************************
                                macro
******************************************/

/*****************************************
                                typedef
******************************************/

/*****************************************
                                variables
******************************************/

/*****************************************
                                function
******************************************/
void AD_Init()
{
    GPIO_Init(AD_Port, AD_A_Pin | AD_B_Pin, GPIO_MODE_IN_FL_NO_IT);
    ADC1_Init(ADC1_CONVERSIONMODE_CONTINUOUS, ADC1_CHANNEL_0, ADC1_PRESSEL_FCPU_D8, 
              ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL0,
              DISABLE);
    ADC1_Init(ADC1_CONVERSIONMODE_CONTINUOUS, ADC1_CHANNEL_1, ADC1_PRESSEL_FCPU_D8, 
              ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL1,
              DISABLE);
}

unsigned short GetADC_Value(unsigned char channelId)
{
    unsigned short value = 0;
    unsigned char i = 0;
    
    if (channelId == ADC_CHANNEL_A)
    {
        ADC1_ConversionConfig(ADC1_CONVERSIONMODE_CONTINUOUS, ADC1_CHANNEL_0, ADC1_ALIGN_RIGHT);
        ADC1_StartConversion();
        for (i = 0; i < ADC_SAMPLE_CNT; i++)
        {
            while(0 == ADC1_GetITStatus(ADC1_IT_EOC));
            value += ADC1_GetConversionValue();
            ADC1_ClearITPendingBit(ADC1_IT_EOC);
        }
        return value/ADC_SAMPLE_CNT;
    }
    else if (channelId == ADC_CHANNEL_B)
    {
        ADC1_ConversionConfig(ADC1_CONVERSIONMODE_CONTINUOUS, ADC1_CHANNEL_1, ADC1_ALIGN_RIGHT);
        ADC1_StartConversion();
        for (i = 0; i < ADC_SAMPLE_CNT; i++)
        {
            while(0 == ADC1_GetITStatus(ADC1_IT_EOC));
            value += ADC1_GetConversionValue();
            ADC1_ClearITPendingBit(ADC1_IT_EOC);
        }
        return value/ADC_SAMPLE_CNT;
    }
    return 0;
}

