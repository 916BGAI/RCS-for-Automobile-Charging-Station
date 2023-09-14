#include <string.h>
#include <stdio.h>
#include "iccard.h"

char IC_Num[80] ; //��ſ���
u8 Get_ID_Card[4]={0};
u8 Last_ID_Card[4]={0};
u8 Default_Key[6] = {0xff,0xff,0xff,0xff,0xff,0xff};

/**************************************************************************************
* �������ƣ�Card_GetNumber	
* ������������ȡIC����
* ��ڲ�����-mode��Ѱ��ģʽ
*				   PICC_REQIDL��Ѱ��������δ��������״̬
*          PICC_REQALL��Ѱ��������ȫ����  
* ���ڲ�����-pNum�����濨�ŵĻ������׵�ַ,4�ֽڿ���
* �� �� ֵ��0���ɹ�,  -1��ʧ��
* ˵    ������
***************************************************************************************/ 
int Card_GetNumber(u8 mode, u8 *pNum)
{	
	u8 tempbuf[10];
	u8 count=0;
	char status;
	/*Ѱ��*/
	while(1)
	{			
		status = PCD_Request(mode, tempbuf);//Ӧ��λ		
		if(status != PCD_OK)
	{    
			PCD_Reset(); 
			count++;
			if(count >= 3)
				return -1;	
			continue;
   	}
		break;	
	}
						
	/*����ͻ,���ؿ���*/
	status = PCD_Anticoll(pNum);
	if(status != PCD_OK)
		return -1;
	
	/*ѡ��*/
	status = PCD_Select(pNum);
	if(status != PCD_OK)
		return -1;
	return 0;		
}

/**************************************************************************************
* �������ƣ�Card_VerifyKeyA	
* ������������֤IC������A
* ��ڲ�����- pNum�����濨�ŵĻ������׵�ַ,4�ֽڿ���
*						-SectorNum��IC��������
*           -pKey����������Ļ������׵�ַ,6�ֽ�����
* ���ڲ�������
* �� �� ֵ��0���ɹ�,  -1��ʧ��
* ˵    ������
***************************************************************************************/ 
int Card_VerifyKeyA(u8* pNum, u8 SectorNum, u8 *pKey)
{
	char status;
	status = PCD_AuthState(PICC_AUTHENT1A, SectorNum, pKey, pNum);   
	if(status != PCD_OK)
		return -1;
	return 0;										
}

/***************************************************************************************
* �������ƣ�Card_VerifyKeyB	
* ������������֤IC������B
* ��ڲ�����- pNum�����濨�ŵĻ������׵�ַ,4�ֽڿ���
*			-BlockNum��IC��������
*           -pKey����������Ļ������׵�ַ,6�ֽ�����
* ���ڲ�������
* �� �� ֵ��0���ɹ�,  -1��ʧ��
* ˵    ������
***************************************************************************************/ 
int Card_VerifyKeyB(u8* pNum, u8 SectorNum, u8 *pKey)
{
	char status;

	status = PCD_AuthState(PICC_AUTHENT1B, SectorNum, pKey, pNum);   
	if(status != PCD_OK)
		return -1;
	return 0;										
}

/**************************************************************************************
* �������ƣ�Card_ReadBlock	
* ������������ȡIC��ָ���������
* ��ڲ�����-BlockNum��IC�����
* ���ڲ�����-pData����ȡ������,16�ֽ�����
* �� �� ֵ��0���ɹ�,  -1��ʧ��
* ˵    ������������֤������ܶ�
***************************************************************************************/ 
int Card_ReadBlock(u8 BlockNum, u8 *pData)
{
	char status;

	status = PCD_ReadBlock(BlockNum, pData);						
	if(status != PCD_OK)
		return -1;
	return 0;										
}

/**************************************************************************************
* �������ƣ�Card_WriteBlock	
* ����������������д��IC��ָ����
* ��ڲ�����-BlockNum��IC�����
* ���ڲ�����-pData��д�������,16�ֽ�����
* �� �� ֵ��0���ɹ�,  -1��ʧ��
* ˵    ������������֤�������д
***************************************************************************************/ 
int Card_WriteBlock(u8 BlockNum, u8 *pData)
{
	char status;
	status = PCD_WriteBlock(BlockNum, pData);
	if(status != PCD_OK)
		return -1;
	return 0;												
}

