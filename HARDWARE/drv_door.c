/******************************************************************************
Copyright 2005 �����״�ͨ�������޹�˾
All rights riserved.

�ļ���	��drv_buzzer.c
ģ�����ƣ��������������
���ܸ�Ҫ���������������

ȡ���汾��0.0.1
�޸���	��������
������ڣ�2015.04.06
����˵����create

******************************************************************************/
#include "all.h"




/******************************************************************************
�������ƣ�door_init
�������������Ƶ��ʼ��
����������

��  ��  ֵ	����
��      ��	��������
��      �ڣ�2015-04-08
�޸���ʷ��
����		�޸���		�޸�����
------		---------	---------
******************************************************************************/
void door_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  //ʹ��PG�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;				 //buzzer-->PA.12 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		 //IO���ٶ�Ϊ2MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA.12
	GPIO_SetBits(GPIOC, GPIO_Pin_6);						 //PA.12 buzzer�����

	set_door(ON);
}

/******************************************************************************
 �������ƣ�set_door
 ���������������Ž�
 ����������
 �������ƣ�	����/�����	����		����
 bell_switch	����		INT8U	ON-���ţ�OFF-����
				
 ��  ��  ֵ����
				   
 ��      ��	��������
 ��      �ڣ�2015-09-02
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void set_door(INT8U door_switch)
{
	ToggleWD();

	if ( door_switch == ON )        //door  open
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_6);			//PA.12 �����
	}
	else if ( door_switch == OFF )  //door close
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_6);		//PA.12 �����
	}

}





