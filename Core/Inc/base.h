#ifndef __BASE_H__
#define __BASE_H__

//LED���ƺ궨��
#define LED0_T   HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin)
#define LED0_ON  HAL_GPIO_WritePin (LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET)
#define LED0_OFF HAL_GPIO_WritePin (LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET  )
#define LED1_T   HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin)
#define LED1_ON  HAL_GPIO_WritePin (LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET)
#define LED1_OFF HAL_GPIO_WritePin (LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET  )

//����ɨ����غ궨��ͺ���
#define KEY0 HAL_GPIO_ReadPin(KEY0_GPIO_Port,KEY0_Pin)
#define KEY1 HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)
#define KEY0_PRES 1
#define KEY1_PRES 2
uint8_t KEY_Scan(uint8_t mode);

// �Զ���delay���������ݲ���Ӳ����
// Ĭ��ʹ�õδ�ʱ��������δ�ʱ����������Ҫ���������ȼ��ߵĶ�ʱ��������
void delay_init(uint8_t  SYSCLK);
void delay_ms  (uint16_t nms);
void delay_us  (uint32_t nus);
void delay_ns  (uint8_t  t);


/*!
 *  \brief ����ת�ַ���
 *  \param format
 *  \param data  Ҫ��ת��������
 */
#define Str(format, data) (sprintf((char*)Str_Buf,format, data), Str_Buf)

#endif
