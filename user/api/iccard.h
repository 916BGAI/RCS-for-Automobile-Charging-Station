#ifndef __ICCARD__H
#define __ICCARD__H

#include <mfrc522.h>

extern u8 Get_ID_Card[4];
extern u8 Last_ID_Card[4];
extern u8 Default_Key[6];
/*MF1���洢����֯,�Կ�Ϊ��λ*/
#define MF1_SectorSize			4				//ÿ����4��,ÿ��16�ֽ�
#define MF1_SectorNum(s)		(s*4)			//������ַ(s=0~15)
#define MF1_BlockNum(s,b)		(s*4+b)			//���ַ(s=0~15, b=0~3),����ÿ�����Ŀ�3ʱ��С��

#define Sector 4
#define Block  2

/*IC���ӿں���*/
/*IC���ӿں���*/
int Card_GetNumber(u8 mode, u8 *pNum);				//��ȡIC����
int Card_VerifyKeyA(u8* pNum, u8 SectorNum, u8 *pKey); 			//��֤IC������A
int Card_VerifyKeyB(u8* pNum, u8 SectorNum, u8 *pKey); 			//��֤IC������B
int Card_ReadBlock(u8 BlockNum, u8 *pData);									//��ȡIC��ָ���������
int Card_WriteBlock(u8 BlockNum, u8 *pData);								//������д��IC��ָ����
int Card_PurseInit(u8 BlockNum, u8 ValueBuf[4]);						//����Ǯ����ʼ��
int Card_PurseGet(u8 BlockNum, u8 ValueBuf[4]);							//��ȡ����Ǯ����ֵ
int Card_PurseIncrement(u8 BlockNum, u8 ValueBuf[4]);				//����Ǯ����ֵ
int Card_PurseDecrement(u8 BlockNum, u8 ValueBuf[4]);				//����Ǯ����ֵ
int Card_ModifyPasswordA(u8 SectorNum, u8 *pOldKey, u8 *pNewKye);		//�޸ĵ�ǰ��������A
int Card_ModifyPasswordB(u8 SectorNum, u8 *pKeyA, u8 *pOldKeyB, u8 *pNewKyeB);	//�޸ĵ�ǰ��������B

int Card_PurseInit_Int(u8 BlockNum, s32 Value);
int Card_PurseGet_Int(u8 BlockNum, s32 *pValue);
int Card_PurseInit_Float(u8 BlockNum, float Value);
int Card_PurseGet_Float(u8 BlockNum, float *pValue);
int Card_PurseIncrement_Int(u8 BlockNum, u32 Value);
int Card_PurseDecrement_Int(u8 BlockNum, u32 Value);
int Card_PurseIncrement_Float(u8 BlockNum, float Value);
int Card_PurseDecrement_Float(u8 BlockNum, float Value);
#endif
