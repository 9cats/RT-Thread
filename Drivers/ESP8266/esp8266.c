#include "esp8266.h"
#include "usart.h"
#include "tim.h"

ESP8266 esp8266;

#define ESP8266_Send(data,size) HAL_UART_Transmit_DMA(&ESP8266_USART, (u8*)data, size);
#define ESP8266_Str(format, data) (sprintf(transformBuf,format, data), transformBuf)
#define CWJAP      (sprintf(transformBuf,"AT+CWJAP=\"%s\",\"%s\"",wifista_ssid,wifista_password),(char*)transformBuf)
#define TRANSLINK  (sprintf(transformBuf,"AT+SAVETRANSLINK=1,\"%s\",%s,\"TCP\"",tcp_ip,tcp_port),(char*)transformBuf)
//STA ģʽ�µ�wifi �� ����
char *wifista_ssid     = "dxxy16-402-1";
char *wifista_password = "dxxy16402";
//TCP ���ӵĶ˿ں�
char *tcp_ip           = "192.168.0.144";
char *tcp_port         = "7220";
//ESP8266 transform buf
static char transformBuf[52];
u8          ESP8266_Rxbuf;
//һЩ�̶�����Ӧ
char *_OK    = "OK\r\n\r\n";
char *_ready = "ready\r\n\r\n";
char *_GETIP = "WIFI GOT IP\n";
//���ն���ͷ�Ͷ���β��
u16 head=0,tail=0;	//RXQUEUE ���ն���ͷβ
static u16 indx=0;				//RXBUF   ���ջ�������


void ESP8266_RXBUF_Clear(void)		{indx=0; esp8266.rxdata[indx] = 0;}
void ESP8266_RXBUF_PUSH(u8 data)
{
	u16 pos = indx;
	if(pos < ESP8266_RX_MAX-1)
	{
		if(*esp8266.ackbuf) //ackbuf��Ϊ�գ�������ж��Ƿ�Ϊָ���ַ���
		{
			if(*esp8266.ackbuf == data) //�����ָ���ַ���������뻺��
			{
				esp8266.ackbuf++;
				esp8266.rxdata[  indx] = data;
				esp8266.rxdata[++indx] = 0   ;
			}
			else		//����Ԥ�ڵ��ַ��������
			{
				esp8266.ackbuf -= indx;
				esp8266.rxdata[indx=0] = 0;
			}
		}
		else//ackbufΪ�պ󣬳�������
		{
			esp8266.rxdata[  indx] = data;
			esp8266.rxdata[++indx] = 0   ;
		}
	}
}
void ESP8266_RXQUEUE_Clear(void)	{head = tail;}
void ESP8266_RXQUEUE_PUSH(u8 data)
{
	u16 pos = (head+1)%ESP8266_RX_MAX;
	if(pos!=tail)
	{
		esp8266.rxqueue[head] = data;
		head = pos;
	}
	else{
		esp8266.rxqueue[head] = data;
		head = (head+1)%ESP8266_RX_MAX;
		tail = (tail+1)%ESP8266_RX_MAX;
	}
}
void ESP8266_RXQUEUE_POP(u8* data)
{
	if(tail != head)
	{
		*data = esp8266.rxqueue[tail];
		tail  = (tail+1)%ESP8266_RX_MAX;
	}
}

void ESP8266_Slave_Deal(void)
{

}


/*!
 *  \brief    ESP8266��������
 *  \param cmd ��Ҫ���͵�����
 *  \param ack ��������Ӧ
 *  \param response ��Ӧ���ݴ�ŵ�ִ�У���Ϊ NULL (�������½��յ�����)
 *  \param timeout  ��ʱʱ�䣬�����������õô�һЩ
 *  \retval ָ���Ƿ��ͳɹ� (YES Ϊ�ɹ�, NO Ϊʧ��)
 */
u8 ESP8266_Data_Send(char* data, char* ack, u8* response, u16 timeout)
{
	while(esp8266.state != ESP8266_IDLE)	//�ȴ�ESP8266����
		rt_thread_delay(1);
	esp8266.ackbuf = (u8*)ack;
	esp8266.state = ESP8266_Master_Working;	//ESP8266 ��������������״̬
	esp8266.tick = timeout;									//���ó�ʱʱ��
	esp8266.timeout = timeout;
	head = tail;														//�������

	ESP8266_Send((u8*)data, strlen(data));


	while(!(esp8266.state == ESP8266_Master_Compelete || \
					esp8266.state == ESP8266_Master_Timeout)) //�ȴ�ESP8266������ɻ�ʱ
		rt_thread_delay(1);

	//TODO: DEAL
	//Deal();

	if(esp8266.state == ESP8266_Master_Compelete)
	{
		esp8266.state = ESP8266_IDLE;
		return OK;
	}
	else
	{
		esp8266.state = ESP8266_IDLE;
		return TIMEOUT;
	}
}
/*!
 *  \brief    ESP8266����ָ��
 *  \param cmd ��Ҫ���͵�ָ��
 *  \param ack ��������Ӧ
 *  \param response ��Ӧ���ݴ�ŵ�ִ�У���Ϊ NULL (�������½��յ�����)
 *  \param timeout  ��ʱʱ�䣬�����������õô�һЩ
 *  \retval ָ���Ƿ��ͳɹ� (YES Ϊ�ɹ�, NO Ϊʧ��)
 */
u8 ESP8266_Cmd_Send(char* cmd,char* ack,u8* response, u16 timeout)
{
	//����Ҫ���͵����ݽ��� BUF ��
	return ESP8266_Data_Send(ESP8266_Str("%s\r\n",(char*)cmd),
														ack,
														response,
														timeout);
}
/*!
 *  \brief    ESP8266��ʼ��
 */
void ESP8266_Init(void)
{
	//�����жϽ���
	HAL_UART_Receive_IT(&ESP8266_USART, &ESP8266_Rxbuf, 1);

	//����Ƿ��������� ESP8266
	while(ESP8266_Cmd_Send("AT",_OK,NULL,1000) != OK)
	{
		ESP8266_Data_Send("+++", "CLOSED\r\n", NULL, 50); //�˳�͸��
	}
	ESP8266_Cmd_Send("ATE0"              ,_OK   ,NULL,500);  //�رջش�
	ESP8266_Cmd_Send("AT+CWMODE_DEF=1"   ,_OK   ,NULL,500);  //����Ϊ STA ģʽ
	ESP8266_Cmd_Send("AT+CIPMODE=0"      ,_OK   ,NULL,1000); //�ر�͸��
	ESP8266_Cmd_Send("AT+SAVETRANSLINK=0",_OK   ,NULL,1000); //�˳�͸������
	ESP8266_Cmd_Send("AT+RST"            ,_ready,NULL,3000); //����ʹ�� STA ģʽ��Ч
	ESP8266_Cmd_Send(CWJAP               ,_OK   ,NULL,4000); //���� WIFI
	ESP8266_Cmd_Send(TRANSLINK           ,_OK   ,NULL,1000); //���� TCP ����
	ESP8266_Cmd_Send("AT+CIPMODE=1"      ,_OK   ,NULL,1000); //����͸��
	ESP8266_Cmd_Send("AT+RST"            ,_GETIP,NULL,3000); //����ʹ��͸����Ч
	//ESP8266_Cmd_Send("AT+CIPSEND"        ,">"   ,NULL,1000); //���û�õ�

}
