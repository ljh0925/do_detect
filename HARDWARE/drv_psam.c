/******************************************************************************
Copyright 2005 大连易达通电子技术有限公司
All rights riserved.

文件名	：drv_psam.c
模块名称：PSAM卡和接触式IC卡驱动程序
功能概要：PSAM卡和接触式IC卡驱动程序

取代版本：0.0.1
修改人	：刘及华
完成日期：2005.10.13
升级说明：create

******************************************************************************/
#include "all.h"


//向PSAM卡发命令用到的一些code变量
// const  INT8U      ReadPOSNumber[5]=         {0x00,0xB0,0x96,0x00,0x06};                         //取POS号
// const  INT8U      SELECT_SAM_ADF1[7]=       {0x00,0xA4,0x00,0x00,0x02,0x10,0x01};       //选1001目录
// const  INT8U      SELECT_SAM_ADF2[7]=       {0x00,0xA4,0x00,0x00,0x02,0x10,0x02};       //选1002目录
// const  INT8U      SELECT_SAM_ADF3[7]=       {0x00,0xA4,0x00,0x00,0x02,0x10,0x03};       //选1003目录
// const  INT8U      DES_INIT[5]=          {0x80,0x1A,0x45,0x01,0x10};                         //DES计算初始化
// const  INT8U      GetResponse[4]=           {0x00,0xC0,0x00,0x00};                                  //取响应
// const  INT8U      DES_CRYPT[4]=             {0x80,0xFA,0x05,0x00};                               //DES计算
// const  INT8U      READ_SAM_KEY_NUM[5]=  {0x00,0xb0,0x97,0x00,0x19};                      //读密钥版本号
// const  INT8U      GET_MAC1[5]=          {0x80,0x70,0x00,0x00,0x24};                       //PSAM卡算MAC1
// const  INT8U      VERIFY_MAC2[5]=           {0x80,0x72,0x00,0x00,0x04};                         //验证MAC2
// const  INT8U      GET_M1_KEY[5]=            {0x80,0xFC,0x00,0x01,0x11};                         //计算M1卡个区密钥
// const  INT8U      TAC_INIT[5]=          {0x80,0x1A,0x44,0x01,0x10};                         //M1卡交易流程中，TAC码计算初始化
// const  INT8U      CAL_TAC_M1[5]=            {0x80,0xFA,0x01,0x00,0x18};                         //M1卡交易过程中，计算TAC码

// //吉林用
// INT8U const      ReadPOSNumber[5]=       	{0x00,0xB0,0x96,0x00,0x06};							//取POS号
// INT8U const      SELECT_SAM_ADF1[7]=      {0x00,0xA4,0x00,0x00,0x02,0x10,0x01};		//选1001目录
// INT8U const      SELECT_SAM_ADF2[7]=      {0x00,0xA4,0x00,0x00,0x02,0x10,0x02};		//选1002目录
// INT8U const      SELECT_SAM_ADF3[7]=      {0x00,0xA4,0x00,0x00,0x02,0x10,0x03};		//选1003目录
// INT8U const      DES_INIT[5]=             		   {0x80,0x1A,0x45,0x01,0x10};							//DES计算初始化
// INT8U const      GetResponse[4]=        		{0x00,0xC0,0x00,0x00};									//取响应
// INT8U const      DES_CRYPT[4]=   				 {0x80,0xFA,0x05,0x00};									 //DES计算
// INT8U const      READ_SAM_KEY_NUM[5]=   {0x00,0xb0,0x97,0x00,0x19};						 //读密钥版本号
// INT8U const      GET_MAC1[5]=            		  {0x80,0x70,0x00,0x00,0x24};						  //PSAM卡算MAC1
// INT8U const      VERIFY_MAC2[5]=          		{0x80,0x72,0x00,0x00,0x04};							//验证MAC2
// // INT8U const    GET_M1_KEY[5]=                {0x80,0xFC,0x00,0x01,0x0E};                         //计算M1卡个区密钥
// INT8U const   	  GET_M1_KEY[5]=          		{0x80,0xFC,0x01,0x01,0x0D};							//计算M1卡个区密钥
// INT8U const      TAC_INIT[5]=             			{0x80,0x1A,0x44,0x01,0x10};							//M1卡交易流程中，TAC码计算初始化
// INT8U const      CAL_TAC_M1[5]=          		{0x80,0xFA,0x01,0x00,0x10};							//M1卡交易过程中，计算TAC码
//
////惠州用
////向PSAM卡发命令用到的一些code变量
//INT8U const       ReadPOSNumber[5]=         {0x00,0xB0,0x96,0x00,0x06};
//INT8U const       GetChallenge[5]=          {0x00,0x84,0x00,0x00,0x04};		//取POS号
//INT8U const       SELECT_SAM_ADF1[7]=      {0x00,0xA4,0x00,0x00,0x02,0x10,0x01};		//选1001目录
//INT8U const       SELECT_SAM_ADF2[7]=      {0x00,0xA4,0x00,0x00,0x02,0x10,0x02};		//选1002目录
//INT8U const       SELECT_SAM_ADF3[7]=      {0x00,0xA4,0x00,0x00,0x02,0x10,0x03};		//选1003目录
//INT8U const       DES_INIT[5]=                     {0x80,0x1A,0x45,0x01,0x10};							//DES计算初始化
//INT8U const      GetResponse[4]=                {0x00,0xC0,0x00,0x00};									//取响应
//INT8U const      DES_CRYPT[4]=                   {0x80,0xFA,0x05,0x00};									 //DES计算
//INT8U const      READ_SAM_KEY_NUM[5]=   {0x00,0xb0,0x97,0x00,0x19};						 //读密钥版本号
//INT8U const      GET_MAC1[5]=                     {0x80,0x70,0x00,0x00,0x24};						  //PSAM卡算MAC1
//INT8U const      VERIFY_MAC2[5]=                {0x80,0x72,0x00,0x00,0x04};							//验证MAC2
//INT8U const      GET_M1_KEY[5]=                 {0x80,0xFC,0x01,0x01,0x11};							//计算M1卡个区密钥
//INT8U const      TAC_INIT[5]=                       {0x80,0x1A,0x44,0x01,0x10};							//M1卡交易流程中，TAC码计算初始化
//INT8U const      CAL_TAC_M1[5]=                 {0x80,0xFA,0x01,0x00,0x18};							//M1卡交易过程中，计算TAC码
//INT8U const      DESCOMMUNICATION1[5]=   {0x80,0xfa,0x05,0x00,0x20};						//CPU卡写0017文件
//INT8U const      DESCOMMUNICATION[4]=   {0x80,0xfa,0x05,0x00};									//DES计算



