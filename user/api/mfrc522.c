#include "mfrc522.h"
#include "app.h"
#include "delay.h"
#include "iccard.h"
#include "lcd.h"
#include "spi.h"
#include <stdio.h>

#define Delay_ms Delay_msnop
/**************************************************************************************
 * �������ƣ�MFRC_Init
 * ����������MFRC��ʼ��
 * ��ڲ�������
 * ���ڲ�������
 * �� �� ֵ����
 * ˵    ����MFRC��SPI�ӿ�����Ϊ0~10Mbps
 ***************************************************************************************/
void MFRC_Init(void) {
  // MFRC�ܽ�����
  SPI_RC522_Init(); // ��ʼ��SPI1
}

/**************************************************************************************
 * ��������: SPI_RW_Byte
 * ��������: ģ��SPI��дһ���ֽ�
 * ��ڲ���: -byte:Ҫ���͵�����
 * ���ڲ���: -byte:���յ�������
 ***************************************************************************************/

u8 SPI2_RW_Byte(u8 byte) { return SOFT_SPI_RW(byte); }

/**************************************************************************************
 * �������ƣ�MFRC_WriteReg
 * ����������дһ���Ĵ���
 * ��ڲ�����-addr:��д�ļĴ�����ַ
 *           -data:��д�ļĴ�������
 * ���ڲ�������
 * �� �� ֵ����
 * ˵    ������
 ***************************************************************************************/
void MFRC_WriteReg(u8 addr, u8 data) {
  u8 AddrByte;
  AddrByte = (addr << 1) & 0x7E; // �����ַ�ֽ�
  CS_522 = 0;                    // NSS����
  Soft_SPI_Write(AddrByte);      // д��ַ�ֽ�
  Soft_SPI_Write(data);          // д����
  CS_522 = 1;                    // NSS����
}

/**************************************************************************************
 * �������ƣ�MFRC_ReadReg
 * ������������һ���Ĵ���
 * ��ڲ�����-addr:�����ļĴ�����ַ
 * ���ڲ�������
 * �� �� ֵ��-data:�����Ĵ���������
 * ˵    ������
 ***************************************************************************************/
u8 MFRC_ReadReg(u8 addr) {
  u8 AddrByte, data;
  AddrByte = ((addr << 1) & 0x7E) | 0x80; // �����ַ�ֽ�
  CS_522 = 0;                             // NSS����
  Soft_SPI_Write(AddrByte);               // д��ַ�ֽ�
  data = Soft_SPI_Read();                 // ������
  CS_522 = 1;                             // NSS����
  return data;
}

/**************************************************************************************
 * �������ƣ�MFRC_SetBitMask
 * �������������üĴ�����λ
 * ��ڲ�����-addr:�����õļĴ�����ַ
 *           -mask:�����üĴ�����λ(��ͬʱ���ö��bit)
 * ���ڲ�������
 * �� �� ֵ����
 * ˵    ������
 ***************************************************************************************/
void MFRC_SetBitMask(u8 addr, u8 mask) {
  u8 temp;
  temp = MFRC_ReadReg(addr);        // �ȶ��ؼĴ�����ֵ
  MFRC_WriteReg(addr, temp | mask); // �������������д��Ĵ���
}

/**************************************************************************************
 * �������ƣ�MFRC_ClrBitMask
 * ��������������Ĵ�����λ
 * ��ڲ�����-addr:������ļĴ�����ַ
 *           -mask:������Ĵ�����λ(��ͬʱ������bit)
 * ���ڲ�������
 * �� �� ֵ����
 * ˵    ������
 ***************************************************************************************/
void MFRC_ClrBitMask(u8 addr, u8 mask) {
  u8 temp;
  temp = MFRC_ReadReg(addr);         // �ȶ��ؼĴ�����ֵ
  MFRC_WriteReg(addr, temp & ~mask); // �������������д��Ĵ���
}

/**************************************************************************************
 * �������ƣ�MFRC_CalulateCRC
 * ������������MFRC����CRC���
 * ��ڲ�����-pInData��������CRC���������
 *			-len��������CRC��������ݳ���
 *			-pOutData��CRC������
 * ���ڲ�����-pOutData��CRC������
 * �� �� ֵ����
 * ˵    ������
 ***************************************************************************************/
