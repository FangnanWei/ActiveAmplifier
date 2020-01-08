#include "Adc.h"
#include "Scene.h"

void FloatTo5p23(float param_dec, uint8_t *param_hex)
{
	long param223 = (long)(param_dec * (1 << 23));//multiply decimal number by 2^23
	//	param_dec:-16 ~ +16

	//	param223 :-2^27 ~ +2^27
	long param227 = param223 + (1 << 27);

	param_hex[3] = (uint8_t)param227;
	param_hex[2] = (uint8_t)(param227 >> 8);
	param_hex[1] = (uint8_t)(param227 >> 16);
	param_hex[0] = (uint8_t)(param227 >> 24);

	param_hex[0] = (uint8_t)(param_hex[0] ^ 0x08);

	if ((param_hex[0] & 0x08) > 0)
	{
		param_hex[0] = (uint8_t)(param_hex[0] | 0xf0);
	}
}

void UintTo5p23(uint32_t param_dec, uint8_t *param_hex)
{

	long param223 = (long)(param_dec << 23);
	
	param_hex[3] = (uint8_t)param223;
	param_hex[2] = (uint8_t)(param223 >> 8);
	param_hex[1] = (uint8_t)(param223 >> 16);
	param_hex[0] = (uint8_t)(param223 >> 24);
}


/*
4������˥����VR11=P.9=ADCIN11=0.23V~2.5V 
	0.23V ~ 1.5V��ӦDSP�㷨��ֵ1.00 ~ 0.15��//127y + 85x = 146.55    (xΪ��ѹ�� yΪ����)
	1.5V ~ 2.3V��ӦDSP�㷨��ֵ0.15 ~ 0.01		 //80y  + 14x = 33
	2.3V ~ 2.5VΪMUTE��
	��������0.2�룬ͬ���ϱ���UI

	//��ʽ
	2^12/3.3 = x/y

	//ADC����ֵ�Ͷ�Ӧ��ѹֵ
	2.5----2.3-------1.5--------------0.23
	3103--2855-------1861-------------285
*/

void Adc_Deal(AP_Inst_t *inst)
{
	struct Adc *adc = inst->drivers.adc;
	static uint16_t oldValue = 0;
	uint16_t currValue = adc->GetValue(adc);
	struct Scene *scene = inst->scene;
	MsgDspExt_t *pMsgDspExt = NULL;
	uint8_t param_hex[4];


	//���˶���
	if(currValue == oldValue)
		return;

	if (currValue > oldValue)
	{
		if ((oldValue + 2) > currValue)
			return;
	}
	else
	{
		if ((currValue + 2) > oldValue)
			return;
	}

	oldValue = 	currValue;
	pMsgDspExt = (MsgDspExt_t*)malloc(sizeof(MsgDspExt_t));

	//MUTE
	if(currValue >= 2855)	
	{
		//������
		uint32_t mute = 1;
		UintTo5p23(mute, param_hex);
		pMsgDspExt->len = 4;

		pMsgDspExt->dspParam[0] = param_hex[0];
		pMsgDspExt->dspParam[1] = param_hex[1];
		pMsgDspExt->dspParam[2] = param_hex[2];
		pMsgDspExt->dspParam[3] = param_hex[3];

		//ͨ��1
		pMsgDspExt->address = AUDIO_INPUT1_MUTE_Addr;
		scene->LoadMsgDspExt(scene, pMsgDspExt, True);
		//SendPacket((uint8_t*)pMsgDspExt, sizeof(MsgDspExt_t), DI_Ok, MSG_DSP_AUDIO_INPUT_Change, 0);
		
		//ͨ��2
		pMsgDspExt->address = AUDIO_INPUT2_MUTE_Addr;
		scene->LoadMsgDspExt(scene, pMsgDspExt, True);
		//SendPacket((uint8_t*)pMsgDspExt, sizeof(MsgDspExt_t), DI_Ok, MSG_DSP_AUDIO_INPUT_Change, 0);
	}
	//GAIN
	else 
	{
		//�ؾ��� ������
		float gain = 0;
		uint32_t mute = 0;

		UintTo5p23(mute, param_hex);
		pMsgDspExt->len = 4;

		pMsgDspExt->dspParam[0] = param_hex[0];
		pMsgDspExt->dspParam[1] = param_hex[1];
		pMsgDspExt->dspParam[2] = param_hex[2];
		pMsgDspExt->dspParam[3] = param_hex[3];

		//ͨ��1
		pMsgDspExt->address = AUDIO_INPUT1_MUTE_Addr;
		scene->LoadMsgDspExt(scene, pMsgDspExt, True);
		//SendPacket((uint8_t*)pMsgDspExt, sizeof(MsgDspExt_t), DI_Ok, MSG_DSP_AUDIO_INPUT_Change, 0);
		
		//ͨ��2
		pMsgDspExt->address = AUDIO_INPUT2_MUTE_Addr;
		scene->LoadMsgDspExt(scene, pMsgDspExt, True);
		//SendPacket((uint8_t*)pMsgDspExt, sizeof(MsgDspExt_t), DI_Ok, MSG_DSP_AUDIO_INPUT_Change, 0);
		
		//���������ֵ
		if (currValue >= 1861) 
		{
			//80y  + 14x = 33
			gain = (33.0 - 14.0*((currValue*3.3)/4096))/80.0;
		}
		else	
		{
			//127y + 85x = 146.55
			gain = (146.55 - 85.0*((currValue*3.3)/4096))/127.0;
		}

		FloatTo5p23(gain, param_hex);
#if 1		
		//pMsgDspExt->type = AUDIO_INPUT_AlgType; //û���ã��ɲ���
		pMsgDspExt->len = 4;
		
		pMsgDspExt->dspParam[0] = param_hex[0];
		pMsgDspExt->dspParam[1] = param_hex[1];
		pMsgDspExt->dspParam[2] = param_hex[2];
		pMsgDspExt->dspParam[3] = param_hex[3];
		
		//ͨ��1
		pMsgDspExt->address = AUDIO_INPUT1_GAIN_Addr;
		scene->LoadMsgDspExt(scene, pMsgDspExt, True);
		//SendPacket((uint8_t*)pMsgDspExt, sizeof(MsgDspExt_t), DI_Ok, MSG_DSP_AUDIO_INPUT_Change, 0);
		
		//ͨ��2
		pMsgDspExt->address = AUDIO_INPUT2_GAIN_Addr;
		scene->LoadMsgDspExt(scene, pMsgDspExt, True);
		//SendPacket((uint8_t*)pMsgDspExt, sizeof(MsgDspExt_t), DI_Ok, MSG_DSP_AUDIO_INPUT_Change, 0);
#endif		
	}

	if(pMsgDspExt)
		free(pMsgDspExt);
}


