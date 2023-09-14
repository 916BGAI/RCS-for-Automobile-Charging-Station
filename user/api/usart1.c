#include "usart1.h"
#include "RGB.h"
#include "beep.h"

// 串口1初始化
// TX->PA9,RX->PA10
void USART1_Config(void)
{
	// 开时钟, GPIOA, USART1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
	// 初始化GPIO的结构体
	GPIO_InitTypeDef USART1_GPIO = {
		.GPIO_Mode = GPIO_Mode_AF_PP,
		.GPIO_Pin = GPIO_Pin_9,
		.GPIO_Speed = GPIO_Speed_50MHz
	};
	// 初始化GPIOA
	GPIO_Init(GPIOA, &USART1_GPIO);
	USART1_GPIO.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	USART1_GPIO.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &USART1_GPIO);

	// 初始化USART1
	USART_InitTypeDef USART1_InitTypeDef = {
		.USART_BaudRate = 115200,
		.USART_HardwareFlowControl = USART_HardwareFlowControl_None,
		.USART_Mode = USART_Mode_Rx | USART_Mode_Tx,
		.USART_Parity = USART_Parity_No,
		.USART_StopBits = USART_StopBits_1,
		.USART_WordLength = USART_WordLength_8b
	};
	// 初始化USART1
	USART_Init(USART1, &USART1_InitTypeDef);
	// 使能串口
	USART_Cmd(USART1, ENABLE);
	// 中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  // 打开接收中断

	// 中断
	NVIC_SetPriority(USART1_IRQn, 0); // 中断优先级分组设置
	NVIC_EnableIRQ(USART1_IRQn);    // 中断使能
}

void USART1_IRQHandler(void)
{
	uint16_t data = 0;   // 保存接收到的数据
	while (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) {
		// 接收数据
		data = USART_ReceiveData(USART1);
		switch (data) {
			case '1': Beep_ON; break;
			case '2': Beep_OFF; break;
			case '3': RGB_B_ON; break;
			case '4': RGB_B_OFF; break;
			default: break;
		}
	}
}

// printf重定向
int fputc(int ch, FILE *f)
{
	// 发送数据
	USART_SendData(USART1, (uint16_t)ch);
	// 判断上一次数据是否发送完成
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	return ch;
}
