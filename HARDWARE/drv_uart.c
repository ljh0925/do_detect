/******************************************************************************
Copyright 2005 �������ķ�������޹�˾
All rights riserved.

�ļ���	��drv_uart.c
ģ�����ƣ�������������
���ܸ�Ҫ��������������

ȡ���汾��0.0.1
�޸���	��������
������ڣ�2005.04.06
����˵����create

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



INT8U Com1SndBuf[UART1_SEND_QUEUE_LENGTH];              //���ڷ��ͻ�����
INT8U Com2SndBuf[UART2_SEND_QUEUE_LENGTH];              //���ڷ��ͻ�����
INT8U Com3SndBuf[UART3_SEND_QUEUE_LENGTH];              //���ڷ��ͻ�����
//INT8U Com4SndBuf[UART4_SEND_QUEUE_LENGTH];              //���ڷ��ͻ�����

INT8U Com1RcvBuf[UART1_RCV_QUEUE_LENGTH];               //���ڽ��ջ�����
INT8U Com2RcvBuf[UART2_RCV_QUEUE_LENGTH];               //���ڽ��ջ�����
INT8U Com3RcvBuf[UART3_RCV_QUEUE_LENGTH];               //���ڽ��ջ�����
//INT8U Com4RcvBuf[UART4_RCV_QUEUE_LENGTH];               //���ڽ��ջ�����

//INT8U TmpTBuf[UART1_SEND_QUEUE_LENGTH];         //���ڷ��ͻ�����

extern volatile DEV_STAT DevStat;


/*********************************************************************************************************
** ��������: QueueCreate
** ��������: ��ʼ�����ݶ���
** �䡡��: Buf      ��Ϊ���з���Ĵ洢�ռ��ַ
**         SizeOfBuf��Ϊ���з���Ĵ洢�ռ��С���ֽڣ�
**         ReadEmpty��Ϊ���ж���ʱ�������
**         WriteFull��Ϊ����д��ʱ�������
** �䡡��: NOT_OK:��������
**         QUEUE_OK:�ɹ�
** ȫ�ֱ���: ��
** ����ģ��: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
**
** ������: ������
** �ա���: 2003��7��2��
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
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

	if ( Buf != NULL && SizeOfBuf >= (sizeof(DataQueue)) )        /* �жϲ����Ƿ���Ч */
	{
		Queue = (DataQueue *)Buf;

		OS_ENTER_CRITICAL();
		/* ��ʼ���ṹ������ */
		Queue->MaxData = (SizeOfBuf - (uint32)(((DataQueue *)0)->Buf)) /
		   sizeof(QUEUE_DATA_TYPE);               /* ������п��Դ洢��������Ŀ */
		Queue->End = Queue->Buf + Queue->MaxData;               /* �������ݻ���Ľ�����ַ */
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
** ��������: QueueRead
** ��������: ��ȡ�����е�����
** �䡡��: Ret:�洢���ص���Ϣ�ĵ�ַ
**         Buf:ָ����е�ָ��
** �䡡��: NOT_OK     ����������
**         QUEUE_OK   ���յ���Ϣ
**         QUEUE_EMPTY������Ϣ
** ȫ�ֱ���: ��
** ����ģ��: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
**
** ������: ������
** �ա���: 2003��7��2��
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
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
	if ( Buf != NULL )                                            /* �����Ƿ���Ч */
	{
		//  	ToggleWD();
		/* ��Ч */
		Queue = (DataQueue *)Buf;

		OS_ENTER_CRITICAL();

		if ( Queue->NData > 0 )                                   /* �����Ƿ�Ϊ�� */
		{
			/* ����         */
			*Ret = Queue->Out[0];                               /* ���ݳ���     */
			Queue->Out++;                                       /* ��������ָ�� */
			if ( Queue->Out >= Queue->End )
			{
				Queue->Out = Queue->Buf;
			}
			Queue->NData--;                                     /* ���ݼ���      */
			err = QUEUE_OK;
		}
		else
		{                                                       /* ��              */
			err = QUEUE_EMPTY;
			if ( Queue->ReadEmpty != NULL )                       /* �����û������� */
			{
				err = Queue->ReadEmpty(Ret, Queue);
			}
		}
		OS_EXIT_CRITICAL();
	}
	return err;
}

