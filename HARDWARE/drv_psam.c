/******************************************************************************
Copyright 2005 �����״�ͨ���Ӽ������޹�˾
All rights riserved.

�ļ���	��drv_psam.c
ģ�����ƣ�PSAM���ͽӴ�ʽIC����������
���ܸ�Ҫ��PSAM���ͽӴ�ʽIC����������

ȡ���汾��0.0.1
�޸���	��������
������ڣ�2005.10.13
����˵����create

******************************************************************************/
#include "all.h"


//��PSAM���������õ���һЩcode����
// const  INT8U      ReadPOSNumber[5]=         {0x00,0xB0,0x96,0x00,0x06};                         //ȡPOS��
// const  INT8U      SELECT_SAM_ADF1[7]=       {0x00,0xA4,0x00,0x00,0x02,0x10,0x01};       //ѡ1001Ŀ¼
// const  INT8U      SELECT_SAM_ADF2[7]=       {0x00,0xA4,0x00,0x00,0x02,0x10,0x02};       //ѡ1002Ŀ¼
// const  INT8U      SELECT_SAM_ADF3[7]=       {0x00,0xA4,0x00,0x00,0x02,0x10,0x03};       //ѡ1003Ŀ¼
// const  INT8U      DES_INIT[5]=          {0x80,0x1A,0x45,0x01,0x10};                         //DES�����ʼ��
// const  INT8U      GetResponse[4]=           {0x00,0xC0,0x00,0x00};                                  //ȡ��Ӧ
// const  INT8U      DES_CRYPT[4]=             {0x80,0xFA,0x05,0x00};                               //DES����
// const  INT8U      READ_SAM_KEY_NUM[5]=  {0x00,0xb0,0x97,0x00,0x19};                      //����Կ�汾��
// const  INT8U      GET_MAC1[5]=          {0x80,0x70,0x00,0x00,0x24};                       //PSAM����MAC1
// const  INT8U      VERIFY_MAC2[5]=           {0x80,0x72,0x00,0x00,0x04};                         //��֤MAC2
// const  INT8U      GET_M1_KEY[5]=            {0x80,0xFC,0x00,0x01,0x11};                         //����M1��������Կ
// const  INT8U      TAC_INIT[5]=          {0x80,0x1A,0x44,0x01,0x10};                         //M1�����������У�TAC������ʼ��
// const  INT8U      CAL_TAC_M1[5]=            {0x80,0xFA,0x01,0x00,0x18};                         //M1�����׹����У�����TAC��

// //������
// INT8U const      ReadPOSNumber[5]=       	{0x00,0xB0,0x96,0x00,0x06};							//ȡPOS��
// INT8U const      SELECT_SAM_ADF1[7]=      {0x00,0xA4,0x00,0x00,0x02,0x10,0x01};		//ѡ1001Ŀ¼
// INT8U const      SELECT_SAM_ADF2[7]=      {0x00,0xA4,0x00,0x00,0x02,0x10,0x02};		//ѡ1002Ŀ¼
// INT8U const      SELECT_SAM_ADF3[7]=      {0x00,0xA4,0x00,0x00,0x02,0x10,0x03};		//ѡ1003Ŀ¼
// INT8U const      DES_INIT[5]=             		   {0x80,0x1A,0x45,0x01,0x10};							//DES�����ʼ��
// INT8U const      GetResponse[4]=        		{0x00,0xC0,0x00,0x00};									//ȡ��Ӧ
// INT8U const      DES_CRYPT[4]=   				 {0x80,0xFA,0x05,0x00};									 //DES����
// INT8U const      READ_SAM_KEY_NUM[5]=   {0x00,0xb0,0x97,0x00,0x19};						 //����Կ�汾��
// INT8U const      GET_MAC1[5]=            		  {0x80,0x70,0x00,0x00,0x24};						  //PSAM����MAC1
// INT8U const      VERIFY_MAC2[5]=          		{0x80,0x72,0x00,0x00,0x04};							//��֤MAC2
// // INT8U const    GET_M1_KEY[5]=                {0x80,0xFC,0x00,0x01,0x0E};                         //����M1��������Կ
// INT8U const   	  GET_M1_KEY[5]=          		{0x80,0xFC,0x01,0x01,0x0D};							//����M1��������Կ
// INT8U const      TAC_INIT[5]=             			{0x80,0x1A,0x44,0x01,0x10};							//M1�����������У�TAC������ʼ��
// INT8U const      CAL_TAC_M1[5]=          		{0x80,0xFA,0x01,0x00,0x10};							//M1�����׹����У�����TAC��
//
////������
////��PSAM���������õ���һЩcode����
//INT8U const       ReadPOSNumber[5]=         {0x00,0xB0,0x96,0x00,0x06};
//INT8U const       GetChallenge[5]=          {0x00,0x84,0x00,0x00,0x04};		//ȡPOS��
//INT8U const       SELECT_SAM_ADF1[7]=      {0x00,0xA4,0x00,0x00,0x02,0x10,0x01};		//ѡ1001Ŀ¼
//INT8U const       SELECT_SAM_ADF2[7]=      {0x00,0xA4,0x00,0x00,0x02,0x10,0x02};		//ѡ1002Ŀ¼
//INT8U const       SELECT_SAM_ADF3[7]=      {0x00,0xA4,0x00,0x00,0x02,0x10,0x03};		//ѡ1003Ŀ¼
//INT8U const       DES_INIT[5]=                     {0x80,0x1A,0x45,0x01,0x10};							//DES�����ʼ��
//INT8U const      GetResponse[4]=                {0x00,0xC0,0x00,0x00};									//ȡ��Ӧ
//INT8U const      DES_CRYPT[4]=                   {0x80,0xFA,0x05,0x00};									 //DES����
//INT8U const      READ_SAM_KEY_NUM[5]=   {0x00,0xb0,0x97,0x00,0x19};						 //����Կ�汾��
//INT8U const      GET_MAC1[5]=                     {0x80,0x70,0x00,0x00,0x24};						  //PSAM����MAC1
//INT8U const      VERIFY_MAC2[5]=                {0x80,0x72,0x00,0x00,0x04};							//��֤MAC2
//INT8U const      GET_M1_KEY[5]=                 {0x80,0xFC,0x01,0x01,0x11};							//����M1��������Կ
//INT8U const      TAC_INIT[5]=                       {0x80,0x1A,0x44,0x01,0x10};							//M1�����������У�TAC������ʼ��
//INT8U const      CAL_TAC_M1[5]=                 {0x80,0xFA,0x01,0x00,0x18};							//M1�����׹����У�����TAC��
//INT8U const      DESCOMMUNICATION1[5]=   {0x80,0xfa,0x05,0x00,0x20};						//CPU��д0017�ļ�
//INT8U const      DESCOMMUNICATION[4]=   {0x80,0xfa,0x05,0x00};									//DES����



