#ifndef __HDC_H_
#define __HDC_H_
#include "sys.h"

//HDC1080
/////////计算公式///////////////////////////////////////////
//temper=[(d15:d0)/2^16]*165c-40c
//relative=[(d15:d0)/2^16]*100%
///////////HDC1080寄存器说明////////////////////////////////
#define HDC1080_Device_Adderss      0x80
#define HDC1080_Read_Temperature    0x00
#define HDC1080_Read_Humidity       0x01
#define HDC1080_Read_Config         0x02

#define HDC1080_DEVICE_ID           0xff
#define HDC1080_ID                  0x1000
//////////////////////////////////////////////////////////////
#define HDC1080_Rst                 15//0x8000软复位
#define HDC1080_Enht                13//0x2000//使能加热
#define HDC1080_Mode                12//0x1000//工作模式-为0时分开来读，为1时可以连续读温度在前
#define HDC1080_Trest               10//0x0000  0为温度14bit 1为11bit
#define HDC1080_Hres                8//0x0000 14 11 7bit 温度

typedef struct HDC1080MenT
{
u16 Humidity ;
u16 Temperature;
u16 Device_ID;
}HDC1080MenTDef;

extern HDC1080MenTDef HDC1080MenTStr;

void HDC1080_Init(void);
uint8_t HDC1080_Write_Buffer(uint8_t addr, uint8_t *buffer, uint8_t len);
void HDC1080_Soft_Reset(void);
void HDC1080_Setting(void);
uint8_t HDC1080_Read_Buffer(uint8_t addr, uint8_t *buffer, uint8_t len);
uint8_t HDC1080_Read_MBuffer(uint8_t addr, uint8_t *buffer, uint8_t len);
uint16_t HDC1080_Read_Temper(void);
uint16_t HDC1080_Read_Humidi(void);
uint16_t HDC1080_Read_Conf(void);
uint16_t HDC1080_Read_ManufacturerID(void);
uint16_t HDC1080_Read_DeviceID(void);


#endif

