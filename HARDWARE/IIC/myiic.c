
/******************************************************************************
Copyright 2005 �����״�ͨ�������޹�˾
All rights riserved.

�ļ���	��drv_buzzer.c
ģ�����ƣ���������������
���ܸ�Ҫ����������������

ȡ���汾��0.0.1
�޸���	��������
������ڣ�2005.04.06
����˵����create

******************************************************************************/
#include "all.h"
//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//24CXX ��������
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/6
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////
//��ʼ��IIC
void IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //ʹ��PA�˿�ʱ��  GPIOA2,3��ʼ������
														  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure); //��ʼ��


	IIC_SCL = 1;
	IIC_SDA = 1;
}
//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA = 1;
	IIC_SCL = 1;
	sleep_us(4);
	IIC_SDA = 0; //START:when CLK is high,DATA change form high to low
	sleep_us(4);
	IIC_SCL = 0; //ǯסI2C���ߣ�׼�����ͻ��������
}
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT(); //sda�����
	IIC_SCL = 0;
	IIC_SDA = 0; //STOP:when CLK is high DATA change form low to high
	sleep_us(4);
	IIC_SCL = 1;
	IIC_SDA = 1; //����I2C���߽����ź�
	sleep_us(4);
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime = 0;
	SDA_IN();      //SDA����Ϊ����
	IIC_SDA = 1;
	sleep_us(1);
	IIC_SCL = 1;
	sleep_us(1);
	while ( READ_SDA )
	{
		ucErrTime++;
		if ( ucErrTime > 250 )
		{
			IIC_Stop();
			return ( 1 );
		}
	}
	IIC_SCL = 0; //ʱ�����0
	return ( 0 );
}

//����ACKӦ��
void IIC_Ack(void)
{
	IIC_SCL = 0;
	SDA_OUT();
	IIC_SDA = 0;
	sleep_us(2);
	IIC_SCL = 1;
	sleep_us(2);
	IIC_SCL = 0;
}

//������ACKӦ��
void IIC_NAck(void)
{
	IIC_SCL = 0;
	SDA_OUT();
	IIC_SDA = 1;
	sleep_us(2);
	IIC_SCL = 1;
	sleep_us(2);
	IIC_SCL = 0;
}

void SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //ʹ��PG�˿�ʱ��
														   //GPIO2,3��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure); //��ʼ��
}


void SDA_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //ʹ��PG�˿�ʱ��
														   //GPIO2,3��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure); //��ʼ��
}

//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��
void IIC_Send_Byte(u8 txd)
{
	u8 t;
	SDA_OUT();
	IIC_SCL = 0; //����ʱ�ӿ�ʼ���ݴ���
	for ( t = 0; t < 8; t++ )
	{
		IIC_SDA = (txd & 0x80) >> 7;
		txd <<= 1;
		sleep_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL = 1;
		sleep_us(2);
		IIC_SCL = 0;
		sleep_us(2);
	}
}

//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i, receive = 0;
	SDA_IN(); //SDA����Ϊ����
	for ( i = 0; i < 8; i++ )
	{
		IIC_SCL = 0;
		sleep_us(2);
		IIC_SCL = 1;
		receive <<= 1;
		if ( READ_SDA )
			receive++;
		sleep_us(1);
	}
	if ( !ack )
		IIC_NAck(); //����nACK
	else
		IIC_Ack(); //����ACK
	return ( receive );
}







