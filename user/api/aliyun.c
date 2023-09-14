#include "aliyun.h"
#include "esp8266.h"
#include "utils_hmac.h"
#include "mqttpacket.h"
#include "cJSON.h"
#include "dht11.h"
#include "ADC.h"
#include "app.h"
#include "mfrc522.h"
#include "delay.h"

_AliIOT_Connect Ali_Buf = {"\0"};
_AliData Ali_data={0};
Aliyun_Status_E ali_status = Aliyun_NOWifi;
/*******************************************************************************
�������ƣ�AliIOT_DataConfig
�������ã����������ݳ�ʼ��
������ڣ���
�������ڣ���
�������ߣ�XYD_WYC
����ʱ�䣺2021.01.16
�޸�ʱ�䣺2021.01.16
*******************************************************************************/
void AliIOT_DataConfig(void)
{
	memset(&Ali_Buf,0,sizeof(_AliIOT_Connect));  
	
	//��ȡClientID
	sprintf(Ali_Buf.ClientID,"%s|securemode=3,signmethod=hmacsha1|",Ali_DeviceName);
	Ali_Buf.ClientID_len = strlen(Ali_Buf.ClientID);
	//��ȡUserName
	sprintf(Ali_Buf.UserName,"%s&%s",Ali_DeviceName,Ali_ProductKey);
	Ali_Buf.UserName_len = strlen(Ali_Buf.UserName);
	//��ȡPassWord
	char Temp_buf[128] = "\0";
	sprintf(Temp_buf,"clientId%sdeviceName%sproductKey%s",Ali_DeviceName,Ali_DeviceName,Ali_ProductKey); 
	utils_hmac_sha1(Temp_buf,strlen(Temp_buf),Ali_Buf.PassWord,Ali_DeviceSecret,strlen(Ali_DeviceSecret));
	Ali_Buf.PassWord_len = strlen(Ali_Buf.PassWord); 
	//��ȡ������IP��ַ���˿ں�
	
											//iot-06z00h4rqmeh75z.mqtt.iothub.aliyuncs.com
	sprintf(Ali_Buf.ServerIP,"%s.iot-as-mqtt.cn-shanghai.aliyuncs.com",Ali_ProductKey);                  //��������������
	Ali_Buf.ServerPort = 1883;
	
	//Topic�б�
	sprintf(Ali_Buf.topic_post,"/sys/%s/%s/thing/event/property/post",Ali_ProductKey,Ali_DeviceName);                 
	sprintf(Ali_Buf.topic_post_reply,"/sys/%s/%s/thing/event/property/post_reply",Ali_ProductKey,Ali_DeviceName);  
	sprintf(Ali_Buf.topic_set,"/sys/%s/%s/thing/service/property/set",Ali_ProductKey,Ali_DeviceName);  
	
	//�������������Ϣ	
	printf("�� �� ����%s:%d\r\n",Ali_Buf.ServerIP,Ali_Buf.ServerPort); 
	printf("�ͻ���ID��%s\r\n",Ali_Buf.ClientID);               
	printf("�� �� ����%s\r\n",Ali_Buf.UserName);               
	printf("��    �룺%s\r\n",Ali_Buf.PassWord);               
}

/*******************************************************************************
�������ƣ�AliIOT_ConnetServer
�������ã����������ӷ�����
������ڣ���
�������ڣ���
�������ߣ�XYD_LSH
����ʱ�䣺2021.06.07
�޸�ʱ�䣺2021.06.07
*******************************************************************************/
uint8_t AliIOT_ConnetServer(void)
{
	uint8_t Timers = 2;
	AliIOT_DataConfig();
	
	WIFI_CloseTransmission();                   //����������˳�͸��
	Delay_ms(2000);
	
	//���ӷ�����
//	u8 flag = 0;
//	
//	
//	char init_esp_buf[128] = {0};
//	
//	sprintf((char *)init_esp_buf, "AT+CIPSTART=\"%s\",\"%s\",%d\r\n", "TCP", Ali_Buf.ServerIP, Ali_Buf.ServerPort);
	while(Timers--)
	{       
		if(WIFI_ConnectServer("TCP",Ali_Buf.ServerIP,Ali_Buf.ServerPort) == 0)
		{
			return 1;
		}
	}
	return 1;
}