//��PSAM���������õ���һЩcode����
INT8U const   ReadPOSNumber[5] =		{ 0x00, 0xB0, 0x96, 0x00, 0x06 };                     //ȡPOS��
INT8U const   SELECT_SAM_ADF1[7] =		{ 0x00, 0xA4, 0x00, 0x00, 0x02, 0x10, 0x01 };           //ѡ1001Ŀ¼
INT8U const   SELECT_SAM_ADF2[7] =		{ 0x00, 0xA4, 0x00, 0x00, 0x02, 0x10, 0x02 };           //ѡ1002Ŀ¼
INT8U const   SELECT_SAM_ADF3[7] =		{ 0x00, 0xA4, 0x00, 0x00, 0x02, 0x10, 0x03 };           //ѡ1003Ŀ¼
INT8U const   DES_INIT[5] =				{ 0x80, 0x1A, 0x45, 0x01, 0x10 };                     //DES�����ʼ��
INT8U const   GetResponse[4] =			{ 0x00, 0xC0, 0x00, 0x00 };                          //ȡ��Ӧ
INT8U const   DES_CRYPT[4] =				{ 0x80, 0xFA, 0x05, 0x00 };                          //DES����
INT8U const   READ_SAM_KEY_NUM[5] =		{ 0x00, 0xb0, 0x97, 0x00, 0x19 };                     //����Կ�汾��
INT8U const   GET_MAC1[5] =				{ 0x80, 0x70, 0x00, 0x00, 0x24 };                     //PSAM����MAC1
INT8U const   VERIFY_MAC2[5] =			{ 0x80, 0x72, 0x00, 0x00, 0x04 };                     //��֤MAC2
INT8U const   GET_M1_KEY[5] =			{ 0x80, 0xFC, 0x01, 0x01, 0x11 };                     //����M1��������Կ zibo
																					  //INT8U const   GET_M1_KEY[5]=			{0x80,0xFC,0x00,0x01,0x11};						//����M1��������Կ laiwu
INT8U const   TAC_INIT[5] =				{ 0x80, 0x1A, 0x44, 0x01, 0x10 };                     //M1�����������У�TAC������ʼ��
INT8U const   CAL_TAC_M1[5] =			{ 0x80, 0xFA, 0x01, 0x00, 0x18 };                     //M1�����׹����У�����TAC��
																					  //����ͨ��
INT8U const   SELECT_SAM_3F01[7] =		{ 0x00, 0xA4, 0x00, 0x00, 0x02, 0x3F, 0x01 };           //ѡ1001Ŀ¼
INT8U const   GET_M1_FUZA_KEY[5] =		{ 0x80, 0xFC, 0x0E, 0x0D, 0x19 };                     //����M1��������Կ
INT8U const   TAC_FUZA_INIT[5] =			{ 0x80, 0x1A, 0x44, 0x01, 0x10 };                     //M1�����������У�TAC������ʼ��
INT8U const   CAL_TAC_M1_FUZA[5] =		{ 0x80, 0xFA, 0x01, 0x00, 0x18 };                     //M1�����׹����У�����TAC��
																						  //INT8U const   GET_MI_FUZA_KEY_YINZI[8]= {0xD1,0xCC,0xCC,0xA8,0x00,0x00,0x00,0x00};	//M1�����׹����У�����TAC��
INT8U const   GET_MI_FUZA_KEY_YINZI[8] ={ 0xD7, 0xCD, 0xB2, 0xA9, 0x00, 0x00, 0x00, 0x00 };      //M1�����׹����У�����TAC��
INT8U const   FUZHA_TAC_INIT[5] =		{ 0x80, 0x1A, 0x48, 0x0A, 0x10 };                     //M1�����������У�TAC������ʼ��
INT8U const   FUZHA_CAL_TAC_M1[5] =		{ 0x80, 0xFA, 0x05, 0x00, 30 };                       //M1�����׹����У�����TAC��

#define CHANNL_PSAM_SLV_T		1
#define CHANNL_PSAM_SLV_R		2
#define CHANNL_PSAM_PRI_T		3
#define CHANNL_PSAM_PRI_R		4
#define CHANNL_PSAM_3_T			5
#define CHANNL_PSAM_3_R			6
#define CHANNL_PSAM_4_T			7
#define CHANNL_PSAM_4_R			8


#define  RST_PSAM_PRI_SetLow()  		GPIO_ResetBits(GPIOA, GPIO_Pin_1)
#define  RST_PSAM_PRI_SetHigh()			GPIO_SetBits(GPIOA, GPIO_Pin_1)		//PA.1 PSAM_RST1 �����
#define  PSAM_PRI_RST()					RST_PSAM_PRI_SetLow()
#define  PSAM_PRI_CLEAR_RST()			RST_PSAM_PRI_SetHigh()

