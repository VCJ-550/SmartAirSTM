#include "timer.h"
#include "led.h"
#include "usart.h"


//TIM1 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM1_PWM_Init(u16 arr,u16 psc)
{  
  //重定向结构体
	GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  //使能TIM1、GPIOA时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOA, ENABLE);   
	//设置该引脚为复用输出功能,输出TIM1 CH1的PWM脉冲波形	GPIOA.8
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //TIM_CH1
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	//初始化TIM1	
  TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
  TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
  TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
  //初始化TIM1 Channel PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
  TIM_OCInitStructure.TIM_Pulse = 0; 
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
  TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //使能TIM3在CCR2上的预装载寄存器
  
	TIM_CtrlPWMOutputs(TIM1,ENABLE);  //MOE 使能PWM输出
  
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH1开启通道1  
  
	TIM_ARRPreloadConfig(TIM1, ENABLE); //使能TIMx的ARR
 
  TIM_Cmd(TIM1, ENABLE);  //使能TIM1
}


void TIM2_PWM_Init(u16 arr,u16 psc)
{

}	

void TIM3_Init(u16 arr,u16 psc)
{

}

