#ifndef __DATA_H__
#define __DATA_H__
#include "main.h"

typedef struct _SVAR
{
  float ADS_AMP;        //所有ADS倍数补偿
  u16   ADS_OFFSET_ALL; //所有ADS的偏置补偿
}SVAR;
extern SVAR Svar;

#endif
