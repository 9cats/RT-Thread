#include "data.h"

//RT-Thread ���
rt_sem_t LED0Toggle_Sem; //LED0��ת�ź���
rt_sem_t LED1Toggle_Sem; //LED1��ת�ź���

//�ɴ��������ʼ����
SVAR Svar = { //32731
  /*float ADS_AMP;         //����ADS�������� */0.965000033f,
  /*float ADS_OFFSET;      //����ADS�������� */32731.0f,
  /*u16   ANTI_SHAKE_PHASE;//����ƫ��        */3000,
  /*u32   OFFSET_PHASE;    //��λƫ��        */43088,
  /*float A;               //�������ϵ��A   */40.915f,
  /*float B;               //�������ϵ��B   */1.3252f,
  /*float C;               //�������ϵ��C   */80.313f,
};

//DEBUG
#define ADS8688_BUF_MAX 5000
u8    ADS8688_STA       = YES;
u32   ADS8688_COUNT     = 0;
u16 ADS8688_BUF [ADS8688_BUF_MAX] = {0};
u16 ADS8688_BUF1[ADS8688_BUF_MAX] = {0};


/*!
 *  \brief  RT-Thread ����ź����ȵĳ�ʼ��
 */
void RT_Thread_Data_Init(void)
{
	LED0Toggle_Sem = rt_sem_create("LED0_Toggle", 5, RT_IPC_FLAG_PRIO);
	LED1Toggle_Sem = rt_sem_create("LED1_Toggle", 5, RT_IPC_FLAG_PRIO);
}
