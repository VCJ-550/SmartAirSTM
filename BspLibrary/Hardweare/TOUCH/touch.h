#ifndef __TOUCH_H__
#define __TOUCH_H__
#include "sys.h"

#define Adujust      1        //1：使用出厂预设校准信息  0手动校准

#define TP_PRES_DOWN 0x80  		//触屏被按下	  
#define TP_CATH_PRES 0x40  		//有按键按下了 
#define CT_MAX_TOUCH  5    		//电容屏支持的点数,固定为5点

//触摸屏控制器
typedef struct 
{
	u8 (*init)(void);			//初始化触摸屏控制器
	u8 (*scan)(u8);				//扫描触摸屏.0,屏幕扫描;1,物理坐标;	 
	void (*adjust)(void);	//触摸屏校准 
	u16 x[CT_MAX_TOUCH]; 	//当前坐标
	u16 y[CT_MAX_TOUCH];	//电容屏有最多5组坐标,电阻屏则用x[0],y[0]代表:此次扫描时,触屏的坐标,用
								//x[4],y[4]存储第一次按下时的坐标. 
	u8  sta;		  //笔的状态 
								//b7:按下1/松开0; 
	              //b6:0,没有按键按下;1,有按键按下. 
								//b5:保留
/////////////////////触摸屏校准参数//////////////////////								
	float xfac;					
	float yfac;
	short xoff;
	short yoff;	   

}_m_tp_dev;

extern _m_tp_dev tp_dev;	 	//触屏控制器在touch.c里面定义

   
//电阻屏函数
void TP_Write_Byte(u8 num);					//向控制芯片写入一个数据
u16 TP_Read_AD(u8 CMD);							//读取AD转换值
u16 TP_Read_XOY(u8 xy);							//带滤波的坐标读取(X/Y)
u8 TP_Read_XY(u16 *x,u16 *y);				//双方向读取(X+Y)
u8 TP_Read_XY2(u16 *x,u16 *y);			//带加强滤波的双方向坐标读取
void TP_Drow_Touch_Point(u16 x,u16 y,u16 color);//画一个坐标校准点
void TP_Draw_Big_Point(u16 x,u16 y,u16 color);	//画一个大点
void LCD_DrawRoughLine(u16 x1,u16 y1,u16 x2,u16 y2,u16 color);//画粗一点的线

void TP_Adjust(void);							//触摸屏校准
u8 TP_Scan(u8 tp);								//扫描
u8 TP_Init(void);								  //初始化
 
#endif

















