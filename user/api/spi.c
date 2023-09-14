#include "spi.h"
#include "delay.h"

#define  Delay_us  Delay_usnop

/*
  Ӳ������
	SDA(CS)  ----  PF4
	SCK      ----  PF3
	MOSI     ----  PF2
	MISO     ----  PF1
	GND      ----  GND
	RST      ----  PF0
	3.3V     ----  3.3V
*/
void SPI_RC522_Init(void)
{ 
  //SPIʱ��ʹ��
	SPI_SCK_GPIO_CLK_ENABLE();		//ʱ����
  SPI_MISO_GPIO_CLK_ENABLE();		//�������գ��ӻ�����
  SPI_MOSI_GPIO_CLK_ENABLE();		//�������ͣ��ӻ�����
  SPI_NSS_GPIO_CLK_ENABLE();		//Ƭѡ
  SPI_RESET_GPIO_CLK_ENABLE();	//��λ
	
	//���Ľṹ��
  GPIO_InitTypeDef GPIO_InitStruct;
	
  //ʱ����ģʽ����
  GPIO_InitStruct.GPIO_Pin  	= SPI_SCK_PIN;
  GPIO_InitStruct.GPIO_Mode 	= GPIO_Mode_Out_PP;
  GPIO_InitStruct.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_Init(SPI_SCK_GPIO_PORT, &GPIO_InitStruct);
	GPIO_SetBits(SPI_SCK_GPIO_PORT, SPI_SCK_PIN);//����ʱ����
	
  //MISO
  GPIO_InitStruct.GPIO_Pin = SPI_MISO_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//��������ģʽ
  GPIO_Init(SPI_MISO_GPIO_PORT, &GPIO_InitStruct);

  //MOSI
  GPIO_InitStruct.GPIO_Pin = SPI_MOSI_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SPI_MOSI_GPIO_PORT, &GPIO_InitStruct);
	
	//CS
  GPIO_InitStruct.GPIO_Pin = SPI_NSS_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SPI_NSS_GPIO_PORT, &GPIO_InitStruct);
	GPIO_SetBits(SPI_NSS_GPIO_PORT, SPI_NSS_PIN);  //����CS��

  //RESET
  GPIO_InitStruct.GPIO_Pin = SPI_RESET_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SPI_RESET_GPIO_PORT, &GPIO_InitStruct);
	GPIO_SetBits(SPI_RESET_GPIO_PORT, SPI_RESET_PIN);
}

//���ģ��SPIд(����)
void Soft_SPI_Write(uint8_t byte)
{
	uint8_t cnt;
	
	for(cnt=0;cnt<8;cnt++)
	{
		SCK_L;//����CLK
		Delay_us(10);//�����ʱʱ�����⣬��Ҫ����оƬ�����ֲ��ϵ�(���뼶��)
		if(byte &0x80)
		{
			MOSI_H;  //�����λΪ�ߣ��������
		}
		else
		{
			MOSI_L;   //�����λΪ�ͣ��������
		}
		byte <<= 1;
		Delay_us(10);
		SCK_H;     //����ʱ��
		Delay_us(10);
	}
}

//���ģ��SPI��(����)
uint8_t Soft_SPI_Read(void)
{
	uint8_t cnt;
	uint8_t Rxdata = 0;
	
	for(cnt=0;cnt<8;cnt++)
	{
		SCK_L;//����CLK
		Delay_us(10);
		Rxdata <<= 1;
		if(MISO)
		{
			Rxdata |= 0x01;
		}
		SCK_H;//����CLK
		Delay_us(10);
	}
	return Rxdata;
}

u8 SOFT_SPI_RW(u8 byte)
{
	u8 i,Temp=0;
	SCK_L;
  for(i=0;i<8;i++)     // ѭ��8��
  {
		SCK_H;     //����ʱ��
		if(byte&0x80)
		{
			MOSI_H;  //�����λΪ�ߣ��������
		}
		else      
		{
			MOSI_L;   //�����λΪ�ͣ��������
		}
		byte <<= 1;     // ��һλ��λ�����λ
		SCK_L;     //����ʱ��
		Temp <<= 1;     //��������

		if(MISO)
			Temp++;     //���Ӵӻ����յ��ߵ�ƽ�������Լ�һ
		SCK_H;     //����ʱ��
	}
  return (Temp);     //��������
}
