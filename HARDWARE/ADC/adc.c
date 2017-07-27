//#include "adc.h"
#include "all.h"
#include "delay.h"

#define ADS1110_WR_ADDR			0X90
#define ADS1110_RD_ADDR			0X91
#define ADS1110_REG_CONFIG		0X8C


//获得ADC值
s16 Get_Adc(void)
{
//	s16 hi_byte = 0;
//	s16 low_byte = 0;
	s16 last_byte = 0;
//	INT8U s[3];
//
//	ads1110_init();
//
//
//	IIC_Start();
//	IIC_Send_Byte(ADS1110_RD_ADDR);
//	IIC_Wait_Ack();
//
//	hi_byte = IIC_Read_Byte();
//	IIC_Ack();
//	low_byte = IIC_Read_Byte();
//	IIC_Ack();


	//	i2c_start();
	//	i2c_send(ADS1110_RD_ADDR);
	//	i2c_nack();
	//
	//	hi_byte = i2c_receive();
	//	i2c_ack();
	//	low_byte = i2c_receive();
	//	i2c_ack();
	//	last_byte = i2c_receive();
	//	i2c_stop();
	//
	//	last_byte = (hi_byte<<8) + low_byte;


	//	i2c_send_and_receive(0X90, 0X01, s, 3);
	//
	//
	//	memcpy((void*)&last_byte, s, 2);

	return ( last_byte );

}


INT8U ads1110_read(INT16S *ad_value)
{
	INT8U ucTemp = 0;
	INT16U uiTemp = 0;

	ToggleWD();

	IIC_Start(); 
	IIC_Send_Byte(ADS1110_RD_ADDR); 
	IIC_Wait_Ack();

	uiTemp = (INT16U)(IIC_Read_Byte(1)); // 读ADC数据
	uiTemp <<= 8;
	//ADC_I2C_Acknowledge();
	uiTemp |= (INT16U)(IIC_Read_Byte(1));
	//ADC_I2C_Acknowledge();

	ucTemp = IIC_Read_Byte(1);         // 读配置字,读配置
	//ADC_I2C_Acknowledge();

	IIC_Stop(); 

	if ( (ucTemp & 0x80) == 0x00 )        // 是否是新数据判断
	{
		*ad_value = (INT16S)uiTemp;
		return ( ok );
	}
	else
	{
		return ( notok );
	}
	
}

//配置ADS1110工作模式
void ads1110_init(void)
{
	//	i2c_start();
	//	i2c_send(ADS1110_WR_ADDR);
	//	i2c_nack();
	//	i2c_send(ADS1110_REG_CONFIG);
	//	i2c_nack();

	IIC_Start();
	IIC_Send_Byte(ADS1110_WR_ADDR);
	IIC_Wait_Ack();
	IIC_Send_Byte(ADS1110_REG_CONFIG);
	IIC_Wait_Ack();
}

//获取AD转换值，取times次,然后平均
//返回值:数字量电压的times次转换结果平均值
s16 Get_Adc_Average(u8 times)
{
	s32 temp_val = 0;
	INT16S ad_val = 0;
	u8 t;
	uchar ret = 0;

	for ( t = 0; t < times; t++ )
	{
		ret = ads1110_read(&ad_val);
		if(ret  != ok )
			continue;
		temp_val += ad_val;
		delay_ms(5);
	}

	return ( temp_val / times );
}










