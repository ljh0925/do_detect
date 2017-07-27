/******************************************************************************
Copyright 2005 大连易达通电子有限公司
All rights riserved.

文件名	：drv_switch.c
模块名称：switch驱动程序
功能概要：switch驱动程序

取代版本：0.0.1
修改人	：刘及华
完成日期：2005.04.06
升级说明：create

******************************************************************************/
#include "all.h"


extern volatile DEV_STAT DevStat;
/******************************************************************************
函数名称：switch_init
功能描述：led显示初始化
参数描述：

返  回  值：无
作      者：刘及华
日      期：2005-04-08
修改历史：
日期		修改人		修改描述
------		---------	---------
******************************************************************************/
void switch_init(void)
{
//	GPIO_InitTypeDef  GPIO_InitStructure;
//
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //使能PB端口时钟
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				 //LED_RED-->PB.8 端口配置
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //带上拉的输入
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		 //IO口速度为2MHz
//	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.8
//	GPIO_SetBits(GPIOB, GPIO_Pin_10);					 	//PB.8 GREEN_RED输出高
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				 //LED_GREEN-->PB.9 端口配置
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //带上拉的输入
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		 //IO口速度为2MHz
//	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.9
//	GPIO_SetBits(GPIOB, GPIO_Pin_11);					 	//PB.9 RED_LED输出高
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				 //LED_RED-->PB.8 端口配置
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //带上拉的输入
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		 //IO口速度为2MHz
//	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.8
//	GPIO_SetBits(GPIOB, GPIO_Pin_12);					 	//PB.8 GREEN_RED输出高
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				 //LED_GREEN-->PB.9 端口配置
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //带上拉的输入
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		 //IO口速度为2MHz
//	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.9
//	GPIO_SetBits(GPIOB, GPIO_Pin_13);					 	//PB.9 RED_LED输出高
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;				 //LED_RED-->PB.8 端口配置
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //带上拉的输入
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		 //IO口速度为2MHz
//	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.8
//	GPIO_SetBits(GPIOB, GPIO_Pin_14);					 	//PB.8 GREEN_RED输出高
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				 //LED_GREEN-->PB.9 端口配置
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //带上拉的输入
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		 //IO口速度为2MHz
//	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.9
//	GPIO_SetBits(GPIOB, GPIO_Pin_15);					 	//PB.9 RED_LED输出高
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //LED_RED-->PB.8 端口配置
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //带上拉的输入
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		 //IO口速度为2MHz
//	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.8
//	GPIO_SetBits(GPIOB, GPIO_Pin_8);					 	//PB.8 GREEN_RED输出高
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				 //LED_GREEN-->PB.9 端口配置
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //带上拉的输入
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		 //IO口速度为2MHz
//	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.9
//	GPIO_SetBits(GPIOB, GPIO_Pin_9);					 	//PB.9 RED_LED输出高
//
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //OUT_KEY-->PB.5 端口配置
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		     //带上拉的输入
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		 //IO口速度为2MHz
//	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
//	GPIO_SetBits(GPIOB, GPIO_Pin_5);					 	 //PB.5
	//SetLight(NEITHER_LIGHT);
}

/******************************************************************************
函数名称: ReadSwitch
功能描述：读取拨码开关的位置
参数描述：
参数名称：	输入/输出	类型		描述


返  回  值：无
				
作      者	：刘及华
日      期：2015-09-23
修改历史：
日期		修改人		修改描述
------		---------	---------
******************************************************************************/
INT8U ReadSwitch(void)
{
//	INT16U buf;
//    char tmp;
//
//	buf = ~GPIO_ReadInputData(GPIOB);
//
//    if((buf & 0x0100) == 0)
//    {
//		DevStat.param_flag = 0;
//    }
//    else
//    {
//        DevStat.param_flag = 1;
//    }
//    buf &= 0xfe00;
//
//    tmp = (INT8U)((buf >> 8));
//
//	return (INT8U)(shift_char(tmp));
	return 0;
}

unsigned char shift_char(unsigned char data) 
{ 
//    unsigned char i;
//    unsigned char tmp=0x00;
//
//    for(i=0;i<8;i++)
//    {
//        tmp=((data>>i)&0x01)|tmp;
//        if(i<7)
//            tmp=tmp<<1;
//    }
//
//    return tmp;
     return 0;
}




