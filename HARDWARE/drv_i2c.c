/******************************************************************************
Copyright 2005 大连易达通电子有限公司
All rights riserved.

文件名	：drv_i2c.c
模块名称：IO口模拟I2C总线驱动程序
功能概要：IO口模拟I2C总线驱动程序

取代版本：0.0.1
修改人	：刘及华
完成日期：2005.04.06
升级说明：create

******************************************************************************/
#include		"all.h"


#ifdef I2C_USE_SEM
OS_EVENT			*i2c_sem;
#endif


#define  IIC_SDA_SetLow()		GPIO_ResetBits(GPIOC, GPIO_Pin_8)
#define  IIC_SDA_SetHigh()		GPIO_SetBits(GPIOC, GPIO_Pin_8)

#define  IIC_SCL_SetLow()		GPIO_ResetBits(GPIOC, GPIO_Pin_9)
#define  IIC_SCL_SetHigh()		GPIO_SetBits(GPIOC, GPIO_Pin_9)



/*****************************************************************
函数原型：i2c_init
功能描述：I2C总线初始化，在应用函数中首先调用本函数
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------

返  回  值：无
作      者：刘及华
日      期：2003-05-12
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
void i2c_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //使能PC端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_8;	//SDA-->PC.8,SCL-->PC.9 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		//普通输出模式
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO口速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					//根据设定参数初始化GPIO
	GPIO_SetBits(GPIOC, GPIO_Pin_9|GPIO_Pin_8);			    //输出高


#ifdef I2C_USE_SEM
	i2c_sem = OSSemCreate(1);			/* 信号量用于互斥操作总线 */
#endif


}

/*****************************************************************
函数原型：i2c_get_sda_stat
功能描述：取得I2C总线中SDA的状态
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------

返  回  值：0-LOW
				1-HIGH
作      者	：刘及华
日      期：2003-05-12
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U i2c_get_sda_stat(void)
{
	INT8U i = 0;
	GPIO_InitTypeDef  GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				//SDA-->PC.8 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 	//普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO口速度为50MHz
	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);					//根据设定参数初始化GPIO
	//PB9输入模式
//    GPIOB->MODER &= ~(3<<(7*2));
//    GPIOB->MODER |= 0<<7*2;

	i2c_delay2();

	if ( GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8) == 0 )
	{
		i = 0;
	}
	else
	{
		i = 1;
	}


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				//SDA-->PC.8 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 	//普通输出模式
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO口速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					//根据设定参数初始化GPIO
	//PB9输出模式
//	GPIOB->MODER &= ~(3<<(7*2));
//	GPIOB->MODER |= 1<<7*2;

	i2c_delay2();

	return i;

}

/*****************************************************************
函数原型：i2c_get_scl_stat
功能描述：取得I2C总线中SCL的状态
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------

返  回  值：0-LOW
				1-HIGH
作      者	：刘及华
日      期：2003-05-12
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U i2c_get_scl_stat(void)
{
	INT8U i = 0;
	GPIO_InitTypeDef  GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				//SCL-->PC.9 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 	//普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO口速度为50MHz
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);					//根据设定参数初始化GPIO
	//PB8输入模式
//	GPIOB->MODER &= ~(3<<(6*2));
//	GPIOB->MODER |= 0<<6*2;

	i2c_delay2();

	if ( GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9) == 0 )
	{
		i = 0;
	}
	else
	{
		i = 1;
	}


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				//SCL-->PC.9 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		//普通输出模式
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO口速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					//根据设定参数初始化GPIO
	//PB6输出模式
//	GPIOB->MODER &= ~(3<<(6*2));
//	GPIOB->MODER |= 1<<6*2;

	i2c_delay2();

	return i;
}



/////////////////////////////////////////////////////////
// 以下为不需要改写部分
/////////////////////////////////////////////////////////



/*****************************************************************
函数原型：I2C_start
功能描述：提供I2C总线工作时序中的起始位
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------

返  回  值：ok(0)-建立成功
				notok(0xFF)-建立失败
作      者	：刘及华
日      期：2003-05-12
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U i2c_start(void)
{
	toggle_wd();

	i2c_delay();
	IIC_SDA_SetHigh();
	i2c_delay();
	IIC_SCL_SetHigh();
	i2c_delay();
	if ( i2c_get_sda_stat() == 0 )
	{
		return notok;
	}
	if ( i2c_get_scl_stat() == 0 )
	{
		return notok;
	}

	IIC_SDA_SetLow();
	i2c_delay();
	IIC_SCL_SetLow();
	i2c_delay();
	return ok;
}

/*****************************************************************
函数原型：i2c_start2
功能描述：提供I2C总线工作时序中的起始位
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------

返  回  值：ok(0)-建立成功
				notok(0xFF)-建立失败
作      者	：刘及华
日      期：2003-05-12
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U i2c_start2(void)
{
	toggle_wd();

	i2c_delay();
	IIC_SDA_SetHigh();
	i2c_delay();
	IIC_SCL_SetHigh();
	i2c_delay();

	IIC_SDA_SetLow();
	i2c_delay();
	IIC_SCL_SetLow();
	i2c_delay();
	return ok;
}

/*****************************************************************
函数原型：I2C_stop
功能描述：提供I2C总线工作时序中的停止位
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------

返  回  值：无
作      者	：刘及华
日      期：2003-05-12
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
void i2c_stop(void)
{
	toggle_wd();

	IIC_SDA_SetLow();
	i2c_delay();
	IIC_SCL_SetHigh();
	i2c_delay();
	IIC_SDA_SetHigh();
	i2c_delay();

	SleepMs(5);
}

/*****************************************************************
函数原型：I2C_Ack
功能描述：向I2C总线发送一个应答信号ACK，一般用于连续数据读取时。
参数描述：
参数名称：	输入/输出？	类型		描述

返  回  值：无
作      者	：刘及华
日      期：2003-05-12
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
void i2c_ack(void)
{
	toggle_wd();

	i2c_delay();
	IIC_SDA_SetLow();
	i2c_delay();
	IIC_SCL_SetHigh();
	i2c_delay();
	IIC_SCL_SetLow();
	i2c_delay();

}

/*****************************************************************
函数原型：i2c_nack
功能描述：伪应答
参数描述：
参数名称：  输入/输出？ 类型        描述
-----------     ----------- ------      -------
// 返  回  值：无
作      者  ：刘及华
日      期：2003-05-12
修改历史：
日期        修改人      修改描述
------      ---------   -------------
*****************************************************************/
void i2c_nack(void)
{
	toggle_wd();

    i2c_delay();
	IIC_SDA_SetHigh();
    i2c_delay();
	IIC_SCL_SetHigh();
    i2c_delay();
	IIC_SCL_SetLow();
    i2c_delay();
}

