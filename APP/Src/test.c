#include "rtthread.h"
#include "usart.h"


/* 线 程 示 例 */
void msh_out(int argc, char *argv[])
{
	HAL_UART_Transmit(&huart1, (u8*)argv[1], sizeof(argv[1]), 10);
}
/* 导 出 到 msh 命 令 列 表 中 */
MSH_CMD_EXPORT(msh_out, out);
