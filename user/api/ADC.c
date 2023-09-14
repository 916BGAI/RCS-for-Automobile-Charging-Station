#include "ADC.h"

typedef_ADCDATA adc_data;

// 初始化ADC, PA5,ADC1
void ADC_Config(void)
{
	// 开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);
	// 初始化GPIO结构体
	GPIO_InitTypeDef adc_gpio = {
		.GPIO_Pin = GPIO_Pin_5,
		.GPIO_Mode = GPIO_Mode_AIN,	// 模拟输入
		.GPIO_Speed = GPIO_Speed_50MHz
	};
	GPIO_Init(GPIOA, &adc_gpio);

	// ADC1分频，6分频，72/6=12
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);

	// 初始化ADC结构体
	ADC_InitTypeDef adc = {
		.ADC_ContinuousConvMode = DISABLE,	// 单次转换
		.ADC_DataAlign = ADC_DataAlign_Right,	// 数据右对齐
		.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None,	// 外部触发转换关闭
		.ADC_Mode = ADC_Mode_Independent,	// 独立模式
		.ADC_NbrOfChannel = 1,	// 转换通道数
		.ADC_ScanConvMode = DISABLE	// 扫描模式关闭
	};
	ADC_Init(ADC1, &adc);	// 初始化ADC1

	// 使能ADC1
	ADC_Cmd(ADC1, ENABLE);
	// 设置通道顺序，采样时间
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_239Cycles5);
	// 复位校准
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	// 开启校准
	ADC_StartCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
}

// 获取ADC转换的结果
uint16_t ADC_GetValue(void)
{
	// 开启转换
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	// 判断转换是否完成
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	// 获取转换结果
	adc_data.light = ADC_GetConversionValue(ADC1);
	return 0;
}
