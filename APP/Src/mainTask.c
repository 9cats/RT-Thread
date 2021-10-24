#include "rtthread.h"
#include "main.h"

static rt_thread_t Main_Thread = RT_NULL; //���߳�����

void Main_Thread_Entry(void *parameter)	{
	for(;;){
		rt_thread_mdelay(2000);
		rt_kprintf("2333\r\n");
	}
}



void RT_Thread_Threads_Init(void) {

	/* SmallTask ��ʼ�� */
	extern void SmallTask_Init(void);
	SmallTask_Init();

	/* �� �� �� */
	Main_Thread = rt_thread_create("Main",			//�߳���
																	Main_Thread_Entry,		//�߳���ں���
																	RT_NULL,							//�߳���ں�������
																	9120,									//�̷߳����ջ��С
																	18,										//�߳����ȼ�
																	1);										//�߳�ʱ��Ƭ
	rt_thread_startup(Main_Thread);												//�߳�����
}
