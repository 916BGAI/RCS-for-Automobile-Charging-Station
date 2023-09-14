#include <stdio.h>
#include "app.h"
#include "lcd.h"
#include "picture.h"

#define Delay_ms Delay_msnop
//extern unsigned char gImage_charge[800];
// extern const unsigned char gImage_time[1748];
// extern const unsigned char gImage_money[1808];
// extern const unsigned char gImage_power[1808];
Charging_Pile device1 = {0,0,0,0,0};
u8 choose_no = 1;//���ѡ��ʽ�ı���
u8 choose_mode = 0;

//�ɷѷ�ʽ��Ϣ�Ĳ���
typedef struct 
{
	char *content;//��ʾ������
	u16 normal_back_color;//û�б�ѡ�еı�����ɫ
	u16 normal_color;//û�б�ѡ�е�ǰ��ɫ
	u16 choose_back_color;//��ѡ�к�ı�����ɫ
	u16 choose_color;//��ѡ�к��ǰ��ɫ
}CHARGER_WAY;
CHARGER_WAY charger_way[3]=
{
	{"ʱ��",WHITE,BLACK,BLUE,YELLOW},
	{"���",WHITE,BLACK,BLUE,YELLOW},
	{"����",WHITE,BLACK,BLUE,YELLOW},
};
//�ɷѷ�ʽѡ�����
/*
-----------
ʱ��
-----------
���
-----------
����
-----------
*/
#define VIME_START_Y 50
void Charge_View(void)
{
	//1.��ʾ����
//	LCD_Show_Photo(65,10,20,20,gImage_charge);
	Draw_Text_8_16_Str(88,10,WHITE,BLACK,"���׮");
	Draw_Text_8_16_Str(10,35,WHITE,BLACK,"����ѡ��");
	Draw_Text_8_16_Str(150,35,WHITE,BLACK,"��ֵ");
	//2.��ָ���
	for(u32 i=0;i<4;i++)
	{
		for(u32 j=0;j<29;j++)
		{
			Draw_Text_8_16_Str(j*8,VIME_START_Y+i*56,WHITE,BLACK,"*");
		}
	}
	//3.�ɷѷ�ʽ��VIME_START_Y��ʼ�� 56������� 24���������ֵĳ���
	for(u32 i = 0;i<3;i++)
	{
			Draw_Text_8_16_Str(40,VIME_START_Y+(i*56)+40,charger_way[i].normal_back_color,
		charger_way[i].normal_color,charger_way[i].content);
	}
	//ģʽͼ����ʾ
	LCD_Show_Photo(5,80,30,29,gImage_time);
	LCD_Show_Photo(5,140,30,30,gImage_money);
	LCD_Show_Photo(5,195,30,30,gImage_power);
	//�����Ϣ��ʾ
	Draw_Text_8_16_Str(5,240,WHITE,0x001F,"����ʱ��");
	Draw_Text_8_16_Str(5,270,WHITE,0x001F,"���Ľ��");
	Draw_Text_8_16_Str(5,300,WHITE,0x001F,"���ĵ���");

}
/*
�޸�ѡ�еĲ˵�
-----------
ʱ��  -- 1
-----------
���  -- 2
-----------
����  -- 3
-----------
����ֵ��1 -- ����  0 -- ��ȷ
*/
u8 Charge_Choose_View(u8 num)
{
	static u8 last_num = 0;
	if(num <= 0 || num >= 4) return 1;
	num -= 1;
	//����ѡ�е�                                                                                                                                          
  Draw_Text_8_16_Str(40,VIME_START_Y+(num*56)+40,charger_way[num].choose_back_color,
	charger_way[num].choose_color,charger_way[num].content);
	if(last_num != num)
	{
		//����ϴ�ѡ��   
		Draw_Text_8_16_Str(40,VIME_START_Y+(last_num*56)+40,charger_way[last_num].normal_back_color,
		charger_way[last_num].normal_color,charger_way[last_num].content);
		
		last_num = num;//�����ϴ�ѡ�еĲ˵�
	}
	
	return 0;
}
//���ѡ���еĲ���ֵ
u8 charge_stepvalue[3] = {1,4,10};
/*
�޸�ѡ�еĵ�����
-----------
ʱ��  -- 1  ��λ�����ӣ� -- ����ֵ=1
----------- 
���  -- 2  ��Ԫ��Ԫ��   -- ����ֵ=4
-----------
����  -- 3  ��λ��W��    -- ����ֵ=10
-----------
����ֵ��1 -- ����  0 -- ��ȷ
num -- ��� flag -- �Ƿ񸲸Ǳ�����ɫ
*/
u16 charge_time = 0;
u16 charge_money = 0;
u16 charge_power = 0;
u16 *p_charge_num[3] = {&charge_time,&charge_money,&charge_power};
u8 Charge_Choose_Num(u8 num)
{
	static u8 last_num = 0;
	static u8 laset_choose_mode = 0;
	u8 flag = 1;
	char buff[6]={0};
	if(num <= 0 || num >= 4) return 1;
	num -= 1;
	//���������ַ���
	sprintf(buff,"%4d",*p_charge_num[num]);
	switch(choose_mode)
	{
		case 0:
			if(laset_choose_mode != choose_mode)
				flag = 0;
			else
				flag = 1;
			//ֻ����������ɫ+����ӡ������ɫ
			Draw_Text_8_16_Str(152,VIME_START_Y+(num*56)+40,WHITE,RED,buff);
			break;
		case 1:
			//����ѡ�е���ɫ+��ӡ������ɫ
			Draw_Text_8_16_Str(152,VIME_START_Y+(num*56)+40,WHITE,RED,buff);
			break;
	}
	if(last_num != num)
		last_num = num;//�����ϴ�ѡ�еĲ˵�
	if(laset_choose_mode != choose_mode)
		laset_choose_mode = choose_mode;
	return 0;
}
/*
0 -- ��緽ʽ��ѡ��
1 -- ��Ӧ���ֵ�ѡ��
*/

