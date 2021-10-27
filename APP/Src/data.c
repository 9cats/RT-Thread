#include "data.h"

//RT-Thread 相关
rt_sem_t LED0Toggle_Sem; //LED0翻转信号量
rt_sem_t LED1Toggle_Sem; //LED1翻转信号量

//可储存变量初始数据
SVAR Svar = { //32731
  /*float ADS_AMP;         //所有ADS倍数补偿 */0.965000033f,
  /*float ADS_OFFSET;      //所有ADS倍数补偿 */32731.0f,
  /*u16   ANTI_SHAKE_PHASE;//消抖偏移        */3000,
  /*u32   OFFSET_PHASE;    //相位偏移        */43088,
  /*float A;               //线性拟合系数A   */40.915f,
  /*float B;               //线性拟合系数B   */1.3252f,
  /*float C;               //线性拟合系数C   */80.313f,
};

//DEBUG
#define ADS8688_BUF_MAX 5000
u8    ADS8688_STA       = YES;
u32   ADS8688_COUNT     = 0;
u16 ADS8688_BUF [ADS8688_BUF_MAX] = {0};
u16 ADS8688_BUF1[ADS8688_BUF_MAX] = {0};


/*!
 *  \brief  RT-Thread 相关信号量等的初始化
 */
void RT_Thread_Data_Init(void)
{
	LED0Toggle_Sem = rt_sem_create("LED0_Toggle", 5, RT_IPC_FLAG_PRIO);
	LED1Toggle_Sem = rt_sem_create("LED1_Toggle", 5, RT_IPC_FLAG_PRIO);
}
