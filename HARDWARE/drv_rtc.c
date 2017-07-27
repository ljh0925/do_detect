/******************************************************************************
Copyright 2005 大连易达通电子有限公司
All rights riserved.

文件名	：drv_rtc.c
模块名称：时钟芯片驱动程序
功能概要：时钟芯片驱动程序

取代版本：0.0.1
修改人	：刘及华
完成日期：2005.04.06
升级说明：create

******************************************************************************/
#include "all.h"

#ifdef I2C_USE_SEM
extern OS_EVENT			*i2c_sem;
#endif

#ifdef RTC_USE_SEM
OS_EVENT *g_rtc_sem;           //访问时钟芯片二进制信号量
#endif



//月份数据表
INT8U const table_week[12] = { 0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5 }; //月修正数据表
INT8U const mon_table[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };       //平年的月份日期表



/*****************************************************************
函数原型：rtc_init
功能描述：时间初始化
参数描述：
参数名称：	输入/输出？	类型		描述

返  回  值：无
作      者：刘及华
日      期：2005-04-12
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
void rtc_init(void)
{
//	//  INT32U i;
//	INT32U k;
//
//	ToggleWD();
//
//	g_rtc_sem = OSSemCreate(1);
//
//	if ( BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5 )      //从指定的后备寄存器中读出数据:读出了与写入的指定数据不相乎
//	{
//		//RTC上电后第一次配置
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);    //使能PWR和BKP外设时钟
//		PWR_BackupAccessCmd(ENABLE);    //使能后备寄存器访问
//		BKP_DeInit();   //复位备份区域
//		RCC_LSEConfig(RCC_LSE_ON);  //设置外部低速晶振(LSE),使用外设低速晶振
//		while ( RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET ) //检查指定的RCC标志位设置与否,等待低速晶振就绪
//		{
//			k++;
//			SleepMs(10);
//			if ( k >= 250 )
//				return; //初始化时钟失败,晶振有问题
//		}
//		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);     //设置RTC时钟(RTCCLK),选择LSE作为RTC时钟
//		RCC_RTCCLKCmd(ENABLE);  //使能RTC时钟
//		RTC_WaitForLastTask();  //等待最近一次对RTC寄存器的写操作完成
//		RTC_WaitForSynchro();       //等待RTC寄存器同步
//		RTC_ITConfig(RTC_IT_SEC, ENABLE);       //使能RTC秒中断
//		RTC_WaitForLastTask();  //等待最近一次对RTC寄存器的写操作完成
//		RTC_EnterConfigMode(); /// 允许配置
//		RTC_SetPrescaler(32767); //设置RTC预分频的值
//		RTC_WaitForLastTask();  //等待最近一次对RTC寄存器的写操作完成
//		RTC_Set(1970, 1, 1, 0, 0, 0);  //设置时间
//		RTC_ExitConfigMode(); //退出配置模式
//		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);   //向指定的后备寄存器中写入用户程序数据
//
//		return;
//	}
//
//	//不是第一次上电
//	RTC_WaitForSynchro();   //等待最近一次对RTC寄存器的写操作完成
//	RTC_ITConfig(RTC_IT_SEC, ENABLE);   //使能RTC秒中断
//	RTC_WaitForLastTask();  //等待最近一次对RTC寄存器的写操作完成


	INT8U i;
	INT8U m;

#ifdef RTC_USE_SEM
	g_rtc_sem = OSSemCreate(1);
#endif

	for ( i=0; i<10; i++ )
	{
		m = write_8025(CTRL1_REG, 0x20);		//24小时模式
		if ( m != ok )
			continue;

		m = write_8025(CTRL2_REG, 0x20);		//2.1v电压检测
		if ( m != ok )
			continue;

		break;
	}

//	if ( i >= 10 )
//		return notok;
//	else
//		return ok;

}

///*****************************************************************
//函数原型：Get_Time
//功能描述：读取时间，读取两次，为了防止偶尔一次不成功
//参数描述：
//参数名称：	输入/输出？	类型		描述
//time		输出   		_TIME*		7个字节长，数据从头到尾顺序是世纪 年 月 日 时 分 秒
//返  回  值：ok(0)-成功
//				notok(0xFF)－失败
//作      者：刘及华
//日      期：2005-04-12
//修改历史：
//日期		修改人		修改描述
//------		---------	-------------
//*****************************************************************/
//INT8U Get_Time(_TIME *time)
//{
//	INT8U i = 0;
//	CALENDART calen;
//#ifdef RTC_USE_SEM
//	INT8U err = 0;
//#endif
//
//	ToggleWD();
//
//#ifdef RTC_USE_SEM
//	OSSemPend(g_rtc_sem, 0, &err);
//#endif
//
//	i = RTC_Get(&calen);
//
//	time->century = hexbcd((INT8U)(calen.year / 100));
//	time->year = hexbcd((INT8U)(calen.year % 100));
//	time->month = hexbcd(calen.month);
//	time->day = hexbcd(calen.day);
//	time->hour = hexbcd(calen.hour);
//	time->minute = hexbcd(calen.min);
//	time->second = hexbcd(calen.sec);
//
//#ifdef RTC_USE_SEM
//	(void)OSSemPost(g_rtc_sem);
//#endif
//	return i;
//}
//
///*****************************************************************
//函数原型：Modify_Time
//功能描述：修改时间，修改两次，为了防止偶尔一次不成功
//参数描述：
//参数名称：	输入/输出？	类型		描述
//*time		输入		_TIME*		7个字节长，数据从头到尾顺序是世纪 年 月 日 时 分 秒
//返  回  值：ok(0)-成功
//				notok(0xFF)－失败
//作      者：刘及华
//日      期：2005-04-12
//修改历史：
//日期		修改人		修改描述
//------		---------	-------------
//*****************************************************************/
//INT8U Modify_Time(_TIME const * const time)
//{
//	INT8U i;
//#ifdef RTC_USE_SEM
//	INT8U err = 0;
//#endif
//
//	ToggleWD();
//
//#ifdef RTC_USE_SEM
//	OSSemPend(g_rtc_sem, 0, &err);
//#endif
//
//	i = RTC_Set((INT16U)bcdhex(time->century) * 100 + (INT16U)bcdhex(time->year),
//				bcdhex(time->month), bcdhex(time->day), bcdhex(time->hour), bcdhex(time->minute), bcdhex(time->second));  //设置时间
//
//#ifdef RTC_USE_SEM
//	(void)OSSemPost(g_rtc_sem);
//#endif
//
//	return i;
//}
//
/*****************************************************************
函数原型：CheckTimeFormat
功能描述：检查时间格式是否合法
参数描述：
参数名称：	输入/输出？	类型		描述
time		输入  			const _TIME *	7个字节长，数据从头到尾顺序是世纪 年 月 日 时 分 秒

返  回  值：ok(0)-成功 
				notok(0xFF)－失败
作      者：刘及华
日      期：2003-05-12
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U CheckTimeFormat(const _TIME *time)
{
	INT8U temp;

	ToggleWD();

	temp = time->month;
	if ( temp >= 0x0a )       //月份应在1－12之间
	{
		if ( temp >= 0x10 && temp <= 0x12 )
		;
		else
			return notok;
	}

	temp = time->day;
	if ( temp > 0x31 )                   //天应在1-31之间
		return notok;
	temp = temp & 0x0F;
	if ( temp > 0x09 )
		return notok;

	temp = time->hour;
	if ( temp > 0x23 )                                   //小时应在0-23之间
		return notok;
	temp = temp & 0x0F;
	if ( temp > 0x09 )
		return notok;

	temp = time->minute;
	if ( temp > 0x59 )                                   //分钟应在0-59之间
		return notok;
	temp = temp & 0x0F;
	if ( temp > 0x09 )
		return notok;

	temp = time->second;
	if ( temp > 0x59 )                                    //秒应在0-59之间
		return notok;
	temp = temp & 0x0F;
	if ( temp > 0x09 )
		return notok;

	return ok;
}


//判断是否是闰年函数
//月份   1  2  3  4  5  6  7  8  9  10 11 12
//闰年   31 29 31 30 31 30 31 31 30 31 30 31
//非闰年 31 28 31 30 31 30 31 31 30 31 30 31
//输入:年份
//输出:该年份是不是闰年.1,是.0,不是
/*****************************************************************
函数原型：Is_Leap_Year
功能描述：判断是否是闰年
参数描述：
参数名称：	输入/输出？	类型		描述
year		输入  		INT16U		年份

返  回  值：该年份是不是闰年.1,是. 0,不是
作      者：刘及华
日      期：2003-05-12
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U Is_Leap_Year(INT16U year)
{
	if ( year % 4 == 0 ) //必须能被4整除
	{
		if ( year % 100 == 0 )
		{
			if ( year % 400 == 0 )
				return 1; //如果以00结尾,还要能被400整除
			else
				return 0;
		}
		else
			return 1;
	}
	else
		return 0;
}

///*****************************************************************
//函数原型：RTC_Set
//功能描述：设置时钟,把输入的时钟转换为秒钟,以1970年1月1日为基准,1970~2099年为合法年份
//参数描述：
//参数名称：	输入/输出？	类型		描述
//syear		输入  		INT16U		年份hex
//smon		输入  		INT8U		月hex
//sday		输入  		INT8U		日hex
//hour		输入  		INT8U		时hex
//min			输入  		INT8U		分hex
//sec			输入  		INT8U		秒hex
//
//返  回  值：ok(0x00),成功;其他:错误代码.
//作      者：刘及华
//日      期：2003-05-12
//修改历史：
//日期		修改人		修改描述
//------		---------	-------------
//*****************************************************************/
//INT8U RTC_Set(INT16U syear, INT8U smon, INT8U sday, INT8U hour, INT8U min, INT8U sec)
//{
//	INT16U t;
//	INT32U seccount = 0;
//
//	if ( syear < 1970 || syear > 2099 )
//		return notok;
//
//	for ( t = 1970; t < syear; t++ ) //把所有年份的秒钟相加
//	{
//		if ( Is_Leap_Year(t) )
//			seccount += 31622400; //闰年的秒钟数
//		else
//			seccount += 31536000;              //平年的秒钟数
//	}
//	smon -= 1;
//	for ( t = 0; t < smon; t++ )    //把前面月份的秒钟数相加
//	{
//		seccount += (INT32U)mon_table[t] * 86400; //月份秒钟数相加
//		if ( Is_Leap_Year(syear) && t == 1 )
//			seccount += 86400; //闰年2月份增加一天的秒钟数
//	}
//	seccount += (INT32U)(sday - 1) * 86400; //把前面日期的秒钟数相加
//	seccount += (INT32U)hour * 3600; //小时秒钟数
//	seccount += (INT32U)min * 60;   //分钟秒钟数
//	seccount += sec; //最后的秒钟加上去
//
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);    //使能PWR和BKP外设时钟
//	PWR_BackupAccessCmd(ENABLE);    //使能RTC和后备寄存器访问
//	RTC_SetCounter(seccount);   //设置RTC计数器的值
//
//	RTC_WaitForLastTask();  //等待最近一次对RTC寄存器的写操作完成
//	return ok;
//}
//
///*****************************************************************
//函数原型：RTC_Get
//功能描述：得到当前的时间
//参数描述：
//参数名称：	输入/输出？	类型		描述
//calendar	输出  		CALENDART*	取回的时间
//
//返  回  值：ok(0x00),成功;其他:错误代码.
//作      者：刘及华
//日      期：2003-05-12
//修改历史：
//日期		修改人		修改描述
//------		---------	-------------
//*****************************************************************/
//INT8U RTC_Get(CALENDART *calendar)
//{
//	INT32U timecount = 0;
//	INT32U temp = 0;
//	INT16U temp1 = 0;
//
//	timecount = RTC->CNTH; //得到计数器中的值(秒钟数)
//	timecount <<= 16;
//	timecount += RTC->CNTL;
//
//	temp = timecount / 86400;   //得到天数(秒钟数对应的)
//
//	temp1 = 1970; //从1970年开始
//	while ( temp >= 365 )
//	{
//		if ( Is_Leap_Year(temp1) ) //是闰年
//		{
//			if ( temp >= 366 )
//			{
//				temp -= 366; //闰年的秒钟数
//			}
//			else
//			{
//				temp1++;
//				break;
//			}
//		}
//		else
//		{
//			temp -= 365;   //平年
//		}
//		temp1++;
//	}
//	calendar->year = temp1; //得到年份
//
//	temp1 = 0;
//	while ( temp >= 28 ) //超过了一个月
//	{
//		if ( Is_Leap_Year(calendar->year) && temp1 == 1 ) //当年是不是闰年/2月份
//		{
//			if ( temp >= 29 )
//				temp -= 29; //闰年的秒钟数
//			else
//				break;
//		}
//		else
//		{
//			if ( temp >= mon_table[temp1] )
//				temp -= mon_table[temp1]; //平年
//			else
//				break;
//		}
//		temp1++;
//	}
//	calendar->month = temp1 + 1;   //得到月份
//	calendar->day = temp + 1;     //得到日期
//
//	temp = timecount % 86400;           //得到秒钟数
//	calendar->hour = temp / 3600;        //小时
//	calendar->min = (temp % 3600) / 60;    //分钟
//	calendar->sec = (temp % 3600) % 60;    //秒钟
//	calendar->week = RTC_Get_Week(calendar->year, calendar->month, calendar->day); //获取星期
//	return ok;
//}
//
///*****************************************************************
//函数原型：RTC_Get_Week
//功能描述：获得现在是星期几,输入公历日期得到星期(只允许1901-2099年)
//参数描述：
//参数名称：	输入/输出？	类型		描述
//syear		输入  		INT16U		年份hex
//smon		输入  		INT8U		月hex
//sday		输入  		INT8U		日hex
//
//返  回  值：星期号
//作      者：刘及华
//日      期：2003-05-12
//修改历史：
//日期		修改人		修改描述
//------		---------	-------------
//*****************************************************************/
//INT8U RTC_Get_Week(INT16U year, INT8U month, INT8U day)
//{
//	INT16U temp2;
//	INT8U yearH, yearL;
//
//	yearH = year / 100;	yearL = year % 100;
//	// 如果为21世纪,年份数加100
//	if ( yearH > 19 )
//		yearL += 100;
//	// 所过闰年数只算1900年之后的
//	temp2 = yearL + yearL / 4;
//	temp2 = temp2 % 7;
//	temp2 = temp2 + day + table_week[month - 1];
//	if ( yearL % 4 == 0 && month < 3 )
//		temp2--;
//
//	return ( temp2 % 7 );
//}

