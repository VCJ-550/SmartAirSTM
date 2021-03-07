#ifndef __EXW25Q128_H
#define __EXW25Q128_H			    
#include "sys.h"  
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//W25QXX ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/6
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

//W25Xϵ��/Qϵ��оƬ�б�	   
//W25Q80  ID  0XEF13
//W25Q16  ID  0XEF14
//W25Q32  ID  0XEF15
//W25Q64  ID  0XEF16	
//W25Q128 ID  0XEF17	
#define EXW25Q80 	0XEF13 	
#define EXW25Q16 	0XEF14
#define EXW25Q32 	0XEF15
#define EXW25Q64 	0XEF16
#define EXW25Q128	0XEF17

extern u16 EXW25QXX_TYPE;					//����W25QXXоƬ�ͺ�		   

#define	EXW25QXX_CS 		PBout(1)  		//W25QXX��Ƭѡ�ź�

////////////////////////////////////////////////////////////////////////////////// 
//ָ���
#define EXW25X_WriteEnable		0x06 
#define EXW25X_WriteDisable		0x04 
#define EXW25X_ReadStatusReg		0x05 
#define EXW25X_WriteStatusReg		0x01 
#define EXW25X_ReadData			0x03 
#define EXW25X_FastReadData		0x0B 
#define EXW25X_FastReadDual		0x3B 
#define EXW25X_PageProgram		0x02 
#define EXW25X_BlockErase			0xD8 
#define EXW25X_SectorErase		0x20 
#define EXW25X_ChipErase			0xC7 
#define EXW25X_PowerDown			0xB9 
#define EXW25X_ReleasePowerDown	0xAB 
#define EXW25X_DeviceID			0xAB 
#define EXW25X_ManufactDeviceID	0x90 
#define EXW25X_JedecDeviceID		0x9F 

void EXW25QXX_Init(void);
u16  EXW25QXX_ReadID(void);  	    		//��ȡFLASH ID
u8	 EXW25QXX_ReadSR(void);        		//��ȡ״̬�Ĵ��� 
void EXW25QXX_Write_SR(u8 sr);  			//д״̬�Ĵ���
void EXW25QXX_Write_Enable(void);  		//дʹ�� 
void EXW25QXX_Write_Disable(void);		//д����
void EXW25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void EXW25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);   //��ȡflash
void EXW25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//д��flash
void EXW25QXX_Erase_Chip(void);    	  	//��Ƭ����
void EXW25QXX_Erase_Sector(u32 Dst_Addr);	//��������
void EXW25QXX_Wait_Busy(void);           	//�ȴ�����
void EXW25QXX_PowerDown(void);        	//�������ģʽ
void EXW25QXX_WAKEUP(void);				//����
#endif

