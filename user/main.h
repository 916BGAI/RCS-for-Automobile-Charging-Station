#ifndef _MAIN_H_
#define _MAIN_H_

#include "stm32f10x.h"

//关闭JTAG功能，打开SWD功能
__STATIC_INLINE void JTAG_SWD_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
}

#endif