void MFRC_CalulateCRC(u8 *pInData, u8 len, u8 *pOutData) {
  u8 temp;
  u32 i;
  MFRC_ClrBitMask(MFRC_DivIrqReg, 0x04);     // ʹ��CRC�ж�
  MFRC_WriteReg(MFRC_CommandReg, MFRC_IDLE); // ȡ����ǰ�����ִ��
  MFRC_SetBitMask(MFRC_FIFOLevelReg, 0x80);  // ���FIFO�����־λ
  for (i = 0; i < len; i++) // ����CRC���������д��FIFO
  {
	MFRC_WriteReg(MFRC_FIFODataReg, *(pInData + i));
  }
  MFRC_WriteReg(MFRC_CommandReg, MFRC_CALCCRC); // ִ��CRC����
  i = 100000;
  do {
	temp = MFRC_ReadReg(MFRC_DivIrqReg); // ��ȡDivIrqReg�Ĵ�����ֵ
	i--;
  } while ((i != 0) && !(temp & 0x04));           // �ȴ�CRC�������
  pOutData[0] = MFRC_ReadReg(MFRC_CRCResultRegL); // ��ȡCRC������
  pOutData[1] = MFRC_ReadReg(MFRC_CRCResultRegM);
}

/**************************************************************************************
 * �������ƣ�MFRC_CmdFrame
 * ����������MFRC522��ISO14443A��ͨѶ������֡����
 * ��ڲ�����-cmd��MFRC522������
 *           -pIndata��MFRC522���͸�MF1�������ݵĻ������׵�ַ
 *           -InLenByte���������ݵ��ֽڳ���
 *           -pOutdata�����ڽ���MF1��Ƭ�������ݵĻ������׵�ַ
 *           -pOutLenBit��MF1���������ݵ�λ����
 * ���ڲ�����-pOutdata�����ڽ���MF1��Ƭ�������ݵĻ������׵�ַ
 *           -pOutLenBit������MF1����������λ���ȵ��׵�ַ
 * �� �� ֵ��-status���������(MFRC_OK��MFRC_NOTAGERR��MFRC_ERR)
 * ˵    ������
 ***************************************************************************************/
char MFRC_CmdFrame(u8 cmd, u8 *pInData, u8 InLenByte, u8 *pOutData,
				   u16 *pOutLenBit) {
  u8 lastBits;
  u8 n;
  u32 i;
  char status = MFRC_ERR;
  u8 irqEn = 0x00;
  u8 waitFor = 0x00;

  /*�����������ñ�־λ*/
  switch (cmd) {
  case MFRC_AUTHENT: // Mifare��֤
	irqEn = 0x12;
	waitFor = 0x10; // idleIRq�жϱ�־
	break;
  case MFRC_TRANSCEIVE: // ���Ͳ���������
	irqEn = 0x77;
	waitFor = 0x30; // RxIRq��idleIRq�жϱ�־
	break;
  }

  /*��������֡ǰ׼��*/
  MFRC_WriteReg(MFRC_ComIEnReg, irqEn | 0x80); // ���ж�
  MFRC_ClrBitMask(MFRC_ComIrqReg, 0x80);       // ����жϱ�־λSET1
  MFRC_WriteReg(MFRC_CommandReg, MFRC_IDLE);   // ȡ����ǰ�����ִ��
  MFRC_SetBitMask(MFRC_FIFOLevelReg, 0x80); // ���FIFO�����������־λ

  /*��������֡*/
  for (i = 0; i < InLenByte; i++) // д���������
  {
	MFRC_WriteReg(MFRC_FIFODataReg, pInData[i]);
  }
  MFRC_WriteReg(MFRC_CommandReg, cmd); // ִ������
  if (cmd == MFRC_TRANSCEIVE) {
	MFRC_SetBitMask(MFRC_BitFramingReg, 0x80); // ��������
  }
  i = 300000; // ����ʱ��Ƶ�ʵ���,����M1�����ȴ�ʱ��25ms
  do {
	n = MFRC_ReadReg(MFRC_ComIrqReg);
	i--;
  } while ((i != 0) && !(n & 0x01) && !(n & waitFor)); // �ȴ��������
  MFRC_ClrBitMask(MFRC_BitFramingReg, 0x80);           // ֹͣ����

  /*������յ�����*/
  if (i != 0) {
	if (!(MFRC_ReadReg(MFRC_ErrorReg) & 0x1B)) {
	  status = MFRC_OK;
	  if (n & irqEn & 0x01) {
		status = MFRC_NOTAGERR;
	  }
	  if (cmd == MFRC_TRANSCEIVE) {
		n = MFRC_ReadReg(MFRC_FIFOLevelReg);
		lastBits = MFRC_ReadReg(MFRC_ControlReg) & 0x07;
		if (lastBits) {
		  *pOutLenBit = (n - 1) * 8 + lastBits;
		} else {
		  *pOutLenBit = n * 8;
		}
		if (n == 0) {
		  n = 1;
		}
		if (n > MFRC_MAXRLEN) {
		  n = MFRC_MAXRLEN;
		}
		for (i = 0; i < n; i++) {
		  pOutData[i] = MFRC_ReadReg(MFRC_FIFODataReg);
		}
	  }
	} else {
	  status = MFRC_ERR;
	}
  }

  MFRC_SetBitMask(MFRC_ControlReg, 0x80);    // ֹͣ��ʱ������
  MFRC_WriteReg(MFRC_CommandReg, MFRC_IDLE); // ȡ����ǰ�����ִ��

  return status;
}

