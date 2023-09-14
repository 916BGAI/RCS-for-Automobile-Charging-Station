#ifndef _USART1_H_
#define _USART1_H_

#include <stm32f10x.h>
#include <stdio.h>
#include <stdint.h>

void USART1_Config(void);
void USART_IRQHandler(void);
int fputc(int ch, FILE *f);

#endif
