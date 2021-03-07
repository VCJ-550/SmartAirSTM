#ifndef __POWER_H_
#define __POWER_H_
#include "sys.h"

#define ChargeOkPin  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)  //������ָʾ����->PB6
#define PowerInPin   GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13) //��Դ����������->PC13

#define ChargeON     GPIO_SetBits(GPIOB,GPIO_Pin_7)       //������
#define ChargeOFF    GPIO_ResetBits(GPIOB,GPIO_Pin_7)     //��ֹ���

#define BoostON      GPIO_SetBits(GPIOC,GPIO_Pin_3)       //������ѹ
#define BoostOFF     GPIO_ResetBits(GPIOC,GPIO_Pin_3)     //��ֹ��ѹ

#define BootLOCK     GPIO_SetBits(GPIOC,GPIO_Pin_14)      //��������
#define BootUNLOCK   GPIO_ResetBits(GPIOC,GPIO_Pin_14)    //�������� 

void PowerControlIoInit(void);
void BootOnSyatem(void);

#endif

