#ifndef __W25QXX_H__
#define __W25QXX_H__
#include "stm32f4xx.h"
#include "main.h"
#include "spi.h"

//W25Xϵ��/Qϵ��оƬ�б�
#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17

//ָ���
#define W25X_WriteEnable			0x06
#define W25X_WriteDisable			0x04
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData					0x03
#define W25X_FastReadData			0x0B
#define W25X_FastReadDual			0x3B
#define W25X_PageProgram			0x02
#define W25X_BlockErase				0xD8
#define W25X_SectorErase			0x20
#define W25X_ChipErase				0xC7
#define W25X_PowerDown				0xB9
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID					0xAB
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F

/* ������дW25QXX_CS��Ӧ��Ƭѡ�� �Լ� ��Ӧ��spi */
#define W25QXX_CS_H HAL_GPIO_WritePin(W25QXX_CS_GPIO_Port, W25QXX_CS_Pin, GPIO_PIN_SET  )
#define W25QXX_CS_L HAL_GPIO_WritePin(W25QXX_CS_GPIO_Port, W25QXX_CS_Pin, GPIO_PIN_RESET)
#define w25qxx hspi1

void W25QXX_Init(void);																									//W25QXX��ʼ��
u16  W25QXX_ReadID(void);																								//��ȡFLASH ID
u8	 W25QXX_ReadSR(void);																								//��ȡ״̬�Ĵ���
void W25QXX_Write_SR(u8 sr);																						//д״̬�Ĵ���
void W25QXX_Write_Enable(void);																					//дʹ��
void W25QXX_Write_Disable(void);																				//д����
void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//����дflash
void W25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);						//��ȡflash
void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);				//д��flash
void W25QXX_Erase_Chip(void);																						//��Ƭ����
void W25QXX_Erase_Sector(u32 Dst_Addr);																	//��������
void W25QXX_Wait_Busy(void);																						//�ȴ�����
void W25QXX_PowerDown(void);																						//�������ģʽ
void W25QXX_WAKEUP(void);																								//����

#endif
