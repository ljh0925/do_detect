/******************************************************************************
Copyright 2005 大连易达通电子有限公司
All rights riserved.

文件名	：drv_other.c
模块名称：其他驱动程序
功能概要：其他驱动程序

取代版本：0.0.1
修改人	：刘及华
完成日期：2005.04.06
升级说明：create

******************************************************************************/
#include "all.h"


INT8U BLACK_LENTH;
extern DEV_STAT DevStat;
INT8U BLACK_NAME_CARD[100][3];      //黑名单，3字节一组，一共是100组

/******************************************************************************
 函数名称：Cal_BCC
 功能描述：计算BCC校验码
 参数描述：
 参数名称：	输入/输出？	类型		描述
 buf		输入		const INT8U*		要进行BCC计算的数据缓冲区
 len		输入		INT16U		长度
				
 返  回  值：计算出来的BCC值
				   
 作      者	：刘及华
 日      期：2004-09-02
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
INT8U Cal_BCC(const INT8U *buf, INT16U len)
{
	INT16U i = 0;
	INT8U bcc = 0;

	for ( i=0; i<len; i++ )
	{
		bcc ^= buf[i];
	}

	return bcc;
}

/******************************************************************************
 函数名称：cal_sec
 功能描述：计算从2000年1月1日到输入时间的秒数
 参数描述：
 参数名称：	输入/输出？	类型		描述
 time			输入			BUS_TIME*	要进行计算的时间，要求输入时间大于2000年
 
 返  回  值：秒数
   
 作      者	：刘及华
 日      期：2006.1.9
 修改历史：
		日期		修改人		修改描述

******************************************************************************/
INT32U cal_sec(_TIME * time)
{
	INT8U i;
	INT16U year;
	INT32U sec;

	const INT32U days[] = 
	{
		0,
		0,
		0 + 31,
		0 + 31 + 28,
		0 + 31 + 28 + 31,
		0 + 31 + 28 + 31 + 30,
		0 + 31 + 28 + 31 + 30 + 31,
		0 + 31 + 28 + 31 + 30 + 31 + 30,
		0 + 31 + 28 + 31 + 30 + 31 + 30 + 31,
		0 + 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31,
		0 + 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30,
		0 + 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31,
		0 + 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30,
	};

	const INT32U days_leap[] = 
	{
		0,
		0,
		0 + 31,
		0 + 31 + 29,
		0 + 31 + 29 + 31,
		0 + 31 + 29 + 31 + 30,
		0 + 31 + 29 + 31 + 30 + 31,
		0 + 31 + 29 + 31 + 30 + 31 + 30,
		0 + 31 + 29 + 31 + 30 + 31 + 30 + 31,
		0 + 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31,
		0 + 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30,
		0 + 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31,
		0 + 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30,
	};

	if ( time->century != 0x20 )		  //输入时间必须为2000年以后
		return 0;

	//计算出当前年的秒数
	i = Is_Leap((INT16U)bcdhex(time->century) * 100 + (INT16U)bcdhex(time->year));
	if ( i == TRUE )
	{	//是闰年
		sec = ( (days_leap[bcdhex(time->month)] + (INT32U)bcdhex(time->day)) * 24 * 60 * 60 
				+ (INT32U)bcdhex(time->hour) * 60 * 60 + (INT32U)bcdhex(time->minute) * 60 + (INT32U)bcdhex(time->second) );
	}
	else
	{
		sec = ( (days[bcdhex(time->month)] + (INT32U)bcdhex(time->day)) * 24 * 60 * 60 
				+ (INT32U)bcdhex(time->hour) * 60 * 60 + (INT32U)bcdhex(time->minute) * 60 + (INT32U)bcdhex(time->second) );
	}

	//计算出从前一年到2000年之间的秒数
	for ( year = 2000; year < ((INT16U)bcdhex(time->century) * 100 + (INT16U)bcdhex(time->year)); year++ )
	{
		if ( Is_Leap(year) == TRUE )
			sec += 366 * 24 * 60 * 60;
		else
			sec	+= 365 * 24 * 60 * 60;
	}

	return sec;
}

