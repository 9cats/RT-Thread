#include "rtthread.h"
#include "usart.h"


/* �� �� ʾ �� */
void msh_out(int argc, char *argv[])
{
	HAL_UART_Transmit(&huart1, (u8*)argv[1], sizeof(argv[1]), 10);
}
/* �� �� �� msh �� �� �� �� �� */
MSH_CMD_EXPORT(msh_out, out);