//向PSAM卡发命令用到的一些code变量
INT8U const   ReadPOSNumber[5] =		{ 0x00, 0xB0, 0x96, 0x00, 0x06 };                     //取POS号
INT8U const   SELECT_SAM_ADF1[7] =		{ 0x00, 0xA4, 0x00, 0x00, 0x02, 0x10, 0x01 };           //选1001目录
INT8U const   SELECT_SAM_ADF2[7] =		{ 0x00, 0xA4, 0x00, 0x00, 0x02, 0x10, 0x02 };           //选1002目录
INT8U const   SELECT_SAM_ADF3[7] =		{ 0x00, 0xA4, 0x00, 0x00, 0x02, 0x10, 0x03 };           //选1003目录
INT8U const   DES_INIT[5] =				{ 0x80, 0x1A, 0x45, 0x01, 0x10 };                     //DES计算初始化
INT8U const   GetResponse[4] =			{ 0x00, 0xC0, 0x00, 0x00 };                          //取响应
INT8U const   DES_CRYPT[4] =				{ 0x80, 0xFA, 0x05, 0x00 };                          //DES计算
INT8U const   READ_SAM_KEY_NUM[5] =		{ 0x00, 0xb0, 0x97, 0x00, 0x19 };                     //读密钥版本号
INT8U const   GET_MAC1[5] =				{ 0x80, 0x70, 0x00, 0x00, 0x24 };                     //PSAM卡算MAC1
INT8U const   VERIFY_MAC2[5] =			{ 0x80, 0x72, 0x00, 0x00, 0x04 };                     //验证MAC2
INT8U const   GET_M1_KEY[5] =			{ 0x80, 0xFC, 0x01, 0x01, 0x11 };                     //计算M1卡个区密钥 zibo
																					  //INT8U const   GET_M1_KEY[5]=			{0x80,0xFC,0x00,0x01,0x11};						//计算M1卡个区密钥 laiwu
INT8U const   TAC_INIT[5] =				{ 0x80, 0x1A, 0x44, 0x01, 0x10 };                     //M1卡交易流程中，TAC码计算初始化
INT8U const   CAL_TAC_M1[5] =			{ 0x80, 0xFA, 0x01, 0x00, 0x18 };                     //M1卡交易过程中，计算TAC码
																					  //城市通卡
INT8U const   SELECT_SAM_3F01[7] =		{ 0x00, 0xA4, 0x00, 0x00, 0x02, 0x3F, 0x01 };           //选1001目录
INT8U const   GET_M1_FUZA_KEY[5] =		{ 0x80, 0xFC, 0x0E, 0x0D, 0x19 };                     //计算M1卡个区密钥
INT8U const   TAC_FUZA_INIT[5] =			{ 0x80, 0x1A, 0x44, 0x01, 0x10 };                     //M1卡交易流程中，TAC码计算初始化
INT8U const   CAL_TAC_M1_FUZA[5] =		{ 0x80, 0xFA, 0x01, 0x00, 0x18 };                     //M1卡交易过程中，计算TAC码
																						  //INT8U const   GET_MI_FUZA_KEY_YINZI[8]= {0xD1,0xCC,0xCC,0xA8,0x00,0x00,0x00,0x00};	//M1卡交易过程中，计算TAC码
INT8U const   GET_MI_FUZA_KEY_YINZI[8] ={ 0xD7, 0xCD, 0xB2, 0xA9, 0x00, 0x00, 0x00, 0x00 };      //M1卡交易过程中，计算TAC码
INT8U const   FUZHA_TAC_INIT[5] =		{ 0x80, 0x1A, 0x48, 0x0A, 0x10 };                     //M1卡交易流程中，TAC码计算初始化
INT8U const   FUZHA_CAL_TAC_M1[5] =		{ 0x80, 0xFA, 0x05, 0x00, 30 };                       //M1卡交易过程中，计算TAC码

#define CHANNL_PSAM_SLV_T		1
#define CHANNL_PSAM_SLV_R		2
#define CHANNL_PSAM_PRI_T		3
#define CHANNL_PSAM_PRI_R		4
#define CHANNL_PSAM_3_T			5
#define CHANNL_PSAM_3_R			6
#define CHANNL_PSAM_4_T			7
#define CHANNL_PSAM_4_R			8


