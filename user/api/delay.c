#include "delay.h"
#include "lcd.h"

uint16_t init_status = 0;

//滴答定时器初始化
void Delay_Config(void)
{
	if(SysTick_Config(72000) == 1)//配置滴答定时器为1ms延时
	{
		while(1);
	}
}

u32 runtime = 0;
u32 ledtime = 0;
u32 keytime = 0;
u32 dht11time = 0;
u32 cardtime = 0;
u32 inittime = 0;

//定时1ms发生的中断
void SysTick_Handler(void)
{
	runtime++;
	ledtime++;
	keytime++;
	dht11time++;
	cardtime++;
	inittime++;

	if(inittime > 500) {
		switch (init_status) {
			case 0:
				break;
			case 1:
				Draw_Text_8_16_Str(24, 118, WHITE, LIGHTBLUE, "正在初始化,请等待.");
				init_status++ ;
				break;
			case 2:
				Draw_Text_8_16_Str(24, 118, WHITE, LIGHTBLUE, "正在初始化,请等待..");
				init_status++ ;
				break;
			case 3:
				Draw_Text_8_16_Str(24, 118, WHITE, LIGHTBLUE, "正在初始化,请等待...");
				init_status++ ;
				break;
			case 4:
				Draw_Text_8_16_Str(24, 118, WHITE, LIGHTBLUE, "正在初始化,请等待....");
				init_status++ ;
				break;
			case 5:
				Draw_Text_8_16_Str(24, 118, WHITE, LIGHTBLUE, "正在初始化,请等待.....");
				init_status++ ;
				break;
			case 6:
				Draw_Text_8_16_Str(24, 118, WHITE, LIGHTBLUE, "正在初始化,请等待......");
				Draw_Text_8_16_Str(24, 118, WHITE, LIGHTBLUE, "正在初始化,请等待      ");
				init_status = 1;
			break;
			default: break;
		}
			inittime = 0;
	}

}

void Delay_ms(u32 time)
{
	u32 nowtime = runtime;
	while(runtime < (nowtime+time));
}

void Delay_usnop(u32 time)
{
	while(time--)
	{
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();		
	}
}
void Delay_msnop(u32 time)
{
	while(time--)
	{
		Delay_usnop(1000);
	}
}