//#define  RST_PSAM_SLV_SetLow()  		GPIO_ResetBits(GPIOC, GPIO_Pin_9)
//#define  RST_PSAM_SLV_SetHigh()		 	GPIO_SetBits(GPIOC, GPIO_Pin_9)		//PC.9 PSAM_RST2 �����
//#define  PSAM_SLV_RST()					RST_PSAM_SLV_SetLow()
//#define  PSAM_SLV_CLEAR_RST()			RST_PSAM_SLV_SetHigh()
//
//#define  RST_PSAM3_SetLow()  			GPIO_ResetBits(GPIOC, GPIO_Pin_8)
//#define  RST_PSAM3_SetHigh()		 	GPIO_SetBits(GPIOC, GPIO_Pin_8)		//PC.8 PSAM_RST3 �����
//#define  PSAM3_RST()					RST_PSAM3_SetLow()
//#define  PSAM3_CLEAR_RST()				RST_PSAM3_SetHigh()
//
//#define  RST_PSAM4_SetLow()  			GPIO_ResetBits(GPIOC, GPIO_Pin_7)
//#define  RST_PSAM4_SetHigh()		 	GPIO_SetBits(GPIOC, GPIO_Pin_7)		//PC.7 PSAM_RST4 �����
//#define  PSAM4_RST()					RST_PSAM4_SetLow()
//#define  PSAM4_CLEAR_RST()				RST_PSAM4_SetHigh()
//
//
//
//#define  EN4245_SetLow()			GPIO_ResetBits(GPIOC, GPIO_Pin_0)
//#define  EN4245_SetHigh()			GPIO_SetBits(GPIOC, GPIO_Pin_0)		//PC.0 4245EN �����
//
//#define  DIR4245_SetLow()			GPIO_ResetBits(GPIOC, GPIO_Pin_1)
//#define  DIR4245_SetHigh()			GPIO_SetBits(GPIOC, GPIO_Pin_1)		//PC.1 4245DIR �����
//
//#define  A4051_SetLow()				GPIO_ResetBits(GPIOC, GPIO_Pin_4)
//#define  A4051_SetHigh()			GPIO_SetBits(GPIOC, GPIO_Pin_4)		//PC.4 4051A �����
//
//#define  B4051_SetLow()				GPIO_ResetBits(GPIOC, GPIO_Pin_5)
//#define  B4051_SetHigh()			GPIO_SetBits(GPIOC, GPIO_Pin_5)		//PC.5 4051B �����
//
//#define  C4051_SetLow()				GPIO_ResetBits(GPIOB, GPIO_Pin_0)
//#define  C4051_SetHigh()			GPIO_SetBits(GPIOB, GPIO_Pin_0)		//PB.0 4051C �����
//
//#define  ENABLE_PSAM_BUS()			EN4245_SetLow()
//#define  DISABLE_PSAM_BUS()			EN4245_SetHigh()
//
//#define  PSAMbusSelectOut()  		DIR4245_SetLow()			//���߷���ѡ�����
//#define  PSAMbusSelectIn()		 	DIR4245_SetHigh()			//����


//extern int UART3RcvBuf[(UART3_RCV_QUEUE_LENGTH + sizeof(int)- 1)/ sizeof(int)];
//extern INT8U Com3SndBuf[UART3_SEND_QUEUE_LENGTH];              //���ڷ��ͻ�����
//extern INT8U Com3RcvBuf[UART3_RCV_QUEUE_LENGTH];               //���ڽ��ջ�����

extern int UART2RcvBuf[(UART2_RCV_QUEUE_LENGTH + sizeof(int)- 1)/ sizeof(int)];
extern INT8U Com2SndBuf[UART2_SEND_QUEUE_LENGTH];              //���ڷ��ͻ�����
extern INT8U Com2RcvBuf[UART2_RCV_QUEUE_LENGTH];               //���ڽ��ջ�����

/******************************************************************************
�������ƣ�psam_init
����������PSAM����ʼ��
����������

��  ��  ֵ	����
��      ��	��������
��      �ڣ�2005-04-08
�޸���ʷ��
����		�޸���		�޸�����
------		---------	---------
******************************************************************************/
void psam_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	ToggleWD();

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ��PA�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;                //PSAM_RST1-->PA.1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;         //IO���ٶ�Ϊ2MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);                   //�����趨������ʼ��GPIO
	GPIO_SetBits(GPIOA, GPIO_Pin_1);                         //PA.1�����

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                //PSAM_RST2-->PC.9
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         //�������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;         //IO���ٶ�Ϊ2MHz
//	GPIO_Init(GPIOC, &GPIO_InitStructure);                   //�����趨������ʼ��GPIO
//	GPIO_SetBits(GPIOC, GPIO_Pin_9);                         //PC.9�����
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;                //PSAM_RST3-->PC.8
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         //�������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;         //IO���ٶ�Ϊ2MHz
//	GPIO_Init(GPIOC, &GPIO_InitStructure);                   //�����趨������ʼ��GPIO
//	GPIO_SetBits(GPIOC, GPIO_Pin_8);                         //PC.9�����
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;                //PSAM_RST4-->PC.7
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         //�������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;         //IO���ٶ�Ϊ2MHz
//	GPIO_Init(GPIOC, &GPIO_InitStructure);                   //�����趨������ʼ��GPIO
//	GPIO_SetBits(GPIOC, GPIO_Pin_7);                         //PC.9�����
//
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                //4245EN-->PC.0
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         //�������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;         //IO���ٶ�Ϊ2MHz
//	GPIO_Init(GPIOC, &GPIO_InitStructure);                   //�����趨������ʼ��GPIO
//	GPIO_SetBits(GPIOC, GPIO_Pin_0);                         //PC.0�����
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;                //4245DIR-->PC.1
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         //�������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;         //IO���ٶ�Ϊ2MHz
//	GPIO_Init(GPIOC, &GPIO_InitStructure);                   //�����趨������ʼ��GPIO
//	GPIO_SetBits(GPIOC, GPIO_Pin_1);                         //PC.1�����
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                //4051A-->PC.4
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         //�������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;         //IO���ٶ�Ϊ2MHz
//	GPIO_Init(GPIOC, &GPIO_InitStructure);                   //�����趨������ʼ��GPIO
//	GPIO_SetBits(GPIOC, GPIO_Pin_4);                         //PC.4�����
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;                //4051B-->PC.5
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         //�������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;         //IO���ٶ�Ϊ2MHz
//	GPIO_Init(GPIOC, &GPIO_InitStructure);                   //�����趨������ʼ��GPIO
//	GPIO_SetBits(GPIOC, GPIO_Pin_5);                         //PC.5�����
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                //4051C-->PB.0
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         //�������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;         //IO���ٶ�Ϊ2MHz
//	GPIO_Init(GPIOB, &GPIO_InitStructure);                   //�����趨������ʼ��GPIO
//	GPIO_SetBits(GPIOB, GPIO_Pin_0);                         //PB.0�����


//	DISABLE_PSAM_BUS();
//	PSAMbusSelectIn();
	uart2_set_bps(38400, 9, 1, EVEN);
	ChangePsamChannel(CHANNL_PSAM_PRI_R);
}

