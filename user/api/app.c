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
u8 choose_no = 1;//充电选择方式的变量
u8 choose_mode = 0;

//缴费方式信息的参数
typedef struct 
{
	char *content;//显示的内容
	u16 normal_back_color;//没有被选中的背景颜色
	u16 normal_color;//没有被选中的前景色
	u16 choose_back_color;//被选中后的背景颜色
	u16 choose_color;//被选中后的前景色
}CHARGER_WAY;
CHARGER_WAY charger_way[3]=
{
	{"时间",WHITE,BLACK,BLUE,YELLOW},
	{"金额",WHITE,BLACK,BLUE,YELLOW},
	{"电量",WHITE,BLACK,BLUE,YELLOW},
};
//缴费方式选择界面
/*
-----------
时间
-----------
金额
-----------
电量
-----------
*/
#define VIME_START_Y 50
void Charge_View(void)
{
	//1.显示名称
//	LCD_Show_Photo(65,10,20,20,gImage_charge);
	Draw_Text_8_16_Str(88,10,WHITE,BLACK,"充电桩");
	Draw_Text_8_16_Str(10,35,WHITE,BLACK,"功能选择");
	Draw_Text_8_16_Str(150,35,WHITE,BLACK,"数值");
	//2.打分割线
	for(u32 i=0;i<4;i++)
	{
		for(u32 j=0;j<29;j++)
		{
			Draw_Text_8_16_Str(j*8,VIME_START_Y+i*56,WHITE,BLACK,"*");
		}
	}
	//3.缴费方式：VIME_START_Y起始点 56间隔长度 24代表线与字的长度
	for(u32 i = 0;i<3;i++)
	{
			Draw_Text_8_16_Str(40,VIME_START_Y+(i*56)+40,charger_way[i].normal_back_color,
		charger_way[i].normal_color,charger_way[i].content);
	}
	//模式图标显示
	LCD_Show_Photo(5,80,30,29,gImage_time);
	LCD_Show_Photo(5,140,30,30,gImage_money);
	LCD_Show_Photo(5,195,30,30,gImage_power);
	//充电信息显示
	Draw_Text_8_16_Str(5,240,WHITE,0x001F,"消耗时间");
	Draw_Text_8_16_Str(5,270,WHITE,0x001F,"消耗金额");
	Draw_Text_8_16_Str(5,300,WHITE,0x001F,"消耗电量");

}
/*
修改选中的菜单
-----------
时间  -- 1
-----------
金额  -- 2
-----------
电量  -- 3
-----------
返回值：1 -- 错误  0 -- 正确
*/
u8 Charge_Choose_View(u8 num)
{
	static u8 last_num = 0;
	if(num <= 0 || num >= 4) return 1;
	num -= 1;
	//绘制选中的                                                                                                                                          
  Draw_Text_8_16_Str(40,VIME_START_Y+(num*56)+40,charger_way[num].choose_back_color,
	charger_way[num].choose_color,charger_way[num].content);
	if(last_num != num)
	{
		//清除上次选中   
		Draw_Text_8_16_Str(40,VIME_START_Y+(last_num*56)+40,charger_way[last_num].normal_back_color,
		charger_way[last_num].normal_color,charger_way[last_num].content);
		
		last_num = num;//更改上次选中的菜单
	}
	
	return 0;
}
//充电选项中的步进值
u8 charge_stepvalue[3] = {1,4,10};
/*
修改选中的的数据
-----------
时间  -- 1  单位（分钟） -- 步进值=1
----------- 
金额  -- 2  单元（元）   -- 步进值=4
-----------
电量  -- 3  单位（W）    -- 步进值=10
-----------
返回值：1 -- 错误  0 -- 正确
num -- 编号 flag -- 是否覆盖背景颜色
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
	//变量构成字符串
	sprintf(buff,"%4d",*p_charge_num[num]);
	switch(choose_mode)
	{
		case 0:
			if(laset_choose_mode != choose_mode)
				flag = 0;
			else
				flag = 1;
			//只绘制正常颜色+不打印背景颜色
			Draw_Text_8_16_Str(152,VIME_START_Y+(num*56)+40,WHITE,RED,buff);
			break;
		case 1:
			//绘制选中的颜色+打印背景颜色
			Draw_Text_8_16_Str(152,VIME_START_Y+(num*56)+40,WHITE,RED,buff);
			break;
	}
	if(last_num != num)
		last_num = num;//更改上次选中的菜单
	if(laset_choose_mode != choose_mode)
		laset_choose_mode = choose_mode;
	return 0;
}
/*
0 -- 充电方式的选择
1 -- 相应数字的选择
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
0 -- 充电方式的选择
1 -- 相应数字的选择
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
扣费功能
-----------
时间  -- 1  单位（分钟）
----------- 
金额  -- 2  单元（元）
-----------
电量  -- 3  单位（W）
-----------
价格：1	W -- 2元
1.充电车的功率 -- 滑动变阻器代替 -- U（滑动变阻器的值0 1 2 3（舍去小数点））*I(0.5A)
返回值: 0 -- 成功
		1 -- 充电枪无连接
		2 -- 寻卡失败
		3 -- 扣费失败
*/
u32 total_money = 0;//保存要扣费的金额
u16 car_power = 2;//车的功率 1分钟2W
u8 Func_Charging(u8 num)
{
	u32 timeout = 0;
	//清屏
	LCD_Fill(0,VIME_START_Y+16,240,VIME_START_Y+3*56,WHITE);
	//提示信息：
	//2.计算扣费金额
	switch(num)
	{
		case 1://时间充   10  40
			total_money = car_power * charge_time * 2;
			break;
		case 2://金额充
			total_money = charge_money;
			break;
		case 3://电量充
			total_money = charge_power * 2;
			break;
	}
	 Draw_Text_8_16_Str(24,VIME_START_Y+64,WHITE,RED,"正在充电,请等待.......");
	return 0;
}

void All_VlaueReset(void)
{
	//1.设备状态复位
	device1.state = 0;
	device1.time = 0; //时间++
	device1.money = 0;//汽车消耗的电能
	device1.power = 0;  //消耗的电能
	
	choose_no = 1;//充电选择方式的变量
	choose_mode = 0;//模式归零
	charge_time = 0;
	charge_money = 0;
	charge_power = 0;
}