#define  RST_PSAM_PRI_SetLow()  		GPIO_ResetBits(GPIOA, GPIO_Pin_1)
#define  RST_PSAM_PRI_SetHigh()			GPIO_SetBits(GPIOA, GPIO_Pin_1)		//PA.1 PSAM_RST1 输出高
#define  PSAM_PRI_RST()					RST_PSAM_PRI_SetLow()
#define  PSAM_PRI_CLEAR_RST()			RST_PSAM_PRI_SetHigh()

//#define  RST_PSAM_SLV_SetLow()  		GPIO_ResetBits(GPIOC, GPIO_Pin_9)
//#define  RST_PSAM_SLV_SetHigh()		 	GPIO_SetBits(GPIOC, GPIO_Pin_9)		//PC.9 PSAM_RST2 输出高
//#define  PSAM_SLV_RST()					RST_PSAM_SLV_SetLow()
//#define  PSAM_SLV_CLEAR_RST()			RST_PSAM_SLV_SetHigh()
//
//#define  RST_PSAM3_SetLow()  			GPIO_ResetBits(GPIOC, GPIO_Pin_8)
//#define  RST_PSAM3_SetHigh()		 	GPIO_SetBits(GPIOC, GPIO_Pin_8)		//PC.8 PSAM_RST3 输出高
//#define  PSAM3_RST()					RST_PSAM3_SetLow()
//#define  PSAM3_CLEAR_RST()				RST_PSAM3_SetHigh()
//
//#define  RST_PSAM4_SetLow()  			GPIO_ResetBits(GPIOC, GPIO_Pin_7)
//#define  RST_PSAM4_SetHigh()		 	GPIO_SetBits(GPIOC, GPIO_Pin_7)		//PC.7 PSAM_RST4 输出高
//#define  PSAM4_RST()					RST_PSAM4_SetLow()
//#define  PSAM4_CLEAR_RST()				RST_PSAM4_SetHigh()
//
//
//
//#define  EN4245_SetLow()			GPIO_ResetBits(GPIOC, GPIO_Pin_0)
//#define  EN4245_SetHigh()			GPIO_SetBits(GPIOC, GPIO_Pin_0)		//PC.0 4245EN 输出高
//
//#define  DIR4245_SetLow()			GPIO_ResetBits(GPIOC, GPIO_Pin_1)
//#define  DIR4245_SetHigh()			GPIO_SetBits(GPIOC, GPIO_Pin_1)		//PC.1 4245DIR 输出高
//
//#define  A4051_SetLow()				GPIO_ResetBits(GPIOC, GPIO_Pin_4)
//#define  A4051_SetHigh()			GPIO_SetBits(GPIOC, GPIO_Pin_4)		//PC.4 4051A 输出高
//
//#define  B4051_SetLow()				GPIO_ResetBits(GPIOC, GPIO_Pin_5)
//#define  B4051_SetHigh()			GPIO_SetBits(GPIOC, GPIO_Pin_5)		//PC.5 4051B 输出高
//
//#define  C4051_SetLow()				GPIO_ResetBits(GPIOB, GPIO_Pin_0)
//#define  C4051_SetHigh()			GPIO_SetBits(GPIOB, GPIO_Pin_0)		//PB.0 4051C 输出高
//
//#define  ENABLE_PSAM_BUS()			EN4245_SetLow()
//#define  DISABLE_PSAM_BUS()			EN4245_SetHigh()
//
//#define  PSAMbusSelectOut()  		DIR4245_SetLow()			//总线方向选择输出
//#define  PSAMbusSelectIn()		 	DIR4245_SetHigh()			//输入


//extern int UART3RcvBuf[(UART3_RCV_QUEUE_LENGTH + sizeof(int)- 1)/ sizeof(int)];
//extern INT8U Com3SndBuf[UART3_SEND_QUEUE_LENGTH];              //串口发送缓冲区
//extern INT8U Com3RcvBuf[UART3_RCV_QUEUE_LENGTH];               //串口接收缓冲区

extern int UART2RcvBuf[(UART2_RCV_QUEUE_LENGTH + sizeof(int)- 1)/ sizeof(int)];
extern INT8U Com2SndBuf[UART2_SEND_QUEUE_LENGTH];              //串口发送缓冲区
extern INT8U Com2RcvBuf[UART2_RCV_QUEUE_LENGTH];               //串口接收缓冲区

