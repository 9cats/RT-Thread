#include "AT24CXX.h"


/*!
 *  \brief  AT24CXX(EEPROM芯片)读取一个字节
 *  \param  ReadAddr 读取的地址
 *  \retval 读取到的数据
 */
u8 AT24CXX_ReadOneByte(u16 ReadAddr)
{
	u8 pBuffer;

	HAL_I2C_Mem_Read(&at24cxx, Read_ADDR, ReadAddr, I2C_MEMADD_SIZE_8BIT, &pBuffer, 1, HAL_MAX_DELAY);

	return pBuffer;
}

/*!
 *  \brief  AT24CXX(EEPROM芯片)写入一个字节
 *  \param  WriteAddr   写入的地址
 *  \param  DataToWrite 写入的数据
 *  \retval HAL Status
 */
HAL_StatusTypeDef AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{
	u8 pBuffer = DataToWrite;

	if(HAL_I2C_Mem_Write(&at24cxx, Write_ADDR, WriteAddr, I2C_MEMADD_SIZE_8BIT, &pBuffer, 1, HAL_MAX_DELAY) == HAL_OK)
	{
		//写入后要延迟5ms
		delay_ms(5);
		return HAL_OK;
	}
	else
		return HAL_ERROR;
}

/*!
 *  \brief  AT24CXX(EEPROM芯片)读出指定个数字节
 *  \param  ReadAddr    开始读出的地址
 *  \param  pBuffer     读出的缓存地址
 *  \param  NumToRead   要读出数据的个数
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
 *  \brief  AT24CXX(EEPROM芯片)写入指定个数字节
 *  \param  WriteAddr   开始写入读出的地址
 *  \param  pBuffer     写入的数据地址
 *  \param  NumToWrite  要写入数据的个数
 *  \retval HAL Status
 */
HAL_StatusTypeDef AT24CXX_Write(u16 WriteAddr, u8* pBuffer, u16 NumToWrite)
{
	u16 i;
	for(i=0; i<NumToWrite; i++)
	{
		if(HAL_I2C_Mem_Write(&at24cxx, Write_ADDR, WriteAddr+i, I2C_MEMADD_SIZE_8BIT, pBuffer+i, 1, HAL_MAX_DELAY) == HAL_OK)
		{
			//写入后要延迟5ms
			delay_ms(5);
			continue;
		}
		else
			return HAL_ERROR;
	}
	return HAL_OK;
}
