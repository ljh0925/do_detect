/******************************************************************************
Copyright 2016 大连易达通电子有限公司
All rights riserved.

文件名	：task_shell.c
模块名称：通讯任务 人机界面
功能概要：通讯任务 人机界面

取代版本：0.0.1
修改人	：刘及华
完成日期：2005.04.06
升级说明：create

******************************************************************************/
#include 		"all.h"


//OS_STK  TASK4_STACK[TASK4_STACK_SIZE];

extern volatile DEV_STAT DevStat;

#define MAX_COMMAND_NUM    9

typedef struct
{
	INT16S num;
	INT8S *name;
	INT8U (*CommandFunc)(INT8U argc,INT8S **argv);
}command;

command ShellComms[MAX_COMMAND_NUM];

#define MAX_LEN_CMD_BUF 50

#define ENTR 0x0D
#define ERRORCOMMAND    notok


static INT8S *argv[20];
static INT8U argc;

//INT8U fpage_buf[512];
extern const INT8U VERSION[];

extern OS_EVENT        *devstat_sem;
extern OS_EVENT        *com_shell_send_sem;
extern OS_EVENT        *com_shell_cmd_sem;

//extern char log_buf[3000];
extern char log_buf[1000];
extern OS_EVENT        *log_buf_sem;


/******************************************************************************
移植需要改写的
******************************************************************************/
//extern INT8U Com4SndBuf[UART4_SEND_QUEUE_LENGTH];              //串口发送缓冲区
//extern INT8U Com4RcvBuf[UART4_RCV_QUEUE_LENGTH];               //串口接收缓冲区
//
//extern int UART4RcvBuf[(UART4_RCV_QUEUE_LENGTH + sizeof(int) - 1) / sizeof(int)];
//
//INT8U *p_arm_snd_buf = Com4SndBuf;
//INT8U *p_arm_rcv_buf = Com4RcvBuf;
//
///******************************************************************************
// 函数名称：armputch
// 功能描述：arm发送一个字节数据
// 参数描述：
// 参数名称： 输入/输出？ 类型		描述
// dat		输入		INT8U		要发送的数据
//
// 返  回  值：无
//
// 作 	 者 ：刘及华
// 日 	 期：2012-04-06
// 修改历史：
//		日期		修改人		修改描述
//		------		---------	-------------
//******************************************************************************/
//void armputch(INT8U dat)
//{
//	INT8U err = 0;
//	OSSemPend(com_shell_send_sem, 0, &err);
//	uart4_putch(dat);
//	(void)OSSemPost(com_shell_send_sem);
//}
//
///******************************************************************************
// 函数名称：armputs
// 功能描述：arm输出函数
// 参数描述：
// 参数名称： 输入/输出？ 类型		描述
// snd_buf	输入			void *		要发送的字符串
//
// 返  回  值：无
//
// 作 	 者 ：刘及华
// 日 	 期：2005-04-07
// 修改历史：
//		日期		修改人		修改描述
//		------		---------	-------------
//******************************************************************************/
//void armputs(const void * snd_buf)
//{
//	INT8U err = 0;
//	OSSemPend(com_shell_send_sem, 0, &err);
//	com4_send_hex((void *)snd_buf, (INT16U)strlen(snd_buf));
//	(void)OSSemPost(com_shell_send_sem);
//}
//
///******************************************************************************
// 函数名称：arm_send_hex
// 功能描述：arm发送一串数据
// 参数描述：
// 参数名称： 输入/输出？ 类型		描述
// str_to_send	输入	INT8U *		要发送的字符串
// len			输入	INT16U		长度
//
// 返  回  值：无
//
// 作 	 者 ：刘及华
// 日 	 期：2005-04-07
// 修改历史：
//		日期		修改人		修改描述
//		------		---------	-------------
//******************************************************************************/
//void arm_send_hex(INT8U *str_to_send, INT16U len)
//{
//	INT8U err = 0;
//	OSSemPend(com_shell_send_sem, 0, &err);
//	com4_send_hex(str_to_send, len);
//	(void)OSSemPost(com_shell_send_sem);
//}
//
///*****************************************************************
//函数原型：armgetch
//功能描述：arm接收一个字节
//参数描述：
//参数名称：	输入/输出？	类型		描述
//-----------		-----------	------	   	-------
//
//返  回  值：串口得到的数
//作      者：刘及华
//日      期：2005-08-20
//修改历史：
//日期		修改人		修改描述
//------		---------	-------------
//*****************************************************************/
//INT8U armgetch(void)
//{
//	INT8U i = 0;
////  INT16U len = 0;
////  INT16U timeout_cntr = 0;
//
//	//第一个字节
//	for ( ;; )
//	{
//		toggle_wd();
//
//		if ( QueueRead(&i, UART4RcvBuf) == QUEUE_OK )
//		{
//			return i;
//		}
//		else
//		{
//			OSTimeDlyHMSM(0, 0, 0, 5);
//		}
//	}
//}
//
///******************************************************************************
// 函数名称：arm_buf_flush
// 功能描述：清空arm串口缓存
// 参数描述：
// 参数名称： 输入/输出？ 类型		描述
//
// 返  回  值：无
//
// 作 	 者 ：刘及华
// 日 	 期：2005-04-11
// 修改历史：
//		日期		修改人		修改描述
//		------		---------	-------------
//******************************************************************************/
//void arm_buf_flush(void)
//{
//	uart4_buf_flush();
//}
//
///*****************************************************************
//函数原型：arm_gets_with_keyboard
//功能描述：arm串口接收，允许按键取消
//参数描述：
//参数名称：		输入/输出？	类型		描述
//-----------		-----------	------	   	-------
//str_got			输出		INT8U*		得到的字符串
//timeout_start	输入		INT32U		首字超时，单位mS，0死等
//timeout			输入		INT16U		接收字节间隔超时，单位mS
//
// 返  回  值：ok(0)-成功
//			KEY_CANCEL-按退出键了
//			notok(0xFF)-失败，超时
//作      者	：刘及华
//日      期：2005-08-20
//修改历史：
//日期		修改人		修改描述
//------		---------	-------------
//*****************************************************************/
//INT8U arm_gets_with_keyboard(INT8U *str_got, INT16U *rcv_len, INT32U timeout_start, INT16U timeout)
//{
//	INT8U i = 0;
//
//	i = com4_gets(str_got, rcv_len, timeout_start, timeout);
//	return i;
//}
//
///******************************************************************************
// 函数名称：xprint
// 功能描述：相当于print，通过串口发送数据
// 参数描述：
// 参数名称： 输入/输出？ 类型		描述
//
// 返  回  值：无
//
// 作 	 者 ：刘及华
// 日 	 期：2005-04-07
// 修改历史：
//		日期		修改人		修改描述
//		------		---------	-------------
//******************************************************************************/
//void xprint(char * fmt, ...)
//{
//	INT8U err = 0;
//	va_list ap;
//
//	va_start(ap, fmt);
//	OSSemPend(com_shell_cmd_sem, 0, &err);
//	OSSemPend(com_shell_send_sem, 0, &err);
//
//	vsprintf((void *)p_arm_snd_buf, fmt, ap);
//	com4_send_hex(p_arm_snd_buf, (INT16U)strlen((char *)p_arm_snd_buf));
//
//	(void)OSSemPost(com_shell_cmd_sem);
//	(void)OSSemPost(com_shell_send_sem);
//}

extern INT8U Com1SndBuf[UART1_SEND_QUEUE_LENGTH];              //串口发送缓冲区
extern INT8U Com1RcvBuf[UART1_RCV_QUEUE_LENGTH];               //串口接收缓冲区

extern int UART1RcvBuf[(UART1_RCV_QUEUE_LENGTH + sizeof(int) - 1) / sizeof(int)];

INT8U *p_arm_snd_buf = Com1SndBuf;
INT8U *p_arm_rcv_buf = Com1RcvBuf;

