#include "rtthread.h"
#include "main.h"

static rt_thread_t LED0Toggle_Thread = RT_NULL; //LED0 ��˸
static rt_thread_t LED1Toggle_Thread = RT_NULL; //LED1 ��˸
static rt_thread_t DataUpdate_Thread = RT_NULL; //�����Զ�����


/*!
 *  \brief  LED0��ת�߳����
 */
void LED0Toggle_Thread_Entry(void *parameter)	{
	for(;;)
	{
		if(rt_sem_take(LED0Toggle_Sem, RT_WAITING_FOREVER) == RT_EOK)
		{
			LED0_ON;
			rt_thread_mdelay(50);
			LED0_OFF;
			rt_thread_mdelay(50);
		}
	}
}

/*!
 *  \brief  LED1��ת�߳����
 */
void LED1Toggle_Thread_Entry(void *parameter)	{
	for(;;)
	{
		if(rt_sem_take(LED1Toggle_Sem, RT_WAITING_FOREVER) == RT_EOK)
		{
			LED1_ON;
			rt_thread_mdelay(50);
			LED1_OFF;
			rt_thread_mdelay(50);
		}
	}
}

/*!
 *  \brief  ��׼�����Զ������߳����
 */
void Data_AutoUpdate_Tread_Entry(void *arguement)	{
	for(;;)
	{
		DATA_UPDATE();
		rt_thread_mdelay(100);
	}
}

/*!
 *  \brief  С�����ʼ��
 */
void SmallTask_Init(void)
{
	/* �� �� LED0 ʵ ʱ �� ˸ �� �� */
	LED0Toggle_Thread = rt_thread_create("led0_t",			//�߳���
													LED0Toggle_Thread_Entry,		//�߳���ں���
																			RT_NULL,				//�߳���ں�������
																			220,						//�̷߳����ջ��С
																			18,							//�߳����ȼ�
																			1);							//�߳�ʱ��Ƭ
	rt_thread_startup(LED0Toggle_Thread);								//�߳�����

	/* �� �� LED1 ʵ ʱ �� ˸ �� �� */
	LED1Toggle_Thread = rt_thread_create("led1_t",			//�߳���
													LED1Toggle_Thread_Entry,		//�߳���ں���
																			RT_NULL,				//�߳���ں�������
																			220,						//�̷߳����ջ��С
																			18,							//�߳����ȼ�
																			1);							//�߳�ʱ��Ƭ
	rt_thread_startup(LED1Toggle_Thread);								//�߳�����


	/* �� �� ��׼�����Զ����� �� �� */
	DataUpdate_Thread = rt_thread_create("dateUpdate",	//�߳���
											Data_AutoUpdate_Tread_Entry,		//�߳���ں���
																RT_NULL,							//�߳���ں�������
																1024,									//�̷߳����ջ��С
																25,										//�߳����ȼ�
																1);										//�߳�ʱ��Ƭ
	rt_thread_startup(DataUpdate_Thread);								//�߳�����
}