/*****************************************************************
函数原型：bit    I2C_clock
功能描述：提供 I2C 总线的时钟信号，并返回在时钟电平为
高期间SDA信号线上状态，本           函数可用于数据发送，
也可用于数据接收。
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------

返  回  值：SDA信号线的状态(0|1)
作      者	：刘及华
日      期：2003-05-12
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U i2c_clock(void)
{
	INT8U h;

	toggle_wd();

	i2c_delay();
	IIC_SCL_SetHigh();
	i2c_delay();
	h = i2c_get_sda_stat();
	IIC_SCL_SetLow();
	i2c_delay();

	if (h == 0)
		return ok;
	else
		return notok;
// 	return h;
}

/*****************************************************************
函数原型：delay
功能描述：本函数实际上只有一条返回命令，在具体应用中可视具体要求增加延时指令
参数描述：无
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------

返  回  值：无
作      者	：刘及华
日      期	：2003-05-12
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
void i2c_delay(void)
{
//	INT8U i = 0;
//	toggle_wd();

//	Sleep10us(100);
//	Sleep10us(1);
	sleep_us(5);

//	for(i=0; i<20; i++)
//		;

}

/*****************************************************************
函数原型：delay
功能描述：本函数实际上只有一条返回命令，在具体应用中可视具体要求增加延时指令
参数描述：无
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------

返  回  值：无
作      者	：刘及华
日      期	：2003-05-12
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
void i2c_delay2(void)
{
//	Sleep10us(100);
//	Sleep10us(1);
	sleep_us(5);
}

/*****************************************************************
函数原型：bit   I2C_send
功能描述：向I2C总线发送8 位数据,并请求一个应答信号ACK。
			      返回SDA 信号线上状态
参数描述：发送的一字节8位发送数据
参数名称：	输入/输出？	类型		描述
data_to_send    输入	INT8U     发送的字节

返  回  值：ok(0)-正确
				notok(0xFF)-错误
作      者	：刘及华
日      期：2003-05-12
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U i2c_send(INT8U data_to_send)
{
	INT8U i;
	INT8U h;

	toggle_wd();

//	OS_ENTER_CRITICAL();
	for ( i=0; i<8; i++ )
	{
		if ( (data_to_send << i) & 0x80 )
		{
			IIC_SDA_SetHigh();
		}
		else
		{
			IIC_SDA_SetLow();
		}
		i2c_delay();
		IIC_SCL_SetHigh();
		i2c_delay();
		IIC_SCL_SetLow();
		i2c_delay();
	}
	IIC_SDA_SetHigh();

	i2c_delay();
	IIC_SCL_SetHigh();
	i2c_delay();
	h = i2c_get_sda_stat();
	IIC_SCL_SetLow();
	i2c_delay();

//	OS_EXIT_CRITICAL();
 	return h;
}

/*****************************************************************
函数原型：I2C_receive
功能描述：从I2C总线接收8位数据信号，并将接收到的8位数据作为一个字节返回，不回送应答信号ACK。
主函数在调用本函数之前应保证SDA信号线处于浮置状态，既使8051该脚电平为1。
参数描述：
参数名称：	输入/输出？	类型		描述

返  回  值：接受到的一个字节数据
作      者	：刘及华
日      期：2003-05-12
修改历史：
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U i2c_receive(void)
{
    INT8U       data_received = 0;
    INT8U       i;

	toggle_wd();


	IIC_SDA_SetHigh();

	for ( i=0; i<8; i++ )
	{
		IIC_SCL_SetHigh();
		i2c_delay();

		data_received <<= 1;

		if (i2c_get_sda_stat() == 1)
		{
			data_received += 1;
		}
		IIC_SCL_SetLow();
		i2c_delay();

	}

	return data_received;
}

/*******************************************************************
向有子地址器件读取多字节数据函数               
函数原型: bit  ISendStr(INT8U sla,INT8U suba,ucahr *s,INT8U no);  
功能:  从启动总线到发送地址，子地址,读数据，
结束总线的全过程,从器件地址sla，子地址suba，
读出的内容放入s指向的存储区，读no个字节。
如果返回1表示操作成功，否则操作有误。
注意：    使用前必须已结束总线。
********************************************************************/
INT8U i2c_send_and_receive(INT8U sla, INT8U suba, INT8U *s, INT16U no)
{
	INT8U i = 0;
	INT16U j = 0;
#ifdef I2C_USE_SEM
	INT8U err = 0;
#endif

	toggle_wd();

#ifdef I2C_USE_SEM
	OSSemPend(i2c_sem, 0, &err);
#endif
//  OS_ENTER_CRITICAL();

	i = i2c_start();					//如果i2c_start不过，重试10次
	if ( i != ok )
	{
		j = 20;
		while ( j )
		{
// 			i2c_nack();			//add by xuyan for debug
			i2c_stop();
			i = i2c_start();
			if ( i != ok )
			{
				j--;
			}
			else
			{
				break;
			}
			if ( j == 0 )
			{
				i2c_stop();
//              OS_EXIT_CRITICAL();
#ifdef I2C_USE_SEM
				OSSemPost(i2c_sem);
#endif
				return notok;
			}
		}
	}

	i = i2c_send(sla);
	if (i != ok)
	{
		i2c_stop();
//      OS_EXIT_CRITICAL();
#ifdef I2C_USE_SEM
		OSSemPost(i2c_sem);
#endif
		return notok;
	}

	i = i2c_send(suba);
	if (i != ok)
	{
		i2c_stop();
//      OS_EXIT_CRITICAL();
#ifdef I2C_USE_SEM
		OSSemPost(i2c_sem);
#endif
		return notok;
	}

	(void)i2c_start();

	i = i2c_send(sla + 1);
	if (i != ok)
	{
		i2c_stop();
//      OS_EXIT_CRITICAL();
#ifdef I2C_USE_SEM
		OSSemPost(i2c_sem);
#endif
		return notok;
	}

	for ( j=0; j<no-1; j++ )
	{
		*s=i2c_receive();				/*发送数据*/
		i2c_ack();				   /*发送就答位*/  
		s++;
	} 
	*s=i2c_receive();

	i2c_nack();					/*发送非应位*/
	i2c_stop();					   /*结束总线*/ 

//  OS_EXIT_CRITICAL();
#ifdef I2C_USE_SEM
	OSSemPost(i2c_sem);
#endif

	return ok;

}

