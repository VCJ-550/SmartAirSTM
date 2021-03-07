#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

	  	
extern uint8_t  WZSReadDataBuff[9]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern uint16_t USART1_RX_STA;         		//����״̬���

extern uint16_t UART5_RX_STA;  //����5������
extern uint8_t PMSReadDataBuff[32]; //���ݻ���

extern uint16_t USART2_RX_STA;
extern uint8_t CO2ReadDataBuff[8];

void uart_init(u32 bound);

void UART5_Init(uint32_t bound);
void USART_SendDataBuff(USART_TypeDef* USARTx, uint8_t *pBuff,uint8_t Length);

void usart3_init(u32 bound);
void USART3SendData(unsigned char pchar);
void usart2_init(u32 bound);

#endif

