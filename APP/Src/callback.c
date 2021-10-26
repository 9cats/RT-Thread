#include "main.h"
#include "spi.h"
#include "tim.h"

//-------------------ADS8688接收和发送BUF-----------------
u8  rxbuf [4]    = {0};
u8  txbuf [4]    = {0};
//--------------------------------------------------------

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM6) HAL_IncTick(); // RT-Thread 系统时钟

	else if(htim == &htim2) {
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
		__HAL_TIM_SET_AUTORELOAD(&htim10, Svar.ANTI_SHAKE_PHASE);
		__HAL_TIM_CLEAR_IT(&htim8,TIM_IT_CC3);
		__HAL_TIM_ENABLE_IT(&htim8, TIM_IT_CC3);
		__HAL_TIM_ENABLE(&htim10);
	}
	//延时输出
	else if(htim == &htim14){
		__HAL_TIM_SET_AUTORELOAD(&htim11, Svar.ANTI_SHAKE_PHASE);
		__HAL_TIM_CLEAR_IT(&htim8,TIM_IT_CC4);
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
