#ifndef _APP_H_
#define _APP_H_

#include <stm32f10x.h>

typedef struct
{
	u16 charger_state;//���ǹ����״̬ 0 -- δ���� 1 -- ����
	u16 state;        //���״̬ -- ���ݼ̵���״̬+���ǹ
	u16 time;         //���ʱ��
	u16 money;        //�����ѵĽ��
	u16 power;        //�����ĵĵ���
	
}Charging_Pile;

extern Charging_Pile device1;

////��ȡ���ǹ״̬
#define Charger (GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4))

#define led4_status (GPIO_ReadOutputDataBit(GPIOE,GPIO_Pin_5))

extern u16 charge_time;
extern u16 charge_money;
extern u16 charge_power;
extern u8 charge_stepvalue[3];
extern u8 choose_no;
extern u8 choose_mode;
extern u32 total_money;
extern u16 car_power;

void Charger_Config(void);
void Charge_View(void);
u8 Charge_Choose_View(u8 num);
u8 Charge_Choose_Num(u8 num);
void Func_Add(u8 mode);
void Func_Sub(u8 mode);
u8 Func_Charging(u8 num);
void All_VlaueReset(void);
#endif
