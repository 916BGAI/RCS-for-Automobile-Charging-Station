#include "esp8266.h"
#include "delay.h"
#include "cjson.h"

WIFI_DATA wifi_data={0};
/*
**********************************************
*@����:����ESP8266  
*@����:USART1 TX PB10 RX PB11  EN PE6
*
**********************************************
*/
void ESP8266_Config(void)
{
	//��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOE,ENABLE);
	//TX
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//��������ģʽ
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	//RX
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	//BUSY
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;//��������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOE,&GPIO_InitStruct);
	GPIO_SetBits(GPIOE,GPIO_Pin_6);
	//����USART3����
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//������ʱ��

	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 115200;//���ڲ�����
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//��ʹ��Ӳ������
	USART_InitStruct.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;//�������ͺͽ���  ȫ˫��
	USART_InitStruct.USART_Parity=USART_Parity_No;//��ż��У��
	USART_InitStruct.USART_StopBits=USART_StopBits_1;//1λֹͣλ
	USART_InitStruct.USART_WordLength=USART_WordLength_8b;//8λ�ֳ�	
	USART_Init(USART3,&USART_InitStruct);
	
//�����ж�
	USART_ITConfig(USART3,USART_IT_RXNE, ENABLE);
//�����ж�
	USART_ITConfig(USART3,USART_IT_IDLE, ENABLE);
	
	NVIC_InitTypeDef esp8266;
	esp8266.NVIC_IRQChannel = USART3_IRQn;
	esp8266.NVIC_IRQChannelCmd = ENABLE;
	esp8266.NVIC_IRQChannelPreemptionPriority = 0;
	esp8266.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&esp8266);
//ENABLEʹ�ܴ���
	USART_Cmd(USART3,ENABLE);
}

char charger_buf[1024] = {0};
//�жϷ�����
void USART3_IRQHandler(void)
{
	u8 data = 0;
	//���������ж� 
	if(USART_GetITStatus(USART3,USART_IT_RXNE) == SET)
	{
		data = USART_ReceiveData(USART3);
		if(data=='\0') data= '0';
		USART1->DR = data;
		wifi_data.recv_data[wifi_data.recv_cnt++] = data;
		wifi_data.recv_cnt %= MAX;
		
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
	}
	//�����ж� 1��
	if(USART_GetITStatus(USART3,USART_IT_IDLE)==SET)
	{
		//��������ж�
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
	//printf("\r\n��������home%s\r\n",charger_buf);
	u8 time = 0;
	if(charger_buf[0] == '{')
	{
		cJSON *Root = cJSON_Parse(charger_buf);
		cJSON_Print(Root);
		cJSON *params = cJSON_GetObjectItem(Root,"params");
		printf("params�����ɹ��������С%d\r\n",cJSON_GetArraySize(params));
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
		//printf("ָ�����\r\n");
		return 1;
	}	
	return 0;
}

void Wifi_Init(void)
{
	if(ESP8266_SendCmd("AT\r\n","OK",10000) == 0)
	{
		printf("ESP8266������\r\n");
		if(ESP8266_SendCmd("AT+CWMODE=1\r\n","OK",10000) == 0)
		{
			printf("ģʽ1�Ѵ�\r\n");
			if(ESP8266_SendCmd("AT+CWJAP=\"qweraz\",\"12345678\"\r\n","OK",10000) == 0)
			{
				printf("wifi������\r\n");
			}
		}
	}
}

//�˳�͸��
void WIFI_CloseTransmission(void)
{
	Usart3_SendStr("+++\r\n");
	Delay_ms(500);
	Usart3_SendStr("+++\r\n");
	Delay_ms(500);
}

/**
 * ���ܣ�ʹ��ָ��Э��(TCP/UDP)���ӵ�������
 * ������
 *         mode:Э������ "TCP","UDP"
 *         ip:Ŀ�������IP
 *         port:Ŀ���Ƿ������˿ں�
 * ����ֵ��
 *         ���ӽ��,��0���ӳɹ�,0����ʧ��
 * ˵���� 
 *         ʧ�ܵ�ԭ�������¼���(UARTͨ�ź�ESP8266���������)
 *         1. Զ�̷�����IP�Ͷ˿ں�����
 *         2. δ����AP
 *         3. �������˽�ֹ���(һ�㲻�ᷢ��)
 */
uint8_t WIFI_ConnectServer(char* mode, char* ip, uint16_t port)
{
	u8 flag = 0;

	char init_esp_buf[128] = {0};
	WIFI_CloseTransmission();//�˳�͸��
	
	sprintf((char *)init_esp_buf, "AT+CIPSTART=\"%s\",\"%s\",%d\r\n", mode, ip, port);

	if(ESP8266_SendCmd(init_esp_buf,"CONNECT",10000) == 0)
	{
		printf("���ӷ������ɹ�\r\n");
	}
	else
	{
		printf("���ӷ�����ʧ��\r\n");
		return 1;
	}
	
	//����͸��ģʽ
	if(ESP8266_SendCmd("AT+CIPMODE=1\r\n","OK",1000) != 0) 
		return 1;
	
	if(ESP8266_SendCmd("AT+CIPSEND\r\n",">",500) != 0)
		return 1;
	printf("ESP8266����͸��ģʽ\r\n>\r\n");
	return 0;
}

/*�����BUFF�ͱ�־λ*/
void WIFI_ClearDat(void)
{
	memset(wifi_data.recv_data,0,MAX);
	wifi_data.recv_flag=0;
	wifi_data.recv_cnt=0;
}

/*WIFI����BUFF*/
void WIFI_SendBuff(uint8_t *buff, uint16_t len)
{
	uint16_t i = 0;
	WIFI_ClearDat();
	for(i=0; i<len; i++) {
		Usart3_SendByte(buff[i]);
	}
}

/*
�ر������������
����ֵ��
0 -- �ɹ�
>1 -- ʧ��
8 -- ��������Ͽ�ʧ��
*/
uint8_t WIFI_DisconnectServer(void)
{
	uint8_t rev = 0;
	WIFI_ClearDat();//���������
	//1.�˳�͸��ģʽ
	printf("ESP8266�Ͽ�������");//��ʾ��Ϣ
	WIFI_CloseTransmission();
	Delay_ms(1000);//��ʱ1s
	WIFI_CloseTransmission();
	Delay_ms(1000);//��ʱ1s
	WIFI_CloseTransmission();
	
	rev =ESP8266_SendCmd("AT+CIPMODE=0","OK",5000);
	if(rev != 0)
		return 10;
	//3.AT+CIPCLOSEָ��˳�����������
	rev =ESP8266_SendCmd("AT+CIPCLOSE","CLOSED",5000);
	if(rev != 0)
		return 8;
	printf("�ɹ�\r\n");//��ʾ��Ϣ
	
	return 0;
}
