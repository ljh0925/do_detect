/******************************************************************************
Copyright 2017 �����״�ͨ�������޹�˾
All rights riserved.

�ļ���	��main.c
ģ�����ƣ��������������
���ܸ�Ҫ��
			
 
��ǰ�汾��0.0.2
�޸���	��������
������ڣ�2017.01.30
����˵���� 
1.����һ���������ӿڣ�5P���ӣ�RS232�ӿڣ����ƴ�������
2.����һ�����Сˮ�õĵ�ѹ��������·�����ˮ���Ƿ�����
 
��ǰ�汾��0.0.1
�޸���	��������
������ڣ�2016.12.11
����˵����create
******************************************************************************/
#include "all.h"
#include "sys.h"


__align(8) OS_STK TASK0_STACK[TASK0_STACK_SIZE];
__align(8) OS_STK TASK1_STACK[TASK1_STACK_SIZE];
__align(8) OS_STK TASK2_STACK[TASK2_STACK_SIZE];
__align(8) OS_STK TASK3_STACK[TASK3_STACK_SIZE];
//__align(8) OS_STK  TASK4_STACK[TASK4_STACK_SIZE];


volatile DEV_STAT DevStat;
const INT8U VERSION[] = { "0.01" };
extern INT8U BLACK_LENTH;



OS_EVENT *devstat_sem;
OS_EVENT *com_shell_send_sem;
OS_EVENT *com_shell_cmd_sem;
OS_EVENT *log_buf_sem;

