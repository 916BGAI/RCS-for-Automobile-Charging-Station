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
函数名称：AliIOT_DataConfig
函数作用：阿里云数据初始化
函数入口：无
函数出口：无
函数作者：XYD_WYC
创建时间：2021.01.16
修改时间：2021.01.16
*******************************************************************************/
void AliIOT_DataConfig(void)
{
	memset(&Ali_Buf,0,sizeof(_AliIOT_Connect));  
	
	//获取ClientID
	sprintf(Ali_Buf.ClientID,"%s|securemode=3,signmethod=hmacsha1|",Ali_DeviceName);
	Ali_Buf.ClientID_len = strlen(Ali_Buf.ClientID);
	//获取UserName
	sprintf(Ali_Buf.UserName,"%s&%s",Ali_DeviceName,Ali_ProductKey);
	Ali_Buf.UserName_len = strlen(Ali_Buf.UserName);
	//获取PassWord
	char Temp_buf[128] = "\0";
	sprintf(Temp_buf,"clientId%sdeviceName%sproductKey%s",Ali_DeviceName,Ali_DeviceName,Ali_ProductKey); 
	utils_hmac_sha1(Temp_buf,strlen(Temp_buf),Ali_Buf.PassWord,Ali_DeviceSecret,strlen(Ali_DeviceSecret));
	Ali_Buf.PassWord_len = strlen(Ali_Buf.PassWord); 
	//获取服务器IP地址、端口号
	
											//iot-06z00h4rqmeh75z.mqtt.iothub.aliyuncs.com
	sprintf(Ali_Buf.ServerIP,"%s.iot-as-mqtt.cn-shanghai.aliyuncs.com",Ali_ProductKey);                  //构建服务器域名
	Ali_Buf.ServerPort = 1883;
	
	//Topic列表
	sprintf(Ali_Buf.topic_post,"/sys/%s/%s/thing/event/property/post",Ali_ProductKey,Ali_DeviceName);                 
	sprintf(Ali_Buf.topic_post_reply,"/sys/%s/%s/thing/event/property/post_reply",Ali_ProductKey,Ali_DeviceName);  
	sprintf(Ali_Buf.topic_set,"/sys/%s/%s/thing/service/property/set",Ali_ProductKey,Ali_DeviceName);  
	
	//串口输出调试信息	
	printf("服 务 器：%s:%d\r\n",Ali_Buf.ServerIP,Ali_Buf.ServerPort); 
	printf("客户端ID：%s\r\n",Ali_Buf.ClientID);               
	printf("用 户 名：%s\r\n",Ali_Buf.UserName);               
	printf("密    码：%s\r\n",Ali_Buf.PassWord);               
}

