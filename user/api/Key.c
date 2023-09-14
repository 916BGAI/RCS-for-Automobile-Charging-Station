#include "Key.h"
#include "delay.h"

// 按键初始化
void Key_Config(void)
{
	// 开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
	// 初始化GPIO结构体
	GPIO_InitTypeDef Key = {
		.GPIO_Mode = GPIO_Mode_IN_FLOATING,
		.GPIO_Pin = GPIO_Pin_0,
		.GPIO_Speed = GPIO_Speed_50MHz
	};
	// 初始化GPIO
	GPIO_Init(GPIOA, &Key);
	Key.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_Init(GPIOC, &Key);
}

// 按键检测
uint16_t Key_GetValue(void) {
	if (Key1 == 1) {
		// 消抖
		Delay_ms(20);
		if (Key1 == 1) {
			while (Key1 == 1);
			return 1;
		}
	}
	if (Key2 == 0) {
		Delay_ms(20);
		if (Key2 == 0) {
			while (Key2 == 0);
			return 2;
		}
	}
	if (Key3 == 0) {
		Delay_ms(20);
		if (Key3 == 0) {
			while (Key3 == 0);
			return 3;
		}
	}
	if (Key4 == 0) {
		Delay_ms(20);
		if (Key4 == 0) {
			while (Key4 == 0);
			return 4;
		}
	}

	return 0;
}
