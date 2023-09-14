/************************************************************************************
*****创建：XYD 
*****说明：1.16*16汉字字模数据，横向取模，字节正序；   
          2.如果用LcmZimo这个软件取模 ，选项则为：
             从左到左，从上到下，横向取模左高位，
  	      3. 字模里的字模数据不能重复，也就是说不要有两个及以上相同
             汉字字模数据
	      4.以下汉字数	据必须要和 hz_index[]中的汉字顺序相同，
		    其中也不能有重复的汉字。
		  5.基于以上3 ，4点，使用LcmZimo取模软件取模就会很好，这个软件可以
		    自动过滤相同的汉字。 取模软件会和本程序一起打包，方便用户。
***********************************************************************************/

#include "zi.h"

unsigned char zf_index[]={"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ,.:;!'\"(){}|\\/?<>~@#$%^&*_-+=` "};
unsigned char hz_index[]={"信盈达郑州充电桩功能选择数值消耗时间金额电量正在扣费请等待成功失败刷卡湿温度光照初始化"};

unsigned char hz[]={

0x08,0x40,0x08,0x20,0x0B,0xFE,0x10,0x00,0x10,0x00,0x31,0xFC,0x30,0x00,0x50,0x00,0x91,0xFC,0x10,0x00,0x10,0x00,0x11,0xFC,0x11,0x04,0x11,0x04,0x11,0xFC,0x11,0x04,/*"信",0*/
0x00,0x00,0x3F,0xF0,0x08,0x10,0x0F,0xA0,0x08,0xBC,0x12,0x84,0x11,0x04,0x22,0x94,0x44,0x48,0x00,0x00,0x3F,0xF8,0x24,0x48,0x24,0x48,0x24,0x48,0xFF,0xFE,0x00,0x00,/*"盈",1*/
0x00,0x40,0x20,0x40,0x10,0x40,0x10,0x40,0x07,0xFE,0x00,0x40,0xF0,0x40,0x10,0xA0,0x10,0x90,0x11,0x08,0x12,0x04,0x14,0x04,0x10,0x00,0x28,0x00,0x47,0xFE,0x00,0x00,/*"达",2*/

//	 郑(0) 州(1)

0x22,0x00,0x12,0x3E,0x14,0x22,0x7F,0x24,0x08,0x24,0x08,0x28,0x08,0x24,0xFF,0xA4,
0x08,0x22,0x08,0x22,0x14,0x22,0x12,0x34,0x21,0x28,0x41,0x20,0x80,0x20,0x00,0x20,/*"郑",0*/
/* (16 X 16 , 宋体 )*/

0x10,0x04,0x10,0x84,0x10,0x84,0x10,0x84,0x10,0x84,0x54,0xA4,0x52,0x94,0x52,0x94,
0x90,0x84,0x10,0x84,0x10,0x84,0x10,0x84,0x20,0x84,0x20,0x84,0x40,0x04,0x80,0x04,/*"州",1*/
/* (16 X 16 , 宋体 )*/

0x03,0x00,0x01,0x80,0xFF,0xFF,0x06,0x00,0x06,0x00,0x0C,0x30,0x18,0x18,0x3F,0xFC,
0x06,0x6C,0x06,0x60,0x06,0x60,0x06,0x60,0x0C,0x66,0x0C,0x66,0x18,0x66,0x70,0x3E,/*"充",0*/
/* (16 X 16 , 宋体, 加粗 )*/

0x01,0x80,0x01,0x80,0x01,0x80,0x3F,0xFC,0x31,0x8C,0x31,0x8C,0x31,0x8C,0x3F,0xFC,
0x31,0x8C,0x31,0x8C,0x31,0x8C,0x3F,0xFC,0x31,0x8F,0x01,0x83,0x01,0x83,0x00,0xFF,/*"电",1*/
/* (16 X 16 , 宋体, 加粗 )*/

0x18,0x30,0x18,0x18,0x18,0x18,0x19,0xFF,0xFF,0x80,0x19,0x98,0x39,0x98,0x3D,0x98,
0x7F,0xFE,0x7F,0x98,0xD9,0x98,0x19,0x98,0x19,0x98,0x1B,0x18,0x1B,0xFF,0x1E,0x00,/*"桩",2*/
/* (16 X 16 , 宋体, 加粗 )*/

0x00,0x60,0x00,0x60,0x00,0x60,0xFF,0x60,0x19,0xFE,0x18,0x66,0x18,0x66,0x18,0x66,
0x18,0x66,0x18,0xC6,0x18,0xC6,0x1F,0xC6,0xF9,0x86,0x61,0x86,0x03,0x3C,0x06,0x18,/*"功",3*/
/* (16 X 16 , 宋体, 加粗 )*/

0x18,0x60,0x36,0x66,0x63,0x6C,0xFF,0xF8,0x01,0xE0,0x00,0x63,0x7F,0x63,0x63,0x3F,
0x63,0x00,0x7F,0x66,0x63,0x6C,0x63,0x78,0x7F,0x60,0x63,0x63,0x6F,0x63,0x66,0x3F,/*"能",4*/
/* (16 X 16 , 宋体, 加粗 )*/

0x00,0x60,0x33,0x60,0x1B,0x60,0x1B,0xFC,0x06,0x60,0x00,0x60,0xFF,0xFE,0x19,0xB0,
0x19,0xB0,0x19,0xB0,0x1B,0x36,0x1B,0x36,0x1E,0x1E,0x3C,0x00,0x67,0xFF,0x00,0x00,/*"选",5*/
/* (16 X 16 , 宋体, 加粗 )*/

0x30,0x00,0x37,0xFC,0x33,0x0C,0x31,0x98,0xF8,0xF0,0x30,0x60,0x31,0xF8,0x37,0x6F,
0x38,0x60,0xF3,0xFC,0x30,0x60,0x30,0x60,0x37,0xFE,0x30,0x60,0xF0,0x60,0x60,0x60,/*"择",6*/
/* (16 X 16 , 宋体, 加粗 )*/

0x0C,0x30,0x6D,0xB0,0x3F,0x30,0x0C,0x3F,0xFF,0xE6,0x3F,0x66,0x6D,0xE6,0xCC,0xF6,
0x18,0x3C,0xFF,0x3C,0x33,0x18,0x63,0x18,0x76,0x3C,0x1C,0x3C,0x3E,0x66,0xE3,0xC3,/*"数",7*/
/* (16 X 16 , 宋体, 加粗 )*/

0x0C,0x60,0x0C,0x60,0x0F,0xFE,0x18,0x60,0x18,0x60,0x3B,0xFC,0x3B,0x0C,0x7B,0xFC,
0xDB,0x0C,0x1B,0xFC,0x1B,0x0C,0x1B,0xFC,0x1B,0x0C,0x1B,0x0C,0x1F,0xFF,0x18,0x00,/*"值",8*/
/* (16 X 16 , 宋体, 加粗 )*/

0x00,0x60,0x33,0x6C,0x19,0xEC,0x19,0xF8,0xC0,0x60,0x63,0xFC,0x6F,0x0C,0x0F,0x0C,
0x1B,0xFC,0x1B,0x0C,0xF3,0x0C,0x33,0xFC,0x33,0x0C,0x33,0x0C,0x33,0x3C,0x03,0x18,/*"消",9*/
/* (16 X 16 , 宋体, 加粗 )*/

0x18,0x0C,0x18,0x1E,0x18,0xF0,0xFF,0x30,0x18,0x30,0x7E,0x3E,0x18,0xF0,0xFF,0x30,
0x18,0x30,0x3C,0x3F,0x3F,0xF0,0x7E,0x30,0x78,0x33,0xD8,0x33,0x18,0x33,0x18,0x1F,/*"耗",10*/
/* (16 X 16 , 宋体, 加粗 )*/

0x00,0x0C,0x00,0x0C,0x7E,0x0C,0x66,0x0C,0x67,0xFF,0x66,0x0C,0x66,0x0C,0x7E,0x0C,
0x66,0xCC,0x66,0x6C,0x66,0x6C,0x66,0x0C,0x7E,0x0C,0x66,0x0C,0x00,0x3C,0x00,0x18,/*"时",11*/
/* (16 X 16 , 宋体, 加粗 )*/

0x30,0x00,0x1B,0xFE,0x18,0x06,0x60,0x06,0x67,0xE6,0x66,0x66,0x66,0x66,0x66,0x66,
0x67,0xE6,0x66,0x66,0x66,0x66,0x66,0x66,0x67,0xE6,0x60,0x06,0x60,0x1E,0x60,0x0C,/*"间",12*/
/* (16 X 16 , 宋体, 加粗 )*/

0x01,0x80,0x01,0x80,0x03,0xC0,0x06,0x60,0x0C,0x30,0x18,0x18,0x3F,0xFC,0xE1,0x87,
0x01,0x80,0x3F,0xFC,0x01,0x80,0x19,0x98,0x0D,0x98,0x0D,0xB0,0xFF,0xFF,0x00,0x00,/*"金",13*/
/* (16 X 16 , 宋体, 加粗 )*/

0x18,0x00,0x0C,0xFF,0x7F,0x98,0x61,0xB0,0x18,0x7E,0x1F,0x66,0x33,0x7E,0x7E,0x7E,
0x0C,0x7E,0x1E,0x7E,0x33,0x7E,0x7F,0xFE,0xF3,0x3C,0x33,0x36,0x3F,0x63,0x33,0xC3,/*"额",14*/
/* (16 X 16 , 宋体, 加粗 )*/

0x01,0x80,0x01,0x80,0x01,0x80,0x3F,0xFC,0x31,0x8C,0x31,0x8C,0x31,0x8C,0x3F,0xFC,
0x31,0x8C,0x31,0x8C,0x31,0x8C,0x3F,0xFC,0x31,0x8F,0x01,0x83,0x01,0x83,0x00,0xFF,/*"电",15*/
/* (16 X 16 , 宋体, 加粗 )*/

0x00,0x00,0x1F,0xF8,0x18,0x18,0x1F,0xF8,0x18,0x18,0xFF,0xFF,0x00,0x00,0x1F,0xF8,
0x19,0x98,0x1F,0xF8,0x19,0x98,0x1F,0xF8,0x01,0x80,0x1F,0xF8,0x01,0x80,0x7F,0xFE,/*"量",16*/
/* (16 X 16 , 宋体, 加粗 )*/

0x00,0x00,0x7F,0xFE,0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,0x19,0x80,0x19,0xFC,
0x19,0x80,0x19,0x80,0x19,0x80,0x19,0x80,0x19,0x80,0x19,0x80,0xFF,0xFF,0x00,0x00,/*"正",17*/
/* (16 X 16 , 宋体, 加粗 )*/

0x03,0x00,0x03,0x00,0x06,0x00,0xFF,0xFF,0x0C,0x00,0x0C,0x60,0x18,0x60,0x38,0x60,
0x7F,0xFE,0xD8,0x60,0x18,0x60,0x18,0x60,0x18,0x60,0x18,0x60,0x1F,0xFF,0x18,0x00,/*"在",18*/
/* (16 X 16 , 宋体, 加粗 )*/

0x18,0x00,0x18,0x00,0x19,0xFE,0x19,0x86,0xFF,0x86,0x19,0x86,0x19,0x86,0x1F,0x86,
0x1D,0x86,0x39,0x86,0xF9,0x86,0x19,0x86,0x19,0x86,0x19,0xFE,0x79,0x86,0x30,0x00,/*"扣",19*/
/* (16 X 16 , 宋体, 加粗 )*/

0x06,0x60,0x7F,0xFC,0x06,0x6C,0x3F,0xFC,0x36,0x60,0x3F,0xFE,0x0C,0x66,0x18,0x7E,
0x30,0x0C,0x7F,0xF8,0x18,0x18,0x19,0x98,0x19,0x98,0x03,0xE0,0x0E,0x38,0x38,0x0C,/*"费",20*/
/* (16 X 16 , 宋体, 加粗 )*/

0x00,0x60,0x60,0x60,0x37,0xFE,0x30,0x60,0x03,0xFC,0x00,0x60,0xF7,0xFF,0x30,0x00,
0x33,0xFC,0x33,0x0C,0x33,0xFC,0x33,0x0C,0x3F,0xFC,0x3B,0x0C,0x33,0x3C,0x03,0x18,/*"请",21*/
/* (16 X 16 , 宋体, 加粗 )*/

0x30,0x60,0x3F,0xFF,0x6C,0xD8,0xC7,0x8C,0x01,0x80,0x3F,0xFC,0x01,0x80,0x01,0x80,
0xFF,0xFF,0x00,0x00,0x00,0x30,0x7F,0xFE,0x0C,0x30,0x06,0x30,0x06,0xF0,0x00,0x60,/*"等",22*/
/* (16 X 16 , 宋体, 加粗 )*/

0x0C,0x60,0x0C,0x60,0x18,0x60,0x33,0xFE,0x6C,0x60,0x0C,0x60,0x1F,0xFF,0x38,0x18,
0x78,0x18,0xDF,0xFF,0x18,0x18,0x1B,0x18,0x19,0x98,0x19,0x98,0x18,0x78,0x18,0x30,/*"待",23*/
/* (16 X 16 , 宋体, 加粗 )*/

0x00,0x78,0x00,0x6C,0x00,0x60,0x3F,0xFF,0x30,0x60,0x30,0x60,0x30,0x66,0x3F,0x66,
0x33,0x66,0x33,0x3C,0x33,0x3C,0x33,0x1B,0x3F,0x3B,0x66,0x6F,0x60,0xC7,0xC1,0x83,/*"成",24*/
/* (16 X 16 , 宋体, 加粗 )*/

0x00,0x60,0x00,0x60,0x00,0x60,0xFF,0x60,0x19,0xFE,0x18,0x66,0x18,0x66,0x18,0x66,
0x18,0x66,0x18,0xC6,0x18,0xC6,0x1F,0xC6,0xF9,0x86,0x61,0x86,0x03,0x3C,0x06,0x18,/*"功",25*/
/* (16 X 16 , 宋体, 加粗 )*/

0x01,0x80,0x19,0x80,0x19,0x80,0x19,0x80,0x3F,0xFC,0x31,0x80,0x61,0x80,0x01,0x80,
0xFF,0xFF,0x03,0xC0,0x06,0x60,0x06,0x60,0x0C,0x30,0x18,0x18,0x30,0x0C,0xE0,0x07,/*"失",26*/
/* (16 X 16 , 宋体, 加粗 )*/

0x00,0x60,0x7E,0x60,0x66,0x60,0x7E,0xC0,0x7E,0xFF,0x7F,0x8C,0x7F,0xCC,0x7E,0xCC,
0x7E,0xCC,0x7E,0x78,0x7E,0x78,0x18,0x30,0x3C,0x78,0x36,0xCC,0x67,0x86,0xC3,0x03,/*"败",27*/
/* (16 X 16 , 宋体, 加粗 )*/


0x00,0x03,0x3F,0xE3,0x30,0x63,0x30,0x63,0x3F,0xFB,0x33,0x1B,0x33,0x1B,0x33,0x1B,
0x3F,0xFB,0x7B,0x7B,0x7B,0x7B,0x7B,0x7B,0xDB,0xE3,0x1B,0xC3,0x03,0x0F,0x03,0x06,/*"刷",0*/
/* (16 X 16 , 宋体, 加粗 )*/

0x03,0x00,0x03,0x00,0x03,0xFC,0x03,0x00,0x03,0x00,0x03,0x00,0xFF,0xFF,0x03,0x00,
0x03,0x00,0x03,0x60,0x03,0x30,0x03,0x18,0x03,0x0C,0x03,0x00,0x03,0x00,0x03,0x00,/*"卡",1*/
/* (16 X 16 , 宋体, 加粗 )*/

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x67,0xF8,0x34,0x08,0x47,0xF8,0x27,0xF8,
0x01,0x20,0x2D,0x24,0x27,0x38,0x41,0x20,0x4F,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,/*"湿"*/

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x67,0xF8,0x34,0x08,0x47,0xF8,0x27,0xF8,
0x00,0x00,0x27,0xF8,0x25,0x28,0x45,0x28,0x5F,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,/*"温"*/

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x80,0x3F,0xFC,0x24,0x20,0x3F,0xF8,
0x27,0xE0,0x2F,0xF0,0x26,0x20,0x41,0xC0,0x9E,0x3C,0x00,0x00,0x00,0x00,0x00,0x00,/*"度"*/

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x62,0x18,0x1A,0x60,0x02,0x00,
0xFF,0xFC,0x08,0x80,0x08,0x84,0x30,0x84,0xC0,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,/*"光"*/

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7B,0xF8,0x48,0x88,0x49,0xF8,0x7B,0x00,
0x49,0xFC,0x49,0x04,0x79,0xFC,0x00,0x10,0x68,0x88,0xC4,0x44,0x00,0x00,0x00,0x00,/*"照"*/

0x30,0x00,0x18,0x00,0x01,0xFE,0xFE,0x66,0x0C,0x66,0x18,0x66,0x18,0x66,0x3E,0x66,
0x7C,0x66,0xDE,0x66,0x1E,0x66,0x18,0xC6,0x18,0xC6,0x19,0x86,0x1B,0x3C,0x1E,0x18,/*"初",0*/

0x18,0x30,0x18,0x30,0x18,0x30,0x18,0x60,0xFE,0x6C,0x36,0xC6,0x37,0xFF,0x36,0xC3,
0x36,0x00,0x6C,0xFE,0x3C,0xC6,0x18,0xC6,0x3C,0xC6,0x66,0xC6,0xC0,0xFE,0x00,0xC6,/*"始",1*/

0x0C,0xC0,0x0C,0xC0,0x0C,0xC6,0x18,0xCC,0x18,0xD8,0x38,0xF0,0x38,0xE0,0x78,0xC0,
0xD9,0xC0,0x1B,0xC0,0x1E,0xC0,0x18,0xC3,0x18,0xC3,0x18,0xC3,0x18,0x7F,0x18,0x00,/*"化",2*/

} ;

