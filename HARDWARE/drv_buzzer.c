/******************************************************************************
Copyright 2005 大连易达通电子有限公司
All rights riserved.

文件名	：drv_buzzer.c
模块名称：蜂鸣器驱动程序
功能概要：蜂鸣器驱动程序

取代版本：0.0.1
修改人	：刘及华
完成日期：2005.04.06
升级说明：create

******************************************************************************/
#include "all.h"



/******************************************************************************
函数名称：buzzer_init
功能描述：蜂鸣器初始化
参数描述：

返  回  值	：无
作      者	：刘及华
日      期：2005-04-08
修改历史：
日期		修改人		修改描述
------		---------	---------
******************************************************************************/
void buzzer_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能PA端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				 //buzzer-->PA.11 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		 //IO口速度为2MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.11
	GPIO_SetBits(GPIOA, GPIO_Pin_11);						 //PA.11 buzzer输出高

	set_bell(OFF);
}

/******************************************************************************
 函数名称：set_bell
 功能描述：控制蜂鸣器
 参数描述：
 参数名称：	输入/输出？	类型		描述
 bell_switch	输入		INT8U	ON-响，OFF-不响
				
 返  回  值：无
				   
 作      者	：刘及华
 日      期：2004-09-02
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void set_bell(INT8U bell_switch)
{
	ToggleWD();

	if ( bell_switch == ON )  // sound on
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_11);						 //PB.14 buzzer输出低
	}
	else if ( bell_switch == OFF )//sound off
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_11);						 //PB.14 buzzer输出高
	}

}

/******************************************************************************
 函数名称：Beep
 功能描述：控制蜂鸣器
 参数描述：
 参数名称：	输入/输出？	类型		描述
 _ms	输入		INT16U			要鸣叫的时间，单位ms
				
 返  回  值：无
				   
 作      者	：刘及华
 日      期：2004-09-02
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void Beep(INT16U _ms)
{
//  INT16U i = 0;

	ToggleWD();

	set_bell(ON);
	SleepMs2(_ms);
	set_bell(OFF);
}


/******************************************************************************
 函数名称：Beeperr
 功能描述：控制蜂鸣器--错误报警
 参数描述：
 参数名称：	输入/输出？	类型		描述
 _ms	输入		INT16U			要鸣叫的时间，单位ms
				
 返  回  值：无
				   
 作      者	：刘及华
 日      期：2004-09-02
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void Beeperr(void)
{
	ToggleWD();

	set_bell(ON);
	SleepMs2(100);
	set_bell(OFF);
	SleepMs2(100);
	set_bell(ON);
	SleepMs2(100);
	set_bell(OFF);
}