/**************************************************************************************
 * �������ƣ�PCD_Reset
 * ����������PCD��λ
 * ��ڲ�������
 * ���ڲ�������
 * �� �� ֵ����
 * ˵    ������
 ***************************************************************************************/
void PCD_Reset(void) {
  /*Ӳ��λ*/
  RST_522 = 1;
  Delay_ms(2);
  RST_522 = 0;
  Delay_ms(2);
  RST_522 = 1;
  Delay_ms(2);

  /*��λ*/
  MFRC_WriteReg(MFRC_CommandReg, MFRC_RESETPHASE);
  Delay_ms(2);

  /*��λ��ĳ�ʼ������*/
  MFRC_WriteReg(MFRC_ModeReg, 0x3D);   // CRC��ʼֵ0x6363
  MFRC_WriteReg(MFRC_TReloadRegL, 30); // ��ʱ����װֵ
  MFRC_WriteReg(MFRC_TReloadRegH, 0);
  MFRC_WriteReg(MFRC_TModeReg, 0x8D);      // ��ʱ������
  MFRC_WriteReg(MFRC_TPrescalerReg, 0x3E); // ��ʱ��Ԥ��Ƶֵ
  MFRC_WriteReg(MFRC_TxAutoReg, 0x40);     // 100%ASK

  PCD_AntennaOff(); // ������
  Delay_ms(2);
  PCD_AntennaOn(); // ������
}

/**************************************************************************************
 * �������ƣ�PCD_AntennaOn
 * ������������������,ʹ��PCD���������ز��ź�
 * ��ڲ�������
 * ���ڲ�������
 * �� �� ֵ����
 * ˵    ����ÿ�ο�����ر�����֮��Ӧ������1ms�ļ��
 ***************************************************************************************/
void PCD_AntennaOn(void) {
  u8 temp;
  temp = MFRC_ReadReg(MFRC_TxControlReg);
  if (!(temp & 0x03)) {
	MFRC_SetBitMask(MFRC_TxControlReg, 0x03);
  }
}

/**************************************************************************************
 * �������ƣ�PCD_AntennaOff
 * �����������ر�����,ʧ��PCD���������ز��ź�
 * ��ڲ�������
 * ���ڲ�������
 * �� �� ֵ����
 * ˵    ����ÿ�ο�����ر�����֮��Ӧ������1ms�ļ��
 ***************************************************************************************/
void PCD_AntennaOff(void) { MFRC_ClrBitMask(MFRC_TxControlReg, 0x03); }

/***************************************************************************************
 * �������ƣ�PCD_Init
 * ������������д����ʼ��
 * ��ڲ�������
 * ���ڲ�������
 * �� �� ֵ����
 * ˵    ������
 ***************************************************************************************/
void PCD_Init(void) {
  MFRC_Init();
  PCD_Reset();
  //	PCD_AntennaOff();
  //	PCD_AntennaOn();
}