/************************************************************************************
*****创建：XYD 
*****说明：1.8*16字符字模数据，横向取模，字节正序；   
           2.如果用LcmZimo这个软件取模 ，选项则为：
            从左到左，从上到下，横向取模左高位，
	       3.字模里的字模数据不能重复，也就是说不要有两个及以上相同
            汉字字模数据
	       4.以下汉字数据必须要和 zf_index[]中的汉字顺序相同，
		    其中也不能有重复的汉字。
		  5.基于以上3 ，4点，使用LcmZimo取模软件取模就会很好，这个软件可以
		    自动过滤相同的汉字。 取模软件会和本程序一起打包，方便用户。
***********************************************************************************/
unsigned char zf[]=
{
 /*	 //横向取模，字节正序
   0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ,.:;!'"(){}|\/?<>~@#$%^&*_-+=`
*/
		/*--  文字:  0  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x18,0x24,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x24,0x18,0x00,0x00,
		
		/*--  文字:  1  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x10,0x70,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00,
		
		/*--  文字:  2  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x04,0x04,0x08,0x10,0x20,0x42,0x7E,0x00,0x00,
		
		/*--  文字:  3  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x3C,0x42,0x42,0x04,0x18,0x04,0x02,0x02,0x42,0x44,0x38,0x00,0x00,
		
		/*--  文字:  4  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x04,0x0C,0x14,0x24,0x24,0x44,0x44,0x7E,0x04,0x04,0x1E,0x00,0x00,
		
		/*--  文字:  5  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x7E,0x40,0x40,0x40,0x58,0x64,0x02,0x02,0x42,0x44,0x38,0x00,0x00,
		
		/*--  文字:  6  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x1C,0x24,0x40,0x40,0x58,0x64,0x42,0x42,0x42,0x24,0x18,0x00,0x00,
		
		/*--  文字:  7  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x7E,0x44,0x44,0x08,0x08,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00,
		
		/*--  文字:  8  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x24,0x18,0x24,0x42,0x42,0x42,0x3C,0x00,0x00,
		
		/*--  文字:  9  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x18,0x24,0x42,0x42,0x42,0x26,0x1A,0x02,0x02,0x24,0x38,0x00,0x00,
		
		/*--  文字:  a  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x42,0x1E,0x22,0x42,0x42,0x3F,0x00,0x00,
		
		/*--  文字:  b  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0xC0,0x40,0x40,0x40,0x58,0x64,0x42,0x42,0x42,0x64,0x58,0x00,0x00,
		
		/*--  文字:  c  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0x22,0x40,0x40,0x40,0x22,0x1C,0x00,0x00,
		
		/*--  文字:  d  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x06,0x02,0x02,0x02,0x1E,0x22,0x42,0x42,0x42,0x26,0x1B,0x00,0x00,
		
		/*--  文字:  e  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x42,0x7E,0x40,0x40,0x42,0x3C,0x00,0x00,
		
		/*--  文字:  f  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x0F,0x11,0x10,0x10,0x7E,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00,
		
		/*--  文字:  g  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3E,0x44,0x44,0x38,0x40,0x3C,0x42,0x42,0x3C,
		
		/*--  文字:  h  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0xC0,0x40,0x40,0x40,0x5C,0x62,0x42,0x42,0x42,0x42,0xE7,0x00,0x00,
		
		/*--  文字:  i  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x70,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00,
		
		/*--  文字:  j  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x0C,0x0C,0x00,0x00,0x1C,0x04,0x04,0x04,0x04,0x04,0x04,0x44,0x78,
		
		/*--  文字:  k  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0xC0,0x40,0x40,0x40,0x4E,0x48,0x50,0x68,0x48,0x44,0xEE,0x00,0x00,
		
		/*--  文字:  l  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x70,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00,
		
		/*--  文字:  m  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x49,0x49,0x49,0x49,0x49,0xED,0x00,0x00,
		
		/*--  文字:  n  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xDC,0x62,0x42,0x42,0x42,0x42,0xE7,0x00,0x00,
		
		/*--  文字:  o  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x42,0x42,0x3C,0x00,0x00,
		
		/*--  文字:  p  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD8,0x64,0x42,0x42,0x42,0x44,0x78,0x40,0xE0,
		
		/*--  文字:  q  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1E,0x22,0x42,0x42,0x42,0x22,0x1E,0x02,0x07,
		
		/*--  文字:  r  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xEE,0x32,0x20,0x20,0x20,0x20,0xF8,0x00,0x00,
		
		/*--  文字:  s  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3E,0x42,0x40,0x3C,0x02,0x42,0x7C,0x00,0x00,
		
		/*--  文字:  t  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x00,0x00,0x10,0x10,0x7C,0x10,0x10,0x10,0x10,0x10,0x0C,0x00,0x00,
		
		/*--  文字:  u  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC6,0x42,0x42,0x42,0x42,0x46,0x3B,0x00,0x00,
		
		/*--  文字:  v  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE7,0x42,0x24,0x24,0x28,0x10,0x10,0x00,0x00,
		
		/*--  文字:  w  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xD7,0x92,0x92,0xAA,0xAA,0x44,0x44,0x00,0x00,
		
		/*--  文字:  x  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x6E,0x24,0x18,0x18,0x18,0x24,0x76,0x00,0x00,
		
		/*--  文字:  y  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE7,0x42,0x24,0x24,0x28,0x18,0x10,0x10,0xE0,
		
		/*--  文字:  z  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x44,0x08,0x10,0x10,0x22,0x7E,0x00,0x00,
		
		/*--  文字:  A  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x10,0x10,0x18,0x28,0x28,0x24,0x3C,0x44,0x42,0x42,0xE7,0x00,0x00,
		
		/*--  文字:  B  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0xF8,0x44,0x44,0x44,0x78,0x44,0x42,0x42,0x42,0x44,0xF8,0x00,0x00,
		
		/*--  文字:  C  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x3E,0x42,0x42,0x80,0x80,0x80,0x80,0x80,0x42,0x44,0x38,0x00,0x00,
		
		/*--  文字:  D  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0xF8,0x44,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x44,0xF8,0x00,0x00,
		
		/*--  文字:  E  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0xFC,0x42,0x48,0x48,0x78,0x48,0x48,0x40,0x42,0x42,0xFC,0x00,0x00,
		
		/*--  文字:  F  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0xFC,0x42,0x48,0x48,0x78,0x48,0x48,0x40,0x40,0x40,0xE0,0x00,0x00,
		
		/*--  文字:  G  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x3C,0x44,0x44,0x80,0x80,0x80,0x8E,0x84,0x44,0x44,0x38,0x00,0x00,
		
		/*--  文字:  H  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0xE7,0x42,0x42,0x42,0x42,0x7E,0x42,0x42,0x42,0x42,0xE7,0x00,0x00,
		
		/*--  文字:  I  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x7C,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00,
		
		/*--  文字:  J  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x3E,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x88,0xF0,
		
		/*--  文字:  K  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0xEE,0x44,0x48,0x50,0x70,0x50,0x48,0x48,0x44,0x44,0xEE,0x00,0x00,
		
		/*--  文字:  L  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0xE0,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x42,0xFE,0x00,0x00,
		
		/*--  文字:  M  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0xEE,0x6C,0x6C,0x6C,0x6C,0x54,0x54,0x54,0x54,0x54,0xD6,0x00,0x00,
		
		/*--  文字:  N  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0xC7,0x62,0x62,0x52,0x52,0x4A,0x4A,0x4A,0x46,0x46,0xE2,0x00,0x00,
		
		/*--  文字:  O  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x38,0x44,0x82,0x82,0x82,0x82,0x82,0x82,0x82,0x44,0x38,0x00,0x00,
		
		/*--  文字:  P  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0xFC,0x42,0x42,0x42,0x42,0x7C,0x40,0x40,0x40,0x40,0xE0,0x00,0x00,
		
		/*--  文字:  Q  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x38,0x44,0x82,0x82,0x82,0x82,0x82,0xB2,0xCA,0x4C,0x38,0x06,0x00,
		
		/*--  文字:  R  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0xFC,0x42,0x42,0x42,0x7C,0x48,0x48,0x44,0x44,0x42,0xE3,0x00,0x00,
		
		/*--  文字:  S  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x3E,0x42,0x42,0x40,0x20,0x18,0x04,0x02,0x42,0x42,0x7C,0x00,0x00,
		
		/*--  文字:  T  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0xFE,0x92,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x38,0x00,0x00,
		
		/*--  文字:  U  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0xE7,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x3C,0x00,0x00,
		
		/*--  文字:  V  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0xE7,0x42,0x42,0x44,0x24,0x24,0x28,0x28,0x18,0x10,0x10,0x00,0x00,
		
		/*--  文字:  W  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0xD6,0x92,0x92,0x92,0x92,0xAA,0xAA,0x6C,0x44,0x44,0x44,0x00,0x00,
		
		/*--  文字:  X  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0xE7,0x42,0x24,0x24,0x18,0x18,0x18,0x24,0x24,0x42,0xE7,0x00,0x00,
		
		/*--  文字:  Y  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0xEE,0x44,0x44,0x28,0x28,0x10,0x10,0x10,0x10,0x10,0x38,0x00,0x00,
		
		/*--  文字:  Z  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x7E,0x84,0x04,0x08,0x08,0x10,0x20,0x20,0x42,0x42,0xFC,0x00,0x00,
		
		/*--  文字:  ,  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x60,0x20,0xC0,
		
		/*--  文字:  .  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x60,0x00,0x00,
		
		/*--  文字:  :  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,
		
		/*--  文字:  ;  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x10,0x10,0x20,
		
		/*--  文字:  !  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00,0x18,0x18,0x00,0x00,
		
		/*--  文字:  '  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x60,0x60,0x20,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		
		/*--  文字:  "  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x12,0x36,0x24,0x48,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		
		/*--  文字:  (  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x02,0x04,0x08,0x08,0x10,0x10,0x10,0x10,0x10,0x10,0x08,0x08,0x04,0x02,0x00,
		
		/*--  文字:  )  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x40,0x20,0x10,0x10,0x08,0x08,0x08,0x08,0x08,0x08,0x10,0x10,0x20,0x40,0x00,
		
		/*--  文字:  {  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x03,0x04,0x04,0x04,0x04,0x04,0x08,0x04,0x04,0x04,0x04,0x04,0x04,0x03,0x00,
		
		/*--  文字:  }  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x60,0x10,0x10,0x10,0x10,0x10,0x08,0x10,0x10,0x10,0x10,0x10,0x10,0x60,0x00,
		
		/*--  文字:  |  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,
		
		/*--  文字:  \  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x40,0x40,0x20,0x20,0x10,0x10,0x10,0x08,0x08,0x04,0x04,0x04,0x02,0x02,
		
		/*--  文字:  /  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x01,0x02,0x02,0x04,0x04,0x08,0x08,0x10,0x10,0x20,0x20,0x40,0x40,0x00,
		
		/*--  文字:  ?  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x3C,0x42,0x42,0x62,0x02,0x04,0x08,0x08,0x00,0x18,0x18,0x00,0x00,
		
		/*--  文字:  <  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x02,0x04,0x08,0x10,0x20,0x40,0x20,0x10,0x08,0x04,0x02,0x00,0x00,
		
		/*--  文字:  >  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x40,0x20,0x10,0x08,0x04,0x02,0x04,0x08,0x10,0x20,0x40,0x00,0x00,
		
		/*--  文字:  ~  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x30,0x4C,0x43,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		
		/*--  文字:  @  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x38,0x44,0x5A,0xAA,0xAA,0xAA,0xAA,0xB4,0x42,0x44,0x38,0x00,0x00,
		
		/*--  文字:  #  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x24,0x24,0x24,0xFE,0x48,0x48,0x48,0xFE,0x48,0x48,0x48,0x00,0x00,
		
		/*--  文字:  $  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x10,0x38,0x54,0x54,0x50,0x30,0x18,0x14,0x14,0x54,0x54,0x38,0x10,0x10,
		
		/*--  文字:  %  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x44,0xA4,0xA8,0xA8,0xA8,0x54,0x1A,0x2A,0x2A,0x2A,0x44,0x00,0x00,
		
		/*--  文字:  ^  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x1C,0x22,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		
		/*--  文字:  &  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x30,0x48,0x48,0x48,0x50,0x6E,0xA4,0x94,0x88,0x89,0x76,0x00,0x00,
		
		/*--  文字:  *  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x00,0x10,0x10,0xD6,0x38,0x38,0xD6,0x10,0x10,0x00,0x00,0x00,0x00,
		
		/*--  文字:  _  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,
		
		/*--  文字:  -  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		
		/*--  文字:  +  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x00,0x10,0x10,0x10,0x10,0xFE,0x10,0x10,0x10,0x10,0x00,0x00,0x00,
		
		/*--  文字:  =  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x00,0x00,0x00,0xFE,0x00,0x00,0x00,0x00,0x00,
		
		/*--  文字:  `  --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x60,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 
				/*--  文字:  空格 --*/
		/*--  宋体12;  此字体下对应的点阵为：宽x高=8x16   --*/
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 
 };
//

/*****************************************************************************
*****函数名：  ASCII_NUM()
*****参数：    无
*****功能：    返回ASCII字符个数
*****作者：    
*****创建时间：
*****备注：创建这个函数是解决KEIL中sizeof()必须和结构变量同一个文件才准确识别的BUGE
*****************************************************************************/  
unsigned int ASCII_NUM(void)
{
  unsigned int ASCI_NUM;
  unsigned char *PT;
  PT = zf_index;
  while(*PT++ != '\0')
  {
  	 ASCI_NUM++;
  }
  return (ASCI_NUM-1);
}  

/*****************************************************************************
*****函数名：  GB16_NUM()
*****参数：    无
*****功能：    返回GB16字库里汉字个数
*****作者：   
*****创建时间：
*****备注：创建这个函数是解决KEIL中sizeof()必须和结构变量同一个文件才准确识别的BUGE
*****************************************************************************/  
unsigned int GB16_NUM(void)
{
  unsigned int HZ_NUM;
  unsigned char *PT;
  PT = hz_index;

  while(*PT++ != '\0')
  {
  	HZ_NUM++;
  }

  return HZ_NUM/2;
} 
