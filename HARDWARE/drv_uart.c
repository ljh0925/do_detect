/******************************************************************************
Copyright 2005 大连金德姆电子有限公司
All rights riserved.

文件名	：drv_uart.c
模块名称：串口驱动程序
功能概要：串口驱动程序

取代版本：0.0.1
修改人	：刘及华
完成日期：2005.04.06
升级说明：create

******************************************************************************/
#include "all.h"

//#define USE485

int UART1SendBuf[(UART1_SEND_QUEUE_LENGTH + sizeof(int)- 1)/ sizeof(int)];
int UART2SendBuf[(UART2_SEND_QUEUE_LENGTH + sizeof(int)- 1)/ sizeof(int)];
int UART3SendBuf[(UART3_SEND_QUEUE_LENGTH + sizeof(int)- 1)/ sizeof(int)];
//int UART4SendBuf[(UART4_SEND_QUEUE_LENGTH + sizeof(int)- 1)/ sizeof(int)];

int UART1RcvBuf[(UART1_RCV_QUEUE_LENGTH + sizeof(int)- 1)/ sizeof(int)];
int UART2RcvBuf[(UART2_RCV_QUEUE_LENGTH + sizeof(int)- 1)/ sizeof(int)];
int UART3RcvBuf[(UART3_RCV_QUEUE_LENGTH + sizeof(int)- 1)/ sizeof(int)];
//int UART4RcvBuf[(UART4_RCV_QUEUE_LENGTH + sizeof(int)- 1)/ sizeof(int)];



INT8U Com1SndBuf[UART1_SEND_QUEUE_LENGTH];              //串口发送缓冲区
INT8U Com2SndBuf[UART2_SEND_QUEUE_LENGTH];              //串口发送缓冲区
INT8U Com3SndBuf[UART3_SEND_QUEUE_LENGTH];              //串口发送缓冲区
//INT8U Com4SndBuf[UART4_SEND_QUEUE_LENGTH];              //串口发送缓冲区

INT8U Com1RcvBuf[UART1_RCV_QUEUE_LENGTH];               //串口接收缓冲区
INT8U Com2RcvBuf[UART2_RCV_QUEUE_LENGTH];               //串口接收缓冲区
INT8U Com3RcvBuf[UART3_RCV_QUEUE_LENGTH];               //串口接收缓冲区
//INT8U Com4RcvBuf[UART4_RCV_QUEUE_LENGTH];               //串口接收缓冲区

//INT8U TmpTBuf[UART1_SEND_QUEUE_LENGTH];         //串口发送缓冲区

extern volatile DEV_STAT DevStat;


/*********************************************************************************************************
** 函数名称: QueueCreate
** 功能描述: 初始化数据队列
** 输　入: Buf      ：为队列分配的存储空间地址
**         SizeOfBuf：为队列分配的存储空间大小（字节）
**         ReadEmpty：为队列读空时处理程序
**         WriteFull：为队列写满时处理程序
** 输　出: NOT_OK:参数错误
**         QUEUE_OK:成功
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
**
** 作　者: 陈明计
** 日　期: 2003年7月2日
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
uint8 QueueCreate(void *Buf,
				  uint32 SizeOfBuf,
				  uint8(*ReadEmpty)(),
				  uint8(*WriteFull)())
{
#if OS_CRITICAL_METHOD == 3u                     /* Allocate storage for CPU status register           */
	OS_CPU_SR  cpu_sr = 0u;
#endif
	DataQueue *Queue;

	//  ToggleWD();

	if ( Buf != NULL && SizeOfBuf >= (sizeof(DataQueue)) )        /* 判断参数是否有效 */
	{
		Queue = (DataQueue *)Buf;

		OS_ENTER_CRITICAL();
		/* 初始化结构体数据 */
		Queue->MaxData = (SizeOfBuf - (uint32)(((DataQueue *)0)->Buf)) /
		   sizeof(QUEUE_DATA_TYPE);               /* 计算队列可以存储的数据数目 */
		Queue->End = Queue->Buf + Queue->MaxData;               /* 计算数据缓冲的结束地址 */
		Queue->Out = Queue->Buf;
		Queue->In = Queue->Buf;
		Queue->NData = 0;
		Queue->ReadEmpty = ReadEmpty;
		Queue->WriteFull = WriteFull;

		OS_EXIT_CRITICAL();

		return QUEUE_OK;
	}
	else
	{
		return NOT_OK;
	}
}


/*********************************************************************************************************
** 函数名称: QueueRead
** 功能描述: 获取队列中的数据
** 输　入: Ret:存储返回的消息的地址
**         Buf:指向队列的指针
** 输　出: NOT_OK     ：参数错误
**         QUEUE_OK   ：收到消息
**         QUEUE_EMPTY：无消息
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
**
** 作　者: 陈明计
** 日　期: 2003年7月2日
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
uint8 QueueRead(QUEUE_DATA_TYPE *Ret, void *Buf)
{
#if OS_CRITICAL_METHOD == 3u                     /* Allocate storage for CPU status register           */
	OS_CPU_SR  cpu_sr = 0u;
#endif
	uint8 err;
	DataQueue *Queue;

	//  ToggleWD();

	err = NOT_OK;
	if ( Buf != NULL )                                            /* 队列是否有效 */
	{
		//  	ToggleWD();
		/* 有效 */
		Queue = (DataQueue *)Buf;

		OS_ENTER_CRITICAL();

		if ( Queue->NData > 0 )                                   /* 队列是否为空 */
		{
			/* 不空         */
			*Ret = Queue->Out[0];                               /* 数据出队     */
			Queue->Out++;                                       /* 调整出队指针 */
			if ( Queue->Out >= Queue->End )
			{
				Queue->Out = Queue->Buf;
			}
			Queue->NData--;                                     /* 数据减少      */
			err = QUEUE_OK;
		}
		else
		{                                                       /* 空              */
			err = QUEUE_EMPTY;
			if ( Queue->ReadEmpty != NULL )                       /* 调用用户处理函数 */
			{
				err = Queue->ReadEmpty(Ret, Queue);
			}
		}
		OS_EXIT_CRITICAL();
	}
	return err;
}

/*********************************************************************************************************
** 函数名称: QueueWrite
** 功能描述: FIFO方式发送数据
** 输　入: Buf :指向队列的指针
**         Data:消息数据
** 输　出: NOT_OK   :参数错误
**         QUEUE_FULL:队列满
**         QUEUE_OK  :发送成功
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
**
** 作　者: 陈明计
** 日　期: 2003年7月2日
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#ifndef EN_QUEUE_WRITE
#define EN_QUEUE_WRITE      0
#endif

#if EN_QUEUE_WRITE > 0
uint8 QueueWrite(void *Buf, QUEUE_DATA_TYPE Data)
{
#if OS_CRITICAL_METHOD == 3u                     /* Allocate storage for CPU status register           */
	OS_CPU_SR  cpu_sr = 0u;
