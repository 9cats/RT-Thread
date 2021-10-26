#include "hmi_user_uart.h"

//���ڽ���BUF
uint8_t TFT_RX_BUF;

/*!
 *  \brief    TFT��������ʼ��
 *  \detail   ��
 */
void TFT_Init(void)
{
	HAL_UART_Receive_IT(&TFT_USART, &TFT_RX_BUF, 1);//ʹ�ܽ����ж�
}

/*!
 *  \brief    TFT���������������ض���
 *  \param    t ���͵��ַ���
 *  \detail   ��
 */
void SendChar(uint8_t t)
{
	HAL_UART_Transmit(&TFT_USART, &t, 1, 1);
}

/*!
 *  \brief    TFT���������ͻص�����
 *  \detail   ��֡β������Ϻ���ô˺���
 */
__weak void TFT_TxCallback(void)
{
	return;
}
