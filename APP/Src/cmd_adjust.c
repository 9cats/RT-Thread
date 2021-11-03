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
	if (!rt_strcmp(data, "ADS_OFFSET"))
	{

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
  cmd[cmd_len] = 0;

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
