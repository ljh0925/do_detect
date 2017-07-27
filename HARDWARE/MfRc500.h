/******************************************************************************
Copyright 2005 �����״�ͨ�������޹�˾
All rights riserved.

�ļ���	��MfRc500.h
ģ�����ƣ�RC500��������ĺ�������ͷ�ļ�
���ܸ�Ҫ��

ȡ���汾��0.0.1
�޸���	��������
������ڣ�2004.09.02
����˵������philips��˾�ṩ��Դ���������޸ģ���������ԭ���ķ��

******************************************************************************/
/*
*         Copyright (c), Philips Semiconductors Gratkorn / Austria
*
*                     (C)PHILIPS Electronics N.V.2000
*       All rights are reserved. Reproduction in whole or in part is 
*      prohibited without the written consent of the copyright owner.
*  Philips reserves the right to make changes without notice at any time.
* Philips makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
*particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. Philips must not be liable for any loss or damage
*                          arising from its use.
*/

#ifndef MFRC500_H
#define MFRC500_H


// #include"Config.h"
#include "data_type.h"
//#include "REG52.h"
//#include "bus.h"


INT8S PcdConfig(void);
INT8S Request(INT8U req_code,INT8U *atq);
INT8S Anticoll (INT8U select_code,INT8U bcnt,INT8U *_SNR);
INT8S Select(INT8U select_code,INT8U *_SNR,INT8U *sak);
INT8S Load_Key(INT8U *keys);
INT8S Authentication(INT8U auth_mode,INT8U *_SNR,INT8U block);
INT8S Read(INT8U addr,INT8U *rec_data);
INT8S Write(INT8U addr,INT8U *rec_data);
INT8S ValueAndTrasfer(INT8U dd_mode,INT8U addr,INT8U *value,INT8U trans_addr);
INT8S Halt(void);
void ResetInfo(void);
void PcdSetTmo(INT8U tmoLength);
INT8S PcdSingleResponseCmd(INT8U cmd,INT8U  *send,INT8U *rcv);
// INT8U ActiveAntennaMasterConfig(void);
// INT8U ActivateIdle(INT8U br,INT8U *atq,INT8U *sak,INT8U *uid,INT8U *uid_len);
// INT8U ActivateWakeup(INT8U br,INT8U *atq,INT8U *sak,INT8U *uid,INT8U uid_len);
// INT8U Auth(INT8U auth_mode,INT8U key_sector,INT8U block);
// INT8U ValueDebit(INT8U dd_mode,INT8U addr,INT8U *value);

//CPU��ר��
INT8U ExchangeBlock(INT8U *send_data,INT8U send_bytelen,INT8U *rec_data,
                           INT8U *rec_bytelen,INT8U append_crc,INT16U  timeout );

//��дE2����
INT8U WriteE2(INT8U i, INT8U j);
INT8U ReadE2(void);
INT8U ReadE21(void);
INT8U ReadE22(void);


/******************************************************************************
 �������ƣ�CmdCPUCard
 ������������CPU����������൱����ǰ��exchange_t_block���˺�����������ݵĲ�����ͺͶԽ��յ������ݰ�����ϣ����Ĵ�СΪ16�ֽ�
 ����������
 �������ƣ�	����/�����	����		����
 snd_buf			����	INT8U *			���ͻ�����
 snd_length			����	INT8U			Ҫ��������ĳ���
 rcv_buf			���		INT8U *		PSAM���ͻص��ֽڻ�����
 rcv_length			���		INT8U *		PSAM���ͻص��ֽڳ���
 
 ��  ��  ֵ��CPU�����ص�״̬�룬��������CPU���޷����򷵻�0xC002���൱���rcv_buf����������ֽڸ���һ����Ϊ����ֵ
				   
 ��      ��	��������
 ��      �ڣ�2004-09-02
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
INT16U CmdCPUCard(INT8U const * const snd_buf, INT8U const snd_length, 
			   INT8U *rcv_buf, INT8U *rcv_length);



/*****************************************************************
 ����ԭ�ͣ�Get_Sector_Permission
 ����������ȡ�õ�N���Ķ�дȨ��
 ����������
 �������ƣ� ����/����� ����        ����
 sector     ����        INT8U       Ҫȡ��Ȩ�޵���
 key        ����        INT8U *     Ҫȡ��Ȩ��������Կ
 csn		����		INT8U *		Ӳ������

 ��  ��  ֵ��ok(0)-�ɹ�
				 notok(0xFF)-ʧ��

 ��      �� ��������
 ��      �ڣ�2003-12-19
 �޸���ʷ��
		����        �޸���      �޸�����
		------      ---------   -------------
*****************************************************************/
INT8S Get_Sector_Permission(INT8U sector, INT8U *key, INT8U *csn);