/***************************************************************************************
 * �������ƣ�PCD_Request
 * ����������Ѱ��
 * ��ڲ�����-RequestMode��Ѷ����ʽ
 *						  PICC_REQIDL��Ѱ��������δ��������״̬
 *             PICC_REQALL��Ѱ��������ȫ����
 *		       	-pCardType�����ڱ��濨Ƭ����
 * ���ڲ�����-pCardType����Ƭ����
 *										0x4400��Mifare_UltraLight
 *                		0x0400��Mifare_One(S50)
 *                		0x0200��Mifare_One(S70)
 *                		0x0800��Mifare_Pro(X)
 *                		0x4403��Mifare_DESFire
 * �� �� ֵ��-status���������(PCD_OK��PCD_NOTAGERR��PCD_ERR)
 * ˵    ������
 ***************************************************************************************/
char PCD_Request(u8 RequestMode, u8 *pCardType) {
  char status;
  u16 unLen;
  u8 CmdFrameBuf[MFRC_MAXRLEN];

  MFRC_ClrBitMask(MFRC_Status2Reg, 0x08);
  MFRC_WriteReg(MFRC_BitFramingReg, 0x07);
  MFRC_SetBitMask(MFRC_TxControlReg, 0x03);

  CmdFrameBuf[0] = RequestMode;

  status = MFRC_CmdFrame(MFRC_TRANSCEIVE, CmdFrameBuf, 1, CmdFrameBuf, &unLen);

  if ((status == PCD_OK) && (unLen == 0x10)) {
	*pCardType = CmdFrameBuf[0];
	*(pCardType + 1) = CmdFrameBuf[1];
  }

  return status;
}

/***************************************************************************************
 * �������ƣ�PCD_Anticoll
 * ��������������ͻ,��ȡ����
 * ��ڲ�����-pSnr�����ڱ��濨Ƭ���к�,4�ֽ�
 * ���ڲ�����-pSnr����Ƭ���к�,4�ֽ�
 * �� �� ֵ��-status���������(PCD_OK��PCD_NOTAGERR��PCD_ERR)
 * ˵    ������
 ***************************************************************************************/
char PCD_Anticoll(u8 *pSnr) {
  char status;
  u8 i, snr_check = 0;
  u16 unLen;
  u8 CmdFrameBuf[MFRC_MAXRLEN];

  MFRC_ClrBitMask(MFRC_Status2Reg, 0x08);
  MFRC_WriteReg(MFRC_BitFramingReg, 0x00);
  MFRC_ClrBitMask(MFRC_CollReg, 0x80);

  CmdFrameBuf[0] = PICC_ANTICOLL1;
  CmdFrameBuf[1] = 0x20;

  status = MFRC_CmdFrame(MFRC_TRANSCEIVE, CmdFrameBuf, 2, CmdFrameBuf, &unLen);

  if (status == PCD_OK) {
	for (i = 0; i < 4; i++) {
	  *(pSnr + i) = CmdFrameBuf[i];
	  snr_check ^= CmdFrameBuf[i];
	}
	if (snr_check != CmdFrameBuf[i]) {
	  status = PCD_ERR;
	}
  }

  MFRC_SetBitMask(MFRC_CollReg, 0x80);
  return status;
}

/***************************************************************************************
 * �������ƣ�PCD_Select
 * ����������ѡ��
 * ��ڲ�����-pSnr����Ƭ���к�,4�ֽ�
 * ���ڲ�������
 * �� �� ֵ��-status���������(PCD_OK��PCD_NOTAGERR��PCD_ERR)
 * ˵    ������
 ***************************************************************************************/
char PCD_Select(u8 *pSnr) {
  char status;
  u8 i;
  u16 unLen;
  u8 CmdFrameBuf[MFRC_MAXRLEN];

  CmdFrameBuf[0] = PICC_ANTICOLL1;
  CmdFrameBuf[1] = 0x70;
  CmdFrameBuf[6] = 0;
  for (i = 0; i < 4; i++) {
	CmdFrameBuf[i + 2] = *(pSnr + i);
	CmdFrameBuf[6] ^= *(pSnr + i);
  }
  MFRC_CalulateCRC(CmdFrameBuf, 7, &CmdFrameBuf[7]);

  MFRC_ClrBitMask(MFRC_Status2Reg, 0x08);

  status = MFRC_CmdFrame(MFRC_TRANSCEIVE, CmdFrameBuf, 9, CmdFrameBuf, &unLen);

  if ((status == PCD_OK) && (unLen == 0x18)) {
	status = PCD_OK;
  } else {
	status = PCD_ERR;
  }

  return status;
}

