#include "data.h"

//RT-Thread 相关
rt_sem_t LED0Toggle_Sem; //LED0翻转信号量
rt_sem_t LED1Toggle_Sem; //LED1翻转信号量

//可储存变量初始数据
SVAR Svar = {
  /*float ADS_AMP;        */  10.24f,
  /*u16   ADS_OFFSET_ALL; */  0x8000
};


/*!
 *  \brief  RT-Thread 相关信号量等的初始化
 */
void RT_Thread_Data_Init(void)
{
	LED0Toggle_Sem = rt_sem_create("LED0_Toggle", 5, RT_IPC_FLAG_PRIO);
	LED1Toggle_Sem = rt_sem_create("LED1_Toggle", 5, RT_IPC_FLAG_PRIO);
}
