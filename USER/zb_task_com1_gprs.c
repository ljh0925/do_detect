/******************************************************************************
Copyright 2005 �����״�ͨ�������޹�˾
All rights riserved.

�ļ���	��task_com1_gprs.c
ģ�����ƣ�com1ͨѶ���� GPRSͨѶ
���ܸ�Ҫ��com1ͨѶ���� GPRSͨѶ

ȡ���汾��0.0.1
�޸���	��������
������ڣ�2005.04.06
����˵����create

******************************************************************************/
#include 		"all.h"


OS_STK  TASK4_STACK[TASK4_STACK_SIZE];

extern volatile DEV_STAT DevStat;

INT8U Com1SndBuf[UART1_SEND_QUEUE_LENGTH];              //���ڷ��ͻ�����
INT8U Com1RcvBuf[UART1_RCV_QUEUE_LENGTH];               //���ڽ��ջ�����

extern int UART1RcvBuf[(UART1_RCV_QUEUE_LENGTH + sizeof(int)- 1)/ sizeof(int)];

//INT8U SndBuf[UART3_SEND_QUEUE_LENGTH];
OS_EVENT        *gprs_sem;

typedef struct _GPRS
{
	INT16U record_base_ptr;                 //δ�ϴ���¼ָ����ʼָ��(ԭp0+2)
	INT16U record_number;                   //δ�ϴ��ļ�¼����(ԭp0+4)

	BUS_TIME last_commu_time;               //�ϴ�ͨѶʱ��
	INT8U have_driver_rec;                  //�Ƿ���˾�����°��¼
	INT16U snd_len;                         //���ͳ���
	INT16U rcv_len;                         //���ճ���


	INT8U driver_rec_num_temp;              //˾�����°��¼���� ��ʱ
	INT8U driver_rec_num;                   //˾�����°��¼����
	INT8U driver_rec_ptr;                   //˾�����°��¼����������������
//  INT8U drec[7][18];                      //˾����¼ 2�ֽ�˾�����+8�ֽڳ���+7�ֽ�ʱ��+1�ֽ����°��־
//  INT8U drec[7][20];                      //˾����¼ 4�ֽ�˾�����+8�ֽڳ���+7�ֽ�ʱ��+1�ֽ����°��־

	INT8U black_name_version[7];            //�������汾��
	INT16U black_num;                       //Ҫ����ĺ���������
	INT16U black_ptr;						//����������ָ��


	INT8U drec[20];							//˾����¼ 4�ֽ�˾�����+8�ֽڳ���+7�ֽ�ʱ��+1�ֽ����°��־
	INT8U have_drec;						//�Ƿ���δ�ϴ���drec����=TRUE ��=FALSE
	INT8U update_time;						//�Ƿ�׼���������ͬ��ʱ�䣬��=TRUE ��=FALSE
	INT8U driver_is_working;				//���°��־

}GPRS;

GPRS gprs;

static INT8U g_pack_num = 0;				//����
static INT8U g_turn_bit = 0;				//����λ
static INT8U g_retx = 0;					//�Ƿ�Ϊ�ط�

INT8U g_tmp_tbuf[300];
INT8U g_tmp_tbuf2[100];

//debug
char disbuf[100];
//debug end


typedef struct _GPRS_ERR
{
	INT8U comm_break;						//�����������Ƿ��жϹ� �жϹ�=TRUE û�жϹ�=FALSE

	INT8U black_name_version[7];            //�ж�ʱ�̵ĺ������汾��
	INT16U black_num;                       //�ж�ʱ�̵�Ҫ����ĺ���������
	INT16U black_ptr;						//�ж�ʱ�̵ĺ���������ָ��

}GPRS_ERR;

GPRS_ERR gprs_err;

