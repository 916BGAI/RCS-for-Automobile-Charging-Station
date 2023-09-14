#ifndef _ADC_H_
#define _ADC_H_

#include <stdint.h>
#include <stm32f10x.h>

typedef struct{
	uint16_t light;
}typedef_ADCDATA;

extern typedef_ADCDATA adc_data;

void ADC_Config(void);
uint16_t ADC_GetValue(void);

#endif // _ADC_H_