/*****************************************************************
函数原型：Is_Leap
功能描述： 计算是否为闰年
参数描述：
参数名称：	输入/输出？ 类型		描述
			year	输入		uint 		需要判断是否为闰年的年份
			
返	回	 值：	0-不是闰年
				1-是闰年
				
作		 者 ：刘及华
日		 期：2003-11-27
修改历史：
		日期		修改人		修改描述
		------		---------	-------------
*****************************************************************/
INT8U Is_Leap(INT16U year)
{
	return(year % 4 == 0) && ( (year % 100 != 0) || (year % 400 == 0) );
}

/******************************************************************************
 函数名称：cal_sec2000
 功能描述：计算从2000年1月1日到输入时间的秒数
 参数描述：
 参数名称：	输入/输出？	类型		描述
 time			输入			BUS_TIME*	要进行计算的时间，要求输入时间大于2000年
 
 返  回  值：秒数
   
 作      者	：刘及华
 日      期：2006.1.9
 修改历史：
		日期		修改人		修改描述

******************************************************************************/
INT32U cal_sec2000(_TIME * time)
{
	INT8U i;
	INT16U year;
	INT32U sec = 0;
	const INT32U days[] = 
	{
		0,
		0,
		0 + 31,
		0 + 31 + 28,
		0 + 31 + 28 + 31,
		0 + 31 + 28 + 31 + 30,
		0 + 31 + 28 + 31 + 30 + 31,
		0 + 31 + 28 + 31 + 30 + 31 + 30,
		0 + 31 + 28 + 31 + 30 + 31 + 30 + 31,
		0 + 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31,
		0 + 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30,
		0 + 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31,
		0 + 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30,
	};

	const INT32U days_leap[] = 
	{
		0,
		0,
		0 + 31,
		0 + 31 + 29,
		0 + 31 + 29 + 31,
		0 + 31 + 29 + 31 + 30,
		0 + 31 + 29 + 31 + 30 + 31,
		0 + 31 + 29 + 31 + 30 + 31 + 30,
		0 + 31 + 29 + 31 + 30 + 31 + 30 + 31,
		0 + 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31,
		0 + 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30,
		0 + 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31,
		0 + 31 + 29 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30,
	};

	if ( time->century != 0x20 )		  //输入时间必须为2000年以后
		return 0;

	//计算出当前年的秒数
	i = Is_Leap((INT16U)bcdhex(time->century) * 100 + (INT16U)bcdhex(time->year));
	if ( i == TRUE )
	{	//是闰年
		sec = ( (days_leap[bcdhex(time->month)] + (INT32U)bcdhex(time->day)) * 24UL * 60UL * 60UL 
				+ (INT32U)bcdhex(time->hour) * 60UL * 60UL + (INT32U)bcdhex(time->minute) * 60UL + (INT32U)bcdhex(time->second) );
	}
	else
	{
		sec = ( (days[bcdhex(time->month)] + (INT32U)bcdhex(time->day)) * 24UL * 60UL * 60UL 
				+ (INT32U)bcdhex(time->hour) * 60UL * 60UL + (INT32U)bcdhex(time->minute) * 60UL + (INT32U)bcdhex(time->second) );
	}

	//计算出从前一年到2000年之间的秒数
	for ( year = 2000; year < ((INT16U)bcdhex(time->century) * 100 + (INT16U)bcdhex(time->year)); year++ )
	{
		if ( Is_Leap(year) == TRUE )
			sec += (366UL * 24UL * 60UL * 60UL);
		else
			sec	+= (365UL * 24UL * 60UL * 60UL);
	}

	return sec;
}