/*********************************************************************************************************
** ��������: QueueWrite
** ��������: FIFO��ʽ��������
** �䡡��: Buf :ָ����е�ָ��
**         Data:��Ϣ����
** �䡡��: NOT_OK   :��������
**         QUEUE_FULL:������
**         QUEUE_OK  :���ͳɹ�
** ȫ�ֱ���: ��
** ����ģ��: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
**
** ������: ������
** �ա���: 2003��7��2��
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
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
	if ( Buf != NULL )                                                    /* �����Ƿ���Ч */
	{
		//  	ToggleWD();

		Queue = (DataQueue *)Buf;

		OS_ENTER_CRITICAL();

		if ( Queue->NData < Queue->MaxData )                              /* �����Ƿ���  */
		{
			/* ����        */
			Queue->In[0] = Data;                                        /* �������    */
			Queue->In++;                                                /* �������ָ��*/
			if ( Queue->In >= Queue->End )
			{
				Queue->In = Queue->Buf;
			}
			Queue->NData++;                                             /* ��������    */
			err = QUEUE_OK;
		}
		else
		{                                                               /* ��           */
			err = QUEUE_FULL;
			if ( Queue->WriteFull != NULL )                               /* �����û������� */
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
** ��������: QueueWriteFront
** ��������: LIFO��ʽ��������
** �䡡��: Buf:ָ����е�ָ��
**         Data:��Ϣ����
** �䡡��: QUEUE_FULL:������
**         QUEUE_OK:���ͳɹ�
** ȫ�ֱ���: ��
** ����ģ��: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
**
** ������: ������
** �ա���: 2003��7��2��
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
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
	if ( Buf != NULL )                                                    /* �����Ƿ���Ч */
	{
		Queue = (DataQueue *)Buf;

		OS_ENTER_CRITICAL();

		if ( Queue->NData < Queue->MaxData )                              /* �����Ƿ���  */
		{
			/* ���� */
			Queue->Out--;                                               /* ��������ָ�� */
			if ( Queue->Out < Queue->Buf )
			{
				Queue->Out = Queue->End - 1;
			}
			Queue->Out[0] = Data;                                       /* �������     */
			Queue->NData++;                                             /* ������Ŀ���� */
			err = QUEUE_OK;
		}
		else
		{                                                               /* ��           */
			err = QUEUE_FULL;
			if ( Queue->WriteFull != NULL )                               /* �����û������� */
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
** ��������: QueueNData
** ��������: ȡ�ö�����������
** �䡡��: Buf:ָ����е�ָ��
** �䡡��: ��Ϣ��
** ȫ�ֱ���: ��
** ����ģ��: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
**
** ������: ������
** �ա���: 2003��7��2��
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
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

	temp = 0;                                                   /* ������Ч����0 */
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
** ��������: QueueSize
** ��������: ȡ�ö���������
** �䡡��: Buf:ָ����е�ָ��
** �䡡��: ����������
** ȫ�ֱ���: ��
** ����ģ��: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
**
** ������: ������
** �ա���: 2003��7��2��
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
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

	temp = 0;                                                   /* ������Ч����0 */
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
** ��������: OSQFlush
** ��������: ��ն���
** �䡡��: Buf:ָ����е�ָ��
** �䡡��: ��
** ȫ�ֱ���: ��
** ����ģ��: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
**
** ������: ������
** �ա���: 2003��7��2��
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
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

	if ( Buf != NULL )                                                /* �����Ƿ���Ч */
	{
		//  	ToggleWD();
		/* ��Ч         */
		Queue = (DataQueue *)Buf;
		OS_ENTER_CRITICAL();
		Queue->Out = Queue->Buf;
		Queue->In = Queue->Buf;
		Queue->NData = 0;                                           /* ������ĿΪ0 */
		OS_EXIT_CRITICAL();
	}
}
#endif

/*********************************************************************************************************
** ��������: Uart1WriteFull
** ��������: ���ݶ���д��������� 
** �䡡��: Buf:ָ�����ݶ���
**        Data:��Ҫ��ӵ�����
**        Mod: ��ӷ�ʽ
** �䡡��:TRUE  :�ɹ�
**        FALSE:ʧ��
** ȫ�ֱ���: ��
** ����ģ��: ��
**-------------------------------------------------------------------------------------------------------
** �޸���: ������
** �ա���: 2004��12��28��
**-------------------------------------------------------------------------------------------------------
** �޸���: ������
** �ա���: 2004��12��28��
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
uint8 Uart1WriteFull(DataQueue *Buf, QUEUE_DATA_TYPE Data, uint8 Mod)
{
	//  uint16 temp;

	Mod = Mod;

	//  ToggleWD();

	//    temp = QueueSize((void *)Buf);
	//    while ( temp <= QueueNData((void *)Buf) )         /* �ȴ����ݶ��в��� */
	//    {
	////  	OSTimeDly(2);
	//    }
	//    return QueueWrite((void *)Buf, Data);           /* ����������� */

	return ok;
}

/******************************************************************************
 �������ƣ�uart1_init
 ������������ʼ��UART1
 ����������
 �������ƣ� ����/����� ����		����
				
 ��  ��  ֵ����
				   
 �� 	 �� ��������
 �� 	 �ڣ�2005-04-11
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void uart1_init(void)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	(void)QueueCreate((void *)UART1SendBuf, sizeof(UART1SendBuf), NULL, (uint8 (*)())Uart1WriteFull);
	(void)QueueCreate((void *)UART1RcvBuf, sizeof(UART1RcvBuf), NULL, (uint8 (*)())Uart1WriteFull);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);   //ʹ��USART1��GPIOAʱ��
	USART_DeInit(USART1);  //��λ����1

	//USART1_TX   PA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9

	//USART1_RX	  PA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA10


	//USART ��ʼ������
//	USART_InitStructure.USART_BaudRate = 9600; //һ������Ϊ9600;
	USART_InitStructure.USART_BaudRate = 115200; //һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1; //һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No; //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); //��ʼ������

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;      //�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure); //����ָ���Ĳ�����ʼ��VIC�Ĵ���

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //���������ж�

	USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���


}

/******************************************************************************
 �������ƣ�uart1_set_bps
 �������������ô���1�����ʣ�����żУ�鷽ʽ
 ����������
 �������ƣ� ����/����� ����		����
 bps		����		INT32U		Ҫ���õĲ�����  
 databit	����		INT8U		����λ��ѡ8 9
 stopbit	����		INT8U		ֹͣλ��ѡ1 2 
 odd_even	����		INT8U		У�鷽ʽ��NONEΪ�ޣ�ODDΪ��У�飬EVENΪżУ��
				
 ��  ��  ֵ����
				   
 �� 	 �� ��������
 �� 	 �ڣ�2005-04-11
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void uart1_set_bps(INT32U bps, INT8U databit, INT8U stopbit, INT8U odd_even)
{
	USART_InitTypeDef USART_InitStructure;

	ToggleWD();


	USART_InitStructure.USART_BaudRate = bps;

	if ( databit == 8 )
	{
		USART_InitStructure.USART_WordLength = USART_WordLength_8b; //�ֳ�Ϊ8λ���ݸ�ʽ
	}
	else
	{
		USART_InitStructure.USART_WordLength = USART_WordLength_9b; //�ֳ�Ϊ9λ���ݸ�ʽ
	}

	if ( stopbit == 1 )
	{
		USART_InitStructure.USART_StopBits = USART_StopBits_1; //һ��ֹͣλ
	}
	else
	{
		USART_InitStructure.USART_StopBits = USART_StopBits_2; //����ֹͣλ
	}

	if ( odd_even == NONE )
	{
		USART_InitStructure.USART_Parity = USART_Parity_No; //����żУ��λ
	}
	else if ( odd_even == ODD )
	{
		USART_InitStructure.USART_Parity = USART_Parity_Odd; //��У��
	}
	else
	{
		USART_InitStructure.USART_Parity = USART_Parity_Even; //żУ��
	}

	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ

	USART_Init(USART1, &USART_InitStructure); //��ʼ������
}

/******************************************************************************
 �������ƣ�uart1_buf_flush
 ������������մ���1����
 ����������
 �������ƣ� ����/����� ����		����

 ��  ��  ֵ����

 �� 	 �� ��������
 �� 	 �ڣ�2005-04-11
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void uart1_buf_flush(void)
{
	//  INT8U i = 0;
	//  volatile INT8U j = 0;

	ToggleWD();

	//  U0IER = 0x00;								/* �رս��պͷ����ж� */
	//  U0FCR = 0x03;								/* ��ʼ��FIFO 1�ֽ��ж� ��RX FIFO*/
	//  for ( i=0; i<17; i++ )
	//  {
	//  	j = U0RBR;			//����FIFO
	//  }
	//  U0IER = 0x05;								/* ������պͷ����ж� */

	QueueFlush(UART1RcvBuf);
}

/******************************************************************************
 �������ƣ�uart1_putch
 ��������������1����һ���ֽ�����
 ����������
 �������ƣ� ����/����� ����		����
 dat		����		INT8U		Ҫ���͵�����
				
 ��  ��  ֵ����
				   
 �� 	 �� ��������
 �� 	 �ڣ�2012-04-06
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void uart1_putch(INT8U dat)
{
//	ToggleWD();
//
//	QueueWrite((void *)UART1SendBuf, dat);         /* ������� */
//
//	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);  /* �������ж� */

	toggle_wd();
	USART_SendData(USART1, dat);
	while( USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET )			//�ȴ��������
		;
}

