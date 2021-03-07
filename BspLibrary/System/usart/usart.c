#include "sys.h"
#include "usart.h"	 
#include "led.h"
#include "wifi.h"
/*
B15:接收完成  B14  B13 B12 
B11:接收轮询字 B10:起始符接收完成 B9:收到ox4d B8:收到0x42    
B0~7:接收长度 
*/
uint16_t UART5_RX_STA = 0;  //串口5接收字
uint8_t PMSReadDataBuff[32]; //数据缓存

uint8_t WZSReadDataBuff[9];     //接收缓冲

uint16_t USART2_RX_STA = 0;
uint8_t CO2ReadDataBuff[8];

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((UART5->SR&0X40)==0);//循环发送,直到发送完毕   
    UART5->DR = (u8) ch;      
	return ch;
}
#endif 
 
	

//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART1_RX_STA=0;       //接收状态标记	  
  
void uart_init(u32 bound)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	  
	//USART1_TX->PA9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);   
  //USART1_RX->PA10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	  
  //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART1, &USART_InitStructure); 
  
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
  USART_Cmd(USART1, ENABLE);                    //使能串口1 
}

void USART1_IRQHandler(void)                	//串口1中断服务程序
	{
	uint8_t u1Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  
		{
		u1Res =USART_ReceiveData(USART1);		
	  WZSReadDataBuff[USART1_RX_STA&0x00FF]	= u1Res;	
		if((USART1_RX_STA&0x00FF)==8)
			USART1_RX_STA |=0x8000; 
    else
      USART1_RX_STA++;				
     } 
} 

void UART5_Init(uint32_t bound)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOC, ENABLE);	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);	
	//UART5_TX->PC12
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOC, &GPIO_InitStructure);   
  //UART5_RX->PD2	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOD, &GPIO_InitStructure); 
  //Usart5 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	  
  //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(UART5, &USART_InitStructure); 
	
  USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(UART5, ENABLE);   
}

void UART5_IRQHandler(void)                	//串口5中断服务程序
{
	uint8_t u5Res;
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  
	{
		u5Res =USART_ReceiveData(UART5);	
		if((UART5_RX_STA&0x8000)==0) //接收未完成
		{
			switch(UART5_RX_STA&0x0F00) //接收轮询
			{
				case 0x0000 : //没有接收到帧头1、2
					PMSReadDataBuff[0] = u5Res; 
				  UART5_RX_STA |= 0x0100; //置位标志帧头1 
				break;
				
				case 0x0100 : //接收到帧头1
					PMSReadDataBuff[1] = u5Res; 
				  UART5_RX_STA |= 0x0200; //置位标志帧头2 
				break;
				
				case 0x0300 : //接收到帧头1、2
					PMSReadDataBuff[2] = u5Res; 
				  UART5_RX_STA |= 0x0400; //置位起始帧接收标志 
				  UART5_RX_STA = UART5_RX_STA+3;
				break;
				
				case 0x0700 : //接收到帧头1、2
					PMSReadDataBuff[UART5_RX_STA&0x00FF] = u5Res; 
					UART5_RX_STA++;
				  if(((UART5_RX_STA&0x0F00)==0x0700)&&((UART5_RX_STA&0x00FF)==32))
						UART5_RX_STA |= 0x8000; //置位接收完成
				break;
			}
		}
   } 
} 
	
void USART_SendDataBuff(USART_TypeDef* USARTx, uint8_t *pBuff,uint8_t Length)
{
	uint8_t i;
	for(i=0;i<Length;i++)
	{
   while((USARTx->SR&0X40)==0);//循环发送,直到发送完毕   
    USARTx->DR = pBuff[i];  
	}
}


void usart3_init(u32 bound)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	
	//USART3_TX->PB10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure);   
  //USART3_RX->PB11
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  //Usart3 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	  
  //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART3, &USART_InitStructure); 
  
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_ClearITPendingBit(USART3, USART_IT_RXNE);
  USART_Cmd(USART3, ENABLE);                    //使能串口1 
}

void USART3_IRQHandler(void)                	//串口3中断服务程序
	{
	uint8_t u3Res;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  
		{
			u3Res =USART_ReceiveData(USART3);		
			uart_receive_input(u3Res);				
    } 
}
	
void USART3SendData(unsigned char pchar)
{
   while((USART3->SR&0X40)==0);//循环发送,直到发送完毕   
    USART3->DR = pchar;  
}
	

void usart2_init(u32 bound)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	
	//USART2_TX->PA2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);   
  //USART2_RX->PA3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //Usart2 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	  
  //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;
	//USART_InitStructure.USART_WordLength = USART_WordLength_8b; 
	USART_InitStructure.USART_WordLength = USART_WordLength_9b; 
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_Even;
//  USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART2, &USART_InitStructure); 
  
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_ClearITPendingBit(USART2, USART_IT_RXNE);
  USART_Cmd(USART2, ENABLE);                    //使能串口1 
}

void USART2_IRQHandler(void)                	//串口1中断服务程序
	{
	uint8_t u2Res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  
		{
		u2Res =USART_ReceiveData(USART2);		
	  CO2ReadDataBuff[USART2_RX_STA&0x00FF]	= u2Res;	
		if((USART2_RX_STA&0x00FF)==8)
			USART2_RX_STA |=0x8000; 
    else
      USART2_RX_STA++;				
     } 
}

