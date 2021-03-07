#ifndef __TOUCH_H__
#define __TOUCH_H__
#include "sys.h"

#define Adujust      1        //1��ʹ�ó���Ԥ��У׼��Ϣ  0�ֶ�У׼

#define TP_PRES_DOWN 0x80  		//����������	  
#define TP_CATH_PRES 0x40  		//�а��������� 
#define CT_MAX_TOUCH  5    		//������֧�ֵĵ���,�̶�Ϊ5��

//������������
typedef struct 
{
	u8 (*init)(void);			//��ʼ��������������
	u8 (*scan)(u8);				//ɨ�败����.0,��Ļɨ��;1,��������;	 
	void (*adjust)(void);	//������У׼ 
	u16 x[CT_MAX_TOUCH]; 	//��ǰ����
	u16 y[CT_MAX_TOUCH];	//�����������5������,����������x[0],y[0]����:�˴�ɨ��ʱ,����������,��
								//x[4],y[4]�洢��һ�ΰ���ʱ������. 
	u8  sta;		  //�ʵ�״̬ 
								//b7:����1/�ɿ�0; 
	              //b6:0,û�а�������;1,�а�������. 
								//b5:����
/////////////////////������У׼����//////////////////////								
	float xfac;					
	float yfac;
	short xoff;
	short yoff;	   

}_m_tp_dev;

extern _m_tp_dev tp_dev;	 	//������������touch.c���涨��

   
//����������
void TP_Write_Byte(u8 num);					//�����оƬд��һ������
u16 TP_Read_AD(u8 CMD);							//��ȡADת��ֵ
u16 TP_Read_XOY(u8 xy);							//���˲��������ȡ(X/Y)
u8 TP_Read_XY(u16 *x,u16 *y);				//˫�����ȡ(X+Y)
u8 TP_Read_XY2(u16 *x,u16 *y);			//����ǿ�˲���˫���������ȡ
void TP_Drow_Touch_Point(u16 x,u16 y,u16 color);//��һ������У׼��
void TP_Draw_Big_Point(u16 x,u16 y,u16 color);	//��һ�����
void LCD_DrawRoughLine(u16 x1,u16 y1,u16 x2,u16 y2,u16 color);//����һ�����

void TP_Adjust(void);							//������У׼
u8 TP_Scan(u8 tp);								//ɨ��
u8 TP_Init(void);								  //��ʼ��
 
#endif

















