#ifndef _DELAY_H_
#define _DELAY_H_

#include <stm32f10x.h>

extern u32 ledtime;
extern u32 keytime;
extern u32 dht11time;
extern u32 cardtime;
extern u32 inittime;
extern uint16_t init_status;

void Delay_Config(void);
void Delay_usnop(u32 time);
void Delay_msnop(u32 time);
void Delay_ms(u32 time);
#endif
