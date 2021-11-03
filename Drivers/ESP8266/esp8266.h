#ifndef __ESP8266_H_
#define __ESP8266_H_
#include "main.h"

#define ESP8266_IDLE							0//������
#define ESP8266_Master_Working		1//����������
#define ESP8266_Master_Receiving	2//����������
#define ESP8266_Master_Compelete	3//�����������
#define ESP8266_Master_Timeout		4//����������ʱ
#define ESP8266_Slave_Working			5//�ӻ�������
#define ESP8266_Slave_Compelete		6//�ӻ�������

#define ESP8266_USART huart6

#define ESP8266_RX_MAX 256
typedef struct _ESP8266
{
  u8  mode;											//��ǰ����ģʽ 0-δ֪ 1-STA 2-AP 3-STA+AP
  u8  state;										//��ǰ״̬     0-���� 1-���������� 2-����������
  u8* ackbuf;										//Ԥ�ڵ���Ӧ����
  u16 tick;											//��¼��ʱ���߳�������
  u16 timeout;									//��¼��ʱ���߼��ʱ��
  u8  rxdata [ESP8266_RX_MAX];	//��Ӧ����
  u8  rxqueue [ESP8266_RX_MAX];	//���ն���
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
