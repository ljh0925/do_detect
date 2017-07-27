/******************************************************************************
Copyright 2005 大连易达通电子有限公司
All rights riserved.

文件名	：task_irda_comm.c
模块名称：红外通讯任务
功能概要：红外通讯任务

取代版本：0.0.1
修改人	：刘及华
完成日期：2005.04.06
升级说明：create

******************************************************************************/
#include 		"all.h"


//OS_STK  TASK2_STACK[TASK2_STACK_SIZE];

extern volatile DEV_STAT DevStat;

extern INT8U Com4SndBuf[UART4_SEND_QUEUE_LENGTH];				//串口发送缓冲区
extern INT8U Com4RcvBuf[UART4_RCV_QUEUE_LENGTH];				//串口接收缓冲区


//INT8U SndBuf[UART3_SEND_QUEUE_LENGTH];

/******************************************************************************
 函数名称：task_irda_comm
 功能描述：红外通讯任务
 参数描述：
 参数名称： 输入/输出？ 类型		描述
				
 返  回  值：无
				   
 作 	 者：刘及华
 日 	 期：2010-06-18
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void task_irda_comm(void * data)
{
	INT16U rcv_len = 0;									//数据域长度

	data = data;		//avoid warning

	ToggleWD();


	for ( ;; )
	{

		(void)com4_gets(Com4RcvBuf, &rcv_len, 0, 5);

		if ( Com4RcvBuf[0] != '@' )			//判断开头
			continue;

		if ( rcv_len < 4 )
			continue;

		if ( (Com4RcvBuf[rcv_len-1] == 0x0D) && (Com4RcvBuf[rcv_len-2] == 0x2A) )		 //判断尾是否已经接收
		{
		}
		else
		{
			continue;
		}

		if ( BCC_Check(Com4RcvBuf, rcv_len) != ok )
		{
			continue;
		}


		switch ( Com4RcvBuf[1] )
		{
			////////////////////////////////////////
			//初始化
			////////////////////////////////////////
			case 'S':				//初始化
				if ( (rcv_len == 16) && Com4RcvBuf[5] == 'I' && Com4RcvBuf[6] == 'n' 
					 && Com4RcvBuf[7] == 'i' && Com4RcvBuf[8] == 't' )
				{
//  				(void)InitImportantParameter();
//  				(void)Rx8025Init();							//时钟初始化
//					init_param();

					memcpy(Com4SndBuf , Com4RcvBuf, rcv_len);
					com4_send_hex(Com4SndBuf , rcv_len);					//收什么，返什么

					Beep(500);
					resetpos();				//复位
				}
				break;

				////////////////////////////////////////
				//更新时间
				////////////////////////////////////////
			case 'T':				//更新时间
				if ( rcv_len == 20 )
				{
					if ( DevStat.allow_upload_data == TRUE )
					{
//  				UploadDisp = UPDATE_TIME_SUCCESS;
						lcddisp(1, 0, "  时间更新成功  ");
					}

					UpdateTime(Com4RcvBuf);

					memcpy(Com4SndBuf , Com4RcvBuf, rcv_len);
					com4_send_hex(Com4SndBuf , rcv_len);					//收什么，返什么
				}
				break;

				////////////////////////////////////////
				//程序下载
				////////////////////////////////////////
			case 'R':				//程序下载
				if ( (rcv_len == 15) && (Com4RcvBuf[5] == 'R') && (Com4RcvBuf[6] == 'E') && (Com4RcvBuf[7] == 'L')
					 && (Com4RcvBuf[8] == 'O') && (Com4RcvBuf[9] == 'A') && (Com4RcvBuf[10] == 'D') )
				{
					memcpy(Com4SndBuf , Com4RcvBuf, rcv_len);
					com4_send_hex(Com4SndBuf , rcv_len);					//收什么，返什么
				}
				break;

			default:	//do nothing
				break;
		}

	}

}

