#include "bord.h"


//�弶��ʼ��
void BordHardweardInit(void)
{
 	LED_Init();			      //LED�˿ڳ�ʼ��
  KEY_Init();
	PowerControlIoInit(); //��Դ�������ų�ʼ��
	BootOnSyatem();   //��������
////	Adc_Init();
	EXW25QXX_Init();			//��ʼ���ⲿW25Q128
	LCD_Init();//LCD��ʼ��
	TP_Init(); //������ʼ��
//  TIM1_PWM_Init(1999,71);	 //50Hz  
//	TIM_SetCompare1(TIM1,500);	//����PWMռ�ձ�
////	WBRD_Init();
////	delay_ms(200);  //����ʱһ��Ҫ��ʱ�������Դ��Ӧ����
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



