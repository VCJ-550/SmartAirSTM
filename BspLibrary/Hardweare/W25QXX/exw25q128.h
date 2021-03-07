#ifndef __EXW25Q128_H
#define __EXW25Q128_H			    
#include "sys.h"  
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//W25QXX 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/6
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

//W25X系列/Q系列芯片列表	   
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

extern u16 EXW25QXX_TYPE;					//定义W25QXX芯片型号		   

#define	EXW25QXX_CS 		PBout(1)  		//W25QXX的片选信号

////////////////////////////////////////////////////////////////////////////////// 
//指令表
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
u16  EXW25QXX_ReadID(void);  	    		//读取FLASH ID
u8	 EXW25QXX_ReadSR(void);        		//读取状态寄存器 
void EXW25QXX_Write_SR(u8 sr);  			//写状态寄存器
void EXW25QXX_Write_Enable(void);  		//写使能 
void EXW25QXX_Write_Disable(void);		//写保护
void EXW25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void EXW25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);   //读取flash
void EXW25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//写入flash
void EXW25QXX_Erase_Chip(void);    	  	//整片擦除
void EXW25QXX_Erase_Sector(u32 Dst_Addr);	//扇区擦除
void EXW25QXX_Wait_Busy(void);           	//等待空闲
void EXW25QXX_PowerDown(void);        	//进入掉电模式
void EXW25QXX_WAKEUP(void);				//唤醒
#endif

