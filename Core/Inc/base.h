#ifndef __BASE_H__
#define __BASE_H__

//LED控制宏定义
#define LED0_T   HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin)
#define LED0_ON  HAL_GPIO_WritePin (LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET)
#define LED0_OFF HAL_GPIO_WritePin (LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET  )
#define LED1_T   HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin)
#define LED1_ON  HAL_GPIO_WritePin (LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET)
#define LED1_OFF HAL_GPIO_WritePin (LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET  )

//按键扫描相关宏定义和函数
#define KEY0 HAL_GPIO_ReadPin(KEY0_GPIO_Port,KEY0_Pin)
#define KEY1 HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)
#define KEY0_PRES 1
#define KEY1_PRES 2
uint8_t KEY_Scan(uint8_t mode);

// 自定义delay函数，兼容部分硬件库
// 默认使用滴答定时器，如果滴答定时器不可用需要用其他优先级高的定时器来代替
void delay_init(uint8_t  SYSCLK);
void delay_ms  (uint16_t nms);
void delay_us  (uint32_t nus);
void delay_ns  (uint8_t  t);


/*!
 *  \brief 数字转字符串
 *  \param format
 *  \param data  要被转换的数据
 */
#define Str(format, data) (sprintf((char*)Str_Buf,format, data), Str_Buf)

#endif
