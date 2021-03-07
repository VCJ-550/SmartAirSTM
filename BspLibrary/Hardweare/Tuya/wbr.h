#ifndef __WBR_H_
#define __WBR_H_
#include "sys.h"

#define WBRDPowerON   GPIO_SetBits(GPIOC,GPIO_Pin_7)      //开启模块
#define WBRDPowerOFF  GPIO_ResetBits(GPIOC,GPIO_Pin_7)    //关闭模块


void WBRD_Init(void);


#endif



