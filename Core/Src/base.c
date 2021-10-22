#include "main.h"
#include "base.h"

//us��ʱ������
static u8 fac_us = 0;
//sprintfת��BUF
u8 Str_Buf[200]= {0};


/*!
 *  \brief  ��ʼ���ӳٺ���
 *  \param  SYSCLK   ϵͳʱ��Ƶ�ʣ���72Mhz����д72��
 *  \note   SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
 */
void delay_init(u8 SYSCLK) {
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK); //SysTickƵ��ΪHCLK
	fac_us = SYSCLK;
}


/*!
 *  \brief  �ӳ�ns
 *  \param  t ns��
 *  \note   delay_init��ʹ��
 */
void delay_ns(u8 t) {
	do {
		;
	} while (--t);
}


/*!
 *  \brief  �ӳ�us
 *  \param  t us��
 *  \note   delay_init��ʹ��
 */
void delay_us(u32 nus) {
	u32 ticks;
	u32 told, tnow, tcnt = 0;
	u32 reload = SysTick->LOAD; //LOAD��ֵ
	ticks = nus * fac_us;       //��Ҫ�Ľ�����
	told = SysTick->VAL;        //�ս���ʱ�ļ�����ֵ
	while (1) {
		tnow = SysTick->VAL;
		if (tnow != told) {
			if (tnow < told)
				tcnt += told - tnow; //����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else
				tcnt += reload - tnow + told;
			told = tnow;
			if (tcnt >= ticks)
				break; //ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}
	};
}


/*!
 *  \brief  �ӳ�ms
 *  \param  t ms��
 *  \note   delay_init��ʹ��
 */
void delay_ms(u16 nms) {
	delay_us(1000*nms);
}

/*!
 *  \brief  ����ɨ�躯��
 *  \param  mode 0��֧��������1֧������
 *  \retval ����ֵ
 *  \note   �˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>WK_UP
 */
u8 KEY_Scan(u8 mode) {
	static u8 key_up = 1; //�������ɿ���־
	if (mode)
		key_up = 1; //������
	if (key_up && (KEY0 == 0 || KEY1 == 0)) {
		HAL_Delay(10); //ȥ����
		key_up = 0;
		if      (KEY0 == 0)
			return KEY0_PRES;
		else if (KEY1 == 0)
			return KEY1_PRES;
	} else if (KEY0 == 1 && KEY1 == 1)
		key_up = 1;
	return 0; // ��������
}