/******************************************************************************
 �������ƣ�PsamRst
 ������������λPSAM��
 ����������
 �������ƣ�		����/�����	����		����
 psam_channel	����		INT8U		Ҫ��λPSAM��ͨ�� =PSAM_PRI��=PSAM_SLV
 rcv_buf		���		INT8U *		��λ���ص�����
 rcv_length		���		INT16U * 	��λ�������ݵĳ���
				
 ��  ��  ֵ��ok(0)-�ɹ�
				 notok(0xFF)-ʧ��
				   
 ��      ��	��������
 ��      �ڣ�2004-09-02
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
INT8U PsamRst(const INT8U psam_channel, INT8U *rcv_buf, INT16U *rcv_length)
{
	INT8U i = 0;

	ToggleWD();

	//�������
//	if ( !( (psam_channel == PSAM_PRI) || (psam_channel == PSAM_SLV) || (psam_channel == PSAM3) || (psam_channel == PSAM4) ) )
	if ( (psam_channel != PSAM_PRI) )
		return ( notok );

//	ENABLE_PSAM_BUS();

//	if ( psam_channel == PSAM_PRI )
//	{
		ChangePsamChannel(CHANNL_PSAM_PRI_R);                   //�������ӣ�������38400��ͨ��Ϊ��PSAM������ͨ��
//		PSAMbusSelectIn();

		PSAM_PRI_RST();
		SleepMs(3);
		PSAM_PRI_CLEAR_RST();
//	}
//	else if ( psam_channel == PSAM_SLV )
//	{
//		ChangePsamChannel(CHANNL_PSAM_SLV_R);                   //�������ӣ�������38400��ͨ��Ϊ��PSAM������ͨ��
//		PSAMbusSelectIn();
//
//		PSAM_SLV_RST();
//		SleepMs(3);
//		PSAM_SLV_CLEAR_RST();
//	}
//	else if ( psam_channel == PSAM3 )
//	{
//		ChangePsamChannel(CHANNL_PSAM_3_R);                   //�������ӣ�������38400��ͨ��ΪPSAM3����ͨ��
//		PSAMbusSelectIn();
//
//		PSAM3_RST();
//		SleepMs(3);
//		PSAM3_CLEAR_RST();
//	}
//	else
//	{
//		ChangePsamChannel(CHANNL_PSAM_4_R);                   //�������ӣ�������38400��ͨ��ΪPSAM4����ͨ��
//		PSAMbusSelectIn();
//
//		PSAM4_RST();
//		SleepMs(3);
//		PSAM4_CLEAR_RST();
//	}


	i = com2_gets(rcv_buf, rcv_length, PSAM_RD_DELAY, 5);
//	DISABLE_PSAM_BUS();
	if ( i != ok )
		return ( notok );
	if ( (*rcv_buf == 0x3B) && ((*(rcv_buf + 1) & 0xF0) == 0x60) )                 //��ͷ�ֽڱ���Ϊ 0x3B6?
	{
		i = rcv_buf[1] & 0x0F;
		if ( *rcv_length == (i + 4) )                                         //�յ��ĳ�����ȷ
			return ( ok );
		else
			return ( notok );
	}
	else
	{
		return ( notok );
	}
}

///******************************************************************************
// �������ƣ�Cmd7816
// ����������ʹ��T=0Э����PSAM����Ӵ�ʽIC������ͨѶ������̨��д��������0x6C(���ͳ��Ȳ���)�Ĵ���
// ����������
// �������ƣ� 	����/����� ����        ����
// psam_channel	����    	INT8U const ͨ����=PSAM_PRI��=PSAM_SLV
// snd_buf		����    	INT8U *     ���ͻ�����
// snd_length		����    	INT16U      Ҫ��������ĳ���
// rcv_buf		���        INT8U *     PSAM���ͻص��ֽڻ�����
// rcv_length		���        INT16U *    PSAM���ͻص��ֽڳ���
//
// ��  ��  ֵ��SAM�����ص�״̬�룬��������PSAM���޷����򷵻�0xC002���൱���rcv_buf����������ֽڸ���һ����Ϊ����ֵ
//
// ��      �� ��������
// ��      �ڣ�2004-09-02
// �޸���ʷ��
//		����        �޸���      �޸�����
//		------      ---------   -------------
//******************************************************************************/
//INT16U Cmd7816(const INT8U psam_channel, const INT8U * const snd_buf, const INT16U snd_length,
//			   INT8U *rcv_buf, INT16U *rcv_length)
//{
//	INT8U i = 0;
//	INT8U temp_buf[50];                 //��ʱ�ķ��ͺͽ��ջ�����
//	INT16U psam_status = 0;                     //PSAM������״̬���ݴ�
//	INT8U cur_psam_channel;
//
//	ToggleWD();
//
//	//�������
////	if ( (psam_channel != PSAM_PRI) && (psam_channel != PSAM_SLV) )
//	if ( !( (psam_channel == PSAM_PRI) || (psam_channel == PSAM_SLV) || (psam_channel == PSAM3) || (psam_channel == PSAM4) ) )
//		return ( 0xC002 );
//
//	if ( snd_length < 4 )                                     //���͵��ֽڳ��ȱ���>=4
//		return ( 0xC002 );
//
//	ENABLE_PSAM_BUS();
//
//	if ( psam_channel == PSAM_PRI )                                         //Ϊ�˼�С������������ComCreateConnctn�������ô˱���
//		cur_psam_channel = CHANNL_PSAM_PRI_T;
//	else if ( psam_channel == PSAM_SLV )
//		cur_psam_channel = CHANNL_PSAM_SLV_T;
//	else if ( psam_channel == PSAM3 )
//		cur_psam_channel = CHANNL_PSAM_3_T;
//	else
//		cur_psam_channel = CHANNL_PSAM_4_T;
//
//
//	if ( snd_length == 4 )                           //��PSAM�����������Ϊ4�Ĵ���
//	{
//		ChangePsamChannel(cur_psam_channel);                    //�������ӣ�������38400��ͨ��Ϊ��PSAM������ͨ��
//		memcpy(temp_buf, snd_buf, 4);
//		temp_buf[5] = 0x00;                             //T=0Э�����1������5�ֽڵ����0
//
//		com3_send_psam((void *)snd_buf, 5);
//
//		ChangePsamChannel(cur_psam_channel + 1);                    //�������ӣ�������38400��ͨ��Ϊ��PSAM������ͨ��
//
//		i = com3_gets(rcv_buf, rcv_length, PSAM_RD_DELAY, 5);
//		if ( i != ok )
//		{
//			DISABLE_PSAM_BUS();
//			return ( 0xC002 );
//		}
//		*rcv_length = 2;
//		psam_status = rcv_buf[0];
//		psam_status = (psam_status << 8) + rcv_buf[1];
//		return ( psam_status );
//	}
//	else if ( snd_length == 5 )                          //��PSAM�����������Ϊ5�Ĵ���
//	{
//		ChangePsamChannel(cur_psam_channel);                    //�������ӣ�������38400��ͨ��Ϊ��PSAM������ͨ��
//		com3_send_psam((void *)snd_buf, 5);
//		ChangePsamChannel(cur_psam_channel + 1);                    //�������ӣ�������38400��ͨ��Ϊ��PSAM������ͨ��
//
//		i = com3_gets(rcv_buf, rcv_length, PSAM_RD_DELAY, 5);
//		if ( i != ok )
//		{
//			DISABLE_PSAM_BUS();
//			return ( 0xC002 );
//		}
//		if ( (*rcv_length > 2) && (rcv_buf[0] != snd_buf[1]) )      //PSAM�����ش��󣬷��صĵ�һ���ֽ�Ӧ�õ��ڷ��͵ĵڶ����ֽ�
//		{
//			DISABLE_PSAM_BUS();
//			return ( 0xC002 );
//		}
//		psam_status = rcv_buf[*rcv_length - 2];
//		psam_status = (psam_status << 8) + rcv_buf[*rcv_length - 1];
//		return ( psam_status );
//	}
//	else                                                     //��PSAM�����������>5�Ĵ���
//	{
//		ChangePsamChannel(cur_psam_channel);                    //�������ӣ�������38400��ͨ��Ϊ��PSAM������ͨ��
//		com3_send_psam((void *)snd_buf, 5);
//		ChangePsamChannel(cur_psam_channel + 1);                    //�������ӣ�������38400��ͨ��Ϊ��PSAM������ͨ��
//
//		i = com3_gets(rcv_buf, rcv_length, PSAM_RD_DELAY, 5);
//		if ( i != ok )
//		{
//			DISABLE_PSAM_BUS();
//			return ( 0xC002 );
//		}
//		if ( *rcv_length == 1 )                                                                       //��������
//		{
//			ChangePsamChannel(cur_psam_channel);                    //�������ӣ�������38400��ͨ��Ϊ��PSAM������ͨ��
//			com3_send_psam((void *)(snd_buf + 5), snd_length - 5);                        //��������
//			ChangePsamChannel(cur_psam_channel + 1);                    //�������ӣ�������38400��ͨ��Ϊ��PSAM������ͨ��
//
//			i = com3_gets(rcv_buf, rcv_length, PSAM_RD_DELAY, 5);
//			if ( i != ok )
//			{
//				DISABLE_PSAM_BUS();
//				return ( 0xC002 );
//			}
//			if ( *rcv_length == 2 )                                                                       //��������
//			{
//				if ( rcv_buf[0] == 0x61 )                                                            //PSAM��Ҫ��ȡ��Ӧ
//				{
//					memcpy(temp_buf, GetResponse, 4);
//					temp_buf[4] = rcv_buf[1];
//
//					ChangePsamChannel(cur_psam_channel);                    //�������ӣ�������38400��ͨ��Ϊ��PSAM������ͨ��
//					com3_send_psam((void *)temp_buf, 5);
//					ChangePsamChannel(cur_psam_channel + 1);                    //�������ӣ�������38400��ͨ��Ϊ��PSAM������ͨ��
//
//					i = com3_gets(rcv_buf, rcv_length, PSAM_RD_DELAY, 5);
//					if ( i != ok )
//					{
//						DISABLE_PSAM_BUS();
//						return ( 0xC002 );
//					}
//					if ( (*rcv_length > 2) && rcv_buf[0] == temp_buf[1] )           //�жϷ����Ƿ���ȷ
//					{
//						psam_status = rcv_buf[*rcv_length - 2];
//						psam_status = (psam_status << 8) + rcv_buf[*rcv_length - 1];
//						return ( psam_status );
//					}
//					else
//					{
//						DISABLE_PSAM_BUS();
//						return ( 0xC002 );                                                              //���ظ�ʽ����
//					}
//
//				}
//				else
//				{
//					psam_status = rcv_buf[*rcv_length - 2];
//					psam_status = (psam_status << 8) + rcv_buf[*rcv_length - 1];
//					return ( psam_status );
//				}
//			}
//			else                                                                                                //���ȴ���
//			{
//				DISABLE_PSAM_BUS();
//				return ( 0xC002 );
//			}
//		}
//		else if ( *rcv_length == 2 )                                                              //����״̬��
//		{
//			psam_status = rcv_buf[*rcv_length - 2];
//			psam_status = (psam_status << 8) + rcv_buf[*rcv_length - 1];
//			DISABLE_PSAM_BUS();
//			return ( psam_status );
//		}
//		else                                                                                                //PSAM�����س��ȴ���
//		{
//			DISABLE_PSAM_BUS();
//			return ( 0xC002 );
//		}
//	}
//}

