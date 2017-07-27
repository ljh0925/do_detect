
/******************************************************************************
Copyright 2005 大连易达通电子有限公司
All rights riserved.

文件名	：drv_buzzer.c
模块名称：蜂鸣器驱动程序
功能概要：蜂鸣器驱动程序

取代版本：0.0.1
修改人	：刘及华
完成日期：2005.04.06
升级说明：create

******************************************************************************/
#include "all.h"
//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//24CXX 驱动代码
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/6
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////
//初始化IIC
void IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //使能PA端口时钟  GPIOA2,3初始化设置
														  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure); //初始化


	IIC_SCL = 1;
	IIC_SDA = 1;
}
//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA = 1;
	IIC_SCL = 1;
	sleep_us(4);
	IIC_SDA = 0; //START:when CLK is high,DATA change form high to low
	sleep_us(4);
	IIC_SCL = 0; //钳住I2C总线，准备发送或接收数据
}
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT(); //sda线输出
	IIC_SCL = 0;
	IIC_SDA = 0; //STOP:when CLK is high DATA change form low to high
	sleep_us(4);
	IIC_SCL = 1;
	IIC_SDA = 1; //发送I2C总线结束信号
	sleep_us(4);
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime = 0;
	SDA_IN();      //SDA设置为输入
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
	IIC_SCL = 0; //时钟输出0
	return ( 0 );
}

//产生ACK应答
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

//不产生ACK应答
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

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //使能PG端口时钟
														   //GPIO2,3初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure); //初始化
}


void SDA_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //使能PG端口时钟
														   //GPIO2,3初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure); //初始化
}

//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答
void IIC_Send_Byte(u8 txd)
{
	u8 t;
	SDA_OUT();
	IIC_SCL = 0; //拉低时钟开始数据传输
	for ( t = 0; t < 8; t++ )
	{
		IIC_SDA = (txd & 0x80) >> 7;
		txd <<= 1;
		sleep_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL = 1;
		sleep_us(2);
		IIC_SCL = 0;
		sleep_us(2);
	}
}

//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i, receive = 0;
	SDA_IN(); //SDA设置为输入
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
		IIC_NAck(); //发送nACK
	else
		IIC_Ack(); //发送ACK
	return ( receive );
}







