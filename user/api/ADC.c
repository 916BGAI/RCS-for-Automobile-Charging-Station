#include "ADC.h"

typedef_ADCDATA adc_data;

// ��ʼ��ADC, PA5,ADC1
void ADC_Config(void)
{
	// ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);
	// ��ʼ��GPIO�ṹ��
	GPIO_InitTypeDef adc_gpio = {
		.GPIO_Pin = GPIO_Pin_5,
		.GPIO_Mode = GPIO_Mode_AIN,	// ģ������
		.GPIO_Speed = GPIO_Speed_50MHz
	};
	GPIO_Init(GPIOA, &adc_gpio);

	// ADC1��Ƶ��6��Ƶ��72/6=12
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);

	// ��ʼ��ADC�ṹ��
	ADC_InitTypeDef adc = {
		.ADC_ContinuousConvMode = DISABLE,	// ����ת��
		.ADC_DataAlign = ADC_DataAlign_Right,	// �����Ҷ���
		.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None,	// �ⲿ����ת���ر�
		.ADC_Mode = ADC_Mode_Independent,	// ����ģʽ
		.ADC_NbrOfChannel = 1,	// ת��ͨ����
		.ADC_ScanConvMode = DISABLE	// ɨ��ģʽ�ر�
	};
	ADC_Init(ADC1, &adc);	// ��ʼ��ADC1

	// ʹ��ADC1
	ADC_Cmd(ADC1, ENABLE);
	// ����ͨ��˳�򣬲���ʱ��
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_239Cycles5);
	// ��λУ׼
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	// ����У׼
	ADC_StartCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
}

// ��ȡADCת���Ľ��
uint16_t ADC_GetValue(void)
{
	// ����ת��
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	// �ж�ת���Ƿ����
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	// ��ȡת�����
	adc_data.light = ADC_GetConversionValue(ADC1);
	return 0;
}