/******************************************************************************
 函数名称：armputch
 功能描述：arm发送一个字节数据
 参数描述：
 参数名称： 输入/输出？ 类型		描述
 dat		输入		INT8U		要发送的数据
				
 返  回  值：无
				   
 作 	 者 ：刘及华
 日 	 期：2012-04-06
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void armputch(INT8U dat)
{
	INT8U err = 0;
	OSSemPend(com_shell_send_sem, 0, &err);
	uart1_putch(dat);
	(void)OSSemPost(com_shell_send_sem);
}

/******************************************************************************
 函数名称：armputs
 功能描述：arm输出函数
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
void armputs(const void * snd_buf)
{
	INT8U err = 0;
	OSSemPend(com_shell_send_sem, 0, &err);
	com1_send_hex((void *)snd_buf, (INT16U)strlen(snd_buf));
	(void)OSSemPost(com_shell_send_sem);
}

/******************************************************************************
 函数名称：arm_send_hex
 功能描述：arm发送一串数据
 参数描述：
 参数名称： 输入/输出？ 类型		描述
 str_to_send	输入	INT8U *		要发送的字符串
 len			输入	INT16U		长度
				
 返  回  值：无
				   
 作 	 者 ：刘及华
 日 	 期：2005-04-07
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void arm_send_hex(INT8U *str_to_send, INT16U len)
{
	INT8U err = 0;
	OSSemPend(com_shell_send_sem, 0, &err);
	com1_send_hex(str_to_send, len);
	(void)OSSemPost(com_shell_send_sem);
}

/*****************************************************************
函数原型：armgetch
功能描述：arm接收一个字节
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------

返  回  值：串口得到的数
作      者：刘及华
日      期：2005-08-20
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U armgetch(void)
{
	INT8U i = 0;
//  INT16U len = 0;
//  INT16U timeout_cntr = 0;

	//第一个字节
	for ( ;; )
	{
		toggle_wd();

		if ( QueueRead(&i, UART1RcvBuf) == QUEUE_OK )
		{
			return i;
		}
		else
		{
			OSTimeDlyHMSM(0, 0, 0, 5);
		}
	}
}

/******************************************************************************
 函数名称：arm_buf_flush
 功能描述：清空arm串口缓存
 参数描述：
 参数名称： 输入/输出？ 类型		描述

 返  回  值：无

 作 	 者 ：刘及华
 日 	 期：2005-04-11
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void arm_buf_flush(void)
{
	uart1_buf_flush();
}

/*****************************************************************
函数原型：arm_gets_with_keyboard
功能描述：arm串口接收，允许按键取消
参数描述：
参数名称：		输入/输出？	类型		描述
-----------		-----------	------	   	-------
str_got			输出		INT8U*		得到的字符串
timeout_start	输入		INT32U		首字超时，单位mS，0死等
timeout			输入		INT16U		接收字节间隔超时，单位mS

 返  回  值：ok(0)-成功
			KEY_CANCEL-按退出键了
			notok(0xFF)-失败，超时
作      者	：刘及华
日      期：2005-08-20
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U arm_gets_with_keyboard(INT8U *str_got, INT16U *rcv_len, INT32U timeout_start, INT16U timeout)
{
	INT8U i = 0;

	i = com1_gets(str_got, rcv_len, timeout_start, timeout);
	return i;
}

/******************************************************************************
 函数名称：xprint
 功能描述：相当于print，通过串口发送数据
 参数描述：
 参数名称： 输入/输出？ 类型		描述
				
 返  回  值：无
				   
 作 	 者 ：刘及华
 日 	 期：2005-04-07
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
//void xprint(char * fmt, ...)
void xprint(void * fmt, ...)
{
//	INT8U err = 0;
//	va_list ap;
//
//	va_start(ap, fmt);
//	OSSemPend(com_shell_send_sem, 0, &err);
//
//	vsprintf((void *)p_arm_snd_buf, fmt, ap);
//	com5_send_hex(p_arm_snd_buf, (INT16U)strlen((char *)p_arm_snd_buf));
//
//	(void)OSSemPost(com_shell_send_sem);
	INT8U err = 0;
	va_list ap;
	_TIME time;
	char time_buf[30];

	va_start(ap, fmt);
	OSSemPend(com_shell_send_sem, 0, &err);

	vsprintf((void *)p_arm_snd_buf, fmt, ap);
	com1_send_hex(p_arm_snd_buf, (INT16U)strlen((char *)p_arm_snd_buf));


	//保存log ver0.0.9
	time_read(&time);
//	2015-10-24 10:35:20
//	sprintf(time_buf, "%02x%02x-%02x-%02x %02x:%02x:%02x ",
//			time.century, time.year, time.month, time.day, time.hour, time.minute, time.second);
	sprintf(time_buf, "%02x:%02x:%02x ", time.hour, time.minute, time.second);			//只显示时分秒
	OSSemPend(log_buf_sem, 0, &err);
	if ( strlen(log_buf) + strlen(time_buf) < sizeof(log_buf) )
	{
		strcat(log_buf, time_buf);
	}
	if ( strlen(log_buf) + strlen((char *)p_arm_snd_buf) < sizeof(log_buf) )
	{
		strcat(log_buf, (char *)p_arm_snd_buf);
	}
	(void)OSSemPost(log_buf_sem);


	(void)OSSemPost(com_shell_send_sem);
}


/******************************************************************************
 函数名称：task_shell
 功能描述：人机界面
 参数描述：
 参数名称： 输入/输出？ 类型		描述
				
 返  回  值：无
				   
 作 	 者：刘及华
 日 	 期：2010-06-18
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void task_shell(void * data)
{
	INT8U i = 0;
//  INT8U passwd_failed_times = 0;
	INT8U num;
//  INT8U login_success = FALSE;
	INT8S ch;
	INT8U (*Func)(INT8U argc,INT8S **argv);
	INT8S cmdbuf[MAX_LEN_CMD_BUF + 1];
	INT8U err = 0;


	data = data;

	InitCommands();
	cmdbuf[0] = '\0';

//	armputs("\r\nWelcome to xuyan's shell.\n");
	armputs("\r\n欢迎!!!\r\n输入info查看信息参数.");

	armputs("\r\n#");									//显示提示符
	i = 0;

	for ( ;; )
	{
		do
		{				  //only accept a-z,0-9,A-Z,.,space,/,-
			ch = armgetch();
		}while ( !((ch>='0'&&ch<='9')||(ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z')||(ch=='.')||(ch==' ')||(ch=='-')||(ch=='/')||(ch==ENTR)||(ch=='\b')||(ch==',')) );

		switch ( ch )
		{
			case ENTR:				//enter
				if ( i==0 )
				{							   //commandbuf is null,begin a new line
					armputs("\r\n#");
				}
				else
				{
					if ( cmdbuf[i-1]==' ' )
						i--;			//get rid of the end space
					cmdbuf[i] = '\0';
					num = CommandAnalys(cmdbuf);	//analys the argv in the commandbuf
					if ( num == ERRORCOMMAND )
					{			   //error or none exist command
						i = 0;
						cmdbuf[i] = '\0';
						armputs("\r\nWrond command.");
						armputs("\r\n#");
					}
					else
					{
						Func = ShellComms[num].CommandFunc;	//call corresponding CommandFunc
//						OSSemPend(com_shell_cmd_sem, 0, &err);
						OSSemPend(devstat_sem, 0, &err);
						Func(argc, argv);
						(void)OSSemPost(devstat_sem);
//						(void)OSSemPost(com_shell_cmd_sem);
						i = 0;
						cmdbuf[i] = '\0';
						armputs("\r\n#");
					}
				}
				break;

			case '\b':				//backspace
				if ( i==0 )
				{		 //has backed to first one
					//do nothing
				}
				else
				{
					i--;			//pointer back once
//  				armputch('\b');		//cursor back once
//  				armputch(' ');		  //earse last char in screen
//  				armputch('\b');		//cursor back again
					armputs("\b \b");
				}
				break;

			case ' ':				//don't allow continuous or begin space(' ')
				if ( (cmdbuf[i-1] == ' ')||(i==0)||(i>MAX_LEN_CMD_BUF - 2) )
				{
					//do nothing
				}
				else
				{
					cmdbuf[i] = ch;
					i++;
					armputch(ch);  //display and store ch
				}
				break;

			default:				//normal key
				if ( i > MAX_LEN_CMD_BUF - 2 )
				{	 //the buf reached MAX 
					//do nothing
				}
				else
				{
					cmdbuf[i] = ch;
					i++;
					armputch(ch);  //display and store ch
				}
				break;
		}  //switch
	}

}


/******************************************************************************
 函数名称：CommandAnalys
 功能描述：命令解析函数
 参数描述：
 参数名称： 输入/输出？ 类型		描述
 Buf			输入			INT8S *		接收到的字符串（要进行命令解析的字符串）
				
 返  回  值：无
				   
 作 	 者 ：刘及华
 日 	 期：2005-04-25
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
INT8U CommandAnalys(INT8S *Buf)
{
	INT8U i;
	INT8U pointer;
	INT8U num;
	INT8S name[20];		//command name length <20

//	ToggleWD();

	argc = 0;			   //argc is global
	pointer = 0;
	num = 0;
	armputs("\r\n");

	while ( (Buf[pointer]!=' ') && (Buf[pointer]!='\0') && pointer<20 )
	{
		name[pointer]=Buf[pointer];
		pointer++;
	}
	name[pointer] = '\0';	//now got the command name, and pointer is to the first space in the Buf

	for ( i=0;i<MAX_COMMAND_NUM;i++ )
	{
		if ( !strcmp((void *)name, (void *)ShellComms[i].name) )
		{
			num = i;
			break;
		}				//find the command number
	}                   
	//not find it
	if ( i >= MAX_COMMAND_NUM ) 
		return ERRORCOMMAND;

	while ( Buf[pointer]!='\0' )
	{
		if ( Buf[pointer]==' ' )
		{
			if ( argc>0 )
			{
				Buf[pointer] = '\0';			//end of last argv
			}
			pointer++;
			argv[argc] = &Buf[pointer];			//add a parameter for every space
			argc++;
		}
		else
		{
			pointer++;
		}
	}//while

	return num;
}

/******************************************************************************
 函数名称：InitCommands
 功能描述：命令初始化
 参数描述：
 参数名称： 输入/输出？ 类型		描述
				
 返  回  值：无
				   
 作 	 者 ：刘及华
 日 	 期：2005-04-07
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
INT8U InitCommands(void)
{
	INT8U i = 0;

//	ToggleWD();

	for ( i=0; i<MAX_COMMAND_NUM; i++ )
	{
		ShellComms[i].name = NULL;
	}

	ShellComms[0].num = 0;
	ShellComms[0].name = (INT8S *)"help";
	ShellComms[0].CommandFunc = HelpFunc;

	ShellComms[1].num = 1;
	ShellComms[1].name = (INT8S *)"flp";
	ShellComms[1].CommandFunc = FLPageFunc0;

	ShellComms[2].num = 2;
	ShellComms[2].name = (INT8S *)"info";
	ShellComms[2].CommandFunc = info_func;

	ShellComms[3].num = 3;
	ShellComms[3].name = (INT8S *)"format";
	ShellComms[3].CommandFunc = FormatFunc;

	ShellComms[4].num = 4;
	ShellComms[4].name = (INT8S *)"chkstk";
	ShellComms[4].CommandFunc = ChkStkFunc;

	ShellComms[5].num = 5;
	ShellComms[5].name = (INT8S *)"set";
	ShellComms[5].CommandFunc = set_func;

	ShellComms[6].num = 6;
	ShellComms[6].name = (INT8S *)"upload";
	ShellComms[6].CommandFunc = upload_func;

//  ShellComms[7].num = 7;
//  ShellComms[7].name = (INT8S *)"flpage";
//  ShellComms[7].CommandFunc = FLPageFunc;
//
//  ShellComms[8].num = 8;
//  ShellComms[8].name = (INT8S *)"chkstk";
//  ShellComms[8].CommandFunc = ChkStkFunc;
//
//  ShellComms[9].num = 9;
//  ShellComms[9].name = (INT8S *)"eraseflash";
//  ShellComms[9].CommandFunc = EraseFlashFunc;

	return 0;
}

/******************************************************************************
 函数名称：HelpFunc
 功能描述：显示帮助
 参数名称： 输入/输出？ 类型        描述
 argc		输入		INT8U 			参数数量
 argv		输入		INT8S**		指向参数的指针

 返  回  值：ok(0)-成功
				 notok(0xFF)-失败

 作      者 ：刘及华
 日      期：2005-05-08
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
INT8U HelpFunc(INT8U argc, INT8S **argv)
{
	INT8U i = 0;

//	ToggleWD();

	armputs("\r\n");
	armputs("all commands:\r\n");
	for ( i=0; i<MAX_COMMAND_NUM; i++ )
	{
		if ( ShellComms[i].name != NULL )
		{
			armputs(ShellComms[i].name);
			armputs("\r\n");
		}
	}

	return ok;
}

//INT8U HelloFunc(INT8U argc,INT8S **argv)
//{
//	INT16S i;
//
////	ToggleWD();
//
//	armputs("\r\nHello! I am xuyan's shell");
//	armputs("\r\nyour argv is :");
//	if ( argc>0 )
//	{
//		for ( i=0;i<argc;i++ )
//		{
//			armputs("\r\n");
//			armputs(argv[i]);
//		}
//	}
//	return ok;
//}

/******************************************************************************
 函数名称：info_func
 功能描述：显示参数
 参数名称： 输入/输出？ 类型        描述
 argc       输入        INT8U           参数数量
 argv       输入        INT8S**     指向参数的指针

 返  回  值：ok(0)-成功
                 notok(0xFF)-失败

 作      者 ：刘及华
 日      期：2005-05-08
 修改历史：
        日期        修改人      修改描述
        ------      ---------   -------------
******************************************************************************/
INT8U info_func(INT8U argc, INT8S **argv)
{
//    INT8S buf[100];
	INT8U i = 0;
	INT8U j = 0;
	BUS_TIME time;

    ToggleWD();

    armputs("\r\n");

	j = ReadSwitch();
	DevStat.bus_number[0] = j / 100;
	DevStat.bus_number[1] = j % 100 / 10;	 
	DevStat.bus_number[2] = j % 10;


	sprintf((void *)p_arm_snd_buf, "版本号: %s\r\n", VERSION);
    armputs(p_arm_snd_buf);

    sprintf((void *)p_arm_snd_buf, "终端编号(id): %06d\r\n", 
			DevStat.bus_number[0]*100 + DevStat.bus_number[1] * 10 + DevStat.bus_number[2]);
    armputs(p_arm_snd_buf);
	
	if( DevStat.allow_upload_data == 1)
		sprintf((void *)p_arm_snd_buf, "存储设备:EEROM\r\n");
	else
		sprintf((void *)p_arm_snd_buf, "存储设备:SRAM\r\n");
    armputs(p_arm_snd_buf);

	i = Get_Time(&time);
	if ( i != ok )
	{
		memset((void *)&time, 0x00, sizeof(time));
	}
	sprintf((void *)p_arm_snd_buf, "时间(time): %02X%02X-%02X-%02X %02X:%02X:%02X\r\n", 
			time.century, time.year, time.month, time.day, time.hour, time.minute, time.second);
    armputs(p_arm_snd_buf);

//  if (strlen((char *)DevStat.apn_name) >= 20)
//  	DevStat.apn_name[19] = '\0';
//  sprintf((void *)p_arm_snd_buf, "apn名(apnname): %s\r\n", (char *)DevStat.apn_name);
//  armputs(p_arm_snd_buf);
//
//  if (strlen((char *)DevStat.apn_username) >= 20)
//  	DevStat.apn_username[19] = '\0';
//  sprintf((void *)p_arm_snd_buf, "apn用户名(apnusername): %s\r\n", (char *)DevStat.apn_username);
//  armputs(p_arm_snd_buf);
//
//  if (strlen((char *)DevStat.apn_password) >= 20)
//  	DevStat.apn_password[19] = '\0';
//  sprintf((void *)p_arm_snd_buf, "apn密码(apnpasswd): %s\r\n", (char *)DevStat.apn_password);
//  armputs(p_arm_snd_buf);
//
//  if (strlen((char *)DevStat.server_ip) >= 20)
//  	DevStat.server_ip[19] = '\0';
//  sprintf((void *)p_arm_snd_buf, "服务器ip(serverip): %s\r\n", (char *)DevStat.server_ip);
//  armputs(p_arm_snd_buf);
//
//  if (strlen((char *)DevStat.server_port) >= 10)
//  	DevStat.server_port[9] = '\0';
//  sprintf((void *)p_arm_snd_buf, "服务器端口号(serverport): %s\r\n", (char *)DevStat.server_port);
//  armputs(p_arm_snd_buf);
//
//  sprintf((void *)p_arm_snd_buf, "一卡通水价: %02X%02X\r\n", DevStat.water_price_cpu[0], DevStat.water_price_cpu[1]);
//  armputs(p_arm_snd_buf);
//
//  sprintf((void *)p_arm_snd_buf, "M1卡水价: %02X%02X\r\n", DevStat.water_price_m1[0], DevStat.water_price_m1[1]);
//  armputs(p_arm_snd_buf);
//
//  sprintf((void *)p_arm_snd_buf, "开灯时间: %02X%02X\r\n", DevStat.light_on_time[0], DevStat.light_on_time[1]);
//  armputs(p_arm_snd_buf);
//
//  sprintf((void *)p_arm_snd_buf, "关灯时间: %02X%02X\r\n", DevStat.light_off_time[0], DevStat.light_off_time[1]);
//  armputs(p_arm_snd_buf);
//
//  sprintf((void *)p_arm_snd_buf, "GPRS通讯时间间隔秒(gprsinterval): %d\r\n", DevStat.gprs_comm_interval);
//  armputs(p_arm_snd_buf);
//
//
//  if (strlen((char *)DevStat.black_list1_version) >= 15)
//  	DevStat.black_list1_version[14] = '\0';
//  sprintf((void *)p_arm_snd_buf, "黑名单库1(一卡通)版本号: %s\r\n", (char *)DevStat.black_list1_version);
//  armputs(p_arm_snd_buf);
//
//  sprintf((void *)p_arm_snd_buf, "黑名单库1(一卡通)数量: %d\r\n", DevStat.black_name1_number);
//  armputs(p_arm_snd_buf);


//  if (strlen((char *)DevStat.black_list2_version) >= 15)
//  	DevStat.black_list2_version[14] = '\0';
//  sprintf((void *)p_arm_snd_buf, "黑名单库2(M1卡)版本号: %s\r\n", (char *)DevStat.black_list2_version);
//  armputs(p_arm_snd_buf);
//
//  sprintf((void *)p_arm_snd_buf, "黑名单库2(M1卡)数量: %d\r\n", DevStat.black_name2_number);
//  armputs(p_arm_snd_buf);
//
//  sprintf((void *)p_arm_snd_buf, "未上传记录数量: %d\r\n", DevStat.record_number);
//  armputs(p_arm_snd_buf);
//
//  if (strlen((char *)DevStat.led_ad_version) >= 15)
//  	DevStat.led_ad_version[14] = '\0';
//  sprintf((void *)p_arm_snd_buf, "LED广告版本号: %s\r\n", (char *)DevStat.led_ad_version);
//  armputs(p_arm_snd_buf);



	if (DevStat.err_occured == TRUE)
	{
		sprintf((void *)p_arm_snd_buf, "设备故障\r\n");
		armputs(p_arm_snd_buf);
//
//  	if ( ( DevStat.err_code & PSAM_ERROR) != 0 )		 //PSAM卡错误
//  	{
//  		armputs("PSAM卡错误!\r\n");
//  	}
    	if ( (DevStat.err_code & RC500_ERROR) != 0 ) //读写头错误
    	{
    		armputs("读写头错误!\r\n");
    	}
//  	if ( (DevStat.err_code & LED_ERROR) != 0 ) //无法连接LED广告屏
//  	{
//  		armputs("无法连接LED广告屏!\r\n");
//  	}

	}
	else
	{
		armputs("设备正常\r\n");
	}

//	sprintf((void *)p_arm_snd_buf, "设备状态码: %s\r\n", (char *)DevStat.err_stat);
	armputs(p_arm_snd_buf);



    return ok;
}

