#include "main.h"
#include "base.h"
#include "AT24CXX.h"

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

/*!
 *  \brief  ���ݱ���ģ�飨���索�棩
 *  \note   �� AT24CXX ���� W25QXX ��ʵ�ֵ��索��
 */

//ѡ���Ӧ�Ĵ��淽ʽ
// #define USE_W25QXX
#define USE_AT24CXX

//�����ַѡ��
const u32 SAVE_ADDR = 0; //�����ַ��ʼ
/*!
 *  \brief  ���ݱ��������0-д�� 1-���� 2-д��
 *  \param mode ����ģʽ
 */
void DATA_OP(u8 mode)
{
  u8 *VAR_ADDR   = (u8*)&Svar; //������ַ
  u32 OP_ADDR = SAVE_ADDR ;    //FLASH�����׵�ַ
//  u8  data;                    //�ݴ�����
  u16 size;                    //��ǰ�Ѿ������С

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
 *  \brief ���ݳ�ʼ����������KEY0��ָ�Ĭ��
 */
void DATA_INIT() {
  u8 key = KEY_Scan(0);
  if(key == KEY0_PRES)  DATA_OP(0);
  else                  DATA_OP(1);
}

/*!
 *  \brief ���ݸ���
 */
void DATA_UPDATE() {
    DATA_OP(2);
}
