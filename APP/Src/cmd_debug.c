/**
 ******************************************************************************
 * @file    debug.c
 * @brief   ���� �� ���������ĵ��Թ���
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
 *  \brief  ����У׼ �̺߳������
 *  \param  data ���׼������
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

  //�����ccmram���ͷſռ�
  if(IN_CCMRAM(cmd)) rt_free(cmd);
}

/*!
 *  \brief  ����У׼ �̺߳������
 *  \param  data ���׼������
 */
void Debug(char *data)
{
  rt_thread_startup(
      rt_thread_create("debug",     //�߳���
    (void (*)(void *)) Debug_Entry, //�߳���ں���
                       (void *)data, //�߳���ں�������
                       2048,        //�̷߳����ջ��С
                       23,          //�߳����ȼ�
                       1)           //�߳�ʱ��Ƭ
  );
}

/*!
 *  \brief  Debug �̺߳������
 *  \param  argc �����������
 *  \param  argv ���������
 *  \note   ���� argv[1] �����������
 */
void debug(int argc, char **argv)
{
  char *cmd = rt_malloc(40);
  u8 cmd_len = rt_strlen(argv[1]);
  rt_strncpy(cmd, argv[1], cmd_len);

  rt_thread_startup(
      rt_thread_create("debug",         //�߳���
    (void (*)(void *)) Debug_Entry,     //�߳���ں���
                       (void *)cmd, //�߳���ں�������
                       2048,            //�̷߳����ջ��С
                       23,              //�߳����ȼ�
                       1)               //�߳�ʱ��Ƭ
  );
}
MSH_CMD_EXPORT(debug, debug sample: debug<argv>);