/******************************************************************************
 函数名称：FormatFunc
 功能描述：格式化FLASH
 参数名称： 输入/输出？ 类型        描述
 argc       输入        INT8U           参数数量
 argv       输入        INT8S**     指向参数的指针

 返  回  值：ok(0)-成功
                 notok(0xFF)-失败

 作      者 ：刘及华
 日      期：2005-05-08
 修改历史：
        日期        修改人      修改描述
        ------      ---------   -------------
******************************************************************************/
INT8U FormatFunc(INT8U argc, INT8S **argv)
{
//    INT8U i;
//    INT8U step1 = FALSE;
//    INT8U step2 = FALSE;
//    INT8U step3 = FALSE;
//    INT8U step4 = FALSE;
//    INT8S * end;
//    INT32S value;

    ToggleWD();

    armputs("\r\n");

    if (argc == 0)              //如果没有参数，则执行全部流程
    {
		armputs("format后面请跟密码.\r\n");
		return notok;
    }
    else
    {
		if ( strcmp((char *)argv[0], "000000") == 0 )
		{

		}
		else
		{
			armputs("密码错误.\r\n");
			return notok;
		}
    }

	armputs("Start formatting...\r\n");

//	init_param();
	armputs("Success!\r\n");

    return ok;
}

///******************************************************************************
// 函数名称：RestartFunc
// 功能描述：复位命令
// 参数名称： 输入/输出？ 类型        描述
// argc       输入        INT8U           参数数量
// argv       输入        INT8S**     指向参数的指针
//
// 返  回  值：ok(0)-成功
//                 notok(0xFF)-失败
//
// 作      者 ：刘及华
// 日      期：2005-05-08
// 修改历史：
//        日期        修改人      修改描述
//        ------      ---------   -------------
//******************************************************************************/
//INT8U RestartFunc(INT8U argc, INT8S **argv)
//{
//    resetpos();             //复位
//
//    return ok;
//}
/******************************************************************************
 函数名称：ChkStkFunc
 功能描述：堆栈检查命令
 参数名称： 输入/输出？ 类型        描述
 argc       输入        INT8U           参数数量
 argv       输入        INT8S**     指向参数的指针

 返  回  值：ok(0)-成功
                 notok(0xFF)-失败

 作      者 ：刘及华
 日      期：2005-05-08
 修改历史：
        日期        修改人      修改描述
        ------      ---------   -------------
******************************************************************************/
INT8U ChkStkFunc(INT8U argc, INT8S **argv)
{
    OS_STK_DATA stk_data;
    INT8U err;
    INT8S buf[20];

    ToggleWD();

    armputs("\r\n");

    armputs("Task                       Prio  Total Stack  Free Stack  Used Stack \r\n");
    armputs("-------------------------  ----  -----------  ----------  ---------- \r\n");
    armputs("TaskStart               :  ");
    err = OSTaskStkChk(TASK0_PRIO, &stk_data);
    if (err == OS_ERR_NONE)
    {
        sprintf((void *)buf, "%4d  ", TASK0_PRIO);         //print Prio
        armputs(buf);
        sprintf((void *)buf, "%11d  ", stk_data.OSFree + stk_data.OSUsed);          //print Total Stack
        armputs(buf);
        sprintf((void *)buf, "%10d  ", stk_data.OSFree);            //print Free Stack
        armputs(buf);
        sprintf((void *)buf, "%10d \r\n", stk_data.OSUsed);           //print Used Stack
        armputs(buf);
    }

    armputs("TaskGeneralCtrl         :  ");
    err = OSTaskStkChk(TASK1_PRIO, &stk_data);
    if (err == OS_ERR_NONE)
    {
        sprintf((void *)buf, "%4d  ", TASK1_PRIO);           //print Prio
        armputs(buf);
        sprintf((void *)buf, "%11d  ", stk_data.OSFree + stk_data.OSUsed);          //print Total Stack
        armputs(buf);
        sprintf((void *)buf, "%10d  ", stk_data.OSFree);            //print Free Stack
        armputs(buf);
        sprintf((void *)buf, "%10d \r\n", stk_data.OSUsed);           //print Used Stack
        armputs(buf);
    }

    armputs("task_shell              :  ");
    err = OSTaskStkChk(TASK2_PRIO, &stk_data);
    if (err == OS_ERR_NONE)
    {
        sprintf((void *)buf, "%4d  ", TASK2_PRIO);          //print Prio
        armputs(buf);
        sprintf((void *)buf, "%11d  ", stk_data.OSFree + stk_data.OSUsed);          //print Total Stack
        armputs(buf);
        sprintf((void *)buf, "%10d  ", stk_data.OSFree);            //print Free Stack
        armputs(buf);
        sprintf((void *)buf, "%10d \r\n", stk_data.OSUsed);           //print Used Stack
        armputs(buf);
    }

    armputs("task_gprs               :  ");
    err = OSTaskStkChk(TASK3_PRIO, &stk_data);
    if (err == OS_ERR_NONE)
    {
        sprintf((void *)buf, "%4d  ", TASK3_PRIO);          //print Prio
        armputs(buf);
        sprintf((void *)buf, "%11d  ", stk_data.OSFree + stk_data.OSUsed);          //print Total Stack
        armputs(buf);
        sprintf((void *)buf, "%10d  ", stk_data.OSFree);            //print Free Stack
        armputs(buf);
        sprintf((void *)buf, "%10d \r\n", stk_data.OSUsed);           //print Used Stack
        armputs(buf);
    }


    return ok;
}

