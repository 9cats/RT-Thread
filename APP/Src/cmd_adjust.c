/**
 ******************************************************************************
 * @file    data_adjust.c
 * @brief   ���� �� �������������ݽ�׼
 ******************************************************************************
 */
#include "main.h"
#include "rtthread.h"
#include "cmd_thread.h"
#include "cmd_process.h"

/*!
 *  \brief  ����У׼ �̺߳������
 *  \param  data ���׼������
 */
void Data_Adjust_Entry(char *data)
{
	float* buf = rt_malloc(800*4);	//BUF����ռ�
	float mean;											//ƽ��ֵ
	u16 i;

	if (!rt_strcmp(data, "ADS_OFFSET"))
	{
		for(i=0; i<800; i++)
		{
			buf[i] = V_recv;
			rt_thread_mdelay(1);
		}
		arm_mean_f32(buf , 800, &mean);
		Svar.ADS_OFFSET = mean;
		SetTextValue(0, 2, Str("%f",Svar.ADS_OFFSET));
	}

	else if (!rt_strcmp(data, "A"))
	{
		for(i=0; i<800; i++)
		{
			buf[i] = V_iir;
			rt_thread_mdelay(1);
		}
		arm_mean_f32(buf , 800, &mean);
		Svar.A = mean;
		SetTextValue(0, 5, Str("%f",Svar.A));
	}

	else if (!rt_strcmp(data, "B"))
	{
		for(i=0; i<800; i++)
		{
			buf[i] = V_iir;
			rt_thread_mdelay(1);
		}
		arm_mean_f32(buf , 800, &mean);
		Svar.B = mean;
		SetTextValue(0, 6, Str("%f",Svar.B));
	}

	else if (!rt_strcmp(data, "C"))
	{
		for(i=0; i<800; i++)
		{
			buf[i] = V_iir;
			rt_thread_mdelay(1);
		}
		arm_mean_f32(buf , 800, &mean);
		Svar.C = mean;
		SetTextValue(0, 7, Str("%f",Svar.C));
	}


  //�����ccmram���ͷſռ�
  if(IN_CCMRAM(data)) rt_free(data);
}

/*!
 *  \brief  ����У׼�̴߳���
 *  \param  data ���׼������
 */
void Data_Adjust(char *argv)
{
	rt_thread_startup(
			rt_thread_create("adjustData",			//�߳���
		(void (*)(void *)) Data_Adjust_Entry, //�߳���ں���
											 (void *)argv,			//�߳���ں�������
											 2048,							//�̷߳����ջ��С
											 17,								//�߳����ȼ�
											 1)									//�߳�ʱ��Ƭ
	);
}

/*!
 *  \brief  ����У׼�̴߳���
 *  \param  argc �����������
 *  \param  argv ���������
 *  \note   ���� argv[1] �����������
 */
void adjust(int argc, char **argv)
{
  char *cmd = rt_malloc(40);
  u8 cmd_len = rt_strlen(argv[1]);
  rt_strncpy(cmd, argv[1], cmd_len);

	rt_thread_startup(
			rt_thread_create("adjustData",			//�߳���
		(void (*)(void *)) Data_Adjust_Entry, //�߳���ں���
											 (void *)cmd,			//�߳���ں�������
											 2048,							//�̷߳����ջ��С
											 17,								//�߳����ȼ�
											 1)									//�߳�ʱ��Ƭ
	);
}
MSH_CMD_EXPORT(adjust, adjust data sample: adjust<dataName>);
