/******************************************************************************
Copyright 2005 �����״�ͨ�������޹�˾
All rights riserved.

�ļ���	��drv_i2c.c
ģ�����ƣ�IO��ģ��I2C������������
���ܸ�Ҫ��IO��ģ��I2C������������

ȡ���汾��0.0.1
�޸���	��������
������ڣ�2005.04.06
����˵����create

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
����ԭ�ͣ�i2c_init
����������I2C���߳�ʼ������Ӧ�ú��������ȵ��ñ�����
����������
�������ƣ�	����/�����	����		����
-----------		-----------	------	   	-------

��  ��  ֵ����
��      �ߣ�������
��      �ڣ�2003-05-12
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
void i2c_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //ʹ��PC�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_8;	//SDA-->PC.8,SCL-->PC.9 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		//��ͨ���ģʽ
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					//�����趨������ʼ��GPIO
	GPIO_SetBits(GPIOC, GPIO_Pin_9|GPIO_Pin_8);			    //�����


#ifdef I2C_USE_SEM
	i2c_sem = OSSemCreate(1);			/* �ź������ڻ���������� */
#endif


}

/*****************************************************************
����ԭ�ͣ�i2c_get_sda_stat
����������ȡ��I2C������SDA��״̬
����������
�������ƣ�	����/�����	����		����
-----------		-----------	------	   	-------

��  ��  ֵ��0-LOW
				1-HIGH
��      ��	��������
��      �ڣ�2003-05-12
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
INT8U i2c_get_sda_stat(void)
{
	INT8U i = 0;
	GPIO_InitTypeDef  GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				//SDA-->PC.8 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 	//��ͨ����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO���ٶ�Ϊ50MHz
	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//����
	GPIO_Init(GPIOC, &GPIO_InitStructure);					//�����趨������ʼ��GPIO
	//PB9����ģʽ
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


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				//SDA-->PC.8 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 	//��ͨ���ģʽ
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					//�����趨������ʼ��GPIO
	//PB9���ģʽ
//	GPIOB->MODER &= ~(3<<(7*2));
//	GPIOB->MODER |= 1<<7*2;

	i2c_delay2();

	return i;

}

/*****************************************************************
����ԭ�ͣ�i2c_get_scl_stat
����������ȡ��I2C������SCL��״̬
����������
�������ƣ�	����/�����	����		����
-----------		-----------	------	   	-------

��  ��  ֵ��0-LOW
				1-HIGH
��      ��	��������
��      �ڣ�2003-05-12
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
INT8U i2c_get_scl_stat(void)
{
	INT8U i = 0;
	GPIO_InitTypeDef  GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				//SCL-->PC.9 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 	//��ͨ����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO���ٶ�Ϊ50MHz
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//����
	GPIO_Init(GPIOC, &GPIO_InitStructure);					//�����趨������ʼ��GPIO
	//PB8����ģʽ
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


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				//SCL-->PC.9 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		//��ͨ���ģʽ
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;			//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					//�����趨������ʼ��GPIO
	//PB6���ģʽ
//	GPIOB->MODER &= ~(3<<(6*2));
//	GPIOB->MODER |= 1<<6*2;

	i2c_delay2();

	return i;
}



/////////////////////////////////////////////////////////
// ����Ϊ����Ҫ��д����
/////////////////////////////////////////////////////////



/*****************************************************************
����ԭ�ͣ�I2C_start
�����������ṩI2C���߹���ʱ���е���ʼλ
����������
�������ƣ�	����/�����	����		����
-----------		-----------	------	   	-------

��  ��  ֵ��ok(0)-�����ɹ�
				notok(0xFF)-����ʧ��
��      ��	��������
��      �ڣ�2003-05-12
�޸���ʷ��
����		�޸���		�޸�����
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
����ԭ�ͣ�i2c_start2
�����������ṩI2C���߹���ʱ���е���ʼλ
����������
�������ƣ�	����/�����	����		����
-----------		-----------	------	   	-------

��  ��  ֵ��ok(0)-�����ɹ�
				notok(0xFF)-����ʧ��
��      ��	��������
��      �ڣ�2003-05-12
�޸���ʷ��
����		�޸���		�޸�����
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
����ԭ�ͣ�I2C_stop
�����������ṩI2C���߹���ʱ���е�ֹͣλ
����������
�������ƣ�	����/�����	����		����
-----------		-----------	------	   	-------

��  ��  ֵ����
��      ��	��������
��      �ڣ�2003-05-12
�޸���ʷ��
����		�޸���		�޸�����
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
����ԭ�ͣ�I2C_Ack
������������I2C���߷���һ��Ӧ���ź�ACK��һ�������������ݶ�ȡʱ��
����������
�������ƣ�	����/�����	����		����

��  ��  ֵ����
��      ��	��������
��      �ڣ�2003-05-12
�޸���ʷ��
����		�޸���		�޸�����
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
����ԭ�ͣ�i2c_nack
����������αӦ��
����������
�������ƣ�  ����/����� ����        ����
-----------     ----------- ------      -------
// ��  ��  ֵ����
��      ��  ��������
��      �ڣ�2003-05-12
�޸���ʷ��
����        �޸���      �޸�����
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
����ԭ�ͣ�bit    I2C_clock
�����������ṩ I2C ���ߵ�ʱ���źţ���������ʱ�ӵ�ƽΪ
���ڼ�SDA�ź�����״̬����           �������������ݷ��ͣ�
Ҳ���������ݽ��ա�
����������
�������ƣ�	����/�����	����		����
-----------		-----------	------	   	-------

��  ��  ֵ��SDA�ź��ߵ�״̬(0|1)
��      ��	��������
��      �ڣ�2003-05-12
�޸���ʷ��
����		�޸���		�޸�����
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
����ԭ�ͣ�delay
����������������ʵ����ֻ��һ����������ھ���Ӧ���п��Ӿ���Ҫ��������ʱָ��
������������
�������ƣ�	����/�����	����		����
-----------		-----------	------	   	-------

��  ��  ֵ����
��      ��	��������
��      ��	��2003-05-12
�޸���ʷ��
����		�޸���		�޸�����
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
����ԭ�ͣ�delay
����������������ʵ����ֻ��һ����������ھ���Ӧ���п��Ӿ���Ҫ��������ʱָ��
������������
�������ƣ�	����/�����	����		����
-----------		-----------	------	   	-------

��  ��  ֵ����
��      ��	��������
��      ��	��2003-05-12
�޸���ʷ��
����		�޸���		�޸�����
------		---------	-------------
*****************************************************************/
void i2c_delay2(void)
{
//	Sleep10us(100);
//	Sleep10us(1);
	sleep_us(5);
}

