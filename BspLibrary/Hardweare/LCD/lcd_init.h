#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "sys.h"

#define USE_HORIZONTAL 0  //���ú�������������ʾ 0��1Ϊ���� 2��3Ϊ����


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 240
#define LCD_H 320

#else
#define LCD_W 320
#define LCD_H 240
#endif

//������оƬ��������
#define TCLK 		PBout(13)  	//PB13->SCLK
#define TDIN 		PBout(15)  	//PB15->MOSI
#define DOUT 		PBin(14)   	//PB14->MISO
#define TCS  		PBout(12)  	//PB12->CS2
#define PEN  		PCin(6)    	//PC6->TEN  INT


//-----------------LCD�˿ڶ���---------------- 

#define LCD_SCLK_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_7)//SCL=SCLK
#define LCD_SCLK_Set() GPIO_SetBits(GPIOA,GPIO_Pin_7)

#define LCD_MOSI_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_5)//SDA=MOSI
#define LCD_MOSI_Set() GPIO_SetBits(GPIOA,GPIO_Pin_5)

#define LCD_RES_Clr()  GPIO_ResetBits(GPIOB,GPIO_Pin_0)//RES
#define LCD_RES_Set()  GPIO_SetBits(GPIOB,GPIO_Pin_0)

#define LCD_DC_Clr()   GPIO_ResetBits(GPIOC,GPIO_Pin_4)//DC
#define LCD_DC_Set()   GPIO_SetBits(GPIOC,GPIO_Pin_4)
 		     
#define LCD_CS_Clr()   GPIO_ResetBits(GPIOC,GPIO_Pin_5)//CS1
#define LCD_CS_Set()   GPIO_SetBits(GPIOC,GPIO_Pin_5)

#define LCD_BLK_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_15)//BLK
#define LCD_BLK_Set()  GPIO_SetBits(GPIOA,GPIO_Pin_15)




void LCD_GPIO_Init(void);//��ʼ��GPIO
void LCD_Writ_Bus(u8 dat);//ģ��SPIʱ��
void LCD_WR_DATA8(u8 dat);//д��һ���ֽ�
void LCD_WR_DATA(u16 dat);//д�������ֽ�
void LCD_WR_REG(u8 dat);//д��һ��ָ��
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2);//�������꺯��
void LCD_Init(void);//LCD��ʼ��
#endif




