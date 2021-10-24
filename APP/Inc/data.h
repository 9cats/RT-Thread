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
  float ADS_AMP;
  u16   ADS_OFFSET_ALL;
}SVAR;

extern SVAR Svar;

#endif