#endif
	uint8 err;
	DataQueue *Queue;

	//  ToggleWD();

	err = NOT_OK;
	if ( Buf != NULL )                                                    /* 队列是否有效 */
	{
		//  	ToggleWD();

		Queue = (DataQueue *)Buf;

		OS_ENTER_CRITICAL();

		if ( Queue->NData < Queue->MaxData )                              /* 队列是否满  */
		{
			/* 不满        */
			Queue->In[0] = Data;                                        /* 数据入队    */
			Queue->In++;                                                /* 调整入队指针*/
			if ( Queue->In >= Queue->End )
			{
				Queue->In = Queue->Buf;
			}
			Queue->NData++;                                             /* 数据增加    */
			err = QUEUE_OK;
		}
		else
		{                                                               /* 满           */
			err = QUEUE_FULL;
			if ( Queue->WriteFull != NULL )                               /* 调用用户处理函数 */
			{
				err = Queue->WriteFull(Queue, Data, Q_WRITE_MODE);
			}
		}
		OS_EXIT_CRITICAL();
	}
	return err;
}
#endif

/*********************************************************************************************************
** 函数名称: QueueWriteFront
** 功能描述: LIFO方式发送数据
** 输　入: Buf:指向队列的指针
**         Data:消息数据
** 输　出: QUEUE_FULL:队列满
**         QUEUE_OK:发送成功
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
**
** 作　者: 陈明计
** 日　期: 2003年7月2日
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#ifndef EN_QUEUE_WRITE_FRONT
#define EN_QUEUE_WRITE_FRONT    0
#endif

#if EN_QUEUE_WRITE_FRONT > 0

uint8 QueueWriteFront(void *Buf, QUEUE_DATA_TYPE Data)
{
#if OS_CRITICAL_METHOD == 3u                     /* Allocate storage for CPU status register           */
	OS_CPU_SR  cpu_sr = 0u;
#endif
	uint8 err;
	DataQueue *Queue;

	//  ToggleWD();

	err = NOT_OK;
	if ( Buf != NULL )                                                    /* 队列是否有效 */
	{
		Queue = (DataQueue *)Buf;

		OS_ENTER_CRITICAL();

		if ( Queue->NData < Queue->MaxData )                              /* 队列是否满  */
		{
			/* 不满 */
			Queue->Out--;                                               /* 调整出队指针 */
			if ( Queue->Out < Queue->Buf )
			{
				Queue->Out = Queue->End - 1;
			}
			Queue->Out[0] = Data;                                       /* 数据入队     */
			Queue->NData++;                                             /* 数据数目增加 */
			err = QUEUE_OK;
		}
		else
		{                                                               /* 满           */
			err = QUEUE_FULL;
			if ( Queue->WriteFull != NULL )                               /* 调用用户处理函数 */
			{
				err = Queue->WriteFull(Queue, Data, Q_WRITE_FRONT_MODE);
			}
		}
		OS_EXIT_CRITICAL();
	}
	return err;
}

#endif

/*********************************************************************************************************
** 函数名称: QueueNData
** 功能描述: 取得队列中数据数
** 输　入: Buf:指向队列的指针
** 输　出: 消息数
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
**
** 作　者: 陈明计
** 日　期: 2003年7月2日
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#ifndef EN_QUEUE_NDATA
#define EN_QUEUE_NDATA    0
#endif

#if EN_QUEUE_NDATA > 0

uint16 QueueNData(void *Buf)
{
#if OS_CRITICAL_METHOD == 3u                     /* Allocate storage for CPU status register           */
	OS_CPU_SR  cpu_sr = 0u;
#endif
	uint16 temp;

	//  ToggleWD();

	temp = 0;                                                   /* 队列无效返回0 */
	if ( Buf != NULL )
	{
		OS_ENTER_CRITICAL();
		temp = ((DataQueue *)Buf)->NData;
		OS_EXIT_CRITICAL();
	}
	return temp;
}

#endif

/*********************************************************************************************************
** 函数名称: QueueSize
** 功能描述: 取得队列总容量
** 输　入: Buf:指向队列的指针
** 输　出: 队列总容量
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
**
** 作　者: 陈明计
** 日　期: 2003年7月2日
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#ifndef EN_QUEUE_SIZE
#define EN_QUEUE_SIZE    0
#endif

#if EN_QUEUE_SIZE > 0

uint16 QueueSize(void *Buf)
{
#if OS_CRITICAL_METHOD == 3u                     /* Allocate storage for CPU status register           */
	OS_CPU_SR  cpu_sr = 0u;
#endif
	uint16 temp;

	//  ToggleWD();

	temp = 0;                                                   /* 队列无效返回0 */
	if ( Buf != NULL )
	{
		OS_ENTER_CRITICAL();
		temp = ((DataQueue *)Buf)->MaxData;
		OS_EXIT_CRITICAL();
	}
	return temp;
}

#endif

/*********************************************************************************************************
** 函数名称: OSQFlush
** 功能描述: 清空队列
** 输　入: Buf:指向队列的指针
** 输　出: 无
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
**
** 作　者: 陈明计
** 日　期: 2003年7月2日
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#ifndef EN_QUEUE_FLUSH
#define EN_QUEUE_FLUSH    0
#endif

#if EN_QUEUE_FLUSH > 0

void QueueFlush(void *Buf)
{
#if OS_CRITICAL_METHOD == 3u                     /* Allocate storage for CPU status register           */
	OS_CPU_SR  cpu_sr = 0u;
#endif
	DataQueue *Queue;

	//  ToggleWD();

	if ( Buf != NULL )                                                /* 队列是否有效 */
	{
		//  	ToggleWD();
		/* 有效         */
		Queue = (DataQueue *)Buf;
		OS_ENTER_CRITICAL();
		Queue->Out = Queue->Buf;
		Queue->In = Queue->Buf;
		Queue->NData = 0;                                           /* 数据数目为0 */
		OS_EXIT_CRITICAL();
	}
}
#endif