/******************************************************************************
 �������ƣ�uart1_write
 ��������������1����һ������
 ����������
 �������ƣ� ����/����� ����		����
 snd_buf	����		INT8U *		Ҫ���͵�����
 snd_length	����		INT16U		Ҫ�������ݵĳ���
				
 ��  ��  ֵ����
				   
 �� 	 �� ��������
 �� 	 �ڣ�2012-04-06
 �޸���ʷ��
		����		�޸���		�޸�����
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

	USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);  /* ��ֹ�����ж� */
	for ( i = 0; i < snd_length; i++ )
	{
		uart1_putch(snd_buf[i]);
	}
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  /* ��������ж� */
}

/******************************************************************************
 �������ƣ�USART1_IRQHandler
 ����������UART1�жϷ������
 ����������
 �������ƣ� ����/����� ����		����
				
 ��  ��  ֵ����
				   
 �� 	 �� ��������
 �� 	 �ڣ�2012-04-06
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
//void uart0_handler (void) __irq
void USART1_IRQHandler(void)
{
	//    INT8U IIR, temp, i;
	//
	////  ToggleWD();
	//
	//    VICIntEnClr = (1 << UART0_INT);     //��ֹUART0�ж�
	////  OS_ENTER_CRITICAL();
	//
	//    while ( ((IIR = U0IIR) & 0x01) == 0 )
	//    {                                                   /* ���ж�δ������ */
	////  	ToggleWD();
	//
	//        switch ( IIR & 0x0e )
	//        {
	//            case 0x02:                                  /* THRE�ж�    */
	//                for ( i = 0; i < UART0_FIFO_LENGTH; i++ ) /* ����FIFO������� */
	//                {
	//                    if ( QueueRead(&temp, UART0SendBuf) == QUEUE_OK )
	//                    {
	//                        U0THR = temp;
	//                    }
	//                    else
	//                    {
	//                        U0IER = U0IER & (~0x02);        /* ���пգ����ֹ�����ж� */
	//                    }
	//                }
	//                break;
	//
	//            case 0x04:                                  /* �������ݿ��� */
	//                i = U0RBR;
	//                QueueWrite((void *)UART0RcvBuf, i);         /* ������� */
	//
	//                T0TC = 0;
	//                T0TCR = 0x01;                   //start timer0
	//                break;
	//
	//            case 0x06:                                  /* ������״̬   */
	//                temp = U0LSR;
	//                break;
	//
	//            case 0x0c:                                  /* �ַ���ʱָʾ */
	//                i = U0RBR;
	//                QueueWrite((void *)UART0RcvBuf, i);         /* ������� */
	//
	//                T0TC = 0;
	//                T0TCR = 0x01;                   //start timer0
	//                break;
	//
	//            default :
	//                break;
	//        }
	//    }
	//    VICIntEnable = (1 << UART0_INT);        //ʹ��UART0�ж�
	//    VICVectAddr = 0;            // ֪ͨ�жϿ������жϽ���
	//
	////  OS_EXIT_CRITICAL();

	INT8U rcv = 0;
	INT8U snd = 0;

	//������յ�������
	if ( USART_GetITStatus(USART1, USART_IT_RXNE) != RESET )
	{
		rcv = USART_ReceiveData(USART1);        //(USART1->DR);	//��ȡ���յ�������
		QueueWrite((void *)UART1RcvBuf, rcv);         /* ������� */

		//  	/* Clear the USART1 Receive interrupt */
		//      USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}


	//�����ж�
	if ( USART_GetITStatus(USART1, USART_IT_TXE) != RESET )
	{
		if ( QueueRead(&snd, UART1SendBuf) == QUEUE_OK )
		{
			USART_SendData(USART1, snd);
		}
		else
		{
			/* ���пգ����ֹ�����ж� */
			USART_ClearITPendingBit(USART1, USART_IT_TXE);
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
		}

	}

}

