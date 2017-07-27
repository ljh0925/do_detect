/******************************************************************************
Copyright 2005 �����״�ͨ�������޹�˾
All rights riserved.

�ļ���	��task_irda_comm.c
ģ�����ƣ�����ͨѶ����
���ܸ�Ҫ������ͨѶ����

ȡ���汾��0.0.1
�޸���	��������
������ڣ�2005.04.06
����˵����create

******************************************************************************/
#include 		"all.h"


//OS_STK  TASK2_STACK[TASK2_STACK_SIZE];

extern volatile DEV_STAT DevStat;

extern INT8U Com4SndBuf[UART4_SEND_QUEUE_LENGTH];				//���ڷ��ͻ�����
extern INT8U Com4RcvBuf[UART4_RCV_QUEUE_LENGTH];				//���ڽ��ջ�����


//INT8U SndBuf[UART3_SEND_QUEUE_LENGTH];

/******************************************************************************
 �������ƣ�task_irda_comm
 ��������������ͨѶ����
 ����������
 �������ƣ� ����/����� ����		����
				
 ��  ��  ֵ����
				   
 �� 	 �ߣ�������
 �� 	 �ڣ�2010-06-18
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void task_irda_comm(void * data)
{
	INT16U rcv_len = 0;									//�����򳤶�

	data = data;		//avoid warning

	ToggleWD();


	for ( ;; )
	{

		(void)com4_gets(Com4RcvBuf, &rcv_len, 0, 5);

		if ( Com4RcvBuf[0] != '@' )			//�жϿ�ͷ
			continue;

		if ( rcv_len < 4 )
			continue;

		if ( (Com4RcvBuf[rcv_len-1] == 0x0D) && (Com4RcvBuf[rcv_len-2] == 0x2A) )		 //�ж�β�Ƿ��Ѿ�����
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
			//��ʼ��
			////////////////////////////////////////
			case 'S':				//��ʼ��
				if ( (rcv_len == 16) && Com4RcvBuf[5] == 'I' && Com4RcvBuf[6] == 'n' 
					 && Com4RcvBuf[7] == 'i' && Com4RcvBuf[8] == 't' )
				{
//  				(void)InitImportantParameter();
//  				(void)Rx8025Init();							//ʱ�ӳ�ʼ��
//					init_param();

					memcpy(Com4SndBuf , Com4RcvBuf, rcv_len);
					com4_send_hex(Com4SndBuf , rcv_len);					//��ʲô����ʲô

					Beep(500);
					resetpos();				//��λ
				}
				break;

				////////////////////////////////////////
				//����ʱ��
				////////////////////////////////////////
			case 'T':				//����ʱ��
				if ( rcv_len == 20 )
				{
					if ( DevStat.allow_upload_data == TRUE )
					{
//  				UploadDisp = UPDATE_TIME_SUCCESS;
						lcddisp(1, 0, "  ʱ����³ɹ�  ");
					}

					UpdateTime(Com4RcvBuf);

					memcpy(Com4SndBuf , Com4RcvBuf, rcv_len);
					com4_send_hex(Com4SndBuf , rcv_len);					//��ʲô����ʲô
				}
				break;

				////////////////////////////////////////
				//��������
				////////////////////////////////////////
			case 'R':				//��������
				if ( (rcv_len == 15) && (Com4RcvBuf[5] == 'R') && (Com4RcvBuf[6] == 'E') && (Com4RcvBuf[7] == 'L')
					 && (Com4RcvBuf[8] == 'O') && (Com4RcvBuf[9] == 'A') && (Com4RcvBuf[10] == 'D') )
				{
					memcpy(Com4SndBuf , Com4RcvBuf, rcv_len);
					com4_send_hex(Com4SndBuf , rcv_len);					//��ʲô����ʲô
				}
				break;

			default:	//do nothing
				break;
		}

	}

}

