/******************************************************************************
Copyright 2005 �����״�ͨ�������޹�˾
All rights riserved.

�ļ���  ��data_type.h
ģ�����ƣ����������е���������

��ǰ�汾��0.0.1
�޸���	��������
������ڣ�2005.04.06
����˵����create
******************************************************************************/
#ifndef __DATA_TYPE_H__
#define __DATA_TYPE_H__

#include "OS_CPU.H"

typedef unsigned char  	uchar;                   /* �޷���8λ���ͱ���                        */
typedef unsigned char  	uint8;                   /* �޷���8λ���ͱ���                        */
typedef signed   char  	int8;                    /* �з���8λ���ͱ���                        */
typedef unsigned short 	uint16;                  /* �޷���16λ���ͱ���                       */
typedef signed   short 	int16;                   /* �з���16λ���ͱ���                       */
typedef unsigned int   	uint32;                  /* �޷���32λ���ͱ���                       */
typedef signed   int   	int32;                   /* �з���32λ���ͱ���                       */
typedef float          	fp32;                    /* �����ȸ�������32λ���ȣ�                 */
typedef double         	fp64;                    /* ˫���ȸ�������64λ���ȣ�                 */



#ifndef QUEUE_DATA_TYPE
	#define QUEUE_DATA_TYPE     		uint8
#endif

typedef struct 
{
    QUEUE_DATA_TYPE     *Out;                   /* ָ���������λ��         */
    QUEUE_DATA_TYPE     *In;                    /* ָ����������λ��         */
    QUEUE_DATA_TYPE     *End;                   /* ָ��Buf�Ľ���λ��        */
    uint16              		NData;                  /* ���������ݸ���           */
    uint16              		MaxData;                /* ����������洢�����ݸ��� */
    uint8               		(* ReadEmpty)();        /* ���մ�����             */
    uint8               		(* WriteFull)();        /* д��������             */
    QUEUE_DATA_TYPE     Buf[1];                 /* �洢���ݵĿռ�           */
} DataQueue;

//typedef float AD_TYPE;
//typedef double AD_TYPE;


//���ػ�ʱ��ṹ
__packed typedef struct _TIME_
{
	INT8U century;
	INT8U year;
	INT8U month;
	INT8U day;
	INT8U hour;
	INT8U minute;
	INT8U second;
}_TIME, BUS_TIME;

////////////////////////////////////////////
//���ӹ���Ա������Ͻṹ����
/////////////////////////////////////////////
typedef union _MANAGER_NUMBER_
{
	INT8U setter_number[3];//����Ա���
	INT8U gather_number[3];//�ɼ�Ա���
	INT8U checker_number[3];//���Ա���
	INT8U signer_number[3];//ǩ��Ա���
	INT8U check_number[3];//����Ա���
}_MANAGER_NUMBER;

//CARD_INFO���õ���struct
typedef struct _CARD_NUM
{
	INT8U city_code[2];	//���д���
	INT8U calling_code[2];//��ҵ����
	INT8U serial_number[4];		//���к�
}CARD_NUM;

//��Ƭ��Ϣ�����׼�¼�ɴ˱�������
typedef struct _CARD_INFO
{
	union _CARD_NUMBER_
	{
		INT8U card_number[8];//����
		CARD_NUM detail_card_number;
	}_CARD_NUMBER;

	BUS_TIME card_in_time;							//����ʱ��
	INT8U card_type;								//������
	INT32U fare;									//���ѽ��
	INT32U display_fare;							//���ѽ��
	INT8U init_purchase_serial_num[2];				//��ʼ�Ŀ��ڽ������
	INT8U purchase_serial_num[2];					//��ʼ�Ŀ��ڽ������+1
	INT32U init_balance;							//ԭ���
	INT32U balance;									//���׺����
	INT8U card_purchase_type;						//��������
	INT8U tac[4];									//TAC��
	INT8U mac2[4];									//CPU�����ص�MAC2

	INT8U csn[4];									//Ӳ������
	INT8U authentication_code[4];					//����֤��

	INT8U card_random_num[4];						//CPU�������
	INT8U arithmetic_sign;							//�㷨��ʶ
	INT8U cpu_key_version;							//CPU����Կ�汾��

//  INT32U psam_purchase_serial_num;				//PSAM���������
	INT8U psam_purchase_serial_num[4];				//PSAM���������
	INT8U mac1[4];									//PSAM���������MAC1

	//����˾���������°������Ա����
	INT8U drv_card_type;							//˾��������

	INT8U station_number[2];//վ����
	INT8U checked_flag;//����־
	_MANAGER_NUMBER manager_number;//����Ա��� 

 	INT8U card_record_point;						//���Ѽ�¼��ָ��

	INT16U favor_times_balance;						//���׺�ʣ���Żݴ���  

	INT8U cpu_black_sign;							//CPU����ʹ�ñ�� 00H��δ��ʧ		01H���ѹ�ʧ		02H��������
	INT8U m1_cpu;									//��������CPU��,����M1�� M1=M1CARD	CPU=CPUCARD

	INT8U is_love_card;								//�Ƿ�Ϊ���Ŀ� ��=TRUE ����=FALSE

	INT8U kind_of_card;								//���࣬CPU_CARD=CPU��		M1_CARD=M1��
	BUS_TIME valid_time;							//��Ч��

}CARD_INFO;






#endif
