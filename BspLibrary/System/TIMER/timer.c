#include "timer.h"
#include "led.h"
#include "usart.h"


//TIM1 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM1_PWM_Init(u16 arr,u16 psc)
{  
  //�ض���ṹ��
	GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  //ʹ��TIM1��GPIOAʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOA, ENABLE);   
	//���ø�����Ϊ�����������,���TIM1 CH1��PWM���岨��	GPIOA.8
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM_CH1
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	//��ʼ��TIM1	
  TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
  TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
  TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
  //��ʼ��TIM1 Channel PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
  TIM_OCInitStructure.TIM_Pulse = 0; 
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
  TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //ʹ��TIM3��CCR2�ϵ�Ԥװ�ؼĴ���
  
	TIM_CtrlPWMOutputs(TIM1,ENABLE);  //MOE ʹ��PWM���
  
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH1����ͨ��1  
  
	TIM_ARRPreloadConfig(TIM1, ENABLE); //ʹ��TIMx��ARR
 
  TIM_Cmd(TIM1, ENABLE);  //ʹ��TIM1
}


void TIM2_PWM_Init(u16 arr,u16 psc)
{

}	

void TIM3_Init(u16 arr,u16 psc)
{

}

