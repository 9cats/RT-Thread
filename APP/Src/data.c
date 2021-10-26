#include "data.h"

//RT-Thread ���
rt_sem_t LED0Toggle_Sem; //LED0��ת�ź���
rt_sem_t LED1Toggle_Sem; //LED1��ת�ź���

//�ɴ��������ʼ����
SVAR Svar = {
  /*float ADS_AMP;         //����ADS�������� */0.965000033f,
  /*u16   ANTI_SHAKE_PHASE;//����ƫ��        */6000,
  /*u32   OFFSET_PHASE;    //��λƫ��        */39660,
  /*float a;               //�������ϵ��a   */40.915f,
  /*float b;               //�������ϵ��b   */1.3252f,
  /*float c;               //�������ϵ��c   */80.313f,
  /*float d;               //�������ϵ��d   */1.2131f,
  /*float e;               //�������ϵ��e   */506.6619188224806f,
};


/*!
 *  \brief  RT-Thread ����ź����ȵĳ�ʼ��
 */
void RT_Thread_Data_Init(void)
{
	LED0Toggle_Sem = rt_sem_create("LED0_Toggle", 5, RT_IPC_FLAG_PRIO);
	LED1Toggle_Sem = rt_sem_create("LED1_Toggle", 5, RT_IPC_FLAG_PRIO);
}
