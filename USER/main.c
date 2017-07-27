/******************************************************************************
Copyright 2017 大连易达通电子有限公司
All rights riserved.

文件名	：main.c
模块名称：溶氧检测主程序
功能概要：
			
 
当前版本：0.0.2
修改人	：刘及华
完成日期：2017.01.30
升级说明： 
1.增加一个触摸屏接口，5P端子，RS232接口，控制触摸屏。
2.增加一个检测小水泵的电压互感器电路，检测水泵是否工作。
 
当前版本：0.0.1
修改人	：刘及华
完成日期：2016.12.11
升级说明：create
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
 函数原型：main()
 功能描述：主函数
 参数描述：	
				 
 返回值：	无
 
 作  者：	刘及华
 日  期：	2013-04-12
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
*****************************************************************/
int main(void)
 {
	setlocale(LC_ALL, "C");         //设置本地区域

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
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
 函数原型：SysTick_Handler()
 功能描述：ucosii定时器中断
 参数描述：	
				 
 返回值：	无
 
 作  者：	刘及华
 日  期：	2013-04-12
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
*****************************************************************/
void SysTick_Handler(void)
{
	OSIntEnter();       //进入中断
	OSTimeTick();       //调用ucos的时钟服务程序
	OSIntExit();        //触发任务切换软中断
}

/*****************************************************************
 函数原型：ucosii_timer_init()
 功能描述：ucosii定时器初始化
 参数描述：	
				 
 返回值：	无
 
 作  者：	刘及华
 日  期：	2013-04-12
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
*****************************************************************/
void ucosii_timer_init(void)
{
	//  u8  fac_us = 0; //us延时倍乘数
	//  u16 fac_ms = 0; //ms延时倍乘数
	u32 reload;

	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);   //选择外部时钟  HCLK/8
															//  fac_us = SystemCoreClock / 8000000; //为系统时钟的1/8


	reload = SystemCoreClock / 8000000;     //每秒钟的计数次数 单位为K
	reload *= 1000000 / OS_TICKS_PER_SEC; //根据OS_TICKS_PER_SEC设定溢出时间
										  //  reload为24位寄存器,最大值:16777216,在72M下,约合1.86s左右
										  //  fac_ms = 1000 / OS_TICKS_PER_SEC; //代表ucos可以延时的最少单位
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;    //开启SYSTICK中断
	SysTick->LOAD = reload;   //每1/OS_TICKS_PER_SEC秒中断一次
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;     //开启SYSTICK
}

/*****************************************************************
 函数原型：task0_start()
 功能描述：开始任务
 参数描述：	
				 
 返回值：	无
 
 作  者：	刘及华
 日  期：	2013-04-12
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
*****************************************************************/
void task0_start(void *pdata)
{
	INT8U i = 0;

	pdata = pdata;

	OSStatInit();                   //初始化统计任务.这里会延时1秒钟左右

	target_init();                  //目标班初始化

	SetLight(BOTH_LIGHT);           //LED灯全灭

	Beeperr(); 
	//SetLight(NEITHER_LIGHT);        //LED灯全灭

	system_init();                          //初始化

	i = system_check();                     //系统自检
	if ( i != ok )        //自检失败处理
	{
		//设备异常
		SetLight(RED_LIGHT);
	}
	else
	{
		SetLight(GREEN_LIGHT);
	}

	TaskStart_CreateTasks();                //创建其他任务

	OSTaskSuspend(OS_PRIO_SELF);            //挂起起始任务.

	for (;;)
	{
		resetpos();
	}
}

/*****************************************************************
 函数原型：target_init()
 功能描述：硬件初始化
 参数描述：	
				 
 返回值：	无
 
 作  者：	刘及华
 日  期：	2013-04-12
 修改历史：
		日期		修改人		修改描述
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
	//	AT24CXX_Init();			//IIC初始化
}

/*****************************************************************
 函数原型：system_init()
 功能描述：变量初始化
 参数描述：

 返回值：无

 作  者：刘及华
 日  期：2004-09-17
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
*****************************************************************/
void system_init(void)
{
	//	INT8U i = 0;
	//	INT8U key = NO_KEY;
	//	INT8U buf[50];


	//DevStat初始化
	memset((void *)&DevStat, 0x00, sizeof(DevStat));
	//	read_param();

	DevStat.err_occured = FALSE;
	DevStat.err_code = 0x0000;

	DevStat.allow_upload_data = FALSE;
	DevStat.databox_cmd_rcvd = FALSE;
	DevStat.purchase_again_req = FALSE;
	DevStat.voice_purchase_again_freq = 0;
	DevStat.normal_interval = 60;


	devstat_sem = OSSemCreate(1);               /* 信号量用于互斥操作总线 */
	com_shell_send_sem = OSSemCreate(1);        /* 信号量用于互斥操作总线 */
	com_shell_cmd_sem = OSSemCreate(1);         /* 信号量用于互斥操作总线 */

	/*清空串口接收缓冲区*/
	queue_reset();
	;
	//	i = ReadSwitch();
	//	DevStat.bus_number[0] = i / 100;
	//	DevStat.bus_number[1] = i % 100 / 10;
	//	DevStat.bus_number[2] = i % 10;

}

/*****************************************************************
 函数原型：system_check()
 功能描述：系统自检
 参数描述：

 返回值：	无

 作  者：	刘及华
 日  期：	2004-09-17
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
*****************************************************************/
INT8U system_check(void)
{
	//	INT8U i = 0;
	//	char buf[50];
	//	BUS_TIME time;

	toggle_wd();

	//	DevStat.allow_upload_data = 0;
	// 	if(!AT24CXX_Check()) 				//检测到了24c04
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
** 函数名称: taskstart creates other task
** 功能描述: μCOS-II建立其它任务
** 输　入: 无
** 输　出: 无
** 全局变量: 无
** 调用模块:
**
** 作　者: 刘及华
** 日　期: 2004年12月28日
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void TaskStart_CreateTasks(void)
{

	ToggleWD();


	//建立总控制任务
	(void)OSTaskCreateExt(task_general_ctrl,
						  (void *)0,
						  &TASK1_STACK[TASK1_STACK_SIZE - 1],
						  TASK1_PRIO,
						  TASK1_ID,
						  &TASK1_STACK[0],
						  TASK1_STACK_SIZE,
						  (void *)0,
						  OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

	//溶氧检测任务
//	(void)OSTaskCreateExt(task_dissolve_oxygen,
//							(void *)0,
//							&TASK3_STACK[TASK3_STACK_SIZE-1],
//							TASK3_PRIO,
//							TASK3_ID,
//							&TASK3_STACK[0],
//							TASK3_STACK_SIZE,
//							(void *)0,
//							OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

	//触摸屏任务
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