/******************************************************************************
 �������ƣ�com1_send_hex
 ����������
 ����������
 �������ƣ� ����/����� ����		����
 str_to_send	����	INT8U *		Ҫ���͵��ַ���
 len			����	INT16U		����
				
 ��  ��  ֵ����
				   
 �� 	 �� ��������
 �� 	 �ڣ�2005-04-07
 �޸���ʷ��
		����		�޸���		�޸�����
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
����ԭ�ͣ�com1_gets
��������������1����
����������
�������ƣ�		����/�����	����		����
-----------		-----------	------	   	-------
str_got			���		INT8U*		�õ����ַ���
timeout_start	����		INT32U		���ֳ�ʱ����λmS��0����
timeout			����		INT16U		�����ֽڼ����ʱ����λmS

 ��  ��  ֵ��ok(0)-�ɹ�
				 KEY_CANCEL-���˳�����
				 notok(0xFF)-ʧ�ܣ���ʱ
��      ��	��������
��      �ڣ�2005-08-20
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
INT8U com1_gets(INT8U *str_got, INT16U *rcv_len, INT32U timeout_start, INT16U timeout)
{
	INT8U i = 0;
	INT16U len = 0;
	INT32U timeout_cntr = 0;

	ToggleWD();

	//��һ���ֽ�
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
				if (timeout_cntr > timeout_start / 5)          //��ʱ
				{
					str_got[len] = '\0';
					*rcv_len = len;
					return notok;
				}
				OSTimeDlyHMSM(0, 0, 0, 5);
			}
		}
	}

	//�����ֽ�
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
			if (timeout_cntr > timeout / 5)            //��ʱ
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
** ��������: Uart2WriteFull
** ��������: ���ݶ���д��������� 
** �䡡��: Buf:ָ�����ݶ���
**        Data:��Ҫ��ӵ�����
**        Mod: ��ӷ�ʽ
** �䡡��:TRUE  :�ɹ�
**        FALSE:ʧ��
** ȫ�ֱ���: ��
** ����ģ��: ��
**-------------------------------------------------------------------------------------------------------
** �޸���: ������
** �ա���: 2004��12��28��
**-------------------------------------------------------------------------------------------------------
** �޸���: ������
** �ա���: 2004��12��28��
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
uint8 Uart2WriteFull(DataQueue *Buf, QUEUE_DATA_TYPE Data, uint8 Mod)
{
	//  uint16 temp;

	Mod = Mod;

	//  ToggleWD();

	//    temp = QueueSize((void *)Buf);
	//    while ( temp <= QueueNData((void *)Buf) )         /* �ȴ����ݶ��в��� */
	//    {
	////  	OSTimeDly(2);
	//    }
	//    return QueueWrite((void *)Buf, Data);           /* ����������� */

	return ok;
}

/******************************************************************************
 �������ƣ�uart2_init
 ������������ʼ��UART2
 ����������
 �������ƣ� ����/����� ����		����
				
 ��  ��  ֵ����
				   
 �� 	 �� ��������
 �� 	 �ڣ�2005-04-11
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void uart2_init(void)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	(void)QueueCreate((void *)UART2SendBuf, sizeof(UART2SendBuf), NULL, (uint8 (*)())Uart2WriteFull);
	(void)QueueCreate((void *)UART2RcvBuf, sizeof(UART2RcvBuf), NULL, (uint8 (*)())Uart2WriteFull);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//ʹ��USART2ʱ��

	USART_DeInit(USART2);  //��λ����2

	//USART2_TX   PA.2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 				//PA.2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure); 					//��ʼ��

	//USART2_RX	  PA.3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//��ʼ��


	//USART ��ʼ������
//	USART_InitStructure.USART_BaudRate = 9600; 							//һ������Ϊ9600;
	USART_InitStructure.USART_BaudRate = 38400; 							//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;			//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;				//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;					//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//�շ�ģʽ
	USART_Init(USART2, &USART_InitStructure);							//��ʼ������

	//Usart2 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;      //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure); //����ָ���Ĳ�����ʼ��VIC�Ĵ���

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //���������ж�

	USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ���


}

/******************************************************************************
 �������ƣ�uart2_set_bps
 �������������ô���2�����ʣ�����żУ�鷽ʽ
 ����������
 �������ƣ� ����/����� ����		����
 bps		����		INT32U		Ҫ���õĲ�����  
 databit	����		INT8U		����λ��ѡ8 9
 stopbit	����		INT8U		ֹͣλ��ѡ1 2 
 odd_even	����		INT8U		У�鷽ʽ��NONEΪ�ޣ�ODDΪ��У�飬EVENΪżУ��
				
 ��  ��  ֵ����
				   
 �� 	 �� ��������
 �� 	 �ڣ�2005-04-11
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void uart2_set_bps(INT32U bps, INT8U databit, INT8U stopbit, INT8U odd_even)
{
	USART_InitTypeDef USART_InitStructure;

	ToggleWD();


	USART_InitStructure.USART_BaudRate = bps;

	if ( databit == 8 )
	{
		USART_InitStructure.USART_WordLength = USART_WordLength_8b; //�ֳ�Ϊ8λ���ݸ�ʽ
	}
	else
	{
		USART_InitStructure.USART_WordLength = USART_WordLength_9b; //�ֳ�Ϊ9λ���ݸ�ʽ
	}

	if ( stopbit == 1 )
	{
		USART_InitStructure.USART_StopBits = USART_StopBits_1; //һ��ֹͣλ
	}
	else
	{
		USART_InitStructure.USART_StopBits = USART_StopBits_2; //����ֹͣλ
	}

	if ( odd_even == NONE )
	{
		USART_InitStructure.USART_Parity = USART_Parity_No; //����żУ��λ
	}
	else if ( odd_even == ODD )
	{
		USART_InitStructure.USART_Parity = USART_Parity_Odd; //��У��
	}
	else
	{
		USART_InitStructure.USART_Parity = USART_Parity_Even; //żУ��
	}

	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ

	USART_Init(USART2, &USART_InitStructure); //��ʼ������
}

/******************************************************************************
 �������ƣ�uart2_buf_flush
 ������������մ���2����
 ����������
 �������ƣ� ����/����� ����		����

 ��  ��  ֵ����

 �� 	 �� ��������
 �� 	 �ڣ�2005-04-11
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void uart2_buf_flush(void)
{
	//  INT8U i = 0;
	//  volatile INT8U j = 0;

	ToggleWD();

	//  U0IER = 0x00;								/* �رս��պͷ����ж� */
	//  U0FCR = 0x03;								/* ��ʼ��FIFO 1�ֽ��ж� ��RX FIFO*/
	//  for ( i=0; i<17; i++ )
	//  {
	//  	j = U0RBR;			//����FIFO
	//  }
	//  U0IER = 0x05;								/* ������պͷ����ж� */

	QueueFlush(UART2RcvBuf);
}

