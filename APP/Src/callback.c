/**
  ******************************************************************************
  * @file    callback.c
  * @brief   回调函数
  ******************************************************************************
  */
#include "main.h"
#include "spi.h"
#include "tim.h"
#include "hmi_user_uart.h"
#include "cmd_queue.h"
#include "cmd_process.h"
#include "arm_math.h"
#include "esp8266.h"
#include "ADS8688.h"

//-------------------ADS8688接收和发送BUF-----------------
u8  ADS8688_RxBuf [4]    = {0};
u8  ADS8688_TxBuf [4]    = {0};
u8  ADS8688_Busy         = NO;

//采样与滤波
float V_recv;					//ADS8688接收值
float V_iir_noScale ;	//IIR滤波后的结果(无放缩)
//float V_mean;					//ADS8688
float V_fir ;					//FIR滤波后的结果
//--------------------------------------------------------

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	extern TIM_HandleTypeDef htim7;
	if (htim == &htim7) HAL_IncTick(); // RT-Thread 系统时钟
	if (htim == &htim14) {
		V_recv = *(u16*)(ADS8688_RxBuf+2);				//转换上次的采样值
		ADS8688_Start_DMA(ADS8688_RxBuf);	//ADS8688开启DMA采样

		arm_biquad_cascade_df1_f32(&iir_S, &V_recv, &V_iir_noScale, 1); // IIR实时滤波
		//arm_fir_f32(&fir, &V_recv, &V_fir, 1);                          // FIR实时滤波
		if(ADS8688_STA)
		{
			ADS8688_BUF [ADS8688_COUNT] = *(u16*)(ADS8688_RxBuf+2)/2;
			ADS8688_BUF1[ADS8688_COUNT] = V_iir_noScale; //IIR-BUF
			//ADS8688_BUF1[ADS8688_COUNT] = V_fir; //FIR-BUF
			if(++ADS8688_COUNT == 5000) ADS8688_STA = NO;
		}
	};
}


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
}



void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	//TFT驱动依赖
	if (huart == &TFT_USART)
	{
		//处理接收队列
		queue_push(TFT_RX_BUF);
		if(queue_find_cmd(cmd_buffer,CMD_MAX_SIZE))
			ProcessMessage((PCTRL_MSG)cmd_buffer,0);
		//开启下一次接收
		HAL_UART_Receive_IT(&TFT_USART, &TFT_RX_BUF, 1);
	}

	if (huart == &ESP8266_USART)
	{
		/* ESP8266 <空闲状态> 接收到字符串时进入 <从机接收状态> */
		if(esp8266.state == ESP8266_IDLE)
		{
			esp8266.state = ESP8266_Slave_Working;
			esp8266.tick  = esp8266.timeout;
			ESP8266_RXQUEUE_Clear();							//清空接收队列
			ESP8266_RXQUEUE_PUSH(ESP8266_Rxbuf);
		}

		/* ESP8266 <从机接收状态> 接收到数据后，将数据放入接收缓存 */
		else if(esp8266.state == ESP8266_Slave_Working){
			esp8266.tick  = esp8266.timeout;
			ESP8266_RXQUEUE_PUSH(ESP8266_Rxbuf);
		}

		/* ESP8266 <主机工作状态> 接收到数据后进入 <主机接收状态>*/
		else if(esp8266.state == ESP8266_Master_Working)
		{
			esp8266.state = ESP8266_Master_Receiving;
			esp8266.tick  = esp8266.timeout;
			ESP8266_RXBUF_Clear();							//清空接收队列
			ESP8266_RXBUF_PUSH(ESP8266_Rxbuf);
		}

		/* ESP8266 <从机接收状态> 接收到数据后，将数据放入接收缓存 */
		else if(esp8266.state == ESP8266_Master_Receiving)
		{
			esp8266.tick  = esp8266.timeout;
			ESP8266_RXBUF_PUSH(ESP8266_Rxbuf);
		}

		HAL_UART_Receive_IT(&ESP8266_USART, &ESP8266_Rxbuf, 1); //开启下一次接收
	}
}