/***************************************************************************************
 * �������ƣ�PCD_AuthState
 * ������������֤��Ƭ����
 * ��ڲ�����-AuthMode����֤ģʽ
 *					PICC_AUTHENT1A����֤A����
 *					PICC_AUTHENT1B����֤B����
 *           -BlockAddr�����ַ(0~63)
 *           -pKey������
 *           -pSnr����Ƭ���к�,4�ֽ�
 * ���ڲ�������
 * �� �� ֵ��-status���������(PCD_OK��PCD_NOTAGERR��PCD_ERR)
 * ˵    ������֤����ʱ,������Ϊ��λ,BlockAddr����������ͬһ�������������
 ***************************************************************************************/
char PCD_AuthState(u8 AuthMode, u8 BlockAddr, u8 *pKey, u8 *pSnr) {
  char status;
  u16 unLen;
  u8 i, CmdFrameBuf[MFRC_MAXRLEN];
  CmdFrameBuf[0] = AuthMode;
  CmdFrameBuf[1] = BlockAddr;
  for (i = 0; i < 6; i++) {
	CmdFrameBuf[i + 2] = *(pKey + i);
  }
  for (i = 0; i < 4; i++) {
	CmdFrameBuf[i + 8] = *(pSnr + i);
  }

  status = MFRC_CmdFrame(MFRC_AUTHENT, CmdFrameBuf, 12, CmdFrameBuf, &unLen);
  if ((status != PCD_OK) || (!(MFRC_ReadReg(MFRC_Status2Reg) & 0x08))) {
	status = PCD_ERR;
  }

  return status;
}

/***************************************************************************************
 * �������ƣ�PCD_WriteBlock
 * ����������дMF1�����ݿ�
 * ��ڲ�����-BlockAddr�����ַ
 *			-pData:	���ڱ����д�������,16�ֽ�
 * ���ڲ�������
 * �� �� ֵ��-status���������(PCD_OK��PCD_NOTAGERR��PCD_ERR)
 * ˵    ������
 ***************************************************************************************/
char PCD_WriteBlock(u8 BlockAddr, u8 *pData) {
  char status;
  u16 unLen;
  u8 i, CmdFrameBuf[MFRC_MAXRLEN];

  CmdFrameBuf[0] = PICC_WRITE;
  CmdFrameBuf[1] = BlockAddr;
  MFRC_CalulateCRC(CmdFrameBuf, 2, &CmdFrameBuf[2]);

  status = MFRC_CmdFrame(MFRC_TRANSCEIVE, CmdFrameBuf, 4, CmdFrameBuf, &unLen);

  if ((status != PCD_OK) || (unLen != 4) || ((CmdFrameBuf[0] & 0x0F) != 0x0A)) {
	status = PCD_ERR;
  }

  if (status == PCD_OK) {
	for (i = 0; i < 16; i++) {
	  CmdFrameBuf[i] = *(pData + i);
	}
	MFRC_CalulateCRC(CmdFrameBuf, 16, &CmdFrameBuf[16]);

	status =
		MFRC_CmdFrame(MFRC_TRANSCEIVE, CmdFrameBuf, 18, CmdFrameBuf, &unLen);

	if ((status != PCD_OK) || (unLen != 4) ||
		((CmdFrameBuf[0] & 0x0F) != 0x0A)) {
	  status = PCD_ERR;
	}
  }

  return status;
}

/***************************************************************************************
 * �������ƣ�PCD_ReadBlock
 * ������������MF1�����ݿ�
 * ��ڲ�����-BlockAddr�����ַ
 *			-pData:	���ڱ������������,16�ֽ�
 * ���ڲ�����-pData:	���ڱ������������,16�ֽ�
 * �� �� ֵ��-status���������(PCD_OK��PCD_NOTAGERR��PCD_ERR)
 * ˵    ������
 ***************************************************************************************/
