#include "main.h"
#include "rtthread.h"
#include "cmd_process.h"


void Data_Adjust_Entry(void *arguement)
{
	u16* param = arguement;
	u16 screen_id,control_id;

	screen_id  = param[0];
	control_id = param[1];

	u16 i;
	float* buf = rt_malloc(800*4);
	float mean;

	if(screen_id == 0 && control_id == 51)
	{
		//TODO: 杂糅！
		//6rmv 对应 10uv 的较准
		for(i=0; i<800; i++)
		{
			buf[i] = V_iir;
			rt_thread_mdelay(1);
		}
		arm_mean_f32(buf , 800, &mean);
		Svar.A = mean;
		SetTextValue(0, 5, Str("%f",Svar.A));
	}
	if(screen_id == 0 && control_id == 52)
	{
		//6rmv 对应 10uv 的较准
		for(i=0; i<800; i++)
		{
			buf[i] = V_iir;
			rt_thread_mdelay(1);
		}
		arm_mean_f32(buf , 800, &mean);
		Svar.B = mean;
		SetTextValue(0, 6, Str("%f",Svar.B));
	}
	if(screen_id == 0 && control_id == 53)
	{
		//6rmv 对应 10uv 的较准
		for(i=0; i<800; i++)
		{
			buf[i] = V_iir;
			rt_thread_mdelay(1);
		}
		arm_mean_f32(buf , 800, &mean);
		Svar.C = mean;
		SetTextValue(0, 7, Str("%f",Svar.C));
	}
	//ADS偏置补偿
	if(screen_id == 0 && control_id == 54)
	{
		//求平均值(rxbuf) 即接收数据
		for(i=0; i<800; i++)
		{
			buf[i] = V_recv;
			rt_thread_mdelay(1);
		}
		arm_mean_f32(buf , 800, &mean);
		Svar.ADS_OFFSET = mean;
		SetTextValue(0, 2, Str("%f",Svar.ADS_OFFSET));
	}


	rt_free(buf);
}


void Cteate_Data_Adjust_Thread(u16 screen_id, u16 control_id)
{
	u16 param[2] = {screen_id, control_id};

	rt_thread_startup(
			rt_thread_create("较准",										//线程名
												Data_Adjust_Entry,					//线程入口函数
												(void*)param,								//线程入口函数参数
												2048,												//线程分配堆栈大小
												17,													//线程优先级
												1)													//线程时间片
	);
}