void Func_Add(u8 mode)
{
	switch(mode)
	{
		case 0:
			if(choose_no < 3)
				choose_no++;
			else
				choose_no = 1;
			break;
		case 1:
			if(*p_charge_num[choose_no-1] < 9999)
				*p_charge_num[choose_no-1] += charge_stepvalue[choose_no-1];	
			break;
		
	}
}
/*
0 -- ��緽ʽ��ѡ��
1 -- ��Ӧ���ֵ�ѡ��
*/
void Func_Sub(u8 mode)
{
	switch(mode)
	{
		case 0:
			if(choose_no > 1)
				choose_no--;
			else
				choose_no = 3;
			break;
		case 1:
			if(*p_charge_num[choose_no-1] > 0)
				*p_charge_num[choose_no-1] -=charge_stepvalue[choose_no-1];	
			break;
		
	}
}

/*
�۷ѹ���
-----------
ʱ��  -- 1  ��λ�����ӣ�
----------- 
���  -- 2  ��Ԫ��Ԫ��
-----------
����  -- 3  ��λ��W��
-----------
�۸�1	W -- 2Ԫ
1.��糵�Ĺ��� -- �������������� -- U��������������ֵ0 1 2 3����ȥС���㣩��*I(0.5A)
����ֵ: 0 -- �ɹ�
		1 -- ���ǹ������
		2 -- Ѱ��ʧ��
		3 -- �۷�ʧ��
*/
u32 total_money = 0;//����Ҫ�۷ѵĽ��
u16 car_power = 2;//���Ĺ��� 1����2W
u8 Func_Charging(u8 num)
{
	u32 timeout = 0;
	//����
	LCD_Fill(0,VIME_START_Y+16,240,VIME_START_Y+3*56,WHITE);
	//��ʾ��Ϣ��
	//2.����۷ѽ��
	switch(num)
	{
		case 1://ʱ���   10  40
			total_money = car_power * charge_time * 2;
			break;
		case 2://����
			total_money = charge_money;
			break;
		case 3://������
			total_money = charge_power * 2;
			break;
	}
	 Draw_Text_8_16_Str(24,VIME_START_Y+64,WHITE,RED,"���ڳ��,��ȴ�.......");
	return 0;
}

void All_VlaueReset(void)
{
	//1.�豸״̬��λ
	device1.state = 0;
	device1.time = 0; //ʱ��++
	device1.money = 0;//�������ĵĵ���
	device1.power = 0;  //���ĵĵ���
	
	choose_no = 1;//���ѡ��ʽ�ı���
	choose_mode = 0;//ģʽ����
	charge_time = 0;
	charge_money = 0;
	charge_power = 0;
}
