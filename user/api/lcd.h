#ifndef _LCD_H_
#define _LCD_H_

#include <stm32f10x.h>

#define LCD_HEIGHT	320
#define LCD_WIDTH		240

#define PI 3.14159

//LCD重要参数集
typedef struct  
{										    
	u16 width;			//LCD 宽度
	u16 height;			//LCD 高度
	u16 id;				//LCD ID
	u8  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	u8	wramcmd;		//开始写gram指令
	u8  setxcmd;		//设置x坐标指令
	u8  setycmd;		//设置y坐标指令	 
}_lcd_dev; 	 

//LCD参数
extern _lcd_dev lcddev;	//管理LCD重要参数  

typedef struct
{
	u16 LCD_REG;
	u16 LCD_RAM;
} LCD_TypeDef;
//使用NOR/SRAM的 Bank1.sector4,地址位HADDR[27,26]=11 A10作为数据命令区分线 		   

 //CPU   0~25    
#define LCD_BASE        ((u32)(0x6C000000 | 0x000007FE))  //111 1111 1110 (A0~A10)     2^10=1K
#define LCD             ((LCD_TypeDef *) LCD_BASE)

//扫描方向定义
#define L2R_U2D  0 //从左到右,从上到下
#define L2R_D2U  1 //从左到右,从下到上
#define R2L_U2D  2 //从右到左,从上到下
#define R2L_D2U  3 //从右到左,从下到上

#define U2D_L2R  4 //从上到下,从左到右
#define U2D_R2L  5 //从上到下,从右到左
#define D2U_L2R  6 //从下到上,从左到右
#define D2U_R2L  7 //从下到上,从右到左	 

#define DFT_SCAN_DIR  L2R_U2D  //默认的扫描方向

//画笔颜色
#define HSBLUE		0x041F

#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
#define PINK				 0XFE19//粉红色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 

#define LIGHTGREEN     	 0X841F //浅绿色
//#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

/****************初始化函数*************************/
void LCD_Init(void);										//初始化

/****************背光操作函数*************************/
void LCD_DisplayOn(void);								//开显示
void LCD_DisplayOff(void);							//关显示

/****************ILI9341底层操作函数*************************/
void LCD_WR_DATA(u16 data);
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue);
u16  LCD_ReadReg(u8 LCD_Reg);
void LCD_WriteRAM_Prepare(void);
void LCD_WriteRAM(u16 RGB_Code);		  
void LCD_Scan_Dir(u8 dir);							//设置屏扫描方向
void LCD_Display_Dir(u8 dir);						//设置屏幕显示方向

/****************LCD屏BASE函数*************************/
void LCD_SetCursor(u16 Xpos, u16 Ypos);	//设置光标
void LCD_Clear(u16 Color);	 						//清屏
void LCD_DrawPoint(u16 x,u16 y,u16 c);  //打点函数

/****************LCD屏显示字符函数*************************/
void  show_xXx(u32 x ,u32 y,u16 backColor,u16 color,const u8 *ch ,u16 size_line,u16 size_bits);
void  Draw_Text_8_16_Str(u32 x,u32 y,u16 backColor,u16 color, char *chs);

/****************LCD屏显示图片函数*************************/
void  LCD_Show_Photo(int x0,int y0,int width,int high,const unsigned char photo[]);

/****************其它操作函数*************************/
u16 LCD_ReadPoint(u16 x,u16 y);
char *my_itoa_n(int num,char *str,int radix);
void LCD_ShowxNum(u16 x,u16 y,u32 num,u16 BlackColor,u16 Color);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);//画线
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);	//矩形填充单色
void LCD_Address_Set(uint16_t x_start,uint16_t y_start,uint16_t x_end,uint16_t y_end);

/********************画圆************************************/
void LCD_DrawCircle(u16 x0,u16 y0,u16 r,u16 color);
void LCD_DrawCircle1(u8 x0,u8 y0,u8 r0,u16 color);

/*******************绘制五角星****************************************/
void LCD_DrawStar(u16 x0,u16 y0,u16 r0,u16 color);
void LCD_DrawFlag(void);

#endif
