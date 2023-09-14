#include "beep.h"

void Beep_Config(void)
{
	// ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	// ��ʼ��GPIO�ṹ��
	GPIO_InitTypeDef beep = {
		.GPIO_Mode = GPIO_Mode_Out_PP,
		.GPIO_Pin = GPIO_Pin_0,
		.GPIO_Speed = GPIO_Speed_50MHz
	};
	// ��ʼ��GPIO
	GPIO_Init(GPIOC, &beep);
	// ��Beep��ʼ��ƽ
	Beep_OFF;
}