/*******************************************************************************
�������ƣ�AliIOT_MQTTAckCheck
�������ã����MQTTЭ��ش�����
������ڣ�
	MQTTtype:���ش���MQTTЭ������
	timeout:��ʱʱ��
�������ڣ������
	0-OK 
	1-��ʱ
	2-�ش�����
�������ߣ�XYD_LSH
����ʱ�䣺2021.06.08
�޸�ʱ�䣺2021.06.08
*******************************************************************************/
uint8_t AliIOT_MQTTAckCheck(uint8_t MQTTtype,uint16_t timeout)
{
	uint8_t res=0;
	while(timeout--)//�ȴ����ڽ��յ�Ӧ���ź�
	{
		Delay_ms(1);
		if(wifi_data.recv_flag == 1) 
		{
			if(wifi_data.recv_data[0] == MQTTtype)
			{
				break;
			}
			else
				wifi_data.recv_flag=0;
		}
	}
	
	if(timeout+1==0)
		return 1;//Ӧ��ʱ
	else
	{
		//��һ���ж���Ӧ����
		switch(MQTTtype)
		{
			case 0x20://��ӡ���ԣ�20 02 00 00
				if((wifi_data.recv_data[2]==0)&&(wifi_data.recv_data[3]==0))
					res = 0;
				else
					res = 2;
				break;
			case 0x90://��ӡ���ԣ�90 03 00 0A 01     ��������4������ΪQOS����
				if((wifi_data.recv_data[4]==0)||(wifi_data.recv_data[4]==1)||(wifi_data.recv_data[4]==2))
					res = 0;
				else
					res = 2;
				break;
			case 0xB0://��ӡ���ԣ�B0 02 00 00
				res = 0;//����Ӧ����
				break;
			case 0x30:
				res = 0;//MQTT_PublishAck_Deal();//Ĭ�ϻش�OK��Ҳ��ʹ�ú��������»ش�������
				break;
			defalut:break;
		}
	}
	
	wifi_data.recv_flag = 0;//���������
	
	//UDebug_SendBuff(USART5Buff,USART5Count);//��ӡ����
	wifi_data.recv_cnt = 0;
	return res;
}

/*******************************************************************************
�������ƣ�MyMQTT_Connect
�������ã�MQTT����
������ڣ���
�������ڣ�
	0-OK 
	1-��ʱ
	2-�ش�����
�������ߣ�XYD_LSH
����ʱ�䣺2021.06.08
�޸�ʱ�䣺2021.06.08
*******************************************************************************/
uint8_t MyMQTT_Connect(void)
{
	uint8_t WIFI_txBuff[firmware_max]={0};
	uint32_t len = 0;
	//1.����MQTT CONNECT �ṹ��
	MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
	//2.�ṹ�帳ֵ
	data.cleansession = 1;//����Ự
	data.clientID.cstring = Ali_Buf.ClientID;//��ʼ���豸ID
	data.keepAliveInterval = 60;//���ֽ���ʱ��60s
	data.MQTTVersion = 4;//ʹ��MQTT 3.1.1
	
	data.username.cstring = Ali_Buf.UserName;//�û���
	data.password.cstring = Ali_Buf.PassWord;//�û�����
	//3.����MQTTSerialize_connect�ѽṹ���е����ݹ��ɶ�������
	len = MQTTSerialize_connect(WIFI_txBuff,firmware_max,&data);
	if(len == 0) {
		printf("��������ʧ��\r\n");
		return 1;//���ɶ�����������ʧ��
	}
	else {
		for(uint16_t i=0; i<len; i++) {
			printf("%02X ", WIFI_txBuff[i]);
		}
		printf("\r\n");
	}
	//4.ͨ��WIFI���ͳ�ȥ
	WIFI_ClearDat();
	WIFI_SendBuff(WIFI_txBuff, len);
	
	return 0;//��ӡ���ԣ�20 02 00 00
}

