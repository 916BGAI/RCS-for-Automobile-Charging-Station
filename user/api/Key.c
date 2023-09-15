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

KEY_STATE KEY_Scan(uint16_t mode)
{
	static uint16_t key_up = 1; // �������ɿ���־
	if (mode)
		key_up = 1; // ֧������
	if (key_up && (Key1 == 1 || Key2 == 0 || Key3 == 0 || Key4 == 0))
	{
		Delay_ms(10); // ȥ����
		key_up = 0;
		if (Key1 == 1)
			return KEY1_PRES;
		else if (Key2 == 0)
			return KEY2_PRES;
		else if (Key3 == 0)
			return KEY3_PRES;
		else if (Key4 == 0)
			return KEY4_PRES;
	}
	else if (Key1 == 0 && Key2 == 1 && Key3 == 1 && Key4 == 1)
		key_up = 1;
	return 0; // �ް�������
}

KEY_VALUE Key_GetValue(void)
{
	KEY_STATE key_state = KEY_NULL;
	KEY_VALUE key_value = KEY_PRES_NULL;
	key_state = KEY_Scan(1);
	// ����һ�εļ�ֵ�Ƚ� �������ȣ������м�ֵ�ı仯����ʼ��ʱ
	if (key_state != 0 && num_on == 0)
	{
		key_old = key_state;
		num_on = 1;
	}
	if (key_state != 0 && num_on >= 1 && num_on <= Key_Scan_Time) // 25*10ms
	{
		num_on++; // ʱ���¼��
	}
	if (key_state == 0 && num_on > 0 && num_on < Key_Scan_Time) // �̰�
	{
		switch (key_old)
		{
		case KEY1_PRES:
			key_value = KEY1_PRES_SHORT;
			break;
		case KEY2_PRES:
			key_value = KEY2_PRES_SHORT;
			break;
		case KEY3_PRES:
			key_value = KEY3_PRES_SHORT;
			break;
		case KEY4_PRES:
			key_value = KEY4_PRES_SHORT;
			break;
		default:
			break;
		}
		num_on = 0;
	}
	else if (key_state == 0 && num_on >= Key_Scan_Time) // ����
	{
		switch (key_old)
		{
		case KEY1_PRES:
			key_value = KEY1_PRES_LONG;
			break;
		case KEY2_PRES:
			key_value = KEY2_PRES_LONG;
			break;
		case KEY3_PRES:
			key_value = KEY3_PRES_LONG;
			break;
		case KEY4_PRES:
			key_value = KEY4_PRES_LONG;
			break;
		default:
			break;
		}
		num_on = 0;
	}

	return key_value;
}