/*****************************************************************
函数原型：time_read
功能描述：读取时间
参数描述：
参数名称：	输入/输出？	类型		描述
time		输出   		_TIME*		7个字节长，数据从头到尾顺序是世纪 年 月 日 时 分 秒
返  回  值：ok(0)-成功 
				notok(0xFF)－失败
作      者：刘及华
日      期：2005-04-12
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U time_read(_TIME *time)
{
	return ( Get_Time(time) );
}

/*****************************************************************
函数原型：time_write
功能描述：修改时间
参数描述：
参数名称：	输入/输出？	类型		描述
*time		输入		_TIME*		7个字节长，数据从头到尾顺序是世纪 年 月 日 时 分 秒
返  回  值：ok(0)-成功 
				notok(0xFF)－失败
作      者：刘及华
日      期：2005-04-12
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U time_write(_TIME const * const time)
{
	return ( Modify_Time(time) );
}



/*****************************************************************
函数原型：write_8025
功能描述：把某个数值写到rx－8025的寄存器中
参数描述：
参数名称：	输入/输出？	类型		描述
	which_reg	输入				INT8U		8025 寄存器地址
	value		输入				INT8U		要写入的值

返  回  值：ok(0)-成功 
				notok(0xFF)－失败
作      者	：刘及华
日      期：2004-12-1
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U write_8025(INT8U which_reg, INT8U value)
{
	INT8U i = 0;
	INT8U j = 0;
//  INT8U h = 0;
#ifdef I2C_USE_SEM
	INT8U err = 0;
#endif

	ToggleWD();

#ifdef I2C_USE_SEM
	OSSemPend(i2c_sem, 0, &err);
#endif
//	OS_ENTER_CRITICAL();

	i = i2c_start();					//如果I2c_Start不过，重试10次
	if ( i != ok )
	{
		j = 20;
		while ( j )
		{
// 			I2c_Nack();			//add by xuyan for debug
			i2c_stop();
			i = i2c_start();
			if ( i != ok )
			{
				j--;
			}
			else
			{
				break;
			}
			if ( j == 0 )
			{
				i2c_stop();
//				OS_EXIT_CRITICAL();
#ifdef I2C_USE_SEM
				OSSemPost(i2c_sem);
#endif
				return notok;
			}
		}
	}

	i = i2c_send(RX8025_WRITE);	//发送硬件地址以及写信号
	if (i != ok)
	{
		i2c_stop();
//		OS_EXIT_CRITICAL();
#ifdef I2C_USE_SEM
		OSSemPost(i2c_sem);
#endif
		return notok;
	}

//  i = i2c_send((which_reg << 4) & 0xF0);  //发送操作寄存器地址("0")
	i = i2c_send(which_reg);	//发送操作寄存器地址("0")
	if (i != ok)
	{
		i2c_stop();
//		OS_EXIT_CRITICAL();
#ifdef I2C_USE_SEM
		OSSemPost(i2c_sem);
#endif
		return notok;
	}

	i = i2c_send(value);	//发送硬件地址以及写信号
	if (i != ok)
	{
		i2c_stop();
//		OS_EXIT_CRITICAL();
#ifdef I2C_USE_SEM
		OSSemPost(i2c_sem);
#endif
		return notok;
	}

	i2c_stop();

//	OS_EXIT_CRITICAL();
#ifdef I2C_USE_SEM
		OSSemPost(i2c_sem);
#endif
	return ok;
}




/////////////////////////////////////////////////////////
// 以下为不需要改写部分
/////////////////////////////////////////////////////////


/*****************************************************************
函数原型：Get_Time
功能描述：读取时间，读取两次，为了防止偶尔一次不成功
参数描述：
参数名称：	输入/输出？	类型		描述
*time_data	输出   _TIME	7个字节长，数据从头到尾顺序是世纪 年 月 日 时 分 秒
返  回  值：ok(0)-成功 
				notok(0xFF)－失败
作      者	：刘及华
日      期：2005-04-12
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U Get_Time(_TIME *time_data)
{
	INT8U i = 0;
	INT8U j = 0;
#ifdef RTC_USE_SEM
	INT8U err = 0;
	OSSemPend(g_rtc_sem, 0, &err);
#endif

	for(j=0; j<10; j++)
	{
		i = Get_Time_Lowlvl(time_data);
		if ( i == ok )
		{
#ifdef RTC_USE_SEM
	(void)OSSemPost(g_rtc_sem);
#endif
			return ok;
		}
	}

#ifdef RTC_USE_SEM
	(void)OSSemPost(g_rtc_sem);
#endif
	return notok;
}

/*****************************************************************
函数原型：Get_Time_Lowlvl
功能描述：读取rx－8025	内的时间
参数描述：
参数名称：	输入/输出？	类型		描述
*time_data	输出   _TIME	7个字节长，数据从尾到头顺序是世纪 年 月 日 时 分 秒
返  回  值：ok(0)-成功 
				notok(0xFF)－失败
作      者	：刘及华
日      期：2005-04-12
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U Get_Time_Lowlvl(_TIME *time_data)
{
	INT8U i = 0;
	INT8U start_reg = 0;		//从第0个寄存器开始读取
	INT8U buf[9];

	ToggleWD();

	i = i2c_send_and_receive(RX8025, start_reg, buf, 7);
	if (i != ok)
	{
		return notok;
	}

	time_data->second = buf[0];				//秒
	time_data->minute = buf[1];				//分
	time_data->hour = buf[2];				//小时
	time_data->day = buf[4];				//日
	time_data->month = buf[5];				//月
	time_data->year = buf[6];				//年
	time_data->century = 0x20;				//世纪定死为0x20

	return ok;
}

/*****************************************************************
函数原型：Modify_Time
功能描述：修改时间，修改两次，为了防止偶尔一次不成功
参数描述：
参数名称：	输入/输出？	类型		描述
*time_data	输入	_TIME	7个字节长，数据从头到尾顺序是世纪 年 月 日 时 分 秒
返  回  值：ok(0)-成功 
				notok(0xFF)－失败
作      者	：刘及华
日      期：2005-04-12
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U Modify_Time(_TIME const * const time_data)
{
	INT8U i = 0;
	INT8U j = 0;
#ifdef RTC_USE_SEM
	INT8U err = 0;
	OSSemPend(g_rtc_sem, 0, &err);
#endif

	for(j=0; j<10; j++)
	{
		i = Modify_Time_Lowlvl(time_data);
		if ( i == ok )
		{
#ifdef RTC_USE_SEM
	(void)OSSemPost(g_rtc_sem);
#endif
			return ok;
		}
	}

#ifdef RTC_USE_SEM
	(void)OSSemPost(g_rtc_sem);
#endif
	return notok;
}

/*****************************************************************
函数原型：Modify_Time_Lowlvl
功能描述：修改rx-8025 内的时间
参数描述：
参数名称：	输入/输出？	类型		描述
*time_data	输入	_TIME	7个字节长，数据从尾到头顺序是
											世纪 年 月 日 时 分 秒
返  回  值：ok(0)-成功 
				notok(0xFF)－失败
作      者	：刘及华
日      期：2003-05-12
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U Modify_Time_Lowlvl(_TIME const * const time_data)			//指针指向的数是不可修改的，同时指针也是不可修改的
{
	INT8U i;

	ToggleWD();

	i = write_8025(CTRL1_REG, 0x20);		//24小时模式
	if ( i != ok )
		return notok;

	i = write_8025(SEC_REG, time_data->second);				//秒
	if ( i != ok )
		return notok;

	i = write_8025(MIN_REG, time_data->minute);				//分
	if ( i != ok )
		return notok;

	i = write_8025(HOUR_REG, time_data->hour);				//小时
	if ( i != ok )
		return notok;

	i = write_8025(DAY_REG, time_data->day);					//日
	if ( i != ok )
		return notok;

	i = write_8025(MONTH_REG, time_data->month);				//月
	if ( i != ok )
		return notok;

	i = write_8025(YEAR_REG, time_data->year);				//年
	if ( i != ok )
		return notok;

	return ok;
}



