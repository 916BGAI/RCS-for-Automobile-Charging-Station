#ifndef _ESP8266_H_
#define _ESP8266_H_
#define MAX 2000
#include "stm32f10x.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void Usart3_Config(void);

typedef struct{
	char recv_data[MAX];//接受数据RX
	u16 recv_cnt;//接受数据计数RX
	u8 recv_flag;//接受完成标志RX
}WIFI_DATA;
extern WIFI_DATA wifi_data;
void ESP8266_Config(void);
u8 ESP8266_SendCmd(char *cmd,char *ack,u16 timeout);
char * FindStr(char *scr,char *dest,u16 timeout);
void Usart3_SendStr(char *p);
void Usart3_SendByte(u8 data);
void Wifi_Init(void);
void WIFI_CloseTransmission(void);
uint8_t WIFI_ConnectServer(char* mode, char* ip, uint16_t port);

void WIFI_ClearDat(void);
void WIFI_SendBuff(uint8_t *buff, uint16_t len);

uint8_t WIFI_DisconnectServer(void);

void Charger_Anlyze(void);



#endif