/*****************************************************************
����ԭ�ͣ�bit   I2C_send
������������I2C���߷���8 λ����,������һ��Ӧ���ź�ACK��
			      ����SDA �ź�����״̬
�������������͵�һ�ֽ�8λ��������
�������ƣ�	����/�����	����		����
data_to_send    ����	INT8U     ���͵��ֽ�

��  ��  ֵ��ok(0)-��ȷ
				notok(0xFF)-����
��      ��	��������
��      �ڣ�2003-05-12
�޸���ʷ��
����		�޸���		�޸�����
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
����ԭ�ͣ�I2C_receive
������������I2C���߽���8λ�����źţ��������յ���8λ������Ϊһ���ֽڷ��أ�������Ӧ���ź�ACK��
�������ڵ��ñ�����֮ǰӦ��֤SDA�ź��ߴ��ڸ���״̬����ʹ8051�ýŵ�ƽΪ1��
����������
�������ƣ�	����/�����	����		����

��  ��  ֵ�����ܵ���һ���ֽ�����
��      ��	��������
��      �ڣ�2003-05-12
�޸���ʷ��
�޸���ʷ��
����		�޸���		�޸�����
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
�����ӵ�ַ������ȡ���ֽ����ݺ���               
����ԭ��: bit  ISendStr(INT8U sla,INT8U suba,ucahr *s,INT8U no);  
����:  ���������ߵ����͵�ַ���ӵ�ַ,�����ݣ�
�������ߵ�ȫ����,��������ַsla���ӵ�ַsuba��
���������ݷ���sָ��Ĵ洢������no���ֽڡ�
�������1��ʾ�����ɹ��������������
ע�⣺    ʹ��ǰ�����ѽ������ߡ�
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

	i = i2c_start();					//���i2c_start����������10��
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
		*s=i2c_receive();				/*��������*/
		i2c_ack();				   /*���;ʹ�λ*/  
		s++;
	} 
	*s=i2c_receive();

	i2c_nack();					/*���ͷ�Ӧλ*/
	i2c_stop();					   /*��������*/ 

//  OS_EXIT_CRITICAL();
#ifdef I2C_USE_SEM
	OSSemPost(i2c_sem);
#endif

	return ok;

}