/******************************************************************************
 函数名称：FLPageFunc0
 功能描述：显示FLASH0一页的内容，转换成ascii码再显示
 参数名称： 输入/输出？ 类型        描述
 argc       输入        INT8U           参数数量
 argv       输入        INT8S**     指向参数的指针

 返  回  值：ok(0)-成功
				 notok(0xFF)-失败

 作      者 ：刘及华
 日      期：2005-05-08
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
INT8U FLPageFunc0(INT8U argc, INT8S **argv)
{
//  INT8S buf[20];
//  INT8U page_buf[SECTOR_SIZE];
//	INT8S *end;
//	INT32S value;
//	INT16U i = 0;
//
////	ToggleWD();
//
//	if (argc == 0)
//	{
//		armputs("flpage0 <page number>\r\n");
//		armputs("page number: 0-4095\r\n");
//		armputs("For example: flp 0\r\n");
//		armputs("get data from page0.\r\n");
//		return notok;
//	}
//
//	value = strtol((void *)argv[0], (void *)&end, 10);
//
//	if (end == argv[0])
//	{   //输入的参数开头不是数字
//		armputs("flpage0 <page number>\r\n");
//		armputs("page number: 0-4095\r\n");
//		armputs("For example: flp 0\r\n");
//		armputs("get data from page0.\r\n");
//		return notok;
//	}
//
//	if ( value > 4095 )
//	{
//		armputs("Page number error!\r\n");
//		armputs("Page number range: 0 ~ 4095\r\n");
//		return notok;
//	}
//
//	at45db_page_read512(value, fpage_buf);
//
//
//	for(i=0; i<16; i++)
//	{
//		htoa(p_arm_snd_buf, fpage_buf + 32 * i, 32);
//		armputs(p_arm_snd_buf);
//		armputs("\r\n");
//	}


	return ok;
}

///******************************************************************************
// 函数名称：FLPageFunc1
// 功能描述：显示FLASH1一页的内容
// 参数名称： 输入/输出？ 类型        描述
// argc       输入        INT8U           参数数量
// argv       输入        INT8S**     指向参数的指针
//
// 返  回  值：ok(0)-成功
//				 notok(0xFF)-失败
//
// 作      者 ：刘及华
// 日      期：2005-05-08
// 修改历史：
//		日期        修改人      修改描述
//		------      ---------   -------------
//******************************************************************************/
//INT8U FLPageFunc1(INT8U argc, INT8S **argv)
//{
////  INT8S buf[20];
////  INT8U page_buf[SECTOR_SIZE];
//	INT8S *end;
//	INT32S value;
////  INT16U i = 0;
//
////	ToggleWD();
//
//	if (argc == 0)
//	{
//		armputs("flpage1 <page number>\r\n");
//		armputs("page number: 0-4095\r\n");
//		armputs("For example: flpage1 0\r\n");
//		armputs("get data from page0.\r\n");
//		return notok;
//	}
//
//	value = strtol((void *)argv[0], (void *)&end, 10);
//
//	if (end == argv[0])
//	{   //输入的参数开头不是数字
//		armputs("flpage1 <page number>\r\n");
//		armputs("page number: 0-4095\r\n");
//		armputs("For example: flpage1 0\r\n");
//		armputs("get data from page0.\r\n");
//		return notok;
//	}
//
//	if ( value > 4095 )
//	{
//		armputs("Page number error!\r\n");
//		armputs("Page number range: 0 ~ 4095\r\n");
//		return notok;
//	}
//
//
//	at45db_page_read512(value, fpage_buf);
//
//	com1_send_hex(fpage_buf, 256);
//	com1_send_hex(&fpage_buf[256], 256);
//	armputs("\r\n");
//
//	return ok;
//}