///******************************************************************************
//函数名称：com1_ctrl_init
//功能描述：com1控制初始化
//参数描述：
//
//返  回  值	：无
//作      者	：刘及华
//日      期：2005-04-08
//修改历史：
//日期		修改人		修改描述
//------		---------	---------
//******************************************************************************/
//void com1_ctrl_init(void)
//{
//    ToggleWD();
//
//    PINSEL3 = PINSEL3 & (~(BIT12 | BIT13));     //P1.22 set gpio mode (ZHUANSU_CTRL0)
//    PINSEL3 = PINSEL3 & (~(BIT14 | BIT15));     //P1.23 set gpio mode (ZHUANSU_CTRL1)
//    PINSEL3 = PINSEL3 & (~(BIT16 | BIT17));     //P1.24 set gpio mode (ZHUANSU_CTRL2)
//    PINSEL3 = PINSEL3 & (~(BIT18 | BIT19));     //P1.25 set gpio mode (ZHUANSU_CTRL3)
//
//
//    FIO1DIR |= BIT22;           //P1.22 set output
//    FIO1DIR |= BIT23;           //P1.23 set output
//    FIO1DIR |= BIT24;           //P1.24 set output
//    FIO1DIR |= BIT25;           //P1.25 set output
//
//    //3-0 0000 txd1_00 rxd1_00
//    //3-0 0001 rxd1_01 rxd1_01
//    ZHUANSU_CTRL3_SetLow();
//    ZHUANSU_CTRL2_SetLow();
//    ZHUANSU_CTRL1_SetLow();
//    ZHUANSU_CTRL0_SetLow();
//}

/*****************************************************************
函数原型：trans_to_int32u
功能描述：将4字节数组转换为INT32U的数
参数描述：
参数名称：	输入/输出？ 类型		描述
buf			输入		INT8U *		需要进行转换的字符串
			
返	回	 值：转换结果 
 
作		 者：刘及华
日		 期：2003-11-27
修改历史：
		日期		修改人		修改描述
		------		---------	-------------
*****************************************************************/
INT32U trans_to_int32u(INT8U *buf)
{
	INT32U i = 0;

	i = (INT32U)buf[0] * 0x1000000 + (INT32U)buf[1] * 0x10000 + (INT32U)buf[2] * 0x100 + (INT32U)buf[3];
	return i;
}

/*****************************************************************
函数原型：trans_to_int16u
功能描述：将4字节数组转换为INT16U的数
参数描述：
参数名称：	输入/输出？ 类型		描述
buf			输入		INT8U *		需要进行转换的字符串
			
返	回	 值：转换结果 
 
作		 者：刘及华
日		 期：2003-11-27
修改历史：
		日期		修改人		修改描述
		------		---------	-------------
*****************************************************************/
INT16U trans_to_int16u(INT8U *buf)
{
	INT16U i = 0;

	i = (INT16U)buf[0] * 0x100 + (INT16U)buf[1];
	return i;
}


