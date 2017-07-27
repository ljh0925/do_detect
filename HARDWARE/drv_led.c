/******************************************************************************
Copyright 2005 �����״�ͨ�������޹�˾
All rights riserved.

�ļ���	��drv_led.c
ģ�����ƣ�led��������
���ܸ�Ҫ��led��������

ȡ���汾��0.0.1
�޸���	��������
������ڣ�2005.04.06
����˵����create

******************************************************************************/
#include "all.h"



/******************************************************************************
�������ƣ�led_init
����������led��ʾ��ʼ��
����������

��  ��  ֵ����
��      �ߣ�������
��      �ڣ�2005-04-08
�޸���ʷ��
����		�޸���		�޸�����
------		---------	---------
******************************************************************************/
void led_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);    //ʹ��PB�˿�ʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                //LED_RED-->PB.9 �˿�����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;         //IO���ٶ�Ϊ2MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);                   //�����趨������ʼ��GPIOB.9
    GPIO_SetBits(GPIOB, GPIO_Pin_9);                         //PB.9 GREEN_RED�����

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;                //LED_GREEN-->PB.8 �˿�����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;         //IO���ٶ�Ϊ2MHz
    GPIO_Init(GPIOB, &GPIO_InitStructure);                   //�����趨������ʼ��GPIOB.8
    GPIO_SetBits(GPIOB, GPIO_Pin_8);                         //PB.8 RED_LED�����

    SetLight(NEITHER_LIGHT);
}

/******************************************************************************
��������  SetLight
�����������Ժ���ָʾ�ƽ��п���
����������
�������ƣ�	����/���	����		����
light_state	 ����		INT8U  ��4���궨��GREEN_LIGHT��RED_LIGHT��BOTH_LIGHT��NEITHE_LIGHT����ʾ

��  ��  ֵ����
                
��      ��	��������
��      �ڣ�2003-09-23
�޸���ʷ��
����		�޸���		�޸�����
------		---------	---------
******************************************************************************/
void SetLight(INT8U light_state)
{
    if ( light_state == GREEN_LIGHT )
    {
		GPIO_ResetBits(GPIOB, GPIO_Pin_8);						 	//PB.9 GREEN�����
		GPIO_SetBits(GPIOB ,GPIO_Pin_9);						 	//PB.8 RED�����

    }
    else if ( light_state == RED_LIGHT )
    {
		GPIO_SetBits(GPIOB, GPIO_Pin_8);						 	//PB.9 GREEN�����
		GPIO_ResetBits(GPIOB, GPIO_Pin_9);						 	//PB.8 RED�����
   
    }
    else if ( light_state == BOTH_LIGHT )
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_9);                          //PB.9 GREEN�����
        GPIO_ResetBits(GPIOB, GPIO_Pin_8);                          //PB.8 RED�����
    }
    else
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_9);                            //PB.9 GREEN�����
        GPIO_SetBits(GPIOB, GPIO_Pin_8);                            //PB.8 RED�����
    }
}

/******************************************************************************
��������  set_led
�����������Ժ���ָʾ�ƽ��п���
����������
�������ƣ�	����/���	����		���� 
which		����		INT8U		�ĸ���GREEN����RED 
on_off		����		INT8U		ON=����OFF=��

��  ��  ֵ����
                
��      ��	��������
��      �ڣ�2014-10-10
�޸���ʷ��
����		�޸���		�޸�����
------		---------	---------
******************************************************************************/
void set_led(INT8U which, INT8U on_off)
{
    if ( which == GREEN )
    {
        if ( on_off == ON )
            GPIO_ResetBits(GPIOB, GPIO_Pin_8);                          //PB.8 GREEN�����
        else
            GPIO_SetBits(GPIOB, GPIO_Pin_8);                            //PB.8 GREEN�����
    }
    else if ( which == RED )
    {
        if ( on_off == ON )
            GPIO_ResetBits(GPIOB, GPIO_Pin_9);                          //PB.9 RED�����
        else
            GPIO_SetBits(GPIOB, GPIO_Pin_9);                            //PB.9 RED�����
    }
    else
    {}

}

/******************************************************************************
��������  work_led
��������������ָʾ��
����������
�������ƣ�	

��  ��  ֵ����
                
��      ��	��������
��      �ڣ�2014-10-10
�޸���ʷ��
����		�޸���		�޸�����
------		---------	---------
******************************************************************************/
void work_led(void)
{
    set_led(GREEN, ON);
    SleepMs(1000);
    set_led(GREEN, OFF);
    SleepMs(1000);
}


