#ifndef __ESP8266_H_
#define __ESP8266_H_
#include "main.h"

#define ESP8266_IDLE							0//空闲中
#define ESP8266_Master_Working		1//主动工作中
#define ESP8266_Master_Receiving	2//主动工作中
#define ESP8266_Master_Compelete	3//主动工作完成
#define ESP8266_Master_Timeout		4//主动工作超时
#define ESP8266_Slave_Working			5//从机工作中
#define ESP8266_Slave_Compelete		6//从机工作中

#define ESP8266_USART huart6

#define ESP8266_RX_MAX 256
typedef struct _ESP8266
{
  u8  mode;											//当前工作模式 0-未知 1-STA 2-AP 3-STA+AP
  u8  state;										//当前状态     0-空闲 1-主动工作中 2-被动工作中
  u8* ackbuf;										//预期的响应缓存
  u16 tick;											//记录超时或者持续工作
  u16 timeout;									//记录超时或者间隔时长
  u8  rxdata [ESP8266_RX_MAX];	//响应缓存
  u8  rxqueue [ESP8266_RX_MAX];	//接收队列
}ESP8266;
extern ESP8266 esp8266;
extern u8      ESP8266_Rxbuf;
void ESP8266_RXBUF_Clear(void);
void ESP8266_RXBUF_PUSH(u8 data);
void ESP8266_RXQUEUE_Clear(void);
void ESP8266_RXQUEUE_PUSH(u8 data);
void ESP8266_RXQUEUE_POP(u8* data);
void ESP8266_Slave_Deal(void);
u8 ESP8266_Cmd_Send(char* cmd, char* ack, u8* response ,u16 timeout);
void ESP8266_Init(void);
#endif