/******************************************************************************
 函数名称：set_func
 功能描述：设置参数
 参数名称： 输入/输出？ 类型        描述
 argc       输入        INT8U           参数数量
 argv       输入        INT8S**     指向参数的指针

 返  回  值：ok(0)-成功
                 notok(0xFF)-失败

 作      者 ：刘及华
 日      期：2005-05-08
 修改历史：
        日期        修改人      修改描述
        ------      ---------   -------------
******************************************************************************/
INT8U set_func(INT8U argc, INT8S **argv)
{
  INT8U buf[50];
	INT8U i = 0;
	BUS_TIME time;
	INT32S value;
	INT32U a = 0;
	INT8S *end;

    ToggleWD();

    armputs("\r\n");

    if (argc != 2)              //如果没有参数，则执行全部流程
    {
		armputs("设置项：id time apnname apnusername apnpasswd serverip serverport gprsinterval\r\n");
		armputs("格式: set xxx xxx.\r\n");
		armputs("例如设置id: set id 000001\r\n");
		armputs("清除apnpasswd: set apnpasswd NULL\r\n");
		return notok;
    }
    else
    {
//		if ( strcmp((char *)argv[0], "000000") == 0 )
//		{
//
//		}
//		else
//		{
//			armputs("密码错误.\r\n");
//			return notok;
//		}
    }

	//设置id
	if ( (strstr((char *)argv[0], "id") != NULL) || (strstr((char *)argv[0], "ID") != NULL) )
	{
		value = strtol((void *)argv[1], (void *)&end, 10);

		if (end == argv[1])
		{   //输入的参数开头不是数字
			armputs("ID为6位数字，请重新输入。\r\n");
			return notok;
		}

		if ( value > 999999 )
		{
			armputs("ID为6位数字，请重新输入。\r\n");
			return notok;
		}
		buf[0] = (INT8U)(value / 100000);
		buf[1] = (INT8U)((value % 100000) / 10000);
		buf[2] = (INT8U)((value % 10000) / 1000);
		buf[3] = (INT8U)((value % 1000) / 100);
		buf[4] = (INT8U)((value % 100) / 10);
		buf[5] = (INT8U)(value % 10);

		DevStat.bus_number[0] = (buf[0] << 4) | buf[1];
		DevStat.bus_number[1] = (buf[2] << 4) | buf[3];
		DevStat.bus_number[2] = (buf[4] << 4) | buf[5];

		//write_param();
		armputs("set id ok.\r\n");

		//read_param();
		sprintf((void *)p_arm_snd_buf, "终端编号(id): %06d\r\n", 
				DevStat.bus_number[0]*100 + DevStat.bus_number[1]*10 + DevStat.bus_number[2]);
		armputs(p_arm_snd_buf);
		return ok;
	}

	//设置时间
	if ( (strstr((char *)argv[0], "time") != NULL) || (strstr((char *)argv[0], "TIME") != NULL) )
	{
		if ( strlen((void *)argv[1]) != 14 )
		{
			armputs("格式不对，请重新输入。格式为YYYYMMDDHHMMSS一共14个字节。\r\n");
			return notok;
		}
		for (a=0; a<strlen((void *)argv[1]); a++)
		{
			if ( isdigit(argv[1][a]) == TRUE )
			{
			}
			else
			{
				armputs("格式不对，请重新输入。格式为YYYYMMDDHHMMSS一共14个字节。\r\n");
				return notok;
			}
		}

		strcpy((char *)buf, (char *)argv[1]);
		time.century = (ascii_to_hex(buf[0]) << 4) | ascii_to_hex(buf[1]);
		time.year = (ascii_to_hex(buf[2]) << 4) | ascii_to_hex(buf[3]);
		time.month = (ascii_to_hex(buf[4]) << 4) | ascii_to_hex(buf[5]);
		time.day = (ascii_to_hex(buf[6]) << 4) | ascii_to_hex(buf[7]);
		time.hour = (ascii_to_hex(buf[8]) << 4) | ascii_to_hex(buf[9]);
		time.minute = (ascii_to_hex(buf[10]) << 4) | ascii_to_hex(buf[11]);
		time.second = (ascii_to_hex(buf[12]) << 4) | ascii_to_hex(buf[13]);
		(void)Modify_Time(&time);
		armputs("set time ok.\r\n");

		i = Get_Time(&time);
		if ( i != ok )
		{
			memset((void *)&time, 0x00, sizeof(time));
		}
		sprintf((void *)p_arm_snd_buf, "TIME: %02X%02X-%02X-%02X %02X:%02X:%02X\r\n", 
				time.century, time.year, time.month, time.day, time.hour, time.minute, time.second);
		armputs(p_arm_snd_buf);
		return ok;
	}

//	//设置apn名
//	if ( (strstr((char *)argv[0], "apnname") != NULL) || (strstr((char *)argv[0], "APNNAME") != NULL) )
//	{
//		if ( strlen((void *)argv[1]) == 0  || strlen((void *)argv[1]) >= 17 )
//		{
//			armputs("格式不对，请重新输入。\r\n");
//			return notok;
//		}
//
//		strcpy((char *)DevStat.apn_name, (char *)argv[1]);
////		strcpy((char *)DevStat.apn_name, "\"");
////		strcat((char *)DevStat.apn_name, (char *)argv[1]);
////		strcat((char *)DevStat.apn_name, "\"");
//
//		write_param();
//		armputs("set apnname ok.\r\n");
//
//		read_param();
//		sprintf((void *)p_arm_snd_buf, "apn名(apnname): %s\r\n", (char *)DevStat.apn_name);
//		armputs(p_arm_snd_buf);
//		return ok;
//	}

	//设置apn用户名
//	if ( (strstr((char *)argv[0], "apnusername") != NULL) || (strstr((char *)argv[0], "APNUSERNAME") != NULL) )
//	{
//		if ( strlen((void *)argv[1]) == 0  || strlen((void *)argv[1]) >= 17 )
//		{
//			armputs("格式不对，请重新输入。\r\n");
//			return notok;
//		}
//
//		if ( strcmp((void *)argv[1], "NULL") == 0 )
//		{
//			memset((void *)DevStat.apn_username, 0x00, sizeof(DevStat.apn_username));
////			strcpy((char *)DevStat.apn_username, "\"\"");
//		}
//		else
//		{
//			strcpy((char *)DevStat.apn_username, (char *)argv[1]);
////			strcpy((char *)DevStat.apn_username, "\"");
////			strcat((char *)DevStat.apn_username, (char *)argv[1]);
////			strcat((char *)DevStat.apn_username, "\"");
//		}
//
//		write_param();
//		armputs("set apnusername ok.\r\n");
//
//		read_param();
//		sprintf((void *)p_arm_snd_buf, "apn用户名(apnusername): %s\r\n", (char *)DevStat.apn_username);
//		armputs(p_arm_snd_buf);
//		return ok;
//	}
//
//	//设置apn密码
//	if ( (strstr((char *)argv[0], "apnpasswd") != NULL) || (strstr((char *)argv[0], "APNPASSWD") != NULL) )
//	{
//		if ( strlen((void *)argv[1]) == 0 || strlen((void *)argv[1]) >= 17 )
//		{
//			armputs("格式不对，请重新输入。\r\n");
//			return notok;
//		}
//
//		if ( strcmp((void *)argv[1], "NULL") == 0 )
//		{
//			memset((void *)DevStat.apn_password, 0x00, sizeof(DevStat.apn_password));
////			strcpy((char *)DevStat.apn_password, "\"\"");
//		}
//		else
//		{
//			strcpy((char *)DevStat.apn_password, (char *)argv[1]);
////			strcpy((char *)DevStat.apn_password, "\"");
////			strcat((char *)DevStat.apn_password, (char *)argv[1]);
////			strcat((char *)DevStat.apn_password, "\"");
//		}
//
//		write_param();
//		armputs("set apnpasswd ok.\r\n");
//
//		read_param();
//		sprintf((void *)p_arm_snd_buf, "apn密码(apnpasswd): %s\r\n", (char *)DevStat.apn_password);
//		armputs(p_arm_snd_buf);
//		return ok;
//	}
//
//	//设置服务器ip
//	if ( (strstr((char *)argv[0], "serverip") != NULL) || (strstr((char *)argv[0], "SERVERIP") != NULL) )
//	{
//		if ( strlen((void *)argv[1]) == 0  || strlen((void *)argv[1]) >= 17 )
//		{
//			armputs("格式不对，请重新输入。\r\n");
//			return notok;
//		}
//
//		strcpy((char *)DevStat.server_ip, (char *)argv[1]);
//		write_param();
//		armputs("set serverip ok.\r\n");
//
//		read_param();
//		sprintf((void *)p_arm_snd_buf, "服务器ip(serverip): %s\r\n", (char *)DevStat.server_ip);
//		armputs(p_arm_snd_buf);
//		return ok;
//	}
//
//	//设置服务器端口号
//	if ( (strstr((char *)argv[0], "serverport") != NULL) || (strstr((char *)argv[0], "SERVERPORT") != NULL) )
//	{
//		if ( strlen((void *)argv[1]) == 0  || strlen((void *)argv[1]) >= 7 )
//		{
//			armputs("格式不对，请重新输入。\r\n");
//			return notok;
//		}
//
//		strcpy((char *)DevStat.server_port, (char *)argv[1]);
//		write_param();
//		armputs("set serverport ok.\r\n");
//
//		read_param();
//		sprintf((void *)p_arm_snd_buf, "服务器端口号(serverport): %s\r\n", (char *)DevStat.server_port);
//		armputs(p_arm_snd_buf);
//		return ok;
//	}

	//GPRS通讯时间间隔秒
//	if ( (strstr((char *)argv[0], "gprsinterval") != NULL) || (strstr((char *)argv[0], "GPRSINTERVAL") != NULL) )
//	{
//		value = strtol((void *)argv[1], (void *)&end, 10);
//
//		if (end == argv[1])
//		{   //输入的参数开头不是数字
//			armputs("gprsinterval为数字，请重新输入。\r\n");
//			return notok;
//		}
//		if ( value > 24L * 60L * 60L )
//		{
//			armputs("不能超过86400秒，请重新输入。\r\n");
//			return notok;
//		}
//
//		DevStat.gprs_comm_interval = value;
//
//		write_param();
//		armputs("set gprsinterval ok.\r\n");
//
//		read_param();
//		sprintf((void *)p_arm_snd_buf, "GPRS通讯时间间隔秒(gprsinterval): %d\r\n", DevStat.gprs_comm_interval);
//		armputs(p_arm_snd_buf);
//		return ok;
//	}


	armputs("设置项：id time apnname apnusername apnpasswd serverip serverport gprsinterval\r\n");
	armputs("格式: set xxx xxx.\r\n");
	armputs("例如设置id: set id 000001\r\n");
	armputs("清除apnpasswd: set apnpasswd NULL\r\n");
	return notok;
}

