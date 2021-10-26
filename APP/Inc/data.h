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
  float ADS_AMP;         //����ADS��������
  u16   ANTI_SHAKE_PHASE;//����ƫ��
  u32   OFFSET_PHASE;    //��λƫ��
  float a;               //�������ϵ��a
  float b;               //�������ϵ��b
  float c;               //�������ϵ��c
  float d;               //�������ϵ��d
  float e;               //�������ϵ��e
}SVAR;
extern SVAR Svar;

#endif
