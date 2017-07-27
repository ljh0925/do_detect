

#include "all.h"

extern  volatile float do_real_I;
extern  volatile short temprature; 
volatile float do_oxygen = 0.0;

INT8U oxoftemp_table[40] = {
	146,142,138,134,131,127,124,121,118,115,
	113,110,108,105,103,101,99,97,95,93,
	91,	89,87,86,84,83,81,80,78,77,
	76,74,73,72,71,70,68,67,66,65
}


//通过输入的温度数据，查表找到对应的溶氧数值
INT8U searchtable(INT8U temp)
{

}

//最终的溶氧数据，小数点后一位
void Oxygen(void)
{
	INT8U t = 0;
	INT8U t = 0;
	t = (INT8U)(temprature / 10.0 + 0.5);			//温度，DS18B20采集到的数据是256，需要除以是0，得到25.6摄氏度
	//do_oxygen = DO_shiji(do_real_I, t, 400);     //do_real_I，氧电极采集的电流数据

	//以下是查表，利用温度查找出，对应的溶氧数值（空气中饱和溶氧的氧分压）
	do_oxygen = searchtable(t);
	//do_oxygen = -do_real_I * 10;
}

float DO(float t)
{ 
	return ( 0.0029 * t * t - 0.3136 * t + 14.2623 );
}

float DOreal(float t)
{
	return ( -0.0026 * t * t + 0.6472 * t + 32.9833 );
}
float Solid(float t)
{
	return ( (0.037 * t * t - 2.912 * t + 91.231) * 0.001 );
}

//  I电流，t是温度, n是盐
float DO_shiji(float I, float t, float n)
{
	return ( (I / DOreal(t)) * DO(t) - Solid(t) * n );
}




