/******************************************************************************
函数名称：psam_init
功能描述：PSAM卡初始化
参数描述：

返  回  值	：无
作      者	：刘及华
日      期：2005-04-08
修改历史：
日期		修改人		修改描述
------		---------	---------
******************************************************************************/
void psam_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	ToggleWD();

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能PA端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;                //PSAM_RST1-->PA.1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;         //IO口速度为2MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);                   //根据设定参数初始化GPIO
	GPIO_SetBits(GPIOA, GPIO_Pin_1);                         //PA.1输出高

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                //PSAM_RST2-->PC.9
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         //推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;         //IO口速度为2MHz
//	GPIO_Init(GPIOC, &GPIO_InitStructure);                   //根据设定参数初始化GPIO
//	GPIO_SetBits(GPIOC, GPIO_Pin_9);                         //PC.9输出高
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;                //PSAM_RST3-->PC.8
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         //推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;         //IO口速度为2MHz
//	GPIO_Init(GPIOC, &GPIO_InitStructure);                   //根据设定参数初始化GPIO
//	GPIO_SetBits(GPIOC, GPIO_Pin_8);                         //PC.9输出高
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;                //PSAM_RST4-->PC.7
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         //推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;         //IO口速度为2MHz
//	GPIO_Init(GPIOC, &GPIO_InitStructure);                   //根据设定参数初始化GPIO
//	GPIO_SetBits(GPIOC, GPIO_Pin_7);                         //PC.9输出高
//
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                //4245EN-->PC.0
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         //推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;         //IO口速度为2MHz
//	GPIO_Init(GPIOC, &GPIO_InitStructure);                   //根据设定参数初始化GPIO
//	GPIO_SetBits(GPIOC, GPIO_Pin_0);                         //PC.0输出高
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;                //4245DIR-->PC.1
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         //推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;         //IO口速度为2MHz
//	GPIO_Init(GPIOC, &GPIO_InitStructure);                   //根据设定参数初始化GPIO
//	GPIO_SetBits(GPIOC, GPIO_Pin_1);                         //PC.1输出高
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                //4051A-->PC.4
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         //推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;         //IO口速度为2MHz
//	GPIO_Init(GPIOC, &GPIO_InitStructure);                   //根据设定参数初始化GPIO
//	GPIO_SetBits(GPIOC, GPIO_Pin_4);                         //PC.4输出高
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;                //4051B-->PC.5
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         //推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;         //IO口速度为2MHz
//	GPIO_Init(GPIOC, &GPIO_InitStructure);                   //根据设定参数初始化GPIO
//	GPIO_SetBits(GPIOC, GPIO_Pin_5);                         //PC.5输出高
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                //4051C-->PB.0
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         //推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;         //IO口速度为2MHz
//	GPIO_Init(GPIOB, &GPIO_InitStructure);                   //根据设定参数初始化GPIO
//	GPIO_SetBits(GPIOB, GPIO_Pin_0);                         //PB.0输出高


//	DISABLE_PSAM_BUS();
//	PSAMbusSelectIn();
	uart2_set_bps(38400, 9, 1, EVEN);
	ChangePsamChannel(CHANNL_PSAM_PRI_R);
}

