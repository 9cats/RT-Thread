#include "W25QXX.h"

//Ĭ�� W25QXX ��оƬ������ W25Q128
u16 W25QXX_TYPE = W25Q128;

//���ڱ��汻���������ݣ�һ��������
static u8 W25QXX_BUFFER[4096];

/*!
 *  \brief  W25QXX(FLASHоƬ)SPI���������һ���ֽ�
 *  \param  TxData ���͵�����
 *  \retval ���յ�����
 */
u8 W25QXX_ReadWriteByte(u8 TxData)
{
	u8 RxBuf,TxBuf = TxData;

	HAL_SPI_TransmitReceive(&w25qxx, &TxBuf, &RxBuf, 1, HAL_MAX_DELAY); //��������

	return RxBuf;
}

/*!
 *  \brief  W25QXX(FLASHоƬ)�����ٶ�
 *  \param  BaudRatePrescaler �����ʷ�����
 */
void W25QXX_SET_SPEED(u8 BaudRatePrescaler)
{
	__HAL_SPI_DISABLE(&w25qxx);								//ʧ��w25qxx��spi
	w25qxx.Instance->CR1 &= 0XFFC7;						//λ3-5���㣬�������ò�����
	w25qxx.Instance->CR1 |= BaudRatePrescaler;//����w25qxx��spi�ٶ�(�����ʷ�Ƶ��)
	__HAL_SPI_ENABLE(&hspi1);                 //ʹ��w25qxx��spi
}

/*!
 *  \brief  W25QXX(FLASHоƬ)��ʼ��
 */
void W25QXX_Init(void)
{
	W25QXX_CS_H;															//SPI FLASH��ѡ��
	W25QXX_ReadWriteByte(0xff);								//��������
	W25QXX_SET_SPEED(SPI_BAUDRATEPRESCALER_4);//����SPIΪ����ģʽ
	W25QXX_TYPE = W25QXX_ReadID();						//��ȡFLASH��ID
}

/*!
 *  \brief  ��ȡW25QXX��״̬�Ĵ���
 *  \retval W25QXX ��æ״̬(1,æ;0,����)
 */
u8 W25QXX_Read_Status(void)
{
	u8 rxBuf;

	W25QXX_CS_L;															//ʹ������
	W25QXX_ReadWriteByte(W25X_ReadStatusReg);	//���Ͷ�ȡ״̬�Ĵ���ָ��
	rxBuf = W25QXX_ReadWriteByte(0xff);				//��ȡ����ֵ
	W25QXX_CS_H;															//ȡ��Ƭѡ

	return rxBuf;
}


/*!
 *  \brief  W25QXXдʹ��
 */
void W25QXX_Write_Enable(void)
{
	W25QXX_CS_L;														//ʹ������
	W25QXX_ReadWriteByte(W25X_WriteEnable);	//����дʹ��ָ��
	W25QXX_CS_H;														//ȡ��Ƭѡ
}

/*!
 *  \brief  W25QXXдʹ��
 */
void W25QXX_Write_Disable(void)
{
	W25QXX_CS_L;															//ʹ������
	W25QXX_ReadWriteByte(W25X_WriteDisable);	//����дʧ��ָ��
	W25QXX_CS_H;															//ȡ��Ƭѡ
}

/*!
 *  \brief  W25QXX��ȡоƬID
 *  \retval оƬID
 *  \note 0XEF13,��ʾоƬ�ͺ�ΪW25Q80
 *  \note 0XEF14,��ʾоƬ�ͺ�ΪW25Q16
 *  \note 0XEF15,��ʾоƬ�ͺ�ΪW25Q32
 *  \note 0XEF16,��ʾоƬ�ͺ�ΪW25Q64
 *  \note 0XEF17,��ʾоƬ�ͺ�ΪW25Q128
 */
