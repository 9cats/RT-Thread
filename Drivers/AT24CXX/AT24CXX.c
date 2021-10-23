#include "AT24CXX.h"


/*!
 *  \brief  AT24CXX(EEPROMоƬ)��ȡһ���ֽ�
 *  \param  ReadAddr ��ȡ�ĵ�ַ
 *  \retval ��ȡ��������
 */
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{
	u8 pBuffer;

	HAL_I2C_Mem_Read(&at24cxx, Read_ADDR, ReadAddr, I2C_MEMADD_SIZE_8BIT, &pBuffer, 1, HAL_MAX_DELAY);

	return pBuffer;
}

/*!
 *  \brief  AT24CXX(EEPROMоƬ)д��һ���ֽ�
 *  \param  WriteAddr   д��ĵ�ַ
 *  \param  DataToWrite д�������
 *  \retval HAL Status
 */
HAL_StatusTypeDef AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{
	u8 pBuffer = DataToWrite;

	if(HAL_I2C_Mem_Write(&at24cxx, Write_ADDR, WriteAddr, I2C_MEMADD_SIZE_8BIT, &pBuffer, 1, HAL_MAX_DELAY) == HAL_OK)
	{
		//д���Ҫ�ӳ�5ms
		delay_ms(5);
		return HAL_OK;
	}
	else
		return HAL_ERROR;
}

/*!
 *  \brief  AT24CXX(EEPROMоƬ)����ָ�������ֽ�
 *  \param  ReadAddr    ��ʼ�����ĵ�ַ
 *  \param  pBuffer     �����Ļ����ַ
 *  \param  NumToRead   Ҫ�������ݵĸ���
 *  \retval HAL Status
 */
HAL_StatusTypeDef AT24CXX_Read (u16 ReadAddr, u8* pBuffer, u16 NumToRead)
{
	if(HAL_I2C_Mem_Read(&at24cxx, Read_ADDR, ReadAddr, I2C_MEMADD_SIZE_8BIT, pBuffer, NumToRead, HAL_MAX_DELAY) == HAL_OK)
		return HAL_OK;
	else
		return HAL_ERROR;
}

/*!
 *  \brief  AT24CXX(EEPROMоƬ)д��ָ�������ֽ�
 *  \param  WriteAddr   ��ʼд������ĵ�ַ
 *  \param  pBuffer     д������ݵ�ַ
 *  \param  NumToWrite  Ҫд�����ݵĸ���
 *  \retval HAL Status
 */
HAL_StatusTypeDef AT24CXX_Write(u16 WriteAddr, u8* pBuffer, u16 NumToWrite)
{
	u16 i;
	for(i=0; i<NumToWrite; i++)
	{
		if(HAL_I2C_Mem_Write(&at24cxx, Write_ADDR, WriteAddr+i, I2C_MEMADD_SIZE_8BIT, pBuffer+i, 1, HAL_MAX_DELAY) == HAL_OK)
		{
			//д���Ҫ�ӳ�5ms
			delay_ms(5);
			continue;
		}
		else
			return HAL_ERROR;
	}
	return HAL_OK;
}