/******************************************************************************
 函数名称：PsamRst
 功能描述：复位PSAM卡
 参数描述：
 参数名称：		输入/输出？	类型		描述
 psam_channel	输入		INT8U		要复位PSAM的通道 =PSAM_PRI或=PSAM_SLV
 rcv_buf		输出		INT8U *		复位返回的数据
 rcv_length		输出		INT16U * 	复位返回数据的长度
				
 返  回  值：ok(0)-成功
				 notok(0xFF)-失败
				   
 作      者	：刘及华
 日      期：2004-09-02
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
INT8U PsamRst(const INT8U psam_channel, INT8U *rcv_buf, INT16U *rcv_length)
{
	INT8U i = 0;

	ToggleWD();

	//参数检查
//	if ( !( (psam_channel == PSAM_PRI) || (psam_channel == PSAM_SLV) || (psam_channel == PSAM3) || (psam_channel == PSAM4) ) )
	if ( (psam_channel != PSAM_PRI) )
		return ( notok );

//	ENABLE_PSAM_BUS();

//	if ( psam_channel == PSAM_PRI )
//	{
		ChangePsamChannel(CHANNL_PSAM_PRI_R);                   //建立连接，波特率38400，通道为主PSAM卡接收通道
//		PSAMbusSelectIn();

		PSAM_PRI_RST();
		SleepMs(3);
		PSAM_PRI_CLEAR_RST();
//	}
//	else if ( psam_channel == PSAM_SLV )
//	{
//		ChangePsamChannel(CHANNL_PSAM_SLV_R);                   //建立连接，波特率38400，通道为从PSAM卡接收通道
//		PSAMbusSelectIn();
//
//		PSAM_SLV_RST();
//		SleepMs(3);
//		PSAM_SLV_CLEAR_RST();
//	}
//	else if ( psam_channel == PSAM3 )
//	{
//		ChangePsamChannel(CHANNL_PSAM_3_R);                   //建立连接，波特率38400，通道为PSAM3接收通道
//		PSAMbusSelectIn();
//
//		PSAM3_RST();
//		SleepMs(3);
//		PSAM3_CLEAR_RST();
//	}
//	else
//	{
//		ChangePsamChannel(CHANNL_PSAM_4_R);                   //建立连接，波特率38400，通道为PSAM4接收通道
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
	if ( (*rcv_buf == 0x3B) && ((*(rcv_buf + 1) & 0xF0) == 0x60) )                 //开头字节必须为 0x3B6?
	{
		i = rcv_buf[1] & 0x0F;
		if ( *rcv_length == (i + 4) )                                         //收到的长度正确
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
// 函数名称：Cmd7816
// 功能描述：使用T=0协议与PSAM卡或接触式IC卡进行通讯。比烟台读写器减少了0x6C(发送长度不对)的处理
// 参数描述：
// 参数名称： 	输入/输出？ 类型        描述
// psam_channel	输入    	INT8U const 通道号=PSAM_PRI或=PSAM_SLV
// snd_buf		输入    	INT8U *     发送缓冲区
// snd_length		输入    	INT16U      要发送命令的长度
// rcv_buf		输出        INT8U *     PSAM卡送回的字节缓冲区
// rcv_length		输出        INT16U *    PSAM卡送回的字节长度
//
// 返  回  值：SAM卡返回的状态码，如果出错或PSAM卡无返回则返回0xC002。相当与把rcv_buf的最后两个字节复制一份做为返回值
//
// 作      者 ：刘及华
// 日      期：2004-09-02
// 修改历史：
//		日期        修改人      修改描述
//		------      ---------   -------------
//******************************************************************************/
//INT16U Cmd7816(const INT8U psam_channel, const INT8U * const snd_buf, const INT16U snd_length,
//			   INT8U *rcv_buf, INT16U *rcv_length)
//{
//	INT8U i = 0;
//	INT8U temp_buf[50];                 //临时的发送和接收缓冲区
//	INT16U psam_status = 0;                     //PSAM卡返回状态的暂存
//	INT8U cur_psam_channel;
//
//	ToggleWD();
//
//	//参数检查
////	if ( (psam_channel != PSAM_PRI) && (psam_channel != PSAM_SLV) )
//	if ( !( (psam_channel == PSAM_PRI) || (psam_channel == PSAM_SLV) || (psam_channel == PSAM3) || (psam_channel == PSAM4) ) )
//		return ( 0xC002 );
//
//	if ( snd_length < 4 )                                     //发送的字节长度必须>=4
//		return ( 0xC002 );
//
//	ENABLE_PSAM_BUS();
//
//	if ( psam_channel == PSAM_PRI )                                         //为了减小代码量，以下ComCreateConnctn函数调用此变量
//		cur_psam_channel = CHANNL_PSAM_PRI_T;
//	else if ( psam_channel == PSAM_SLV )
//		cur_psam_channel = CHANNL_PSAM_SLV_T;
//	else if ( psam_channel == PSAM3 )
//		cur_psam_channel = CHANNL_PSAM_3_T;
//	else
//		cur_psam_channel = CHANNL_PSAM_4_T;
//
//
//	if ( snd_length == 4 )                           //向PSAM卡发送命令长度为4的处理
//	{
//		ChangePsamChannel(cur_psam_channel);                    //建立连接，波特率38400，通道为主PSAM卡接收通道
//		memcpy(temp_buf, snd_buf, 4);
//		temp_buf[5] = 0x00;                             //T=0协议情况1，不足5字节的命令补0
//
//		com3_send_psam((void *)snd_buf, 5);
//
//		ChangePsamChannel(cur_psam_channel + 1);                    //建立连接，波特率38400，通道为主PSAM卡接收通道
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
//	else if ( snd_length == 5 )                          //向PSAM卡发送命令长度为5的处理
//	{
//		ChangePsamChannel(cur_psam_channel);                    //建立连接，波特率38400，通道为主PSAM卡接收通道
//		com3_send_psam((void *)snd_buf, 5);
//		ChangePsamChannel(cur_psam_channel + 1);                    //建立连接，波特率38400，通道为主PSAM卡接收通道
//
//		i = com3_gets(rcv_buf, rcv_length, PSAM_RD_DELAY, 5);
//		if ( i != ok )
//		{
//			DISABLE_PSAM_BUS();
//			return ( 0xC002 );
//		}
//		if ( (*rcv_length > 2) && (rcv_buf[0] != snd_buf[1]) )      //PSAM卡返回错误，返回的第一个字节应该等于发送的第二个字节
//		{
//			DISABLE_PSAM_BUS();
//			return ( 0xC002 );
//		}
//		psam_status = rcv_buf[*rcv_length - 2];
//		psam_status = (psam_status << 8) + rcv_buf[*rcv_length - 1];
//		return ( psam_status );
//	}
//	else                                                     //向PSAM卡发送命令长度>5的处理
//	{
//		ChangePsamChannel(cur_psam_channel);                    //建立连接，波特率38400，通道为主PSAM卡接收通道
//		com3_send_psam((void *)snd_buf, 5);
//		ChangePsamChannel(cur_psam_channel + 1);                    //建立连接，波特率38400，通道为主PSAM卡接收通道
//
//		i = com3_gets(rcv_buf, rcv_length, PSAM_RD_DELAY, 5);
//		if ( i != ok )
//		{
//			DISABLE_PSAM_BUS();
//			return ( 0xC002 );
//		}
//		if ( *rcv_length == 1 )                                                                       //发后续包
//		{
//			ChangePsamChannel(cur_psam_channel);                    //建立连接，波特率38400，通道为主PSAM卡接收通道
//			com3_send_psam((void *)(snd_buf + 5), snd_length - 5);                        //发后续包
//			ChangePsamChannel(cur_psam_channel + 1);                    //建立连接，波特率38400，通道为主PSAM卡接收通道
//
//			i = com3_gets(rcv_buf, rcv_length, PSAM_RD_DELAY, 5);
//			if ( i != ok )
//			{
//				DISABLE_PSAM_BUS();
//				return ( 0xC002 );
//			}
//			if ( *rcv_length == 2 )                                                                       //发后续包
//			{
//				if ( rcv_buf[0] == 0x61 )                                                            //PSAM卡要求取响应
//				{
//					memcpy(temp_buf, GetResponse, 4);
//					temp_buf[4] = rcv_buf[1];
//
//					ChangePsamChannel(cur_psam_channel);                    //建立连接，波特率38400，通道为主PSAM卡接收通道
//					com3_send_psam((void *)temp_buf, 5);
//					ChangePsamChannel(cur_psam_channel + 1);                    //建立连接，波特率38400，通道为主PSAM卡接收通道
//
//					i = com3_gets(rcv_buf, rcv_length, PSAM_RD_DELAY, 5);
//					if ( i != ok )
//					{
//						DISABLE_PSAM_BUS();
//						return ( 0xC002 );
//					}
//					if ( (*rcv_length > 2) && rcv_buf[0] == temp_buf[1] )           //判断返回是否正确
//					{
//						psam_status = rcv_buf[*rcv_length - 2];
//						psam_status = (psam_status << 8) + rcv_buf[*rcv_length - 1];
//						return ( psam_status );
//					}
//					else
//					{
//						DISABLE_PSAM_BUS();
//						return ( 0xC002 );                                                              //返回格式不对
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
//			else                                                                                                //长度错误
//			{
//				DISABLE_PSAM_BUS();
//				return ( 0xC002 );
//			}
//		}
//		else if ( *rcv_length == 2 )                                                              //返回状态码
//		{
//			psam_status = rcv_buf[*rcv_length - 2];
//			psam_status = (psam_status << 8) + rcv_buf[*rcv_length - 1];
//			DISABLE_PSAM_BUS();
//			return ( psam_status );
//		}
//		else                                                                                                //PSAM卡返回长度错误
//		{
//			DISABLE_PSAM_BUS();
//			return ( 0xC002 );
//		}
//	}
//}

