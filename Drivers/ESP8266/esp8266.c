#include "esp8266.h"
#include "usart.h"
#include "tim.h"

ESP8266 esp8266;

#define ESP8266_Send(data,size) HAL_UART_Transmit_DMA(&ESP8266_USART, (u8*)data, size);
#define ESP8266_Str(format, data) (sprintf(transformBuf,format, data), transformBuf)
#define CWJAP      (sprintf(transformBuf,"AT+CWJAP=\"%s\",\"%s\"",wifista_ssid,wifista_password),(char*)transformBuf)
#define TRANSLINK  (sprintf(transformBuf,"AT+SAVETRANSLINK=1,\"%s\",%s,\"TCP\"",tcp_ip,tcp_port),(char*)transformBuf)
//STA 模式下的wifi 及 密码
char *wifista_ssid     = "dxxy16-402-1";
char *wifista_password = "dxxy16402";
//TCP 连接的端口号
char *tcp_ip           = "192.168.0.144";
char *tcp_port         = "7220";
//ESP8266 transform buf
static char transformBuf[52];
u8          ESP8266_Rxbuf;
//一些固定的响应
char *_OK    = "OK\r\n\r\n";
char *_ready = "ready\r\n\r\n";
char *_GETIP = "WIFI GOT IP\n";
//接收队列头和队列尾部
u16 head=0,tail=0;	//RXQUEUE 接收队列头尾
static u16 indx=0;				//RXBUF   接收缓存索引


void ESP8266_RXBUF_Clear(void)		{indx=0; esp8266.rxdata[indx] = 0;}
void ESP8266_RXBUF_PUSH(u8 data)
{
	u16 pos = indx;
	if(pos < ESP8266_RX_MAX-1)
	{
		if(*esp8266.ackbuf) //ackbuf不为空，则持续判断是否为指定字符串
		{
			if(*esp8266.ackbuf == data) //如果是指定字符串则添加入缓存
			{
				esp8266.ackbuf++;
				esp8266.rxdata[  indx] = data;
				esp8266.rxdata[++indx] = 0   ;
			}
			else		//不是预期的字符串则回溯
			{
				esp8266.ackbuf -= indx;
				esp8266.rxdata[indx=0] = 0;
			}
		}
		else//ackbuf为空后，持续接收
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
 *  \brief    ESP8266发送数据
 *  \param cmd 将要发送的数据
 *  \param ack 期望的响应
 *  \param response 响应内容存放的执行，可为 NULL (即不存下接收的数据)
 *  \param timeout  超时时间，尽量可以设置得大一些
 *  \retval 指令是否发送成功 (YES 为成功, NO 为失败)
 */
u8 ESP8266_Data_Send(char* data, char* ack, u8* response, u16 timeout)
{
	while(esp8266.state != ESP8266_IDLE)	//等待ESP8266空闲
		rt_thread_delay(1);
	esp8266.ackbuf = (u8*)ack;
	esp8266.state = ESP8266_Master_Working;	//ESP8266 更换至主动接收状态
	esp8266.tick = timeout;									//设置超时时间
	esp8266.timeout = timeout;
	head = tail;														//队列清空

	ESP8266_Send((u8*)data, strlen(data));


	while(!(esp8266.state == ESP8266_Master_Compelete || \
					esp8266.state == ESP8266_Master_Timeout)) //等待ESP8266发送完成或超时
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
 *  \brief    ESP8266发送指令
 *  \param cmd 将要发送的指令
 *  \param ack 期望的响应
 *  \param response 响应内容存放的执行，可为 NULL (即不存下接收的数据)
 *  \param timeout  超时时间，尽量可以设置得大一些
 *  \retval 指令是否发送成功 (YES 为成功, NO 为失败)
 */
u8 ESP8266_Cmd_Send(char* cmd,char* ack,u8* response, u16 timeout)
{
	//处理将要发送得内容进入 BUF 中
	return ESP8266_Data_Send(ESP8266_Str("%s\r\n",(char*)cmd),
														ack,
														response,
														timeout);
}
/*!
 *  \brief    ESP8266初始化
 */
void ESP8266_Init(void)
{
	//开启中断接收
	HAL_UART_Receive_IT(&ESP8266_USART, &ESP8266_Rxbuf, 1);

	//检查是否正常连接 ESP8266
	while(ESP8266_Cmd_Send("AT",_OK,NULL,1000) != OK)
	{
		ESP8266_Data_Send("+++", "CLOSED\r\n", NULL, 50); //退出透传
	}
	ESP8266_Cmd_Send("ATE0"              ,_OK   ,NULL,500);  //关闭回传
	ESP8266_Cmd_Send("AT+CWMODE_DEF=1"   ,_OK   ,NULL,500);  //设置为 STA 模式
	ESP8266_Cmd_Send("AT+CIPMODE=0"      ,_OK   ,NULL,1000); //关闭透传
	ESP8266_Cmd_Send("AT+SAVETRANSLINK=0",_OK   ,NULL,1000); //退出透传连接
	ESP8266_Cmd_Send("AT+RST"            ,_ready,NULL,3000); //重启使得 STA 模式生效
	ESP8266_Cmd_Send(CWJAP               ,_OK   ,NULL,4000); //连入 WIFI
	ESP8266_Cmd_Send(TRANSLINK           ,_OK   ,NULL,1000); //建立 TCP 连接
	ESP8266_Cmd_Send("AT+CIPMODE=1"      ,_OK   ,NULL,1000); //开启透传
	ESP8266_Cmd_Send("AT+RST"            ,_GETIP,NULL,3000); //重启使得透传生效
	//ESP8266_Cmd_Send("AT+CIPSEND"        ,">"   ,NULL,1000); //这句没用到

}
