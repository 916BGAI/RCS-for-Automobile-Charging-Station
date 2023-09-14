#include "spi.h"
#include "delay.h"

#define  Delay_us  Delay_usnop

/*
  硬件接线
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
  //SPI时钟使能
	SPI_SCK_GPIO_CLK_ENABLE();		//时钟线
  SPI_MISO_GPIO_CLK_ENABLE();		//主机接收，从机发送
  SPI_MOSI_GPIO_CLK_ENABLE();		//主机发送，从机接收
  SPI_NSS_GPIO_CLK_ENABLE();		//片选
  SPI_RESET_GPIO_CLK_ENABLE();	//复位
	
	//核心结构体
  GPIO_InitTypeDef GPIO_InitStruct;
	
  //时钟线模式配置
  GPIO_InitStruct.GPIO_Pin  	= SPI_SCK_PIN;
  GPIO_InitStruct.GPIO_Mode 	= GPIO_Mode_Out_PP;
  GPIO_InitStruct.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_Init(SPI_SCK_GPIO_PORT, &GPIO_InitStruct);
	GPIO_SetBits(SPI_SCK_GPIO_PORT, SPI_SCK_PIN);//拉高时钟线
	
  //MISO
  GPIO_InitStruct.GPIO_Pin = SPI_MISO_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//浮空输入模式
  GPIO_Init(SPI_MISO_GPIO_PORT, &GPIO_InitStruct);

  //MOSI
  GPIO_InitStruct.GPIO_Pin = SPI_MOSI_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SPI_MOSI_GPIO_PORT, &GPIO_InitStruct);
	
	//CS
  GPIO_InitStruct.GPIO_Pin = SPI_NSS_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SPI_NSS_GPIO_PORT, &GPIO_InitStruct);
	GPIO_SetBits(SPI_NSS_GPIO_PORT, SPI_NSS_PIN);  //拉高CS线

  //RESET
  GPIO_InitStruct.GPIO_Pin = SPI_RESET_PIN;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SPI_RESET_GPIO_PORT, &GPIO_InitStruct);
	GPIO_SetBits(SPI_RESET_GPIO_PORT, SPI_RESET_PIN);
}

//软件模拟SPI写(发送)
void Soft_SPI_Write(uint8_t byte)
{
	uint8_t cnt;
	
	for(cnt=0;cnt<8;cnt++)
	{
		SCK_L;//拉低CLK
		Delay_us(10);//这个延时时间任意，但要大于芯片数据手册上的(纳秒级的)
		if(byte &0x80)
		{
			MOSI_H;  //若最高位为高，则输出高
		}
		else
		{
			MOSI_L;   //若最高位为低，则输出低
		}
		byte <<= 1;
		Delay_us(10);
		SCK_H;     //拉高时钟
		Delay_us(10);
	}
}

//软件模拟SPI读(接收)
uint8_t Soft_SPI_Read(void)
{
	uint8_t cnt;
	uint8_t Rxdata = 0;
	
	for(cnt=0;cnt<8;cnt++)
	{
		SCK_L;//拉低CLK
		Delay_us(10);
		Rxdata <<= 1;
		if(MISO)
		{
			Rxdata |= 0x01;
		}
		SCK_H;//拉高CLK
		Delay_us(10);
	}
	return Rxdata;
}

u8 SOFT_SPI_RW(u8 byte)
{
	u8 i,Temp=0;
	SCK_L;
  for(i=0;i<8;i++)     // 循环8次
  {
		SCK_H;     //拉高时钟
		if(byte&0x80)
		{
			MOSI_H;  //若最高位为高，则输出高
		}
		else      
		{
			MOSI_L;   //若最高位为低，则输出低
		}
		byte <<= 1;     // 低一位移位到最高位
		SCK_L;     //拉低时钟
		Temp <<= 1;     //数据左移

		if(MISO)
			Temp++;     //若从从机接收到高电平，数据自加一
		SCK_H;     //拉高时钟
	}
  return (Temp);     //返回数据
}
