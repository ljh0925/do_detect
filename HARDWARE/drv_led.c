/******************************************************************************
Copyright 2005 大连易达通电子有限公司
All rights riserved.

文件名	：drv_led.c
模块名称：led驱动程序
功能概要：led驱动程序

取代版本：0.0.1
修改人	：刘及华
完成日期：2005.04.06
升级说明：create

******************************************************************************/
#include "all.h"



/******************************************************************************
函数名称：led_init
功能描述：led显示初始化
参数描述：

返  回  值：无
作      者：刘及华
日      期：2005-04-08
修改历史：
日期		修改人		修改描述
------		---------	---------
******************************************************************************/
void led_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);    //使能PB端口时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                //LED_RED-->PB.9 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;         //IO口速度为2MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);                   //根据设定参数初始化GPIOB.9
    GPIO_SetBits(GPIOB, GPIO_Pin_9);                         //PB.9 GREEN_RED输出高

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;                //LED_GREEN-->PB.8 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;         //IO口速度为2MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);                   //根据设定参数初始化GPIOB.8
    GPIO_SetBits(GPIOB, GPIO_Pin_8);                         //PB.8 RED_LED输出高

    SetLight(NEITHER_LIGHT);
}

/******************************************************************************
函数名称  SetLight
功能描述：对红绿指示灯进行控制
参数描述：
参数名称：	输入/输出	类型		描述
light_state	 输入		INT8U  用4个宏定义GREEN_LIGHT、RED_LIGHT、BOTH_LIGHT、NEITHE_LIGHT来表示

返  回  值：无
                
作      者	：刘及华
日      期：2003-09-23
修改历史：
日期		修改人		修改描述
------		---------	---------
******************************************************************************/
void SetLight(INT8U light_state)
{
    if ( light_state == GREEN_LIGHT )
    {
		GPIO_ResetBits(GPIOB, GPIO_Pin_8);						 	//PB.9 GREEN输出低
		GPIO_SetBits(GPIOB ,GPIO_Pin_9);						 	//PB.8 RED输出高

    }
    else if ( light_state == RED_LIGHT )
    {
		GPIO_SetBits(GPIOB, GPIO_Pin_8);						 	//PB.9 GREEN输出高
		GPIO_ResetBits(GPIOB, GPIO_Pin_9);						 	//PB.8 RED输出低
   
    }
    else if ( light_state == BOTH_LIGHT )
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_9);                          //PB.9 GREEN输出低
        GPIO_ResetBits(GPIOB, GPIO_Pin_8);                          //PB.8 RED输出低
    }
    else
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_9);                            //PB.9 GREEN输出高
        GPIO_SetBits(GPIOB, GPIO_Pin_8);                            //PB.8 RED输出高
    }
}

/******************************************************************************
函数名称  set_led
功能描述：对红绿指示灯进行控制
参数描述：
参数名称：	输入/输出	类型		描述 
which		输入		INT8U		哪个灯GREEN或者RED 
on_off		输入		INT8U		ON=亮，OFF=灭

返  回  值：无
                
作      者	：刘及华
日      期：2014-10-10
修改历史：
日期		修改人		修改描述
------		---------	---------
******************************************************************************/
void set_led(INT8U which, INT8U on_off)
{
    if ( which == GREEN )
    {
        if ( on_off == ON )
            GPIO_ResetBits(GPIOB, GPIO_Pin_8);                          //PB.8 GREEN输出低
        else
            GPIO_SetBits(GPIOB, GPIO_Pin_8);                            //PB.8 GREEN输出高
    }
    else if ( which == RED )
    {
        if ( on_off == ON )
            GPIO_ResetBits(GPIOB, GPIO_Pin_9);                          //PB.9 RED输出低
        else
            GPIO_SetBits(GPIOB, GPIO_Pin_9);                            //PB.9 RED输出高
    }
    else
    {}

}

/******************************************************************************
函数名称  work_led
功能描述：工作指示灯
参数描述：
参数名称：	

返  回  值：无
                
作      者	：刘及华
日      期：2014-10-10
修改历史：
日期		修改人		修改描述
------		---------	---------
******************************************************************************/
void work_led(void)
{
    set_led(GREEN, ON);
    SleepMs(1000);
    set_led(GREEN, OFF);
    SleepMs(1000);
}