u16 W25QXX_ReadID(void)
{
	u8 rxBuf[2];

	W25QXX_CS_L;																//ʹ������
	W25QXX_ReadWriteByte(W25X_ManufactDeviceID);//���Ͷ�ȡ�豸IDָ��
	W25QXX_ReadWriteByte(0);										//��
	W25QXX_ReadWriteByte(0);
	W25QXX_ReadWriteByte(0);
	rxBuf[0] = W25QXX_ReadWriteByte(0xff);			//��ȡID�߰�λ
	rxBuf[1] = W25QXX_ReadWriteByte(0xff);			//��ȡID�Ͱ�λ
	W25QXX_CS_H;							//ȡ��Ƭѡ

	return ((u16)rxBuf[0]<<8) + rxBuf[1];
}

/*!
 *  \brief  ��ȡW25QXX����
 *  \param  pBuffer ��ȡ������ĵ�ַ
 *  \param  ReadAddr FLASH��Ŀ���ַ
 *  \param  NumByteToRead Ҫ��ȡ���ֽ���
 */
void W25QXX_Read(u8 *pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
	u16 i;

	W25QXX_CS_L;								//ʹ������

	/* ���Ͷ�ȡָ��,��ַ */
	W25QXX_ReadWriteByte(W25X_ReadData     );
	W25QXX_ReadWriteByte((u8)(ReadAddr>>16));
	W25QXX_ReadWriteByte((u8)(ReadAddr>>8 ));
	W25QXX_ReadWriteByte((u8)(ReadAddr    ));
	/* ��ȡ���� */
	for(i=0; i<NumByteToRead; i++) {
		pBuffer[i] = W25QXX_ReadWriteByte(0xff);
	}

	W25QXX_CS_H;								//ȡ��Ƭѡ
}

/*!
 *  \brief  W25QXX��һҳд������256���ֽڵ�����
 *  \param  pBuffer        Ҫд�������ĵ�ַ
 *  \param  WriteAddr      FLASH��Ŀ���ַ
 *  \param  NumByteToWrite Ҫд����ֽ���
 *  \warn   ���ܷ�ҳд������ / д��ǰ��Ҫ����
 */
void W25QXX_Write_Page(u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
	u16 i;

	W25QXX_Write_Enable();			//дʹ��
	W25QXX_CS_L;								//ʹ������

	/* ����д��ָ��,��ַ */
	W25QXX_ReadWriteByte(W25X_PageProgram   );
	W25QXX_ReadWriteByte((u8)(WriteAddr>>16));
	W25QXX_ReadWriteByte((u8)(WriteAddr>>8 ));
	W25QXX_ReadWriteByte((u8)(WriteAddr    ));
	for(i=0; i<NumByteToWrite; i++) {
		W25QXX_ReadWriteByte(pBuffer[i]);
	}

	W25QXX_CS_H;								//ȡ��Ƭѡ
	W25QXX_Wait_Busy();						//�ȴ�����
}

/*!
 *  \brief  �޼���дSPI FLASH
 *  \param  pBuffer        Ҫд�������ĵ�ַ
 *  \param  WriteAddr      FLASH��Ŀ���ַ
 *  \param  NumByteToWrite Ҫд����ֽ���
 *  \note   д��ǰ��Ҫ����
 */
void W25QXX_Write_NoCheck(u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
	u16 pageremain;

	pageremain = 256 - WriteAddr % 256; //��ҳʣ����ֽ���
	if (NumByteToWrite <= pageremain)
		pageremain = NumByteToWrite; //������256���ֽ�

	for(;;)
	{
		W25QXX_Write_Page(pBuffer, WriteAddr, pageremain);
		if (NumByteToWrite == pageremain)
			break; //д�������
		else     //NumByteToWrite>pageremain
		{
			pBuffer += pageremain;
			WriteAddr += pageremain;

			NumByteToWrite -= pageremain; //��ȥ�Ѿ�д���˵��ֽ���
			if (NumByteToWrite > 256)
				pageremain = 256; //һ�ο���д��256���ֽ�
			else
				pageremain = NumByteToWrite; //����256���ֽ���
		}
	}
}

/*!
 *  \brief  ����дSPI FLASH
 *  \param  pBuffer        Ҫд�������ĵ�ַ
 *  \param  WriteAddr      FLASH��Ŀ���ַ
 *  \param  NumByteToWrite Ҫд����ֽ���
 */
