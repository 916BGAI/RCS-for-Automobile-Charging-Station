#ifndef __SPI_H_
#define __SPI_H_

#include "stm32f10x.h"

#include "io_bit.h"						  	    													  
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
/*MFRC522管脚定义*/
#define MOSI_522	 PFin(2) 
#define CS_522     PFout(4)
#define RST_522    PFout(0)

#define SPI_SCK_PIN                     GPIO_Pin_3
#define SPI_SCK_GPIO_PORT               GPIOF
#define SPI_SCK_PIN_CLK                 RCC_APB2Periph_GPIOF

#define SPI_MOSI_PIN                    GPIO_Pin_2
#define SPI_MOSI_GPIO_PORT              GPIOF
#define SPI_MOSI_PIN_CLK                RCC_APB2Periph_GPIOF

#define SPI_MISO_PIN                    GPIO_Pin_1
#define SPI_MISO_GPIO_PORT              GPIOF
#define SPI_MISO_PIN_CLK                RCC_APB2Periph_GPIOF

#define SPI_NSS_PIN                     GPIO_Pin_4
#define SPI_NSS_GPIO_PORT               GPIOF
#define SPI_NSS_PIN_CLK                 RCC_APB2Periph_GPIOF

#define SPI_RESET_PIN                   GPIO_Pin_0
#define SPI_RESET_GPIO_PORT             GPIOF
#define SPI_RESET_PIN_CLK               RCC_APB2Periph_GPIOF

#define SPI_SCK_GPIO_CLK_ENABLE()       RCC_APB2PeriphClockCmd(SPI_SCK_PIN_CLK,ENABLE)
#define SPI_MISO_GPIO_CLK_ENABLE()      RCC_APB2PeriphClockCmd(SPI_MISO_PIN_CLK,ENABLE)
#define SPI_MOSI_GPIO_CLK_ENABLE()      RCC_APB2PeriphClockCmd(SPI_MOSI_PIN_CLK,ENABLE)
#define SPI_NSS_GPIO_CLK_ENABLE()       RCC_APB2PeriphClockCmd(SPI_NSS_PIN_CLK,ENABLE)
#define SPI_RESET_GPIO_CLK_ENABLE()     RCC_APB2PeriphClockCmd(SPI_RESET_PIN_CLK,ENABLE)

#define MOSI_H  GPIO_SetBits(SPI_MOSI_GPIO_PORT, SPI_MOSI_PIN) 
#define MOSI_L  GPIO_ResetBits(SPI_MOSI_GPIO_PORT, SPI_MOSI_PIN)  
#define SCK_H   GPIO_SetBits(SPI_SCK_GPIO_PORT, SPI_SCK_PIN)  
#define SCK_L   GPIO_ResetBits(SPI_SCK_GPIO_PORT, SPI_SCK_PIN)  
#define MISO    GPIO_ReadInputDataBit(SPI_MISO_GPIO_PORT, SPI_MISO_PIN) 
#define NSS_H   GPIO_SetBits(SPI_NSS_GPIO_PORT, SPI_NSS_PIN)  
#define NSS_L   GPIO_ResetBits(SPI_NSS_GPIO_PORT, SPI_NSS_PIN) 

void SPI_RC522_Init(void);
void Soft_SPI_Write(uint8_t byte);
uint8_t Soft_SPI_Read(void);
u8 SOFT_SPI_RW(u8 byte);

#endif
