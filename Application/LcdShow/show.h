#ifndef __SHOW_H_
#define __SHOW_H_
#include "sys.h"
#include "text.h"

//��ʾ������ṹ��
//typedef struct ShowTid
//{
//  //��ʼX
//	//��ʼY
//	//���W
//	//�߶�H
//	//����ɫ
//  //����ɫ
//}ShowTidDef;

//extern ShowTidDef ShowTidStr;


void LCD_Clear(void); //��ȫ��
void Show_Welcome(void);
void ShowSensorPage(uint8_t tid);
void ShowWifiPage(uint8_t tid);


#endif

