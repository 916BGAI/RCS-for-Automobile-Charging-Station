#include <stdio.h>
#include <stdint.h>
#include "main.h"
#include "delay.h"
#include "led.h"
#include "beep.h"
#include "RGB.h"
#include "Key.h"
#include "mfrc522.h"
#include "usart1.h"
#include "lcd.h"
#include "dht11.h"
#include "ADC.h"
#include "esp8266.h"
#include "aliyun.h"
#include "mfrc522.h"
#include "app.h"

int main()
{
	JTAG_SWD_Config();	// 关闭JTAG功能 打开SWD功能
	Delay_Config();		// 延时初始化
	LED_Config();		// 初始化LED
	Beep_Config();		// 初始化蜂鸣器
	RGB_Config();		// 初始化RGB
	Key_Config();		// 初始化按键
	USART1_Config();	// 初始化串口1
	LCD_Init();			// 初始化屏幕
	LCD_Clear(WHITE);
	init_status = 1;
	PCD_Init();			// 初始化RC552
	ADC_Config();		// 初始化ADC
	ESP8266_Config();	// 初始化wifi
	Delay_ms(1000);
	Wifi_Init();		// 连接wifi
	Delay_ms(1000);
	AliIOT_ConnetServer();	// 连接服务器
	Delay_ms(1000);
	MyMQTT_Connect();		// 连接MQTT
	init_status = 0;
	LCD_Clear(WHITE);
	MFRC522_Add_IC();	// 获取卡号，并且充值500

	// LCD_Show_Photo(0, 0, 226, 320, (uint8_t *)gImage_test);

	LED1_OFF;
	LED2_OFF;
	LED3_OFF;
	LED4_OFF;
	Beep_OFF;

	KEY_VALUE key_value = KEY_PRES_NULL;
	uint16_t rgb_value = 1;
	char lcd_show_buf[2][29] = {0};
	char showbuff [3] = {0};

start_view:
	LCD_Clear(WHITE);
	All_VlaueReset();		// 数据复位
	Charge_View();			// 选择充电方式界面
	Charge_Choose_View(choose_no);

	while(1)
	{
		if(dht11time > 1000) {
			DHT11_ReadData();	// 获取温湿度
			ADC_GetValue();		// 获取光照强度
			printf("湿度：%d\t温度：%d\t光照：%d\r\n", dht11_data.HUM, dht11_data.TEMP, adc_data.light);
			Up_Data();
			sprintf(lcd_show_buf[0], "光照: %d", adc_data.light);
			sprintf(lcd_show_buf[1], "湿度: %d 温度: %d", dht11_data.HUM, dht11_data.TEMP);
			// Draw_Text_8_16_Str(0, 0, WHITE, BLACK, lcd_show_buf[0]);
			// Draw_Text_8_16_Str(0, 16, WHITE, BLACK, lcd_show_buf[1]);
			dht11time = 0;
		}

		if(keytime > 10) {
			key_value = Key_GetValue();
			if(key_value != KEY_PRES_NULL) {
				switch (key_value) {
					case KEY1_PRES_SHORT:
						Func_Add(choose_mode); break;
					case KEY2_PRES_SHORT:
						Func_Sub(choose_mode); break;
					case KEY3_PRES_SHORT:
						if (choose_mode > 0) choose_mode--; break;	// 返回
					case KEY4_PRES_SHORT:
						if (choose_mode < 3) choose_mode++; break;	// 确认
					default:
						break;
				}
				if (choose_mode <= 1) {		// 模式1和0
					Charge_Choose_View(choose_no);		// 充电方式的选择
					Charge_Choose_Num(choose_no);		// 数字的变化
				} else {
					if (Func_Charging(choose_no) == 0) {
						LED4_ON;							// 开启充电
					} else {
						LED4_OFF;							// 关闭充电
						Delay_msnop(300);
						goto start_view;
					}
				}
			}
			keytime = 0;
		}

		if (cardtime > 1000) {
			if(choose_mode > 1) {
				if(led4_status == 0) {			// 判断正在充电
					device1.state = 1;				// 状态为1
					device1.time ++;				// 已经充的时间
					device1.money += car_power*2;	// 已经花费的钱
					device1.power += car_power;		// 已经充的电量

					// 显示充电消耗信息
					sprintf(showbuff,":%d", device1.time);
					Draw_Text_8_16_Str(70, 240, WHITE, RED, showbuff);
					sprintf(showbuff,":%d", device1.money);
					Draw_Text_8_16_Str(70, 270, WHITE, RED, showbuff);
					sprintf(showbuff,":%d", device1.power);
					Draw_Text_8_16_Str(70, 300, WHITE, RED, showbuff);

					if ((total_money - device1.money) == 0) {
						Draw_Text_8_16_Str(24, 118, WHITE, RED, "充电成功,请刷卡消费......");
						MFRC522_Add_IC1();	// 扣费
						card_money = card_money - total_money;
						LED4_OFF;
						goto start_view;
					}
				} else {
					device1.state = 0;
					device1.time = 0;		// 时间++
					device1.money = 0;		// 汽车消费的电能
					device1.power = 0;		// 消耗的电能
					LED4_OFF;				// 充电结束
					goto start_view;
				}
			}
			cardtime = 0;
		}

		if(ledtime > 1000) {
			if(rgb_value)
				RGB_ls();
			ledtime = 0;
		}
	}
}
