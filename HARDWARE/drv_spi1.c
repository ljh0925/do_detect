/******************************************************************************
Copyright 2005 �����״�ͨ�������޹�˾
All rights riserved.

�ļ���	��drv_spi1.c
ģ�����ƣ�spi1������������
���ܸ�Ҫ��spi1������������

ȡ���汾��0.0.1
�޸���	��������
������ڣ�2005.04.06
����˵����create

******************************************************************************/
#include "all.h"




/******************************************************************************
�������ƣ�spi1_init
����������spi1���߳�ʼ��
����������

��  ��  ֵ����
��      �ߣ�������
��      �ڣ�2005-04-08
�޸���ʷ��
����		�޸���		�޸�����
------		---------	---------
******************************************************************************/
void spi1_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //PORTAʱ��ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,  ENABLE); //SPI1ʱ��ʹ��

	//����SPI1��MISO PA.6Ϊ��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//����SPI1��MOSI PA.7��SCK PA.5Ϊ�����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;       //����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;       //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;     //����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;    //����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;       //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;        //���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256 APB2���72MHz  72/4=18MHz
//	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;        //���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256 APB2���72MHz  72/2=36MHz
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;    //CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���

	SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����

	spi1_readwritebyte(0xff); //��������
}

/******************************************************************************
 �������ƣ�spi1_setspeed
 ����������spi1�����ٶ����ú��� APB2���72MHz
 ����������
 �������ƣ� 				����/����� ����        ���� 
 SPI_BaudRatePrescaler 		����		INT8U		��Ƶ�� SPI_BaudRatePrescaler_2 ~ SPI_BaudRatePrescaler_256

 �� �� ֵ����
 ��    �ߣ�������
 ��    �ڣ�2005-04-08
 �޸���ʷ��
����		�޸���		�޸�����
------		---------	---------
******************************************************************************/
void spi1_setspeed(INT8U SPI_BaudRatePrescaler)
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI1->CR1 &= 0XFFC7;
	SPI1->CR1 |= SPI_BaudRatePrescaler;   //����SPI1�ٶ�
	SPI_Cmd(SPI1, ENABLE);
}

/******************************************************************************
 �������ƣ�spi1_readwritebyte
 ����������spi1�շ��ֽ�
 ����������
 �������ƣ� ����/����� ����		����
 txdata		����		INT8U		Ҫ���͵��ֽ�
				
 �� �� ֵ����ȡ�����ֽ�
				   
 ��    �ߣ�������
 ��    �ڣ�2005-04-07
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
INT8U spi1_readwritebyte(INT8U txdata)
{
	INT8U retry = 0;
	while ( SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET ) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
	{
		retry++;
		if ( retry > 200 )
			return 0;
	}
	SPI_I2S_SendData(SPI1, txdata); //ͨ������SPIx����һ������
	retry = 0;

	while ( SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET ) //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
	{
		retry++;
		if ( retry > 200 )
			return 0;
	}
	return SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx������յ�����
}