/******************************************************************************
 函数名称：ascii_to_hex
 功能描述：把一个0-9的ascii代码，转化为16进制数
 参数名称： 输入/输出？ 类型        描述
 num        输入        INT8U           要转化为ascii代码的数，要求范围0-9 a-f A-F

 返  回  值：转化后的16进制数

 作      者 ：刘及华
 日      期：2004-09-02
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
INT8U ascii_to_hex(INT8U num)
{
	ToggleWD();

// 	return (num - 0x30);
	if ( num <= '9' )
		return(num - 0x30);
	else if ( num == 'a' || num == 'A' )
		return 0x0A;
	else if ( num == 'b' || num == 'B' )
		return 0x0B;
	else if ( num == 'c' || num == 'C' )
		return 0x0C;
	else if ( num == 'd' || num == 'D' )
		return 0x0D;
	else if ( num == 'e' || num == 'E' )
		return 0x0E;
	else
		return 0x0F;
}

/*****************************************************************
函数原型：bcdhex
功能描述：bcd码转换为hex码
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------
bcd				输入			INT8U		输入的bcd码

返  回  值：转换后的hex码
作      者	：刘及华
日      期：2006-01-09
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U bcdhex(INT8U bcd)
{
	ToggleWD();

	return((bcd >> 4) * 10 + (bcd & 0x0F));
}

/******************************************************************************
 函数名称：hexbcd
 功能描述：把一个16进制数，转化为bcd码
 参数名称： 输入/输出？ 类型        描述
 num        输入        INT8U           要转化为bcd码的数

 返  回  值：转化后的bcd码

 作      者 ：刘及华
 日      期：2004-09-02
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
INT8U hexbcd(INT8U num)
{
	return( ((num / 10) << 4) | (num % 10) );
}

/******************************************************************************
 函数名称：ascii
 功能描述：把一个0-9的数转化为ascii代码，此函数也可扩展用于通讯中，通讯中涉及到一些A-F的非法输入，认为合法
 参数名称：	输入/输出？	类型		描述
 num		输入		INT8U			要转化为ascii代码的数，要求范围0-9
 
 返  回  值：转化后的ascii代码
 
 作      者	：刘及华
 日      期：2004-09-02
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
INT8U ascii(INT8U num)
{
	ToggleWD();

//     return (num + 0x30);
	if ( num <= 9 )
		return(num + 0x30);
	else if ( num == 0x0A )
		return 'A';
	else if ( num == 0x0B )
		return 'B';
	else if ( num == 0x0C )
		return 'C';
	else if ( num == 0x0D )
		return 'D';
	else if ( num == 0x0E )
		return 'E';
	else
//      if (num == 0x0F)
		return 'F';
}

// *****************************************************************
// 功能：在屏幕上输出字符
// 入口参数：	lb->显示输出字符
// 出口参数：	Null
// 作者：
// 函数出处：EH0218---COMMON.C例子程序
// *****************************************************************
char HextoBcd(char sour)
{
	ToggleWD();

	return(((sour/10)<<4) |(sour%10));
}

/******************************************************************************
 函数名称：htoa
 功能描述：hex转换为ascii码
 参数描述：
 参数名称： 输入/输出？ 类型		描述

 返  回  值：无

 作 	 者 ：刘及华
 日 	 期：2005-04-07
 修改历史：
	 日期		修改人		修改描述
	 ------		---------	-------------
******************************************************************************/
void htoa(void *des, INT8U *sour, int hex_len)
{
	int i = 0;
	char *dest = (char *)des;

	if ( hex_len<=0 )
		return;

	for ( i=0; i<hex_len; i++ )
		sprintf((char *)dest + i * 2, "%02x", sour[i]);

	dest[hex_len * 2] = '\0';
}

