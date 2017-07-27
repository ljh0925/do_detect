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


//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; //2MHz
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	GPIO_ResetBits(GPIOC, GPIO_Pin_8);

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
//��ʼ��IIC�ӿ�
void AT24CXX_Init(void)
{
	IIC_Init(); //IIC��ʼ��
}
//��AT24CXXָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ
//����ֵ  :����������
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{
	u8 temp = 0;
	IIC_Start();
	if ( EE_TYPE > AT24C16 )
	{
		IIC_Send_Byte(0XA0);       //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr >> 8); //���͸ߵ�ַ
	}
	else
		IIC_Send_Byte(0XA0 + ((ReadAddr / 256) << 1));   //����������ַ0XA0,д����
	IIC_Wait_Ack();
	IIC_Send_Byte(ReadAddr % 256);   //���͵͵�ַ
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(0XA1);           //�������ģʽ
	IIC_Wait_Ack();
	temp = IIC_Read_Byte(0);
	IIC_Stop(); //����һ��ֹͣ����
	return ( temp );
}
//��AT24CXXָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ
//DataToWrite:Ҫд�������
void AT24CXX_WriteOneByte(u16 WriteAddr, u8 DataToWrite)
{
	IIC_Start();
	if ( EE_TYPE > AT24C16 )
	{
		IIC_Send_Byte(0XA0);        //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr >> 8); //���͸ߵ�ַ
	}
	else
		IIC_Send_Byte(0XA0 + ((WriteAddr / 256) << 1));   //����������ַ0XA0,д����
	IIC_Wait_Ack();
	IIC_Send_Byte(WriteAddr % 256);   //���͵͵�ַ
	IIC_Wait_Ack();
	IIC_Send_Byte(DataToWrite);     //�����ֽ�
	IIC_Wait_Ack();
	IIC_Stop(); //����һ��ֹͣ����
	SleepMs(10);
}
//��AT24CXX�����ָ����ַ��ʼд�볤��ΪLen������
//�ú�������д��16bit����32bit������.
//WriteAddr  :��ʼд��ĵ�ַ
//DataToWrite:���������׵�ַ
//Len        :Ҫд�����ݵĳ���2,4
void AT24CXX_WriteLenByte(u16 WriteAddr, u32 DataToWrite, u8 Len)
{
	u8 t;
	for ( t = 0; t < Len; t++ )
	{
		AT24CXX_WriteOneByte(WriteAddr + t, (DataToWrite >> (8 * t)) & 0xff);
	}
}

//��AT24CXX�����ָ����ַ��ʼ��������ΪLen������
//�ú������ڶ���16bit����32bit������.
//ReadAddr   :��ʼ�����ĵ�ַ
//����ֵ     :����
//Len        :Ҫ�������ݵĳ���2,4
u32 AT24CXX_ReadLenByte(u16 ReadAddr, u8 Len)
{
	u8 t;
	u32 temp = 0;
	for ( t = 0; t < Len; t++ )
	{
		temp <<= 8;
		temp += AT24CXX_ReadOneByte(ReadAddr + Len - t - 1);
	}
	return ( temp );
}
//���AT24CXX�Ƿ�����
//��������24XX�����һ����ַ(255)���洢��־��.
//���������24Cϵ��,�����ַҪ�޸�
//����1:���ʧ��
//����0:���ɹ�
u8 AT24CXX_Check(void)
{
	u8 temp;
	temp = AT24CXX_ReadOneByte(255); //����ÿ�ο�����дAT24CXX
	if ( temp == 0X55 )
		return ( 0 );
	else //�ų���һ�γ�ʼ�������
	{
		AT24CXX_WriteOneByte(255, 0X55);
		temp = AT24CXX_ReadOneByte(255);
		if ( temp == 0X55 )
			return ( 0 );
	}
	return ( 1 );
}

//��AT24CXX�����ָ����ַ��ʼ����ָ������������
//ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
//pBuffer  :���������׵�ַ
//NumToRead:Ҫ�������ݵĸ���
void AT24CXX_Read(u16 ReadAddr, u8 *pBuffer, u16 NumToRead)
{
	while ( NumToRead )
	{
		*pBuffer++ = AT24CXX_ReadOneByte(ReadAddr++);
		NumToRead--;
	}
}
//��AT24CXX�����ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
//pBuffer   :���������׵�ַ
//NumToWrite:Ҫд�����ݵĸ���
void AT24CXX_Write(u16 WriteAddr, u8 *pBuffer, u16 NumToWrite)
{
	while ( NumToWrite-- )
	{
		AT24CXX_WriteOneByte(WriteAddr, *pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}








