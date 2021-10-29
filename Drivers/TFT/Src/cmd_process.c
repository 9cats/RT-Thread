#include "base.h"
#include "hmi_driver.h"
#include "hmi_user_uart.h"
#include "cmd_queue.h"
#include "cmd_process.h"
#include "stdlib.h"
#include "stdio.h"
#include "rtthread.h"
#include "cmd_thread.h"

/*! 
 *  \brief  ��Ϣ��������
 *  \param msg ��������Ϣ
 *  \param size ��Ϣ����
 */

uint8 cmd_buffer[CMD_MAX_SIZE]; //ָ���

void ProcessMessage(PCTRL_MSG msg, uint16 size)
{
	uint8 cmd_type = msg->cmd_type;				   //ָ������
	uint8 ctrl_msg = msg->ctrl_msg;				   //��Ϣ������
	uint8 control_type = msg->control_type;		   //�ؼ�����
	uint16 screen_id = PTR2U16(&msg->screen_id);   //����ID
	uint16 control_id = PTR2U16(&msg->control_id); //�ؼ�ID
	uint32 value = PTR2U32(msg->param);			   //��ֵ

	switch (cmd_type)
	{
	case NOTIFY_TOUCH_PRESS:   //����������
	case NOTIFY_TOUCH_RELEASE: //�������ɿ�
		NotifyTouchXY(cmd_buffer[1], PTR2U16(cmd_buffer + 2), PTR2U16(cmd_buffer + 4), NULL);
		break;
	case NOTIFY_WRITE_FLASH_OK: //дFLASH�ɹ�
		NotifyWriteFlash(1, NULL);
		break;
	case NOTIFY_WRITE_FLASH_FAILD: //дFLASHʧ��
		NotifyWriteFlash(0, NULL);
		break;
	case NOTIFY_READ_FLASH_OK:	//��ȡFLASH�ɹ�
		NotifyReadFlash(1, cmd_buffer + 2, size - 6, NULL); //ȥ��֡ͷ֡β
		break;
	case NOTIFY_READ_FLASH_FAILD: //��ȡFLASHʧ��
		NotifyReadFlash(0, 0, 0, NULL);
		break;
	case NOTIFY_READ_RTC: //��ȡRTCʱ��
		NotifyReadRTC(cmd_buffer[1], cmd_buffer[2], cmd_buffer[3], cmd_buffer[4], cmd_buffer[5], cmd_buffer[6], cmd_buffer[7], NULL);
		break;
	case NOTIFY_CONTROL:
	{
		if (ctrl_msg == MSG_GET_CURRENT_SCREEN) //����ID�仯֪ͨ
		{
			NotifyScreen(screen_id, NULL);
		}
		else
		{
			switch (control_type)
			{
			case kCtrlButton: //��ť�ؼ�
				NotifyButton(screen_id, control_id, msg->param[1], NULL);
				break;
			case kCtrlText: //�ı��ؼ�
				NotifyText(screen_id, control_id, msg->param, NULL);
				break;
			case kCtrlProgress: //�������ؼ�
				NotifyProgress(screen_id, control_id, value, NULL);
				break;
			case kCtrlSlider: //�������ؼ�
				NotifySlider(screen_id, control_id, value, NULL);
				break;
			case kCtrlMeter: //�Ǳ�ؼ�
				NotifyMeter(screen_id, control_id, value, NULL);
				break;
			case kCtrlMenu: //�˵��ؼ�
				NotifyMenu(screen_id, control_id, msg->param[0], msg->param[1], NULL);
				break;
			case kCtrlSelector: //ѡ��ؼ�
				NotifySelector(screen_id, control_id, msg->param[0], NULL);
				break;
			case kCtrlRTC: //����ʱ�ؼ�
				NotifyTimer(screen_id, control_id, NULL);
				break;
			default:
				break;
			}
		}
	}
	break;
	default:
		break;
	}
}

void SetTFTText(uint16 screen_id, uint16 control_id, char *text, ...)
{
}

void SetTextValueInt32(uint16 screen_id, uint16 control_id, int32 value)
{
	uchar buffer[12] = {0};
	snprintf((char *)buffer, sizeof(buffer), "%ld", value); //������ת��Ϊ�ַ���
	SetTextValue(screen_id, control_id, buffer);
}

void SetTextValueFloat(uint16 screen_id, uint16 control_id, float value)
{
	/*
	uchar buffer[12] = {0};
	snprintf((char *)buffer, sizeof(buffer), "%.1f", value); //�Ѹ�����ת��Ϊ�ַ���(����һλС��)
	SetTextValue(screen_id, control_id, buffer);
	*/
}

void UpdateUI()
{
}

/*! 
 *  \brief  �����л�֪ͨ
 *  \details  ��ǰ����ı�ʱ(�����GetScreen)��ִ�д˺���
 *  \param screen_id ��ǰ����ID
 */
void NotifyScreen(uint16 screen_id, void *userdata)
{
}

/*! 
 *  \brief  ���������¼���Ӧ
 *  \param press 1���´�������3�ɿ�������
 *  \param x x����
 *  \param y y����
 */