/******************************************************************************
 函数名称：Cmd7816
 功能描述：使用T=0协议与PSAM卡或接触式IC卡进行通讯。比烟台读写器减少了0x6C(发送长度不对)的处理
 参数描述：
 参数名称： 	输入/输出？ 类型        描述
 psam_channel	输入    	INT8U const 通道号=PSAM_PRI或=PSAM_SLV
 snd_buf		输入    	INT8U *     发送缓冲区
 snd_length		输入    	INT16U      要发送命令的长度
 rcv_buf		输出        INT8U *     PSAM卡送回的字节缓冲区
 rcv_length		输出        INT16U *    PSAM卡送回的字节长度

 返  回  值：SAM卡返回的状态码，如果出错或PSAM卡无返回则返回0xC002。相当与把rcv_buf的最后两个字节复制一份做为返回值

 作      者 ：刘及华
 日      期：2004-09-02
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
INT16U Cmd7816(const INT8U psam_channel, const INT8U * const snd_buf, const INT16U snd_length,
			   INT8U *rcv_buf, INT16U *rcv_length)
{
	INT8U i = 0;
	INT8U temp_buf[50];                 //临时的发送和接收缓冲区
	INT16U psam_status = 0;                     //PSAM卡返回状态的暂存
	INT8U cur_psam_channel;

	ToggleWD();

	//参数检查
//	if ( !( (psam_channel == PSAM_PRI) || (psam_channel == PSAM_SLV) || (psam_channel == PSAM3) || (psam_channel == PSAM4) ) )
	if ( (psam_channel != PSAM_PRI) )
		return ( 0xC002 );

	if ( snd_length < 4 )                                     //发送的字节长度必须>=4
		return ( 0xC002 );

//	ENABLE_PSAM_BUS();

	if ( psam_channel == PSAM_PRI )                                         //为了减小代码量，以下ComCreateConnctn函数调用此变量
		cur_psam_channel = CHANNL_PSAM_PRI_T;
	else if ( psam_channel == PSAM_SLV )
		cur_psam_channel = CHANNL_PSAM_SLV_T;
	else if ( psam_channel == PSAM3 )
		cur_psam_channel = CHANNL_PSAM_3_T;
	else
		cur_psam_channel = CHANNL_PSAM_4_T;
	

	if ( snd_length == 4 )                           //向PSAM卡发送命令长度为4的处理
	{
		ChangePsamChannel(cur_psam_channel);                    //建立连接，波特率38400，通道为主PSAM卡接收通道
		memcpy(temp_buf, snd_buf, 4);
		temp_buf[5] = 0x00;                             //T=0协议情况1，不足5字节的命令补0

		com2_send_psam((void *)snd_buf, 5);

		ChangePsamChannel(cur_psam_channel + 1);                    //建立连接，波特率38400，通道为主PSAM卡接收通道

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
	else if ( snd_length == 5 )                          //向PSAM卡发送命令长度为5的处理
	{
		ChangePsamChannel(cur_psam_channel);                    //建立连接，波特率38400，通道为主PSAM卡接收通道
		com2_send_psam((void *)snd_buf, 5);
		ChangePsamChannel(cur_psam_channel + 1);                    //建立连接，波特率38400，通道为主PSAM卡接收通道

		i = com2_gets(rcv_buf, rcv_length, PSAM_RD_DELAY, 5);
		if ( i != ok )
		{
//			DISABLE_PSAM_BUS();
			return ( 0xC002 );
		}
		if ( (*rcv_length > 2) && (rcv_buf[0] != snd_buf[1]) )      //PSAM卡返回错误，返回的第一个字节应该等于发送的第二个字节
		{
//			DISABLE_PSAM_BUS();
			return ( 0xC002 );
		}
		psam_status = rcv_buf[*rcv_length - 2];
		psam_status = (psam_status << 8) + rcv_buf[*rcv_length - 1];
		return ( psam_status );
	}
	else                                                     //向PSAM卡发送命令长度>5的处理
	{
		ChangePsamChannel(cur_psam_channel);                    //建立连接，波特率38400，通道为主PSAM卡接收通道
		com2_send_psam((void *)snd_buf, 5);
		ChangePsamChannel(cur_psam_channel + 1);                    //建立连接，波特率38400，通道为主PSAM卡接收通道

//		i = com3_gets(rcv_buf, rcv_length, PSAM_RD_DELAY, 5);
		i = com2_get_2ch(rcv_buf, rcv_length, PSAM_RD_DELAY, 5);
		if ( i != ok )
		{
//			DISABLE_PSAM_BUS();
			return ( 0xC002 );
		}
		if ( *rcv_length == 1 )                                                                       //发后续包
		{
			ChangePsamChannel(cur_psam_channel);                    //建立连接，波特率38400，通道为主PSAM卡接收通道
			com2_send_psam((void *)(snd_buf + 5), snd_length - 5);                        //发后续包
			ChangePsamChannel(cur_psam_channel + 1);                    //建立连接，波特率38400，通道为主PSAM卡接收通道

//			i = com3_gets(rcv_buf, rcv_length, PSAM_RD_DELAY, 5);
			i = com2_get_2ch(rcv_buf, rcv_length, PSAM_RD_DELAY, 5);
			if ( i != ok )
			{
//				DISABLE_PSAM_BUS();
				return ( 0xC002 );
			}
			if ( *rcv_length == 2 )                                                                       //发后续包
			{
				if ( rcv_buf[0] == 0x61 )                                                            //PSAM卡要求取响应
				{
					memcpy(temp_buf, GetResponse, 4);
					temp_buf[4] = rcv_buf[1];

					ChangePsamChannel(cur_psam_channel);                    //建立连接，波特率38400，通道为主PSAM卡接收通道
					com2_send_psam((void *)temp_buf, 5);
					ChangePsamChannel(cur_psam_channel + 1);                    //建立连接，波特率38400，通道为主PSAM卡接收通道

					i = com2_gets(rcv_buf, rcv_length, PSAM_RD_DELAY, 5);
					if ( i != ok )
					{
//						DISABLE_PSAM_BUS();
						return ( 0xC002 );
					}
					if ( (*rcv_length > 2) && rcv_buf[0] == temp_buf[1] )           //判断返回是否正确
					{
						psam_status = rcv_buf[*rcv_length - 2];
						psam_status = (psam_status << 8) + rcv_buf[*rcv_length - 1];
						return ( psam_status );
					}
					else
					{
//						DISABLE_PSAM_BUS();
						return ( 0xC002 );                                                              //返回格式不对
					}

				}
				else
				{
					psam_status = rcv_buf[*rcv_length - 2];
					psam_status = (psam_status << 8) + rcv_buf[*rcv_length - 1];
					return ( psam_status );
				}
			}
			else                                                                                                //长度错误
			{
//				DISABLE_PSAM_BUS();
				return ( 0xC002 );
			}
		}
		else if ( *rcv_length == 2 )                                                              //返回状态码
		{
			psam_status = rcv_buf[*rcv_length - 2];
			psam_status = (psam_status << 8) + rcv_buf[*rcv_length - 1];
//			DISABLE_PSAM_BUS();
			return ( psam_status );
		}
		else                                                                                                //PSAM卡返回长度错误
		{
//			DISABLE_PSAM_BUS();
			return ( 0xC002 );
		}
	}
}

/******************************************************************************
 函数名称：ChangePsamChannel
 功能描述：改变psam卡通道函数
 参数描述：
 参数名称：	输入/输出？	类型		描述
 com_channel	输入	INT8U	要改为的通道
				
 返  回  值：无
				   
 作      者	：刘及华
 日      期：2004-09-02
 修改历史：
		日期		修改人		修改描述
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
//		case CHANNL_PSAM_PRI_T:                 //主PSAM卡发送通道(PSAM1)
//			A4051_SetLow();
//			B4051_SetLow();
//			C4051_SetLow();
//			break;
//
//		case CHANNL_PSAM_PRI_R:                 //主PSAM卡接收通道(PSAM1)
//			A4051_SetHigh();
//			B4051_SetLow();
//			C4051_SetLow();
//			break;
//
//		case CHANNL_PSAM_SLV_T:                 //从PSAM卡发送通道(PSAM2)
//			A4051_SetLow();
//			B4051_SetHigh();
//			C4051_SetLow();
//			break;
//
//		case CHANNL_PSAM_SLV_R:                 //从PSAM卡接收通道(PSAM2)
//			A4051_SetHigh();
//			B4051_SetHigh();
//			C4051_SetLow();
//			break;
//
//		case CHANNL_PSAM_3_T:                  //PSAM3发送通道(PSAM3)
//			A4051_SetLow();
//			B4051_SetLow();
//			C4051_SetHigh();
//			break;
//
//		case CHANNL_PSAM_3_R:                  //PSAM3接收通道(PSAM3)
//			A4051_SetHigh();
//			B4051_SetLow();
//			C4051_SetHigh();
//			break;
//
//		case CHANNL_PSAM_4_T:                  //PSAM4发送通道(PSAM4)
//			A4051_SetLow();
//			B4051_SetHigh();
//			C4051_SetHigh();
//			break;
//
//		case CHANNL_PSAM_4_R:                  //PSAM4接收通道(PSAM4)
//			A4051_SetHigh();
//			B4051_SetHigh();
//			C4051_SetHigh();
//			break;
//
//
//		default:                                //默认为主PSAM卡接收通道(PSAM1)
//			A4051_SetHigh();
//			B4051_SetLow();
//			C4051_SetLow();
//			break;
//	}

}

/******************************************************************************
 函数名称：com2_send_psam
 功能描述：串口2发送一串数据给psam卡
 参数描述：
 参数名称： 输入/输出？ 类型		描述
 str_to_send	输入	INT8U *		要发送的字符串
 len			输入	INT16U		长度
				
 返  回  值：无
				   
 作 	 者 ：刘及华
 日 	 期：2005-04-07
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void com2_send_psam(INT8U *str_to_send, INT16U len)
{
	INT16U i = 0;

	ToggleWD();

	USART_ITConfig(USART2, USART_IT_TXE, DISABLE);      //关发送中断
//	PSAMbusSelectOut();
	for ( i = 0; i < len; i++ )
	{
		uart2_putch_psam(str_to_send[i]);
	}
//	PSAMbusSelectIn();
	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);  /* 允许发送中断 */
}

