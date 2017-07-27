
#include "all.h"

#define TIME_100MS 10
#define TIME_1000MS 100



uint8 cmd_buffer[CMD_MAX_SIZE];

extern volatile short temprature;
extern volatile float do_oxygen; 
volatile uint8 update_en = 0;
volatile uint32  test_value = 0; //��ʱ������

/*! 
 *  \brief  ��Ϣ�������̣��˴�һ�㲻��Ҫ����
 *  \param msg ��������Ϣ
 *  \param size ��Ϣ����
 */
void ProcessMessage(PCTRL_MSG msg, uint16 size)
{
	uint8 cmd_type = msg->cmd_type; //ָ������
									//uint8 ctrl_msg = msg->ctrl_msg;   //��Ϣ������
	uint8 control_type = msg->control_type; //�ؼ�����
	uint16 screen_id = PTR2U16(&msg->screen_id); //����ID
	uint16 control_id = PTR2U16(&msg->control_id); //�ؼ�ID
	uint32 value = PTR2U32(msg->param); //��ֵ

	switch ( cmd_type )
	{
		case NOTIFY_SCREEN: //�л�����
			NotifyScreen(screen_id);
			break;
		case NOTIFY_TOUCH_PRESS:
		case NOTIFY_TOUCH_RELEASE:
			NotifyTouchXY(cmd_buffer[1], PTR2U16(cmd_buffer + 2), PTR2U16(cmd_buffer + 4));
			break;
		case NOTIFY_WRITE_FLASH_OK:
			NotifyWriteFlash(1);
			break;
		case NOTIFY_WRITE_FLASH_FAILD:
			NotifyWriteFlash(0);
			break;
		case NOTIFY_READ_FLASH_OK:
			NotifyReadFlash(1, cmd_buffer + 2, size - 6); //ȥ��֡ͷ֡β
			break;
		case NOTIFY_READ_FLASH_FAILD:
			NotifyReadFlash(0, 0, 0);
			break;
		case NOTIFY_CONTROL:
		{
			switch ( control_type )
			{
				case kCtrlButton: //��ť�ؼ�
					NotifyButton(screen_id, control_id, msg->param[1]);
					break;
				case kCtrlText: //�ı��ؼ�
					NotifyText(screen_id, control_id, msg->param);
					break;
				case kCtrlProgress: //�������ؼ�
					NotifyProgress(screen_id, control_id, value);
					break;
				case kCtrlSlider: //�������ؼ�
					NotifySlider(screen_id, control_id, value);
					break;
				case kCtrlMeter: //�Ǳ�ؼ�
					NotifyMeter(screen_id, control_id, value);
					break;
				case kCtrlMenu: //�˵��ؼ�
					NotifyMenu(screen_id, control_id, msg->param[0], msg->param[1]);
					break;
				case kCtrlSelector: //ѡ��ؼ�
					NotifySelector(screen_id, control_id, msg->param[0]);
					break;
				case kCtrlRTC: //����ʱ�ؼ�
					NotifyTimer(screen_id, control_id);
					break;
				default:
					break;
			}
		}
			break;
		default:
			break;
	}
}

/*! 
 *  \brief  �����л�֪ͨ
 *  \details  ��ǰ����ı�ʱ(�����GetScreen)��ִ�д˺���
 *  \param screen_id ��ǰ����ID
 */
void NotifyScreen(uint16 screen_id)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  ���������¼���Ӧ
 *  \param press 1���´�������3�ɿ�������
 *  \param x x����
 *  \param y y����
 */
void NotifyTouchXY(uint8 press, uint16 x, uint16 y)
{
	//TODO: ����û�����
}

//�ı��ؼ���ʾ����ֵ
void SetTextValueInt32(uint16 screen_id, uint16 control_id, int32 value)
{
	char buffer[12] = { 0 };
	sprintf(buffer, "%2.1f", (float)value/10.0);
	SetTextValue(screen_id, control_id, (uchar *)buffer);
}

//�ַ���ת����
int32 StringToInt32(uint8 *str)
{
	int32 v = 0;
	sscanf((char *)str, "%ld", &v);
	return ( v );
}

//���½���ؼ���ʾ
void UpdateUI(void)
{
	//	SetProgressValue(0,3,test_value);
	//	SetSliderValue(0,4,test_value);
	SetMeterValue(0, 1, do_oxygen);  	//���������ı���
	SetTextValueInt32(0, 7, do_oxygen);

	SetMeterValue(0, 2, temprature );  	//�¶ȼ��ı���
	SetTextValueInt32(0, 4, temprature);
}

/*! 
 *  \brief  ��ť�ؼ�֪ͨ
 *  \details  ����ť״̬�ı�(�����GetControlValue)ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param state ��ť״̬��0����1����
 */
void NotifyButton(uint16 screen_id, uint16 control_id, uint8 state)
{
	//TODO: ����û�����
	if ( screen_id == 0 && control_id == 5 ) //������ť
	{
		if ( test_value < 100 )
			++test_value;
	}
	else if ( screen_id == 0 && control_id == 6 ) //�ݼ���ť
	{
		if ( test_value > 0 )
			--test_value;
	}

	//UpdateUI();
	update_en = 1;
}

/*! 
 *  \brief  �ı��ؼ�֪ͨ
 *  \details  ���ı�ͨ�����̸���(�����GetControlValue)ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param str �ı��ؼ�����
 */
void NotifyText(uint16 screen_id, uint16 control_id, uint8 *str)
{
	//TODO: ����û�����
	test_value = StringToInt32(str);

	if ( test_value > 100 )
		test_value = 100;
	else if ( test_value < 1 )
		test_value = 0;

	//UpdateUI();
	update_en = 1;
}

/*! 
 *  \brief  �������ؼ�֪ͨ
 *  \details  ����GetControlValueʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param value ֵ
 */
void NotifyProgress(uint16 screen_id, uint16 control_id, uint32 value)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  �������ؼ�֪ͨ
 *  \details  ���������ı�(�����GetControlValue)ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param value ֵ
 */
void NotifySlider(uint16 screen_id, uint16 control_id, uint32 value)
{
	//TODO: ����û�����
	test_value = value;

	//UpdateUI();
	update_en = 1;
}

/*! 
 *  \brief  �Ǳ�ؼ�֪ͨ
 *  \details  ����GetControlValueʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param value ֵ
 */
void NotifyMeter(uint16 screen_id, uint16 control_id, uint32 value)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  �˵��ؼ�֪ͨ
 *  \details  ���˵���»��ɿ�ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param item �˵�������
 *  \param state ��ť״̬��0�ɿ���1����
 */
void NotifyMenu(uint16 screen_id, uint16 control_id, uint8 item, uint8 state)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  ѡ��ؼ�֪ͨ
 *  \details  ��ѡ��ؼ��仯ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param item ��ǰѡ��
 */
void NotifySelector(uint16 screen_id, uint16 control_id, uint8 item)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  ��ʱ����ʱ֪ͨ����
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 */
void NotifyTimer(uint16 screen_id, uint16 control_id)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  ��ȡ�û�FLASH״̬����
 *  \param status 0ʧ�ܣ�1�ɹ�
 *  \param _data ��������
 *  \param length ���ݳ���
 */
void NotifyReadFlash(uint8 status, uint8 *_data, uint16 length)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  д�û�FLASH״̬����
 *  \param status 0ʧ�ܣ�1�ɹ�
 */
void NotifyWriteFlash(uint8 status)
{
	//TODO: ����û�����
}