/******************************************************************************
 函数名称：atoh
 功能描述：ascii转换为hex码
 参数描述：
 参数名称： 输入/输出？ 类型		描述
				
 返  回  值：无
				   
 作 	 者 ：刘及华
 日 	 期：2005-04-07
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void atoh(void *des, INT8U *sour, int ascii_len)
{
	int i = 0;
	char *dest = (char *)des;

	ToggleWD();

	if ( ascii_len<=0 )
		return;

	for ( i=0; i<ascii_len; i++ )
	{
		if ( i % 2 == 0 )
			continue;

		dest[i / 2] = (ascii_to_hex(sour[i - 1]) << 4) | ascii_to_hex(sour[i]);
	}
}

/******************************************************************************
 函数名称：ascii_comm
 功能描述：把一个0-9的数转化为ascii代码，此函数也可扩展用于通讯中，通讯中涉及到一些A-F的非法输入，认为合法
 参数名称：	输入/输出？	类型		描述
 num		输入		INT8U			要转化为ascii代码的数，要求范围0-9
 
 返  回  值：转化后的ascii代码
 
 作      者	：刘及华
 日      期：2004-09-02
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
INT8U ascii_comm(INT8U num)
{
	return(num + 0x30);
}

/******************************************************************************
 函数名称：BCC_Check
 功能描述：对接收到的数据进行BCC校验
 参数描述：
 参数名称：	输入/输出？	类型		描述
 rcv_buf	输入		const INT8U*		要进行BCC校验的数据缓冲区
 rcvd_len	输入		INT8U		接收到的长度
				
 返  回  值：ok(0)-成功
				 notok(0xFF)-失败(BCC校验错误)
				   
 作      者	：刘及华
 日      期：2004-09-02
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
INT8U BCC_Check(const INT8U *rcv_buf, INT8U rcvd_len)
{
	INT8U i = 0;
	INT8U bcc = 0;

	ToggleWD();

	for ( i=0; i<rcvd_len-4; i++ )
	{
		bcc ^= rcv_buf[i];
	}

	if ( (rcv_buf[rcvd_len - 3] == ((bcc & 0x0F) + 0x30)) && (rcv_buf[rcvd_len - 4] == (((bcc >> 4) & 0x0F) + 0x30)) )
		return ok;
	else
		return notok;
}

/*****************************************************************
函数原型：Cal_Crc
 功能描述：计算一串数据的CRC值，采用CCITT算法
 参数描述：
 参数名称：	输入/输出？	类型		描述
 data_to_cal_crc	输入	INT8U const *		要进行CRC计算的数组的起始指针
 len				 		输入	INT16U				要进行CRC计算的数组的长度

 返  回  值：uint型的CRC值(2字节)
 作      者	：刘及华
 日      期：2004-02-04
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
*****************************************************************/
INT16U Cal_Crc(INT8U const *data_to_cal_crc, INT16U len)
{
	INT16U crc;
	INT8U i;

	ToggleWD();

	crc = 0xFFFF;			//CRC初始值

	while ( len-- != 0 )
	{
		ToggleWD();

		crc = crc ^ ((INT16U)*data_to_cal_crc++ << 8);

		for ( i = 0; i < 8; ++i )
			if ( crc & 0x8000 )
				crc = (crc << 1) ^ 0x1021;
			else
				crc	= crc << 1;
	}

	return crc;
}

/*****************************************************************
函数原型：cal_crc
 功能描述：计算一串数据的CRC值，采用CCITT算法
 参数描述：
 参数名称：	输入/输出？	类型			描述
 buf		输入		INT8U const *	要进行CRC计算的数组的起始指针
 len		输入		INT16U			要进行CRC计算的数组的长度

 返  回  值：uint型的CRC值(2字节)
 作      者：刘及华
 日      期：2004-02-04
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
*****************************************************************/
INT16U cal_crc(INT8U const *buf, INT16U len)
{
	return Cal_Crc(buf, len);
}

/******************************************************************************
 函数名称：crc_check
 功能描述：对接收到的数据进行CRC校验
 参数描述：
 参数名称：	输入/输出？	类型		描述
 rcv_buf	输入		const INT8U*	要进行BCC校验的数据缓冲区
 rcvd_len	输入		INT16U		接收到的长度
				
 返  回  值：ok(0)-成功
				 notok(0xFF)-失败(BCC校验错误)
				   
 作      者	：刘及华
 日      期：2004-09-02
 修改历史：
		日期		修改人		修改描述
		------		---------	------------- 
******************************************************************************/
INT8U crc_check(const INT8U *rcv_buf, INT16U rcvd_len)
{
	INT16U crc = 0;
	//  INT16U crc_rcv = 0;
//	INT8U crc_ascii[10];

	ToggleWD();

	crc = Cal_Crc((void *)rcv_buf, rcvd_len - 2);

	if ( (rcv_buf[rcvd_len - 2] == (INT8U)(crc / 256)) && (rcv_buf[rcvd_len - 1] == (INT8U)(crc % 256)) )
		return ok;
	else 
		return notok;

}


