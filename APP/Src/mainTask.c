#include "rtthread.h"
#include "main.h"
#include "ADS8688.h"
#include "tim.h"
#include "cmd_process.h"
#include "output.h"

static rt_thread_t Main_Thread = RT_NULL; //主线程任务
float mean[800];


float THRESHOL =  0.01f;
float is_Smooth(float* data, u32 len)
{
	float result,max,min,useless;
	arm_mean_f32(data , len, &result);

	arm_max_f32(data ,len,&max,(u32*)&useless); //计算最大值
	arm_min_f32(data ,len,&min,(u32*)&useless); //计算最小值

	if(( (max-result)/result< THRESHOL )\
	 &&( (result-min)/result< THRESHOL ))
		return result;
	else return result+10000.0f;
}


void Main_Thread_Entry(void *parameter)	{

	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	ADS8688_CONFIG(0x01, 0x02);
	TFT_Init();

	SetTextValue(0, 1, Str("%f", Svar.ADS_AMP));
	SetTextValue(0, 2, Str("%f",Svar.ADS_OFFSET));
	SetTextValue(0, 3, Str("%d",Svar.ANTI_SHAKE_PHASE));
	SetTextValue(0, 4, Str("%ld",Svar.OFFSET_PHASE));
	SetTextValue(0, 5, Str("%f",Svar.A));
	SetTextValue(0, 6, Str("%f",Svar.B));
	SetTextValue(0, 7, Str("%f",Svar.C));

	//锁相所用定时器
  __HAL_TIM_ENABLE_IT(&htim10 , TIM_IT_UPDATE);
  __HAL_TIM_ENABLE_IT(&htim11 , TIM_IT_UPDATE);
  __HAL_TIM_ENABLE_IT(&htim13, TIM_IT_UPDATE);
  __HAL_TIM_ENABLE_IT(&htim14, TIM_IT_UPDATE);
  HAL_TIM_IC_Start_IT(&htim8 , TIM_CHANNEL_4);
  HAL_TIM_IC_Start_IT(&htim8 , TIM_CHANNEL_3);

  extern void arm_iir_init(void);
  arm_iir_init();
  //extern void arm_fir_init(void);
  //arm_fir_init();

  //ADS采样用定时器
  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, htim2.Instance->ARR-2);
  __HAL_TIM_ENABLE_IT(&htim2, TIM_IT_UPDATE);

	for(;;){
		float mean_result;
		u16 i;

		for(i=0; i<800; i++)
		{
			mean[i] = V_iir;
			rt_thread_mdelay(1);
		}

		mean_result = is_Smooth(mean, 800);

		SetTextValue(0, 31, Str("%.3f", mean_result));
		if(mean_result < 10000) {
			if( mean_result < Svar.B )
			{
				SetTextValue(0, 32, Str("%.3f", (mean_result - Svar.A)*90/(Svar.B - Svar.A) + 10 ));
				//SetTextValue(0, 32, Str("%.3f", ((mean_result - Svar.A)*(Svar.B - Svar.A)/54+6) * 10 / 6 ));
			}
			else
			{
				SetTextValue(0, 32, Str("%.3f", (mean_result - Svar.B)*900/(Svar.C - Svar.B) + 100));
				//SetTextValue(0, 32, Str("%.3f", ((mean_result - Svar.B)*(Svar.C - Svar.B)/540+60) * 10 / 6));
			}
		}
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
