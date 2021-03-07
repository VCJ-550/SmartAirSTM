#ifndef __POWER_H_
#define __POWER_H_
#include "sys.h"

#define ChargeOkPin  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)  //充电完成指示引脚->PB6
#define PowerInPin   GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13) //电源输入检测引脚->PC13

#define ChargeON     GPIO_SetBits(GPIOB,GPIO_Pin_7)       //允许充电
#define ChargeOFF    GPIO_ResetBits(GPIOB,GPIO_Pin_7)     //禁止充电

#define BoostON      GPIO_SetBits(GPIOC,GPIO_Pin_3)       //允许升压
#define BoostOFF     GPIO_ResetBits(GPIOC,GPIO_Pin_3)     //禁止升压

#define BootLOCK     GPIO_SetBits(GPIOC,GPIO_Pin_14)      //锁定开机
#define BootUNLOCK   GPIO_ResetBits(GPIOC,GPIO_Pin_14)    //解锁开机 

void PowerControlIoInit(void);
void BootOnSyatem(void);

#endif

