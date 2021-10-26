#include "data.h"

//RT-Thread 相关
rt_sem_t LED0Toggle_Sem; //LED0翻转信号量
rt_sem_t LED1Toggle_Sem; //LED1翻转信号量

//可储存变量初始数据
SVAR Svar = {
  /*float ADS_AMP;         //所有ADS倍数补偿 */0.965000033f,
  /*u16   ANTI_SHAKE_PHASE;//消抖偏移        */6000,
  /*u32   OFFSET_PHASE;    //相位偏移        */39660,
  /*float a;               //线性拟合系数a   */40.915f,
  /*float b;               //线性拟合系数b   */1.3252f,
  /*float c;               //线性拟合系数c   */80.313f,
  /*float d;               //线性拟合系数d   */1.2131f,
  /*float e;               //线性拟合系数e   */506.6619188224806f,
};


/*!
 *  \brief  RT-Thread 相关信号量等的初始化
 */
void RT_Thread_Data_Init(void)
{
	LED0Toggle_Sem = rt_sem_create("LED0_Toggle", 5, RT_IPC_FLAG_PRIO);
	LED1Toggle_Sem = rt_sem_create("LED1_Toggle", 5, RT_IPC_FLAG_PRIO);
}
