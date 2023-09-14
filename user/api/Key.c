#include "Key.h"
#include "delay.h"

// ������ʼ��
void Key_Config(void)
{
	// ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
	// ��ʼ��GPIO�ṹ��
	GPIO_InitTypeDef Key = {
		.GPIO_Mode = GPIO_Mode_IN_FLOATING,
		.GPIO_Pin = GPIO_Pin_0,
		.GPIO_Speed = GPIO_Speed_50MHz
	};
	// ��ʼ��GPIO
	GPIO_Init(GPIOA, &Key);
	Key.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_Init(GPIOC, &Key);
}

// �������
uint16_t Key_GetValue(void) {
	if (Key1 == 1) {
		// ����
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
