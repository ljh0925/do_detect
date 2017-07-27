/******************************************************************************
Copyright 2005 大连易达通电子有限公司
All rights riserved.

文件名	：task_com1_gprs.c
模块名称：com1通讯任务 GPRS通讯
功能概要：com1通讯任务 GPRS通讯

取代版本：0.0.1
修改人	：刘及华
完成日期：2005.04.06
升级说明：create

******************************************************************************/
#include 		"all.h"


OS_STK  TASK4_STACK[TASK4_STACK_SIZE];

extern volatile DEV_STAT DevStat;

INT8U Com1SndBuf[UART1_SEND_QUEUE_LENGTH];              //串口发送缓冲区
INT8U Com1RcvBuf[UART1_RCV_QUEUE_LENGTH];               //串口接收缓冲区

extern int UART1RcvBuf[(UART1_RCV_QUEUE_LENGTH + sizeof(int)- 1)/ sizeof(int)];

//INT8U SndBuf[UART3_SEND_QUEUE_LENGTH];
OS_EVENT        *gprs_sem;

typedef struct _GPRS
{
	INT16U record_base_ptr;                 //未上传记录指针起始指针(原p0+2)
	INT16U record_number;                   //未上传的记录条数(原p0+4)

	BUS_TIME last_commu_time;               //上次通讯时间
	INT8U have_driver_rec;                  //是否是司机上下班记录
	INT16U snd_len;                         //发送长度
	INT16U rcv_len;                         //接收长度


	INT8U driver_rec_num_temp;              //司机上下班记录条数 临时
	INT8U driver_rec_num;                   //司机上下班记录条数
	INT8U driver_rec_ptr;                   //司机上下班记录条数，传到那条了
//  INT8U drec[7][18];                      //司机记录 2字节司机编号+8字节车号+7字节时间+1字节上下班标志
//  INT8U drec[7][20];                      //司机记录 4字节司机编号+8字节车号+7字节时间+1字节上下班标志

	INT8U black_name_version[7];            //黑名单版本号
	INT16U black_num;                       //要传输的黑名单条数
	INT16U black_ptr;						//黑名单传输指针


	INT8U drec[20];							//司机记录 4字节司机编号+8字节车号+7字节时间+1字节上下班标志
	INT8U have_drec;						//是否有未上传的drec，是=TRUE 否=FALSE
	INT8U update_time;						//是否准备与服务器同步时间，是=TRUE 否=FALSE
	INT8U driver_is_working;				//上下班标志

}GPRS;

GPRS gprs;

static INT8U g_pack_num = 0;				//包号
static INT8U g_turn_bit = 0;				//交替位
static INT8U g_retx = 0;					//是否为重发

INT8U g_tmp_tbuf[300];
INT8U g_tmp_tbuf2[100];

//debug
char disbuf[100];
//debug end


typedef struct _GPRS_ERR
{
	INT8U comm_break;						//黑名单传输是否中断过 中断过=TRUE 没中断过=FALSE

	INT8U black_name_version[7];            //中断时刻的黑名单版本号
	INT16U black_num;                       //中断时刻的要传输的黑名单条数
	INT16U black_ptr;						//中断时刻的黑名单传输指针

}GPRS_ERR;

GPRS_ERR gprs_err;