/*******************************************************************************
函数名称：AliIOT_ConnetServer
函数作用：阿里云链接服务器
函数入口：无
函数出口：无
函数作者：XYD_LSH
创建时间：2021.06.07
修改时间：2021.06.07
*******************************************************************************/
uint8_t AliIOT_ConnetServer(void)
{
	uint8_t Timers = 2;
	AliIOT_DataConfig();
	
	WIFI_CloseTransmission();                   //多次连接需退出透传
	Delay_ms(2000);
	
	//连接服务器
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
函数名称：AliIOT_MQTTAckCheck
函数作用：检测MQTT协议回传数据
函数入口：
	MQTTtype:检测回传的MQTT协议类型
	timeout:超时时间
函数出口：检测结果
	0-OK 
	1-超时
	2-回传错误
函数作者：XYD_LSH
创建时间：2021.06.08
修改时间：2021.06.08
*******************************************************************************/
uint8_t AliIOT_MQTTAckCheck(uint8_t MQTTtype,uint16_t timeout)
{
	uint8_t res=0;
	while(timeout--)//等待串口接收到应答信号
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
		return 1;//应答超时
	else
	{
		//进一步判断响应内容
		switch(MQTTtype)
		{
			case 0x20://打印测试：20 02 00 00
				if((wifi_data.recv_data[2]==0)&&(wifi_data.recv_data[3]==0))
					res = 0;
				else
					res = 2;
				break;
			case 0x90://打印测试：90 03 00 0A 01     描述：第4个数据为QOS质量
				if((wifi_data.recv_data[4]==0)||(wifi_data.recv_data[4]==1)||(wifi_data.recv_data[4]==2))
					res = 0;
				else
					res = 2;
				break;
			case 0xB0://打印测试：B0 02 00 00
				res = 0;//有响应即可
				break;
			case 0x30:
				res = 0;//MQTT_PublishAck_Deal();//默认回传OK，也可使用函数解析下回传的数据
				break;
			defalut:break;
		}
	}
	
	wifi_data.recv_flag = 0;//清除缓冲区
	
	//UDebug_SendBuff(USART5Buff,USART5Count);//打印测试
	wifi_data.recv_cnt = 0;
	return res;
}

/*******************************************************************************
函数名称：MyMQTT_Connect
函数作用：MQTT连接
函数入口：无
函数出口：
	0-OK 
	1-超时
	2-回传错误
函数作者：XYD_LSH
创建时间：2021.06.08
修改时间：2021.06.08
*******************************************************************************/
uint8_t MyMQTT_Connect(void)
{
	uint8_t WIFI_txBuff[firmware_max]={0};
	uint32_t len = 0;
	//1.创建MQTT CONNECT 结构体
	MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
	//2.结构体赋值
	data.cleansession = 1;//清除会话
	data.clientID.cstring = Ali_Buf.ClientID;//初始化设备ID
	data.keepAliveInterval = 60;//保持建立时长60s
	data.MQTTVersion = 4;//使用MQTT 3.1.1
	
	data.username.cstring = Ali_Buf.UserName;//用户名
	data.password.cstring = Ali_Buf.PassWord;//用户密码
	//3.调用MQTTSerialize_connect把结构体中的数据构成二进制流
	len = MQTTSerialize_connect(WIFI_txBuff,firmware_max,&data);
	if(len == 0) {
		printf("整合数据失败\r\n");
		return 1;//构成二进制数据流失败
	}
	else {
		for(uint16_t i=0; i<len; i++) {
			printf("%02X ", WIFI_txBuff[i]);
		}
		printf("\r\n");
	}
	//4.通过WIFI发送出去
	WIFI_ClearDat();
	WIFI_SendBuff(WIFI_txBuff, len);
	
	return 0;//打印测试：20 02 00 00
}

/*******************************************************************************
函数名称：MyMQTT_DisConnect
函数作用：MQTT断开连接
函数入口：无
函数出口：
	0-OK 
	1-超时
	2-回传错误
函数作者：XYD_LSH
创建时间：2021.06.08
修改时间：2021.06.08
*******************************************************************************/
uint8_t MyMQTT_DisConnect(void)
{
	uint8_t WIFI_txBuff[firmware_max]={0};
	uint32_t len = 0;
	//1.把DISCONNECT报文结构体，变为二进制数据存入发送缓冲区中
	len = MQTTSerialize_disconnect(WIFI_txBuff,firmware_max);
	if(len == 0)
		return 1;
	//2.调用底层网络发送数据
	WIFI_SendBuff(WIFI_txBuff,len);
	return 0;
}

/*******************************************************************************
函数名称：MyMQTT_Publish
函数作用：MQTT发布数据
函数入口：
	*topic：发布的Topic列表
	*payload：发布的数据内容，指向有效荷载存储区
函数出口：
	0-OK 
	1-超时
	2-回传错误
函数作者：XYD_LSH
创建时间：2021.06.08
修改时间：2021.06.08
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
	//1.初始化相关参数
	MQTTString topicString = MQTTString_initializer;
	topicString.cstring = topic;//保存主题
	payload_len = strlen(payload);//有效荷载的长度
	//2.构成Publish的二进制数据流
	len = MQTTSerialize_publish(WIFI_txBuff,firmware_max,dup, qos, retained, msgid,\
												topicString,(uint8_t *)payload,payload_len);
	if(len <= 0)
		return 1;
	//3.WIFI发送出去
	WIFI_SendBuff(WIFI_txBuff,len);	

	return 0;//打印测试：回传发布是否成功的响应
}

/*******************************************************************************
函数名称：MyMQTT_Subscribe
函数作用：MQTT订阅主题
函数入口：
	*topic：订阅的Topic列表
函数出口：
	0-OK 
	1-超时
	2-回传错误
函数作者：XYD_LSH
创建时间：2021.06.08
修改时间：2021.06.08
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
	//3.WIFI发送出去
	WIFI_SendBuff(WIFI_txBuff,len);
	
	return AliIOT_MQTTAckCheck(0x90,1000);//打印测试：90 03 00 0A 01
}

/*******************************************************************************
函数名称：MyMQTT_UnSubscribe
函数作用：MQTT取消订阅主题
函数入口：
	*topic：订阅的Topic列表
函数出口：
	0-OK 
	1-超时
	2-回传错误
函数作者：XYD_LSH
创建时间：2021.06.08
修改时间：2021.06.08
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
	//3.WIFI发送出去
	WIFI_SendBuff(WIFI_txBuff,len);
	
	return AliIOT_MQTTAckCheck(0xB0,1000);//打印测试：B0 02 00 00
}

/*******************************************************************************
函数名称：MQTT_GETJson_Parse
函数作用：解析MQTT协议中的JSON数据
函数入口：
	*str:待解析字符串
  *dststr:目标字符串
函数出口：
	0-OK
	1-root_error
	2-result_error
函数作者：XYD_LSH
创建时间：2021.06.08
修改时间：2021.06.08
*********************************************************************************/
uint8_t MQTT_GETJson_Parse(uint8_t *str,char *dststr)
{
	cJSON *root = NULL,*json_results = NULL, *value = NULL;
	//JSON解析返回的数据
	root = cJSON_Parse((char *)str);
	if(!root)
	{
		printf("root Error before: %s\n", cJSON_GetErrorPtr());
		return 1;
	}
	else
	{
		json_results = cJSON_GetObjectItem(root,"params");//获取参数对象
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
函数名称：MQTT_Getdata_Deal
函数作用：接收云端数据解析
函数入口：无
函数出口：无
函数作者：XYD_LSH
创建时间：2021.06.08
修改时间：2021.06.08
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
		固定报头：30 99 01  剩余长度=153
		TopicLEN： 00 33 
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
			//printf("当前Topic=%s\r\n",topic_str);
			
			//memcpy(message_str,&USART5Buff[BuffCount],OverLen - TopicLen);
			//printf("当前消息=%s\r\n",message_str);
			
			
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
			//printf("解析字符串=%s\r\n",&USART5Buff[BuffCount]);
			MQTT_GETJson_Parse((uint8_t *)&wifi_data.recv_data[BuffCount],"LEDSwitch");

		}
		WIFI_ClearDat();
		wifi_data.recv_flag=0;
	}
}

