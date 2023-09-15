#ifndef _KEY_H_
#define _KEY_H_

#include <stdint.h>
#include <stm32f10x.h>

static uint16_t num_on = 0;
static uint16_t key_old = 0;

#define Key_Scan_Time 20

typedef enum
{
    KEY_NULL = 0,
    KEY1_PRES = 1,
    KEY2_PRES = 2,
    KEY3_PRES = 3,
    KEY4_PRES = 4,
}KEY_STATE;

typedef enum
{
    KEY_PRES_NULL = 0,
    KEY1_PRES_SHORT = 1,
    KEY1_PRES_LONG = 2,
    KEY2_PRES_SHORT = 3,
    KEY2_PRES_LONG = 4,
    KEY3_PRES_SHORT = 5,
    KEY3_PRES_LONG = 6,
    KEY4_PRES_SHORT = 7,
    KEY4_PRES_LONG = 8,
}KEY_VALUE;

#define Key1 GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)
#define Key2 GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4)
#define Key3 GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5)
#define Key4 GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6)

void Key_Config(void);
KEY_VALUE Key_GetValue(void);

#endif // _KEY_H_
