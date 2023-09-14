#include "usart1.h"
#include "RGB.h"
#include "beep.h"

// ����1��ʼ��
// TX->PA9,RX->PA10
void USART1_Config(void)
{
	// ��ʱ��, GPIOA, USART1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
	// ��ʼ��GPIO�Ľṹ��
	GPIO_InitTypeDef USART1_GPIO = {
		.GPIO_Mode = GPIO_Mode_AF_PP,
		.GPIO_Pin = GPIO_Pin_9,
		.GPIO_Speed = GPIO_Speed_50MHz
	};
	// ��ʼ��GPIOA
	GPIO_Init(GPIOA, &USART1_GPIO);
	USART1_GPIO.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	USART1_GPIO.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &USART1_GPIO);

	// ��ʼ��USART1
	USART_InitTypeDef USART1_InitTypeDef = {
		.USART_BaudRate = 115200,
		.USART_HardwareFlowControl = USART_HardwareFlowControl_None,
		.USART_Mode = USART_Mode_Rx | USART_Mode_Tx,
		.USART_Parity = USART_Parity_No,
		.USART_StopBits = USART_StopBits_1,
		.USART_WordLength = USART_WordLength_8b
	};
	// ��ʼ��USART1
	USART_Init(USART1, &USART1_InitTypeDef);
	// ʹ�ܴ���
	USART_Cmd(USART1, ENABLE);
	// �ж�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  // �򿪽����ж�

	// �ж�
	NVIC_SetPriority(USART1_IRQn, 0); // �ж����ȼ���������
	NVIC_EnableIRQ(USART1_IRQn);    // �ж�ʹ��
}

void USART1_IRQHandler(void)
{
	uint16_t data = 0;   // ������յ�������
	while (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) {
		// ��������
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

// printf�ض���
int fputc(int ch, FILE *f)
{
	// ��������
	USART_SendData(USART1, (uint16_t)ch);
	// �ж���һ�������Ƿ������
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	return ch;
}
