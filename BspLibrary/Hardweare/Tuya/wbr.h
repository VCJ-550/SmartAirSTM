#ifndef __WBR_H_
#define __WBR_H_
#include "sys.h"

#define WBRDPowerON   GPIO_SetBits(GPIOC,GPIO_Pin_7)      //����ģ��
#define WBRDPowerOFF  GPIO_ResetBits(GPIOC,GPIO_Pin_7)    //�ر�ģ��


void WBRD_Init(void);


#endif



