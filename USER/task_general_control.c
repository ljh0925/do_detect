/******************************************************************************
Copyright 2005 大连易达通电子有限公司
All rights riserved.

文件名	：task_general_control.c
模块名称：总控制任务程序
功能概要：总控制任务程序

取代版本：0.0.1
修改人	：刘及华
完成日期：2005.04.06
升级说明：create

******************************************************************************/
#include 		"all.h"


extern volatile DEV_STAT DevStat;
extern const INT8U VERSION[];



volatile short temprature = 0;
extern volatile uint8 update_en;
volatile float do_real_I = 0.0;

/******************************************************************************
 函数名称：task_general_ctrl
 功能描述：总控制任务
 参数描述：
 参数名称： 输入/输出？ 类型		描述
				
 返  回  值：无
				   
 作 	 者 ：刘及华
 日 	 期：2005-04-07
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void task_general_ctrl(void *data)
{
	INT8U t = 0;
	INT8U j = 0;
	INT8U f_light = 1;
	INT16S adcx = 0;

	data = data;                    //avoid warning

	toggle_wd();

	for (;;)
	{
		toggle_wd();

		j++;
		if ( j >= 2 )
		{
			f_light = !f_light;
			j = 0;
			set_led(RED, f_light);
		}

		update_en = 1;
		t = 0;
		temprature = DS18B20_Get_Temp();
		if ( temprature < 0 )
		{
			temprature = 0;                        //负数转为0，最低显示到零度
		}
		else
			;

		adcx = Get_Adc_Average(5);

		//temp = (float)adcx * (5.0 / 4096);       //电压值
		do_real_I = adcx;                          //电流的单位是纳安 nA
		Oxygen(); 

		OSTimeDlyHMSM(0, 0, 0, 20);                //每20ms采集一次溶氧数据
	}

}

///******************************************************************************
// 函数名称：SaveCardInTime
// 功能描述：入卡时保存时间
// 参数描述：
// 参数名称：	输入/输出？	类型		描述
// card_info	输入		CARD_INFO *		准备要保存时间的变量的指针
//
// 返  回  值：ok(0)-成功
//				 notok(0xFF)-失败
//
// 作      者	：刘及华
// 日      期：2004-09-02
// 修改历史：
//		日期		修改人		修改描述
//		------		---------	-------------
//******************************************************************************/
INT8U SaveCardInTime(CARD_INFO *card_info)
{
	//	INT8U i = 0;
	//	BUS_TIME time;

	ToggleWD();

	//	i = Get_Time(&time);


	//	i = CheckTimeFormat(&time);                             //检测时间格式
	//	if ( i != ok )
	//		return notok;

	//	memcpy((void *)&card_info->card_in_time, (void *)&time, sizeof(BUS_TIME));

	return ( ok );
}

///******************************************************************************
// 函数名称：CheckPeriodOfValidity
// 功能描述：有效期判断
// 参数描述：
// 参数名称：	输入/输出？	类型		描述
// start			输入		INT32U		启用日期
// current		输入		INT32U		当前日期
// end			输入		INT32U		有效截止日期
//
// 返  回  值：ok(0)-成功
//				 notok(0xFF)-失败(过期)
//
// 作      者	：刘及华
// 日      期：2004-09-20
// 修改历史：
//		日期		修改人		修改描述
//		------		---------	-------------
//******************************************************************************/
INT8U CheckPeriodOfValidity(INT32U current, INT32U end)
{
	if ( DevStat.param_flag == 0 )  //拨码开关最高位，要是高就开始有效日期判断
	{
		return ( ok );
	}
	else
	{
		if ( (current <= end) )
			return ( ok );
		else
			return ( notok );
	}
}


