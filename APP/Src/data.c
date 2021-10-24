#include "data.h"

//RT-Thread ���
rt_sem_t LED0Toggle_Sem; //LED0��ת�ź���
rt_sem_t LED1Toggle_Sem; //LED1��ת�ź���

//�ɴ��������ʼ����
SVAR Svar = {
  /*float ADS_AMP;        */  10.24f,
  /*u16   ADS_OFFSET_ALL; */  0x8000
};


/*!
 *  \brief  RT-Thread ����ź����ȵĳ�ʼ��
 */
void RT_Thread_Data_Init(void)
{
	LED0Toggle_Sem = rt_sem_create("LED0_Toggle", 5, RT_IPC_FLAG_PRIO);
	LED1Toggle_Sem = rt_sem_create("LED1_Toggle", 5, RT_IPC_FLAG_PRIO);
}
