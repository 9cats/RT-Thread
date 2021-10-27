#include "main.h"
#include "output.h"
#include "rtthread.h"

extern float ScaleValue;

void Digital_Scope_Show()
{
	u16 i;
	if(ADS8688_STA == NO)
	{
		for(i=0;i<ADS8688_COUNT;i++)
		{
			OutData[0] = ADS8688_BUF [i];
			OutData[1] = ADS8688_BUF1[i] * ScaleValue;
			OutPut_Data();
		}
		ADS8688_COUNT = 0;
		ADS8688_STA = YES;
	}
}

void Show()
{
	static rt_thread_t tid = RT_NULL; //调试输出到虚拟示波器
	tid = rt_thread_create("波形显示",
													Digital_Scope_Show,
													RT_NULL,
													2048,
													23,
													10);
	rt_thread_startup(tid);
}


MSH_CMD_EXPORT(Show, console to digital scope);
