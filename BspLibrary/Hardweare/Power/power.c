#include "power.h"

void PowerControlIoInit(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB, ENABLE);	 
 //������ָʾ����->PB6	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; 
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //��������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);		
 //����������->PB7
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
 GPIO_Init(GPIOB, &GPIO_InitStructure);	
 //��ѹ��������->PC3
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
 GPIO_Init(GPIOC, &GPIO_InitStructure);		
 //������������->PC14 GPIO_Pin_14
 GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
 GPIO_Init(GPIOC, &GPIO_InitStructure); 
 //��Դ����������->PC13
 GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //��������
 GPIO_Init(GPIOC, &GPIO_InitStructure);
}


//����������
void BootOnSyatem(void)
{
	 if(PowerInPin)  //��Դ����,���û�����
	 {
		 ChargeON;  //�������
		 BoostOFF;  //�ر���ѹ
		 BootUNLOCK;	//��������	 
	 }
	 else //û�е�Դ���룬�û�����
	 {
	  ChargeOFF;//�رճ��
		BoostON;  //������ѹ
		BootLOCK; //��������	 
	 }
   
}