/*********************************************************************************************************
** 函数名称: Uart1WriteFull
** 功能描述: 数据队列写满处理程序 
** 输　入: Buf:指向数据队列
**        Data:将要入队的数据
**        Mod: 入队方式
** 输　出:TRUE  :成功
**        FALSE:失败
** 全局变量: 无
** 调用模块: 无
**-------------------------------------------------------------------------------------------------------
** 修改人: 刘及华
** 日　期: 2004年12月28日
**-------------------------------------------------------------------------------------------------------
** 修改人: 刘及华
** 日　期: 2004年12月28日
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
uint8 Uart1WriteFull(DataQueue *Buf, QUEUE_DATA_TYPE Data, uint8 Mod)
{
	//  uint16 temp;

	Mod = Mod;

	//  ToggleWD();

	//    temp = QueueSize((void *)Buf);
	//    while ( temp <= QueueNData((void *)Buf) )         /* 等待数据队列不满 */
	//    {
	////  	OSTimeDly(2);
	//    }
	//    return QueueWrite((void *)Buf, Data);           /* 数据重新入队 */

	return ok;
}

/******************************************************************************
 函数名称：uart1_init
 功能描述：初始化UART1
 参数描述：
 参数名称： 输入/输出？ 类型		描述
				
 返  回  值：无
				   
 作 	 者 ：刘及华
 日 	 期：2005-04-11
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void uart1_init(void)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	(void)QueueCreate((void *)UART1SendBuf, sizeof(UART1SendBuf), NULL, (uint8 (*)())Uart1WriteFull);
	(void)QueueCreate((void *)UART1RcvBuf, sizeof(UART1RcvBuf), NULL, (uint8 (*)())Uart1WriteFull);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);   //使能USART1，GPIOA时钟
	USART_DeInit(USART1);  //复位串口1

	//USART1_TX   PA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9

	//USART1_RX	  PA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10


	//USART 初始化设置
//	USART_InitStructure.USART_BaudRate = 9600; //一般设置为9600;
	USART_InitStructure.USART_BaudRate = 115200; //一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No; //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;      //子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化VIC寄存器

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //开启接收中断

	USART_Cmd(USART1, ENABLE);                    //使能串口


}

/******************************************************************************
 函数名称：uart1_set_bps
 功能描述：设置串口1波特率，和奇偶校验方式
 参数描述：
 参数名称： 输入/输出？ 类型		描述
 bps		输入		INT32U		要设置的波特率  
 databit	输入		INT8U		数据位可选8 9
 stopbit	输入		INT8U		停止位可选1 2 
 odd_even	输入		INT8U		校验方式：NONE为无，ODD为奇校验，EVEN为偶校验
				
 返  回  值：无
				   
 作 	 者 ：刘及华
 日 	 期：2005-04-11
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void uart1_set_bps(INT32U bps, INT8U databit, INT8U stopbit, INT8U odd_even)
{
	USART_InitTypeDef USART_InitStructure;

	ToggleWD();


	USART_InitStructure.USART_BaudRate = bps;

	if ( databit == 8 )
	{
		USART_InitStructure.USART_WordLength = USART_WordLength_8b; //字长为8位数据格式
	}
	else
	{
		USART_InitStructure.USART_WordLength = USART_WordLength_9b; //字长为9位数据格式
	}

	if ( stopbit == 1 )
	{
		USART_InitStructure.USART_StopBits = USART_StopBits_1; //一个停止位
	}
	else
	{
		USART_InitStructure.USART_StopBits = USART_StopBits_2; //两个停止位
	}

	if ( odd_even == NONE )
	{
		USART_InitStructure.USART_Parity = USART_Parity_No; //无奇偶校验位
	}
	else if ( odd_even == ODD )
	{
		USART_InitStructure.USART_Parity = USART_Parity_Odd; //奇校验
	}
	else
	{
		USART_InitStructure.USART_Parity = USART_Parity_Even; //偶校验
	}

	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式

	USART_Init(USART1, &USART_InitStructure); //初始化串口
}

/******************************************************************************
 函数名称：uart1_buf_flush
 功能描述：清空串口1缓存
 参数描述：
 参数名称： 输入/输出？ 类型		描述

 返  回  值：无

 作 	 者 ：刘及华
 日 	 期：2005-04-11
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void uart1_buf_flush(void)
{
	//  INT8U i = 0;
	//  volatile INT8U j = 0;

	ToggleWD();

	//  U0IER = 0x00;								/* 关闭接收和发送中断 */
	//  U0FCR = 0x03;								/* 初始化FIFO 1字节中断 清RX FIFO*/
	//  for ( i=0; i<17; i++ )
	//  {
	//  	j = U0RBR;			//读空FIFO
	//  }
	//  U0IER = 0x05;								/* 允许接收和发送中断 */

	QueueFlush(UART1RcvBuf);
}

/******************************************************************************
 函数名称：uart1_putch
 功能描述：串口1发送一个字节数据
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
void uart1_putch(INT8U dat)
{
//	ToggleWD();
//
//	QueueWrite((void *)UART1SendBuf, dat);         /* 数据入队 */
//
//	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);  /* 允许发送中断 */

	toggle_wd();
	USART_SendData(USART1, dat);
	while( USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET )			//等待发送完成
		;
}

/******************************************************************************
 函数名称：uart1_write
 功能描述：串口1发送一串数据
 参数描述：
 参数名称： 输入/输出？ 类型		描述
 snd_buf	输入		INT8U *		要发送的数据
 snd_length	输入		INT16U		要发送数据的长度
				
 返  回  值：无
				   
 作 	 者 ：刘及华
 日 	 期：2012-04-06
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void uart1_write(INT8U const * const snd_buf, INT16U const snd_length)
{
//	INT16U i = 0;
//
//	ToggleWD();
//
//	for ( i = 0; i < snd_length; i++ )
//	{
//		uart1_putch(snd_buf[i]);
//	}
//
	INT16U i = 0;

	ToggleWD();

	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);  /* 禁止接收中断 */
	for ( i = 0; i < snd_length; i++ )
	{
		uart1_putch(snd_buf[i]);
	}
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  /* 允许接收中断 */
}

