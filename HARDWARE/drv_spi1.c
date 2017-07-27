/******************************************************************************
Copyright 2005 大连易达通电子有限公司
All rights riserved.

文件名	：drv_spi1.c
模块名称：spi1总线驱动程序
功能概要：spi1总线驱动程序

取代版本：0.0.1
修改人	：刘及华
完成日期：2005.04.06
升级说明：create

******************************************************************************/
#include "all.h"




/******************************************************************************
函数名称：spi1_init
功能描述：spi1总线初始化
参数描述：

返  回  值：无
作      者：刘及华
日      期：2005-04-08
修改历史：
日期		修改人		修改描述
------		---------	---------
******************************************************************************/
void spi1_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //PORTA时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,  ENABLE); //SPI1时钟使能

	//配置SPI1的MISO PA.6为浮空输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//配置SPI1的MOSI PA.7和SCK PA.5为复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;       //设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;       //设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;     //串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;    //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;       //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;        //定义波特率预分频的值:波特率预分频值为256 APB2最大72MHz  72/4=18MHz
//	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;        //定义波特率预分频的值:波特率预分频值为256 APB2最大72MHz  72/2=36MHz
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;    //CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器

	SPI_Cmd(SPI1, ENABLE); //使能SPI外设

	spi1_readwritebyte(0xff); //启动传输
}

/******************************************************************************
 函数名称：spi1_setspeed
 功能描述：spi1总线速度设置函数 APB2最大72MHz
 参数描述：
 参数名称： 				输入/输出？ 类型        描述 
 SPI_BaudRatePrescaler 		输入		INT8U		分频数 SPI_BaudRatePrescaler_2 ~ SPI_BaudRatePrescaler_256

 返 回 值：无
 作    者：刘及华
 日    期：2005-04-08
 修改历史：
日期		修改人		修改描述
------		---------	---------
******************************************************************************/
void spi1_setspeed(INT8U SPI_BaudRatePrescaler)
{
	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI1->CR1 &= 0XFFC7;
	SPI1->CR1 |= SPI_BaudRatePrescaler;   //设置SPI1速度
	SPI_Cmd(SPI1, ENABLE);
}

/******************************************************************************
 函数名称：spi1_readwritebyte
 功能描述：spi1收发字节
 参数描述：
 参数名称： 输入/输出？ 类型		描述
 txdata		输入		INT8U		要发送的字节
				
 返 回 值：读取到的字节
				   
 作    者：刘及华
 日    期：2005-04-07
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
INT8U spi1_readwritebyte(INT8U txdata)
{
	INT8U retry = 0;
	while ( SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET ) //检查指定的SPI标志位设置与否:发送缓存空标志位
	{
		retry++;
		if ( retry > 200 )
			return 0;
	}
	SPI_I2S_SendData(SPI1, txdata); //通过外设SPIx发送一个数据
	retry = 0;

	while ( SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET ) //检查指定的SPI标志位设置与否:接受缓存非空标志位
	{
		retry++;
		if ( retry > 200 )
			return 0;
	}
	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据
}



