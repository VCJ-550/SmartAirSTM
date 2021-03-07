#ifndef __HDC_H_
#define __HDC_H_
#include "sys.h"

//HDC1080
/////////���㹫ʽ///////////////////////////////////////////
//temper=[(d15:d0)/2^16]*165c-40c
//relative=[(d15:d0)/2^16]*100%
///////////HDC1080�Ĵ���˵��////////////////////////////////
#define HDC1080_Device_Adderss      0x80
#define HDC1080_Read_Temperature    0x00
#define HDC1080_Read_Humidity       0x01
#define HDC1080_Read_Config         0x02

#define HDC1080_DEVICE_ID           0xff
#define HDC1080_ID                  0x1000
//////////////////////////////////////////////////////////////
#define HDC1080_Rst                 15//0x8000��λ
#define HDC1080_Enht                13//0x2000//ʹ�ܼ���
#define HDC1080_Mode                12//0x1000//����ģʽ-Ϊ0ʱ�ֿ�������Ϊ1ʱ�����������¶���ǰ
#define HDC1080_Trest               10//0x0000  0Ϊ�¶�14bit 1Ϊ11bit
#define HDC1080_Hres                8//0x0000 14 11 7bit �¶�

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

