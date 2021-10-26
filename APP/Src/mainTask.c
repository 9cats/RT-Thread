#include "rtthread.h"
#include "main.h"
#include "ADS8688.h"
#include "tim.h"

static rt_thread_t Main_Thread = RT_NULL; //���߳�����

void Main_Thread_Entry(void *parameter)	{
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	ADS8688_CONFIG(0x01, 0x00);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, htim2.Instance->ARR/2);
	__HAL_TIM_ENABLE_IT(&htim2, TIM_IT_UPDATE);
	for(;;){
		rt_thread_mdelay(2000);
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
