#ifndef __OXYGEN_H_
#define __OXYGEN_H_


#include "data_type.h"   
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//DS18B20��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/12
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
void Oxygen(void);

float DO(float t);

float DOreal(float t);

float Solid(float t);

//  I������t���¶�, n����
float DO_shiji(float I, float t, float n);
#endif















