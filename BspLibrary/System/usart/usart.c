#include "sys.h"
#include "usart.h"	 
#include "led.h"
#include "wifi.h"
/*
B15:�������  B14  B13 B12 
B11:������ѯ�� B10:��ʼ��������� B9:�յ�ox4d B8:�յ�0x42    
B0~7:���ճ��� 
*/
uint16_t UART5_RX_STA = 0;  //����5������
uint8_t PMSReadDataBuff[32]; //���ݻ���

uint8_t WZSReadDataBuff[9];     //���ջ���

uint16_t USART2_RX_STA = 0;
uint8_t CO2ReadDataBuff[8];

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((UART5->SR&0X40)==0);//ѭ������,ֱ���������   
    UART5->DR = (u8) ch;      
	return ch;
}
#endif 
 
	

//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART1_RX_STA=0;       //����״̬���	  
  
void uart_init(u32 bound)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	  
	//USART1_TX->PA9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);   
  //USART1_RX->PA10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	  
  //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART1, &USART_InitStructure); 
  
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 
}

void USART1_IRQHandler(void)                	//����1�жϷ������
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
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOC, &GPIO_InitStructure);   
  //UART5_RX->PD2	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOD, &GPIO_InitStructure); 
  //Usart5 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	  
  //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(UART5, &USART_InitStructure); 
	
  USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(UART5, ENABLE);   
}

void UART5_IRQHandler(void)                	//����5�жϷ������
{
	uint8_t u5Res;
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  
	{
		u5Res =USART_ReceiveData(UART5);	
		if((UART5_RX_STA&0x8000)==0) //����δ���
		{
			switch(UART5_RX_STA&0x0F00) //������ѯ
			{
				case 0x0000 : //û�н��յ�֡ͷ1��2
					PMSReadDataBuff[0] = u5Res; 
				  UART5_RX_STA |= 0x0100; //��λ��־֡ͷ1 
				break;
				
				case 0x0100 : //���յ�֡ͷ1
					PMSReadDataBuff[1] = u5Res; 
				  UART5_RX_STA |= 0x0200; //��λ��־֡ͷ2 
				break;
				
				case 0x0300 : //���յ�֡ͷ1��2
					PMSReadDataBuff[2] = u5Res; 
				  UART5_RX_STA |= 0x0400; //��λ��ʼ֡���ձ�־ 
				  UART5_RX_STA = UART5_RX_STA+3;
				break;
				
				case 0x0700 : //���յ�֡ͷ1��2
					PMSReadDataBuff[UART5_RX_STA&0x00FF] = u5Res; 
					UART5_RX_STA++;
				  if(((UART5_RX_STA&0x0F00)==0x0700)&&((UART5_RX_STA&0x00FF)==32))
						UART5_RX_STA |= 0x8000; //��λ�������
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
   while((USARTx->SR&0X40)==0);//ѭ������,ֱ���������   
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
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOB, &GPIO_InitStructure);   
  //USART3_RX->PB11
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  //Usart3 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	  
  //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART3, &USART_InitStructure); 
  
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_ClearITPendingBit(USART3, USART_IT_RXNE);
  USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ���1 
}

void USART3_IRQHandler(void)                	//����3�жϷ������
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
   while((USART3->SR&0X40)==0);//ѭ������,ֱ���������   
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
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);   
  //USART2_RX->PA3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //Usart2 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	  
  //USART ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;
	//USART_InitStructure.USART_WordLength = USART_WordLength_8b; 
	USART_InitStructure.USART_WordLength = USART_WordLength_9b; 
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_Even;
//  USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART2, &USART_InitStructure); 
  
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_ClearITPendingBit(USART2, USART_IT_RXNE);
  USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ���1 
}

void USART2_IRQHandler(void)                	//����1�жϷ������
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