/******************************************************************************
 函数名称：USART1_IRQHandler
 功能描述：UART1中断服务程序
 参数描述：
 参数名称： 输入/输出？ 类型		描述
				
 返  回  值：无
				   
 作 	 者 ：刘及华
 日 	 期：2012-04-06
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
//void uart0_handler (void) __irq
void USART1_IRQHandler(void)
{
	//    INT8U IIR, temp, i;
	//
	////  ToggleWD();
	//
	//    VICIntEnClr = (1 << UART0_INT);     //禁止UART0中断
	////  OS_ENTER_CRITICAL();
	//
	//    while ( ((IIR = U0IIR) & 0x01) == 0 )
	//    {                                                   /* 有中断未处理完 */
	////  	ToggleWD();
	//
	//        switch ( IIR & 0x0e )
	//        {
	//            case 0x02:                                  /* THRE中断    */
	//                for ( i = 0; i < UART0_FIFO_LENGTH; i++ ) /* 向发送FIFO填充数据 */
	//                {
	//                    if ( QueueRead(&temp, UART0SendBuf) == QUEUE_OK )
	//                    {
	//                        U0THR = temp;
	//                    }
	//                    else
	//                    {
	//                        U0IER = U0IER & (~0x02);        /* 队列空，则禁止发送中断 */
	//                    }
	//                }
	//                break;
	//
	//            case 0x04:                                  /* 接收数据可用 */
	//                i = U0RBR;
	//                QueueWrite((void *)UART0RcvBuf, i);         /* 数据入队 */
	//
	//                T0TC = 0;
	//                T0TCR = 0x01;                   //start timer0
	//                break;
	//
	//            case 0x06:                                  /* 接收线状态   */
	//                temp = U0LSR;
	//                break;
	//
	//            case 0x0c:                                  /* 字符超时指示 */
	//                i = U0RBR;
	//                QueueWrite((void *)UART0RcvBuf, i);         /* 数据入队 */
	//
	//                T0TC = 0;
	//                T0TCR = 0x01;                   //start timer0
	//                break;
	//
	//            default :
	//                break;
	//        }
	//    }
	//    VICIntEnable = (1 << UART0_INT);        //使能UART0中断
	//    VICVectAddr = 0;            // 通知中断控制器中断结束
	//
	////  OS_EXIT_CRITICAL();

	INT8U rcv = 0;
	INT8U snd = 0;

	//处理接收到的数据
	if ( USART_GetITStatus(USART1, USART_IT_RXNE) != RESET )
	{
		rcv = USART_ReceiveData(USART1);        //(USART1->DR);	//读取接收到的数据
		QueueWrite((void *)UART1RcvBuf, rcv);         /* 数据入队 */

		//  	/* Clear the USART1 Receive interrupt */
		//      USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}


	//发送中断
	if ( USART_GetITStatus(USART1, USART_IT_TXE) != RESET )
	{
		if ( QueueRead(&snd, UART1SendBuf) == QUEUE_OK )
		{
			USART_SendData(USART1, snd);
		}
		else
		{
			/* 队列空，则禁止发送中断 */
			USART_ClearITPendingBit(USART1, USART_IT_TXE);
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
		}

	}

}

/******************************************************************************
 函数名称：com1_send_hex
 功能描述：
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
void com1_send_hex(INT8U *str_to_send, INT16U len)
{
	INT16U i = 0;

	ToggleWD();

	for ( i = 0; i < len; i++ )
	{
		uart1_putch(str_to_send[i]);
	}
}

/*****************************************************************
函数原型：com1_gets
功能描述：串口1接收
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
INT8U com1_gets(INT8U *str_got, INT16U *rcv_len, INT32U timeout_start, INT16U timeout)
{
	INT8U i = 0;
	INT16U len = 0;
	INT32U timeout_cntr = 0;

	ToggleWD();

	//第一个字节
	for(;;)
	{
		if( QueueRead(&i, UART1RcvBuf) == QUEUE_OK )
		{
			str_got[len++] = i;
			break;
		}
		else
		{
			toggle_wd();
			if (timeout_start == 0)
			{
				OSTimeDlyHMSM(0, 0, 0, 5);
			}
			else
			{
				timeout_cntr++;
				if (timeout_cntr > timeout_start / 5)          //超时
				{
					str_got[len] = '\0';
					*rcv_len = len;
					return notok;
				}
				OSTimeDlyHMSM(0, 0, 0, 5);
			}
		}
	}

	//其余字节
	timeout_cntr = 0;
	for(;;)
	{
		if( QueueRead(&i, UART1RcvBuf) == QUEUE_OK )
		{
			str_got[len++] = i;
			timeout_cntr = 0;
		}
		else
		{
			toggle_wd();
			timeout_cntr++;
			if (timeout_cntr > timeout / 5)            //超时
			{
				str_got[len] = '\0';
				*rcv_len = len;
				return ok;
			}
			OSTimeDlyHMSM(0, 0, 0, 5);
		}
	}

}

/*********************************************************************************************************
** 函数名称: Uart2WriteFull
** 功能描述: 数据队列写满处理程序 
** 输　入: Buf:指向数据队列
**        Data:将要入队的数据
**        Mod: 入队方式
** 输　出:TRUE  :成功
**        FALSE:失败
** 全局变量: 无
** 调用模块: 无
**-------------------------------------------------------------------------------------------------------
** 修改人: 刘及华
** 日　期: 2004年12月28日
**-------------------------------------------------------------------------------------------------------
** 修改人: 刘及华
** 日　期: 2004年12月28日
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
uint8 Uart2WriteFull(DataQueue *Buf, QUEUE_DATA_TYPE Data, uint8 Mod)
{
	//  uint16 temp;

	Mod = Mod;

	//  ToggleWD();

	//    temp = QueueSize((void *)Buf);
	//    while ( temp <= QueueNData((void *)Buf) )         /* 等待数据队列不满 */
	//    {
	////  	OSTimeDly(2);
	//    }
	//    return QueueWrite((void *)Buf, Data);           /* 数据重新入队 */

	return ok;
}

