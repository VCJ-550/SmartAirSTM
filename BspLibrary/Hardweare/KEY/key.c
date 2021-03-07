#include "key.h"
#include "delay.h"

void KEY_Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure; 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//KEY-->PC10 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //上拉输入
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//IO口速度为50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 
}


uint8_t KEY_Scan(void)
{
  if(KEY == KEY_OFF)
	{
	 delay_ms(10);
		if(KEY == KEY_OFF)
		{
			while(!KEY);
		   return KEY_ON;
		}
		else 
			return KEY_OFF;
	}
  return KEY_OFF;
}

