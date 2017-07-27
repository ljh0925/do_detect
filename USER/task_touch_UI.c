/******************************************************************************
Copyright 2005 大连易达通电子有限公司
All rights riserved.

文件名	：task_touch_UI.c
模块名称：触摸屏程序
功能概要：

取代版本：0.0.1
修改人	：刘及华
完成日期：2016.04.06
升级说明：create

******************************************************************************/
#include 		"all.h"

#define TIME_100MS 10
#define TIME_1000MS 100

volatile uint32  timer_tick_count = 0; //定时器节拍


extern volatile DEV_STAT DevStat;
extern uint8 cmd_buffer[CMD_MAX_SIZE];

//extern volatile float do_real_I;
//extern volatile short temprature;
extern volatile uint8 update_en;
/******************************************************************************
 函数名称：task_touch_UI
 功能描述：触摸屏程序
 参数描述：
 参数名称： 输入/输出？ 类型		描述

 返  回  值：无

 作 	 者：刘及华
 日 	 期：2016-06-18
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void task_touch_UI(void *data)
{
	qsize size = 0;
	uint8 timer_tick_cnt = 0;

	for (;;)
	{
		size = queue_find_cmd(cmd_buffer, CMD_MAX_SIZE); //从缓冲区中获取一条指令
		if ( size > 0 ) //接收到指令
		{
			ProcessMessage((PCTRL_MSG)cmd_buffer, size); //指令处理
		}

		/****************************************************************************************************************
		特别注意
		MCU不要频繁向串口屏发送数据，否则串口屏的内部缓存区会满，从而导致数据丢失(缓冲区大小：标准型8K，基本型4.7K)
		1) 一般情况下，控制MCU向串口屏发送数据的周期大于100ms，就可以避免数据丢失的问题；
		2) 如果仍然有数据丢失的问题，请判断串口屏的BUSY引脚，为高时不能发送数据给串口屏。
		******************************************************************************************************************/
		
		//TODO: 添加用户代码
		timer_tick_cnt = timer_tick_cnt + 1;
		OSTimeDlyHMSM(0, 0, 0, 10);	
		//数据有更新时，每1秒刷新一次
		if(update_en&&timer_tick_cnt >= 10)
		{
			update_en = 0;
			timer_tick_cnt = 0;
			
			UpdateUI();
		}
		
		
	}

}