/******************************************************************************
 �������ƣ�uart2_putch
 ��������������2����һ���ֽ�����
 ����������
 �������ƣ� ����/����� ����		����
 dat		����		INT8U		Ҫ���͵�����
				
 ��  ��  ֵ����
				   
 �� 	 �� ��������
 �� 	 �ڣ�2012-04-06
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void uart2_putch(INT8U dat)
{
	ToggleWD();

	QueueWrite((void *)UART2SendBuf, dat);         /* ������� */

	USART_ITConfig(USART2, USART_IT_TXE, ENABLE);  /* �������ж� */
}

/******************************************************************************
 �������ƣ�uart2_write
 ��������������2����һ������
 ����������
 �������ƣ� ����/����� ����		����
 snd_buf	����		INT8U *		Ҫ���͵�����
 snd_length	����		INT16U		Ҫ�������ݵĳ���
				
 ��  ��  ֵ����
				   
 �� 	 �� ��������
 �� 	 �ڣ�2012-04-06
 �޸���ʷ��
		����		�޸���		�޸�����
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
 �������ƣ�USART2_IRQHandler
 ����������UART2�жϷ������
 ����������
 �������ƣ� ����/����� ����		����
				
 ��  ��  ֵ����
				   
 �� 	 �� ��������
 �� 	 �ڣ�2012-04-06
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void USART2_IRQHandler(void)
{
	INT8U rcv = 0;
	INT8U snd = 0;

	//������յ�������
	if ( USART_GetITStatus(USART2, USART_IT_RXNE) != RESET )
	{
		rcv = USART_ReceiveData(USART2);        //��ȡ���յ�������
		QueueWrite((void *)UART2RcvBuf, rcv);         /* ������� */
	}

	//�����ж�
	if ( USART_GetITStatus(USART2, USART_IT_TXE) != RESET )
	{
		if ( QueueRead(&snd, UART2SendBuf) == QUEUE_OK )
		{
			USART_SendData(USART2, snd);
		}
		else
		{
			/* ���пգ����ֹ�����ж� */
			USART_ClearITPendingBit(USART2, USART_IT_TXE);
			USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
		}
	}

}

/******************************************************************************
 �������ƣ�com2_send_hex
 ��������������2����һ������
 ����������
 �������ƣ� ����/����� ����		����
 str_to_send	����	INT8U *		Ҫ���͵��ַ���
 len			����	INT16U		����
				
 ��  ��  ֵ����
				   
 �� 	 �� ��������
 �� 	 �ڣ�2005-04-07
 �޸���ʷ��
		����		�޸���		�޸�����
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
����ԭ�ͣ�com2_gets
��������������2����
����������
�������ƣ�		����/�����	����		����
-----------		-----------	------	   	-------
str_got			���		INT8U*		�õ����ַ���
timeout_start	����		INT32U		���ֳ�ʱ����λmS��0����
timeout			����		INT16U		�����ֽڼ����ʱ����λmS

 ��  ��  ֵ��ok(0)-�ɹ�
				 KEY_CANCEL-���˳�����
				 notok(0xFF)-ʧ�ܣ���ʱ
��      ��	��������
��      �ڣ�2005-08-20
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
INT8U com2_gets(INT8U *str_got, INT16U *rcv_len, INT32U timeout_start, INT16U timeout)
{
	INT8U i = 0;
	INT16U len = 0;
	INT32U timeout_cntr = 0;

	ToggleWD();

	//��һ���ֽ�
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
				if (timeout_cntr > timeout_start / 5)          //��ʱ
				{
					str_got[len] = '\0';
					*rcv_len = len;
					return notok;
				}
				OSTimeDlyHMSM(0, 0, 0, 5);
			}
		}
	}

	//�����ֽ�
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
			if (timeout_cntr > timeout / 5)            //��ʱ
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
** ��������: Uart3WriteFull
** ��������: ���ݶ���д��������� 
** �䡡��: Buf:ָ�����ݶ���
**        Data:��Ҫ��ӵ�����
**        Mod: ��ӷ�ʽ
** �䡡��:TRUE  :�ɹ�
**        FALSE:ʧ��
** ȫ�ֱ���: ��
** ����ģ��: ��
**-------------------------------------------------------------------------------------------------------
** �޸���: ������
** �ա���: 2004��12��28��
**-------------------------------------------------------------------------------------------------------
** �޸���: ������
** �ա���: 2004��12��28��
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
uint8 Uart3WriteFull(DataQueue *Buf, QUEUE_DATA_TYPE Data, uint8 Mod)
{
	//  uint16 temp;

	Mod = Mod;

	//  ToggleWD();

	//    temp = QueueSize((void *)Buf);
	//    while ( temp <= QueueNData((void *)Buf) )         /* �ȴ����ݶ��в��� */
	//    {
	////  	OSTimeDly(2);
	//    }
	//    return QueueWrite((void *)Buf, Data);           /* ����������� */

	return ok;
}

