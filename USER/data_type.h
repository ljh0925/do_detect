/******************************************************************************
Copyright 2005 大连易达通电子有限公司
All rights riserved.

文件名  ：data_type.h
模块名称：定义了所有的数据类型

当前版本：0.0.1
修改人	：刘及华
完成日期：2005.04.06
升级说明：create
******************************************************************************/
#ifndef __DATA_TYPE_H__
#define __DATA_TYPE_H__

#include "OS_CPU.H"

typedef unsigned char  	uchar;                   /* 无符号8位整型变量                        */
typedef unsigned char  	uint8;                   /* 无符号8位整型变量                        */
typedef signed   char  	int8;                    /* 有符号8位整型变量                        */
typedef unsigned short 	uint16;                  /* 无符号16位整型变量                       */
typedef signed   short 	int16;                   /* 有符号16位整型变量                       */
typedef unsigned int   	uint32;                  /* 无符号32位整型变量                       */
typedef signed   int   	int32;                   /* 有符号32位整型变量                       */
typedef float          	fp32;                    /* 单精度浮点数（32位长度）                 */
typedef double         	fp64;                    /* 双精度浮点数（64位长度）                 */



#ifndef QUEUE_DATA_TYPE
	#define QUEUE_DATA_TYPE     		uint8
#endif

typedef struct 
{
    QUEUE_DATA_TYPE     *Out;                   /* 指向数据输出位置         */
    QUEUE_DATA_TYPE     *In;                    /* 指向数据输入位置         */
    QUEUE_DATA_TYPE     *End;                   /* 指向Buf的结束位置        */
    uint16              		NData;                  /* 队列中数据个数           */
    uint16              		MaxData;                /* 队列中允许存储的数据个数 */
    uint8               		(* ReadEmpty)();        /* 读空处理函数             */
    uint8               		(* WriteFull)();        /* 写满处理函数             */
    QUEUE_DATA_TYPE     Buf[1];                 /* 存储数据的空间           */
} DataQueue;

//typedef float AD_TYPE;
//typedef double AD_TYPE;


//车载机时间结构
__packed typedef struct _TIME_
{
	INT8U century;
	INT8U year;
	INT8U month;
	INT8U day;
	INT8U hour;
	INT8U minute;
	INT8U second;
}_TIME, BUS_TIME;

////////////////////////////////////////////
//增加管理员编号联合结构变量
/////////////////////////////////////////////
typedef union _MANAGER_NUMBER_
{
	INT8U setter_number[3];//设置员编号
	INT8U gather_number[3];//采集员编号
	INT8U checker_number[3];//监测员编号
	INT8U signer_number[3];//签点员编号
	INT8U check_number[3];//稽查员编号
}_MANAGER_NUMBER;

//CARD_INFO中用到的struct
typedef struct _CARD_NUM
{
	INT8U city_code[2];	//城市代码
	INT8U calling_code[2];//行业代码
	INT8U serial_number[4];		//序列号
}CARD_NUM;

//卡片信息，交易记录由此变量生成
typedef struct _CARD_INFO
{
	union _CARD_NUMBER_
	{
		INT8U card_number[8];//卡号
		CARD_NUM detail_card_number;
	}_CARD_NUMBER;

	BUS_TIME card_in_time;							//交易时间
	INT8U card_type;								//卡类型
	INT32U fare;									//消费金额
	INT32U display_fare;							//消费金额
	INT8U init_purchase_serial_num[2];				//初始的卡内交易序号
	INT8U purchase_serial_num[2];					//初始的卡内交易序号+1
	INT32U init_balance;							//原余额
	INT32U balance;									//交易后余额
	INT8U card_purchase_type;						//交易类型
	INT8U tac[4];									//TAC码
	INT8U mac2[4];									//CPU卡返回的MAC2

	INT8U csn[4];									//硬件卡号
	INT8U authentication_code[4];					//卡认证码

	INT8U card_random_num[4];						//CPU卡随机数
	INT8U arithmetic_sign;							//算法标识
	INT8U cpu_key_version;							//CPU卡密钥版本号

//  INT32U psam_purchase_serial_num;				//PSAM卡交易序号
	INT8U psam_purchase_serial_num[4];				//PSAM卡交易序号
	INT8U mac1[4];									//PSAM卡计算出的MAC1

	//关于司机卡的上下班操作和员工卡
	INT8U drv_card_type;							//司机卡类型

	INT8U station_number[2];//站点编号
	INT8U checked_flag;//检测标志
	_MANAGER_NUMBER manager_number;//管理员编号 

 	INT8U card_record_point;						//消费记录区指针

	INT16U favor_times_balance;						//交易后剩余优惠次数  

	INT8U cpu_black_sign;							//CPU卡的使用标记 00H：未挂失		01H：已挂失		02H：黑名单
	INT8U m1_cpu;									//卡类型是CPU卡,还是M1卡 M1=M1CARD	CPU=CPUCARD

	INT8U is_love_card;								//是否为爱心卡 是=TRUE 不是=FALSE

	INT8U kind_of_card;								//卡类，CPU_CARD=CPU卡		M1_CARD=M1卡
	BUS_TIME valid_time;							//有效期

}CARD_INFO;






#endif