/******************************************************************************
 函数名称：TaskCom1Gprs
 功能描述：com1gprs通讯任务
 参数描述：
 参数名称： 输入/输出？ 类型		描述
				
 返  回  值：无
				   
 作 	 者：刘及华
 日 	 期：2010-06-18
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void TaskCom1Gprs(void *data)
{
	INT16U k = 0;
	//  INT8U num;
	INT8U err = 0;
	BUS_TIME ltime;
	BUS_TIME last_time;
	RECORD rec;
//  INT8U bcc = 0;
	INT8U i = 0;
	INT16U j = 0;
	INT32S time_last;
	INT32S time_cur;

	INT16U trans_times = 0;
//  INT16U k = 0;
	INT16U last_package_black_num = 0;
	INT16U comm_retry = 0;
	INT8U black_rec[4];
	INT16U m = 0;
	INT16U crc = 0;
//  INT32U black = 0;

//#define BLACK_NUM_PER_PACK	40
#define BLACK_NUM_PER_PACK	32
//#define BLACK_NUM_PER_PACK	20
//#define REC_NUM_PER_PACK	4			//每包4条记录
#define REC_NUM_PER_PACK	2			//每包2条记录
#define COMM_RETRY_TIMES	1			//通讯时候重试次数

	data = data;

	//  OSTaskSuspend(TASK4_PRIO);				//挂起task_com1_shell

	UART1_SetBPS(9600, NONE);

	gprs.driver_rec_num_temp = 0;
	gprs.driver_rec_num = 0;
	gprs.driver_rec_ptr = 0;
	gprs_err.comm_break = FALSE;


	g_pack_num = 0;				//包号
	g_turn_bit = 0;				//交替位
	g_retx = 0;					//是否为重发

	//  com1_send_hex((void *)"12345", 5);
//  goto update_blacklist;
	//debug

//  memcpy((void *)g_tmp_tbuf, "\x04\x03\x13", 3);
//  g_pack_num++;
//  g_tmp_tbuf[3] = g_pack_num;
//
//  g_tmp_tbuf[4] = (INT8U)((k * BLACK_NUM_PER_PACK) / 0x100);
//  g_tmp_tbuf[5] = (INT8U)((k * BLACK_NUM_PER_PACK) % 0x100);
//  g_tmp_tbuf[6] = (INT8U)(BLACK_NUM_PER_PACK / 0x100);
//  g_tmp_tbuf[7] = (INT8U)(BLACK_NUM_PER_PACK % 0x100);
//
//  g_retx = FALSE;
//
//  gprs.snd_len = 4 + 4;
//  form_comm_package(g_tmp_tbuf, &gprs.snd_len);

//  UART1_SetBPS(57600, NONE);

//    sprintf((void *)Com1SndBuf, "black name version: %02X%02X%02X%02X%02X%02X%02X\n\r",
//            DevStat.black_name_version[0],
//            DevStat.black_name_version[1],
//            DevStat.black_name_version[2],
//            DevStat.black_name_version[3],
//            DevStat.black_name_version[4],
//            DevStat.black_name_version[5],
//            DevStat.black_name_version[6] );
//    com1_send_hex(Com1SndBuf, strlen((char *)Com1SndBuf));
//
//    sprintf((void *)Com1SndBuf, "black updated time: %02X%02X%02X%02X%02X%02X%02X\n\r",
//            DevStat.update_black_time.century,
//            DevStat.update_black_time.year,
//            DevStat.update_black_time.month,
//            DevStat.update_black_time.day,
//            DevStat.update_black_time.hour,
//            DevStat.update_black_time.minute,
//            DevStat.update_black_time.second );
//    com1_send_hex(Com1SndBuf, strlen((char *)Com1SndBuf));
//
//    sprintf( (void *)Com1SndBuf, "blacklist amount=%d\n\r", DevStat.black_name_number );
//    com1_send_hex(Com1SndBuf, strlen((char *)Com1SndBuf));
//
////  black = 1;
////  sprintf((void *)Com1SndBuf, "%08X ", black);
////  com1_send_hex(Com1SndBuf, strlen((char *)Com1SndBuf));
////
////  black = 0x100;
////  sprintf((void *)Com1SndBuf, "%08X ", black);
////  com1_send_hex(Com1SndBuf, strlen((char *)Com1SndBuf));
////
////  black = 0x10000;
////  sprintf((void *)Com1SndBuf, "%08X ", black);
////  com1_send_hex(Com1SndBuf, strlen((char *)Com1SndBuf));
//
//    for(k=0; k<DevStat.black_name_number; k++)
//    {
//        GetBlackName(&black, k);
//        sprintf((void *)Com1SndBuf, "%08X", black);
//        com1_send_hex(Com1SndBuf, strlen((char *)Com1SndBuf));
//        SleepMs2(500);
//    }

	//debug end






	for (;;)
	{
com1_start:
		//  	OSTimeDlyHMSM(0, 0, 10, 0);
		OSTimeDlyHMSM(0, 0, 10, 0);

		////////////////////////////////////////////////////
		//有司机记录先传司机记录
		////////////////////////////////////////////////////
		OSSemPend(gprs_sem, 0, &err);
		gprs.have_drec = DevStat.have_drec;
		memcpy((void *)&gprs.drec[0], (void *)&DevStat.drec[0], sizeof(DevStat.drec));
		(void)OSSemPost(gprs_sem);

		if ( gprs.have_drec == TRUE )
		{
			memcpy((void *)g_tmp_tbuf, "\x04\x02\x00\x00", 4);
			memcpy((void *)&g_tmp_tbuf[4], (void *)&gprs.drec[0], 20);
			g_retx = FALSE;
//  		gprs.snd_len = 4 + 20;
//  		form_comm_package(g_tmp_tbuf, &gprs.snd_len);


			//开始通讯
			for ( j = 0; j < COMM_RETRY_TIMES; j++ )
			{
				gprs.snd_len = 4 + 20;
				form_comm_package(g_tmp_tbuf, &gprs.snd_len);

				CleanCom1Buf();
				com1_send_hex(Com1SndBuf, gprs.snd_len);
				i = com1_gets(Com1RcvBuf, &gprs.rcv_len, 3000, 10);
				if ( i == ok )
				{
					break;
				}

				g_retx = TRUE;
			}
			if ( j >= COMM_RETRY_TIMES )
			{
				continue;
			}

//  		i = com1_gets(Com1RcvBuf, &gprs.rcv_len, 30000, 10);
//  		if ( i == ok && Com1RcvBuf[0] == 0x02 && Com1RcvBuf[2] == 0x00 )
//  		{
//  			//do nothing
//  		}
//  		else
//  		{
//  			continue;
//  		}
//
//  		Com1SndBuf[0] = 0x06;
//  		Com1SndBuf[1] = Com1RcvBuf[1];
//  		com1_send_hex(Com1SndBuf, 2);


			OSSemPend(gprs_sem, 0, &err);
			DevStat.have_drec = FALSE;
			(void)OSSemPost(gprs_sem);

			continue;
		}



		////////////////////////////////////////////////////
		//与服务器同步时间
		////////////////////////////////////////////////////
		OSSemPend(gprs_sem, 0, &err);
		gprs.update_time = DevStat.update_time;
		(void)OSSemPost(gprs_sem);

		if ( gprs.update_time == TRUE )
		{

			memcpy((void *)g_tmp_tbuf, "\x04\x03\x14", 3);
			g_pack_num++;
			g_tmp_tbuf[3] = g_pack_num;

			g_retx = FALSE;


			//开始通讯
			for ( j = 0; j < COMM_RETRY_TIMES; j++ )
			{
				gprs.snd_len = 4;
				form_comm_package(g_tmp_tbuf, &gprs.snd_len);

				CleanCom1Buf();
				com1_send_hex(Com1SndBuf, gprs.snd_len);
				i = com1_gets(Com1RcvBuf, &gprs.rcv_len, 1000, 10);
				if ( i == ok )
				{
					break;
				}

				g_retx = TRUE;
			}
			if ( j >= COMM_RETRY_TIMES )
			{
				continue;
			}

			i = com1_gets(Com1RcvBuf, &gprs.rcv_len, 10000, 10);
			if ( i == ok && Com1RcvBuf[0] == 0x02 )
			{
				//do nothing
			}
			else
			{
				continue;
			}

			Com1SndBuf[0] = 0x06;
			Com1SndBuf[1] = Com1RcvBuf[1];
			com1_send_hex(Com1SndBuf, 2);


			dle_del(Com1RcvBuf, &gprs.rcv_len);

			if ( Com1RcvBuf[2] == 0x14 && Com1RcvBuf[3] == g_pack_num )
			{
				//do nothing
			}
			else
			{
				continue;
			}

			crc = Cal_Crc(&Com1RcvBuf[2], 9);
			if ( (Com1RcvBuf[2 + 9] == (INT8U)(crc >> 8)) && (Com1RcvBuf[2 + 9 + 1] = (INT8U)(crc & 0x00FF)) )
			{
				//do nothing
			}
			else
			{
				continue;
			}

			ltime.century = 0x20;
			ltime.year = Com1RcvBuf[5];
			ltime.month = Com1RcvBuf[6];
			ltime.day = Com1RcvBuf[7];
			ltime.hour = Com1RcvBuf[8];
			ltime.minute = Com1RcvBuf[9];
			ltime.second = Com1RcvBuf[10];

			OSSemPend(gprs_sem, 0, &err);
			(void)Modify_Time(&ltime);
			DevStat.update_time = FALSE;
			(void)OSSemPost(gprs_sem);
		}



		////////////////////////////////////////////////////
		//传输记录
		////////////////////////////////////////////////////
		OSSemPend(gprs_sem, 0, &err);
		gprs.record_base_ptr = DevStat.record_base_ptr;

		ReadParamRecNum();
		gprs.record_number = DevStat.record_number;
		gprs.driver_is_working = DevStat.driver_is_working;
		if ( gprs.record_number == 0 )
		{
			(void)OSSemPost(gprs_sem);
			goto update_blacklist;
		}

//  	if ( gprs.record_number < REC_NUM_PER_PACK )
//  	{
//  		(void)OSSemPost(gprs_sem);
//  		continue;
//  	}

		if ( gprs.record_number == 1 )
		{
//  		(void)OSSemPost(gprs_sem);

			//如果最后一条是司机卡记录，则上传
			memcpy((void *)g_tmp_tbuf, "\x04\x03\x11", 3);
			g_pack_num++;
			g_tmp_tbuf[3] = g_pack_num;

			gprs.driver_rec_num_temp = 0;

			for ( k = 0; k < 1; k++ )
			{
				Get_Record((INT8U *)&rec, gprs.record_base_ptr, k);
				memcpy(&g_tmp_tbuf[4 + k * REC_LEN], &rec, REC_LEN);
			}
			(void)OSSemPost(gprs_sem);

			crc = Cal_Crc(&g_tmp_tbuf[2], 2 + (1 * REC_LEN));
			g_tmp_tbuf[4 + (1 * REC_LEN)] = (INT8U)(crc >> 8);
			g_tmp_tbuf[4 + (1 * REC_LEN) + 1] = (INT8U)(crc & 0x00FF);

			g_retx = FALSE;

//  		if (rec.card_type == CARDT_DRIVER)
			if (gprs.driver_is_working != TRUE)					//下班状态下有1条记录也传
			{
				//开始通讯
				for ( j = 0; j < COMM_RETRY_TIMES; j++ )
				{
					gprs.snd_len = 4 + (1 * REC_LEN) + 2;
					form_comm_package(g_tmp_tbuf, &gprs.snd_len);

					CleanCom1Buf();
					com1_send_hex(Com1SndBuf, gprs.snd_len);
					i = com1_gets(Com1RcvBuf, &gprs.rcv_len, 1000, 10);
					if ( i == ok )
					{
						break;
					}

					g_retx = TRUE;
				}
				if ( j >= COMM_RETRY_TIMES )
				{
					continue;
				}

				i = com1_gets(Com1RcvBuf, &gprs.rcv_len, 10000, 10);
		//  	if ( i == ok && Com1RcvBuf[0] == 0x02 && Com1RcvBuf[2] == 0x00 )
				if ( i == ok && Com1RcvBuf[0] == 0x02 )
				{
					//do nothing
				}
				else
				{
					continue;
				}

				Com1SndBuf[0] = 0x06;
				Com1SndBuf[1] = Com1RcvBuf[1];
				com1_send_hex(Com1SndBuf, 2);

				dle_del(Com1RcvBuf, &gprs.rcv_len);

				if ( Com1RcvBuf[2] == 0x11 && Com1RcvBuf[3] == g_pack_num && Com1RcvBuf[4] == 0x00 )
				{
					//do nothing
				}
				else
				{
					continue;
				}


				//保存记录指针
				gprs.driver_rec_num = gprs.driver_rec_num_temp;
				gprs.driver_rec_num_temp = 0;

				OSSemPend(gprs_sem, 0, &err);

				DevStat.record_base_ptr += 1;                   //调整、储存正常采记录指针
				if ( DevStat.record_base_ptr >= (INT16U)MAX_REC_NUM )
				{
					DevStat.record_base_ptr %= (INT16U)MAX_REC_NUM;
				}
				DevStat.record_number -= 1;

				WriteParam();
				WriteParamRecNum();

				(void)OSSemPost(gprs_sem);

			}

		}
		else
		{
			memcpy((void *)g_tmp_tbuf, "\x04\x03\x11", 3);
			g_pack_num++;
			g_tmp_tbuf[3] = g_pack_num;

			gprs.driver_rec_num_temp = 0;

			for ( k = 0; k < REC_NUM_PER_PACK; k++ )
			{
				Get_Record((INT8U *)&rec, gprs.record_base_ptr, k);
				memcpy(&g_tmp_tbuf[4 + k * REC_LEN], &rec, REC_LEN);
			}
			(void)OSSemPost(gprs_sem);

			crc = Cal_Crc(&g_tmp_tbuf[2], 2 + (REC_NUM_PER_PACK * REC_LEN));
			g_tmp_tbuf[4 + (REC_NUM_PER_PACK * REC_LEN)] = (INT8U)(crc >> 8);
			g_tmp_tbuf[4 + (REC_NUM_PER_PACK * REC_LEN) + 1] = (INT8U)(crc & 0x00FF);

			g_retx = FALSE;
	//  	gprs.snd_len = 4 + (REC_NUM_PER_PACK * REC_LEN) + 2;
	//  	form_comm_package(g_tmp_tbuf, &gprs.snd_len);


			//开始通讯
			for ( j = 0; j < COMM_RETRY_TIMES; j++ )
			{
				gprs.snd_len = 4 + (REC_NUM_PER_PACK * REC_LEN) + 2;
				form_comm_package(g_tmp_tbuf, &gprs.snd_len);

				CleanCom1Buf();
				com1_send_hex(Com1SndBuf, gprs.snd_len);
				i = com1_gets(Com1RcvBuf, &gprs.rcv_len, 1000, 10);
				if ( i == ok )
				{
	//  			//debug
	//  			lcddisp(0, 0, "                ");
	//  			memset(disbuf, 0x00, sizeof(disbuf));
	//  			htoa(disbuf, Com1RcvBuf, gprs.rcv_len);
	//  			lcddisp(0, 0, disbuf);
	//  			lcddisp(1, 0, "                ");
	//  			sprintf(disbuf, "len=%d", gprs.rcv_len);
	//  			lcddisp(1, 0, disbuf);
	//  			SleepMs2(5000);
	//  			lcddisp(0, 0, "                ");
	//  			lcddisp(1, 0, "                ");
	//  			//debug end
					break;
				}

				g_retx = TRUE;
			}
			if ( j >= COMM_RETRY_TIMES )
			{
				continue;
			}

			i = com1_gets(Com1RcvBuf, &gprs.rcv_len, 10000, 10);
	//  	if ( i == ok && Com1RcvBuf[0] == 0x02 && Com1RcvBuf[2] == 0x00 )
			if ( i == ok && Com1RcvBuf[0] == 0x02 )
			{
				//do nothing
			}
			else
			{
				continue;
			}

			Com1SndBuf[0] = 0x06;
			Com1SndBuf[1] = Com1RcvBuf[1];
			com1_send_hex(Com1SndBuf, 2);

			dle_del(Com1RcvBuf, &gprs.rcv_len);

			if ( Com1RcvBuf[2] == 0x11 && Com1RcvBuf[3] == g_pack_num && Com1RcvBuf[4] == 0x00 )
			{
				//do nothing
			}
			else
			{
				continue;
			}


			//保存记录指针
			gprs.driver_rec_num = gprs.driver_rec_num_temp;
			gprs.driver_rec_num_temp = 0;

			OSSemPend(gprs_sem, 0, &err);

			DevStat.record_base_ptr += REC_NUM_PER_PACK;                   //调整、储存正常采记录指针
			if ( DevStat.record_base_ptr >= (INT16U)MAX_REC_NUM )
			{
				DevStat.record_base_ptr %= (INT16U)MAX_REC_NUM;
			}
			DevStat.record_number -= REC_NUM_PER_PACK;

			WriteParam();
			WriteParamRecNum();

			(void)OSSemPost(gprs_sem);


		}




update_blacklist:
		////////////////////////////////////////////////////
		//判断黑名单
		////////////////////////////////////////////////////
		Get_Time(&ltime);
		OSSemPend(gprs_sem, 0, &err);
		memcpy((void *)&last_time, (void *)&DevStat.update_black_time, 7);
		(void)OSSemPost(gprs_sem);

		time_cur = cal_sec(&ltime);
		time_last = cal_sec(&last_time);
		if ( (time_cur - time_last >= (INT32S)(24 * 60 * 60)) || (time_last - time_cur >= (INT32S)(24 * 60 * 60)) )   //超过1天没传黑名单
//  	if ( 0 )   //超过1天没传黑名单
		{
			//传输黑名单流程，先请求黑名单版本号和条数
			memcpy((void *)g_tmp_tbuf, "\x04\x03\x12", 3);
			g_pack_num++;
			g_tmp_tbuf[3] = g_pack_num;

			g_retx = FALSE;

//  		//add for debug
//  		gprs.snd_len = 4;
//  		form_comm_package(g_tmp_tbuf, &gprs.snd_len);
//  		com1_send_hex(Com1SndBuf, gprs.snd_len);
//  		com1puts("\xFF\xFF\xFF\xFF");
//
//  		gprs.black_num = 5000;
//  		//debug end

			//开始通讯
			for ( j = 0; j < COMM_RETRY_TIMES; j++ )
			{
				gprs.snd_len = 4;
				form_comm_package(g_tmp_tbuf, &gprs.snd_len);

				CleanCom1Buf();
				com1_send_hex(Com1SndBuf, gprs.snd_len);
				i = com1_gets(Com1RcvBuf, &gprs.rcv_len, 1000, 10);
				if ( i == ok )
				{
					break;
				}

				g_retx = TRUE;
			}
			if ( j >= COMM_RETRY_TIMES )
			{
				continue;
			}

			i = com1_gets(Com1RcvBuf, &gprs.rcv_len, 10000, 10);
			if ( i == ok && Com1RcvBuf[0] == 0x02 )
			{
				//do nothing
			}
			else
			{
				continue;
			}

			Com1SndBuf[0] = 0x06;
			Com1SndBuf[1] = Com1RcvBuf[1];
			com1_send_hex(Com1SndBuf, 2);


			dle_del(Com1RcvBuf, &gprs.rcv_len);

			if ( Com1RcvBuf[2] == 0x12 && Com1RcvBuf[3] == g_pack_num )
			{
				//do nothing
			}
			else
			{
				continue;
			}

			crc = Cal_Crc(&Com1RcvBuf[2], 11);
			if ( (Com1RcvBuf[2 + 11] == (INT8U)(crc >> 8)) && (Com1RcvBuf[2 + 11 + 1] = (INT8U)(crc & 0x00FF)) )
			{
				//do nothing
			}
			else
			{
				continue;
			}

			memcpy(&gprs.black_name_version[0], &Com1RcvBuf[4], 7);
			gprs.black_num = (INT16U)Com1RcvBuf[11] * 0x100 + (INT16U)Com1RcvBuf[12];


			//传输黑名单流程，开始传输黑名单
			gprs.black_ptr = 0;

			if ( gprs.black_num % BLACK_NUM_PER_PACK == 0 )
			{
				trans_times = gprs.black_num / BLACK_NUM_PER_PACK;
			}
			else
			{
				trans_times = (gprs.black_num / BLACK_NUM_PER_PACK) + 1;
			}


			//传输中断判断
			if ( gprs_err.comm_break == TRUE )
			{
				gprs.black_ptr = gprs_err.black_ptr;
				k = gprs_err.black_ptr / BLACK_NUM_PER_PACK;				//继续传输黑名单
			}
			else
			{
				k = 0;														//重头开始传输黑名单
			}



//  		for ( k = 0; k < trans_times; k++ )
			for ( ; k < trans_times; k++ )
			{
				if ( k == trans_times - 1 )               //最后一包
				{
					if ( gprs.black_num % BLACK_NUM_PER_PACK == 0 )
					{
						last_package_black_num = BLACK_NUM_PER_PACK;
					}
					else
					{
						last_package_black_num = gprs.black_num % BLACK_NUM_PER_PACK;
					}


					memcpy((void *)g_tmp_tbuf, "\x04\x03\x13", 3);
					g_pack_num++;
					g_tmp_tbuf[3] = g_pack_num;

					g_tmp_tbuf[4] = (INT8U)((k * (INT16U)BLACK_NUM_PER_PACK) / 0x100);
					g_tmp_tbuf[5] = (INT8U)((k * (INT16U)BLACK_NUM_PER_PACK) % 0x100);
					g_tmp_tbuf[6] = (INT8U)(last_package_black_num / 0x100);
					g_tmp_tbuf[7] = (INT8U)(last_package_black_num % 0x100);

					g_retx = FALSE;

					//开始通讯
					for ( comm_retry = 0; comm_retry < 10; comm_retry++ )
					{
//  					if ( (comm_retry % 10 == 0) && (comm_retry != 0) )
//  					{
//  						SleepMs2(20000);
//  						CleanCom1Buf();
//  						(void)com1_gets(Com1RcvBuf, &gprs.rcv_len, 1000, 10);
//  					}

						for ( j = 0; j < COMM_RETRY_TIMES; j++ )
						{
							gprs.snd_len = 4 + 4;
							form_comm_package(g_tmp_tbuf, &gprs.snd_len);

							CleanCom1Buf();
							com1_send_hex(Com1SndBuf, gprs.snd_len);
							i = com1_gets(Com1RcvBuf, &gprs.rcv_len, 1000, 10);
							if ( i == ok )
							{
								break;
							}

							g_retx = TRUE;
						}
						if ( j >= COMM_RETRY_TIMES )
						{
							SleepMs2(5000);
							continue;
						}

						i = com1_gets(Com1RcvBuf, &gprs.rcv_len, 10000, 10);
						if ( i == ok && Com1RcvBuf[0] == 0x02 )
						{
							//do nothing
						}
						else
						{
							SleepMs2(5000);
							continue;
						}

						Com1SndBuf[0] = 0x06;
						Com1SndBuf[1] = Com1RcvBuf[1];
						com1_send_hex(Com1SndBuf, 2);

						dle_del(Com1RcvBuf, &gprs.rcv_len);

						if ( gprs.rcv_len < last_package_black_num * 4 + 2)
						{
							SleepMs2(5000);
							continue;		//长度不对
						}

						if ( Com1RcvBuf[2] == 0x13 && Com1RcvBuf[3] == g_pack_num )
						{
							//do nothing
						}
						else
						{
							SleepMs2(5000);
							continue;
						}

						crc = Cal_Crc(&Com1RcvBuf[2], last_package_black_num * 4 + 2);
						if ( (Com1RcvBuf[2 + last_package_black_num * 4 + 2] == (INT8U)(crc >> 8)) 
							 && (Com1RcvBuf[2 + last_package_black_num * 4 + 2 + 1] = (INT8U)(crc & 0x00FF)) )
						{
							//do nothing
						}
						else
						{
							SleepMs2(5000);
							continue;
						}


						break;
					}
					if (comm_retry >= 10)
					{
						gprs_err.comm_break = TRUE;
						gprs_err.black_ptr = gprs.black_ptr;
						gprs_err.black_num = gprs.black_num;
						memcpy(&gprs_err.black_name_version[0], &gprs.black_name_version[0], sizeof(gprs_err.black_name_version));

						goto com1_start;			//通讯失败
					}

					
					for ( j = 0; j < last_package_black_num; j++ )
					{
						black_rec[0] = Com1RcvBuf[4 + j * 4];
						black_rec[1] = Com1RcvBuf[4 + j * 4 + 1];
						black_rec[2] = Com1RcvBuf[4 + j * 4 + 2];
						black_rec[3] = Com1RcvBuf[4 + j * 4 + 3];

						OSSemPend(gprs_sem, 0, &err);
						(void)StoreBlackName2(black_rec, gprs.black_ptr);                    //将黑名单存入NVRAM
						(void)OSSemPost(gprs_sem);

						gprs.black_ptr++;
					}

					//传输完毕，导入正式库
					OSSemPend(gprs_sem, 0, &err);
					for (m = 0; m < gprs.black_ptr; m++)
					{
						GetBlackName2(black_rec, m);
						(void)StoreBlackName(black_rec, m);                    //将黑名单存入NVRAM
					}
//  				DevStat.black_name_number = gprs.black_ptr;
					DevStat.black_name_number = gprs.black_num;
					memcpy((void *)&DevStat.black_name_version[0], (void *)&gprs.black_name_version[0], 7);
					Get_Time(&ltime);
					memcpy((void *)&DevStat.update_black_time, (void *)&ltime, 7);

					gprs_err.comm_break = FALSE;

					WriteParam();
					(void)OSSemPost(gprs_sem);

					goto com1_start;

				}
				else                                            //不是最后一包
				{
					memcpy((void *)g_tmp_tbuf, "\x04\x03\x13", 3);
					g_pack_num++;
					g_tmp_tbuf[3] = g_pack_num;

					g_tmp_tbuf[4] = (INT8U)((k * (INT16U)BLACK_NUM_PER_PACK) / 0x100);
					g_tmp_tbuf[5] = (INT8U)((k * (INT16U)BLACK_NUM_PER_PACK) % 0x100);
					g_tmp_tbuf[6] = (INT8U)(BLACK_NUM_PER_PACK / 0x100);
					g_tmp_tbuf[7] = (INT8U)(BLACK_NUM_PER_PACK % 0x100);

					g_retx = FALSE;

//  				//add for debug
//  				gprs.snd_len = 4 + 4;
//  				form_comm_package(g_tmp_tbuf, &gprs.snd_len);
//  				com1_send_hex(Com1SndBuf, gprs.snd_len);
//  				com1puts("\xFF\xFF\xFF\xFF");
//
//
//  				if (gprs_err.comm_break == FALSE)
//  				{
//  					if (k == 5)
//  					{
//  						gprs_err.comm_break = TRUE;
//  						gprs_err.black_ptr = gprs.black_ptr;
//  						gprs_err.black_num = gprs.black_num;
//  						memcpy(&gprs_err.black_name_version[0], &gprs.black_name_version[0], sizeof(gprs_err.black_name_version));
//
//  						goto com1_start;            //通讯失败
//  					}
//  				}
//
//
//  				//debug end

					//开始通讯
					for ( comm_retry = 0; comm_retry < 10; comm_retry++ )
					{
//  					if ( (comm_retry % 10 == 0) && (comm_retry != 0) )
//  					{
//  						SleepMs2(20000);
//  						CleanCom1Buf();
//  						(void)com1_gets(Com1RcvBuf, &gprs.rcv_len, 1000, 10);
//  					}

						for ( j = 0; j < COMM_RETRY_TIMES; j++ )
						{
							gprs.snd_len = 4 + 4;
							form_comm_package(g_tmp_tbuf, &gprs.snd_len);

							CleanCom1Buf();
							com1_send_hex(Com1SndBuf, gprs.snd_len);
							i = com1_gets(Com1RcvBuf, &gprs.rcv_len, 1000, 10);
							if ( i == ok )
							{
								break;
							}

							g_retx = TRUE;
						}
						if ( j >= COMM_RETRY_TIMES )
						{
							SleepMs2(5000);
							continue;
						}

						i = com1_gets(Com1RcvBuf, &gprs.rcv_len, 10000, 10);
						if ( i == ok && Com1RcvBuf[0] == 0x02 )
						{
							//do nothing
						}
						else
						{
							SleepMs2(5000);
							continue;
						}

						Com1SndBuf[0] = 0x06;
						Com1SndBuf[1] = Com1RcvBuf[1];
						com1_send_hex(Com1SndBuf, 2);

						dle_del(Com1RcvBuf, &gprs.rcv_len);

						if ( gprs.rcv_len < BLACK_NUM_PER_PACK * 4 + 2)
						{
							SleepMs2(5000);
							continue;       //长度不对
						}

						if ( Com1RcvBuf[2] == 0x13 && Com1RcvBuf[3] == g_pack_num )
						{
							//do nothing
						}
						else
						{
							SleepMs2(5000);
							continue;
						}

						crc = Cal_Crc(&Com1RcvBuf[2], BLACK_NUM_PER_PACK * 4 + 2);
						if ( (Com1RcvBuf[2 + BLACK_NUM_PER_PACK * 4 + 2] == (INT8U)(crc >> 8))
							 && (Com1RcvBuf[2 + BLACK_NUM_PER_PACK * 4 + 2 + 1] = (INT8U)(crc & 0x00FF)) )
						{
							//do nothing
						}
						else
						{
							SleepMs2(5000);
							continue;
						}

						break;
					}
					if (comm_retry >= 10)
					{
//  					//debug
//  					memset(disbuf, 0x00, sizeof(disbuf));
//  					lcddisp(0, 0, "                ");
//  					sprintf(disbuf, "retry=%d", comm_retry);
//  					lcddisp(0, 0, disbuf);
//  					lcddisp(1, 0, "err4            ");
//  					SleepMs2(2000);
//  					lcddisp(0, 0, "                ");
//  					lcddisp(1, 0, "                ");
//  					//debug end
						gprs_err.comm_break = TRUE;
						gprs_err.black_ptr = gprs.black_ptr;
						gprs_err.black_num = gprs.black_num;
						memcpy(&gprs_err.black_name_version[0], &gprs.black_name_version[0], sizeof(gprs_err.black_name_version));

						goto com1_start;            //通讯失败
					}

					
					for ( j = 0; j < BLACK_NUM_PER_PACK; j++ )
					{
						black_rec[0] = Com1RcvBuf[4 + j * 4];
						black_rec[1] = Com1RcvBuf[4 + j * 4 + 1];
						black_rec[2] = Com1RcvBuf[4 + j * 4 + 2];
						black_rec[3] = Com1RcvBuf[4 + j * 4 + 3];

						OSSemPend(gprs_sem, 0, &err);
						(void)StoreBlackName2(black_rec, gprs.black_ptr);                    //将黑名单存入NVRAM
						(void)OSSemPost(gprs_sem);

						gprs.black_ptr++;
					}

					//显示进度
					memset(disbuf, 0x00, sizeof(disbuf));
					lcddisp(0, 0, "                ");
					sprintf(disbuf, "黑名单%d/%d", BLACK_NUM_PER_PACK * k, gprs.black_num);
					lcddisp(0, 0, disbuf);


				}

			}

		}











	}

}

/******************************************************************************
 函数名称：dle_add
 功能描述：整理数据包，添加dle
 参数描述：
 参数名称： 输入/输出？ 类型		描述  
 buf		输入输出	INT8U *		要进行dle整理的数据包  
 len		输入输出	INT8U *		长度 
				
 返  回  值：无
				   
 作 	 者：刘及华
 日 	 期：2010-06-18
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void dle_add(INT8U *buf, INT16U *len)
{
	//  INT16U len1 = *len;
	INT16U len2 = *len;
	INT16U k = 0;

	for (;;)
	{
		if ( buf[k] == 0x02 || buf[k] == 0x03 || buf[k] == 0x10 )
		{
			memmove(&buf[k + 1], &buf[k], len2 - k);
			buf[k] = 0x10;
			len2++;
			k++;
		}
		k++;
		if ( k >= len2 )
		{
			*len = len2;
			return;
		}
	}

}

/******************************************************************************
 函数名称：dle_del
 功能描述：整理数据包，删除dle
 参数描述：
 参数名称： 输入/输出？ 类型		描述  
 buf		输入输出	INT8U *		要进行dle整理的数据包  
 len		输入		INT16U *	长度 
				
 返  回  值：无
				   
 作 	 者：刘及华
 日 	 期：2010-06-18
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void dle_del(INT8U *buf, INT16U *len)
{
	//  INT16U len1 = *len;
	INT16U len2 = *len;
	INT16U k = 0;

	for (;;)
	{
		if ( buf[k] == 0x10 )
		{
			if ( buf[k + 1] == 0x02 || buf[k + 1] == 0x03 || buf[k + 1] == 0x10 )
			{
				memmove(&buf[k], &buf[k + 1], len2 - k);
				len2--;
			}
		}

		k++;
		if ( k >= len2 )
		{
			*len = len2;
			return;
		}

	}

}

/******************************************************************************
 函数名称：form_comm_package
 功能描述：整理数据包，添加bcc dle 包头包尾，维护交替位
 参数描述：
 参数名称： 输入/输出？ 类型		描述  
 buf		输入输出	INT8U *		要进行整理的数据包  
 len		输入输出	INT8U *		长度 
				
 返  回  值：无
				   
 作 	 者：刘及华
 日 	 期：2010-06-18
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void form_comm_package(INT8U *buf, INT16U *len)
{
	INT8U bcc = 0;
	INT16U llen = *len;

	memcpy(Com1SndBuf, buf, llen);

	if (g_turn_bit == 0x00)
	{
		Com1SndBuf[0] =( Com1SndBuf[0] & (~BIT0) );
		g_turn_bit = 0x01;
	}
	else
	{
		Com1SndBuf[0] |= 0x01;
		g_turn_bit = 0x00;
	}

	if (g_retx == TRUE)
	{
		Com1SndBuf[0] |= 0x02;
	}
	else
	{
		Com1SndBuf[0] =( Com1SndBuf[0] & (~BIT1) );
	}

	bcc = Cal_BCC(Com1SndBuf, llen);
	Com1SndBuf[llen] = bcc;
	llen++;

	dle_add(Com1SndBuf, &llen);

	memmove(&Com1SndBuf[1], &Com1SndBuf[0], llen);
	Com1SndBuf[0] = 0x02;
	Com1SndBuf[llen + 1] = 0x03;
	llen += 2;

	*len = llen;
}

///******************************************************************************
// 函数名称：form_comm_package
// 功能描述：整理数据包，添加bcc dle 包头包尾，维护交替位
// 参数描述：
// 参数名称： 输入/输出？ 类型        描述
// buf        输入输出    INT8U *     要进行整理的数据包
// len        输入输出    INT8U *     长度
//
// 返  回  值：无
//
// 作      者：刘及华
// 日      期：2010-06-18
// 修改历史：
//        日期        修改人      修改描述
//        ------      ---------   -------------
//******************************************************************************/
//void form_comm_package(INT8U *buf, INT16U *len)
//{
//
//            //传输黑名单流程，先请求黑名单版本号和条数
//
//            memcpy((void *)g_tmp_tbuf, "\x04\x03\x12", 3);
//            g_pack_num++;
//            g_tmp_tbuf[3] = g_pack_num;
//
//            g_retx = FALSE;
//
//
//            //开始通讯
//            for ( j = 0; j < 2; j++ )
//            {
//                gprs.snd_len = 4;
//                form_comm_package(g_tmp_tbuf, &gprs.snd_len);
//
//                CleanCom1Buf();
//                com1_send_hex(Com1SndBuf, gprs.snd_len);
//                i = com1_gets(Com1RcvBuf, &gprs.rcv_len, 1000, 10);
//                if ( i == ok )
//                {
//                    break;
//                }
//
//                g_retx = TRUE;
//            }
//            if ( j >= 2 )
//            {
//                continue;
//            }
//
//            i = com1_gets(Com1RcvBuf, &gprs.rcv_len, 10000, 10);
//            if ( i == ok && Com1RcvBuf[0] == 0x02 )
//            {
//                //do nothing
//            }
//            else
//            {
//                continue;
//            }
//
//            Com1SndBuf[0] = 0x06;
//            Com1SndBuf[1] = Com1RcvBuf[1];
//            com1_send_hex(Com1SndBuf, 2);
//
//
//            dle_del(Com1RcvBuf, &gprs.rcv_len);
//
//            if ( Com1RcvBuf[2] == 0x12 && Com1RcvBuf[3] == g_pack_num )
//            {
//                //do nothing
//            }
//            else
//            {
//                continue;
//            }
//
//            crc = Cal_Crc(&Com1RcvBuf[2], 11);
//            if ( (Com1RcvBuf[2 + 11] == (INT8U)(crc >> 8)) && (Com1RcvBuf[2 + 11 + 1] = (INT8U)(crc & 0x00FF)) )
//            {
//                //do nothing
//            }
//            else
//            {
//                continue;
//            }
//
//}

/******************************************************************************
 函数名称：com1puts
 功能描述：串口1输出函数
 参数描述：
 参数名称： 输入/输出？ 类型		描述
 snd_buf	输入			void *		要发送的字符串
				
 返  回  值：无
				   
 作 	 者 ：刘及华
 日 	 期：2005-04-07
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void com1puts(const void * snd_buf)
{
	com1_send_hex((void *)snd_buf, (INT16U)strlen(snd_buf));
}
