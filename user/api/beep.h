#ifndef _BEEP_H_
#define _BEEP_H_

#include <stm32f10x.h>

void Beep_Config(void);

#define Beep_ON GPIO_WriteBit(GPIOC,GPIO_Pin_0,Bit_SET)
#define Beep_OFF GPIO_WriteBit(GPIOC,GPIO_Pin_0,Bit_RESET)

#endif