/*******************************************************************************
函数名称：MQTT_PublishAck_Deal
函数作用：发布响应解析
函数入口：无
函数出口：
    0-OK 
		>0---Error
函数作者：XYD_LSH
创建时间：2021.06.08
修改时间：2021.06.08
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
		固定报头：30 99 01  剩余长度=153
		TopicLEN： 00 33 
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
			//printf("当前Topic=%s\r\n",topic_str);
			
			//memcpy(message_str,&USART5Buff[BuffCount],OverLen - TopicLen);
			//printf("当前消息=%s\r\n",message_str);
			
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
			//JSON解析返回的数据
			root = cJSON_Parse((char *)&wifi_data.recv_data[BuffCount]);
			
			printf("解析字符串=%s\r\n",&wifi_data.recv_data[BuffCount]);
			if(!root)
			{
				printf("root Error before: %s\n", cJSON_GetErrorPtr());
			}
			else
			{
				json_results = cJSON_GetObjectItem(root,"message");//获取参数对象
				if(!json_results)
				{
					printf("results Error before: %s\n", cJSON_GetErrorPtr());
				}
				else
				{
					printf("响应=%s\r\n",json_results->valuestring);
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
函数名称：MQTT_Test
函数作用：MQTT测试：连接阿里云平台并向平台发布数据
函数入口：无
函数出口：无
函数作者：XYD_LSH
创建时间：2021.06.08
修改时间：2021.06.08
*********************************************************************************/
void MQTT_Test(void)
{
	
	//1.WIFI连接到aliyun
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
		
		//2.建立MQTT连接
		if(MyMQTT_Connect()!=0)
			printf("连接MQTT失败\r\n");
		else
		{
			//3.给smarthome/204/light主题发送2021消息
			if(MyMQTT_Publish(Ali_Buf.topic_post,Send_JsonBuf)!=0)
				printf("发布数据失败\r\n");
			
			Delay_ms(5000);
			//4.断开MQTT连接
			if(MyMQTT_DisConnect()!=0)
				printf("断开MQTT失败\r\n");
		}
		
		//5.断开服务器连接
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
	//printf("数据上报完成\r\n");
}
