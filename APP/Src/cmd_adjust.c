/**
 ******************************************************************************
 * @file    data_adjust.c
 * @brief   调试 或 按键触发的数据较准
 ******************************************************************************
 */
#include "main.h"
#include "rtthread.h"
#include "cmd_thread.h"
#include "cmd_process.h"

/*!
 *  \brief  数据校准 线程函数入口
 *  \param  data 需较准的数据
 */
void Data_Adjust_Entry(char *data)
{
	float* buf = rt_malloc(800*4);	//BUF分配空间
	float mean;											//平均值
	u16 i;

	if (!rt_strcmp(data, "ADS_OFFSET"))
	{
		for(i=0; i<800; i++)
		{
			buf[i] = V_recv;
			rt_thread_mdelay(1);
		}
		arm_mean_f32(buf , 800, &mean);
		Svar.ADS_OFFSET = mean;
		SetTextValue(0, 2, Str("%f",Svar.ADS_OFFSET));
	}

	else if (!rt_strcmp(data, "A"))
	{
		for(i=0; i<800; i++)
		{
			buf[i] = V_iir;
			rt_thread_mdelay(1);
		}
		arm_mean_f32(buf , 800, &mean);
		Svar.A = mean;
		SetTextValue(0, 5, Str("%f",Svar.A));
	}

	else if (!rt_strcmp(data, "B"))
	{
		for(i=0; i<800; i++)
		{
			buf[i] = V_iir;
			rt_thread_mdelay(1);
		}
		arm_mean_f32(buf , 800, &mean);
		Svar.B = mean;
		SetTextValue(0, 6, Str("%f",Svar.B));
	}

	else if (!rt_strcmp(data, "C"))
	{
		for(i=0; i<800; i++)
		{
			buf[i] = V_iir;
			rt_thread_mdelay(1);
		}
		arm_mean_f32(buf , 800, &mean);
		Svar.C = mean;
		SetTextValue(0, 7, Str("%f",Svar.C));
	}


  //如果在ccmram则释放空间
  if(IN_CCMRAM(data)) rt_free(data);
}

/*!
 *  \brief  数据校准线程创建
 *  \param  data 需较准的数据
 */
void Data_Adjust(char *argv)
{
	rt_thread_startup(
			rt_thread_create("adjustData",			//线程名
		(void (*)(void *)) Data_Adjust_Entry, //线程入口函数
											 (void *)argv,			//线程入口函数参数
											 2048,							//线程分配堆栈大小
											 17,								//线程优先级
											 1)									//线程时间片
	);
}

/*!
 *  \brief  数据校准线程创建
 *  \param  argc 传入参数个数
 *  \param  argv 传入参数名
 *  \note   例如 argv[1] 就是输入参数
 */
void adjust(int argc, char **argv)
{
  char *cmd = rt_malloc(40);
  u8 cmd_len = rt_strlen(argv[1]);
  rt_strncpy(cmd, argv[1], cmd_len);

	rt_thread_startup(
			rt_thread_create("adjustData",			//线程名
		(void (*)(void *)) Data_Adjust_Entry, //线程入口函数
											 (void *)cmd,			//线程入口函数参数
											 2048,							//线程分配堆栈大小
											 17,								//线程优先级
											 1)									//线程时间片
	);
}
MSH_CMD_EXPORT(adjust, adjust data sample: adjust<dataName>);