/******************************************************************************
 �������ƣ�Cmd7816
 ����������ʹ��T=0Э����PSAM����Ӵ�ʽIC������ͨѶ������̨��д��������0x6C(���ͳ��Ȳ���)�Ĵ���
 ����������
 �������ƣ� 	����/����� ����        ����
 psam_channel	����    	INT8U const ͨ����=PSAM_PRI��=PSAM_SLV
 snd_buf		����    	INT8U *     ���ͻ�����
 snd_length		����    	INT16U      Ҫ��������ĳ���
 rcv_buf		���        INT8U *     PSAM���ͻص��ֽڻ�����
 rcv_length		���        INT16U *    PSAM���ͻص��ֽڳ���

 ��  ��  ֵ��SAM�����ص�״̬�룬��������PSAM���޷����򷵻�0xC002���൱���rcv_buf����������ֽڸ���һ����Ϊ����ֵ

 ��      �� ��������
 ��      �ڣ�2004-09-02
 �޸���ʷ��
		����        �޸���      �޸�����
		------      ---------   -------------
******************************************************************************/
INT16U Cmd7816(const INT8U psam_channel, const INT8U * const snd_buf, const INT16U snd_length,
			   INT8U *rcv_buf, INT16U *rcv_length)
{
	INT8U i = 0;
	INT8U temp_buf[50];                 //��ʱ�ķ��ͺͽ��ջ�����
	INT16U psam_status = 0;                     //PSAM������״̬���ݴ�
	INT8U cur_psam_channel;

	ToggleWD();

	//�������
//	if ( !( (psam_channel == PSAM_PRI) || (psam_channel == PSAM_SLV) || (psam_channel == PSAM3) || (psam_channel == PSAM4) ) )
	if ( (psam_channel != PSAM_PRI) )
		return ( 0xC002 );

	if ( snd_length < 4 )                                     //���͵��ֽڳ��ȱ���>=4
		return ( 0xC002 );

//	ENABLE_PSAM_BUS();

	if ( psam_channel == PSAM_PRI )                                         //Ϊ�˼�С������������ComCreateConnctn�������ô˱���
		cur_psam_channel = CHANNL_PSAM_PRI_T;
	else if ( psam_channel == PSAM_SLV )
		cur_psam_channel = CHANNL_PSAM_SLV_T;
	else if ( psam_channel == PSAM3 )
		cur_psam_channel = CHANNL_PSAM_3_T;
	else
		cur_psam_channel = CHANNL_PSAM_4_T;
	

	if ( snd_length == 4 )                           //��PSAM�����������Ϊ4�Ĵ���
	{
		ChangePsamChannel(cur_psam_channel);                    //�������ӣ�������38400��ͨ��Ϊ��PSAM������ͨ��
		memcpy(temp_buf, snd_buf, 4);
		temp_buf[5] = 0x00;                             //T=0Э�����1������5�ֽڵ����0

		com2_send_psam((void *)snd_buf, 5);

		ChangePsamChannel(cur_psam_channel + 1);                    //�������ӣ�������38400��ͨ��Ϊ��PSAM������ͨ��

		i = com2_gets(rcv_buf, rcv_length, PSAM_RD_DELAY, 5);
		if ( i != ok )
		{
//			DISABLE_PSAM_BUS();
			return ( 0xC002 );
		}
		*rcv_length = 2;
		psam_status = rcv_buf[0];
		psam_status = (psam_status << 8) + rcv_buf[1];
		return ( psam_status );
	}
	else if ( snd_length == 5 )                          //��PSAM�����������Ϊ5�Ĵ���
	{
		ChangePsamChannel(cur_psam_channel);                    //�������ӣ�������38400��ͨ��Ϊ��PSAM������ͨ��
		com2_send_psam((void *)snd_buf, 5);
		ChangePsamChannel(cur_psam_channel + 1);                    //�������ӣ�������38400��ͨ��Ϊ��PSAM������ͨ��

		i = com2_gets(rcv_buf, rcv_length, PSAM_RD_DELAY, 5);
		if ( i != ok )
		{
//			DISABLE_PSAM_BUS();
			return ( 0xC002 );
		}
		if ( (*rcv_length > 2) && (rcv_buf[0] != snd_buf[1]) )      //PSAM�����ش��󣬷��صĵ�һ���ֽ�Ӧ�õ��ڷ��͵ĵڶ����ֽ�
		{
//			DISABLE_PSAM_BUS();
			return ( 0xC002 );
		}
		psam_status = rcv_buf[*rcv_length - 2];
		psam_status = (psam_status << 8) + rcv_buf[*rcv_length - 1];
		return ( psam_status );
	}
	else                                                     //��PSAM�����������>5�Ĵ���
	{
		ChangePsamChannel(cur_psam_channel);                    //�������ӣ�������38400��ͨ��Ϊ��PSAM������ͨ��
		com2_send_psam((void *)snd_buf, 5);
		ChangePsamChannel(cur_psam_channel + 1);                    //�������ӣ�������38400��ͨ��Ϊ��PSAM������ͨ��

//		i = com3_gets(rcv_buf, rcv_length, PSAM_RD_DELAY, 5);
		i = com2_get_2ch(rcv_buf, rcv_length, PSAM_RD_DELAY, 5);
		if ( i != ok )
		{
//			DISABLE_PSAM_BUS();
			return ( 0xC002 );
		}
		if ( *rcv_length == 1 )                                                                       //��������
		{
			ChangePsamChannel(cur_psam_channel);                    //�������ӣ�������38400��ͨ��Ϊ��PSAM������ͨ��
			com2_send_psam((void *)(snd_buf + 5), snd_length - 5);                        //��������
			ChangePsamChannel(cur_psam_channel + 1);                    //�������ӣ�������38400��ͨ��Ϊ��PSAM������ͨ��

//			i = com3_gets(rcv_buf, rcv_length, PSAM_RD_DELAY, 5);
			i = com2_get_2ch(rcv_buf, rcv_length, PSAM_RD_DELAY, 5);
			if ( i != ok )
			{
//				DISABLE_PSAM_BUS();
				return ( 0xC002 );
			}
			if ( *rcv_length == 2 )                                                                       //��������
			{
				if ( rcv_buf[0] == 0x61 )                                                            //PSAM��Ҫ��ȡ��Ӧ
				{
					memcpy(temp_buf, GetResponse, 4);
					temp_buf[4] = rcv_buf[1];

					ChangePsamChannel(cur_psam_channel);                    //�������ӣ�������38400��ͨ��Ϊ��PSAM������ͨ��
					com2_send_psam((void *)temp_buf, 5);
					ChangePsamChannel(cur_psam_channel + 1);                    //�������ӣ�������38400��ͨ��Ϊ��PSAM������ͨ��

					i = com2_gets(rcv_buf, rcv_length, PSAM_RD_DELAY, 5);
					if ( i != ok )
					{
//						DISABLE_PSAM_BUS();
						return ( 0xC002 );
					}
					if ( (*rcv_length > 2) && rcv_buf[0] == temp_buf[1] )           //�жϷ����Ƿ���ȷ
					{
						psam_status = rcv_buf[*rcv_length - 2];
						psam_status = (psam_status << 8) + rcv_buf[*rcv_length - 1];
						return ( psam_status );
					}
					else
					{
//						DISABLE_PSAM_BUS();
						return ( 0xC002 );                                                              //���ظ�ʽ����
					}

				}
				else
				{
					psam_status = rcv_buf[*rcv_length - 2];
					psam_status = (psam_status << 8) + rcv_buf[*rcv_length - 1];
					return ( psam_status );
				}
			}
			else                                                                                                //���ȴ���
			{
//				DISABLE_PSAM_BUS();
				return ( 0xC002 );
			}
		}
		else if ( *rcv_length == 2 )                                                              //����״̬��
		{
			psam_status = rcv_buf[*rcv_length - 2];
			psam_status = (psam_status << 8) + rcv_buf[*rcv_length - 1];
//			DISABLE_PSAM_BUS();
			return ( psam_status );
		}
		else                                                                                                //PSAM�����س��ȴ���
		{
//			DISABLE_PSAM_BUS();
			return ( 0xC002 );
		}
	}
}