/**************************************************************************************
* �������ƣ�Card_PurseInit	
* ��������������Ǯ����ʼ��
* ��ڲ�����-BlockNum��IC�����
* ���ڲ�����-ValueBuf���������Ǯ��ֵ������,4�ֽ���ֵ
* �� �� ֵ��0���ɹ�,  -1��ʧ��
* ˵    ������������֤�������ʹ��
*           ValueBuf[0]<->��ֵ����ֽ�,ValueBuf[3]<->��ֵ����ֽ�
***************************************************************************************/ 
int Card_PurseInit(u8 BlockNum, u8 ValueBuf[4])
{
 	char status;	
	u8 DataBuf[16];
	
	DataBuf[0] = DataBuf[8] =  ValueBuf[0];
	DataBuf[1] = DataBuf[9] =  ValueBuf[1];
	DataBuf[2] = DataBuf[10] =  ValueBuf[2];
	DataBuf[3] = DataBuf[11] =  ValueBuf[3];
	DataBuf[4] = ValueBuf[0] ^ 0xFF;
	DataBuf[5] = ValueBuf[1] ^ 0xFF;
	DataBuf[6] = ValueBuf[2] ^ 0xFF;
	DataBuf[7] = ValueBuf[3] ^ 0xFF;
	DataBuf[12] = DataBuf[14] = BlockNum;	 
	DataBuf[13] = DataBuf[15] = BlockNum ^ 0xFF;	 
	
	status = PCD_WriteBlock(BlockNum, DataBuf);
	if(status != PCD_OK)
		return -1;
	return 0;												
}

/**************************************************************************************
* �������ƣ�Card_PurseGet	
* ������������ȡ����Ǯ����ֵ
* ��ڲ�����-BlockNum��IC�����
* ���ڲ�����-ValueBuf���������Ǯ��ֵ������,4�ֽ���ֵ
* �� �� ֵ��0���ɹ�,  -1��ʧ��
* ˵    ������������֤�������ʹ��
*           ValueBuf[0]<->��ֵ����ֽ�,ValueBuf[3]<->��ֵ����ֽ�
***************************************************************************************/ 
int Card_PurseGet(u8 BlockNum, u8 ValueBuf[4])
{
	char status;	
	u8 DataBuf[16];
	
	status = Card_ReadBlock(BlockNum, DataBuf);		
	if(status != PCD_OK)
		return -1;

	ValueBuf[0] =  DataBuf[0];
	ValueBuf[1] =  DataBuf[1];
	ValueBuf[2] =  DataBuf[2];
	ValueBuf[3] =  DataBuf[3];	 	

	return 0;														
}

/**************************************************************************************
* �������ƣ�Card_PurseIncrement	
* ��������������Ǯ����ֵ
* ��ڲ�����-BlockNum��IC�����
* ���ڲ�����-ValueBuf���������Ǯ����ֵ������,4�ֽڷֱ�����ָ����ֵ
* �� �� ֵ��0���ɹ�,  -1��ʧ��
* ˵    ������������֤�������ʹ��
*           ValueBuf[0]����0xff��,����1��ValueBuf[1]��1,�������ơ�
*        	��ֵʱValueBuf[3]���ֵ0x7f,��Ϊ��ֵ�ɱ�ʾ������
***************************************************************************************/ 
int Card_PurseIncrement(u8 BlockNum, u8 ValueBuf[4])
{
	char status;	
	status = PCD_Value(PICC_INCREMENT, BlockNum, ValueBuf);   

	if(status != PCD_OK)
		return -1;
	return 0;
}

/**************************************************************************************
* �������ƣ�Card_PurseDecrement	
* ��������������Ǯ����ֵ
* ��ڲ�����-BlockNum��IC�����
* ���ڲ�����-ValueBuf���������Ǯ����ֵ������,4�ֽڷֱ��ȥָ����ֵ
* �� �� ֵ��0���ɹ�,  -1��ʧ��
* ˵    ������������֤�������ʹ��
*           ValueBuf[0]����0��,�ټ�1��ValueBuf[1]��1,�������ơ�
***************************************************************************************/ 
int Card_PurseDecrement(u8 BlockNum, u8 ValueBuf[4])
{
	char status;	
	status = PCD_Value(PICC_DECREMENT, BlockNum, ValueBuf);   

	if(status != PCD_OK)
		return -1;
	return 0;
}

/**************************************************************************************
* �������ƣ�Card_ModifyPasswordA	
* �����������޸ĵ�ǰ��������A
* ��ڲ�����-SectorNum��������
*			-pOldKey��������,6�ֽ�����
*           -pNewKey��������,6�ֽ�����
* ���ڲ�������
* �� �� ֵ��0���ɹ�,  -1������ԭ��ʧ��,  -2�������ų���,  -3���������������
* ˵    ������������֤�������ʹ��
***************************************************************************************/ 
int Card_ModifyPasswordA(u8 SectorNum, u8 *pOldKey, u8 *pNewKye)
{
	char status, i;	
	u8 DataBuf[16];

	/*�������������ַ,���������Ų�������*/
	if(SectorNum % MF1_SectorSize != 0)	
		return -2;	

	/*��ȡ��ǰ���Ŀ���*/
	status = Card_GetNumber(PICC_REQIDL, DataBuf);
	if(status != 0)
		return -1;

	/*������A��֤,��������벻��ȷ���ؾ������������*/
	status = Card_VerifyKeyA(DataBuf, SectorNum, pOldKey);
	if(status != 0)
		return -3;

	/*��ȡ��������3����*/
	status = Card_ReadBlock(SectorNum + 3, DataBuf);
	if(status != 0)
		return -1;

	/*д�뱾������3����,�޸�����A*/
	for(i=0; i<6; i++)
	{
		DataBuf[i] = pNewKye[i];
	}
	status = Card_WriteBlock(SectorNum + 3, DataBuf);
   	if(status != 0)
		return -1;
	
	return 0;
}

