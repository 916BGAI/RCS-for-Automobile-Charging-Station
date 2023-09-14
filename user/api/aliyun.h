#ifndef _ALIYUN_H
#define _ALIYUN_H

#include <stm32f10x.h>

//三元组
#define Ali_ProductKey 		"k07j9fnIL7j"
#define Ali_DeviceName 		"chongdianzhuang"
#define Ali_DeviceSecret 	"d8acfed60565c4cfbf52643f6003c08a"

#define firmware_max 1024

#define ALIBUF_SIZE 128
typedef struct{
	char ClientID[ALIBUF_SIZE];
	uint16_t ClientID_len;
	char UserName[ALIBUF_SIZE];
	uint16_t UserName_len;
	char PassWord[ALIBUF_SIZE];
	uint16_t PassWord_len;
	char ServerIP[ALIBUF_SIZE];
	uint16_t ServerPort;
	
	char topic_post[ALIBUF_SIZE];//发布Topic
	char topic_post_reply[ALIBUF_SIZE];//订阅回传响应
	char topic_set[ALIBUF_SIZE];//订阅设置信息
}_AliIOT_Connect;

extern _AliIOT_Connect Ali_Buf;

typedef struct
{
	uint8_t LEDSwitch;
	uint16_t Tem;
	uint16_t Hum;
	uint16_t light;
	u8 rgb_r;
	u8 rgb_g;
	u8 rgb_b;
}_AliData;
extern _AliData Ali_data;

typedef enum{Aliyun_NOWifi=0,Aliyun_Connect_Error,Aliyun_MQTT_OK}Aliyun_Status_E;
extern Aliyun_Status_E ali_status;
void AliIOT_DataConfig(void);
uint8_t AliIOT_ConnetServer(void);
uint8_t MyMQTT_Connect(void);
uint8_t MyMQTT_DisConnect(void);
uint8_t MyMQTT_Publish(char *topic,char *payload);
uint8_t MyMQTT_Subscribe(char *topic);
uint8_t MyMQTT_UnSubscribe(char *topic);
void MQTT_Test(void);

void MQTT_Getdatas_Deal(void);
void MQTT_Publishdata_Deal(void);
uint8_t MQTT_PublishAck_Deal(void);

void Up_Data(void);

#endif
