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

//-------------------ADS8688接收和发送BUF-----------------
u8  rxbuf [4]    = {0};
u8  txbuf [4]    = {0};

//采样与滤波
float V_recv;					//ADS8688接收值
float V_iir_noScale ;	//IIR滤波后的结果(无放缩)
//float V_mean;					//ADS8688
//float V_fir ;					//FIR滤波后的结果
//--------------------------------------------------------

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM6) HAL_IncTick(); // RT-Thread 系统时钟

	else if(htim == &htim2) {
		V_recv = *(u16*)(rxbuf+2);

		arm_biquad_cascade_df1_f32(&iir_S, &V_recv, &V_iir_noScale, 1);
		//数据收集
		if(ADS8688_STA == YES)
		{
			if(ADS8688_COUNT < 5000) {
				ADS8688_BUF [ADS8688_COUNT] = V_recv;
				ADS8688_BUF1[ADS8688_COUNT] = V_iir_noScale;
				ADS8688_COUNT++;
			}
			else
				ADS8688_STA = NO;
		}

		HAL_SPI_TransmitReceive_DMA(&hspi3, txbuf, rxbuf, 2);
	}
	//输出高电平
	else if(htim == &htim10){
		HAL_GPIO_WritePin(LOCK_PHASE_OUT_GPIO_Port, LOCK_PHASE_OUT_Pin, GPIO_PIN_SET);
	}
	//输出低电平
	else if(htim == &htim11){
		HAL_GPIO_WritePin(LOCK_PHASE_OUT_GPIO_Port, LOCK_PHASE_OUT_Pin, GPIO_PIN_RESET);
	}
	//延时输出
	else if(htim == &htim13){
		__HAL_TIM_SET_AUTORELOAD(&htim10, Svar.OFFSET_PHASE);
		__HAL_TIM_CLEAR_IT(&htim8,TIM_IT_CC3);
		__HAL_TIM_CLEAR_IT(&htim8,TIM_IT_CC4);
		__HAL_TIM_ENABLE_IT(&htim8, TIM_IT_CC3);
		__HAL_TIM_ENABLE_IT(&htim8, TIM_IT_CC4);
		__HAL_TIM_ENABLE(&htim10);
	}
	//延时输出
	else if(htim == &htim14){
		__HAL_TIM_SET_AUTORELOAD(&htim11, Svar.OFFSET_PHASE);
		__HAL_TIM_CLEAR_IT(&htim8,TIM_IT_CC3);
		__HAL_TIM_CLEAR_IT(&htim8,TIM_IT_CC4);
		__HAL_TIM_ENABLE_IT(&htim8, TIM_IT_CC3);
		__HAL_TIM_ENABLE_IT(&htim8, TIM_IT_CC4);
		__HAL_TIM_ENABLE(&htim11);
	}
}


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if (htim == &htim8) {

		//捕获到上升沿
		if			(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3){
			__HAL_TIM_DISABLE_IT(&htim8, TIM_IT_CC3);
			__HAL_TIM_SET_AUTORELOAD(&htim13, Svar.ANTI_SHAKE_PHASE);
			__HAL_TIM_ENABLE(&htim13);
		}

		//捕获到下降沿
		else if	(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4){
			__HAL_TIM_DISABLE_IT(&htim8, TIM_IT_CC4);
			__HAL_TIM_SET_AUTORELOAD(&htim14, Svar.ANTI_SHAKE_PHASE);
			__HAL_TIM_ENABLE(&htim14);
		}
	}
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
}

