#ifndef __AT24CXX_H__
#define __AT24CXX_H__
#include "main.h"
#include "i2c.h"

//��Ӧ�ͺ�оƬ��������λ��Byte��
#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64		8191
#define AT24C128	16383
#define AT24C256	32767

/*! Private macros define */
#define Write_ADDR 0xA0
#define  Read_ADDR 0XA1

/* ������дATоƬ�ͺ��Լ���Ӧ��i2c */
#define EE_TYPE AT24C02
#define at24cxx hi2c1

               u8 AT24CXX_ReadOneByte(u16 ReadAddr);													//ָ����ַ��ȡһ���ֽ�
HAL_StatusTypeDef AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite);					//ָ����ַд��һ���ֽ�
HAL_StatusTypeDef AT24CXX_Write(u16 WriteAddr, u8 *pBuffer, u16 NumToWrite);	//��ָ����ַ��ʼд��ָ�����ȵ�����
HAL_StatusTypeDef AT24CXX_Read (u16 ReadAddr , u8 *pBuffer, u16 NumToRead);		//��ָ����ַ��ʼ����ָ�����ȵ�����
#endif
