#ifndef _____zi_____h______
#define _____zi_____h______
 extern unsigned char zf[];
 extern unsigned char zf_index[];

 extern unsigned char hz[];
 extern unsigned char hz_index[];

 /*****************************************************************************
*****��������  ASCII_NUM()
*****������    ��
*****���ܣ�    ����ASCII�ַ�����
*****���ߣ�    
*****����ʱ�䣺
*****��ע��������������ǽ��KEIL��sizeof()����ͽṹ����ͬһ���ļ���׼ȷʶ���BUGE
*****************************************************************************/  
extern unsigned int ASCII_NUM(void);

 /*****************************************************************************
*****��������  GB16_NUM()
*****������    ��
*****���ܣ�    ����GB16�ַ�����
*****���ߣ�    
*****����ʱ�䣺
*****��ע��������������ǽ��KEIL��sizeof()����ͽṹ����ͬһ���ļ���׼ȷʶ���BUGE
*****************************************************************************/  
extern unsigned int GB16_NUM(void) ;
 #endif