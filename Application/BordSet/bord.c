#include "bord.h"


//板级初始化
void BordHardweardInit(void)
{
 	LED_Init();			      //LED端口初始化
  KEY_Init();
	PowerControlIoInit(); //电源控制引脚初始化
	BootOnSyatem();   //开机服务
////	Adc_Init();
	EXW25QXX_Init();			//初始化外部W25Q128
	LCD_Init();//LCD初始化
	TP_Init(); //触摸初始化
//  TIM1_PWM_Init(1999,71);	 //50Hz  
//	TIM_SetCompare1(TIM1,500);	//设置PWM占空比
////	WBRD_Init();
////	delay_ms(200);  //启动时一定要延时，否则电源供应不上
////	SensorCO2Init();	
 // delay_ms(200);	
//  SensorWZSInit(); 
//  delay_ms(200);  
////	HDC1080_Init();  
////	BH1750_Configure();
//	delay_ms(200);
//	SensorPMSInit();
	LCD_Clear();
}



