#include "rtthread.h"
#include "main.h"

static rt_thread_t LED0Toggle_Thread = RT_NULL; //LED0 闪烁
static rt_thread_t LED1Toggle_Thread = RT_NULL; //LED1 闪烁
static rt_thread_t DataUpdate_Thread = RT_NULL; //数据自动保存


/*!
 *  \brief  LED0翻转线程入口
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
 *  \brief  LED1翻转线程入口
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
 *  \brief  较准数据自动更新线程入口
 */
void Data_AutoUpdate_Tread_Entry(void *arguement)	{
	for(;;)
	{
		DATA_UPDATE();
		rt_thread_mdelay(100);
	}
}

/*!
 *  \brief  小任务初始化
 */
void SmallTask_Init(void)
{
	/* 创 建 LED0 实 时 闪 烁 线 程 */
	LED0Toggle_Thread = rt_thread_create("led0_t",			//线程名
													LED0Toggle_Thread_Entry,		//线程入口函数
																			RT_NULL,				//线程入口函数参数
																			220,						//线程分配堆栈大小
																			18,							//线程优先级
																			1);							//线程时间片
	rt_thread_startup(LED0Toggle_Thread);								//线程启动

	/* 创 建 LED1 实 时 闪 烁 线 程 */
	LED1Toggle_Thread = rt_thread_create("led1_t",			//线程名
													LED1Toggle_Thread_Entry,		//线程入口函数
																			RT_NULL,				//线程入口函数参数
																			220,						//线程分配堆栈大小
																			18,							//线程优先级
																			1);							//线程时间片
	rt_thread_startup(LED1Toggle_Thread);								//线程启动


	/* 创 建 较准数据自动更新 线 程 */
	DataUpdate_Thread = rt_thread_create("dateUpdate",	//线程名
											Data_AutoUpdate_Tread_Entry,		//线程入口函数
																RT_NULL,							//线程入口函数参数
																1024,									//线程分配堆栈大小
																25,										//线程优先级
																1);										//线程时间片
	rt_thread_startup(DataUpdate_Thread);								//线程启动
}