/******************************************************************************
 �������ƣ�ChangePsamChannel
 �����������ı�psam��ͨ������
 ����������
 �������ƣ�	����/�����	����		����
 com_channel	����	INT8U	Ҫ��Ϊ��ͨ��
				
 ��  ��  ֵ����
				   
 ��      ��	��������
 ��      �ڣ�2004-09-02
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void ChangePsamChannel(INT8U com_channel)
{
	com_channel = com_channel;				//avoid warning

//	ToggleWD();
//
//
//	switch ( com_channel )
//	{
//		case CHANNL_PSAM_PRI_T:                 //��PSAM������ͨ��(PSAM1)
//			A4051_SetLow();
//			B4051_SetLow();
//			C4051_SetLow();
//			break;
//
//		case CHANNL_PSAM_PRI_R:                 //��PSAM������ͨ��(PSAM1)
//			A4051_SetHigh();
//			B4051_SetLow();
//			C4051_SetLow();
//			break;
//
//		case CHANNL_PSAM_SLV_T:                 //��PSAM������ͨ��(PSAM2)
//			A4051_SetLow();
//			B4051_SetHigh();
//			C4051_SetLow();
//			break;
//
//		case CHANNL_PSAM_SLV_R:                 //��PSAM������ͨ��(PSAM2)
//			A4051_SetHigh();
//			B4051_SetHigh();
//			C4051_SetLow();
//			break;
//
//		case CHANNL_PSAM_3_T:                  //PSAM3����ͨ��(PSAM3)
//			A4051_SetLow();
//			B4051_SetLow();
//			C4051_SetHigh();
//			break;
//
//		case CHANNL_PSAM_3_R:                  //PSAM3����ͨ��(PSAM3)
//			A4051_SetHigh();
//			B4051_SetLow();
//			C4051_SetHigh();
//			break;
//
//		case CHANNL_PSAM_4_T:                  //PSAM4����ͨ��(PSAM4)
//			A4051_SetLow();
//			B4051_SetHigh();
//			C4051_SetHigh();
//			break;
//
//		case CHANNL_PSAM_4_R:                  //PSAM4����ͨ��(PSAM4)
//			A4051_SetHigh();
//			B4051_SetHigh();
//			C4051_SetHigh();
//			break;
//
//
//		default:                                //Ĭ��Ϊ��PSAM������ͨ��(PSAM1)
//			A4051_SetHigh();
//			B4051_SetLow();
//			C4051_SetLow();
//			break;
//	}

}

/******************************************************************************
 �������ƣ�com2_send_psam
 ��������������2����һ�����ݸ�psam��
 ����������
 �������ƣ� ����/����� ����		����
 str_to_send	����	INT8U *		Ҫ���͵��ַ���
 len			����	INT16U		����
				
 ��  ��  ֵ����
				   
 �� 	 �� ��������
 �� 	 �ڣ�2005-04-07
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void com2_send_psam(INT8U *str_to_send, INT16U len)
{
	INT16U i = 0;

	ToggleWD();

	USART_ITConfig(USART2, USART_IT_TXE, DISABLE);      //�ط����ж�
//	PSAMbusSelectOut();
	for ( i = 0; i < len; i++ )
	{
		uart2_putch_psam(str_to_send[i]);
	}
//	PSAMbusSelectIn();
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);  /* �������ж� */
}