/*******************************************************************************
�������ƣ�MyMQTT_DisConnect
�������ã�MQTT�Ͽ�����
������ڣ���
�������ڣ�
	0-OK 
	1-��ʱ
	2-�ش�����
�������ߣ�XYD_LSH
����ʱ�䣺2021.06.08
�޸�ʱ�䣺2021.06.08
*******************************************************************************/
uint8_t MyMQTT_DisConnect(void)
{
	uint8_t WIFI_txBuff[firmware_max]={0};
	uint32_t len = 0;
	//1.��DISCONNECT���Ľṹ�壬��Ϊ���������ݴ��뷢�ͻ�������
	len = MQTTSerialize_disconnect(WIFI_txBuff,firmware_max);
	if(len == 0)
		return 1;
	//2.���õײ����緢������
	WIFI_SendBuff(WIFI_txBuff,len);
	return 0;
}

/*******************************************************************************
�������ƣ�MyMQTT_Publish
�������ã�MQTT��������
������ڣ�
	*topic��������Topic�б�
	*payload���������������ݣ�ָ����Ч���ش洢��
�������ڣ�
	0-OK 
	1-��ʱ
	2-�ش�����
�������ߣ�XYD_LSH
����ʱ�䣺2021.06.08
�޸�ʱ�䣺2021.06.08
*******************************************************************************/
uint8_t MyMQTT_Publish(char *topic,char *payload)
{
	uint8_t WIFI_txBuff[firmware_max]={0};
	uint32_t payload_len = 0;
	unsigned char dup = 0;
	int qos = 0;
	unsigned char retained = 0;
	unsigned short msgid = 0;
	int len = 0;
	//1.��ʼ����ز���
	MQTTString topicString = MQTTString_initializer;
	topicString.cstring = topic;//��������
	payload_len = strlen(payload);//��Ч���صĳ���
	//2.����Publish�Ķ�����������
	len = MQTTSerialize_publish(WIFI_txBuff,firmware_max,dup, qos, retained, msgid,\
												topicString,(uint8_t *)payload,payload_len);
	if(len <= 0)
		return 1;
	//3.WIFI���ͳ�ȥ
	WIFI_SendBuff(WIFI_txBuff,len);	

	return 0;//��ӡ���ԣ��ش������Ƿ�ɹ�����Ӧ
}

/*******************************************************************************
�������ƣ�MyMQTT_Subscribe
�������ã�MQTT��������
������ڣ�
	*topic�����ĵ�Topic�б�
�������ڣ�
	0-OK 
	1-��ʱ
	2-�ش�����
�������ߣ�XYD_LSH
����ʱ�䣺2021.06.08
�޸�ʱ�䣺2021.06.08
*******************************************************************************/
uint8_t MyMQTT_Subscribe(char *topic)
{
	#define TOPIC_COUNT 1
	unsigned char WIFI_txBuff[100];
	int buflen = sizeof(WIFI_txBuff);
	unsigned char dup = 0;
	unsigned short msgid = 10;
	int count = TOPIC_COUNT;
	int req_qoss[TOPIC_COUNT] = {2};
	int len = 0;

	MQTTString topicStrings[TOPIC_COUNT] = { MQTTString_initializer };
	topicStrings[0].cstring = topic;
	len = MQTTSerialize_subscribe(WIFI_txBuff, buflen, dup, msgid, count, topicStrings, req_qoss);
	
	if(len <= 0)
		return 1;
	//3.WIFI���ͳ�ȥ
	WIFI_SendBuff(WIFI_txBuff,len);
	
	return AliIOT_MQTTAckCheck(0x90,1000);//��ӡ���ԣ�90 03 00 0A 01
}

