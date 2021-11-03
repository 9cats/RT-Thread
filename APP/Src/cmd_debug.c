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
	//��ʾ��һ��
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

	//����������ʾ
	if (!rt_strcmp(cmd, "start"))
	{
		u32 i = 0;
		//���֮ǰ�Ĳ���ֵ
		ADS8688_COUNT = 0;
		ADS8688_STA = YES;
		//�ȴ��������
		while(ADS8688_STA) delay_ms(10);
		//���������ʾ����
		for(i=0;i<ADS8688_COUNT;i++)
		{
			OutData[0] = ADS8688_BUF[i];
			OutPut_Data();
		}
		//������һ�ν���
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

		//����������ת����ֵ������FFT����������
		for(i=0;i<FFT_NUM;i++) FFT_INPUT[i] = (float)ADS8688_BUF[i]/1500;

		arm_rfft_fast_init_f32(&fft_S, FFT_NUM);								//FFT��ʼ��
		arm_rfft_fast_f32(&fft_S,FFT_INPUT,FFT_OUTPUT,0);				//FFT
		arm_cmplx_mag_f32(FFT_OUTPUT,FFT_OUTPUT_REAL,FFT_NUM);	//��ģ

		//������ݵ�����ʾ����
		//ע�� Real_Point =  ������ / FFT�� * ���ɲ�Ƶ��
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

		//�ͷſռ�
		rt_free(FFT_INPUT);
		rt_free(FFT_OUTPUT);
		rt_free(FFT_OUTPUT_REAL);
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
  cmd[cmd_len] = 0;

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