/******************************************************************************
 函数名称：upload_func
 功能描述：上传数据
 参数名称： 输入/输出？ 类型        描述
 argc       输入        INT8U           参数数量
 argv       输入        INT8S**     指向参数的指针

 返  回  值：ok(0)-成功
                 notok(0xFF)-失败

 作      者 ：刘及华
 日      期：2005-05-08
 修改历史：
        日期        修改人      修改描述
        ------      ---------   -------------
******************************************************************************/
INT8U upload_func(INT8U argc, INT8S **argv)
{
//	armputs("进入上传记录状态...\r\n");

	upload_process();

	return ok;
}

/******************************************************************************
 函数名称：upload_process
 功能描述：上传数据
 参数名称： 输入/输出？ 类型        描述

 返  回  值：无
 作      者 ：刘及华
 日      期：2005-05-08
 修改历史：
        日期        修改人      修改描述
        ------      ---------   -------------
******************************************************************************/
void upload_process(void)
{
//	INT8U i = 0;

//	//更新时间
//	i = com_shell_update_time();
//	if ( i != ok )
//		return;
//
//	//更新参数
//	i = com_shell_update_param();
//	if ( i != ok )
//		return;
//
//	//更新黑名单库1
//	i = com_shell_update_blacklist1();
//	if ( i != ok )
//		return;
//
	//更新黑名单库2
//	i = com_shell_update_blacklist2();
//	if ( i != ok )
//		return;
//
//	//上传记录
//	i = com_shell_upload_recfile_all();
//	if ( i != ok )
//		return;


}

/******************************************************************************
 函数名称：com_shell_update_time
 功能描述：与服务器同步时间
 参数名称：输入/输出？	类型		描述
 
 返  回  值：ok(0)-成功  
			KEY_CANCEL-按退出键了
			notok(0xFF)-失败，超时
 
 作      者：刘及华
 日      期：2007-03-05
 修改历史：
		日期		修改人		修改描述
******************************************************************************/
INT8U com_shell_update_time(void)
{
	INT8U i;
	INT16U rcv_len = 0;
	_TIME time;

	toggle_wd();

	strcpy((void *)p_arm_snd_buf, "TIME000");
	i = com_shell_snd_and_rcv_protocol0(p_arm_snd_buf, strlen((void *)p_arm_snd_buf), (void *)p_arm_rcv_buf, &rcv_len, SHELL_DELAY, 10);
	if ( i == ok )
	{
		time.century = (ascii_to_hex(p_arm_rcv_buf[19 + 4]) << 4) | ascii_to_hex(p_arm_rcv_buf[20 + 4]);
		time.year = (ascii_to_hex(p_arm_rcv_buf[21 + 4]) << 4) | ascii_to_hex(p_arm_rcv_buf[22 + 4]);
		time.month = (ascii_to_hex(p_arm_rcv_buf[23 + 4]) << 4) | ascii_to_hex(p_arm_rcv_buf[24 + 4]);
		time.day = (ascii_to_hex(p_arm_rcv_buf[25 + 4]) << 4) | ascii_to_hex(p_arm_rcv_buf[26 + 4]);
		time.hour = (ascii_to_hex(p_arm_rcv_buf[27 + 4]) << 4) | ascii_to_hex(p_arm_rcv_buf[28 + 4]);
		time.minute = (ascii_to_hex(p_arm_rcv_buf[29 + 4]) << 4) | ascii_to_hex(p_arm_rcv_buf[30 + 4]);
		time.second = (ascii_to_hex(p_arm_rcv_buf[31 + 4]) << 4) | ascii_to_hex(p_arm_rcv_buf[32 + 4]);

		time_write(&time);

//		lcd_disp(2, 0, "3.正在更新时间...ok           ");
		return ( ok );
	}
//	else if ( i == KEY_CANCEL )
//	{
//		return ( KEY_CANCEL );
//	}
	else
	{
		return ( notok );
	}

}