/*******************************************************************************
�������ƣ�MyMQTT_UnSubscribe
�������ã�MQTTȡ����������
������ڣ�
	*topic�����ĵ�Topic�б�
�������ڣ�
	0-OK 
	1-��ʱ
	2-�ش�����
�������ߣ�XYD_LSH
����ʱ�䣺2021.06.08
�޸�ʱ�䣺2021.06.08
*********************************************************************************/
uint8_t MyMQTT_UnSubscribe(char *topic)
{
	unsigned char WIFI_txBuff[100];
	int buflen = sizeof(WIFI_txBuff);
	#define TOPIC_COUNT 1

	unsigned char dup = 0;
	unsigned short msgid = 0;
	int count = TOPIC_COUNT;
	int len = 0;

	MQTTString topicStrings[TOPIC_COUNT] = { MQTTString_initializer };

	
	topicStrings[0].cstring = topic;
	len = MQTTSerialize_unsubscribe(WIFI_txBuff, buflen, dup, msgid, count, topicStrings);
	
	if(len <= 0)
		return 1;
	//3.WIFI���ͳ�ȥ
	WIFI_SendBuff(WIFI_txBuff,len);
	
	return AliIOT_MQTTAckCheck(0xB0,1000);//��ӡ���ԣ�B0 02 00 00
}

/*******************************************************************************
�������ƣ�MQTT_GETJson_Parse
�������ã�����MQTTЭ���е�JSON����
������ڣ�
	*str:�������ַ���
  *dststr:Ŀ���ַ���
�������ڣ�
	0-OK
	1-root_error
	2-result_error
�������ߣ�XYD_LSH
����ʱ�䣺2021.06.08
�޸�ʱ�䣺2021.06.08
*********************************************************************************/
uint8_t MQTT_GETJson_Parse(uint8_t *str,char *dststr)
{
	cJSON *root = NULL,*json_results = NULL, *value = NULL;
	//JSON�������ص�����
	root = cJSON_Parse((char *)str);
	if(!root)
	{
		printf("root Error before: %s\n", cJSON_GetErrorPtr());
		return 1;
	}
	else
	{
		json_results = cJSON_GetObjectItem(root,"params");//��ȡ��������
		if(!json_results)
		{
			printf("results Error before: %s\n", cJSON_GetErrorPtr());
			return 2;
		}
		else
		{
			value = cJSON_GetObjectItem(json_results,dststr);
			
			if(!value)
			{
				printf("location Error before: %s\n", cJSON_GetErrorPtr());
			}
			else
				printf("LEDSwitch=%d\r\n",value->valueint);
		}
	}
	cJSON_Delete(root);
	return 0;
}

