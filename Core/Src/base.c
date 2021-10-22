#include "main.h"
#include "base.h"
#include "AT24CXX.h"

//us延时倍乘数
static u8 fac_us = 0;
//sprintf转换BUF
u8 Str_Buf[200]= {0};


/*!
 *  \brief  初始化延迟函数
 *  \param  SYSCLK   系统时钟频率（如72Mhz，填写72）
 *  \note   SYSTICK的时钟固定为HCLK时钟的1/8
 */
void delay_init(u8 SYSCLK) {
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK); //SysTick频率为HCLK
	fac_us = SYSCLK;
}


/*!
 *  \brief  延迟ns
 *  \param  t ns数
 *  \note   delay_init后使用
 */
void delay_ns(u8 t) {
	do {
		;
	} while (--t);
}


/*!
 *  \brief  延迟us
 *  \param  t us数
 *  \note   delay_init后使用
 */
void delay_us(u32 nus) {
	u32 ticks;
	u32 told, tnow, tcnt = 0;
	u32 reload = SysTick->LOAD; //LOAD的值
	ticks = nus * fac_us;       //需要的节拍数
	told = SysTick->VAL;        //刚进入时的计数器值
	while (1) {
		tnow = SysTick->VAL;
		if (tnow != told) {
			if (tnow < told)
				tcnt += told - tnow; //这里注意一下SYSTICK是一个递减的计数器就可以了.
			else
				tcnt += reload - tnow + told;
			told = tnow;
			if (tcnt >= ticks)
				break; //时间超过/等于要延迟的时间,则退出.
		}
	};
}


/*!
 *  \brief  延迟ms
 *  \param  t ms数
 *  \note   delay_init后使用
 */
void delay_ms(u16 nms) {
	delay_us(1000*nms);
}

/*!
 *  \brief  按键扫描函数
 *  \param  mode 0不支持连按，1支持连按
 *  \retval 按键值
 *  \note   此函数有响应优先级,KEY0>KEY1>KEY2>WK_UP
 */
u8 KEY_Scan(u8 mode) {
	static u8 key_up = 1; //按键按松开标志
	if (mode)
		key_up = 1; //持连按
	if (key_up && (KEY0 == 0 || KEY1 == 0)) {
		HAL_Delay(10); //去抖动
		key_up = 0;
		if      (KEY0 == 0)
			return KEY0_PRES;
		else if (KEY1 == 0)
			return KEY1_PRES;
	} else if (KEY0 == 1 && KEY1 == 1)
		key_up = 1;
	return 0; // 按键按下
}

/*!
 *  \brief  数据保存模块（掉电储存）
 *  \note   用 AT24CXX 或者 W25QXX 来实现掉电储存
 */

//选择对应的储存方式
// #define USE_W25QXX
#define USE_AT24CXX

//储存地址选择
const u32 SAVE_ADDR = 0; //储存地址起始
/*!
 *  \brief  数据保存操作，0-写入 1-读出 2-写入
 *  \param mode 操作模式
 */
void DATA_OP(u8 mode)
{
  u8 *VAR_ADDR   = (u8*)&Svar; //变量地址
  u32 OP_ADDR = SAVE_ADDR ;    //FLASH储存首地址
//  u8  data;                    //暂存数据
  u16 size;                    //当前已经储存大小

  for(size=0;size<sizeof(SVAR);size++,VAR_ADDR++,OP_ADDR++)
  {
		switch(mode)
		{
#ifdef USE_W25QXX
			case 0:W25QXX_Write(VAR_ADDR,OP_ADDR,1);break;
			case 1:W25QXX_Read (VAR_ADDR,OP_ADDR,1);break;
			case 2:{
				W25QXX_Read (&data,OP_ADDR,1);
				if(data != *VAR_ADDR)
				W25QXX_Write(VAR_ADDR,OP_ADDR,1);
			}
#endif
#ifdef USE_AT24CXX
			case 0:AT24CXX_Write(OP_ADDR, VAR_ADDR, 1);break;
			case 1:AT24CXX_Read (OP_ADDR, VAR_ADDR, 1);break;
			case 2:{
				if(AT24CXX_ReadOneByte (OP_ADDR) != *VAR_ADDR)
					 AT24CXX_WriteOneByte(OP_ADDR,    *VAR_ADDR);
			}
#endif
		}
  }
}

/*!
 *  \brief 数据初始化，若按下KEY0则恢复默认
 */
void DATA_INIT() {
  u8 key = KEY_Scan(0);
  if(key == KEY0_PRES)  DATA_OP(0);
  else                  DATA_OP(1);
}

/*!
 *  \brief 数据更新
 */
void DATA_UPDATE() {
    DATA_OP(2);
}