char PCD_ReadBlock(u8 BlockAddr, u8 *pData) {
  char status;
  u16 unLen;
  u8 i, CmdFrameBuf[MFRC_MAXRLEN];

  CmdFrameBuf[0] = PICC_READ;
  CmdFrameBuf[1] = BlockAddr;
  MFRC_CalulateCRC(CmdFrameBuf, 2, &CmdFrameBuf[2]);

  status = MFRC_CmdFrame(MFRC_TRANSCEIVE, CmdFrameBuf, 4, CmdFrameBuf, &unLen);
  if ((status == PCD_OK) && (unLen == 0x90)) {
	for (i = 0; i < 16; i++) {
	  *(pData + i) = CmdFrameBuf[i];
	}
  } else {
	status = PCD_ERR;
  }

  return status;
}

/***************************************************************************************
 * �������ƣ�PCD_Value
 * ������������MF1�����ݿ�����ֵ����
 * ��ڲ�����-BlockAddr�����ַ
 *			-pValue�����ֽ���ֵ��ֵ,��λ��ǰ
 *			-mode����ֵ�����ģʽ
 *				   PICC_INCREMENT����ֵ
 * 				   PICC_DECREMENT����ֵ
 * ���ڲ�������
 * �� �� ֵ��-status���������(PCD_OK��PCD_NOTAGERR��PCD_ERR)
 * ˵    ������
 ***************************************************************************************/
char PCD_Value(u8 mode, u8 BlockAddr, u8 *pValue) {
  char status;
  u16 unLen;
  u8 i, CmdFrameBuf[MFRC_MAXRLEN];

  CmdFrameBuf[0] = mode;
  CmdFrameBuf[1] = BlockAddr;
  MFRC_CalulateCRC(CmdFrameBuf, 2, &CmdFrameBuf[2]);

  status = MFRC_CmdFrame(MFRC_TRANSCEIVE, CmdFrameBuf, 4, CmdFrameBuf, &unLen);

  if ((status != PCD_OK) || (unLen != 4) || ((CmdFrameBuf[0] & 0x0F) != 0x0A)) {
	status = PCD_ERR;
  }

  if (status == PCD_OK) {
	for (i = 0; i < 16; i++) {
	  CmdFrameBuf[i] = *(pValue + i);
	}
	MFRC_CalulateCRC(CmdFrameBuf, 4, &CmdFrameBuf[4]);
	unLen = 0;
	status =
		MFRC_CmdFrame(MFRC_TRANSCEIVE, CmdFrameBuf, 6, CmdFrameBuf, &unLen);
	if (status != PCD_ERR) {
	  status = PCD_OK;
	}
  }

  if (status == PCD_OK) {
	CmdFrameBuf[0] = PICC_TRANSFER;
	CmdFrameBuf[1] = BlockAddr;
	MFRC_CalulateCRC(CmdFrameBuf, 2, &CmdFrameBuf[2]);

	status =
		MFRC_CmdFrame(MFRC_TRANSCEIVE, CmdFrameBuf, 4, CmdFrameBuf, &unLen);

	if ((status != PCD_OK) || (unLen != 4) ||
		((CmdFrameBuf[0] & 0x0F) != 0x0A)) {
	  status = PCD_ERR;
	}
  }
  return status;
}

/***************************************************************************************
 * �������ƣ�PCD_BakValue
 * ��������������Ǯ��(��ת��)
 * ��ڲ�����-sourceBlockAddr��Դ���ַ
 *			-goalBlockAddr��Ŀ����ַ
 * ���ڲ�������
 * �� �� ֵ��-status���������(PCD_OK��PCD_NOTAGERR��PCD_ERR)
 * ˵    ����ֻ����ͬһ��������ת��
 ***************************************************************************************/
