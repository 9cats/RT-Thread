#ifndef _USER_UART__
#define _USER_UART__

#include "main.h"
#include "usart.h"

//�ض�����õ�����������
#define uchar    unsigned char
#define uint8    unsigned char
#define uint16   unsigned short int
#define uint32   unsigned long
#define int16    short int
#define int32    long

//�û��Զ����޸�
#define TFT_USART huart1           /*!< */
#define CRC16_ENABLE     0         /*!< �����ҪCRC16У�鹦�ܣ��޸Ĵ˺�Ϊ1(��ʱ��Ҫ��VisualTFT��������CRCУ��)*/
#define CMD_MAX_SIZE     25        /*!<����ָ���С��������Ҫ�������������ô�һЩ*/
#define QUEUE_MAX_SIZE   128       /*!< ָ����ջ�������С��������Ҫ�������������ô�һЩ*/

//һ���ֽڵĽ���BUF
extern uint8_t TFT_RX_BUF;

//�û���Ҫʹ�õĺ�������
void TFT_Init(void);       //TFT��ʼ��
void SendChar(uint8_t t);          //TFT���ݷ����ض���
void TFT_TxCallback(void);         //TFT���ݷ������֮��Ļص�����
#endif
