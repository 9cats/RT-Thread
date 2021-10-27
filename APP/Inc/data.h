#ifndef __DATA_H__
#define __DATA_H__
#include "rtthread.h"
#include "main.h"
#include "arm_math.h"
//IIR滤波
extern arm_biquad_casd_df1_inst_f32 iir_S;
extern float32_t ScaleValue;
//FIR滤波
extern arm_fir_instance_f32 fir_S;
//采样与滤波
extern float V_recv;					//ADS8688接收值
extern float V_iir_noScale ;	//实时IIR滤波的值
#define V_real (V_recv                   - Svar.ADS_OFFSET) * 5.12 / 0xffff * Svar.ADS_AMP
#define V_iir  (V_iir_noScale*ScaleValue - Svar.ADS_OFFSET) * 5.12 / 0xffff * Svar.ADS_AMP

//RT-Thread 相关
extern rt_sem_t LED0Toggle_Sem; //LED0翻转信号量
extern rt_sem_t LED1Toggle_Sem; //LED1翻转信号量



//ADS8688
extern u8  ADS8688_STA  ;
extern u32 ADS8688_COUNT;
extern u16 ADS8688_BUF[];
extern u16 ADS8688_BUF1[];




#pragma pack(1) //按1字节对齐
typedef struct _SVAR
{
  float ADS_AMP;         //所有ADS倍数补偿
  float ADS_OFFSET;      //所有ADS偏置补偿
  u16   ANTI_SHAKE_PHASE;//消抖偏移
  u32   OFFSET_PHASE;    //相位偏移
  float A;               //线性拟合系数A
  float B;               //线性拟合系数B
  float C;               //线性拟合系数C
}SVAR;
extern SVAR Svar;

#endif
