#ifndef _DHT11_H_
#define _DHT11_H_

#include <stdint.h>
#include <stm32f10x.h>

#define DHT11_H (GPIO_WriteBit(GPIOG, GPIO_Pin_11, Bit_SET))
#define DHT11_L (GPIO_WriteBit(GPIOG, GPIO_Pin_11, Bit_RESET))
#define DHT11_ReadPin (GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_11))

typedef struct
{
    uint8_t HUM; // Êª¶È
    uint8_t TEMP; // ÎÂ¶È
} typedef_DATA;

extern typedef_DATA dht11_data;

void DHT11_Config(uint8_t mode);
uint16_t DHT11_ReadData(void);

#endif // _DHT11_H_
