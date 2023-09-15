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
	JTAG_SWD_Config();	// �ر�JTAG���� ��SWD����
	Delay_Config();		// ��ʱ��ʼ��
	LED_Config();		// ��ʼ��LED
	Beep_Config();		// ��ʼ��������
	RGB_Config();		// ��ʼ��RGB
	Key_Config();		// ��ʼ������
	USART1_Config();	// ��ʼ������1
	LCD_Init();			// ��ʼ����Ļ
	LCD_Clear(WHITE);
	init_status = 1;
	PCD_Init();			// ��ʼ��RC552
	ADC_Config();		// ��ʼ��ADC
	ESP8266_Config();	// ��ʼ��wifi
	Delay_ms(1000);
	Wifi_Init();		// ����wifi
	Delay_ms(1000);
	AliIOT_ConnetServer();	// ���ӷ�����
	Delay_ms(1000);
	MyMQTT_Connect();		// ����MQTT
	init_status = 0;
	LCD_Clear(WHITE);
	MFRC522_Add_IC();	// ��ȡ���ţ����ҳ�ֵ500

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
	All_VlaueReset();		// ���ݸ�λ
	Charge_View();			// ѡ���緽ʽ����
	Charge_Choose_View(choose_no);

	while(1)
	{
		if(dht11time > 1000) {
			DHT11_ReadData();	// ��ȡ��ʪ��
			ADC_GetValue();		// ��ȡ����ǿ��
			printf("ʪ�ȣ�%d\t�¶ȣ�%d\t���գ�%d\r\n", dht11_data.HUM, dht11_data.TEMP, adc_data.light);
			Up_Data();
			sprintf(lcd_show_buf[0], "����: %d", adc_data.light);
			sprintf(lcd_show_buf[1], "ʪ��: %d �¶�: %d", dht11_data.HUM, dht11_data.TEMP);
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
						if (choose_mode > 0) choose_mode--; break;	// ����
					case KEY4_PRES_SHORT:
						if (choose_mode < 3) choose_mode++; break;	// ȷ��
					default:
						break;
				}
				if (choose_mode <= 1) {		// ģʽ1��0
					Charge_Choose_View(choose_no);		// ��緽ʽ��ѡ��
					Charge_Choose_Num(choose_no);		// ���ֵı仯
				} else {
					if (Func_Charging(choose_no) == 0) {
						LED4_ON;							// �������
					} else {
						LED4_OFF;							// �رճ��
						Delay_msnop(300);
						goto start_view;
					}
				}
			}
			keytime = 0;
		}

		if (cardtime > 1000) {
			if(choose_mode > 1) {
				if(led4_status == 0) {			// �ж����ڳ��
					device1.state = 1;				// ״̬Ϊ1
					device1.time ++;				// �Ѿ����ʱ��
					device1.money += car_power*2;	// �Ѿ����ѵ�Ǯ
					device1.power += car_power;		// �Ѿ���ĵ���

					// ��ʾ���������Ϣ
					sprintf(showbuff,":%d", device1.time);
					Draw_Text_8_16_Str(70, 240, WHITE, RED, showbuff);
					sprintf(showbuff,":%d", device1.money);
					Draw_Text_8_16_Str(70, 270, WHITE, RED, showbuff);
					sprintf(showbuff,":%d", device1.power);
					Draw_Text_8_16_Str(70, 300, WHITE, RED, showbuff);

					if ((total_money - device1.money) == 0) {
						Draw_Text_8_16_Str(24, 118, WHITE, RED, "���ɹ�,��ˢ������......");
						MFRC522_Add_IC1();	// �۷�
						card_money = card_money - total_money;
						LED4_OFF;
						goto start_view;
					}
				} else {
					device1.state = 0;
					device1.time = 0;		// ʱ��++
					device1.money = 0;		// �������ѵĵ���
					device1.power = 0;		// ���ĵĵ���
					LED4_OFF;				// ������
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