void W25QXX_Write(u8 *pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
	u32 secpos;
	u16 secoff;
	u16 secremain;
	u16 i;
	u8 *W25QXX_BUF;
	W25QXX_BUF = W25QXX_BUFFER;
	secpos = WriteAddr / 4096; //������ַ
	secoff = WriteAddr % 4096; //�������ڵ�ƫ��
	secremain = 4096 - secoff; //����ʣ��ռ��С

	if (NumByteToWrite <= secremain)
		secremain = NumByteToWrite; //������4096���ֽ�
	for(;;) {
		W25QXX_Read(W25QXX_BUF, secpos * 4096, 4096); //������������������
		for (i = 0; i < secremain; i++)				  //У������
		{
			if (W25QXX_BUF[secoff + i] != 0XFF)
				break; //��Ҫ����
		}
		if (i < secremain) //��Ҫ����
		{
			W25QXX_Erase_Sector(secpos);	//�����������
			for (i = 0; i < secremain; i++) //����
			{
				W25QXX_BUF[i + secoff] = pBuffer[i];
			}
			W25QXX_Write_NoCheck(W25QXX_BUF, secpos * 4096, 4096); //д����������
		}
		else
			W25QXX_Write_NoCheck(pBuffer, WriteAddr, secremain); //д�Ѿ������˵�,ֱ��д������ʣ������.
		if (NumByteToWrite == secremain)
			break; //д�������
		else	   //д��δ����
		{
			secpos++;	//������ַ��1
			secoff = 0; //ƫ��λ��Ϊ0
			pBuffer += secremain;		 //ָ��ƫ��
			WriteAddr += secremain;		 //д��ַƫ��
			NumByteToWrite -= secremain; //�ֽ����ݼ�
			if (NumByteToWrite > 4096)
				secremain = 4096; //��һ����������д����
			else
				secremain = NumByteToWrite; //��һ����������д����
		}
	}
}

/*!
 *  \brief  ��������оƬ
 *  \warn   ��Ҫʱ�䳤
 */
void W25QXX_Erase_Chip(void)
{
	W25QXX_Write_Enable();								//дʹ��
	W25QXX_Wait_Busy();										//�ȴ�����
	W25QXX_CS_L;													//ʹ������
	W25QXX_ReadWriteByte(W25X_ChipErase);	//���Ͳ�������оƬָ��
	W25QXX_CS_H;													//ȡ��Ƭѡ
	W25QXX_Wait_Busy();										//�ȴ�оƬ��������
}

/*!
 *  \brief  ����һ������
 *  \param  Dst_Addr ������
 *  \warn   ����һ������������Ҫ150ms
 */
void W25QXX_Erase_Sector(u32 Dst_Addr)
{
	Dst_Addr *= 4096;

	W25QXX_Write_Enable();											//дʹ��
	W25QXX_Wait_Busy();													//�ȴ�����
	W25QXX_CS_L;																//ʹ������
	W25QXX_ReadWriteByte(W25X_SectorErase);			//���Ͳ���һ��ɽ��ָ��
	W25QXX_ReadWriteByte((u8)(Dst_Addr)>>16);		//����24λ��ַ
	W25QXX_ReadWriteByte((u8)(Dst_Addr)>>8 );
	W25QXX_ReadWriteByte((u8)(Dst_Addr)    );
	W25QXX_CS_H;																//ȡ��Ƭѡ
	W25QXX_Wait_Busy();											//�ȴ�оƬ��������
}

/*!
 *  \brief  �������ģʽ
 */
void W25QXX_PowerDown(void)
{
	W25QXX_CS_L;													//ʹ������
	W25QXX_ReadWriteByte(W25X_PowerDown);	//���Ͳ���һ��ɽ��ָ��
	W25QXX_CS_H;													//ȡ��Ƭѡ
	delay_ms(3);													//�ȴ�TPD
}


/*!
 *  \brief  �ȴ�W25QXX����
 */
void W25QXX_Wait_Busy(void)
{
	while ((W25QXX_Read_Status() & 0x01) == 0x01);
}

