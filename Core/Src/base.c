#include "main.h"
#include "base.h"
#include "string.h"
#include "AT24CXX.h"
#include "W25QXX.h"

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
#define USE_RT_THREAD_Delay_MS

#ifdef USE_RT_THREAD_Delay_MS
	rt_thread_mdelay(nms);
#else
	delay_us(nms*1000);
#endif
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
		delay_ms(10); //去抖动
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
//#define USE_W25QXX
#define USE_AT24CXX

//储存地址选择
u32 SAVE_ADDR = 0x00; //储存地址起始
/*!
 *  \brief  数据保存操作，0-写入 1-读出 2-写入
 *  \param mode 操作模式
 */
void DATA_OP(u8 mode)
{
  SVAR ROMData;

#ifdef USE_W25QXX
	switch(mode) {
		case 0:W25QXX_Write((u8*)&Svar   , SAVE_ADDR, sizeof(SVAR));break;
		case 1:W25QXX_Read ((u8*)&Svar   , SAVE_ADDR, sizeof(SVAR));break;
		case 2:{
			W25QXX_Read ((u8*)&ROMData, SAVE_ADDR, sizeof(SVAR));
			if(memcmp(&ROMData, &Svar, sizeof(SVAR)))
				W25QXX_Write((u8*)&Svar     , SAVE_ADDR, sizeof(SVAR));
		}
	}
#endif

#ifdef USE_AT24CXX
	switch(mode) {
		case 0:AT24CXX_Write(SAVE_ADDR , (u8*)&Svar, sizeof(SVAR));break;
		case 1:AT24CXX_Read (SAVE_ADDR , (u8*)&Svar, sizeof(SVAR));break;
		case 2:{
			AT24CXX_Read(SAVE_ADDR , (u8*)&ROMData, sizeof(SVAR));
			if(memcmp(&ROMData, &Svar, sizeof(SVAR)))
				AT24CXX_Write(SAVE_ADDR , (u8*)&Svar, sizeof(SVAR));
		}
	}
#endif
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
