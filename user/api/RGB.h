#ifndef _RGB_H_
#define _RGB_H_

#include <stdint.h>
#include <stm32f10x.h>

void RGB_Config(void);
void RGB_ls(void);
void RGB_Breath(void);

#define RGB_R_ON GPIO_WriteBit(GPIOA,GPIO_Pin_8,Bit_RESET);
#define RGB_R_OFF GPIO_WriteBit(GPIOA,GPIO_Pin_8,Bit_SET);
#define RGB_G_ON GPIO_WriteBit(GPIOA,GPIO_Pin_7,Bit_RESET);
#define RGB_G_OFF GPIO_WriteBit(GPIOA,GPIO_Pin_7,Bit_SET);
#define RGB_B_ON GPIO_WriteBit(GPIOA,GPIO_Pin_6,Bit_RESET);
#define RGB_B_OFF GPIO_WriteBit(GPIOA,GPIO_Pin_6,Bit_SET);

#endif
