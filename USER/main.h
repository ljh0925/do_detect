/****************************************Copyright (c) ****************************************
**                         Guangzhou ZHIYUAN electronics Co.,LTD.                               
**                                     
**                               http://www.embedtools.com
**
**�D�D�DFile Info�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D
**  File          name :main.h
**  Last modified Date :
**  Last       Version :V1.0
**  Descriptions       :�û�����������ͷ�ļ�
**
**�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D
** Created   	    by :
** Created  	  date :2007-4-17
** Version             :
** Descriptions        :
**
**�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D�D
** Modified	        by :
** Modified	      date :
** Version		       :
** Descriptions		   :
**
*******************************************************************************************/
#ifndef __MAIN_H 
#define __MAIN_H

#ifdef __cplusplus

extern "C" {
#endif

#include "data_type.h"



//ȫ�������������
//#define EN_WDT    //��ֹ���Ź�


//#define TEST_CARD
//#define DATA_BOX_COMM_32


/*********************************************************************************************************
*											   DATA TYPES
*										  (Compiler Specific)
*********************************************************************************************************/
#define 	TRUE					1
#define 	FALSE					0
#define 	ok						0
#define     notok					0xFF
#define 	HIGH					1
#define 	LOW						0
#define 	ON						0
#define 	OFF						1

#define CPU_CARD					0x08
#define M1_CARD						0x04

#define RDK 0			//����������Կ
#define WRK 1			//д�ͳ�ֵ��Կ

#define PSAM_PRI	0
#define PSAM_SLV	1
#define PSAM3		2
#define PSAM4		3

#define DISP_DELAY_COUNTER_END		0x00
#define DISP_DELAY					1000						//���׳ɹ������ͣ������Ļ�ϵ�ʱ��
#define SINGLE_KEY_QUERY_DELAY		2000					//������ѯʱ

#define	RECORD_IS_MAN     		0x01
#define	PRICE_IS_ZERO	  		0x02
#define	CARD_TIME_NOUSE			0x03
#define	CARD_LAW_LESS			0x04
#define	PRI_FLASH_ERROR    		0x05
#define	SLV_FLASH_ERROR			0x06
#define	DOUBLE_FLASH_ERROR		0x07
#define	FORMAT_FLASH			0x08
#define	FLASH_SAVE_ERROR		0x09
#define	LINE_ERROR				0x1000


#define MAX_REC_NUM				10200
#define	REC_LEN					48
#define	BLACK_LEN				4
#define MAX_REC_NUM_FLASH2		40000
#define MAX_BLACK_NUM			8000


#define SHELL_DELAY             500

#define MAX_DISP_REC	61						//������ѯʱ��������ʾ�ļ�¼����Ҫ��<=255



//��������������
#define	NO_ERROR				0
#define	PSAM_ERROR				0x01
#define	READ_CLOCK_ERROR		0x02
#define	CLOCK_FORMAT_ERROR		0x04
#define	RC500_ERROR				0x08
#define	RAM_ERROR				0x10
#define	NVRAM_ERROR				0x20
#define	FERRO_ERROR				0x40
#define FLASH_ERROR				0x80
#define FLASH_SLV_ERROR			0x0100
#define RX8025_ERROR			0x0200
#define BATTARY_ERROR			0x0400
#define GPS_ID_ERROR			0x0800
#define NRF_ERROR				0x1000



#define TASK0_ID            5      			// �����ID		taskstart
#define TASK0_PRIO          TASK0_ID		// ��������ȼ�
#define TASK0_STACK_SIZE    512     		// �����û���ջ����

#define TASK1_ID            9     			// �����ID 	�ܿ�����
#define TASK1_PRIO          TASK1_ID		// ��������ȼ�
#define TASK1_STACK_SIZE    1024     		// �����û���ջ����

#define TASK2_ID            7				// �����ID		������ͨѶ��������
#define TASK2_PRIO          TASK2_ID		// ��������ȼ�
#define TASK2_STACK_SIZE    1024			// �����û���ջ����

#define TASK3_ID            6				// �����ID		�����������		
#define TASK3_PRIO          TASK3_ID		// ��������ȼ�
#define TASK3_STACK_SIZE    1024			// �����û���ջ����

//#define TASK4_ID            8				// �����ID		com1 gprs
//#define TASK4_PRIO          TASK4_ID		// ��������ȼ�
//#define TASK4_STACK_SIZE    256				// �����û���ջ����




//�豸״̬������Ҫ�ı�����������ӱ���������SysInit����г�ʼ��
typedef struct _DEV_STAT
{
	INT8U err_occured;                      // == TRUE���� ����FALSE�豸����
	INT16U err_code;                        //�������
//  INT8U flash_err;						//FLASH����	��TRUE ����	��FALSE ����

	INT8U allow_upload_data;                //�ɼ���־
	INT8U databox_cmd_rcvd;                 //�Ƿ���յ����ݺе�����
//  INT8U gps_cmd_rcvd;                 	//�Ƿ���յ����ݺе�����

	INT8U purchase_again_req;               //����ˢ��������־����TRUE��������ˢ�� ==FALSE������
	INT8U voice_purchase_again_freq;


	INT8U block9[16];                       //m1��block9����

	INT16U record_number;                   //δ�ϴ��ļ�¼����(ԭp0+4)


	INT8U start;


	INT16U gprs_rec_ptr;                    //GPRS�ϴ���¼ָ��
	INT16U comm_gprs_packno;
	INT16U record_base_ptr;					//δ�ϴ���¼ָ����ʼָ��(ԭp0+2) 
	INT16U last_record_base_ptr;			//�������ɵļ�¼��ʼָ��
	INT16U last_record_number;				//�������ɵļ�¼����
											//
	INT8U line_number[2];					//��·��
	INT8U bus_number[3];					// 3�ֽڳ���	
	INT32U normal_card_fare;				//��ͨ��Ʊ��
	INT32U fare[7];							//������Ʊ��
	INT8U cur_driver_number[6]; 			//����˾����

	INT8U have_been_set;					//�Ƿ�ˢ�����ÿ�
	INT8U driver_is_working;				//���°��־
	INT8U time_modify_times;				//�޸�ʱ�����

	INT16U black_name_number;				//����������

	INT8U typea_para_position;				//A�������רָ��¼�����������FLASH�ĵ�ҳ����Χ0-99
	INT8U NVRAM_cycled;						//NVRAM�д�ż�¼�Ƿ񷭹�ҳ == TRUE ���� else û����

	INT8U param_flag;						//������־λ	

	BUS_TIME update_black_time;				//�ϴθ��º�������ʱ��
	INT8U black_name_version[7];			//�������汾��



	INT16U crc;                             //������CRCУ����
	INT8U end;

	INT16U normal_interval;                 //��ͨ�����ʱ��            //��λ��
//  INT16U favord_interval;					//�Żݿ����ʱ��
	INT8U long_disp_dly;
	INT8U enter_irda_commu;                 //׼���������ͨѶ=TRUE ����=FALSE

	INT8U drec[20];                         //˾����¼ 4�ֽ�˾�����+8�ֽڳ���+7�ֽ�ʱ��+1�ֽ����°��־
	INT8U have_drec;                        //�Ƿ���δ�ϴ���drec����=TRUE ��=FALSE
	INT8U update_time;                      //�Ƿ�׼���������ͬ��ʱ�䣬��=TRUE ��=FALSE
	INT8U normal_gather_over;				//�Ƿ������ɼ���ϱ�־ ��=TRUE ��=FALSE

	INT16U bak0_rec_num;              		//REC_BAK0�ļ�¼����
	INT16U bak1_rec_num;              		//REC_BAK1�ļ�¼����
	INT16U abnormal_rec_num;				//�쳣���ɵļ�¼����

	INT8U new_wireless_address[5];			//��nrf24l01ͨѶ��ַ

	INT8U last_rec[10];						//��һ��ˢ�����׼�¼
	INT8U month_card_valid;					//�Ƿ�����Ʊ������

}DEV_STAT;





//���׼�¼
typedef struct _RECORD
{
	INT8U card_type;                        //��Ƭ����  1�ֽ�
	INT8U card_purchase_type;               //��������  1�ֽ�
	INT8U psam_purchase_serial_num[3];      //PSAM���������3�ֽ�
//  INT8U bus_number[4];					//����	4�ֽ�
//  INT8U card_serial_number[4];			//������ˮ��	8�ֽ�
	INT8U card_number[8];                   //������ˮ��    8�ֽ�
	INT8U balance[3];                       //���׺����    3�ֽ�
	INT8U fare[3];                          //���ѽ��3�ֽ�
	BUS_TIME card_in_time;                  //����ʱ��  7�ֽ�
	INT8U purchase_serial_num[2];           //��ʼ�Ŀ��ڽ������+1  2�ֽ�
	INT8U operator_number[4];               //����Ա��  4�ֽ�

	INT8U line_number[2];                   //��·��    2�ֽ�
	INT8U bus_number[3];                    //����      3�ֽ�
	INT8U driver_number[4];                 //��ʻԱ��  4�ֽ�
	INT8U pos_number[6];                    //PSAM����  6�ֽ�
	INT8U no_use[1];                        //����      1�ֽ�

	INT8U crc[2];                           //crcУ��   2�ֽ�
}RECORD;



/*****************************************************************
 ����ԭ�ͣ�main()
 ����������������
 ����������	
				 
 ����ֵ��	��
 
 ��  �ߣ�	������
 ��  �ڣ�	2013-04-12
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
*****************************************************************/
int main(void);



/*****************************************************************
 ����ԭ�ͣ�SysTick_Handler()
 ����������ucosii��ʱ���ж�
 ����������	
				 
 ����ֵ��	��
 
 ��  �ߣ�	������
 ��  �ڣ�	2013-04-12
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
*****************************************************************/
void SysTick_Handler(void);



/*****************************************************************
 ����ԭ�ͣ�ucosii_timer_init()
 ����������ucosii��ʱ����ʼ��
 ����������	
				 
 ����ֵ��	��
 
 ��  �ߣ�	������
 ��  �ڣ�	2013-04-12
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
*****************************************************************/
void ucosii_timer_init(void);



/*****************************************************************
 ����ԭ�ͣ�task0_start()
 ������������ʼ����
 ����������	
				 
 ����ֵ��	��
 
 ��  �ߣ�	������
 ��  �ڣ�	2013-04-12
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
*****************************************************************/
void task0_start(void *pdata);



/*****************************************************************
 ����ԭ�ͣ�target_init()
 ����������Ӳ����ʼ��
 ����������	
				 
 ����ֵ��	��
 
 ��  �ߣ�	������
 ��  �ڣ�	2013-04-12
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
*****************************************************************/
void target_init(void);



/*****************************************************************
 ����ԭ�ͣ�system_init()
 ����������������ʼ��
 ����������	
				 
 ����ֵ����
 
 ��  �ߣ�������
 ��  �ڣ�2004-09-17
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
*****************************************************************/
void system_init(void);



/*****************************************************************
 ����ԭ�ͣ�system_check()
 ����������ϵͳ�Լ�
 ����������	
				 
 ����ֵ��	��
 
 ��  �ߣ�	������
 ��  �ڣ�	2004-09-17
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
*****************************************************************/
INT8U system_check(void);



/******************************************************************************
 �������ƣ�psam_check
 �������������Psam��
 ����������
 �������ƣ� ����/����� ����        ����

 ��  ��  ֵ��ok(0)-�Լ�ɹ�
				 notok(0xFF)-�Լ�ʧ��

 ��      �ߣ�������
 ��      �ڣ�2004-09-02
 �޸���ʷ��
		����        �޸���      �޸�����
		------      ---------   -------------
******************************************************************************/
INT8U psam_check(void);



/******************************************************************************
 �������ƣ�GetFormatKeySequence
 ����������ȷ������ø�ʽ���������У�ִ�и�ʽ������
 �������ƣ�	����/�����	����		����
 
 ��  ��  ֵ����
 
 ��      ��	��������
 ��      �ڣ�2004-11-25
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void GetFormatKeySequence(void);



/******************************************************************************
 �������ƣ�w25q64_check
 �������������flash w25q64
 ����������
 �������ƣ� ����/����� ����        ����

 ��  ��  ֵ��ok(0)-�Լ�ɹ�
				 notok(0xFF)-�Լ�ʧ��

 ��      �ߣ�������
 ��      �ڣ�2004-09-02
 �޸���ʷ��
		����        �޸���      �޸�����
		------      ---------   -------------
******************************************************************************/
INT8U w25q64_check(void);



/*****************************************************************
����ԭ�ͣ�Lcd_Draw_SelfChk_Err_Result
����������LCD��ʾ�豸�쳣
		// 
		//		�豸�쳣
		//		�������
		//		  2004-03-01
����������
�������ƣ�	����/�����	����		����
-----------		-----------	------	   	-------

��  ��  ֵ����
��      ��	��������
��      �ڣ�2004-03-01
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
void Lcd_Draw_SelfChk_Err_Result(void);



/******************************************************************************
 �������ƣ�get_psam1_info
 ����������ȡ��psam1��Ϣ
 �������ƣ�	����/�����	����		����
 
 ��  ��  ֵ��ok(0)-�ɹ�
				 notok(0xFF)-ʧ��
 
 ��      ��	��������
 ��      �ڣ�2004-09-02
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
INT8U get_psam1_info(void);



/******************************************************************************
 �������ƣ�get_psam2_info
 ����������ȡ��psam2��Ϣ
 �������ƣ�	����/�����	����		����
 
 ��  ��  ֵ��ok(0)-�ɹ�
				 notok(0xFF)-ʧ��
 
 ��      ��	��������
 ��      �ڣ�2004-09-02
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
INT8U get_psam2_info(void);



/*********************************************************************************************************
** ��������: taskstart creates other task
** ��������: ��COS-II������������
** �䡡��: ��
** �䡡��: ��
** ȫ�ֱ���: ��
** ����ģ��: 
**
** ������: ������
** �ա���: 2004��12��28��
**-------------------------------------------------------------------------------------------------------
** �޸���: 
** �ա���: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void TaskStart_CreateTasks(void);



/******************************************************************************
 �������ƣ�init_param
 ������������ʼ������
 ����������
 �������ƣ�	����/�����	����		����
				
 ��  ��  ֵ����

 ��      ��	��������
 ��      �ڣ�2004-09-02
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void init_param(void);














#ifdef __cplusplus
	}
#endif
#endif
/*******************************************************************************************
                              End Of File
*******************************************************************************************/
