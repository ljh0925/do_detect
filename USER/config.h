/****************************************Copyright (c)**************************************************
**                         Guangzhou ZHIYUAN electronics Co.,LTD.                               
**                                     
**                               http://www.embedtools.com
**
**--------------File Info-------------------------------------------------------------------------------
** File Name			: config.h
** Last modified Date	: 2004-09-17
** Last Version			: V1.00
** Descriptions			: User Configurable File
**
**------------------------------------------------------------------------------------------------------
** Created By			: Chenmingji
** Created date			: 2004-09-17
** Version				: V1.00
** Descriptions			: First version
**
**------------------------------------------------------------------------------------------------------
** Modified by			: LinEnqiang
** Modified date		: 2007-05-15
** Version				: V1.01
** Descriptions			: Modified for LPC23xx
**
********************************************************************************************************/
#ifndef __CONFIG_H
#define __CONFIG_H

#ifdef __cplusplus
extern "C"
{
#endif

//这一段无需改动
#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#include "data_type.h"

#define GLOBAL	 extern

/********************************
**      uC/OS-II的特殊代码
********************************/
//#define     USER_USING_MODE    0x10         	//  用户模式,ARM代码
//  只能是0x10,0x30,0x1f,0x3f之一
#include "Includes.h"

///********************************
//**      ARM的特殊代码
//*********************************/
#include	"stm32f10x.h"

/********************************
**     应用程序配置             
*********************************/
//以下根据需要改动
#include    <stdio.h>
#include    <string.h>
#include    <ctype.h>
#include    <stdlib.h>
#include    <setjmp.h>
#include    <rt_misc.h>
#include	<locale.h>
#include	<stdlib.h>


#include	"main.h"

////fatfs
//#include	"diskio.h"
//#include	"ff.h"
//#include	"ffconf.h"
//#include	"integer.h"
//#include	"exfuns.h"
//#include	"fattester.h"

////driver
#include	"drv_led.h"
#include	"drv_delay.h"
#include	"drv_buzzer.h"
#include	"drv_other.h"
#include    "drv_door.h"
//#include	"drv_switch8.h"
#include	"drv_wdt.h"
//#include	"drv_rtc.h"
#include	"drv_i2c.h"
#include	"drv_spi1.h"
//#include	"24CXX.h"
#include 	"ds18b20.h"
#include 	"adc.h"
#include 	"oxygen.h"
#include 	"myiic.h"

//触摸屏相关文件
#include "hmi_driver.h"
#include "hmi_user_uart.h"
#include "cmd_queue.h"
#include "cmd_process.h"
#include "hw_config.h"
////tasks
#include	"task_general_control.h"
#include	"task_dissolve_oxygen.h"
//#include	"task_shell.h"
#include	"task_touch_UI.h"



/*define	pin	macro*/
#define	BIT0	0x00000001
#define	BIT1	0x00000002
#define	BIT2	0x00000004
#define	BIT3	0x00000008

#define	BIT4	0x00000010
#define	BIT5	0x00000020
#define	BIT6	0x00000040
#define	BIT7	0x00000080

#define	BIT8	0x00000100
#define	BIT9	0x00000200
#define	BIT10	0x00000400
#define	BIT11	0x00000800

#define	BIT12	0x00001000
#define	BIT13	0x00002000
#define	BIT14	0x00004000
#define	BIT15	0x00008000

#define	BIT16	0x00010000
#define	BIT17	0x00020000
#define	BIT18	0x00040000
#define	BIT19	0x00080000

#define	BIT20	0x00100000
#define	BIT21	0x00200000
#define	BIT22	0x00400000
#define	BIT23	0x00800000

#define	BIT24	0x01000000
#define	BIT25	0x02000000
#define	BIT26	0x04000000
#define	BIT27	0x08000000

#define	BIT28	0x10000000
#define	BIT29	0x20000000
#define	BIT30	0x40000000
#define	BIT31	0x80000000

#define BANK0   0x80000000
#define BANK1   0x81000000
#define BANK2   0x82000000
#define BANK3   0x83000000


#define resetpos() (*(void(*)())0)()


#ifdef __cplusplus
}
#endif

#endif
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