char PCD_BakValue(u8 sourceBlockAddr, u8 goalBlockAddr) {
  char status;
  u16 unLen;
  u8 CmdFrameBuf[MFRC_MAXRLEN];

  CmdFrameBuf[0] = PICC_RESTORE;
  CmdFrameBuf[1] = sourceBlockAddr;
  MFRC_CalulateCRC(CmdFrameBuf, 2, &CmdFrameBuf[2]);
  status = MFRC_CmdFrame(MFRC_TRANSCEIVE, CmdFrameBuf, 4, CmdFrameBuf, &unLen);
  if ((status != PCD_OK) || (unLen != 4) || ((CmdFrameBuf[0] & 0x0F) != 0x0A)) {
	status = PCD_ERR;
  }

  if (status == PCD_OK) {
	CmdFrameBuf[0] = 0;
	CmdFrameBuf[1] = 0;
	CmdFrameBuf[2] = 0;
	CmdFrameBuf[3] = 0;
	MFRC_CalulateCRC(CmdFrameBuf, 4, &CmdFrameBuf[4]);
	status =
		MFRC_CmdFrame(MFRC_TRANSCEIVE, CmdFrameBuf, 6, CmdFrameBuf, &unLen);
	if (status != PCD_ERR) {
	  status = PCD_OK;
	}
  }

  if (status != PCD_OK) {
	return PCD_ERR;
  }

  CmdFrameBuf[0] = PICC_TRANSFER;
  CmdFrameBuf[1] = goalBlockAddr;
  MFRC_CalulateCRC(CmdFrameBuf, 2, &CmdFrameBuf[2]);
  status = MFRC_CmdFrame(MFRC_TRANSCEIVE, CmdFrameBuf, 4, CmdFrameBuf, &unLen);
  if ((status != PCD_OK) || (unLen != 4) || ((CmdFrameBuf[0] & 0x0F) != 0x0A)) {
	status = PCD_ERR;
  }

  return status;
}

/***************************************************************************************
 * �������ƣ�PCD_Halt
 * �������������Ƭ��������״̬
 * ��ڲ�������
 * ���ڲ�������
 * �� �� ֵ��-status���������(PCD_OK��PCD_NOTAGERR��PCD_ERR)
 * ˵    ������
 ***************************************************************************************/
char PCD_Halt(void) {
  char status;
  u16 unLen;
  u8 CmdFrameBuf[MFRC_MAXRLEN];

  CmdFrameBuf[0] = PICC_HALT;
  CmdFrameBuf[1] = 0;
  MFRC_CalulateCRC(CmdFrameBuf, 2, &CmdFrameBuf[2]);

  status = MFRC_CmdFrame(MFRC_TRANSCEIVE, CmdFrameBuf, 4, CmdFrameBuf, &unLen);

  return status;
}

s32 money_num = 0;
u16 card_money = 500;
// ���ǰ��Ǯ���鿴���
void MFRC522_Add_IC(void) {
  while (1) {
	if (Card_GetNumber(PICC_REQALL, Get_ID_Card) != 0) // ���û�л����ȷ�Ŀ���
	{
	  Draw_Text_8_16_Str(50, 80, WHITE, BLUE, "�����ˢ��.......");
	  printf("Plase put IC_Card\r\n");
	} else {
	  printf("IC_Card id:%x-%x-%x-%x\r\n", Get_ID_Card[0], Get_ID_Card[1],
			 Get_ID_Card[2], Get_ID_Card[3]);
	  break;
	}
  }
  printf("Get a new IC_Card\r\n");
  if (Card_VerifyKeyA(Get_ID_Card, MF1_SectorNum(4), Default_Key) == 0) {
	if (Card_PurseInit_Int(MF1_BlockNum(4, 1), card_money) == 0) {
	  printf("money:%d\r\n", card_money);
	  return;
	}
  } else {
  }
}

// �����ɺ��Ǯ
void MFRC522_Add_IC1(void) {
  while (1) {
	if (Card_GetNumber(PICC_REQALL, Get_ID_Card) != 0) // ���û�л����ȷ�Ŀ���
	{
	  printf("Plase put IC_Card\r\n");
	} else {
	  printf("IC_Card id:%x-%x-%x-%x\r\n", Get_ID_Card[0], Get_ID_Card[1],
			 Get_ID_Card[2], Get_ID_Card[3]);
	  break;
	}
  }
  printf("Get a new IC_Card\r\n");
  if (Card_VerifyKeyA(Get_ID_Card, MF1_SectorNum(4), Default_Key) == 0) {
	if (Card_PurseInit_Int(MF1_BlockNum(4, 1), (card_money - device1.money)) ==
		0) {
	  printf("money:%d\r\n", card_money - device1.money);
	}
  } else {
  }
}