/******************************************************************************
 函数名称：uart2_init
 功能描述：初始化UART2
 参数描述：
 参数名称： 输入/输出？ 类型		描述
				
 返  回  值：无
				   
 作 	 者 ：刘及华
 日 	 期：2005-04-11
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void uart2_init(void)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	(void)QueueCreate((void *)UART2SendBuf, sizeof(UART2SendBuf), NULL, (uint8 (*)())Uart2WriteFull);
	(void)QueueCreate((void *)UART2RcvBuf, sizeof(UART2RcvBuf), NULL, (uint8 (*)())Uart2WriteFull);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//使能USART2时钟

	USART_DeInit(USART2);  //复位串口2

	//USART2_TX   PA.2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 				//PA.2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure); 					//初始化

	//USART2_RX	  PA.3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//初始化


	//USART 初始化设置
//	USART_InitStructure.USART_BaudRate = 9600; 							//一般设置为9600;
	USART_InitStructure.USART_BaudRate = 38400; 							//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;			//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;				//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;					//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//收发模式
	USART_Init(USART2, &USART_InitStructure);							//初始化串口

	//Usart2 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;      //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化VIC寄存器

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //开启接收中断

	USART_Cmd(USART2, ENABLE);                    //使能串口


}

/******************************************************************************
 函数名称：uart2_set_bps
 功能描述：设置串口2波特率，和奇偶校验方式
 参数描述：
 参数名称： 输入/输出？ 类型		描述
 bps		输入		INT32U		要设置的波特率  
 databit	输入		INT8U		数据位可选8 9
 stopbit	输入		INT8U		停止位可选1 2 
 odd_even	输入		INT8U		校验方式：NONE为无，ODD为奇校验，EVEN为偶校验
				
 返  回  值：无
				   
 作 	 者 ：刘及华
 日 	 期：2005-04-11
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void uart2_set_bps(INT32U bps, INT8U databit, INT8U stopbit, INT8U odd_even)
{
	USART_InitTypeDef USART_InitStructure;

	ToggleWD();


	USART_InitStructure.USART_BaudRate = bps;

	if ( databit == 8 )
	{
		USART_InitStructure.USART_WordLength = USART_WordLength_8b; //字长为8位数据格式
	}
	else
	{
		USART_InitStructure.USART_WordLength = USART_WordLength_9b; //字长为9位数据格式
	}

	if ( stopbit == 1 )
	{
		USART_InitStructure.USART_StopBits = USART_StopBits_1; //一个停止位
	}
	else
	{
		USART_InitStructure.USART_StopBits = USART_StopBits_2; //两个停止位
	}

	if ( odd_even == NONE )
	{
		USART_InitStructure.USART_Parity = USART_Parity_No; //无奇偶校验位
	}
	else if ( odd_even == ODD )
	{
		USART_InitStructure.USART_Parity = USART_Parity_Odd; //奇校验
	}
	else
	{
		USART_InitStructure.USART_Parity = USART_Parity_Even; //偶校验
	}

	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式

	USART_Init(USART2, &USART_InitStructure); //初始化串口
}

/******************************************************************************
 函数名称：uart2_buf_flush
 功能描述：清空串口2缓存
 参数描述：
 参数名称： 输入/输出？ 类型		描述

 返  回  值：无

 作 	 者 ：刘及华
 日 	 期：2005-04-11
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void uart2_buf_flush(void)
{
	//  INT8U i = 0;
	//  volatile INT8U j = 0;

	ToggleWD();

	//  U0IER = 0x00;								/* 关闭接收和发送中断 */
	//  U0FCR = 0x03;								/* 初始化FIFO 1字节中断 清RX FIFO*/
	//  for ( i=0; i<17; i++ )
	//  {
	//  	j = U0RBR;			//读空FIFO
	//  }
	//  U0IER = 0x05;								/* 允许接收和发送中断 */

	QueueFlush(UART2RcvBuf);
}

/******************************************************************************
 函数名称：uart2_putch
 功能描述：串口2发送一个字节数据
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
void uart2_putch(INT8U dat)
{
	ToggleWD();

	QueueWrite((void *)UART2SendBuf, dat);         /* 数据入队 */

	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);  /* 允许发送中断 */
}

/******************************************************************************
 函数名称：uart2_write
 功能描述：串口2发送一串数据
 参数描述：
 参数名称： 输入/输出？ 类型		描述
 snd_buf	输入		INT8U *		要发送的数据
 snd_length	输入		INT16U		要发送数据的长度
				
 返  回  值：无
				   
 作 	 者 ：刘及华
 日 	 期：2012-04-06
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void uart2_write(INT8U const * const snd_buf, INT16U const snd_length)
{
	INT16U i = 0;

	ToggleWD();

	for ( i = 0; i < snd_length; i++ )
	{
		uart2_putch(snd_buf[i]);
	}
}

/******************************************************************************
 函数名称：USART2_IRQHandler
 功能描述：UART2中断服务程序
 参数描述：
 参数名称： 输入/输出？ 类型		描述
				
 返  回  值：无
				   
 作 	 者 ：刘及华
 日 	 期：2012-04-06
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void USART2_IRQHandler(void)
{
	INT8U rcv = 0;
	INT8U snd = 0;

	//处理接收到的数据
	if ( USART_GetITStatus(USART2, USART_IT_RXNE) != RESET )
	{
		rcv = USART_ReceiveData(USART2);        //读取接收到的数据
		QueueWrite((void *)UART2RcvBuf, rcv);         /* 数据入队 */
	}

	//发送中断
	if ( USART_GetITStatus(USART2, USART_IT_TXE) != RESET )
	{
		if ( QueueRead(&snd, UART2SendBuf) == QUEUE_OK )
		{
			USART_SendData(USART2, snd);
		}
		else
		{
			/* 队列空，则禁止发送中断 */
			USART_ClearITPendingBit(USART2, USART_IT_TXE);
			USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
		}
	}

}

/******************************************************************************
 函数名称：com2_send_hex
 功能描述：串口2发送一串数据
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
void com2_send_hex(INT8U *str_to_send, INT16U len)
{
	INT16U i = 0;

	ToggleWD();

	for ( i = 0; i < len; i++ )
	{
		uart2_putch(str_to_send[i]);
	}
}

/*****************************************************************
函数原型：com2_gets
功能描述：串口2接收
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
INT8U com2_gets(INT8U *str_got, INT16U *rcv_len, INT32U timeout_start, INT16U timeout)
{
	INT8U i = 0;
	INT16U len = 0;
	INT32U timeout_cntr = 0;

	ToggleWD();

	//第一个字节
	for(;;)
	{
		if( QueueRead(&i, UART2RcvBuf) == QUEUE_OK )
		{
			str_got[len++] = i;
			break;
		}
		else
		{
			if (timeout_start == 0)
			{
				OSTimeDlyHMSM(0, 0, 0, 5);
			}
			else
			{
				timeout_cntr++;
				if (timeout_cntr > timeout_start / 5)          //超时
				{
					str_got[len] = '\0';
					*rcv_len = len;
					return notok;
				}
				OSTimeDlyHMSM(0, 0, 0, 5);
			}
		}
	}

	//其余字节
	timeout_cntr = 0;
	for(;;)
	{
		if( QueueRead(&i, UART2RcvBuf) == QUEUE_OK )
		{
			str_got[len++] = i;
			timeout_cntr = 0;
		}
		else
		{
			timeout_cntr++;
			if (timeout_cntr > timeout / 5)            //超时
			{
				str_got[len] = '\0';
				*rcv_len = len;
				return ok;
			}
			OSTimeDlyHMSM(0, 0, 0, 5);
		}
	}

}

/*********************************************************************************************************
** 函数名称: Uart3WriteFull
** 功能描述: 数据队列写满处理程序 
** 输　入: Buf:指向数据队列
**        Data:将要入队的数据
**        Mod: 入队方式
** 输　出:TRUE  :成功
**        FALSE:失败
** 全局变量: 无
** 调用模块: 无
**-------------------------------------------------------------------------------------------------------
** 修改人: 刘及华
** 日　期: 2004年12月28日
**-------------------------------------------------------------------------------------------------------
** 修改人: 刘及华
** 日　期: 2004年12月28日
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
uint8 Uart3WriteFull(DataQueue *Buf, QUEUE_DATA_TYPE Data, uint8 Mod)
{
	//  uint16 temp;

	Mod = Mod;

	//  ToggleWD();

	//    temp = QueueSize((void *)Buf);
	//    while ( temp <= QueueNData((void *)Buf) )         /* 等待数据队列不满 */
	//    {
	////  	OSTimeDly(2);
	//    }
	//    return QueueWrite((void *)Buf, Data);           /* 数据重新入队 */

	return ok;
}

