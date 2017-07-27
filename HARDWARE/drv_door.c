/******************************************************************************
Copyright 2005 大连易达通电子有限公司
All rights riserved.

文件名	：drv_buzzer.c
模块名称：电磁锁驱动程序
功能概要：电磁锁驱动程序

取代版本：0.0.1
修改人	：刘及华
完成日期：2015.04.06
升级说明：create

******************************************************************************/
#include "all.h"




/******************************************************************************
函数名称：door_init
功能描述：控制点初始化
参数描述：

返  回  值	：无
作      者	：刘及华
日      期：2015-04-08
修改历史：
日期		修改人		修改描述
------		---------	---------
******************************************************************************/
void door_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //使能PG端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				 //buzzer-->PA.12 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		 //IO口速度为2MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.12
	GPIO_SetBits(GPIOC, GPIO_Pin_6);						 //PA.12 buzzer输出高

	set_door(ON);
}

/******************************************************************************
 函数名称：set_door
 功能描述：控制门禁
 参数描述：
 参数名称：	输入/输出？	类型		描述
 bell_switch	输入		INT8U	ON-开门，OFF-关门
				
 返  回  值：无
				   
 作      者	：刘及华
 日      期：2015-09-02
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void set_door(INT8U door_switch)
{
	ToggleWD();

	if ( door_switch == ON )        //door  open
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_6);			//PA.12 输出低
	}
	else if ( door_switch == OFF )  //door close
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_6);		//PA.12 输出高
	}

}





