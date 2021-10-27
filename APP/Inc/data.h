#ifndef __DATA_H__
#define __DATA_H__
#include "rtthread.h"
#include "main.h"
#include "arm_math.h"
//IIR�˲�
extern arm_biquad_casd_df1_inst_f32 iir_S;
extern float32_t ScaleValue;
//FIR�˲�
extern arm_fir_instance_f32 fir_S;
//�������˲�
extern float V_recv;					//ADS8688����ֵ
extern float V_iir_noScale ;	//ʵʱIIR�˲���ֵ
#define V_real (V_recv                   - Svar.ADS_OFFSET) * 5.12 / 0xffff * Svar.ADS_AMP
#define V_iir  (V_iir_noScale*ScaleValue - Svar.ADS_OFFSET) * 5.12 / 0xffff * Svar.ADS_AMP

//RT-Thread ���
extern rt_sem_t LED0Toggle_Sem; //LED0��ת�ź���
extern rt_sem_t LED1Toggle_Sem; //LED1��ת�ź���



//ADS8688
extern u8  ADS8688_STA  ;
extern u32 ADS8688_COUNT;
extern u16 ADS8688_BUF[];
extern u16 ADS8688_BUF1[];




#pragma pack(1) //��1�ֽڶ���
typedef struct _SVAR
{
  float ADS_AMP;         //����ADS��������
  float ADS_OFFSET;      //����ADSƫ�ò���
  u16   ANTI_SHAKE_PHASE;//����ƫ��
  u32   OFFSET_PHASE;    //��λƫ��
  float A;               //�������ϵ��A
  float B;               //�������ϵ��B
  float C;               //�������ϵ��C
}SVAR;
extern SVAR Svar;

#endif