/******************************************************************************
 函数名称：uart3_init
 功能描述：初始化UART3
 参数描述：
 参数名称： 输入/输出？ 类型		描述
				
 返  回  值：无
				   
 作 	 者 ：刘及华
 日 	 期：2005-04-11
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void uart3_init(void)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	(void)QueueCreate((void *)UART3SendBuf, sizeof(UART3SendBuf), NULL, (uint8 (*)())Uart3WriteFull);
	(void)QueueCreate((void *)UART3RcvBuf, sizeof(UART3RcvBuf), NULL, (uint8 (*)())Uart3WriteFull);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   //使能GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//使能USART3时钟

	USART_DeInit(USART3);  //复位串口3

	//USART3_TX   PB.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 				//PB.10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		//复用推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure); 					//初始化

	//USART3_RX	  PB.11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//初始化


	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = 9600; 							//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;			//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;				//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;					//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//收发模式
	USART_Init(USART3, &USART_InitStructure);							//初始化串口

	//Usart2 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;      //子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化VIC寄存器

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //开启接收中断

	USART_Cmd(USART3, ENABLE);                    //使能串口
}

/******************************************************************************
 函数名称：uart3_set_bps
 功能描述：设置串口3波特率，和奇偶校验方式
 参数描述：
 参数名称： 输入/输出？ 类型		描述
 bps		输入		INT32U		要设置的波特率  
 databit	输入		INT8U		数据位可选8 9
 stopbit	输入		INT8U		停止位可选1 2 
 odd_even	输入		INT8U		校验方式：NONE为无，ODD为奇校验，EVEN为偶校验
				
 返  回  值：无
				   
 作 	 者 ：刘及华
 日 	 期：2005-04-11
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void uart3_set_bps(INT32U bps, INT8U databit, INT8U stopbit, INT8U odd_even)
{
	USART_InitTypeDef USART_InitStructure;

	ToggleWD();


	USART_InitStructure.USART_BaudRate = bps;

	if ( databit == 8 )
	{
		USART_InitStructure.USART_WordLength = USART_WordLength_8b; //字长为8位数据格式
	}
	else
	{
		USART_InitStructure.USART_WordLength = USART_WordLength_9b; //字长为9位数据格式
	}

	if ( stopbit == 1 )
	{
		USART_InitStructure.USART_StopBits = USART_StopBits_1; //一个停止位
	}
	else
	{
		USART_InitStructure.USART_StopBits = USART_StopBits_2; //两个停止位
	}

	if ( odd_even == NONE )
	{
		USART_InitStructure.USART_Parity = USART_Parity_No; //无奇偶校验位
	}
	else if ( odd_even == ODD )
	{
		USART_InitStructure.USART_Parity = USART_Parity_Odd; //奇校验
	}
	else
	{
		USART_InitStructure.USART_Parity = USART_Parity_Even; //偶校验
	}

	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式

	USART_Init(USART3, &USART_InitStructure); //初始化串口
}

/******************************************************************************
 函数名称：uart3_buf_flush
 功能描述：清空串口3缓存
 参数描述：
 参数名称： 输入/输出？ 类型		描述

 返  回  值：无

 作 	 者 ：刘及华
 日 	 期：2005-04-11
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void uart3_buf_flush(void)
{
	//  INT8U i = 0;
	//  volatile INT8U j = 0;

	ToggleWD();

	//  U0IER = 0x00;								/* 关闭接收和发送中断 */
	//  U0FCR = 0x03;								/* 初始化FIFO 1字节中断 清RX FIFO*/
	//  for ( i=0; i<17; i++ )
	//  {
	//  	j = U0RBR;			//读空FIFO
	//  }
	//  U0IER = 0x05;								/* 允许接收和发送中断 */

	QueueFlush(UART3RcvBuf);
}

/******************************************************************************
 函数名称：uart3_putch
 功能描述：串口3发送一个字节数据
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
void uart3_putch(INT8U dat)
{
//	ToggleWD();
//
//	QueueWrite((void *)UART3SendBuf, dat);         /* 数据入队 */
//
//	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);  /* 允许发送中断 */

	USART_SendData(USART3, dat);
	while( USART_GetFlagStatus(USART3, USART_FLAG_TC) != SET )			//等待发送完成
		;
}

/******************************************************************************
 函数名称：uart3_write
 功能描述：串口3发送一串数据
 参数描述：
 参数名称： 输入/输出？ 类型		描述
 snd_buf	输入		INT8U *		要发送的数据
 snd_length	输入		INT16U		要发送数据的长度
				
 返  回  值：无
				   
 作 	 者 ：刘及华
 日 	 期：2012-04-06
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void uart3_write(INT8U const * const snd_buf, INT16U const snd_length)
{
//	INT16U i = 0;
//
//	ToggleWD();
//
//	for ( i = 0; i < snd_length; i++ )
//	{
//		uart3_putch(snd_buf[i]);
//	}

	INT16U i = 0;

	ToggleWD();

	USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);  /* 禁止接收中断 */
	for ( i = 0; i < snd_length; i++ )
	{
		uart3_putch(snd_buf[i]);
	}
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);  /* 允许接收中断 */
}

/******************************************************************************
 函数名称：USART3_IRQHandler
 功能描述：UART3中断服务程序
 参数描述：
 参数名称： 输入/输出？ 类型		描述
				
 返  回  值：无
				   
 作 	 者 ：刘及华
 日 	 期：2012-04-06
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void USART3_IRQHandler(void)
{
	INT8U rcv = 0;
	INT8U snd = 0;

	//处理接收到的数据
	if ( USART_GetITStatus(USART3, USART_IT_RXNE) != RESET )
	{
		rcv = USART_ReceiveData(USART3);        //读取接收到的数据
		QueueWrite((void *)UART3RcvBuf, rcv);         /* 数据入队 */
	}

	//发送中断
	if ( USART_GetITStatus(USART3, USART_IT_TXE) != RESET )
	{
		if ( QueueRead(&snd, UART3SendBuf) == QUEUE_OK )
		{
			USART_SendData(USART3, snd);
		}
		else
		{
			/* 队列空，则禁止发送中断 */
			USART_ClearITPendingBit(USART3, USART_IT_TXE);
			USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
		}
	}

}

