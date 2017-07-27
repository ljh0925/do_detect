/****************************************Copyright (c) ****************************************
**                         Guangzhou ZHIYUAN electronics Co.,LTD.                               
**                                     
**                               http://www.embedtools.com
**
**―――File Info――――――――――――――――――――――――――――――――――――――
**  File          name :main.h
**  Last modified Date :
**  Last       Version :V1.0
**  Descriptions       :用户程序主函数头文件
**
**―――――――――――――――――――――――――――――――――――――――――――――
** Created   	    by :
** Created  	  date :2007-4-17
** Version             :
** Descriptions        :
**
**―――――――――――――――――――――――――――――――――――――――――――――
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



//全局条件编译控制
//#define EN_WDT    //禁止看门狗


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

#define RDK 0			//读和消费密钥
#define WRK 1			//写和充值密钥

#define PSAM_PRI	0
#define PSAM_SLV	1
#define PSAM3		2
#define PSAM4		3

#define DISP_DELAY_COUNTER_END		0x00
#define DISP_DELAY					1000						//交易成功后，余额停留在屏幕上的时间
#define SINGLE_KEY_QUERY_DELAY		2000					//单键查询时

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

#define MAX_DISP_REC	61						//单键查询时最大可以显示的记录数，要求<=255



//报错条件故障码
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



#define TASK0_ID            5      			// 任务的ID		taskstart
#define TASK0_PRIO          TASK0_ID		// 任务的优先级
#define TASK0_STACK_SIZE    512     		// 定义用户堆栈长度

#define TASK1_ID            9     			// 任务的ID 	总控任务
#define TASK1_PRIO          TASK1_ID		// 任务的优先级
#define TASK1_STACK_SIZE    1024     		// 定义用户堆栈长度

#define TASK2_ID            7				// 任务的ID		触摸屏通讯控制任务
#define TASK2_PRIO          TASK2_ID		// 任务的优先级
#define TASK2_STACK_SIZE    1024			// 定义用户堆栈长度

#define TASK3_ID            6				// 任务的ID		溶氧检测任务		
#define TASK3_PRIO          TASK3_ID		// 任务的优先级
#define TASK3_STACK_SIZE    1024			// 定义用户堆栈长度

//#define TASK4_ID            8				// 任务的ID		com1 gprs
//#define TASK4_PRIO          TASK4_ID		// 任务的优先级
//#define TASK4_STACK_SIZE    256				// 定义用户堆栈长度




//设备状态，最重要的变量，如果增加变量，请在SysInit里进行初始化
typedef struct _DEV_STAT
{
	INT8U err_occured;                      // == TRUE出错 ＝＝FALSE设备正常
	INT16U err_code;                        //出错代码
//  INT8U flash_err;						//FLASH错误	＝TRUE 错误	＝FALSE 正常

	INT8U allow_upload_data;                //采集标志
	INT8U databox_cmd_rcvd;                 //是否接收到数据盒的命令
//  INT8U gps_cmd_rcvd;                 	//是否接收到数据盒的命令

	INT8U purchase_again_req;               //重新刷卡语音标志＝＝TRUE播放重新刷卡 ==FALSE不播放
	INT8U voice_purchase_again_freq;


	INT8U block9[16];                       //m1卡block9缓存

	INT16U record_number;                   //未上传的记录条数(原p0+4)


	INT8U start;


	INT16U gprs_rec_ptr;                    //GPRS上传记录指针
	INT16U comm_gprs_packno;
	INT16U record_base_ptr;					//未上传记录指针起始指针(原p0+2) 
	INT16U last_record_base_ptr;			//正常补采的记录起始指针
	INT16U last_record_number;				//正常补采的记录条数
											//
	INT8U line_number[2];					//线路号
	INT8U bus_number[3];					// 3字节车号	
	INT32U normal_card_fare;				//普通卡票价
	INT32U fare[7];							//其他卡票价
	INT8U cur_driver_number[6]; 			//当班司机号

	INT8U have_been_set;					//是否刷过设置卡
	INT8U driver_is_working;				//上下班标志
	INT8U time_modify_times;				//修改时间次数

	INT16U black_name_number;				//黑名单数量

	INT8U typea_para_position;				//A类参数（专指记录条数）存放在FLASH的第页，范围0-99
	INT8U NVRAM_cycled;						//NVRAM中存放记录是否翻过页 == TRUE 翻过 else 没翻过

	INT8U param_flag;						//参数标志位	

	BUS_TIME update_black_time;				//上次更新黑名单的时间
	INT8U black_name_version[7];			//黑名单版本号



	INT16U crc;                             //参数的CRC校验码
	INT8U end;

	INT16U normal_interval;                 //普通卡间隔时间            //单位秒
//  INT16U favord_interval;					//优惠卡间隔时间
	INT8U long_disp_dly;
	INT8U enter_irda_commu;                 //准备进入红外通讯=TRUE 否则=FALSE

	INT8U drec[20];                         //司机记录 4字节司机编号+8字节车号+7字节时间+1字节上下班标志
	INT8U have_drec;                        //是否有未上传的drec，是=TRUE 否=FALSE
	INT8U update_time;                      //是否准备与服务器同步时间，是=TRUE 否=FALSE
	INT8U normal_gather_over;				//是否正常采集完毕标志 是=TRUE 否=FALSE

	INT16U bak0_rec_num;              		//REC_BAK0的记录条数
	INT16U bak1_rec_num;              		//REC_BAK1的记录条数
	INT16U abnormal_rec_num;				//异常补采的记录条数

	INT8U new_wireless_address[5];			//新nrf24l01通讯地址

	INT8U last_rec[10];						//上一次刷卡交易记录
	INT8U month_card_valid;					//是否是月票卡交易

}DEV_STAT;





//交易记录
typedef struct _RECORD
{
	INT8U card_type;                        //卡片类型  1字节
	INT8U card_purchase_type;               //交易类型  1字节
	INT8U psam_purchase_serial_num[3];      //PSAM卡交易序号3字节
//  INT8U bus_number[4];					//车号	4字节
//  INT8U card_serial_number[4];			//发行流水号	8字节
	INT8U card_number[8];                   //发行流水号    8字节
	INT8U balance[3];                       //交易后余额    3字节
	INT8U fare[3];                          //消费金额3字节
	BUS_TIME card_in_time;                  //交易时间  7字节
	INT8U purchase_serial_num[2];           //初始的卡内交易序号+1  2字节
	INT8U operator_number[4];               //操作员号  4字节

	INT8U line_number[2];                   //线路号    2字节
	INT8U bus_number[3];                    //车号      3字节
	INT8U driver_number[4];                 //驾驶员号  4字节
	INT8U pos_number[6];                    //PSAM卡号  6字节
	INT8U no_use[1];                        //备用      1字节

	INT8U crc[2];                           //crc校验   2字节
}RECORD;



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
int main(void);



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
void SysTick_Handler(void);



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
void ucosii_timer_init(void);



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
void task0_start(void *pdata);



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
void target_init(void);



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
void system_init(void);



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
INT8U system_check(void);



/******************************************************************************
 函数名称：psam_check
 功能描述：检测Psam卡
 参数描述：
 参数名称： 输入/输出？ 类型        描述

 返  回  值：ok(0)-自检成功
				 notok(0xFF)-自检失败

 作      者：刘及华
 日      期：2004-09-02
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
INT8U psam_check(void);



/******************************************************************************
 函数名称：GetFormatKeySequence
 功能描述：确认输入得格式化按键序列，执行格式化操作
 参数名称：	输入/输出？	类型		描述
 
 返  回  值：无
 
 作      者	：刘及华
 日      期：2004-11-25
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void GetFormatKeySequence(void);



/******************************************************************************
 函数名称：w25q64_check
 功能描述：检测flash w25q64
 参数描述：
 参数名称： 输入/输出？ 类型        描述

 返  回  值：ok(0)-自检成功
				 notok(0xFF)-自检失败

 作      者：刘及华
 日      期：2004-09-02
 修改历史：
		日期        修改人      修改描述
		------      ---------   -------------
******************************************************************************/
INT8U w25q64_check(void);



