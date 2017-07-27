#ifndef __OXYGEN_H_
#define __OXYGEN_H_


#include "data_type.h"   
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//DS18B20驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/12
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
void Oxygen(void);

float DO(float t);

float DOreal(float t);

float Solid(float t);

//  I电流，t是温度, n是盐
float DO_shiji(float I, float t, float n);
#endif