//INT8U fbuf[4096];
//char log_buf[1000];

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
int main(void)
 {
	setlocale(LC_ALL, "C");         //���ñ�������

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	ucosii_timer_init();

	OSInit();

	OSTaskCreateExt(task0_start,
					(void *)0,
					&TASK0_STACK[TASK0_STACK_SIZE - 1],
					TASK0_PRIO,
					TASK0_ID,
					&TASK0_STACK[0],
					TASK0_STACK_SIZE,
					(void *)0,
					OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

	OSStart();
}

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
void SysTick_Handler(void)
{
	OSIntEnter();       //�����ж�
	OSTimeTick();       //����ucos��ʱ�ӷ������
	OSIntExit();        //���������л����ж�
}

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
void ucosii_timer_init(void)
{
	//  u8  fac_us = 0; //us��ʱ������
	//  u16 fac_ms = 0; //ms��ʱ������
	u32 reload;

	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);   //ѡ���ⲿʱ��  HCLK/8
															//  fac_us = SystemCoreClock / 8000000; //Ϊϵͳʱ�ӵ�1/8


	reload = SystemCoreClock / 8000000;     //ÿ���ӵļ������� ��λΪK
	reload *= 1000000 / OS_TICKS_PER_SEC; //����OS_TICKS_PER_SEC�趨���ʱ��
										  //  reloadΪ24λ�Ĵ���,���ֵ:16777216,��72M��,Լ��1.86s����
										  //  fac_ms = 1000 / OS_TICKS_PER_SEC; //����ucos������ʱ�����ٵ�λ
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;    //����SYSTICK�ж�
	SysTick->LOAD = reload;   //ÿ1/OS_TICKS_PER_SEC���ж�һ��
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;     //����SYSTICK
}

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
void task0_start(void *pdata)
{
	INT8U i = 0;

	pdata = pdata;

	OSStatInit();                   //��ʼ��ͳ������.�������ʱ1��������

	target_init();                  //Ŀ����ʼ��

	SetLight(BOTH_LIGHT);           //LED��ȫ��

	Beeperr(); 
	//SetLight(NEITHER_LIGHT);        //LED��ȫ��

	system_init();                          //��ʼ��

	i = system_check();                     //ϵͳ�Լ�
	if ( i != ok )        //�Լ�ʧ�ܴ���
	{
		//�豸�쳣
		SetLight(RED_LIGHT);
	}
	else
	{
		SetLight(GREEN_LIGHT);
	}

	TaskStart_CreateTasks();                //������������

	OSTaskSuspend(OS_PRIO_SELF);            //������ʼ����.

	for (;;)
	{
		resetpos();
	}
}

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
void target_init(void)
{
	wdt_init();
	led_init();
	//	rc531_ctrlio_init();
	//	uart1_init();
	UartInit(115200);
	DS18B20_Init();
//	Adc_Init();
//	i2c_init();
	IIC_Init();
	//	rtc_init();
	//	buzzer_init();
//	door_init();
//	ads1110_init();
	//	switch_init();
	//	AT24CXX_Init();			//IIC��ʼ��
}

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
void system_init(void)
{
	//	INT8U i = 0;
	//	INT8U key = NO_KEY;
	//	INT8U buf[50];


	//DevStat��ʼ��
	memset((void *)&DevStat, 0x00, sizeof(DevStat));
	//	read_param();

	DevStat.err_occured = FALSE;
	DevStat.err_code = 0x0000;

	DevStat.allow_upload_data = FALSE;
	DevStat.databox_cmd_rcvd = FALSE;
	DevStat.purchase_again_req = FALSE;
	DevStat.voice_purchase_again_freq = 0;
	DevStat.normal_interval = 60;


	devstat_sem = OSSemCreate(1);               /* �ź������ڻ���������� */
	com_shell_send_sem = OSSemCreate(1);        /* �ź������ڻ���������� */
	com_shell_cmd_sem = OSSemCreate(1);         /* �ź������ڻ���������� */

	/*��մ��ڽ��ջ�����*/
	queue_reset();
	;
	//	i = ReadSwitch();
	//	DevStat.bus_number[0] = i / 100;
	//	DevStat.bus_number[1] = i % 100 / 10;
	//	DevStat.bus_number[2] = i % 10;

}

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
INT8U system_check(void)
{
	//	INT8U i = 0;
	//	char buf[50];
	//	BUS_TIME time;

	toggle_wd();

	//	DevStat.allow_upload_data = 0;
	// 	if(!AT24CXX_Check()) 				//��⵽��24c04
	//	{
	//		DevStat.allow_upload_data = 1;
	//		BLACK_LENTH = AT24CXX_ReadOneByte(254);
	//	}

	if ( DevStat.err_occured == TRUE )
		return ( notok );
	else
		return ( ok );
}

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
void TaskStart_CreateTasks(void)
{

	ToggleWD();


	//�����ܿ�������
	(void)OSTaskCreateExt(task_general_ctrl,
						  (void *)0,
						  &TASK1_STACK[TASK1_STACK_SIZE - 1],
						  TASK1_PRIO,
						  TASK1_ID,
						  &TASK1_STACK[0],
						  TASK1_STACK_SIZE,
						  (void *)0,
						  OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

	//�����������
//	(void)OSTaskCreateExt(task_dissolve_oxygen,
//							(void *)0,
//							&TASK3_STACK[TASK3_STACK_SIZE-1],
//							TASK3_PRIO,
//							TASK3_ID,
//							&TASK3_STACK[0],
//							TASK3_STACK_SIZE,
//							(void *)0,
//							OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

	//����������
	(void)OSTaskCreateExt(task_touch_UI,
						  (void *)0,
						  &TASK2_STACK[TASK2_STACK_SIZE - 1],
						  TASK2_PRIO,
						  TASK2_ID,
						  &TASK2_STACK[0],
						  TASK2_STACK_SIZE,
						  (void *)0,
						  OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

//	(void)OSTaskCreateExt(task_shell,
//						(void *)0,
//						&TASK2_STACK[TASK2_STACK_SIZE-1],
//						TASK2_PRIO,
//						TASK2_ID,
//						&TASK2_STACK[0],
//						TASK2_STACK_SIZE,
//						(void *)0,
//						OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
}


