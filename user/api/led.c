#include "led.h"
#include "delay.h"

_Bool led1_status = false;

// 初始化LED
// LED1\2\3\4,PE2、3、4、5
void LED_Config(void)
{
	// 开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	// 初始化GPIO结构体
	GPIO_InitTypeDef led = {
		.GPIO_Mode = GPIO_Mode_Out_PP,
		.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5,
		.GPIO_Speed = GPIO_Speed_50MHz
	};
	// 初始化GPIO
	GPIO_Init(GPIOE, &led);
	// 给LED初始电平
	LED1_OFF;
	LED2_OFF;
	LED3_OFF;
	LED4_OFF;
}

void LED_ls(void)
{
	static uint16_t state = 0;
	switch (state) {
		case 0: LED1_ON; LED2_OFF; LED3_OFF; LED4_OFF; break;
		case 1: LED1_OFF; LED2_ON; LED3_OFF; LED4_OFF; break;
		case 2: LED1_OFF; LED2_OFF; LED3_ON; LED4_OFF; break;
		case 3: LED1_OFF; LED2_OFF; LED3_OFF; LED4_ON; break;
		default: break;
	}
	state ++;
	state %= 4;
}

void LED_Breath(void)
{
	uint32_t i = 0;
	for(i=0; i<2000; i+=5)
	{
		LED1_ON;
		Delay_usnop(i);
		LED1_OFF;
		Delay_usnop(2000-i);
	}
	for(i=0; i<2000; i+=5)
	{
		LED1_ON;
		Delay_usnop(2000-i);
		LED1_OFF;
		Delay_usnop(i);
	}
}