void NotifyTouchXY(uint8 press, uint16 x, uint16 y, void *userdata)
{
}

/*! 
 *  \brief  ��ť�ؼ�֪ͨ
 *  \details  ����ť״̬�ı�(�����GetControlValue)ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param state ��ť״̬��0����1����
 */
void NotifyButton(uint16 screen_id, uint16 control_id, uint8 state, void *userdata)
{
	void Cteate_Data_Adjust_Thread(u16 screen_id, u16 control_id);
	if(state)
	{
		if(screen_id == 0 && control_id == 51)
		{
			Data_Adjust("A");
		}
		if(screen_id == 0 && control_id == 52)
		{
			Data_Adjust("B");
		}
		if(screen_id == 0 && control_id == 53)
		{
			Data_Adjust("C");
		}
			if(screen_id == 0 && control_id == 54)
		{
			Data_Adjust("ADS_OFFSET");
		}
		if(screen_id == 0 && control_id == 55)
		{
			Data_Adjust("AUTO");
		}
	}
}

/*! 
 *  \brief  �ı��ؼ�֪ͨ
 *  \details  ���ı�ͨ�����̸���(�����GetControlValue)ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param str �ı��ؼ�����
 */
void NotifyText(uint16 screen_id, uint16 control_id, uint8 *str, void *userdata)
{
	if(screen_id == 0 && control_id == 1)
	{
		Svar.ADS_AMP = atoi((char*) str);
	}
	if(screen_id == 0 && control_id == 2)
	{
		Svar.ADS_OFFSET    = atoi((char*) str);
	}
	if(screen_id == 0 && control_id == 3)
	{
		Svar.ANTI_SHAKE_PHASE = atoff((char*) str);
	}
	if(screen_id == 0 && control_id == 4)
	{
		Svar.OFFSET_PHASE = atoff((char*) str);
	}
	if(screen_id == 0 && control_id == 5)
	{
		Svar.A               = atoff((char*) str);
	}
	if(screen_id == 0 && control_id == 6)
	{
		Svar.B               = atoff((char*) str);
	}
	if(screen_id == 0 && control_id == 7)
	{
		Svar.C               = atoff((char*) str);
	}
}

/*! 
 *  \brief  �������ؼ�֪ͨ
 *  \details  ����GetControlValueʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param value ֵ
 */
void NotifyProgress(uint16 screen_id, uint16 control_id, uint32 value, void *userdata)
{
}

/*! 
 *  \brief  �������ؼ�֪ͨ
 *  \details  ���������ı�(�����GetControlValue)ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param value ֵ
 */
void NotifySlider(uint16 screen_id, uint16 control_id, uint32 value, void *userdata)
{
	if(screen_id == 0 && control_id == 42)
	{
		if(value == 360) value=0;
		u32 Phase = 84000000 / 360 / 1000 * value;
		if(Phase < 250) Phase = 0;
		else Phase = Phase-250;
		Svar.OFFSET_PHASE = Phase;

		SetTextValue(0, 4, Str("%ld",Svar.OFFSET_PHASE));
	}
}

/*! 
 *  \brief  �Ǳ�ؼ�֪ͨ
 *  \details  ����GetControlValueʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param value ֵ
 */
void NotifyMeter(uint16 screen_id, uint16 control_id, uint32 value, void *userdata)
{
}

/*! 
 *  \brief  �˵��ؼ�֪ͨ
 *  \details  ���˵���»��ɿ�ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param item �˵�������
 *  \param state ��ť״̬��0�ɿ���1����
 */
void NotifyMenu(uint16 screen_id, uint16 control_id, uint8 item, uint8 state, void *userdata)
{
}

/*! 
 *  \brief  ѡ��ؼ�֪ͨ
 *  \details  ��ѡ��ؼ��仯ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param item ��ǰѡ��
 */

void NotifySelector(uint16 screen_id, uint16 control_id, uint8 item, void *userdata)
{
}

/*! 
 *  \brief  ��ʱ����ʱ֪ͨ����
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 */
void NotifyTimer(uint16 screen_id, uint16 control_id, void *userdata)
{
}

/*! 
 *  \brief  ��ȡ�û�FLASH״̬����
 *  \param status 0ʧ�ܣ�1�ɹ�
 *  \param _data ��������
 *  \param length ���ݳ���
 */
void NotifyReadFlash(uint8 status, uint8 *_data, uint16 length, void *userdata)
{
}

/*! 
 *  \brief  д�û�FLASH״̬����
 *  \param status 0ʧ�ܣ�1�ɹ�
 */
void NotifyWriteFlash(uint8 status, void *userdata)
{
}

/*! 
 *  \brief  ��ȡRTCʱ�䣬ע�ⷵ�ص���BCD��
 *  \param year �꣨BCD��
 *  \param month �£�BCD��
 *  \param week ���ڣ�BCD��
 *  \param day �գ�BCD��
 *  \param hour ʱ��BCD��
 *  \param minute �֣�BCD��
 *  \param second �루BCD��
 */
void NotifyReadRTC(uint8 year, uint8 month, uint8 week, uint8 day, uint8 hour, uint8 minute, uint8 second, void *userdata)
{
}
