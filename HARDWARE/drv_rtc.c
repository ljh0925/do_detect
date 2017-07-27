/******************************************************************************
Copyright 2005 �����״�ͨ�������޹�˾
All rights riserved.

�ļ���	��drv_rtc.c
ģ�����ƣ�ʱ��оƬ��������
���ܸ�Ҫ��ʱ��оƬ��������

ȡ���汾��0.0.1
�޸���	��������
������ڣ�2005.04.06
����˵����create

******************************************************************************/
#include "all.h"

#ifdef I2C_USE_SEM
extern OS_EVENT			*i2c_sem;
#endif

#ifdef RTC_USE_SEM
OS_EVENT *g_rtc_sem;           //����ʱ��оƬ�������ź���
#endif



//�·����ݱ�
INT8U const table_week[12] = { 0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5 }; //���������ݱ�
INT8U const mon_table[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };       //ƽ����·����ڱ�



/*****************************************************************
����ԭ�ͣ�rtc_init
����������ʱ���ʼ��
����������
�������ƣ�	����/�����	����		����

��  ��  ֵ����
��      �ߣ�������
��      �ڣ�2005-04-12
�޸���ʷ��
����		�޸���		�޸�����
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
//	if ( BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5 )      //��ָ���ĺ󱸼Ĵ����ж�������:��������д���ָ�����ݲ����
//	{
//		//RTC�ϵ���һ������
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);    //ʹ��PWR��BKP����ʱ��
//		PWR_BackupAccessCmd(ENABLE);    //ʹ�ܺ󱸼Ĵ�������
//		BKP_DeInit();   //��λ��������
//		RCC_LSEConfig(RCC_LSE_ON);  //�����ⲿ���پ���(LSE),ʹ��������پ���
//		while ( RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET ) //���ָ����RCC��־λ�������,�ȴ����پ������
//		{
//			k++;
//			SleepMs(10);
//			if ( k >= 250 )
//				return; //��ʼ��ʱ��ʧ��,����������
//		}
//		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);     //����RTCʱ��(RTCCLK),ѡ��LSE��ΪRTCʱ��
//		RCC_RTCCLKCmd(ENABLE);  //ʹ��RTCʱ��
//		RTC_WaitForLastTask();  //�ȴ����һ�ζ�RTC�Ĵ�����д�������
//		RTC_WaitForSynchro();       //�ȴ�RTC�Ĵ���ͬ��
//		RTC_ITConfig(RTC_IT_SEC, ENABLE);       //ʹ��RTC���ж�
//		RTC_WaitForLastTask();  //�ȴ����һ�ζ�RTC�Ĵ�����д�������
//		RTC_EnterConfigMode(); /// ��������
//		RTC_SetPrescaler(32767); //����RTCԤ��Ƶ��ֵ
//		RTC_WaitForLastTask();  //�ȴ����һ�ζ�RTC�Ĵ�����д�������
//		RTC_Set(1970, 1, 1, 0, 0, 0);  //����ʱ��
//		RTC_ExitConfigMode(); //�˳�����ģʽ
//		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);   //��ָ���ĺ󱸼Ĵ�����д���û���������
//
//		return;
//	}
//
//	//���ǵ�һ���ϵ�
//	RTC_WaitForSynchro();   //�ȴ����һ�ζ�RTC�Ĵ�����д�������
//	RTC_ITConfig(RTC_IT_SEC, ENABLE);   //ʹ��RTC���ж�
//	RTC_WaitForLastTask();  //�ȴ����һ�ζ�RTC�Ĵ�����д�������


	INT8U i;
	INT8U m;

#ifdef RTC_USE_SEM
	g_rtc_sem = OSSemCreate(1);
#endif

	for ( i=0; i<10; i++ )
	{
		m = write_8025(CTRL1_REG, 0x20);		//24Сʱģʽ
		if ( m != ok )
			continue;

		m = write_8025(CTRL2_REG, 0x20);		//2.1v��ѹ���
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
//����ԭ�ͣ�Get_Time
//������������ȡʱ�䣬��ȡ���Σ�Ϊ�˷�ֹż��һ�β��ɹ�
//����������
//�������ƣ�	����/�����	����		����
//time		���   		_TIME*		7���ֽڳ������ݴ�ͷ��β˳�������� �� �� �� ʱ �� ��
//��  ��  ֵ��ok(0)-�ɹ�
//				notok(0xFF)��ʧ��
//��      �ߣ�������
//��      �ڣ�2005-04-12
//�޸���ʷ��
//����		�޸���		�޸�����
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
//����ԭ�ͣ�Modify_Time
//�����������޸�ʱ�䣬�޸����Σ�Ϊ�˷�ֹż��һ�β��ɹ�
//����������
//�������ƣ�	����/�����	����		����
//*time		����		_TIME*		7���ֽڳ������ݴ�ͷ��β˳�������� �� �� �� ʱ �� ��
//��  ��  ֵ��ok(0)-�ɹ�
//				notok(0xFF)��ʧ��
//��      �ߣ�������
//��      �ڣ�2005-04-12
//�޸���ʷ��
//����		�޸���		�޸�����
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
//				bcdhex(time->month), bcdhex(time->day), bcdhex(time->hour), bcdhex(time->minute), bcdhex(time->second));  //����ʱ��
//
//#ifdef RTC_USE_SEM
//	(void)OSSemPost(g_rtc_sem);
//#endif
//
//	return i;
//}
//
/*****************************************************************
����ԭ�ͣ�CheckTimeFormat
�������������ʱ���ʽ�Ƿ�Ϸ�
����������
�������ƣ�	����/�����	����		����
time		����  			const _TIME *	7���ֽڳ������ݴ�ͷ��β˳�������� �� �� �� ʱ �� ��

��  ��  ֵ��ok(0)-�ɹ� 
				notok(0xFF)��ʧ��
��      �ߣ�������
��      �ڣ�2003-05-12
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
INT8U CheckTimeFormat(const _TIME *time)
{
	INT8U temp;

	ToggleWD();

	temp = time->month;
	if ( temp >= 0x0a )       //�·�Ӧ��1��12֮��
	{
		if ( temp >= 0x10 && temp <= 0x12 )
		;
		else
			return notok;
	}

	temp = time->day;
	if ( temp > 0x31 )                   //��Ӧ��1-31֮��
		return notok;
	temp = temp & 0x0F;
	if ( temp > 0x09 )
		return notok;

	temp = time->hour;
	if ( temp > 0x23 )                                   //СʱӦ��0-23֮��
		return notok;
	temp = temp & 0x0F;
	if ( temp > 0x09 )
		return notok;

	temp = time->minute;
	if ( temp > 0x59 )                                   //����Ӧ��0-59֮��
		return notok;
	temp = temp & 0x0F;
	if ( temp > 0x09 )
		return notok;

	temp = time->second;
	if ( temp > 0x59 )                                    //��Ӧ��0-59֮��
		return notok;
	temp = temp & 0x0F;
	if ( temp > 0x09 )
		return notok;

	return ok;
}


//�ж��Ƿ������꺯��
//�·�   1  2  3  4  5  6  7  8  9  10 11 12
//����   31 29 31 30 31 30 31 31 30 31 30 31
//������ 31 28 31 30 31 30 31 31 30 31 30 31
//����:���
//���:������ǲ�������.1,��.0,����
/*****************************************************************
����ԭ�ͣ�Is_Leap_Year
�����������ж��Ƿ�������
����������
�������ƣ�	����/�����	����		����
year		����  		INT16U		���

��  ��  ֵ��������ǲ�������.1,��. 0,����
��      �ߣ�������
��      �ڣ�2003-05-12
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
INT8U Is_Leap_Year(INT16U year)
{
	if ( year % 4 == 0 ) //�����ܱ�4����
	{
		if ( year % 100 == 0 )
		{
			if ( year % 400 == 0 )
				return 1; //�����00��β,��Ҫ�ܱ�400����
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
//����ԭ�ͣ�RTC_Set
//��������������ʱ��,�������ʱ��ת��Ϊ����,��1970��1��1��Ϊ��׼,1970~2099��Ϊ�Ϸ����
//����������
//�������ƣ�	����/�����	����		����
//syear		����  		INT16U		���hex
//smon		����  		INT8U		��hex
//sday		����  		INT8U		��hex
//hour		����  		INT8U		ʱhex
//min			����  		INT8U		��hex
//sec			����  		INT8U		��hex
//
//��  ��  ֵ��ok(0x00),�ɹ�;����:�������.
//��      �ߣ�������
//��      �ڣ�2003-05-12
//�޸���ʷ��
//����		�޸���		�޸�����
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
//	for ( t = 1970; t < syear; t++ ) //��������ݵ��������
//	{
//		if ( Is_Leap_Year(t) )
//			seccount += 31622400; //�����������
//		else
//			seccount += 31536000;              //ƽ���������
//	}
//	smon -= 1;
//	for ( t = 0; t < smon; t++ )    //��ǰ���·ݵ����������
//	{
//		seccount += (INT32U)mon_table[t] * 86400; //�·����������
//		if ( Is_Leap_Year(syear) && t == 1 )
//			seccount += 86400; //����2�·�����һ���������
//	}
//	seccount += (INT32U)(sday - 1) * 86400; //��ǰ�����ڵ����������
//	seccount += (INT32U)hour * 3600; //Сʱ������
//	seccount += (INT32U)min * 60;   //����������
//	seccount += sec; //�������Ӽ���ȥ
//
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);    //ʹ��PWR��BKP����ʱ��
//	PWR_BackupAccessCmd(ENABLE);    //ʹ��RTC�ͺ󱸼Ĵ�������
//	RTC_SetCounter(seccount);   //����RTC��������ֵ
//
//	RTC_WaitForLastTask();  //�ȴ����һ�ζ�RTC�Ĵ�����д�������
//	return ok;
//}
//
///*****************************************************************
//����ԭ�ͣ�RTC_Get
//�����������õ���ǰ��ʱ��
//����������
//�������ƣ�	����/�����	����		����
//calendar	���  		CALENDART*	ȡ�ص�ʱ��
//
//��  ��  ֵ��ok(0x00),�ɹ�;����:�������.
//��      �ߣ�������
//��      �ڣ�2003-05-12
//�޸���ʷ��
//����		�޸���		�޸�����
//------		---------	-------------
//*****************************************************************/
//INT8U RTC_Get(CALENDART *calendar)
//{
//	INT32U timecount = 0;
//	INT32U temp = 0;
//	INT16U temp1 = 0;
//
//	timecount = RTC->CNTH; //�õ��������е�ֵ(������)
//	timecount <<= 16;
//	timecount += RTC->CNTL;
//
//	temp = timecount / 86400;   //�õ�����(��������Ӧ��)
//
//	temp1 = 1970; //��1970�꿪ʼ
//	while ( temp >= 365 )
//	{
//		if ( Is_Leap_Year(temp1) ) //������
//		{
//			if ( temp >= 366 )
//			{
//				temp -= 366; //�����������
//			}
//			else
//			{
//				temp1++;
//				break;
//			}
//		}
//		else
//		{
//			temp -= 365;   //ƽ��
//		}
//		temp1++;
//	}
//	calendar->year = temp1; //�õ����
//
//	temp1 = 0;
//	while ( temp >= 28 ) //������һ����
//	{
//		if ( Is_Leap_Year(calendar->year) && temp1 == 1 ) //�����ǲ�������/2�·�
//		{
//			if ( temp >= 29 )
//				temp -= 29; //�����������
//			else
//				break;
//		}
//		else
//		{
//			if ( temp >= mon_table[temp1] )
//				temp -= mon_table[temp1]; //ƽ��
//			else
//				break;
//		}
//		temp1++;
//	}
//	calendar->month = temp1 + 1;   //�õ��·�
//	calendar->day = temp + 1;     //�õ�����
//
//	temp = timecount % 86400;           //�õ�������
//	calendar->hour = temp / 3600;        //Сʱ
//	calendar->min = (temp % 3600) / 60;    //����
//	calendar->sec = (temp % 3600) % 60;    //����
//	calendar->week = RTC_Get_Week(calendar->year, calendar->month, calendar->day); //��ȡ����
//	return ok;
//}
//
///*****************************************************************
//����ԭ�ͣ�RTC_Get_Week
//����������������������ڼ�,���빫�����ڵõ�����(ֻ����1901-2099��)
//����������
//�������ƣ�	����/�����	����		����
//syear		����  		INT16U		���hex
//smon		����  		INT8U		��hex
//sday		����  		INT8U		��hex
//
//��  ��  ֵ�����ں�
//��      �ߣ�������
//��      �ڣ�2003-05-12
//�޸���ʷ��
//����		�޸���		�޸�����
//------		---------	-------------
//*****************************************************************/
//INT8U RTC_Get_Week(INT16U year, INT8U month, INT8U day)
//{
//	INT16U temp2;
//	INT8U yearH, yearL;
//
//	yearH = year / 100;	yearL = year % 100;
//	// ���Ϊ21����,�������100
//	if ( yearH > 19 )
//		yearL += 100;
//	// ����������ֻ��1900��֮���
//	temp2 = yearL + yearL / 4;
//	temp2 = temp2 % 7;
//	temp2 = temp2 + day + table_week[month - 1];
//	if ( yearL % 4 == 0 && month < 3 )
//		temp2--;
//
//	return ( temp2 % 7 );
//}