/*****************************************************************
函数原型：com_shell_snd_and_rcv_protocol0
功能描述：com_shell发送与接收,协议0
参数描述：
参数名称：	输入/输出？	类型		描述

返  回  值：ok-成功
			KEY_CANCEL-按退出键了
			notok(0xFF)-失败，超时

作      者	：刘及华
日      期：2005-05-19
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U com_shell_snd_and_rcv_protocol0(INT8U *str_to_snd, INT16U snd_len, INT8U *str_rcv, INT16U *rcv_len, INT16U timeout_start, INT16U timeout)
{
	INT8U i = 0;
	INT8U j = 0;
	INT8U packno2[10];

	DevStat.comm_gprs_packno++;
	if ( DevStat.comm_gprs_packno >= 100 )
		DevStat.comm_gprs_packno = 0;
	sprintf((void *)packno2, "%02d", DevStat.comm_gprs_packno);

	//gprs_protocol0_add_end(str_to_snd, snd_len);

	for ( i = 0; i <= 10; i++ )
	{
		arm_buf_flush();
		arm_send_hex(str_to_snd, snd_len + 24);

		j = arm_gets_with_keyboard(str_rcv, rcv_len, timeout_start, timeout);
		if ( j == ok )
		{
			if ( str_rcv[0] != '@' )            //判断开头
				continue;
			if ( crc_check(str_rcv, *rcv_len) != ok )
				continue;
			if ( (str_rcv[19 + 4] == 'E' && str_rcv[20 + 4] == 'R') )
				continue;
			if ( !(str_rcv[6] == packno2[0] && str_rcv[7] == packno2[1]) )      //判断包号
				continue;

			return ( ok );
		}

//		if ( j == KEY_CANCEL )
//			return ( KEY_CANCEL );       //按取消键了
	}
	return ( notok );
}

/******************************************************************************
 函数名称：com_shell_update_param
 功能描述：与服务器同步参数
 参数名称：输入/输出？	类型		描述
 
 返  回  值：ok(0)-成功  
			KEY_CANCEL-按退出键了
			notok(0xFF)-失败，超时
 
 作      者：刘及华
 日      期：2007-03-05
 修改历史：
		日期		修改人		修改描述
******************************************************************************/
//INT8U com_shell_update_param(void)
//{
//	INT8U i;
//	INT16U rcv_len = 0;
////	_TIME time;
//	INT8U water_price_cpu[2];				//一卡通水价，单位分/升103分为0x00 0x67
//	INT8U water_price_m1[2];				//自发M1卡水价，单位分/升103分为0x00 0x67
//
//	INT8U light_on_time[2];					//开灯时间，16:30为0x16 0x30
//	INT8U light_off_time[2];				//关灯时间，05:00为0x05 0x00
//
//	toggle_wd();
//
//	strcpy((void *)p_arm_snd_buf, "PRM1000");
//	i = com_shell_snd_and_rcv_protocol0(p_arm_snd_buf, strlen((void *)p_arm_snd_buf), (void *)p_arm_rcv_buf, &rcv_len, SHELL_DELAY, 10);
//	if ( i == ok )
//	{
//		water_price_cpu[0] = (ascii_to_hex(p_arm_rcv_buf[19 + 4]) << 4) | ascii_to_hex(p_arm_rcv_buf[20 + 4]);
//		water_price_cpu[1] = (ascii_to_hex(p_arm_rcv_buf[21 + 4]) << 4) | ascii_to_hex(p_arm_rcv_buf[22 + 4]);
//		water_price_m1[0] = (ascii_to_hex(p_arm_rcv_buf[23 + 4]) << 4) | ascii_to_hex(p_arm_rcv_buf[24 + 4]);
//		water_price_m1[1] = (ascii_to_hex(p_arm_rcv_buf[25 + 4]) << 4) | ascii_to_hex(p_arm_rcv_buf[26 + 4]);
//		light_on_time[0] = (ascii_to_hex(p_arm_rcv_buf[27 + 4]) << 4) | ascii_to_hex(p_arm_rcv_buf[28 + 4]);
//		light_on_time[1] = (ascii_to_hex(p_arm_rcv_buf[29 + 4]) << 4) | ascii_to_hex(p_arm_rcv_buf[30 + 4]);
//		light_off_time[0] = (ascii_to_hex(p_arm_rcv_buf[31 + 4]) << 4) | ascii_to_hex(p_arm_rcv_buf[32 + 4]);
//		light_off_time[1] = (ascii_to_hex(p_arm_rcv_buf[33 + 4]) << 4) | ascii_to_hex(p_arm_rcv_buf[34 + 4]);
//
//		if ( (memcmp((void *)DevStat.water_price_cpu, water_price_cpu, 2) == 0)
//			 && (memcmp((void *)DevStat.water_price_m1, water_price_m1, 2) == 0)
//			 && (memcmp((void *)DevStat.light_on_time, light_on_time, 2) == 0)
//			 && (memcmp((void *)DevStat.light_off_time, light_off_time, 2) == 0) )
//		{
//			//参数没变化，不保存了
//			return ok;
//		}
//
//		write_param();
//
//		return ( ok );
//	}
//	else if ( i == KEY_CANCEL )
//	{
//		return ( KEY_CANCEL );
//	}
//	else
//	{
//		return ( notok );
//	}
//
//}
//
/******************************************************************************
 函数名称：com_shell_update_blacklist1
 功能描述：从与服务器下载黑名单库1
 参数名称：输入/输出？	类型		描述

 返  回  值：ok(0)-成功
			KEY_CANCEL-按退出键了
			notok(0xFF)-失败，超时

 作      者：刘及华
 日      期：2007-03-05
 修改历史：
		日期		修改人		修改描述
******************************************************************************/
//INT8U com_shell_update_blacklist1(void)
//{
//	INT8U i;
//	INT16U rcv_len = 0;
//	INT8S buf[50];
//	INT16U black_num = 0;
//	int temp_int = 0;
//	INT16U trans_times = 0;
//	INT16U last_package_black_num = 0;
//	INT16U k = 0;
//	INT16U j = 0;
//	INT8U black_rec[BLACK_LEN];
//
//
//	toggle_wd();
//
//	//查询黑名单版本
//	if ( strlen((char *)DevStat.black_list1_version) != 14 )
//	{
//		strcpy((void *)p_arm_snd_buf, "BLV102000000000000000000000");
//	}
//	else
//	{
//		strcpy((void *)p_arm_snd_buf, "BLV1020");
//		strcat((void *)p_arm_snd_buf, (char *)DevStat.black_list1_version);
//		strcat((void *)p_arm_snd_buf, "000000");
//	}
//	i = com_shell_snd_and_rcv_protocol0(p_arm_snd_buf, strlen((void *)p_arm_snd_buf), (void *)p_arm_rcv_buf, &rcv_len, SHELL_DELAY, 10);
//	if ( i == ok )
//	{
//		memcpy((void *)DevStat.black_list_version_temp, &p_arm_rcv_buf[19 + 4], 14);
//		DevStat.black_list_version_temp[14] = '\0';
//
//		memcpy((void *)buf, &p_arm_rcv_buf[19 + 4 + 14], 6);
//		buf[7] = '\0';
//		sscanf((void *)buf, "%d", &temp_int);
//		black_num = (INT16U)temp_int;
//
//	}
//	else if ( i == KEY_CANCEL )
//	{
//		return ( KEY_CANCEL );
//	}
//	else
//	{
//		return ( notok );
//	}
//
//	if ( strcmp((char *)DevStat.black_list1_version, (char *)DevStat.black_list_version_temp) == 0 )
//	{
//		//黑名单版本已经是最新
//		return ok;
//	}
//
//	//更新黑名单流程
//	if ( black_num % BLACK_NUM_PER_PACK == 0 )
//	{
//		trans_times = black_num / BLACK_NUM_PER_PACK;
//	}
//	else
//	{
//		trans_times = (black_num / BLACK_NUM_PER_PACK) + 1;
//	}
//
//	for ( k = 0; k < trans_times; k++ )
//	{
//
//		if ( k == trans_times - 1 )               //最后一包
//		{
//			if ( black_num % BLACK_NUM_PER_PACK == 0 )
//			{
//				last_package_black_num = BLACK_NUM_PER_PACK;
//			}
//			else
//			{
//				last_package_black_num = black_num % BLACK_NUM_PER_PACK;
//			}
//
//			sprintf((void *)p_arm_snd_buf, "BLK1010%06d%04d", k * BLACK_NUM_PER_PACK, last_package_black_num);
//			i = com_shell_snd_and_rcv_protocol0(p_arm_snd_buf, strlen((void *)p_arm_snd_buf), (void *)p_arm_rcv_buf, &rcv_len, SHELL_DELAY, 10);
//			if ( i == ok )
//			{
//				for ( j = 0; j < last_package_black_num; j++ )
//				{
//					(void)StoreBlackName3((void *)&p_arm_rcv_buf[19 + 4 + j * BLACK_LEN], k * BLACK_NUM_PER_PACK + j);
//				}
//			}
//			else if ( i == KEY_CANCEL )
//			{
//				return ( KEY_CANCEL );
//			}
//			else
//			{
//				return ( notok );
//			}
//		}
//		else                                            //不是最后一包
//		{
//			sprintf((void *)p_arm_snd_buf, "BLK1010%06d%04d", k * BLACK_NUM_PER_PACK, BLACK_NUM_PER_PACK);
//			i = com_shell_snd_and_rcv_protocol0(p_arm_snd_buf, strlen((void *)p_arm_snd_buf), (void *)p_arm_rcv_buf, &rcv_len, SHELL_DELAY, 10);
//			if ( i == ok )
//			{
//				for ( j = 0; j < BLACK_NUM_PER_PACK; j++ )
//				{
//					(void)StoreBlackName3((void *)&p_arm_rcv_buf[19 + 4 + j * BLACK_LEN], k * BLACK_NUM_PER_PACK + j);
//				}
//			}
//			else if ( i == KEY_CANCEL )
//			{
//				return ( KEY_CANCEL );
//			}
//			else
//			{
//				return ( notok );
//			}
//		}
//
//	}
//
//	//传输完毕，导入正式库
//	for (k = 0; k < black_num; k++)
//	{
//		GetBlackName3(black_rec, k);
//		(void)StoreBlackName(black_rec, k);                    //将黑名单存入NVRAM
//	}
//	DevStat.black_name1_number = black_num;
//
//	strcpy((void *)DevStat.black_list1_version, (void *)DevStat.black_list_version_temp);
//	write_param();
//
//	return ( ok );
//}

