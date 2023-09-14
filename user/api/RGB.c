#include "RGB.h"
#include "delay.h"

void RGB_Config(void)
{
	// 开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	// 初始化结构体
	GPIO_InitTypeDef RGB = {
		.GPIO_Mode = GPIO_Mode_Out_PP,
		.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8,
		.GPIO_Speed = GPIO_Speed_50MHz
	};
	// 初始化GPIO
	GPIO_Init(GPIOA, &RGB);
	// 初始化状态
	RGB_R_OFF;
	RGB_G_OFF;
	RGB_B_OFF;
}

void RGB_ls(void)
{
	static uint16_t state = 0;
	switch (state) {
		case 0: RGB_R_ON; RGB_G_OFF; RGB_B_OFF; break;
		case 1: RGB_R_OFF; RGB_G_ON; RGB_B_OFF; break;
		case 2: RGB_R_OFF; RGB_G_OFF; RGB_B_ON; break;
		default: break;
	}
	state ++;
	state %= 3;
}

void RGB_Breath(void)
{
	uint32_t i = 0;
	for(i=0; i<2000; i+=5)
	{
		RGB_R_ON;
		Delay_usnop(i);
		RGB_R_OFF;
		Delay_usnop(2000-i);
	}
	for(i=0; i<2000; i+=5)
	{
		RGB_R_ON;
		Delay_usnop(2000-i);
		RGB_R_OFF;
		Delay_usnop(i);
	}
	for(i=0; i<2000; i+=5)
	{
		RGB_G_ON;
		Delay_usnop(i);
		RGB_G_OFF;
		Delay_usnop(2000-i);
	}
	for(i=0; i<2000; i+=5)
	{
		RGB_G_ON;
		Delay_usnop(2000-i);
		RGB_G_OFF;
		Delay_usnop(i);
	}
	for(i=0; i<2000; i+=5)
	{
		RGB_B_ON;
		Delay_usnop(i);
		RGB_B_OFF;
		Delay_usnop(2000-i);
	}
	for(i=0; i<2000; i+=5)
	{
		RGB_B_ON;
		Delay_usnop(2000-i);
		RGB_B_OFF;
		Delay_usnop(i);
	}
}
