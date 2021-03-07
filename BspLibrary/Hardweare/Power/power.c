#include "power.h"

void PowerControlIoInit(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB, ENABLE);	 
 //充电完成指示引脚->PB6	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //上拉输入
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);		
 //充电控制引脚->PB7
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
 GPIO_Init(GPIOB, &GPIO_InitStructure);	
 //升压控制引脚->PC3
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
 GPIO_Init(GPIOC, &GPIO_InitStructure);		
 //开机锁定引脚->PC14 GPIO_Pin_14
 GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
 GPIO_Init(GPIOC, &GPIO_InitStructure); 
 //电源输入检测引脚->PC13
 GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //上拉输入
 GPIO_Init(GPIOC, &GPIO_InitStructure);
}


//开机服务函数
void BootOnSyatem(void)
{
	 if(PowerInPin)  //电源插入,非用户开机
	 {
		 ChargeON;  //开启充电
		 BoostOFF;  //关闭升压
		 BootUNLOCK;	//解锁开机	 
	 }
	 else //没有电源插入，用户开机
	 {
	  ChargeOFF;//关闭充电
		BoostON;  //开启升压
		BootLOCK; //锁定开机	 
	 }
   
}