/******************************************************************************
 �������ƣ�uart3_init
 ������������ʼ��UART3
 ����������
 �������ƣ� ����/����� ����		����
				
 ��  ��  ֵ����
				   
 �� 	 �� ��������
 �� 	 �ڣ�2005-04-11
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void uart3_init(void)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	(void)QueueCreate((void *)UART3SendBuf, sizeof(UART3SendBuf), NULL, (uint8 (*)())Uart3WriteFull);
	(void)QueueCreate((void *)UART3RcvBuf, sizeof(UART3RcvBuf), NULL, (uint8 (*)())Uart3WriteFull);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);   //ʹ��GPIOBʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//ʹ��USART3ʱ��

	USART_DeInit(USART3);  //��λ����3

	//USART3_TX   PB.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 				//PB.10
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		//�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure); 					//��ʼ��

	//USART3_RX	  PB.11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//��ʼ��


	//USART ��ʼ������
	USART_InitStructure.USART_BaudRate = 9600; 							//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;			//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;				//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;					//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//�շ�ģʽ
	USART_Init(USART3, &USART_InitStructure);							//��ʼ������

	//Usart2 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;      //�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure); //����ָ���Ĳ�����ʼ��VIC�Ĵ���

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //���������ж�

	USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���
}

/******************************************************************************
 �������ƣ�uart3_set_bps
 �������������ô���3�����ʣ�����żУ�鷽ʽ
 ����������
 �������ƣ� ����/����� ����		����
 bps		����		INT32U		Ҫ���õĲ�����  
 databit	����		INT8U		����λ��ѡ8 9
 stopbit	����		INT8U		ֹͣλ��ѡ1 2 
 odd_even	����		INT8U		У�鷽ʽ��NONEΪ�ޣ�ODDΪ��У�飬EVENΪżУ��
				
 ��  ��  ֵ����
				   
 �� 	 �� ��������
 �� 	 �ڣ�2005-04-11
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void uart3_set_bps(INT32U bps, INT8U databit, INT8U stopbit, INT8U odd_even)
{
	USART_InitTypeDef USART_InitStructure;

	ToggleWD();


	USART_InitStructure.USART_BaudRate = bps;

	if ( databit == 8 )
	{
		USART_InitStructure.USART_WordLength = USART_WordLength_8b; //�ֳ�Ϊ8λ���ݸ�ʽ
	}
	else
	{
		USART_InitStructure.USART_WordLength = USART_WordLength_9b; //�ֳ�Ϊ9λ���ݸ�ʽ
	}

	if ( stopbit == 1 )
	{
		USART_InitStructure.USART_StopBits = USART_StopBits_1; //һ��ֹͣλ
	}
	else
	{
		USART_InitStructure.USART_StopBits = USART_StopBits_2; //����ֹͣλ
	}

	if ( odd_even == NONE )
	{
		USART_InitStructure.USART_Parity = USART_Parity_No; //����żУ��λ
	}
	else if ( odd_even == ODD )
	{
		USART_InitStructure.USART_Parity = USART_Parity_Odd; //��У��
	}
	else
	{
		USART_InitStructure.USART_Parity = USART_Parity_Even; //żУ��
	}

	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ

	USART_Init(USART3, &USART_InitStructure); //��ʼ������
}

/******************************************************************************
 �������ƣ�uart3_buf_flush
 ������������մ���3����
 ����������
 �������ƣ� ����/����� ����		����

 ��  ��  ֵ����

 �� 	 �� ��������
 �� 	 �ڣ�2005-04-11
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void uart3_buf_flush(void)
{
	//  INT8U i = 0;
	//  volatile INT8U j = 0;

	ToggleWD();

	//  U0IER = 0x00;								/* �رս��պͷ����ж� */
	//  U0FCR = 0x03;								/* ��ʼ��FIFO 1�ֽ��ж� ��RX FIFO*/
	//  for ( i=0; i<17; i++ )
	//  {
	//  	j = U0RBR;			//����FIFO
	//  }
	//  U0IER = 0x05;								/* ������պͷ����ж� */

	QueueFlush(UART3RcvBuf);
}

/******************************************************************************
 �������ƣ�uart3_putch
 ��������������3����һ���ֽ�����
 ����������
 �������ƣ� ����/����� ����		����
 dat		����		INT8U		Ҫ���͵�����
				
 ��  ��  ֵ����
				   
 �� 	 �� ��������
 �� 	 �ڣ�2012-04-06
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void uart3_putch(INT8U dat)
{
//	ToggleWD();
//
//	QueueWrite((void *)UART3SendBuf, dat);         /* ������� */
//
//	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);  /* �������ж� */

	USART_SendData(USART3, dat);
	while( USART_GetFlagStatus(USART3, USART_FLAG_TC) != SET )			//�ȴ��������
		;
}

/******************************************************************************
 �������ƣ�uart3_write
 ��������������3����һ������
 ����������
 �������ƣ� ����/����� ����		����
 snd_buf	����		INT8U *		Ҫ���͵�����
 snd_length	����		INT16U		Ҫ�������ݵĳ���
				
 ��  ��  ֵ����
				   
 �� 	 �� ��������
 �� 	 �ڣ�2012-04-06
 �޸���ʷ��
		����		�޸���		�޸�����
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

	USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);  /* ��ֹ�����ж� */
	for ( i = 0; i < snd_length; i++ )
	{
		uart3_putch(snd_buf[i]);
	}
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);  /* ��������ж� */
}

/******************************************************************************
 �������ƣ�USART3_IRQHandler
 ����������UART3�жϷ������
 ����������
 �������ƣ� ����/����� ����		����
				
 ��  ��  ֵ����
				   
 �� 	 �� ��������
 �� 	 �ڣ�2012-04-06
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void USART3_IRQHandler(void)
{
	INT8U rcv = 0;
	INT8U snd = 0;

	//������յ�������
	if ( USART_GetITStatus(USART3, USART_IT_RXNE) != RESET )
	{
		rcv = USART_ReceiveData(USART3);        //��ȡ���յ�������
		QueueWrite((void *)UART3RcvBuf, rcv);         /* ������� */
	}

	//�����ж�
	if ( USART_GetITStatus(USART3, USART_IT_TXE) != RESET )
	{
		if ( QueueRead(&snd, UART3SendBuf) == QUEUE_OK )
		{
			USART_SendData(USART3, snd);
		}
		else
		{
			/* ���пգ����ֹ�����ж� */
			USART_ClearITPendingBit(USART3, USART_IT_TXE);
			USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
		}
	}

}