/******************************************************************************
 函数名称：uart2_putch_psam
 功能描述：串口2发送一个字节数据
 参数描述：
 参数名称： 输入/输出？ 类型		描述
 dat		输入		INT8U		要发送的数据
				
 返  回  值：无
				   
 作 	 者 ：刘及华
 日 	 期：2012-04-06
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void uart2_putch_psam(INT8U dat)
{
	USART_SendData(USART2, dat);
	while ( USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET )           //等待数据发送完成
	{
		;
	}

}

/*****************************************************************
函数原型：com2_getch
功能描述：串口2接收1个字节，带超时控制
参数描述：
参数名称：		输入/输出？	类型		描述
-----------		-----------	------	   	-------
str_got			输出		INT8U*		得到的字符串
timeout_start	输入		INT32U		首字超时，单位mS，0死等

 返  回  值：ok(0)-成功
				 KEY_CANCEL-按退出键了
				 notok(0xFF)-失败，超时
作      者	：刘及华
日      期：2005-08-20
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U com2_getch(INT8U *str_got, INT32U timeout_start)
{
	INT8U i = 0;
//	INT16U len = 0;
	INT32U timeout_cntr = 0;

//	ToggleWD();

	//第一个字节
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
				if (timeout_cntr > timeout_start)          //超时
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
函数原型：com2_get_2ch
功能描述：串口2接收1-2个字节，psam卡通讯专用
参数描述：
参数名称：		输入/输出？	类型		描述
-----------		-----------	------	   	-------
str_got			输出		INT8U*		得到的字符串 
rcv_len			输出		INT16U*		接收长度 
timeout_start	输入		INT32U		首字超时，单位mS，0死等
timeout			输入		INT16U		接收字节间隔超时，单位mS

 返  回  值：ok(0)-成功
				 KEY_CANCEL-按退出键了
				 notok(0xFF)-失败，超时
作      者	：刘及华
日      期：2005-08-20
修改历史：
日期		修改人		修改描述
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
		//0x6X和0x9X要接收2个字节
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
 函数名称：psam_rst_fast
 功能描述：快速复位PSAM卡
 参数描述：
 参数名称：		输入/输出？	类型		描述
 psam_channel	输入		INT8U		要复位PSAM的通道 =PSAM_PRI或=PSAM_SLV
 rcv_buf		输出		INT8U *		复位返回的数据
 rcv_length		输出		INT16U * 	复位返回数据的长度
				
 返  回  值：ok(0)-成功
				 notok(0xFF)-失败
				   
 作      者	：刘及华
 日      期：2004-09-02
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
INT8U psam_rst_fast(const INT8U psam_channel, INT8U *rcv_buf, INT16U *rcv_length)
{
	INT8U i = 0;

	ToggleWD();

	//参数检查
//	if ( !( (psam_channel == PSAM_PRI) || (psam_channel == PSAM_SLV) || (psam_channel == PSAM3) || (psam_channel == PSAM4) ) )
	if ( (psam_channel != PSAM_PRI) )
		return ( notok );

//	ENABLE_PSAM_BUS();
//
//	if ( psam_channel == PSAM_PRI )
//	{
		ChangePsamChannel(CHANNL_PSAM_PRI_R);                   //建立连接，波特率38400，通道为主PSAM卡接收通道
//		PSAMbusSelectIn();

		PSAM_PRI_RST();
		SleepMs(1);
		PSAM_PRI_CLEAR_RST();
//	}
//	else if ( psam_channel == PSAM_SLV )
//	{
//		ChangePsamChannel(CHANNL_PSAM_SLV_R);                   //建立连接，波特率38400，通道为从PSAM卡接收通道
//		PSAMbusSelectIn();
//
//		PSAM_SLV_RST();
//		SleepMs(1);
//		PSAM_SLV_CLEAR_RST();
//	}
//	else if ( psam_channel == PSAM3 )
//	{
//		ChangePsamChannel(CHANNL_PSAM_3_R);                   //建立连接，波特率38400，通道为PSAM3接收通道
//		PSAMbusSelectIn();
//
//		PSAM3_RST();
//		SleepMs(1);
//		PSAM3_CLEAR_RST();
//	}
//	else
//	{
//		ChangePsamChannel(CHANNL_PSAM_4_R);                   //建立连接，波特率38400，通道为PSAM4接收通道
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
	if ( (*rcv_buf == 0x3B) && ((*(rcv_buf + 1) & 0xF0) == 0x60) )                 //开头字节必须为 0x3B6?
	{
		i = rcv_buf[1] & 0x0F;
		if ( *rcv_length == (i + 4) )                                         //收到的长度正确
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
函数原型：com2_gets2
功能描述：串口2接收一串字节，psam卡通讯专用
参数描述：
参数名称：		输入/输出？	类型		描述
-----------		-----------	------	   	-------
str_got			输出		INT8U*		得到的字符串 
rcv_len			输出		INT16U*		接收长度 
timeout_start	输入		INT32U		首字超时，单位mS，0死等
timeout			输入		INT16U		接收字节间隔超时，单位mS

 返  回  值：ok(0)-成功
				 KEY_CANCEL-按退出键了
				 notok(0xFF)-失败，超时
作      者	：刘及华
日      期：2005-08-20
修改历史：
日期		修改人		修改描述
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

