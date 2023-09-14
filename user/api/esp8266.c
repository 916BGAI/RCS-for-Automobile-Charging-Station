#include "esp8266.h"
#include "delay.h"
#include "cjson.h"

WIFI_DATA wifi_data={0};
/*
**********************************************
*@功能:配置ESP8266  
*@描述:USART1 TX PB10 RX PB11  EN PE6
*
**********************************************
*/
void ESP8266_Config(void)
{
	//开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOE,ENABLE);
	//TX
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽模式
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	//RX
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	//BUSY
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;//浮空输入
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOE,&GPIO_InitStruct);
	GPIO_SetBits(GPIOE,GPIO_Pin_6);
	//串口USART3配置
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//开串口时钟

	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 115200;//串口波特率
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//不使用硬件控制
	USART_InitStruct.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;//开启发送和接受  全双工
	USART_InitStruct.USART_Parity=USART_Parity_No;//奇偶不校验
	USART_InitStruct.USART_StopBits=USART_StopBits_1;//1位停止位
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;//8位字长	
	USART_Init(USART3,&USART_InitStruct);
	
//接受中断
	USART_ITConfig(USART3,USART_IT_RXNE, ENABLE);
//空闲中断
	USART_ITConfig(USART3,USART_IT_IDLE, ENABLE);
	
	NVIC_InitTypeDef esp8266;
	esp8266.NVIC_IRQChannel = USART3_IRQn;
	esp8266.NVIC_IRQChannelCmd = ENABLE;
	esp8266.NVIC_IRQChannelPreemptionPriority = 0;
	esp8266.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&esp8266);
//ENABLE使能串口
	USART_Cmd(USART3,ENABLE);
}

char charger_buf[1024] = {0};
//中断服务函数
void USART3_IRQHandler(void)
{
	u8 data = 0;
	//接收数据中断 
	if(USART_GetITStatus(USART3,USART_IT_RXNE) == SET)
	{
		data = USART_ReceiveData(USART3);
		if(data=='\0') data= '0';
		USART1->DR = data;
		wifi_data.recv_data[wifi_data.recv_cnt++] = data;
		wifi_data.recv_cnt %= MAX;
		
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
	}
	//空闲中断 1次
	if(USART_GetITStatus(USART3,USART_IT_IDLE)==SET)
	{
		//清除空闲中断
		wifi_data.recv_flag = 1;
		if(wifi_data.recv_data[0] == '0')
		{
			sprintf(charger_buf,"%s",strstr(wifi_data.recv_data,"{"));
		}
		data = USART3->SR;
		data = USART3->DR;
	}
}

void Charger_Anlyze(void)
{
	//printf("\r\n接受数据home%s\r\n",charger_buf);
	u8 time = 0;
	if(charger_buf[0] == '{')
	{
		cJSON *Root = cJSON_Parse(charger_buf);
		cJSON_Print(Root);
		cJSON *params = cJSON_GetObjectItem(Root,"params");
		printf("params解析成功，数组大小%d\r\n",cJSON_GetArraySize(params));
		printf("test:%d\r\n",params->child->valueint);
		cJSON_Delete(Root);
		memset(charger_buf,0,1024);
	}
	
}

void Usart3_SendByte(u8 data)
{
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC) ==RESET){}
	USART_SendData(USART3,data);
}

void Usart3_SendStr(char *p)
{
	while(*p != '\0')
	{
		while(USART_GetFlagStatus(USART3,USART_FLAG_TC) ==RESET){}
		USART_SendData(USART3,*p);
		p++;
	}
}

char * FindStr(char *scr,char *dest,u16 timeout)
{
	while(strstr(scr,dest)==NULL)
	{
		timeout--;
		if(timeout == 0)
		{
			return NULL;
		}
		Delay_ms(1);
	}
	return strstr(scr,dest);
}

