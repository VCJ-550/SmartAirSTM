#ifndef __SHOW_H_
#define __SHOW_H_
#include "sys.h"
#include "text.h"

//显示框参数结构体
//typedef struct ShowTid
//{
//  //起始X
//	//起始Y
//	//宽度W
//	//高度H
//	//背景色
//  //画笔色
//}ShowTidDef;

//extern ShowTidDef ShowTidStr;


void LCD_Clear(void); //清全屏
void Show_Welcome(void);
void ShowSensorPage(uint8_t tid);
void ShowWifiPage(uint8_t tid);


#endif

