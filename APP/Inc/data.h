#ifndef __DATA_H__
#define __DATA_H__
#include "rtthread.h"
#include "main.h"

//RT-Thread ���
extern rt_sem_t LED0Toggle_Sem; //LED0��ת�ź���
extern rt_sem_t LED1Toggle_Sem; //LED1��ת�ź���


#pragma pack(1) //��1�ֽڶ���
typedef struct _SVAR
{
  float ADS_AMP;
  u16   ADS_OFFSET_ALL;
}SVAR;

extern SVAR Svar;

#endif
