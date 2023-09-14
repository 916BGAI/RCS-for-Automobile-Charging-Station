#ifndef _LCD_H_
#define _LCD_H_

#include <stm32f10x.h>

#define LCD_HEIGHT	320
#define LCD_WIDTH		240

#define PI 3.14159

//LCD��Ҫ������
typedef struct  
{										    
	u16 width;			//LCD ���
	u16 height;			//LCD �߶�
	u16 id;				//LCD ID
	u8  dir;			//���������������ƣ�0��������1��������	
	u8	wramcmd;		//��ʼдgramָ��
	u8  setxcmd;		//����x����ָ��
	u8  setycmd;		//����y����ָ��	 
}_lcd_dev; 	 

//LCD����
extern _lcd_dev lcddev;	//����LCD��Ҫ����  

typedef struct
{
	u16 LCD_REG;
	u16 LCD_RAM;
} LCD_TypeDef;
//ʹ��NOR/SRAM�� Bank1.sector4,��ַλHADDR[27,26]=11 A10��Ϊ�������������� 		   

 //CPU   0~25    
#define LCD_BASE        ((u32)(0x6C000000 | 0x000007FE))  //111 1111 1110 (A0~A10)     2^10=1K
#define LCD             ((LCD_TypeDef *) LCD_BASE)

//ɨ�跽����
#define L2R_U2D  0 //������,���ϵ���
#define L2R_D2U  1 //������,���µ���
#define R2L_U2D  2 //���ҵ���,���ϵ���
#define R2L_D2U  3 //���ҵ���,���µ���

#define U2D_L2R  4 //���ϵ���,������
#define U2D_R2L  5 //���ϵ���,���ҵ���
#define D2U_L2R  6 //���µ���,������
#define D2U_R2L  7 //���µ���,���ҵ���	 

#define DFT_SCAN_DIR  L2R_U2D  //Ĭ�ϵ�ɨ�跽��

//������ɫ
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
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
#define PINK				 0XFE19//�ۺ�ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 

#define LIGHTGREEN     	 0X841F //ǳ��ɫ
//#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)

/****************��ʼ������*************************/
void LCD_Init(void);										//��ʼ��

/****************�����������*************************/
void LCD_DisplayOn(void);								//����ʾ
void LCD_DisplayOff(void);							//����ʾ

/****************ILI9341�ײ��������*************************/
void LCD_WR_DATA(u16 data);
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue);
u16  LCD_ReadReg(u8 LCD_Reg);
void LCD_WriteRAM_Prepare(void);
void LCD_WriteRAM(u16 RGB_Code);		  
void LCD_Scan_Dir(u8 dir);							//������ɨ�跽��
void LCD_Display_Dir(u8 dir);						//������Ļ��ʾ����

/****************LCD��BASE����*************************/
void LCD_SetCursor(u16 Xpos, u16 Ypos);	//���ù��
void LCD_Clear(u16 Color);	 						//����
void LCD_DrawPoint(u16 x,u16 y,u16 c);  //��㺯��

/****************LCD����ʾ�ַ�����*************************/
void  show_xXx(u32 x ,u32 y,u16 backColor,u16 color,const u8 *ch ,u16 size_line,u16 size_bits);
void  Draw_Text_8_16_Str(u32 x,u32 y,u16 backColor,u16 color, char *chs);

/****************LCD����ʾͼƬ����*************************/
void  LCD_Show_Photo(int x0,int y0,int width,int high,const unsigned char photo[]);

/****************������������*************************/
u16 LCD_ReadPoint(u16 x,u16 y);
char *my_itoa_n(int num,char *str,int radix);
void LCD_ShowxNum(u16 x,u16 y,u32 num,u16 BlackColor,u16 Color);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);//����
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);	//������䵥ɫ
void LCD_Address_Set(uint16_t x_start,uint16_t y_start,uint16_t x_end,uint16_t y_end);

/********************��Բ************************************/
void LCD_DrawCircle(u16 x0,u16 y0,u16 r,u16 color);
void LCD_DrawCircle1(u8 x0,u8 y0,u8 r0,u16 color);

/*******************���������****************************************/
void LCD_DrawStar(u16 x0,u16 y0,u16 r0,u16 color);
void LCD_DrawFlag(void);

#endif
