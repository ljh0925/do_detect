/******************************************************************************
Copyright 2005 �����״�ͨ�������޹�˾
All rights riserved.

�ļ���	��drv_switch.c
ģ�����ƣ�switch��������
���ܸ�Ҫ��switch��������

ȡ���汾��0.0.1
�޸���	��������
������ڣ�2005.04.06
����˵����create

******************************************************************************/
#include "all.h"


extern volatile DEV_STAT DevStat;
/******************************************************************************
�������ƣ�switch_init
����������led��ʾ��ʼ��
����������

��  ��  ֵ����
��      �ߣ�������
��      �ڣ�2005-04-08
�޸���ʷ��
����		�޸���		�޸�����
------		---------	---------
******************************************************************************/
void switch_init(void)
{
//	GPIO_InitTypeDef  GPIO_InitStructure;
//
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  //ʹ��PB�˿�ʱ��
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;				 //LED_RED-->PB.8 �˿�����
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //������������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		 //IO���ٶ�Ϊ2MHz
//	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.8
//	GPIO_SetBits(GPIOB, GPIO_Pin_10);					 	//PB.8 GREEN_RED�����
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				 //LED_GREEN-->PB.9 �˿�����
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //������������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		 //IO���ٶ�Ϊ2MHz
//	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.9
//	GPIO_SetBits(GPIOB, GPIO_Pin_11);					 	//PB.9 RED_LED�����
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				 //LED_RED-->PB.8 �˿�����
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //������������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		 //IO���ٶ�Ϊ2MHz
//	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.8
//	GPIO_SetBits(GPIOB, GPIO_Pin_12);					 	//PB.8 GREEN_RED�����
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				 //LED_GREEN-->PB.9 �˿�����
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //������������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		 //IO���ٶ�Ϊ2MHz
//	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.9
//	GPIO_SetBits(GPIOB, GPIO_Pin_13);					 	//PB.9 RED_LED�����
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;				 //LED_RED-->PB.8 �˿�����
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //������������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		 //IO���ٶ�Ϊ2MHz
//	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.8
//	GPIO_SetBits(GPIOB, GPIO_Pin_14);					 	//PB.8 GREEN_RED�����
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				 //LED_GREEN-->PB.9 �˿�����
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //������������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		 //IO���ٶ�Ϊ2MHz
//	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.9
//	GPIO_SetBits(GPIOB, GPIO_Pin_15);					 	//PB.9 RED_LED�����
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //LED_RED-->PB.8 �˿�����
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //������������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		 //IO���ٶ�Ϊ2MHz
//	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.8
//	GPIO_SetBits(GPIOB, GPIO_Pin_8);					 	//PB.8 GREEN_RED�����
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				 //LED_GREEN-->PB.9 �˿�����
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //������������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		 //IO���ٶ�Ϊ2MHz
//	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.9
//	GPIO_SetBits(GPIOB, GPIO_Pin_9);					 	//PB.9 RED_LED�����
//
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //OUT_KEY-->PB.5 �˿�����
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		     //������������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		 //IO���ٶ�Ϊ2MHz
//	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
//	GPIO_SetBits(GPIOB, GPIO_Pin_5);					 	 //PB.5
	//SetLight(NEITHER_LIGHT);
}

/******************************************************************************
��������: ReadSwitch
������������ȡ���뿪�ص�λ��
����������
�������ƣ�	����/���	����		����


��  ��  ֵ����
				
��      ��	��������
��      �ڣ�2015-09-23
�޸���ʷ��
����		�޸���		�޸�����
------		---------	---------
******************************************************************************/
INT8U ReadSwitch(void)
{
//	INT16U buf;
//    char tmp;
//
//	buf = ~GPIO_ReadInputData(GPIOB);
//
//    if((buf & 0x0100) == 0)
//    {
//		DevStat.param_flag = 0;
//    }
//    else
//    {
//        DevStat.param_flag = 1;
//    }
//    buf &= 0xfe00;
//
//    tmp = (INT8U)((buf >> 8));
//
//	return (INT8U)(shift_char(tmp));
	return 0;
}

unsigned char shift_char(unsigned char data) 
{ 
//    unsigned char i;
//    unsigned char tmp=0x00;
//
//    for(i=0;i<8;i++)
//    {
//        tmp=((data>>i)&0x01)|tmp;
//        if(i<7)
//            tmp=tmp<<1;
//    }
//
//    return tmp;
     return 0;
}