uint16_t Adc_GetValue(struct Adc *self)
{
	return self->pp.advalue;
}

struct Adc *Adc_Open(void)
{
	struct Adc *adc = (struct Adc *)malloc(sizeof(struct Adc));
	DMA_InitTypeDef *DMA_InitStruct = &adc->pp.DMA_InitStruct;
	ADC_InitTypeDef *ADC_InitStruct = &adc->pp.ADC_InitStruct;
	
	adc->pp.gpio = Gpio_Open(GPIOC, GPIO_Pin_1, GPIO_Speed_50MHz, GPIO_Mode_AIN, ENABLE);
	adc->GetValue = Adc_GetValue;

//////////////////////////////////////////////
	/* DMA1 channel1 configuration ----------------------------------------------*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStruct->DMA_PeripheralBaseAddr = ((uint32_t)0x4001244C);			//DMA��Ӧ���������ַ
	DMA_InitStruct->DMA_MemoryBaseAddr = (uint32_t)&adc->pp.advalue;			//�ڴ�洢����ַ
	DMA_InitStruct->DMA_DIR = DMA_DIR_PeripheralSRC;							//DMA��ת��ģʽΪSRCģʽ����������Ƶ��ڴ�
	DMA_InitStruct->DMA_BufferSize = 1;											//DMA�����СCH_NUM��
	DMA_InitStruct->DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//����һ�����ݺ��豸��ַ��ֹ����
	DMA_InitStruct->DMA_MemoryInc = DMA_MemoryInc_Disable;						//����һ�����ݺ�Ŀ���ڴ��ַ��ֹ����
	DMA_InitStruct->DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//�����������ݿ��Ϊ16λ
	DMA_InitStruct->DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;			//DMA�������ݳߴ磬HalfWord����Ϊ16λ 
	DMA_InitStruct->DMA_Mode = DMA_Mode_Circular;								//ת��ģʽ��ѭ������ģʽ��
	DMA_InitStruct->DMA_Priority = DMA_Priority_High;							//DMA���ȼ���
	DMA_InitStruct->DMA_M2M = DMA_M2M_Disable;									//M2Mģʽ����
	DMA_Init(DMA1_Channel1, DMA_InitStruct);

	/* Enable DMA1 channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);

	/* ADC1 configuration ------------------------------------------------------*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);												//����adʱ��Ϊ72/6=12M,ad���ʱ��Ƶ��Ϊ14M
	
	ADC_InitStruct->ADC_Mode = ADC_Mode_Independent;								//�����ڶ���ģʽ
	ADC_InitStruct->ADC_ScanConvMode = ENABLE;										//����ת��ģʽ
	ADC_InitStruct->ADC_ContinuousConvMode = ENABLE;								//����ת��ģʽ
	ADC_InitStruct->ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;				//��ʹ���ⲿ����
	ADC_InitStruct->ADC_DataAlign = ADC_DataAlign_Right;							//�����Ҷ���
	ADC_InitStruct->ADC_NbrOfChannel = 1;											//ת��1��ͨ��
	ADC_Init(ADC1, ADC_InitStruct);

	/* ADC1 regular channel11 configuration */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_55Cycles5); 	//�趨ָ����ADCͨ����,����ת��˳��Ͳ���ʱ��

	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);										//����adc��DMA

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);											//ʹ��ADC1

	/* Enable ADC1 reset calibration register */   
	ADC_ResetCalibration(ADC1);										//����У׼
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));						//�ȴ�����У׼���

	/* Start ADC1 calibration */
	ADC_StartCalibration(ADC1);										//��ʼУ׼
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1));							//�ȴ�У׼���
	   
	/* Start ADC1 Software Conversion */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);							//����ת����ʼ��ADCͨ��DMA��ʽ���ϵĸ���RAM����

//////////////////////////////////////////////

	return adc;
}