/***************************************************************************************
* �������ƣ�Card_ModifyPasswordB	
* �����������޸ĵ�ǰ��������B
* ��ڲ�����-SectorNum��������
*           -pKeyA������A,6�ֽ�����
*			-pOldKeyB��������B,6�ֽ�����
*           -pNewKeyB��������B,6�ֽ�����
* ���ڲ�������
* �� �� ֵ��0:�ɹ�, -1:����ԭ��ʧ��, -2:�����ų���, -3:������B����, -4:������A����
* ˵    ������������֤�������ʹ��
***************************************************************************************/ 
int Card_ModifyPasswordB(u8 SectorNum, u8 *pKeyA, u8 *pOldKeyB, u8 *pNewKyeB)
{
	char status, i;	
	u8 DataBuf[16];

	/*�������������ַ,���������Ų�������*/
	if(SectorNum % MF1_SectorSize != 0)	
		return -2;	

	/*��ȡ��ǰ���Ŀ���*/
	status = Card_GetNumber(PICC_REQIDL, DataBuf);
	if(status != 0)
		return -1;

	/*������B��֤,��������벻��ȷ���ؾ������������*/
	status = Card_VerifyKeyB(DataBuf, SectorNum, pOldKeyB);
	if(status != 0)
		return -3;

	/*������A��֤,��������벻��ȷ���ؾ������������*/
	status = Card_VerifyKeyA(DataBuf, SectorNum, pKeyA);
	if(status != 0)
		return -4;

	/*��ȡ��������3����*/
	status = Card_ReadBlock(SectorNum + 3, DataBuf);
	if(status != 0)
		return -1;

	/*д�뱾������3����,�޸�����B*/
	for(i=0; i<6; i++)
	{
		DataBuf[i] = pKeyA[i];
	}
	for(i=0; i<6; i++)
	{
		DataBuf[10 + i] = pNewKyeB[i];		
	}
	status = Card_WriteBlock(SectorNum + 3, DataBuf);
   	if(status != 0)
		return -1;
	
	return 0;
}

/***************************************************************************************
* �������ƣ�Card_PurseInit_Int
* ��������������Ǯ����ʼ��һ��������ֵ
* ��ڲ�����-BlockNum�����
*			-Value����ʼ������ֵ
* ���ڲ�������
* �� �� ֵ��0���ɹ�,  -1��ʧ��
* ˵    ������������֤�������ʹ��
***************************************************************************************/ 
int Card_PurseInit_Int(u8 BlockNum, s32 Value)
{
	char status;
	u8 ValueBuf[4];
	u8 *pTemp = (u8 *)(&Value);
	
	ValueBuf[0] = pTemp[0];
	ValueBuf[1] = pTemp[1];
	ValueBuf[2] = pTemp[2];
	ValueBuf[3] = pTemp[3];

	status = Card_PurseInit(BlockNum, ValueBuf);
   	if(status != 0)
		return -1;
	
	return 0;
}

/***************************************************************************************
* �������ƣ�Card_PurseGet_Int
* ������������ȡ����Ǯ����������ֵ
* ��ڲ�����-BlockNum�����			
* ���ڲ�����-pValue�������ȡ����ֵ�ĵ�ַ
* �� �� ֵ��0���ɹ�,  -1��ʧ��
* ˵    ������������֤�������ʹ��
***************************************************************************************/ 
int Card_PurseGet_Int(u8 BlockNum, s32 *pValue)
{
	char status;
	u8 ValueBuf[4];
	u8 *pTemp = (u8 *)pValue;

	status = Card_PurseGet(BlockNum, ValueBuf);
	if(status != 0)
		return -1;

	pTemp[0] = ValueBuf[0];
	pTemp[1] = ValueBuf[1];
	pTemp[2] = ValueBuf[2];
	pTemp[3] = ValueBuf[3];

	return 0;
}