// // // PCD Configuration
//  INT8S Mf500PcdConfig(void);
// //
// // // Active Antenna Slave Configuration of the MF RC500.
//  INT8S Mf500ActiveAntennaSlaveConfig(void);
// //
// // // Active Antenna Master Configuration of the MF RC 500
//  INT8S Mf500ActiveAntennaMasterConfig(void);
// //
// // // Set default attributes for the baudrate divider
//  INT8S Mf500PcdSetDefaultAttrib(void);
// //
//  // Set attributes for the baudrate divider
//  INT8S Mf500PcdSetAttrib(INT8U DSI,
//                             INT8U DRI);
// //
// // // Get transmission properties of the PCD
//  INT8S Mf500PcdGetAttrib(INT8U *FSCImax,
//                            INT8U *FSDImax,
//                            INT8U *DSsupp,
//                            INT8U *DRsupp,
//                            INT8U *DREQDS);
//
// // // PICC Request command
//  INT8S Mf500PiccRequest(INT8U req_code,
//                         INT8U *atq);
// //
// // // PICC Request command for ISO 14443 A-4 Command set
//  INT8S Mf500PiccCommonRequest(INT8U req_code,
//                               INT8U *atq);
// //
// // // PICC Anticollision Command
//  INT8S Mf500PiccAnticoll (INT8U bcnt,
//                           INT8U *snr);
// //
//  // PICC Cascaded Anticollision Command
//  INT8S Mf500PiccCascAnticoll (INT8U select_code,
//                               INT8U bcnt,
//                               INT8U *snr);
// //
// // // PICC Select Command
//  INT8S Mf500PiccSelect(INT8U *snr,
//                        INT8U *sak);
//
// // // PICC Select Command
//  INT8S Mf500PiccCascSelect(INT8U select_code,
//                               INT8U *snr,
//                               INT8U *sak);
// //
// // // Activation of a PICC in IDLE mode
//  INT8S Mf500PiccActivateIdle(INT8U br,
//                             INT8U *atq,
//                             INT8U *sak,
//                             INT8U *uid,
//                             INT8U *uid_len);
// //
// // // Activation of all PICC's in the RF field
//  INT8S Mf500PiccActivateWakeup(INT8U br,
//                               INT8U *atq,
//                               INT8U *sak,
//                               INT8U *uid,
//                               INT8U uid_len);
// //
// // // MIFARE�  Authentication
//  INT8S Mf500PiccAuth(INT8U auth_mode,
//                        INT8U key_sector,
//                        INT8U block);
//
// // // MIFARE � Authentication with  keys stored  in the MF RC 500's EEPROM.
//  INT8S Mf500PiccAuthE2( INT8U auth_mode,
//                           INT8U *snr,
//                           INT8U key_sector,
//                           INT8U block);
// //
// // // Authentication Key Coding
//  INT8S Mf500HostCodeKey(INT8U *uncoded,
//                           INT8U *coded);
//
// // // Key Loading into the MF RC500's EEPROM.
//  INT8S Mf500PcdLoadKeyE2(INT8U key_type,
//                            INT8U sector,
//                            INT8U *uncoded_keys);
// //
// // // Authentication with direct key loading form the microcontroller
//  INT8S Mf500PiccAuthKey(INT8U auth_mode,
//                           INT8U *snr,
//                           INT8U *keys,
//                           INT8U sector);
// //
// // // PICC Read Block
//  INT8S Mf500PiccRead(INT8U addr,
//                      INT8U* _data);
// //
// // // PICC Read Block of variable length
//  INT8S Mf500PiccCommonRead(INT8U cmd,
//                               INT8U addr,
//                               INT8U datalen,
//                               INT8U *_data);
// //
// // // PICC Write Block
//  INT8S Mf500PiccWrite(INT8U addr,
//                          INT8U *_data);
// //
// // // PICC Write 4 Byte Block
//  INT8S Mf500PiccWrite4(INT8U addr,
//                           INT8U *_data);
// //
// // // PICC Write Block of variable length
//  INT8S Mf500PiccCommonWrite(INT8U cmd,
//                                INT8U addr,
//                                INT8U datalen,
//                                INT8U *_data);
// //
// // // PICC Value Block Operation
//  INT8S Mf500PiccValue(INT8U dd_mode,
//                         INT8U addr,
//                         INT8U *value,
//                         INT8U trans_addr);
// //
// // // PICC Value Block Operation for Cards with automatic transfer
//  INT8S Mf500PiccValueDebit(INT8U dd_mode,
//                               INT8U addr,
//                               INT8U *value);
// //
// // // Exchange Data Blocks PCD --> PICC --> PCD
//  INT8S Mf500PiccExchangeBlock(INT8U *send_data,
//                                 INT16U send_bytelen,
//                                 INT8U *rec_data,
//                                 INT16U *rec_bytelen,
//                                 INT8U append_crc,
//                                 INT32U timeout );
// //
// // // PICC Halt
//  INT8S Mf500PiccHalt(void);
// //
// // // Reset the reader ic
//  INT8S PcdReset(void);
// //
// // // Exchange Data Stream PCD --> PICC --> PCD
//  INT8S ExchangeByteStream(INT8U Cmd,
//                              INT8U *send_data,
//                              INT16U send_bytelen,
//                              INT8U *rec_data,
//                              INT16U *rec_bytelen);
// //
// // // Set RF communication timeout
//  INT8S PcdSetTmo(INT32U numberOfEtus);
// //
// // // Read Serial Number from Reader IC
//  INT8S PcdGetSnr(INT8U *snr);
// //
// // // Read EEPROM Memory Block
//  INT8S PcdReadE2(INT16U startaddr,
//                     INT8U length,
//                     INT8U* _data);
// //
// // // Writes data to the reader IC's EEPROM blocks.
//  INT8S PcdWriteE2(  INT16U startaddr,
//                        INT8U length,
//                        INT8U* _data);
// //
// // // Turns ON/OFF RF field
//  INT8S PcdRfReset(INT16U ms);

#endif