/******************************************************************************
 �������ƣ�TaskCom1Gprs
 ����������com1gprsͨѶ����
 ����������
 �������ƣ� ����/����� ����		����
				
 ��  ��  ֵ����
				   
 �� 	 �ߣ�������
 �� 	 �ڣ�2010-06-18
 �޸���ʷ��
		����		�޸���		�޸�����
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
//#define REC_NUM_PER_PACK	4			//ÿ��4����¼
#define REC_NUM_PER_PACK	2			//ÿ��2����¼
#define COMM_RETRY_TIMES	1			//ͨѶʱ�����Դ���

	data = data;

	//  OSTaskSuspend(TASK4_PRIO);				//����task_com1_shell

	UART1_SetBPS(9600, NONE);

	gprs.driver_rec_num_temp = 0;
	gprs.driver_rec_num = 0;
	gprs.driver_rec_ptr = 0;
	gprs_err.comm_break = FALSE;


	g_pack_num = 0;				//����
	g_turn_bit = 0;				//����λ
	g_retx = 0;					//�Ƿ�Ϊ�ط�

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
		//��˾����¼�ȴ�˾����¼
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


			//��ʼͨѶ
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
		//�������ͬ��ʱ��
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


			//��ʼͨѶ
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
		//�����¼
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

			//������һ����˾������¼�����ϴ�
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
			if (gprs.driver_is_working != TRUE)					//�°�״̬����1����¼Ҳ��
			{
				//��ʼͨѶ
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


				//�����¼ָ��
				gprs.driver_rec_num = gprs.driver_rec_num_temp;
				gprs.driver_rec_num_temp = 0;

				OSSemPend(gprs_sem, 0, &err);

				DevStat.record_base_ptr += 1;                   //���������������ɼ�¼ָ��
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


			//��ʼͨѶ
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


			//�����¼ָ��
			gprs.driver_rec_num = gprs.driver_rec_num_temp;
			gprs.driver_rec_num_temp = 0;

			OSSemPend(gprs_sem, 0, &err);

			DevStat.record_base_ptr += REC_NUM_PER_PACK;                   //���������������ɼ�¼ָ��
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
		//�жϺ�����
		////////////////////////////////////////////////////
		Get_Time(&ltime);
		OSSemPend(gprs_sem, 0, &err);
		memcpy((void *)&last_time, (void *)&DevStat.update_black_time, 7);
		(void)OSSemPost(gprs_sem);

		time_cur = cal_sec(&ltime);
		time_last = cal_sec(&last_time);
		if ( (time_cur - time_last >= (INT32S)(24 * 60 * 60)) || (time_last - time_cur >= (INT32S)(24 * 60 * 60)) )   //����1��û��������
//  	if ( 0 )   //����1��û��������
		{
			//������������̣�������������汾�ź�����
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

			//��ʼͨѶ
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


			//������������̣���ʼ���������
			gprs.black_ptr = 0;

			if ( gprs.black_num % BLACK_NUM_PER_PACK == 0 )
			{
				trans_times = gprs.black_num / BLACK_NUM_PER_PACK;
			}
			else
			{
				trans_times = (gprs.black_num / BLACK_NUM_PER_PACK) + 1;
			}


			//�����ж��ж�
			if ( gprs_err.comm_break == TRUE )
			{
				gprs.black_ptr = gprs_err.black_ptr;
				k = gprs_err.black_ptr / BLACK_NUM_PER_PACK;				//�������������
			}
			else
			{
				k = 0;														//��ͷ��ʼ���������
			}



//  		for ( k = 0; k < trans_times; k++ )
			for ( ; k < trans_times; k++ )
			{
				if ( k == trans_times - 1 )               //���һ��
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

					//��ʼͨѶ
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
							continue;		//���Ȳ���
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

						goto com1_start;			//ͨѶʧ��
					}

					
					for ( j = 0; j < last_package_black_num; j++ )
					{
						black_rec[0] = Com1RcvBuf[4 + j * 4];
						black_rec[1] = Com1RcvBuf[4 + j * 4 + 1];
						black_rec[2] = Com1RcvBuf[4 + j * 4 + 2];
						black_rec[3] = Com1RcvBuf[4 + j * 4 + 3];

						OSSemPend(gprs_sem, 0, &err);
						(void)StoreBlackName2(black_rec, gprs.black_ptr);                    //������������NVRAM
						(void)OSSemPost(gprs_sem);

						gprs.black_ptr++;
					}

					//������ϣ�������ʽ��
					OSSemPend(gprs_sem, 0, &err);
					for (m = 0; m < gprs.black_ptr; m++)
					{
						GetBlackName2(black_rec, m);
						(void)StoreBlackName(black_rec, m);                    //������������NVRAM
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
				else                                            //�������һ��
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
//  						goto com1_start;            //ͨѶʧ��
//  					}
//  				}
//
//
//  				//debug end

					//��ʼͨѶ
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
							continue;       //���Ȳ���
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

						goto com1_start;            //ͨѶʧ��
					}

					
					for ( j = 0; j < BLACK_NUM_PER_PACK; j++ )
					{
						black_rec[0] = Com1RcvBuf[4 + j * 4];
						black_rec[1] = Com1RcvBuf[4 + j * 4 + 1];
						black_rec[2] = Com1RcvBuf[4 + j * 4 + 2];
						black_rec[3] = Com1RcvBuf[4 + j * 4 + 3];

						OSSemPend(gprs_sem, 0, &err);
						(void)StoreBlackName2(black_rec, gprs.black_ptr);                    //������������NVRAM
						(void)OSSemPost(gprs_sem);

						gprs.black_ptr++;
					}

					//��ʾ����
					memset(disbuf, 0x00, sizeof(disbuf));
					lcddisp(0, 0, "                ");
					sprintf(disbuf, "������%d/%d", BLACK_NUM_PER_PACK * k, gprs.black_num);
					lcddisp(0, 0, disbuf);


				}

			}

		}











	}

}

/******************************************************************************
 �������ƣ�dle_add
 �����������������ݰ������dle
 ����������
 �������ƣ� ����/����� ����		����  
 buf		�������	INT8U *		Ҫ����dle��������ݰ�  
 len		�������	INT8U *		���� 
				
 ��  ��  ֵ����
				   
 �� 	 �ߣ�������
 �� 	 �ڣ�2010-06-18
 �޸���ʷ��
		����		�޸���		�޸�����
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
 �������ƣ�dle_del
 �����������������ݰ���ɾ��dle
 ����������
 �������ƣ� ����/����� ����		����  
 buf		�������	INT8U *		Ҫ����dle��������ݰ�  
 len		����		INT16U *	���� 
				
 ��  ��  ֵ����
				   
 �� 	 �ߣ�������
 �� 	 �ڣ�2010-06-18
 �޸���ʷ��
		����		�޸���		�޸�����
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
 �������ƣ�form_comm_package
 �����������������ݰ������bcc dle ��ͷ��β��ά������λ
 ����������
 �������ƣ� ����/����� ����		����  
 buf		�������	INT8U *		Ҫ������������ݰ�  
 len		�������	INT8U *		���� 
				
 ��  ��  ֵ����
				   
 �� 	 �ߣ�������
 �� 	 �ڣ�2010-06-18
 �޸���ʷ��
		����		�޸���		�޸�����
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
// �������ƣ�form_comm_package
// �����������������ݰ������bcc dle ��ͷ��β��ά������λ
// ����������
// �������ƣ� ����/����� ����        ����
// buf        �������    INT8U *     Ҫ������������ݰ�
// len        �������    INT8U *     ����
//
// ��  ��  ֵ����
//
// ��      �ߣ�������
// ��      �ڣ�2010-06-18
// �޸���ʷ��
//        ����        �޸���      �޸�����
//        ------      ---------   -------------
//******************************************************************************/
//void form_comm_package(INT8U *buf, INT16U *len)
//{
//
//            //������������̣�������������汾�ź�����
//
//            memcpy((void *)g_tmp_tbuf, "\x04\x03\x12", 3);
//            g_pack_num++;
//            g_tmp_tbuf[3] = g_pack_num;
//
//            g_retx = FALSE;
//
//
//            //��ʼͨѶ
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
 �������ƣ�com1puts
 ��������������1�������
 ����������
 �������ƣ� ����/����� ����		����
 snd_buf	����			void *		Ҫ���͵��ַ���
				
 ��  ��  ֵ����
				   
 �� 	 �� ��������
 �� 	 �ڣ�2005-04-07
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void com1puts(const void * snd_buf)
{
	com1_send_hex((void *)snd_buf, (INT16U)strlen(snd_buf));
}
