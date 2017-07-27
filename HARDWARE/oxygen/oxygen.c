

#include "all.h"

extern  volatile float do_real_I;
extern  volatile short temprature; 
volatile float do_oxygen = 0.0;

//�����������¶ȵĶ��ձ������������������εģ���Ҫ����10�õ���ʵ����
INT8U oxoftemp_table[40] = {
	146,142,138,134,131,127,124,121,118,115,
	113,110,108,105,103,101,99,97,95,93,
	91,	89,87,86,84,83,81,80,78,77,
	76,74,73,72,71,70,68,67,66,65
};

INT16U do_of_getv[29][2] = {
	{3.2 ,201 },
	{3.9 ,283 },
	{4.1 ,345 },
	{4.4 ,368 },
	{4.7 ,390 },
	{5.1 ,461 },
	{5.2 ,487 },
	{5.3 ,504 },
	{5.8 ,541 },
	{6.3 ,612 },
	{6.53,629 },
	{6.7 ,638 },
	{6.8 ,687 },
	{7.28,715 },
	{7.4 ,820 },
	{7.8 ,878 },
	{8.4 ,923 },
	{8.5 ,940 },
	{8.6 ,957 },
	{9.5 ,1020 },
	{9.6 ,1043 },
	{10 ,1076 },
	{10.2 ,1096 },
	{11.2 ,1120 },
	{12.73,1374 },
	{13.9 ,1395 },
	{14 ,1461 },
	{14.29,1583 },
	{17.4 ,1725 }
};

//ͨ��������¶����ݣ�����ҵ���Ӧ��������ֵ
INT8U searchtable(INT8U temp)
{
	return ( oxoftemp_table[temp] );
}

//���ݲ����ĵ�ѹ����ҵ���Ӧ��������ֵ
INT8U do_from_table(INT16U value)
{
	INT8U i = 0;
	INT8U do_temp = 0;

	for(i=0;i<29;i++)
	{
		if(value <= do_of_getv[i][1])
			break;
	}

	do_temp = (INT8U)do_of_getv[i][0] * (value / do_of_getv[i][1])

	//do_temp = do_of_getv[i][0];
	return ( do_temp );
}

//���յ��������ݣ�С�����һλ
void Oxygen(void)
{
	INT8U v_do_get = 0;
	INT8U t = 0;


	t = (INT8U)(temprature / 10.0 + 0.5);			//�¶ȣ�DS18B20�ɼ�����������256����Ҫ������0���õ�25.6���϶�
	

	//�����ǲ�������¶Ȳ��ҳ�����Ӧ��������ֵ�������б�������������ѹ��
	//do_oxygen = searchtable(t);

	//�����缫�ɼ�����������ѹ��ֵ
//	v_do_get = -do_real_I * 10;

	//ֱ�Ӹ��ݵ�ѹ�������ʾ������������������

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

//  I������t���¶�, n����
float DO_shiji(float I, float t, float n)
{
	return ( (I / DOreal(t)) * DO(t) - Solid(t) * n );
}


