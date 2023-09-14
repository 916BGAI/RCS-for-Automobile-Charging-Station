#include <string.h>
#include <stdio.h>
#include "iccard.h"

char IC_Num[80] ; //存放卡号
u8 Get_ID_Card[4]={0};
u8 Last_ID_Card[4]={0};
u8 Default_Key[6] = {0xff,0xff,0xff,0xff,0xff,0xff};

/**************************************************************************************
* 函数名称：Card_GetNumber	
* 功能描述：获取IC卡号
* 入口参数：-mode：寻卡模式
*				   PICC_REQIDL：寻天线区内未进入休眠状态
*          PICC_REQALL：寻天线区内全部卡  
* 出口参数：-pNum：保存卡号的缓冲区首地址,4字节卡号
* 返 回 值：0：成功,  -1：失败
* 说    明：无
***************************************************************************************/ 
int Card_GetNumber(u8 mode, u8 *pNum)
{	
	u8 tempbuf[10];
	u8 count=0;
	char status;
	/*寻卡*/
	while(1)
	{			
		status = PCD_Request(mode, tempbuf);//应答复位		
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
						
	/*防冲突,返回卡号*/
	status = PCD_Anticoll(pNum);
	if(status != PCD_OK)
		return -1;
	
	/*选卡*/
	status = PCD_Select(pNum);
	if(status != PCD_OK)
		return -1;
	return 0;		
}

/**************************************************************************************
* 函数名称：Card_VerifyKeyA	
* 功能描述：验证IC卡密码A
* 入口参数：- pNum：保存卡号的缓冲区首地址,4字节卡号
*						-SectorNum：IC卡扇区号
*           -pKey：保存密码的缓冲区首地址,6字节密码
* 出口参数：无
* 返 回 值：0：成功,  -1：失败
* 说    明：无
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
* 函数名称：Card_VerifyKeyB	
* 功能描述：验证IC卡密码B
* 入口参数：- pNum：保存卡号的缓冲区首地址,4字节卡号
*			-BlockNum：IC卡扇区号
*           -pKey：保存密码的缓冲区首地址,6字节密码
* 出口参数：无
* 返 回 值：0：成功,  -1：失败
* 说    明：无
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
* 函数名称：Card_ReadBlock	
* 功能描述：读取IC卡指定块的数据
* 入口参数：-BlockNum：IC卡块号
* 出口参数：-pData：读取的数据,16字节数据
* 返 回 值：0：成功,  -1：失败
* 说    明：必须先验证密码才能读
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
* 函数名称：Card_WriteBlock	
* 功能描述：将数据写入IC卡指定块
* 入口参数：-BlockNum：IC卡块号
* 出口参数：-pData：写入的数据,16字节数据
* 返 回 值：0：成功,  -1：失败
* 说    明：必须先验证密码才能写
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
* 函数名称：Card_PurseInit	
* 功能描述：电子钱包初始化
* 入口参数：-BlockNum：IC卡块号
* 出口参数：-ValueBuf：保存电子钱包值的数组,4字节数值
* 返 回 值：0：成功,  -1：失败
* 说    明：必须先验证密码才能使用
*           ValueBuf[0]<->数值最低字节,ValueBuf[3]<->数值最高字节
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
* 函数名称：Card_PurseGet	
* 功能描述：获取电子钱包的值
* 入口参数：-BlockNum：IC卡块号
* 出口参数：-ValueBuf：保存电子钱包值的数组,4字节数值
* 返 回 值：0：成功,  -1：失败
* 说    明：必须先验证密码才能使用
*           ValueBuf[0]<->数值最低字节,ValueBuf[3]<->数值最高字节
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
* 函数名称：Card_PurseIncrement	
* 功能描述：电子钱包增值
* 入口参数：-BlockNum：IC卡块号
* 出口参数：-ValueBuf：保存电子钱包曾值的数组,4字节分别增加指定的值
* 返 回 值：0：成功,  -1：失败
* 说    明：必须先验证密码才能使用
*           ValueBuf[0]增到0xff后,再增1则ValueBuf[1]进1,依次类推。
*        	增值时ValueBuf[3]最大值0x7f,因为数值可表示正负数
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
* 函数名称：Card_PurseDecrement	
* 功能描述：电子钱包减值
* 入口参数：-BlockNum：IC卡块号
* 出口参数：-ValueBuf：保存电子钱包减值的数组,4字节分别减去指定的值
* 返 回 值：0：成功,  -1：失败
* 说    明：必须先验证密码才能使用
*           ValueBuf[0]减到0后,再减1则ValueBuf[1]减1,依次类推。
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
* 函数名称：Card_ModifyPasswordA	
* 功能描述：修改当前卡的密码A
* 入口参数：-SectorNum：扇区号
*			-pOldKey：旧密码,6字节密码
*           -pNewKey：新密码,6字节密码
* 出口参数：无
* 返 回 值：0：成功,  -1：其他原因失败,  -2：扇区号出错,  -3：旧密码参数出错
* 说    明：必须先验证密码才能使用
***************************************************************************************/ 
int Card_ModifyPasswordA(u8 SectorNum, u8 *pOldKey, u8 *pNewKye)
{
	char status, i;	
	u8 DataBuf[16];

	/*如果不是扇区地址,返回扇区号参数错误*/
	if(SectorNum % MF1_SectorSize != 0)	
		return -2;	

	/*获取当前卡的卡号*/
	status = Card_GetNumber(PICC_REQIDL, DataBuf);
	if(status != 0)
		return -1;

	/*旧密码A验证,如果旧密码不正确返回旧密码参数错误*/
	status = Card_VerifyKeyA(DataBuf, SectorNum, pOldKey);
	if(status != 0)
		return -3;

	/*读取本扇区块3数据*/
	status = Card_ReadBlock(SectorNum + 3, DataBuf);
	if(status != 0)
		return -1;

	/*写入本扇区块3数据,修改密码A*/
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
* 函数名称：Card_ModifyPasswordB	
* 功能描述：修改当前卡的密码B
* 入口参数：-SectorNum：扇区号
*           -pKeyA：密码A,6字节密码
*			-pOldKeyB：旧密码B,6字节密码
*           -pNewKeyB：新密码B,6字节密码
* 出口参数：无
* 返 回 值：0:成功, -1:其他原因失败, -2:扇区号出错, -3:旧密码B出错, -4:旧密码A出错
* 说    明：必须先验证密码才能使用
***************************************************************************************/ 
int Card_ModifyPasswordB(u8 SectorNum, u8 *pKeyA, u8 *pOldKeyB, u8 *pNewKyeB)
{
	char status, i;	
	u8 DataBuf[16];

	/*如果不是扇区地址,返回扇区号参数错误*/
	if(SectorNum % MF1_SectorSize != 0)	
		return -2;	

	/*获取当前卡的卡号*/
	status = Card_GetNumber(PICC_REQIDL, DataBuf);
	if(status != 0)
		return -1;

	/*旧密码B验证,如果旧密码不正确返回旧密码参数错误*/
	status = Card_VerifyKeyB(DataBuf, SectorNum, pOldKeyB);
	if(status != 0)
		return -3;

	/*旧密码A验证,如果旧密码不正确返回旧密码参数错误*/
	status = Card_VerifyKeyA(DataBuf, SectorNum, pKeyA);
	if(status != 0)
		return -4;

	/*读取本扇区块3数据*/
	status = Card_ReadBlock(SectorNum + 3, DataBuf);
	if(status != 0)
		return -1;

	/*写入本扇区块3数据,修改密码B*/
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
* 函数名称：Card_PurseInit_Int
* 功能描述：电子钱包初始化一个整型数值
* 入口参数：-BlockNum：块号
*			-Value：初始化的数值
* 出口参数：无
* 返 回 值：0：成功,  -1：失败
* 说    明：必须先验证密码才能使用
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
* 函数名称：Card_PurseGet_Int
* 功能描述：获取电子钱包的整型数值
* 入口参数：-BlockNum：块号			
* 出口参数：-pValue：保存获取的数值的地址
* 返 回 值：0：成功,  -1：失败
* 说    明：必须先验证密码才能使用
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
* 函数名称：Card_PurseInit_Float
* 功能描述：电子钱包初始化一个浮点数值
* 入口参数：-BlockNum：块号
*			-Value：初始化的数值
* 出口参数：无
* 返 回 值：0：成功,  -1：失败
* 说    明：必须先验证密码才能使用
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
* 函数名称：Card_PurseGet_Float
* 功能描述：获取电子钱包的浮点数值
* 入口参数：-BlockNum：块号			
* 出口参数：-pValue：保存获取的数值的地址
* 返 回 值：0：成功,  -1：失败
* 说    明：必须先验证密码才能使用
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
* 函数名称：Card_PurseIncrement_Int
* 功能描述：电子钱包增值
* 入口参数：-BlockNum：块号			
* 出口参数：-Value：增加的值
* 返 回 值：0：成功,  -1：失败
* 说    明：必须先验证密码才能使用
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
* 函数名称：Card_PurseDecrement_Int
* 功能描述：电子钱包减值
* 入口参数：-BlockNum：块号			
* 出口参数：-Value：减少的值
* 返 回 值：0：成功,  -1：失败
* 说    明：必须先验证密码才能使用
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
* 函数名称：Card_PurseIncrement_Float
* 功能描述：电子钱包增值
* 入口参数：-BlockNum：块号			
* 出口参数：-Value：增加的值
* 返 回 值：0：成功,  -1：失败
* 说    明：必须先验证密码才能使用
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
* 函数名称：Card_PurseDecrement_Float
* 功能描述：电子钱包减值
* 入口参数：-BlockNum：块号			
* 出口参数：-Value：减少的值
* 返 回 值：0：成功,  -1：失败
* 说    明：必须先验证密码才能使用
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
