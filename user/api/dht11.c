#include "dht11.h"
#include "delay.h"

typedef_DATA dht11_data;

// DHT11 初始化
void DHT11_Config(uint8_t mode)
{
	// 开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
	// 初始化结构体
	GPIO_InitTypeDef dht11 = {
		.GPIO_Pin = GPIO_Pin_11,
		.GPIO_Speed = GPIO_Speed_50MHz
	};
	// 配置输出模式
	if (mode == 1)
		dht11.GPIO_Mode = GPIO_Mode_Out_PP;
	else
		dht11.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOG, &dht11);
}

// 读取DHT11的数据
// 单总线通信
uint16_t DHT11_ReadData(void)
{
	uint8_t data[5] = {0};
	// 输出模式，主机向从机发送起始信号
	DHT11_Config(1);
	// 起始信号
	DHT11_H;
	DHT11_L;
	Delay_ms(20);
	DHT11_H;
	// 输入模式，从机向主机发送响应信号
	DHT11_Config(0);
	// 主机等待响应
	while(DHT11_ReadPin == 1);	// 等待低电平到来
	while(DHT11_ReadPin == 0);	// 等待高电平到来
	// 主机接收40bit数据 高位先发
	for (uint8_t i=0; i<40; i++)
	{
		while(DHT11_ReadPin == 1);	// 等待低电平到来
		while(DHT11_ReadPin == 0);	// 等待高电平到来
		Delay_usnop(30);
		// 判断数据是1还是0，接收到的数据存放在data内
		if (DHT11_ReadPin == 1) {
			data[i/8] |= (1 << (7 - i%8));	// 数据1移到高位，其他位不变
		} else {
			data[i/8] &= ~(1 << (7 - i%8));	// 数据0移到高位，其他位不变
		}
	}
	// 校验
	if (data[4] == data[0] + data[1] + data[2] + data[3]) {
		dht11_data.HUM = data[0];	// 湿度
		dht11_data.TEMP = data[2];	// 温度
		return 1;
	} else {
		return 0;
	}
}
