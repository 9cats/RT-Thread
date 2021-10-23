#ifndef __AT24CXX_H__
#define __AT24CXX_H__
#include "main.h"
#include "i2c.h"

//对应型号芯片容量（单位：Byte）
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

/* 自行填写AT芯片型号以及对应的i2c */
#define EE_TYPE AT24C02
#define at24cxx hi2c1

               u8 AT24CXX_ReadOneByte(u16 ReadAddr);													//指定地址读取一个字节
HAL_StatusTypeDef AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite);					//指定地址写入一个字节
HAL_StatusTypeDef AT24CXX_Write(u16 WriteAddr, u8 *pBuffer, u16 NumToWrite);	//从指定地址开始写入指定长度的数据
HAL_StatusTypeDef AT24CXX_Read (u16 ReadAddr , u8 *pBuffer, u16 NumToRead);		//从指定地址开始读出指定长度的数据
#endif
