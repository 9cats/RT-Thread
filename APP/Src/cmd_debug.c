/**
 ******************************************************************************
 * @file    debug.c
 * @brief   调试 或 按键触发的调试功能
 ******************************************************************************
 */
#include "main.h"
#include "rtthread.h"
#include "cmd_thread.h"
#include "cmd_process.h"
#include "output.h"
#include "AD9959.h"

float freq = 1000;
float amp  = 400;
/*!
 *  \brief  数据校准 线程函数入口
 *  \param  data 需较准的数据
 */
void Debug_Entry(char *cmd)
{
	//显示第一次
	if (!rt_strcmp(cmd, "show"))
	{
		u32 i = 0;
		for(i=0;i<ADS8688_COUNT;i++)
		{

			OutData[0] = ADS8688_BUF[i];
			OutPut_Data();
		}
		//ADS8688_COUNT = 0;
		//ADS8688_STA = YES;
	}

	//开启采样显示
	if (!rt_strcmp(cmd, "start"))
	{
		u32 i = 0;
		//清空之前的采样值
		ADS8688_COUNT = 0;
		ADS8688_STA = YES;
		//等待采样完毕
		while(ADS8688_STA) delay_ms(10);
		//输出到虚拟示波器
		for(i=0;i<ADS8688_COUNT;i++)
		{
			OutData[0] = ADS8688_BUF[i];
			OutPut_Data();
		}
		//开启下一次接收
		//ADS8688_COUNT = 0;
		//ADS8688_STA = YES;
	}

	if (!rt_strcmp(cmd, "fft"))
	{
		arm_rfft_fast_instance_f32 fft_S;
		u32 i = 0;
		#define FFT_NUM 2048

		float* FFT_INPUT       = rt_malloc(FFT_NUM*4);
		float* FFT_OUTPUT      = rt_malloc(FFT_NUM*4);
		float* FFT_OUTPUT_REAL = rt_malloc(FFT_NUM*4);

		//将采样数据转换数值，放入FFT输入数组内
		for(i=0;i<FFT_NUM;i++) FFT_INPUT[i] = (float)ADS8688_BUF[i]/1500;

		arm_rfft_fast_init_f32(&fft_S, FFT_NUM);								//FFT初始化
		arm_rfft_fast_f32(&fft_S,FFT_INPUT,FFT_OUTPUT,0);				//FFT
		arm_cmplx_mag_f32(FFT_OUTPUT,FFT_OUTPUT_REAL,FFT_NUM);	//求模

		//输出数据到虚拟示波器
		//注： Real_Point =  采样率 / FFT数 * 待采波频率
		for(i=0;i<FFT_NUM;i++)
		{
			OutData[0] = ADS8688_BUF[i];
			OutData[1] = FFT_INPUT[i];
			OutData[2] = FFT_OUTPUT[i];
			if(i < FFT_NUM/2)
			OutData[3] = FFT_OUTPUT_REAL[i];
			else
			OutData[3] = 0x7fff;
			OutPut_Data();
		}

		//释放空间
		rt_free(FFT_INPUT);
		rt_free(FFT_OUTPUT);
		rt_free(FFT_OUTPUT_REAL);
	}



  //如果在ccmram则释放空间
  if(IN_CCMRAM(cmd)) rt_free(cmd);
}

/*!
 *  \brief  数据校准 线程函数入口
 *  \param  data 需较准的数据
 */
void Debug(char *data)
{
  rt_thread_startup(
      rt_thread_create("debug",     //线程名
    (void (*)(void *)) Debug_Entry, //线程入口函数
                       (void *)data, //线程入口函数参数
                       2048,        //线程分配堆栈大小
                       23,          //线程优先级
                       1)           //线程时间片
  );
}

/*!
 *  \brief  Debug 线程函数入口
 *  \param  argc 传入参数个数
 *  \param  argv 传入参数名
 *  \note   例如 argv[1] 就是输入参数
 */
void debug(int argc, char **argv)
{
  char *cmd = rt_malloc(40);
  u8 cmd_len = rt_strlen(argv[1]);
  rt_strncpy(cmd, argv[1], cmd_len);
  cmd[cmd_len] = 0;

  rt_thread_startup(
      rt_thread_create("debug",         //线程名
    (void (*)(void *)) Debug_Entry,     //线程入口函数
                       (void *)cmd, //线程入口函数参数
                       2048,            //线程分配堆栈大小
                       23,              //线程优先级
                       1)               //线程时间片
  );
}
MSH_CMD_EXPORT(debug, debug sample: debug<argv>);