/******************************************************************************
 �������ƣ�uart2_putch_psam
 ��������������2����һ���ֽ�����
 ����������
 �������ƣ� ����/����� ����		����
 dat		����		INT8U		Ҫ���͵�����
				
 ��  ��  ֵ����
				   
 �� 	 �� ��������
 �� 	 �ڣ�2012-04-06
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void uart2_putch_psam(INT8U dat)
{
	USART_SendData(USART2, dat);
	while ( USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET )           //�ȴ����ݷ������
	{
		;
	}

}

/*****************************************************************
����ԭ�ͣ�com2_getch
��������������2����1���ֽڣ�����ʱ����
����������
�������ƣ�		����/�����	����		����
-----------		-----------	------	   	-------
str_got			���		INT8U*		�õ����ַ���
timeout_start	����		INT32U		���ֳ�ʱ����λmS��0����

 ��  ��  ֵ��ok(0)-�ɹ�
				 KEY_CANCEL-���˳�����
				 notok(0xFF)-ʧ�ܣ���ʱ
��      ��	��������
��      �ڣ�2005-08-20
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
INT8U com2_getch(INT8U *str_got, INT32U timeout_start)
{
	INT8U i = 0;
//	INT16U len = 0;
	INT32U timeout_cntr = 0;

//	ToggleWD();

	//��һ���ֽ�
	for(;;)
	{
		if( QueueRead(&i, UART2RcvBuf) == QUEUE_OK )
		{
			str_got[0] = i;
			return ok;
		}
		else
		{
			if (timeout_start == 0)
			{
				SleepMs(1);
			}
			else
			{
				timeout_cntr++;
				if (timeout_cntr > timeout_start)          //��ʱ
				{
					str_got[0] = '\0';
					return notok;
				}
				SleepMs(1);
			}
		}
	}

}

/*****************************************************************
����ԭ�ͣ�com2_get_2ch
��������������2����1-2���ֽڣ�psam��ͨѶר��
����������
�������ƣ�		����/�����	����		����
-----------		-----------	------	   	-------
str_got			���		INT8U*		�õ����ַ��� 
rcv_len			���		INT16U*		���ճ��� 
timeout_start	����		INT32U		���ֳ�ʱ����λmS��0����
timeout			����		INT16U		�����ֽڼ����ʱ����λmS

 ��  ��  ֵ��ok(0)-�ɹ�
				 KEY_CANCEL-���˳�����
				 notok(0xFF)-ʧ�ܣ���ʱ
��      ��	��������
��      �ڣ�2005-08-20
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
INT8U com2_get_2ch(INT8U *str_got, INT16U *rcv_len, INT32U timeout_start, INT16U timeout)
{
	INT8U i = 0;
//	INT16U len = 0;
//	INT32U timeout_cntr = 0;
	INT8U buf[10];

//	ToggleWD();
	timeout = timeout;			//avoid warning

	i = com2_getch(buf, timeout_start);
	if (i != ok)
	{
		str_got[0] = '\0';
		*rcv_len = 0;
		return notok;
	}

	if ( (buf[0] & 0xF0) == 0x60 || (buf[0] & 0xF0) == 0x90 )
	{
		//0x6X��0x9XҪ����2���ֽ�
		i = com2_getch(&buf[1], 1);
		if (i != ok)
		{
			memcpy(str_got, buf, 1);
			*rcv_len = 1;
			return ok;
		}

		memcpy(str_got, buf, 2);
		*rcv_len = 2;
		return ok;
	}

	memcpy(str_got, buf, 1);
	*rcv_len = 1;
	return ok;
}

/******************************************************************************
 �������ƣ�psam_rst_fast
 �������������ٸ�λPSAM��
 ����������
 �������ƣ�		����/�����	����		����
 psam_channel	����		INT8U		Ҫ��λPSAM��ͨ�� =PSAM_PRI��=PSAM_SLV
 rcv_buf		���		INT8U *		��λ���ص�����
 rcv_length		���		INT16U * 	��λ�������ݵĳ���
				
 ��  ��  ֵ��ok(0)-�ɹ�
				 notok(0xFF)-ʧ��
				   
 ��      ��	��������
 ��      �ڣ�2004-09-02
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
INT8U psam_rst_fast(const INT8U psam_channel, INT8U *rcv_buf, INT16U *rcv_length)
{
	INT8U i = 0;

	ToggleWD();

	//�������
//	if ( !( (psam_channel == PSAM_PRI) || (psam_channel == PSAM_SLV) || (psam_channel == PSAM3) || (psam_channel == PSAM4) ) )
	if ( (psam_channel != PSAM_PRI) )
		return ( notok );

//	ENABLE_PSAM_BUS();
//
//	if ( psam_channel == PSAM_PRI )
//	{
		ChangePsamChannel(CHANNL_PSAM_PRI_R);                   //�������ӣ�������38400��ͨ��Ϊ��PSAM������ͨ��
//		PSAMbusSelectIn();

		PSAM_PRI_RST();
		SleepMs(1);
		PSAM_PRI_CLEAR_RST();
//	}
//	else if ( psam_channel == PSAM_SLV )
//	{
//		ChangePsamChannel(CHANNL_PSAM_SLV_R);                   //�������ӣ�������38400��ͨ��Ϊ��PSAM������ͨ��
//		PSAMbusSelectIn();
//
//		PSAM_SLV_RST();
//		SleepMs(1);
//		PSAM_SLV_CLEAR_RST();
//	}
//	else if ( psam_channel == PSAM3 )
//	{
//		ChangePsamChannel(CHANNL_PSAM_3_R);                   //�������ӣ�������38400��ͨ��ΪPSAM3����ͨ��
//		PSAMbusSelectIn();
//
//		PSAM3_RST();
//		SleepMs(1);
//		PSAM3_CLEAR_RST();
//	}
//	else
//	{
//		ChangePsamChannel(CHANNL_PSAM_4_R);                   //�������ӣ�������38400��ͨ��ΪPSAM4����ͨ��
//		PSAMbusSelectIn();
//
//		PSAM4_RST();
//		SleepMs(1);
//		PSAM4_CLEAR_RST();
//	}


//	i = com3_gets(rcv_buf, rcv_length, PSAM_RD_DELAY, 5);
	i = com2_gets2(rcv_buf, rcv_length, PSAM_RD_DELAY, 5);
//	DISABLE_PSAM_BUS();
	if ( i != ok )
		return ( notok );
	if ( (*rcv_buf == 0x3B) && ((*(rcv_buf + 1) & 0xF0) == 0x60) )                 //��ͷ�ֽڱ���Ϊ 0x3B6?
	{
		i = rcv_buf[1] & 0x0F;
		if ( *rcv_length == (i + 4) )                                         //�յ��ĳ�����ȷ
			return ( ok );
		else
			return ( notok );
	}
	else
	{
		return ( notok );
	}
}

/*****************************************************************
����ԭ�ͣ�com2_gets2
��������������2����һ���ֽڣ�psam��ͨѶר��
����������
�������ƣ�		����/�����	����		����
-----------		-----------	------	   	-------
str_got			���		INT8U*		�õ����ַ��� 
rcv_len			���		INT16U*		���ճ��� 
timeout_start	����		INT32U		���ֳ�ʱ����λmS��0����
timeout			����		INT16U		�����ֽڼ����ʱ����λmS

 ��  ��  ֵ��ok(0)-�ɹ�
				 KEY_CANCEL-���˳�����
				 notok(0xFF)-ʧ�ܣ���ʱ
��      ��	��������
��      �ڣ�2005-08-20
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
INT8U com2_gets2(INT8U *str_got, INT16U *rcv_len, INT32U timeout_start, INT16U timeout)
{
	INT8U i = 0;
//	INT16U len = 0;
//	INT32U timeout_cntr = 0;
//	INT8U buf[100];
	INT8U *pbuf = str_got;
	INT16U rlen = 0;

//	ToggleWD();
	timeout = timeout;			//avoid warning

	i = com2_getch(pbuf, timeout_start);
	if (i != ok)
	{
		str_got[0] = '\0';
		*rcv_len = 0;
		return notok;
	}
	rlen++;

	for(;;)
	{
		i = com2_getch(pbuf + rlen, 1);
		if (i != ok)
		{
//			memcpy(str_got, buf, 1);
			*rcv_len = rlen;
			return ok;
		}
		rlen++;
	}
}

