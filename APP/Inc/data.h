#ifndef __DATA_H__
#define __DATA_H__
#include "rtthread.h"
#include "main.h"

//RT-Thread 相关
extern rt_sem_t LED0Toggle_Sem; //LED0翻转信号量
extern rt_sem_t LED1Toggle_Sem; //LED1翻转信号量


#pragma pack(1) //按1字节对齐
typedef struct _SVAR
{
  float ADS_AMP;         //所有ADS倍数补偿
  u16   ANTI_SHAKE_PHASE;//消抖偏移
  u32   OFFSET_PHASE;    //相位偏移
  float a;               //线性拟合系数a
  float b;               //线性拟合系数b
  float c;               //线性拟合系数c
  float d;               //线性拟合系数d
  float e;               //线性拟合系数e
}SVAR;
extern SVAR Svar;

#endif