/*******************************************************************************
�������ƣ�MQTT_Getdata_Deal
�������ã������ƶ����ݽ���
������ڣ���
�������ڣ���
�������ߣ�XYD_LSH
����ʱ�䣺2021.06.08
�޸�ʱ�䣺2021.06.08
*********************************************************************************/
void MQTT_Getdatas_Deal(void)
{
	if(wifi_data.recv_flag)
	{
		uint32_t OverLen = 0;
		uint16_t BuffCount = 0;
		uint16_t TopicLen = 0;
		//uint8_t topic_str[100]={0};
		//uint8_t message_str[512]={0};
		//printf("%s\r\n",USART5Buff);
		//UDebug_SendBuff(USART5Buff,USART5Count);
		/*
		�̶���ͷ��30 99 01  ʣ�೤��=153
		TopicLEN�� 00 33 
		2F 73 79 73 2F 61 31 62 61 77 70 69 7A 69 6F 68 2F 49 4F 54 5F 32 30 36 2F 74 68 69 6E 67 2F 73 65 72 76 69 63 65 2F 70 72 6F 70 65 72 74 79 2F 73 65 74 
		7B 22 6D 65 74 68 6F 64 22 3A 22 74 68 69 6E 67 2E 73 65 72 76 69 63 65 2E 70 72 6F 70 65 72 74 79 2E 73 65 74 22 2C 22 69 64 22 3A 22 31 31 39 31 36 37 34 30 39 33 22 2C 22 70 61 72 61 6D 73 22 3A 7B 22 4C 45 44 53 77 69 74 63 68 22 3A 31 7D 2C 22 76 65 72 73 69 6F 6E 22 3A 22 31 2E 30 2E 30 22 7D 
		*/
		
		if(wifi_data.recv_data[BuffCount++] == 0x30)
		{
			if(wifi_data.recv_data[BuffCount] &0x80)
			{
				wifi_data.recv_data[BuffCount] &=~(0x01<<7);
				OverLen = wifi_data.recv_data[BuffCount+1]*128 +wifi_data.recv_data[BuffCount];
				BuffCount += 2;
			}
			else
			{
				OverLen = wifi_data.recv_data[BuffCount];
				BuffCount++;
			}
			//printf("OverLen = %d\r\n",OverLen);
			
			TopicLen = (wifi_data.recv_data[BuffCount]<<8)|wifi_data.recv_data[BuffCount+1];
			BuffCount += 2;
			//printf("TopicLen = %d\r\n",TopicLen);
			
			//memcpy(topic_str,&USART5Buff[BuffCount],TopicLen);
			BuffCount+=TopicLen;
			//printf("��ǰTopic=%s\r\n",topic_str);
			
			//memcpy(message_str,&USART5Buff[BuffCount],OverLen - TopicLen);
			//printf("��ǰ��Ϣ=%s\r\n",message_str);
			
			
			/*
			{
			"method":"thing.service.property.set",
			"id":"1367672506",
			"params":{
					"LEDSwitch":0
			},
			"version":"1.0.0"
		}
			*/
			//printf("�����ַ���=%s\r\n",&USART5Buff[BuffCount]);
			MQTT_GETJson_Parse((uint8_t *)&wifi_data.recv_data[BuffCount],"LEDSwitch");

		}
		WIFI_ClearDat();
		wifi_data.recv_flag=0;
	}
}

/*******************************************************************************
�������ƣ�MQTT_PublishAck_Deal
�������ã�������Ӧ����
������ڣ���
�������ڣ�
    0-OK 
		>0---Error
�������ߣ�XYD_LSH
����ʱ�䣺2021.06.08
�޸�ʱ�䣺2021.06.08
*********************************************************************************/
uint8_t MQTT_PublishAck_Deal(void)
{
	uint8_t result = 0x02;
	if(wifi_data.recv_flag)
	{
		uint32_t OverLen = 0;
		uint16_t BuffCount = 0;
		uint16_t TopicLen = 0;
		//uint8_t topic_str[100]={0};
		//uint8_t message_str[512]={0};
		//printf("%s\r\n",USART5Buff);
		//UDebug_SendBuff(USART5Buff,USART5Count);
		/*
		�̶���ͷ��30 99 01  ʣ�೤��=153
		TopicLEN�� 00 33 
		2F 73 79 73 2F 61 31 62 61 77 70 69 7A 69 6F 68 2F 49 4F 54 5F 32 30 36 2F 74 68 69 6E 67 2F 73 65 72 76 69 63 65 2F 70 72 6F 70 65 72 74 79 2F 73 65 74 
		7B 22 6D 65 74 68 6F 64 22 3A 22 74 68 69 6E 67 2E 73 65 72 76 69 63 65 2E 70 72 6F 70 65 72 74 79 2E 73 65 74 22 2C 22 69 64 22 3A 22 31 31 39 31 36 37 34 30 39 33 22 2C 22 70 61 72 61 6D 73 22 3A 7B 22 4C 45 44 53 77 69 74 63 68 22 3A 31 7D 2C 22 76 65 72 73 69 6F 6E 22 3A 22 31 2E 30 2E 30 22 7D 
		*/
		
		if(wifi_data.recv_data[BuffCount++] == 0x30)
		{
			if(wifi_data.recv_data[BuffCount] &0x80)
			{
				wifi_data.recv_data[BuffCount] &=~(0x01<<7);
				OverLen = wifi_data.recv_data[BuffCount+1]*128 +wifi_data.recv_data[BuffCount];
				BuffCount += 2;
			}
			else
			{
				OverLen = wifi_data.recv_data[BuffCount];
				BuffCount++;
			}
			//printf("OverLen = %d\r\n",OverLen);
			
			TopicLen = (wifi_data.recv_data[BuffCount]<<8)|wifi_data.recv_data[BuffCount+1];
			BuffCount += 2;
			//printf("TopicLen = %d\r\n",TopicLen);
			
			//memcpy(topic_str,&USART5Buff[BuffCount],TopicLen);
			BuffCount+=TopicLen;
			//printf("��ǰTopic=%s\r\n",topic_str);
			
			//memcpy(message_str,&USART5Buff[BuffCount],OverLen - TopicLen);
			//printf("��ǰ��Ϣ=%s\r\n",message_str);
			
			/*
			{
					"code":200,
					"data":{

					},
					"id":"230788029",
					"message":"success",
					"method":"thing.event.property.post",
					"version":"1.0"
			}
			*/
			cJSON *root = NULL,*json_results = NULL;
			//JSON�������ص�����
			root = cJSON_Parse((char *)&wifi_data.recv_data[BuffCount]);
			
			printf("�����ַ���=%s\r\n",&wifi_data.recv_data[BuffCount]);
			if(!root)
			{
				printf("root Error before: %s\n", cJSON_GetErrorPtr());
			}
			else
			{
				json_results = cJSON_GetObjectItem(root,"message");//��ȡ��������
				if(!json_results)
				{
					printf("results Error before: %s\n", cJSON_GetErrorPtr());
				}
				else
				{
					printf("��Ӧ=%s\r\n",json_results->valuestring);
					result=0;
				}
			}
			cJSON_Delete(root);
		}
		WIFI_ClearDat();
		wifi_data.recv_flag=0;
	}
	return result;
}

