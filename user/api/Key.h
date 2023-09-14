#ifndef _KEY_H_
#define _KEY_H_

#include <stm32f10x.h>

#define Key1 GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)
#define Key2 GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4)
#define Key3 GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5)
#define Key4 GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6)

void Key_Config(void);
uint16_t Key_GetValue(void);

#endif // _KEY_H_
