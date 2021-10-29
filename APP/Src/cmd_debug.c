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
  if (!rt_strcmp(cmd, "show"))
  {
  	u16 i;
    if (ADS8688_STA == NO)
    {
      for (i = 0; i < ADS8688_COUNT; i++)
      {
        OutData[0] = ADS8688_BUF[i];
        OutData[1] = ADS8688_BUF1[i] * ScaleValue;
        OutPut_Data();
      }
      ADS8688_COUNT = 0;
      ADS8688_STA = YES;
    }
  }

  if (!rt_strcmp(cmd, "da"))
  {
  	AD9959_Config_Freq(0, freq);
  	AD9959_Config_Amp (0, amp);
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
