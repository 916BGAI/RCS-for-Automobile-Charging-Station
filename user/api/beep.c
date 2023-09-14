#include "beep.h"

void Beep_Config(void)
{
	// 开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	// 初始化GPIO结构体
	GPIO_InitTypeDef beep = {
		.GPIO_Mode = GPIO_Mode_Out_PP,
		.GPIO_Pin = GPIO_Pin_0,
		.GPIO_Speed = GPIO_Speed_50MHz
	};
	// 初始化GPIO
	GPIO_Init(GPIOC, &beep);
	// 给Beep初始电平
	Beep_OFF;
}
