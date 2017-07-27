/******************************************************************************
Copyright 2005 �����״�ͨ�������޹�˾
All rights riserved.

�ļ���	��task_general_control.c
ģ�����ƣ��ܿ����������
���ܸ�Ҫ���ܿ����������

ȡ���汾��0.0.1
�޸���	��������
������ڣ�2005.04.06
����˵����create

******************************************************************************/
#include 		"all.h"


extern volatile DEV_STAT DevStat;
extern const INT8U VERSION[];



volatile short temprature = 0;
extern volatile uint8 update_en;
volatile float do_real_I = 0.0;

/******************************************************************************
 �������ƣ�task_general_ctrl
 �����������ܿ�������
 ����������
 �������ƣ� ����/����� ����		����
				
 ��  ��  ֵ����
				   
 �� 	 �� ��������
 �� 	 �ڣ�2005-04-07
 �޸���ʷ��
		����		�޸���		�޸�����
		------		---------	-------------
******************************************************************************/
void task_general_ctrl(void *data)
{
	INT8U t = 0;
	INT8U j = 0;
	INT8U f_light = 1;
	INT16S adcx = 0;

	data = data;                    //avoid warning

	toggle_wd();

	for (;;)
	{
		toggle_wd();

		j++;
		if ( j >= 2 )
		{
			f_light = !f_light;
			j = 0;
			set_led(RED, f_light);
		}

		update_en = 1;
		t = 0;
		temprature = DS18B20_Get_Temp();
		if ( temprature < 0 )
		{
			temprature = 0;                        //����תΪ0�������ʾ�����
		}
		else
			;

		adcx = Get_Adc_Average(5);

		//temp = (float)adcx * (5.0 / 4096);       //��ѹֵ
		do_real_I = adcx;                          //�����ĵ�λ���ɰ� nA
		Oxygen(); 

		OSTimeDlyHMSM(0, 0, 0, 20);                //ÿ20ms�ɼ�һ����������
	}

}

///******************************************************************************
// �������ƣ�SaveCardInTime
// �����������뿨ʱ����ʱ��
// ����������
// �������ƣ�	����/�����	����		����
// card_info	����		CARD_INFO *		׼��Ҫ����ʱ��ı�����ָ��
//
// ��  ��  ֵ��ok(0)-�ɹ�
//				 notok(0xFF)-ʧ��
//
// ��      ��	��������
// ��      �ڣ�2004-09-02
// �޸���ʷ��
//		����		�޸���		�޸�����
//		------		---------	-------------
//******************************************************************************/
INT8U SaveCardInTime(CARD_INFO *card_info)
{
	//	INT8U i = 0;
	//	BUS_TIME time;

	ToggleWD();

	//	i = Get_Time(&time);


	//	i = CheckTimeFormat(&time);                             //���ʱ���ʽ
	//	if ( i != ok )
	//		return notok;

	//	memcpy((void *)&card_info->card_in_time, (void *)&time, sizeof(BUS_TIME));

	return ( ok );
}

///******************************************************************************
// �������ƣ�CheckPeriodOfValidity
// ������������Ч���ж�
// ����������
// �������ƣ�	����/�����	����		����
// start			����		INT32U		��������
// current		����		INT32U		��ǰ����
// end			����		INT32U		��Ч��ֹ����
//
// ��  ��  ֵ��ok(0)-�ɹ�
//				 notok(0xFF)-ʧ��(����)
//
// ��      ��	��������
// ��      �ڣ�2004-09-20
// �޸���ʷ��
//		����		�޸���		�޸�����
//		------		---------	-------------
//******************************************************************************/
INT8U CheckPeriodOfValidity(INT32U current, INT32U end)
{
	if ( DevStat.param_flag == 0 )  //���뿪�����λ��Ҫ�Ǹ߾Ϳ�ʼ��Ч�����ж�
	{
		return ( ok );
	}
	else
	{
		if ( (current <= end) )
			return ( ok );
		else
			return ( notok );
	}
}