/*****************************************************************
����ԭ�ͣ�time_read
������������ȡʱ��
����������
�������ƣ�	����/�����	����		����
time		���   		_TIME*		7���ֽڳ������ݴ�ͷ��β˳�������� �� �� �� ʱ �� ��
��  ��  ֵ��ok(0)-�ɹ� 
				notok(0xFF)��ʧ��
��      �ߣ�������
��      �ڣ�2005-04-12
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
INT8U time_read(_TIME *time)
{
	return ( Get_Time(time) );
}

/*****************************************************************
����ԭ�ͣ�time_write
�����������޸�ʱ��
����������
�������ƣ�	����/�����	����		����
*time		����		_TIME*		7���ֽڳ������ݴ�ͷ��β˳�������� �� �� �� ʱ �� ��
��  ��  ֵ��ok(0)-�ɹ� 
				notok(0xFF)��ʧ��
��      �ߣ�������
��      �ڣ�2005-04-12
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
INT8U time_write(_TIME const * const time)
{
	return ( Modify_Time(time) );
}



/*****************************************************************
����ԭ�ͣ�write_8025
������������ĳ����ֵд��rx��8025�ļĴ�����
����������
�������ƣ�	����/�����	����		����
	which_reg	����				INT8U		8025 �Ĵ�����ַ
	value		����				INT8U		Ҫд���ֵ

��  ��  ֵ��ok(0)-�ɹ� 
				notok(0xFF)��ʧ��
��      ��	��������
��      �ڣ�2004-12-1
�޸���ʷ��
����		�޸���		�޸�����
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

	i = i2c_start();					//���I2c_Start����������10��
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

	i = i2c_send(RX8025_WRITE);	//����Ӳ����ַ�Լ�д�ź�
	if (i != ok)
	{
		i2c_stop();
//		OS_EXIT_CRITICAL();
#ifdef I2C_USE_SEM
		OSSemPost(i2c_sem);
#endif
		return notok;
	}

//  i = i2c_send((which_reg << 4) & 0xF0);  //���Ͳ����Ĵ�����ַ("0")
	i = i2c_send(which_reg);	//���Ͳ����Ĵ�����ַ("0")
	if (i != ok)
	{
		i2c_stop();
//		OS_EXIT_CRITICAL();
#ifdef I2C_USE_SEM
		OSSemPost(i2c_sem);
#endif
		return notok;
	}

	i = i2c_send(value);	//����Ӳ����ַ�Լ�д�ź�
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
// ����Ϊ����Ҫ��д����
/////////////////////////////////////////////////////////


/*****************************************************************
����ԭ�ͣ�Get_Time
������������ȡʱ�䣬��ȡ���Σ�Ϊ�˷�ֹż��һ�β��ɹ�
����������
�������ƣ�	����/�����	����		����
*time_data	���   _TIME	7���ֽڳ������ݴ�ͷ��β˳�������� �� �� �� ʱ �� ��
��  ��  ֵ��ok(0)-�ɹ� 
				notok(0xFF)��ʧ��
��      ��	��������
��      �ڣ�2005-04-12
�޸���ʷ��
����		�޸���		�޸�����
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
����ԭ�ͣ�Get_Time_Lowlvl
������������ȡrx��8025	�ڵ�ʱ��
����������
�������ƣ�	����/�����	����		����
*time_data	���   _TIME	7���ֽڳ������ݴ�β��ͷ˳�������� �� �� �� ʱ �� ��
��  ��  ֵ��ok(0)-�ɹ� 
				notok(0xFF)��ʧ��
��      ��	��������
��      �ڣ�2005-04-12
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
INT8U Get_Time_Lowlvl(_TIME *time_data)
{
	INT8U i = 0;
	INT8U start_reg = 0;		//�ӵ�0���Ĵ�����ʼ��ȡ
	INT8U buf[9];

	ToggleWD();

	i = i2c_send_and_receive(RX8025, start_reg, buf, 7);
	if (i != ok)
	{
		return notok;
	}

	time_data->second = buf[0];				//��
	time_data->minute = buf[1];				//��
	time_data->hour = buf[2];				//Сʱ
	time_data->day = buf[4];				//��
	time_data->month = buf[5];				//��
	time_data->year = buf[6];				//��
	time_data->century = 0x20;				//���Ͷ���Ϊ0x20

	return ok;
}

/*****************************************************************
����ԭ�ͣ�Modify_Time
�����������޸�ʱ�䣬�޸����Σ�Ϊ�˷�ֹż��һ�β��ɹ�
����������
�������ƣ�	����/�����	����		����
*time_data	����	_TIME	7���ֽڳ������ݴ�ͷ��β˳�������� �� �� �� ʱ �� ��
��  ��  ֵ��ok(0)-�ɹ� 
				notok(0xFF)��ʧ��
��      ��	��������
��      �ڣ�2005-04-12
�޸���ʷ��
����		�޸���		�޸�����
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
����ԭ�ͣ�Modify_Time_Lowlvl
�����������޸�rx-8025 �ڵ�ʱ��
����������
�������ƣ�	����/�����	����		����
*time_data	����	_TIME	7���ֽڳ������ݴ�β��ͷ˳����
											���� �� �� �� ʱ �� ��
��  ��  ֵ��ok(0)-�ɹ� 
				notok(0xFF)��ʧ��
��      ��	��������
��      �ڣ�2003-05-12
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
INT8U Modify_Time_Lowlvl(_TIME const * const time_data)			//ָ��ָ������ǲ����޸ĵģ�ͬʱָ��Ҳ�ǲ����޸ĵ�
{
	INT8U i;

	ToggleWD();

	i = write_8025(CTRL1_REG, 0x20);		//24Сʱģʽ
	if ( i != ok )
		return notok;

	i = write_8025(SEC_REG, time_data->second);				//��
	if ( i != ok )
		return notok;

	i = write_8025(MIN_REG, time_data->minute);				//��
	if ( i != ok )
		return notok;

	i = write_8025(HOUR_REG, time_data->hour);				//Сʱ
	if ( i != ok )
		return notok;

	i = write_8025(DAY_REG, time_data->day);					//��
	if ( i != ok )
		return notok;

	i = write_8025(MONTH_REG, time_data->month);				//��
	if ( i != ok )
		return notok;

	i = write_8025(YEAR_REG, time_data->year);				//��
	if ( i != ok )
		return notok;

	return ok;
}