/*******************************************************************************
�������ƣ�MQTT_Test
�������ã�MQTT���ԣ����Ӱ�����ƽ̨����ƽ̨��������
������ڣ���
�������ڣ���
�������ߣ�XYD_LSH
����ʱ�䣺2021.06.08
�޸�ʱ�䣺2021.06.08
*********************************************************************************/
void MQTT_Test(void)
{
	
	//1.WIFI���ӵ�aliyun
	if(AliIOT_ConnetServer())
	{
		char Send_JsonBuf[256] = "\0";
		
		sprintf((char *)Send_JsonBuf,
					"{\"method\":\"thing.service.property.set\",\
						\"id\":\"230788029\",\
						\"params\":{\
						\"humi\":%d,\
						\"temp\":%d,\
						}\
						,\"version\":\"1.0\"}",
						Ali_data.LEDSwitch,Ali_data.Hum);
		
		//2.����MQTT����
		if(MyMQTT_Connect()!=0)
			printf("����MQTTʧ��\r\n");
		else
		{
			//3.��smarthome/204/light���ⷢ��2021��Ϣ
			if(MyMQTT_Publish(Ali_Buf.topic_post,Send_JsonBuf)!=0)
				printf("��������ʧ��\r\n");
			
			Delay_ms(5000);
			//4.�Ͽ�MQTT����
			if(MyMQTT_DisConnect()!=0)
				printf("�Ͽ�MQTTʧ��\r\n");
		}
		
		//5.�Ͽ�����������
		WIFI_DisconnectServer();
	}
}

void Up_Data(void)
{
	char Send_JsonBuf[256] = "\0";
	char buf[10];
	sprintf((char *)Send_JsonBuf,
					"{\"method\":\"thing.service.property.set\",\
						\"id\":\"230788029\",\
						\"params\":{\
						\"temperature\":%d,\
						\"Humidity\":%d,\
						\"LightLux\":%d,\
						\"ChargingState\":%d,\
						\"ChargingTime\":%d,\
						\"CardMoney\":%d,\
						}\
						,\"version\":\"1.0\"}",
						dht11_data.TEMP,dht11_data.HUM,adc_data.light,device1.state,device1.time,card_money);
	MyMQTT_Publish(Ali_Buf.topic_post,Send_JsonBuf);
	//printf("�����ϱ����\r\n");
}
