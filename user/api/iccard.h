#ifndef __ICCARD__H
#define __ICCARD__H

#include <mfrc522.h>

extern u8 Get_ID_Card[4];
extern u8 Last_ID_Card[4];
extern u8 Default_Key[6];
/*MF1卡存储器组织,以块为单位*/
#define MF1_SectorSize			4				//每扇区4块,每块16字节
#define MF1_SectorNum(s)		(s*4)			//扇区地址(s=0~15)
#define MF1_BlockNum(s,b)		(s*4+b)			//块地址(s=0~15, b=0~3),操作每扇区的块3时需小心

#define Sector 4
#define Block  2

/*IC卡接口函数*/
/*IC卡接口函数*/
int Card_GetNumber(u8 mode, u8 *pNum);				//获取IC卡号
int Card_VerifyKeyA(u8* pNum, u8 SectorNum, u8 *pKey); 			//验证IC卡密码A
int Card_VerifyKeyB(u8* pNum, u8 SectorNum, u8 *pKey); 			//验证IC卡密码B
int Card_ReadBlock(u8 BlockNum, u8 *pData);									//读取IC卡指定块的数据
int Card_WriteBlock(u8 BlockNum, u8 *pData);								//将数据写入IC卡指定块
int Card_PurseInit(u8 BlockNum, u8 ValueBuf[4]);						//电子钱包初始化
int Card_PurseGet(u8 BlockNum, u8 ValueBuf[4]);							//获取电子钱包的值
int Card_PurseIncrement(u8 BlockNum, u8 ValueBuf[4]);				//电子钱包曾值
int Card_PurseDecrement(u8 BlockNum, u8 ValueBuf[4]);				//电子钱包减值
int Card_ModifyPasswordA(u8 SectorNum, u8 *pOldKey, u8 *pNewKye);		//修改当前卡的密码A
int Card_ModifyPasswordB(u8 SectorNum, u8 *pKeyA, u8 *pOldKeyB, u8 *pNewKyeB);	//修改当前卡的密码B

int Card_PurseInit_Int(u8 BlockNum, s32 Value);
int Card_PurseGet_Int(u8 BlockNum, s32 *pValue);
int Card_PurseInit_Float(u8 BlockNum, float Value);
int Card_PurseGet_Float(u8 BlockNum, float *pValue);
int Card_PurseIncrement_Int(u8 BlockNum, u32 Value);
int Card_PurseDecrement_Int(u8 BlockNum, u32 Value);
int Card_PurseIncrement_Float(u8 BlockNum, float Value);
int Card_PurseDecrement_Float(u8 BlockNum, float Value);
#endif
