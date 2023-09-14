#include "dht11.h"
#include "delay.h"

typedef_DATA dht11_data;

// DHT11 ��ʼ��
void DHT11_Config(uint8_t mode)
{
	// ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
	// ��ʼ���ṹ��
	GPIO_InitTypeDef dht11 = {
		.GPIO_Pin = GPIO_Pin_11,
		.GPIO_Speed = GPIO_Speed_50MHz
	};
	// �������ģʽ
	if (mode == 1)
		dht11.GPIO_Mode = GPIO_Mode_Out_PP;
	else
		dht11.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOG, &dht11);
}

// ��ȡDHT11������
// ������ͨ��
uint16_t DHT11_ReadData(void)
{
	uint8_t data[5] = {0};
	// ���ģʽ��������ӻ�������ʼ�ź�
	DHT11_Config(1);
	// ��ʼ�ź�
	DHT11_H;
	DHT11_L;
	Delay_ms(20);
	DHT11_H;
	// ����ģʽ���ӻ�������������Ӧ�ź�
	DHT11_Config(0);
	// �����ȴ���Ӧ
	while(DHT11_ReadPin == 1);	// �ȴ��͵�ƽ����
	while(DHT11_ReadPin == 0);	// �ȴ��ߵ�ƽ����
	// ��������40bit���� ��λ�ȷ�
	for (uint8_t i=0; i<40; i++)
	{
		while(DHT11_ReadPin == 1);	// �ȴ��͵�ƽ����
		while(DHT11_ReadPin == 0);	// �ȴ��ߵ�ƽ����
		Delay_usnop(30);
		// �ж�������1����0�����յ������ݴ����data��
		if (DHT11_ReadPin == 1) {
			data[i/8] |= (1 << (7 - i%8));	// ����1�Ƶ���λ������λ����
		} else {
			data[i/8] &= ~(1 << (7 - i%8));	// ����0�Ƶ���λ������λ����
		}
	}
	// У��
	if (data[4] == data[0] + data[1] + data[2] + data[3]) {
		dht11_data.HUM = data[0];	// ʪ��
		dht11_data.TEMP = data[2];	// �¶�
		return 1;
	} else {
		return 0;
	}
}
