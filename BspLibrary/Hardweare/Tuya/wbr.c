#include "wbr.h"
#include "usart.h"

void WBRD_Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//WBRDPower-->PC7 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//IO口速度为50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 
 GPIO_ResetBits(GPIOC,GPIO_Pin_7);//PC7 输出低	
 usart3_init(9600);
 WBRDPowerON;
}	

