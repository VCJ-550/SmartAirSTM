#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

	  	
extern uint8_t  WZSReadDataBuff[9]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern uint16_t USART1_RX_STA;         		//接收状态标记

extern uint16_t UART5_RX_STA;  //串口5接收字
extern uint8_t PMSReadDataBuff[32]; //数据缓存

extern uint16_t USART2_RX_STA;
extern uint8_t CO2ReadDataBuff[8];

void uart_init(u32 bound);

void UART5_Init(uint32_t bound);
void USART_SendDataBuff(USART_TypeDef* USARTx, uint8_t *pBuff,uint8_t Length);

void usart3_init(u32 bound);
void USART3SendData(unsigned char pchar);
void usart2_init(u32 bound);

#endif