/******************************************************************************
 函数名称：com3_send_hex
 功能描述：串口3发送一串数据
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
void com3_send_hex(INT8U *str_to_send, INT16U len)
{
	INT16U i = 0;

	ToggleWD();

	for ( i = 0; i < len; i++ )
	{
		uart3_putch(str_to_send[i]);
	}
}

/*****************************************************************
函数原型：com3_gets
功能描述：串口3接收
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
INT8U com3_gets(INT8U *str_got, INT16U *rcv_len, INT32U timeout_start, INT16U timeout)
{
	INT8U i = 0;
	INT16U len = 0;
	INT32U timeout_cntr = 0;

	ToggleWD();

	//第一个字节
	for(;;)
	{
		if( QueueRead(&i, UART3RcvBuf) == QUEUE_OK )
		{
			str_got[len++] = i;
			break;
		}
		else
		{
			if (timeout_start == 0)
			{
				OSTimeDlyHMSM(0, 0, 0, 5);
			}
			else
			{
				timeout_cntr++;
				if (timeout_cntr > timeout_start / 5)          //超时
				{
					str_got[len] = '\0';
					*rcv_len = len;
					return notok;
				}
				OSTimeDlyHMSM(0, 0, 0, 5);
			}
		}
	}

	//其余字节
	timeout_cntr = 0;
	for(;;)
	{
		if( QueueRead(&i, UART3RcvBuf) == QUEUE_OK )
		{
			str_got[len++] = i;
			timeout_cntr = 0;
		}
		else
		{
			timeout_cntr++;
			if (timeout_cntr > timeout / 5)            //超时
			{
				str_got[len] = '\0';
				*rcv_len = len;
				return ok;
			}
			OSTimeDlyHMSM(0, 0, 0, 5);
		}
	}

}

///*********************************************************************************************************
//** 函数名称: Uart4WriteFull
//** 功能描述: 数据队列写满处理程序
//** 输　入: Buf:指向数据队列
//**        Data:将要入队的数据
//**        Mod: 入队方式
//** 输　出:TRUE  :成功
//**        FALSE:失败
//** 全局变量: 无
//** 调用模块: 无
//**-------------------------------------------------------------------------------------------------------
//** 修改人: 刘及华
//** 日　期: 2004年12月28日
//**-------------------------------------------------------------------------------------------------------
//** 修改人: 刘及华
//** 日　期: 2004年12月28日
//**------------------------------------------------------------------------------------------------------
//********************************************************************************************************/
//uint8 Uart4WriteFull(DataQueue *Buf, QUEUE_DATA_TYPE Data, uint8 Mod)
//{
//	//  uint16 temp;
//
//	Mod = Mod;
//
//	//  ToggleWD();
//
//	//    temp = QueueSize((void *)Buf);
//	//    while ( temp <= QueueNData((void *)Buf) )         /* 等待数据队列不满 */
//	//    {
//	////  	OSTimeDly(2);
//	//    }
//	//    return QueueWrite((void *)Buf, Data);           /* 数据重新入队 */
//
//	return ok;
//}
//
///******************************************************************************
// 函数名称：uart4_init
// 功能描述：初始化UART4
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
//void uart4_init(void)
//{
//	//GPIO端口设置
//	GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//
//	(void)QueueCreate((void *)UART4SendBuf, sizeof(UART4SendBuf), NULL, (uint8 (*)())Uart4WriteFull);
//	(void)QueueCreate((void *)UART4RcvBuf, sizeof(UART4RcvBuf), NULL, (uint8 (*)())Uart4WriteFull);
//
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);   //使能GPIOC时钟
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);	//使能USART4时钟
//
//	USART_DeInit(UART4);  //复位串口4
//
//	//USART4_TX   PC.10
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 				//PC.10
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		//复用推挽输出
//	GPIO_Init(GPIOC, &GPIO_InitStructure); 					//初始化
//
//	//USART4_RX	  PC.11
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
//	GPIO_Init(GPIOC, &GPIO_InitStructure);					//初始化
//
//
//	//USART 初始化设置
////  USART_InitStructure.USART_BaudRate = 9600; 							//一般设置为9600;
//	USART_InitStructure.USART_BaudRate = 115200; 							//一般设置为9600;
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;			//字长为8位数据格式
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;				//一个停止位
//	USART_InitStructure.USART_Parity = USART_Parity_No;					//无奇偶校验位
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//收发模式
//	USART_Init(UART4, &USART_InitStructure);							//初始化串口
//
//	//Usart4 NVIC 配置
//	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //抢占优先级3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;      //子优先级1
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化VIC寄存器
//
//	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE); //开启接收中断
//
//	USART_Cmd(UART4, ENABLE);                    //使能串口
//}
//
///******************************************************************************
// 函数名称：uart4_set_bps
// 功能描述：设置串口4波特率，和奇偶校验方式
// 参数描述：
// 参数名称： 输入/输出？ 类型		描述
// bps		输入		INT32U		要设置的波特率
// databit	输入		INT8U		数据位可选8 9
// stopbit	输入		INT8U		停止位可选1 2
// odd_even	输入		INT8U		校验方式：NONE为无，ODD为奇校验，EVEN为偶校验
//
// 返  回  值：无
//
// 作 	 者 ：刘及华
// 日 	 期：2005-04-11
// 修改历史：
//		日期		修改人		修改描述
//		------		---------	-------------
//******************************************************************************/
//void uart4_set_bps(INT32U bps, INT8U databit, INT8U stopbit, INT8U odd_even)
//{
//	USART_InitTypeDef USART_InitStructure;
//
//	ToggleWD();
//
//
//	USART_InitStructure.USART_BaudRate = bps;
//
//	if ( databit == 8 )
//	{
//		USART_InitStructure.USART_WordLength = USART_WordLength_8b; //字长为8位数据格式
//	}
//	else
//	{
//		USART_InitStructure.USART_WordLength = USART_WordLength_9b; //字长为9位数据格式
//	}
//
//	if ( stopbit == 1 )
//	{
//		USART_InitStructure.USART_StopBits = USART_StopBits_1; //一个停止位
//	}
//	else
//	{
//		USART_InitStructure.USART_StopBits = USART_StopBits_2; //两个停止位
//	}
//
//	if ( odd_even == NONE )
//	{
//		USART_InitStructure.USART_Parity = USART_Parity_No; //无奇偶校验位
//	}
//	else if ( odd_even == ODD )
//	{
//		USART_InitStructure.USART_Parity = USART_Parity_Odd; //奇校验
//	}
//	else
//	{
//		USART_InitStructure.USART_Parity = USART_Parity_Even; //偶校验
//	}
//
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式
//
//	USART_Init(UART4, &USART_InitStructure); //初始化串口
//}
//
///******************************************************************************
// 函数名称：uart4_buf_flush
// 功能描述：清空串口4缓存
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
//void uart4_buf_flush(void)
//{
//	//  INT8U i = 0;
//	//  volatile INT8U j = 0;
//
//	ToggleWD();
//
//	//  U0IER = 0x00;								/* 关闭接收和发送中断 */
//	//  U0FCR = 0x03;								/* 初始化FIFO 1字节中断 清RX FIFO*/
//	//  for ( i=0; i<17; i++ )
//	//  {
//	//  	j = U0RBR;			//读空FIFO
//	//  }
//	//  U0IER = 0x05;								/* 允许接收和发送中断 */
//
//	QueueFlush(UART4RcvBuf);
//}
//
///******************************************************************************
// 函数名称：uart4_putch
// 功能描述：串口4发送一个字节数据
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
//void uart4_putch(INT8U dat)
//{
////	ToggleWD();
////
////	QueueWrite((void *)UART4SendBuf, dat);         /* 数据入队 */
////
////	USART_ITConfig(UART4, USART_IT_TXE, ENABLE);  /* 允许发送中断 */
//
//	USART_SendData(UART4, dat);
//	while( USART_GetFlagStatus(UART4, USART_FLAG_TC) != SET )			//等待发送完成
//		;
//}
//
///******************************************************************************
// 函数名称：uart4_write
// 功能描述：串口4发送一串数据
// 参数描述：
// 参数名称： 输入/输出？ 类型		描述
// snd_buf	输入		INT8U *		要发送的数据
// snd_length	输入		INT16U		要发送数据的长度
//
// 返  回  值：无
//
// 作 	 者 ：刘及华
// 日 	 期：2012-04-06
// 修改历史：
//		日期		修改人		修改描述
//		------		---------	-------------
//******************************************************************************/
//void uart4_write(INT8U const * const snd_buf, INT16U const snd_length)
//{
//	INT16U i = 0;
//
//	ToggleWD();
//
//	USART_ITConfig(UART4, USART_IT_RXNE, DISABLE);  /* 禁止接收中断 */
//	for ( i = 0; i < snd_length; i++ )
//	{
//		uart4_putch(snd_buf[i]);
//	}
//	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);  /* 允许接收中断 */
//}
//
///******************************************************************************
// 函数名称：UART4_IRQHandler
// 功能描述：UART4中断服务程序
// 参数描述：
// 参数名称： 输入/输出？ 类型		描述
//
// 返  回  值：无
//
// 作 	 者 ：刘及华
// 日 	 期：2012-04-06
// 修改历史：
//		日期		修改人		修改描述
//		------		---------	-------------
//******************************************************************************/
//void UART4_IRQHandler(void)
//{
//	INT8U rcv = 0;
//	INT8U snd = 0;
//
//	//处理接收到的数据
//	if ( USART_GetITStatus(UART4, USART_IT_RXNE) != RESET )
//	{
//		rcv = USART_ReceiveData(UART4);        //读取接收到的数据
//		QueueWrite((void *)UART4RcvBuf, rcv);         /* 数据入队 */
//	}
//
//	//发送中断
//	if ( USART_GetITStatus(UART4, USART_IT_TXE) != RESET )
//	{
//		if ( QueueRead(&snd, UART4SendBuf) == QUEUE_OK )
//		{
//			USART_SendData(UART4, snd);
//		}
//		else
//		{
//			/* 队列空，则禁止发送中断 */
//			USART_ClearITPendingBit(UART4, USART_IT_TXE);
//			USART_ITConfig(UART4, USART_IT_TXE, DISABLE);
//		}
//	}
//
//}
//
///******************************************************************************
// 函数名称：com4_send_hex
// 功能描述：串口4发送一串数据
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
//void com4_send_hex(INT8U *str_to_send, INT16U len)
//{
//	INT16U i = 0;
//
//	ToggleWD();
//
//	for ( i = 0; i < len; i++ )
//	{
//		uart4_putch(str_to_send[i]);
//	}
//}
//
///*****************************************************************
//函数原型：com4_gets
//功能描述：串口4接收
//参数描述：
//参数名称：		输入/输出？	类型		描述
//-----------		-----------	------	   	-------
//str_got			输出		INT8U*		得到的字符串
//timeout_start	输入		INT32U		首字超时，单位mS，0死等
//timeout			输入		INT16U		接收字节间隔超时，单位mS
//
// 返  回  值：ok(0)-成功
//				 KEY_CANCEL-按退出键了
//				 notok(0xFF)-失败，超时
//作      者	：刘及华
//日      期：2005-08-20
//修改历史：
//日期		修改人		修改描述
//------		---------	-------------
//*****************************************************************/
//INT8U com4_gets(INT8U *str_got, INT16U *rcv_len, INT32U timeout_start, INT16U timeout)
//{
//	INT8U i = 0;
//	INT16U len = 0;
//	INT32U timeout_cntr = 0;
//
//	ToggleWD();
//
//	//第一个字节
//	for(;;)
//	{
//		if( QueueRead(&i, UART4RcvBuf) == QUEUE_OK )
//		{
//			str_got[len++] = i;
//			break;
//		}
//		else
//		{
//			if (timeout_start == 0)
//			{
//				OSTimeDlyHMSM(0, 0, 0, 5);
//			}
//			else
//			{
//				timeout_cntr++;
//				if (timeout_cntr > timeout_start / 5)          //超时
//				{
//					str_got[len] = '\0';
//					*rcv_len = len;
//					return notok;
//				}
//				OSTimeDlyHMSM(0, 0, 0, 5);
//			}
//		}
//	}
//
//	//其余字节
//	timeout_cntr = 0;
//	for(;;)
//	{
//		if( QueueRead(&i, UART4RcvBuf) == QUEUE_OK )
//		{
//			str_got[len++] = i;
//			timeout_cntr = 0;
//		}
//		else
//		{
//			timeout_cntr++;
//			if (timeout_cntr > timeout / 5)            //超时
//			{
//				str_got[len] = '\0';
//				*rcv_len = len;
//				return ok;
//			}
//			OSTimeDlyHMSM(0, 0, 0, 5);
//		}
//	}
//
//}




/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/