/******************************************************************************
 函数名称：is_black_card
 功能描述：判断是否是黑卡，如果不是，添加进黑名单里边
 参数描述：
 参数名称：	
				
 返  回  值：ok(0)-成功不是黑名单的卡，此时添加进去黑名单里边
    		 notok(0xFF)-失败，是黑卡不能刷卡
   
				   
 作      者	：刘及华
 日      期：2016-07-02
 修改历史：
 日期		修改人		修改描述
******************************************************************************/
INT8U is_black_card(INT8U *card_no)
{
	INT8U i = 0;
	//INT8U j = 0;

	if ( DevStat.allow_upload_data == 0 ) 
	{
		if( BLACK_LENTH == 0 )   //目前还没有黑名单，进来的第一个,临时当做黑卡处理
		{
			memcpy(BLACK_NAME_CARD, card_no, 3);
			BLACK_LENTH++;
			return ok;
		}

		if( BLACK_LENTH >= 80) 
			BLACK_LENTH = 80;

		for( i=0; i<BLACK_LENTH; i++ )
		{
			if(memcmp(card_no, &BLACK_NAME_CARD[i][0], 2) == 0)	//确实是黑名单的卡
			{
				if(BLACK_NAME_CARD[i][2] > *(card_no+2))
					return notok;				//这里直接退出，挂卡的次数比目前的还大
				else if(BLACK_NAME_CARD[i][2] == *(card_no+2))
					return ok;
				else
				{
					//BLACK_LENTH++;
					memcpy(&BLACK_NAME_CARD[i][0], card_no, 3);
					return ok;
				}
			}
		}

		if((i>=BLACK_LENTH) && (*(card_no+2) != 0))   //新进来的黑卡，要添加进去黑名单里边
		{
			BLACK_LENTH++;
			memcpy(&BLACK_NAME_CARD[BLACK_LENTH][0], card_no, 3);
			return ok;
		}

		return ok;  //不是黑名单的卡
	}
	else
	{
		if( BLACK_LENTH == 0 )   //目前还没有黑名单，进来的第一个
		{
			AT24CXX_Write(0, (u8*)card_no, 3);	 
			BLACK_LENTH++;
			AT24CXX_WriteOneByte(254, BLACK_LENTH);
			return ok;
		}

		if( BLACK_LENTH >= 80) 
			BLACK_LENTH = 80;

		for( i=0; i<BLACK_LENTH; i++ )
		{
			AT24CXX_Read(i*3, &BLACK_NAME_CARD[i][0], 3);
			if(memcmp(card_no, &BLACK_NAME_CARD[i][0], 2) == 0)	//确实是黑名单的卡
			{
				if(BLACK_NAME_CARD[i][2] > *(card_no+2))
					return notok;				//这里直接退出，报错！
				else if(BLACK_NAME_CARD[i][2] == *(card_no+2))
					return ok;
				else
				{
					//BLACK_LENTH++;
					//memcpy(&BLACK_NAME_CARD[i][0], card_no, 3);
					AT24CXX_Write(i*3, (u8*)card_no, 3);
					return ok;
				}
			}
		}

		if((i>=BLACK_LENTH) && (*(card_no+2) != 0))   //游历一遍完成，是新进来的黑卡，要添加进去黑名单里边
		{
			//BLACK_LENTH++;
			//memcpy(&BLACK_NAME_CARD[BLACK_LENTH][0], card_no, 3);
			AT24CXX_Write(BLACK_LENTH*3, (u8*)card_no, 3);	 
			BLACK_LENTH++;
			AT24CXX_WriteOneByte(254, BLACK_LENTH);
			return ok;
		}
//		else											//游历一遍未完成，是之前的一个卡号，重复的要覆盖原来的位置
//		{
//			AT24CXX_Write(BLACK_LENTH*3, (u8*)card_no, 3);
//			return ok;
//		}

		return ok;  //不是黑名单的卡
	}
}