/*****************************************************************
函数原型：Lcd_Draw_SelfChk_Err_Result
功能描述：LCD显示设备异常
		// 
		//		设备异常
		//		出错代码
		//		  2004-03-01
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------

返  回  值：无
作      者	：刘及华
日      期：2004-03-01
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
void Lcd_Draw_SelfChk_Err_Result(void);



/******************************************************************************
 函数名称：get_psam1_info
 功能描述：取得psam1信息
 参数名称：	输入/输出？	类型		描述
 
 返  回  值：ok(0)-成功
				 notok(0xFF)-失败
 
 作      者	：刘及华
 日      期：2004-09-02
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
INT8U get_psam1_info(void);



/******************************************************************************
 函数名称：get_psam2_info
 功能描述：取得psam2信息
 参数名称：	输入/输出？	类型		描述
 
 返  回  值：ok(0)-成功
				 notok(0xFF)-失败
 
 作      者	：刘及华
 日      期：2004-09-02
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
INT8U get_psam2_info(void);



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
void TaskStart_CreateTasks(void);



/******************************************************************************
 函数名称：init_param
 功能描述：初始化参数
 参数描述：
 参数名称：	输入/输出？	类型		描述
				
 返  回  值：无

 作      者	：刘及华
 日      期：2004-09-02
 修改历史：
		日期		修改人		修改描述
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