/******************************************************************************
 函数名称：com_shell_update_blacklist2
 功能描述：从与服务器下载黑名单库2
 参数名称：输入/输出？	类型		描述

 返  回  值：ok(0)-成功
			KEY_CANCEL-按退出键了
			notok(0xFF)-失败，超时

 作      者：刘及华
 日      期：2007-03-05
 修改历史：
		日期		修改人		修改描述
******************************************************************************/
//INT8U com_shell_update_blacklist2(void)
//{
//	INT8U i;
//	INT16U rcv_len = 0;
//	INT8S buf[50];
//	INT16U black_num = 0;
//	int temp_int = 0;
//	INT16U trans_times = 0;
//	INT16U last_package_black_num = 0;
//	INT16U k = 0;
//	INT16U j = 0;
//	INT8U black_rec[BLACK_LEN];
//
//
//	toggle_wd();
//
//	//查询黑名单版本
//	if ( strlen((char *)DevStat.black_list1_version) != 14 )
//	{
//		strcpy((void *)p_arm_snd_buf, "BLV202000000000000000000000");
//	}
//	else
//	{
//		strcpy((void *)p_arm_snd_buf, "BLV2020");
//		strcat((void *)p_arm_snd_buf, (char *)DevStat.black_list2_version);
//		strcat((void *)p_arm_snd_buf, "000000");
//	}
//	i = com_shell_snd_and_rcv_protocol0(p_arm_snd_buf, strlen((void *)p_arm_snd_buf), (void *)p_arm_rcv_buf, &rcv_len, SHELL_DELAY, 10);
//	if ( i == ok )
//	{
//		memcpy((void *)DevStat.black_list_version_temp, &p_arm_rcv_buf[19 + 4], 14);
//		DevStat.black_list_version_temp[14] = '\0';
//
//		memcpy((void *)buf, &p_arm_rcv_buf[19 + 4 + 14], 6);
//		buf[7] = '\0';
//		sscanf((void *)buf, "%d", &temp_int);
//		black_num = (INT16U)temp_int;
//
//	}
//	else if ( i == KEY_CANCEL )
//	{
//		return ( KEY_CANCEL );
//	}
//	else
//	{
//		return ( notok );
//	}
//
//	if ( strcmp((char *)DevStat.black_list2_version, (char *)DevStat.black_list_version_temp) == 0 )
//	{
//		//黑名单版本已经是最新
//		return ok;
//	}
//
//	//更新黑名单流程
//	if ( black_num % BLACK_NUM_PER_PACK == 0 )
//	{
//		trans_times = black_num / BLACK_NUM_PER_PACK;
//	}
//	else
//	{
//		trans_times = (black_num / BLACK_NUM_PER_PACK) + 1;
//	}
//
//	for ( k = 0; k < trans_times; k++ )
//	{
//
//		if ( k == trans_times - 1 )               //最后一包
//		{
//			if ( black_num % BLACK_NUM_PER_PACK == 0 )
//			{
//				last_package_black_num = BLACK_NUM_PER_PACK;
//			}
//			else
//			{
//				last_package_black_num = black_num % BLACK_NUM_PER_PACK;
//			}
//
//			sprintf((void *)p_arm_snd_buf, "BLK2010%06d%04d", k * BLACK_NUM_PER_PACK, last_package_black_num);
//			i = com_shell_snd_and_rcv_protocol0(p_arm_snd_buf, strlen((void *)p_arm_snd_buf), (void *)p_arm_rcv_buf, &rcv_len, SHELL_DELAY, 10);
//			if ( i == ok )
//			{
//				for ( j = 0; j < last_package_black_num; j++ )
//				{
//					(void)StoreBlackName3((void *)&p_arm_rcv_buf[19 + 4 + j * BLACK_LEN], k * BLACK_NUM_PER_PACK + j);
//				}
//			}
//			else if ( i == KEY_CANCEL )
//			{
//				return ( KEY_CANCEL );
//			}
//			else
//			{
//				return ( notok );
//			}
//		}
//		else                                            //不是最后一包
//		{
//			sprintf((void *)p_arm_snd_buf, "BLK2010%06d%04d", k * BLACK_NUM_PER_PACK, BLACK_NUM_PER_PACK);
//			i = com_shell_snd_and_rcv_protocol0(p_arm_snd_buf, strlen((void *)p_arm_snd_buf), (void *)p_arm_rcv_buf, &rcv_len, SHELL_DELAY, 10);
//			if ( i == ok )
//			{
//				for ( j = 0; j < BLACK_NUM_PER_PACK; j++ )
//				{
//					(void)StoreBlackName3((void *)&p_arm_rcv_buf[19 + 4 + j * BLACK_LEN], k * BLACK_NUM_PER_PACK + j);
//				}
//			}
//			else if ( i == KEY_CANCEL )
//			{
//				return ( KEY_CANCEL );
//			}
//			else
//			{
//				return ( notok );
//			}
//		}
//
//	}
//
//	//传输完毕，导入正式库
//	for (k = 0; k < black_num; k++)
//	{
//		GetBlackName3(black_rec, k);
//		(void)StoreBlackName2(black_rec, k);                    //将黑名单存入NVRAM
//	}
//	DevStat.black_name2_number = black_num;
//
//	strcpy((void *)DevStat.black_list2_version, (void *)DevStat.black_list_version_temp);
//	write_param();
//
//	return ( ok );
//}
//
/******************************************************************************
 函数名称：com_shell_upload_recfile
 功能描述：向服务器上传记录文件
 参数名称：输入/输出？	类型		描述

 返  回  值：ok(0)-成功
			KEY_CANCEL-按退出键了
			notok(0xFF)-失败，超时

 作      者：刘及华
 日      期：2007-03-05
 修改历史：
		日期		修改人		修改描述
******************************************************************************/
//INT8U com_shell_upload_recfile(void)
//{
//	INT8U i;
//	INT16U rcv_len = 0;
//	INT8S buf[51];
////	FRESULT res;
////	UINT br;         //读写变量
//	INT16U trans_times = 0;
//	INT16U last_package_rec_num = 0;
//	INT16U k = 0;
//	INT16U j = 0;
////#define REC48_NUM_PER_PACK	20				//每包传多少条48字节记录
////#define REC32_NUM_PER_PACK	25				//每包传多少条32字节记录
//	INT16U rec_num = 0;     //记录数量
//
////	INT32U bus_num = 0;     //文件数量
////	int rec_len = 0;
////	INT32U one_file_rec_num = 0;
////	INT32U bus_ptr = 0;
//
////	char new_file_name[100];
//
//	toggle_wd();
//
//	read_param_recnum();
//	read_param_recbaseptr();
//
//	rec_num = DevStat.record_number;
//	if ( rec_num == 0 )               //无记录
//	{
//		return ( ok );
//	}
//
//
//	//48字节记录文件传输
//	if ( rec_num % REC48_NUM_PER_PACK == 0 )
//	{
//		trans_times = rec_num / REC48_NUM_PER_PACK;
//	}
//	else
//	{
//		trans_times = (rec_num / REC48_NUM_PER_PACK) + 1;
//	}
//
//
//	for ( k = 0; k < trans_times; k++ )
//	{
//		if ( k == trans_times - 1 )               //最后一包
//		{
//			if ( rec_num  % REC48_NUM_PER_PACK == 0 )
//			{
//				last_package_rec_num = REC48_NUM_PER_PACK;
//			}
//			else
//			{
//				last_package_rec_num = rec_num  % REC48_NUM_PER_PACK;
//			}
//
//			sprintf((void *)p_arm_snd_buf, "REC1%03d", last_package_rec_num * (INT16U)REC_LEN);
//			for ( j = 0; j < last_package_rec_num; j++ )
//			{
//				get_record((void *)buf, DevStat.record_base_ptr + k * REC48_NUM_PER_PACK + j);
//				memcpy((void *)&p_arm_snd_buf[7 + j * REC_LEN], (void *)buf, REC_LEN);
//			}
//			i = com_shell_snd_and_rcv_protocol0(p_arm_snd_buf, 7 + last_package_rec_num * REC_LEN, (void *)p_arm_rcv_buf, &rcv_len, SHELL_DELAY, 10);
//			if ( i != ok )
//			{
//				return ( notok );
//			}
//		}
//		else                                            //不是最后一包
//		{
////			strcpy((void *)p_arm_snd_buf, "REC1960");		//上传记录
//			sprintf((void *)p_arm_snd_buf, "REC3%03d", (INT16U)REC48_NUM_PER_PACK * (INT16U)REC_LEN);
//
//			for ( j = 0; j < REC48_NUM_PER_PACK; j++ )
//			{
//				get_record((void *)buf, DevStat.record_base_ptr + k * REC48_NUM_PER_PACK + j);
//				memcpy((void *)&p_arm_snd_buf[7 + j * REC_LEN], (void *)buf, REC_LEN);
//			}
//
//			i = com_shell_snd_and_rcv_protocol0(p_arm_snd_buf, 7 + (INT16U)REC48_NUM_PER_PACK * (INT16U)REC_LEN,
//												(void *)p_arm_rcv_buf, &rcv_len, SHELL_DELAY, 10);
//			if ( i != ok )
//			{
//				return ( notok );
//			}
//
//		}
//	}
//
//
//	return ok;
//}
//
/******************************************************************************
 函数名称：com_shell_upload_recfile_all
 功能描述：向服务器上传记录文件，异常补采
 参数名称：输入/输出？	类型		描述

 返  回  值：ok(0)-成功
			KEY_CANCEL-按退出键了
			notok(0xFF)-失败，超时

 作      者：刘及华
 日      期：2007-03-05
 修改历史：
		日期		修改人		修改描述
******************************************************************************/
//INT8U com_shell_upload_recfile_all(void)
//{
//	INT8U i;
//	INT16U rcv_len = 0;
//	INT8S buf[51];
////	FRESULT res;
////	UINT br;         //读写变量
//	INT16U trans_times = 0;
//	INT16U last_package_rec_num = 0;
//	INT16U k = 0;
//	INT16U j = 0;
////#define REC48_NUM_PER_PACK	20				//每包传多少条48字节记录
////#define REC32_NUM_PER_PACK	25				//每包传多少条32字节记录
//	INT16U rec_num = 0;     //记录数量
//
////	INT32U bus_num = 0;     //文件数量
////	int rec_len = 0;
////	INT32U one_file_rec_num = 0;
////	INT32U bus_ptr = 0;
//
////	char new_file_name[100];
//
//	toggle_wd();
//
//	read_param_recnum();
//	read_param_recbaseptr();
//
//	if ( DevStat.NVRAM_cycled == TRUE )       //循环过，12000条记录全部都采
//	{
//		rec_num = MAX_REC_NUM;               //正常补采时的记录起始指针和条数
//	}
//	else
//	{
//		rec_num = DevStat.record_base_ptr + DevStat.record_number;   //正常补采时的记录起始指针和条数
//	}
//	if ( rec_num == 0 )               //无记录
//	{
//		return ( ok );
//	}
//
//
//	//48字节记录文件传输
//	if ( rec_num % REC48_NUM_PER_PACK == 0 )
//	{
//		trans_times = rec_num / REC48_NUM_PER_PACK;
//	}
//	else
//	{
//		trans_times = (rec_num / REC48_NUM_PER_PACK) + 1;
//	}
//
//
//	for ( k = 0; k < trans_times; k++ )
//	{
//		if ( k == trans_times - 1 )               //最后一包
//		{
//			if ( rec_num  % REC48_NUM_PER_PACK == 0 )
//			{
//				last_package_rec_num = REC48_NUM_PER_PACK;
//			}
//			else
//			{
//				last_package_rec_num = rec_num  % REC48_NUM_PER_PACK;
//			}
//
//			sprintf((void *)p_arm_snd_buf, "REC1%03d", last_package_rec_num * REC_LEN);
//			for ( j = 0; j < last_package_rec_num; j++ )
//			{
////				get_record((void *)buf, DevStat.record_base_ptr + k * REC48_NUM_PER_PACK + j);
//				get_record((void *)buf, k * REC48_NUM_PER_PACK + j);
//				memcpy((void *)&p_arm_snd_buf[7 + j * REC_LEN], (void *)buf, REC_LEN);
//			}
//			i = com_shell_snd_and_rcv_protocol0(p_arm_snd_buf, 7 + last_package_rec_num * REC_LEN, (void *)p_arm_rcv_buf, &rcv_len, SHELL_DELAY, 10);
//			if ( i != ok )
//			{
//				return ( notok );
//			}
//		}
//		else                                            //不是最后一包
//		{
////			strcpy((void *)p_arm_snd_buf, "REC1960");		//上传记录
//			sprintf((void *)p_arm_snd_buf, "REC3%03d", (INT16U)REC48_NUM_PER_PACK * (INT16U)REC_LEN);
//
//			for ( j = 0; j < REC48_NUM_PER_PACK; j++ )
//			{
////				get_record((void *)buf, DevStat.record_base_ptr + k * REC48_NUM_PER_PACK + j);
//				get_record((void *)buf, k * REC48_NUM_PER_PACK + j);
//				memcpy((void *)&p_arm_snd_buf[7 + j * REC_LEN], (void *)buf, REC_LEN);
//			}
//
////			i = com_shell_snd_and_rcv_protocol0(p_arm_snd_buf, 7 + 960, (void *)p_arm_rcv_buf, &rcv_len, SHELL_DELAY, 10);
//			i = com_shell_snd_and_rcv_protocol0(p_arm_snd_buf, 7 + (INT16U)REC48_NUM_PER_PACK * (INT16U)REC_LEN,
//												(void *)p_arm_rcv_buf, &rcv_len, SHELL_DELAY, 10);
//			if ( i != ok )
//			{
//				return ( notok );
//			}
//
//		}
//	}
//
//
//	return ok;
//}

