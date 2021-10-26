#include "rtthread.h"
#include "main.h"
#include "ADS8688.h"
#include "tim.h"

static rt_thread_t Main_Thread = RT_NULL; //主线程任务

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

	/* SmallTask 初始化 */
	extern void SmallTask_Init(void);
	SmallTask_Init();

	/* 主 线 程 */
	Main_Thread = rt_thread_create("Main",			//线程名
																	Main_Thread_Entry,		//线程入口函数
																	RT_NULL,							//线程入口函数参数
																	9120,									//线程分配堆栈大小
																	18,										//线程优先级
																	1);										//线程时间片
	rt_thread_startup(Main_Thread);												//线程启动
}