u8 ESP8266_SendCmd(char *cmd,char *ack,u16 timeout)
{
	memset(wifi_data.recv_data,0,MAX);
	wifi_data.recv_cnt = 0;
	Usart3_SendStr(cmd);
	if(ack == NULL)
	{
		return 0;
	}else if(FindStr(wifi_data.recv_data,ack,timeout) == NULL)
	{
		//printf("指令错误\r\n");
		return 1;
	}	
	return 0;
}

void Wifi_Init(void)
{
	if(ESP8266_SendCmd("AT\r\n","OK",10000) == 0)
	{
		printf("ESP8266已连接\r\n");
		if(ESP8266_SendCmd("AT+CWMODE=1\r\n","OK",10000) == 0)
		{
			printf("模式1已打开\r\n");
			if(ESP8266_SendCmd("AT+CWJAP=\"qweraz\",\"12345678\"\r\n","OK",10000) == 0)
			{
				printf("wifi已连接\r\n");
			}
		}
	}
}

//退出透传
void WIFI_CloseTransmission(void)
{
	Usart3_SendStr("+++\r\n");
	Delay_ms(500);
	Usart3_SendStr("+++\r\n");
	Delay_ms(500);
}

/**
 * 功能：使用指定协议(TCP/UDP)连接到服务器
 * 参数：
 *         mode:协议类型 "TCP","UDP"
 *         ip:目标服务器IP
 *         port:目标是服务器端口号
 * 返回值：
 *         连接结果,非0连接成功,0连接失败
 * 说明： 
 *         失败的原因有以下几种(UART通信和ESP8266正常情况下)
 *         1. 远程服务器IP和端口号有误
 *         2. 未连接AP
 *         3. 服务器端禁止添加(一般不会发生)
 */
uint8_t WIFI_ConnectServer(char* mode, char* ip, uint16_t port)
{
	u8 flag = 0;

	char init_esp_buf[128] = {0};
	WIFI_CloseTransmission();//退出透传
	
	sprintf((char *)init_esp_buf, "AT+CIPSTART=\"%s\",\"%s\",%d\r\n", mode, ip, port);

	if(ESP8266_SendCmd(init_esp_buf,"CONNECT",10000) == 0)
	{
		printf("连接服务器成功\r\n");
	}
	else
	{
		printf("连接服务器失败\r\n");
		return 1;
	}
	
	//设置透传模式
	if(ESP8266_SendCmd("AT+CIPMODE=1\r\n","OK",1000) != 0) 
		return 1;
	
	if(ESP8266_SendCmd("AT+CIPSEND\r\n",">",500) != 0)
		return 1;
	printf("ESP8266进入透传模式\r\n>\r\n");
	return 0;
}

/*清接收BUFF和标志位*/
void WIFI_ClearDat(void)
{
	memset(wifi_data.recv_data,0,MAX);
	wifi_data.recv_flag=0;
	wifi_data.recv_cnt=0;
}

/*WIFI发送BUFF*/
void WIFI_SendBuff(uint8_t *buff, uint16_t len)
{
	uint16_t i = 0;
	WIFI_ClearDat();
	for(i=0; i<len; i++) {
		Usart3_SendByte(buff[i]);
	}
}

/*
关闭与服务器连接
返回值：
0 -- 成功
>1 -- 失败
8 -- 与服务器断开失败
*/
uint8_t WIFI_DisconnectServer(void)
{
	uint8_t rev = 0;
	WIFI_ClearDat();//清接收数据
	//1.退出透传模式
	printf("ESP8266断开服务器");//提示信息
	WIFI_CloseTransmission();
	Delay_ms(1000);//延时1s
	WIFI_CloseTransmission();
	Delay_ms(1000);//延时1s
	WIFI_CloseTransmission();
	
	rev =ESP8266_SendCmd("AT+CIPMODE=0","OK",5000);
	if(rev != 0)
		return 10;
	//3.AT+CIPCLOSE指令，退出服务器连接
	rev =ESP8266_SendCmd("AT+CIPCLOSE","CLOSED",5000);
	if(rev != 0)
		return 8;
	printf("成功\r\n");//提示信息
	
	return 0;
}
