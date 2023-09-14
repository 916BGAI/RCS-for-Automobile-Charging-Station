#ifndef _LED_H_
#define _LED_H_

#include <stdint.h>
#include <stdbool.h>
#include <stm32f10x.h>

extern _Bool led1_status;

void LED_Config(void);
void LED_ls(void);
void LED_Breath(void);

#define LED1_ON GPIO_WriteBit(GPIOE,GPIO_Pin_2,Bit_RESET);led1_status=true
#define LED1_OFF GPIO_WriteBit(GPIOE,GPIO_Pin_2,Bit_SET);led1_status=false
#define LED2_ON GPIO_WriteBit(GPIOE,GPIO_Pin_3,Bit_RESET)
#define LED2_OFF GPIO_WriteBit(GPIOE,GPIO_Pin_3,Bit_SET)
#define LED3_ON GPIO_WriteBit(GPIOE,GPIO_Pin_4,Bit_RESET)
#define LED3_OFF GPIO_WriteBit(GPIOE,GPIO_Pin_4,Bit_SET)
#define LED4_ON GPIO_WriteBit(GPIOE,GPIO_Pin_5,Bit_RESET);
#define LED4_OFF GPIO_WriteBit(GPIOE,GPIO_Pin_5,Bit_SET);

#endif