/***************************************************************************************
* �������ƣ�Card_PurseInit_Float
* ��������������Ǯ����ʼ��һ��������ֵ
* ��ڲ�����-BlockNum�����
*			-Value����ʼ������ֵ
* ���ڲ�������
* �� �� ֵ��0���ɹ�,  -1��ʧ��
* ˵    ������������֤�������ʹ��
***************************************************************************************/ 
int Card_PurseInit_Float(u8 BlockNum, float Value)
{
	char status;
	u8 ValueBuf[4];
	u8 *pTemp = (u8 *)(&Value);
	
	ValueBuf[0] = pTemp[0];
	ValueBuf[1] = pTemp[1];
	ValueBuf[2] = pTemp[2];
	ValueBuf[3] = pTemp[3];

	status = Card_PurseInit(BlockNum, ValueBuf);
   	if(status != 0)
		return -1;
	
	return 0;
}

/***************************************************************************************
* �������ƣ�Card_PurseGet_Float
* ������������ȡ����Ǯ���ĸ�����ֵ
* ��ڲ�����-BlockNum�����			
* ���ڲ�����-pValue�������ȡ����ֵ�ĵ�ַ
* �� �� ֵ��0���ɹ�,  -1��ʧ��
* ˵    ������������֤�������ʹ��
***************************************************************************************/ 
int Card_PurseGet_Float(u8 BlockNum, float *pValue)
{
	char status;
	u8 ValueBuf[4];
	u8 *pTemp = (u8 *)pValue;

	status = Card_PurseGet(BlockNum, ValueBuf);
	if(status != 0)
		return -1;

	pTemp[0] = ValueBuf[0];
	pTemp[1] = ValueBuf[1];
	pTemp[2] = ValueBuf[2];
	pTemp[3] = ValueBuf[3];

	return 0;
}

/***************************************************************************************
* �������ƣ�Card_PurseIncrement_Int
* ��������������Ǯ����ֵ
* ��ڲ�����-BlockNum�����			
* ���ڲ�����-Value�����ӵ�ֵ
* �� �� ֵ��0���ɹ�,  -1��ʧ��
* ˵    ������������֤�������ʹ��
***************************************************************************************/ 
int Card_PurseIncrement_Int(u8 BlockNum, u32 Value)
{	
	char status;
	s32 ReadValue;
	s32 WriteValue;
	
	status = Card_PurseGet_Int(BlockNum, &ReadValue);
	if(status != 0)
		return -1;
	WriteValue = ReadValue + Value;
	Card_PurseInit_Int(BlockNum, WriteValue);
	if(status != 0)
		return -1;

	return 0;
}

/***************************************************************************************
* �������ƣ�Card_PurseDecrement_Int
* ��������������Ǯ����ֵ
* ��ڲ�����-BlockNum�����			
* ���ڲ�����-Value�����ٵ�ֵ
* �� �� ֵ��0���ɹ�,  -1��ʧ��
* ˵    ������������֤�������ʹ��
***************************************************************************************/ 
int Card_PurseDecrement_Int(u8 BlockNum, u32 Value)
{
	char status;
	s32 ReadValue;
	s32 WriteValue;
	
	status = Card_PurseGet_Int(BlockNum, &ReadValue);
	if(status != 0)
		return -1;
	WriteValue = ReadValue - Value;
	Card_PurseInit_Int(BlockNum, WriteValue);
	if(status != 0)
		return -1;

	return 0;
}

/***************************************************************************************
* �������ƣ�Card_PurseIncrement_Float
* ��������������Ǯ����ֵ
* ��ڲ�����-BlockNum�����			
* ���ڲ�����-Value�����ӵ�ֵ
* �� �� ֵ��0���ɹ�,  -1��ʧ��
* ˵    ������������֤�������ʹ��
***************************************************************************************/ 
int Card_PurseIncrement_Float(u8 BlockNum, float Value)
{
	char status;
	float ReadValue;
	float WriteValue;
	
	status = Card_PurseGet_Float(BlockNum, &ReadValue);
	if(status != 0)
		return -1;
	WriteValue = ReadValue + Value;
	Card_PurseInit_Float(BlockNum, WriteValue);
	if(status != 0)
		return -1;

	return 0;	
}

/***************************************************************************************
* �������ƣ�Card_PurseDecrement_Float
* ��������������Ǯ����ֵ
* ��ڲ�����-BlockNum�����			
* ���ڲ�����-Value�����ٵ�ֵ
* �� �� ֵ��0���ɹ�,  -1��ʧ��
* ˵    ������������֤�������ʹ��
***************************************************************************************/ 
int Card_PurseDecrement_Float(u8 BlockNum, float Value)
{
	char status;
	float ReadValue;
	float WriteValue;
	
	status = Card_PurseGet_Float(BlockNum, &ReadValue);
	if(status != 0)
		return -1;
	WriteValue = ReadValue - Value;
	Card_PurseInit_Float(BlockNum, WriteValue);
	if(status != 0)
		return -1;

	return 0;
}