/******************************************************************************
 �������ƣ�com3_send_hex
 ��������������3����һ������
 ����������
 �������ƣ� ����/����� ����		����
 str_to_send	����	INT8U *		Ҫ���͵��ַ���
 len			����	INT16U		����
				
 ��  ��  ֵ����
				   
 �� 	 �� ��������
 �� 	 �ڣ�2005-04-07
 �޸���ʷ��
		����		�޸���		�޸�����
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
����ԭ�ͣ�com3_gets
��������������3����
����������
�������ƣ�		����/�����	����		����
-----------		-----------	------	   	-------
str_got			���		INT8U*		�õ����ַ���
timeout_start	����		INT32U		���ֳ�ʱ����λmS��0����
timeout			����		INT16U		�����ֽڼ����ʱ����λmS

 ��  ��  ֵ��ok(0)-�ɹ�
				 KEY_CANCEL-���˳�����
				 notok(0xFF)-ʧ�ܣ���ʱ
��      ��	��������
��      �ڣ�2005-08-20
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
INT8U com3_gets(INT8U *str_got, INT16U *rcv_len, INT32U timeout_start, INT16U timeout)
{
	INT8U i = 0;
	INT16U len = 0;
	INT32U timeout_cntr = 0;

	ToggleWD();

	//��һ���ֽ�
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
				if (timeout_cntr > timeout_start / 5)          //��ʱ
				{
					str_got[len] = '\0';
					*rcv_len = len;
					return notok;
				}
				OSTimeDlyHMSM(0, 0, 0, 5);
			}
		}
	}

	//�����ֽ�
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
			if (timeout_cntr > timeout / 5)            //��ʱ
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
//** ��������: Uart4WriteFull
//** ��������: ���ݶ���д���������
//** �䡡��: Buf:ָ�����ݶ���
//**        Data:��Ҫ��ӵ�����
//**        Mod: ��ӷ�ʽ
//** �䡡��:TRUE  :�ɹ�
//**        FALSE:ʧ��
//** ȫ�ֱ���: ��
//** ����ģ��: ��
//**-------------------------------------------------------------------------------------------------------
//** �޸���: ������
//** �ա���: 2004��12��28��
//**-------------------------------------------------------------------------------------------------------
//** �޸���: ������
//** �ա���: 2004��12��28��
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
//	//    while ( temp <= QueueNData((void *)Buf) )         /* �ȴ����ݶ��в��� */
//	//    {
//	////  	OSTimeDly(2);
//	//    }
//	//    return QueueWrite((void *)Buf, Data);           /* ����������� */
//
//	return ok;
//}
//
///******************************************************************************
// �������ƣ�uart4_init
// ������������ʼ��UART4
// ����������
// �������ƣ� ����/����� ����		����
//
// ��  ��  ֵ����
//
// �� 	 �� ��������
// �� 	 �ڣ�2005-04-11
// �޸���ʷ��
//		����		�޸���		�޸�����
//		------		---------	-------------
//******************************************************************************/
//void uart4_init(void)
//{
//	//GPIO�˿�����
//	GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//
//	(void)QueueCreate((void *)UART4SendBuf, sizeof(UART4SendBuf), NULL, (uint8 (*)())Uart4WriteFull);
//	(void)QueueCreate((void *)UART4RcvBuf, sizeof(UART4RcvBuf), NULL, (uint8 (*)())Uart4WriteFull);
//
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);   //ʹ��GPIOCʱ��
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);	//ʹ��USART4ʱ��
//
//	USART_DeInit(UART4);  //��λ����4
//
//	//USART4_TX   PC.10
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 				//PC.10
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		//�����������
//	GPIO_Init(GPIOC, &GPIO_InitStructure); 					//��ʼ��
//
//	//USART4_RX	  PC.11
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
//	GPIO_Init(GPIOC, &GPIO_InitStructure);					//��ʼ��
//
//
//	//USART ��ʼ������
////  USART_InitStructure.USART_BaudRate = 9600; 							//һ������Ϊ9600;
//	USART_InitStructure.USART_BaudRate = 115200; 							//һ������Ϊ9600;
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;			//�ֳ�Ϊ8λ���ݸ�ʽ
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;				//һ��ֹͣλ
//	USART_InitStructure.USART_Parity = USART_Parity_No;					//����żУ��λ
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//�շ�ģʽ
//	USART_Init(UART4, &USART_InitStructure);							//��ʼ������
//
//	//Usart4 NVIC ����
//	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;      //�����ȼ�1
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQͨ��ʹ��
//	NVIC_Init(&NVIC_InitStructure); //����ָ���Ĳ�����ʼ��VIC�Ĵ���
//
//	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE); //���������ж�
//
//	USART_Cmd(UART4, ENABLE);                    //ʹ�ܴ���
//}
//
///******************************************************************************
// �������ƣ�uart4_set_bps
// �������������ô���4�����ʣ�����żУ�鷽ʽ
// ����������
// �������ƣ� ����/����� ����		����
// bps		����		INT32U		Ҫ���õĲ�����
// databit	����		INT8U		����λ��ѡ8 9
// stopbit	����		INT8U		ֹͣλ��ѡ1 2
// odd_even	����		INT8U		У�鷽ʽ��NONEΪ�ޣ�ODDΪ��У�飬EVENΪżУ��
//
// ��  ��  ֵ����
//
// �� 	 �� ��������
// �� 	 �ڣ�2005-04-11
// �޸���ʷ��
//		����		�޸���		�޸�����
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
//		USART_InitStructure.USART_WordLength = USART_WordLength_8b; //�ֳ�Ϊ8λ���ݸ�ʽ
//	}
//	else
//	{
//		USART_InitStructure.USART_WordLength = USART_WordLength_9b; //�ֳ�Ϊ9λ���ݸ�ʽ
//	}
//
//	if ( stopbit == 1 )
//	{
//		USART_InitStructure.USART_StopBits = USART_StopBits_1; //һ��ֹͣλ
//	}
//	else
//	{
//		USART_InitStructure.USART_StopBits = USART_StopBits_2; //����ֹͣλ
//	}
//
//	if ( odd_even == NONE )
//	{
//		USART_InitStructure.USART_Parity = USART_Parity_No; //����żУ��λ
//	}
//	else if ( odd_even == ODD )
//	{
//		USART_InitStructure.USART_Parity = USART_Parity_Odd; //��У��
//	}
//	else
//	{
//		USART_InitStructure.USART_Parity = USART_Parity_Even; //żУ��
//	}
//
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ
//
//	USART_Init(UART4, &USART_InitStructure); //��ʼ������
//}
//
///******************************************************************************
// �������ƣ�uart4_buf_flush
// ������������մ���4����
// ����������
// �������ƣ� ����/����� ����		����
//
// ��  ��  ֵ����
//
// �� 	 �� ��������
// �� 	 �ڣ�2005-04-11
// �޸���ʷ��
//		����		�޸���		�޸�����
//		------		---------	-------------
//******************************************************************************/
//void uart4_buf_flush(void)
//{
//	//  INT8U i = 0;
//	//  volatile INT8U j = 0;
//
//	ToggleWD();
//
//	//  U0IER = 0x00;								/* �رս��պͷ����ж� */
//	//  U0FCR = 0x03;								/* ��ʼ��FIFO 1�ֽ��ж� ��RX FIFO*/
//	//  for ( i=0; i<17; i++ )
//	//  {
//	//  	j = U0RBR;			//����FIFO
//	//  }
//	//  U0IER = 0x05;								/* ������պͷ����ж� */
//
//	QueueFlush(UART4RcvBuf);
//}
//
///******************************************************************************
// �������ƣ�uart4_putch
// ��������������4����һ���ֽ�����
// ����������
// �������ƣ� ����/����� ����		����
// dat		����		INT8U		Ҫ���͵�����
//
// ��  ��  ֵ����
//
// �� 	 �� ��������
// �� 	 �ڣ�2012-04-06
// �޸���ʷ��
//		����		�޸���		�޸�����
//		------		---------	-------------
//******************************************************************************/
//void uart4_putch(INT8U dat)
//{
////	ToggleWD();
////
////	QueueWrite((void *)UART4SendBuf, dat);         /* ������� */
////
////	USART_ITConfig(UART4, USART_IT_TXE, ENABLE);  /* �������ж� */
//
//	USART_SendData(UART4, dat);
//	while( USART_GetFlagStatus(UART4, USART_FLAG_TC) != SET )			//�ȴ��������
//		;
//}
//
///******************************************************************************
// �������ƣ�uart4_write
// ��������������4����һ������
// ����������
// �������ƣ� ����/����� ����		����
// snd_buf	����		INT8U *		Ҫ���͵�����
// snd_length	����		INT16U		Ҫ�������ݵĳ���
//
// ��  ��  ֵ����
//
// �� 	 �� ��������
// �� 	 �ڣ�2012-04-06
// �޸���ʷ��
//		����		�޸���		�޸�����
//		------		---------	-------------
//******************************************************************************/
//void uart4_write(INT8U const * const snd_buf, INT16U const snd_length)
//{
//	INT16U i = 0;
//
//	ToggleWD();
//
//	USART_ITConfig(UART4, USART_IT_RXNE, DISABLE);  /* ��ֹ�����ж� */
//	for ( i = 0; i < snd_length; i++ )
//	{
//		uart4_putch(snd_buf[i]);
//	}
//	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);  /* ��������ж� */
//}
//
///******************************************************************************
// �������ƣ�UART4_IRQHandler
// ����������UART4�жϷ������
// ����������
// �������ƣ� ����/����� ����		����
//
// ��  ��  ֵ����
//
// �� 	 �� ��������
// �� 	 �ڣ�2012-04-06
// �޸���ʷ��
//		����		�޸���		�޸�����
//		------		---------	-------------
//******************************************************************************/
//void UART4_IRQHandler(void)
//{
//	INT8U rcv = 0;
//	INT8U snd = 0;
//
//	//������յ�������
//	if ( USART_GetITStatus(UART4, USART_IT_RXNE) != RESET )
//	{
//		rcv = USART_ReceiveData(UART4);        //��ȡ���յ�������
//		QueueWrite((void *)UART4RcvBuf, rcv);         /* ������� */
//	}
//
//	//�����ж�
//	if ( USART_GetITStatus(UART4, USART_IT_TXE) != RESET )
//	{
//		if ( QueueRead(&snd, UART4SendBuf) == QUEUE_OK )
//		{
//			USART_SendData(UART4, snd);
//		}
//		else
//		{
//			/* ���пգ����ֹ�����ж� */
//			USART_ClearITPendingBit(UART4, USART_IT_TXE);
//			USART_ITConfig(UART4, USART_IT_TXE, DISABLE);
//		}
//	}
//
//}
//
///******************************************************************************
// �������ƣ�com4_send_hex
// ��������������4����һ������
// ����������
// �������ƣ� ����/����� ����		����
// str_to_send	����	INT8U *		Ҫ���͵��ַ���
// len			����	INT16U		����
//
// ��  ��  ֵ����
//
// �� 	 �� ��������
// �� 	 �ڣ�2005-04-07
// �޸���ʷ��
//		����		�޸���		�޸�����
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
//����ԭ�ͣ�com4_gets
//��������������4����
//����������
//�������ƣ�		����/�����	����		����
//-----------		-----------	------	   	-------
//str_got			���		INT8U*		�õ����ַ���
//timeout_start	����		INT32U		���ֳ�ʱ����λmS��0����
//timeout			����		INT16U		�����ֽڼ����ʱ����λmS
//
// ��  ��  ֵ��ok(0)-�ɹ�
//				 KEY_CANCEL-���˳�����
//				 notok(0xFF)-ʧ�ܣ���ʱ
//��      ��	��������
//��      �ڣ�2005-08-20
//�޸���ʷ��
//����		�޸���		�޸�����
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
//	//��һ���ֽ�
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
//				if (timeout_cntr > timeout_start / 5)          //��ʱ
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
//	//�����ֽ�
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
//			if (timeout_cntr > timeout / 5)            //��ʱ
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

