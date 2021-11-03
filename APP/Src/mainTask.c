#include "rtthread.h"
#include "main.h"
#include "ADS8688.h"
#include "AD9959.h"
#include "tim.h"
#include "cmd_process.h"
#include "output.h"
#include "cmd_thread.h"

static rt_thread_t Main_Thread = RT_NULL; //���߳�����
float mean[800];


float THRESHOL =  0.01f;
float is_Smooth(float* data, u32 len)
{
	float result,max,min,useless;
	arm_mean_f32(data , len, &result);

	arm_max_f32(data ,len,&max,(u32*)&useless); //�������ֵ
	arm_min_f32(data ,len,&min,(u32*)&useless); //������Сֵ

	if(( (max-result)/result< THRESHOL )\
	 &&( (result-min)/result< THRESHOL ))
		return result;
	else return result+10000.0f;
}


void Main_Thread_Entry(void *parameter)
{
	//AD9959_Init();
	//TFT_Init();
	ADS8688_CONFIG(0x01, 0x06);

	delay_ms(1000);
  extern void arm_iir_init(void);
  arm_iir_init();
  extern void arm_fir_init(void);
  arm_fir_init();

  //ADS�����ö�ʱ��
	HAL_TIM_Base_Start_IT(&htim14);
	for(;;){
		delay_ms(10);
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
																	17,										//�߳����ȼ�
																	1);										//�߳�ʱ��Ƭ
	rt_thread_startup(Main_Thread);												//�߳�����
}
