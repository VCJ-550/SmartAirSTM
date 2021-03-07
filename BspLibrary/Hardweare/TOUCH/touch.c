#include "touch.h" 
#include "lcd_init.h"
#include "lcd.h"
#include "delay.h"
#include "stdlib.h"
#include "math.h"
#include "spi.h"
#include "stdio.h"

_m_tp_dev tp_dev=
{
	TP_Init,
	TP_Scan,
	TP_Adjust,
	0,
	0, 
	0,
	0,
	0,
	0,	  	 		
	0,  	 		
};					

#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
u8 CMD_RDX=0XD0;
u8 CMD_RDY=0X90;

#else
u8 CMD_RDX=0X90;
u8 CMD_RDY=0XD0;
#endif
 	 			    					   
//SPIд����
//������ICд��1byte����    
//num:Ҫд�������
void TP_Write_Byte(u8 num)    
{  
 SPI2_ReadWriteByte(num);
} 		 
//SPI������ 
//�Ӵ�����IC��ȡadcֵ
//CMD:ָ��
//����ֵ:����������	   
u16 TP_Read_AD(u8 CMD)	  
{
	u16 Num=0; 
	TCLK=0;		//������ʱ�� 	 
	TDIN=0; 	//����������
	TCS=0; 		//ѡ�д�����IC
	TP_Write_Byte(CMD);//����������
	delay_us(6);
	TCLK=0;  	     	    
	delay_us(1);    	   
	TCLK=1; 		//��1��ʱ�ӣ����BUSY
	delay_us(1);    
	TCLK=0;  	     	    
  Num=SPI2_ReadWriteByte(0xff)<<8;
	Num|=SPI2_ReadWriteByte(0xff);
	Num>>=4;   	//ֻ�и�12λ��Ч.
	TCS=1;		//�ͷ�Ƭѡ	 
	return(Num);   
}
//��ȡһ������ֵ(x����y)
//������ȡREAD_TIMES������,����Щ������������,
//Ȼ��ȥ����ͺ����LOST_VAL����,ȡƽ��ֵ 
//xy:ָ�CMD_RDX/CMD_RDY��
//����ֵ:����������
#define READ_TIMES 5 	//��ȡ����
#define LOST_VAL 1	  	//����ֵ
u16 TP_Read_XOY(u8 xy)
{
	u16 i, j;
	u16 buf[READ_TIMES];
	u16 sum=0;
	u16 temp;
	for(i=0;i<READ_TIMES;i++)buf[i]=TP_Read_AD(xy);
	for(i=0;i<READ_TIMES-1; i++)//����
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])//��������
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	sum=0;
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)sum+=buf[i];
	temp=sum/(READ_TIMES-2*LOST_VAL);
	return temp;   
} 
//��ȡx,y����
//��Сֵ��������100.
//x,y:��ȡ��������ֵ
//����ֵ:0,ʧ��;1,�ɹ���
u8 TP_Read_XY(u16 *x,u16 *y)
{
	u16 xtemp,ytemp;
	xtemp=TP_Read_XOY(CMD_RDX);
	ytemp=TP_Read_XOY(CMD_RDY);
	*x=xtemp;
	*y=ytemp;
	return 1;//�����ɹ�
}
//����2�ζ�ȡ������IC,�������ε�ƫ��ܳ���
//ERR_RANGE,��������,����Ϊ������ȷ,�����������.	   
//�ú����ܴ�����׼ȷ��
//x,y:��ȡ��������ֵ
//����ֵ:0,ʧ��;1,�ɹ���
#define ERR_RANGE 50 //��Χ 
u8 TP_Read_XY2(u16 *x,u16 *y) 
{
	u16 x1,y1;
 	u16 x2,y2;
 	u8 flag;
	SPI2_SetSpeed(SPI_BaudRatePrescaler_32);//SPI 32��Ƶ
	flag=TP_Read_XY(&x1,&y1);   
	if(flag==0)return(0);
	flag=TP_Read_XY(&x2,&y2);	   
	if(flag==0)return(0);
	SPI2_SetSpeed(SPI_BaudRatePrescaler_4);//SP1  4��Ƶ
	if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//ǰ�����β�����+-50��
	&&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
	{
		*x=(x1+x2)/2;
		*y=(y1+y2)/2;
		return 1;
	}else return 0;	  
}  
//////////////////////////////////////////////////////////////////////////////////		  
//��LCD�����йصĺ���  
//��һ��������
//����У׼�õ�
//x,y:����
//color:��ɫ
void TP_Drow_Touch_Point(u16 x,u16 y,u16 color)
{
	LCD_DrawLine(x-12,y,x+13,y,color);//����
	LCD_DrawLine(x,y-12,x,y+13,color);//����
	LCD_DrawPoint(x+1,y+1,color);
	LCD_DrawPoint(x-1,y+1,color);
	LCD_DrawPoint(x+1,y-1,color);
	LCD_DrawPoint(x-1,y-1,color);
	Draw_Circle(x,y,6,color);//������Ȧ
}	  
//��һ�����(2*2�ĵ�)		   
//x,y:����
//color:��ɫ
void TP_Draw_Big_Point(u16 x,u16 y,u16 color)
{	    
	LCD_DrawPoint(x,y,color);//���ĵ� 
	LCD_DrawPoint(x+1,y,color);
	LCD_DrawPoint(x,y+1,color);
	LCD_DrawPoint(x+1,y+1,color);	 	  	
}

/******************************************************************************
      ����˵��������
      ������ݣ�x1,y1   ��ʼ����
                x2,y2   ��ֹ����
                color   �ߵ���ɫ
      ����ֵ��  ��
******************************************************************************/
void LCD_DrawRoughLine(u16 x1,u16 y1,u16 x2,u16 y2,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1;
	uRow=x1;//�����������
	uCol=y1;
	if(delta_x>0)incx=1; //���õ������� 
	else if (delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;//ˮƽ�� 
	else {incy=-1;delta_y=-delta_y;}
	if(delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		TP_Draw_Big_Point(uRow,uCol,color);//����
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////		  
//��������ɨ��
//tp:0,��Ļ����;1,��������(У׼�����ⳡ����)
//����ֵ:��ǰ����״̬.
//0,�����޴���;1,�����д���
u8 TP_Scan(u8 tp)
{			   
	if(PEN==0)//�а�������
	{
		if(tp)TP_Read_XY2(&tp_dev.x[0],&tp_dev.y[0]);//��ȡ��������
		else if(TP_Read_XY2(&tp_dev.x[0],&tp_dev.y[0]))//��ȡ��Ļ����
		{
	 		tp_dev.x[0]=tp_dev.xfac*tp_dev.x[0]+tp_dev.xoff;//�����ת��Ϊ��Ļ����
			tp_dev.y[0]=tp_dev.yfac*tp_dev.y[0]+tp_dev.yoff;  
	 	} 
		if((tp_dev.sta&TP_PRES_DOWN)==0)//֮ǰû�б�����
		{		 
			tp_dev.sta=TP_PRES_DOWN|TP_CATH_PRES;//��������  
			tp_dev.x[4]=tp_dev.x[0];//��¼��һ�ΰ���ʱ������
			tp_dev.y[4]=tp_dev.y[0];  	   			 
		}			   
	}else
	{
		if(tp_dev.sta&TP_PRES_DOWN)//֮ǰ�Ǳ����µ�
		{
			tp_dev.sta&=~(1<<7);//��ǰ����ɿ�
		}else//֮ǰ��û�б�����
		{
			tp_dev.x[4]=0;
			tp_dev.y[4]=0;
			tp_dev.x[0]=0xffff;
			tp_dev.y[0]=0xffff;
		}	    
	}
	return tp_dev.sta&TP_PRES_DOWN;//���ص�ǰ�Ĵ���״̬
}
		 
//������У׼����
//�õ��ĸ�У׼����
void TP_Adjust(void)
{								 
	u16 pos_temp[4][2];//���껺��ֵ
	u8  cnt=0;	
	u16 d1,d2;
	u32 tem1,tem2;
	double fac; 	
 	cnt=0;				
  LCD_Fill(0,0,LCD_W,LCD_H,WHITE);//����   
	LCD_ShowString(5,40,"Please adjustment the screen.",RED,WHITE,16,0);//��ʾ��ʾ��Ϣ
	TP_Drow_Touch_Point(20,20,RED);//����1 
	tp_dev.sta=0;//���������ź� 
	tp_dev.xfac=0;//xfac��������Ƿ�У׼��,����У׼֮ǰ�������!�������	 
	while(1)//�������10����û�а���,���Զ��˳�
	{
		tp_dev.scan(1);//ɨ����������
		if((tp_dev.sta&0xc0)==TP_CATH_PRES)//����������һ��(��ʱ�����ɿ���.)
		{		
			tp_dev.sta&=~(1<<6);//��ǰ����Ѿ����������.
						   			   
			pos_temp[cnt][0]=tp_dev.x[0];
			pos_temp[cnt][1]=tp_dev.y[0];
			cnt++;	  
			switch(cnt)
			{			   
				case 1:						 
					TP_Drow_Touch_Point(20,20,WHITE);				//�����1 
					TP_Drow_Touch_Point(LCD_W-20,20,RED);	//����2
					break;
				case 2:
 					TP_Drow_Touch_Point(LCD_W-20,20,WHITE);	//�����2
					TP_Drow_Touch_Point(20,LCD_H-20,RED);	//����3
					break;
				case 3:
 					TP_Drow_Touch_Point(20,LCD_H-20,WHITE);			//�����3
 					TP_Drow_Touch_Point(LCD_W-20,LCD_H-20,RED);	//����4
					break;
				case 4:	 //ȫ���ĸ����Ѿ��õ�
	    		    //�Ա����
					tem1=abs(pos_temp[0][0]-pos_temp[1][0]);//x1-x2
					tem2=abs(pos_temp[0][1]-pos_temp[1][1]);//y1-y2
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);//�õ�1,2�ľ���
					
					tem1=abs(pos_temp[2][0]-pos_temp[3][0]);//x3-x4
					tem2=abs(pos_temp[2][1]-pos_temp[3][1]);//y3-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);//�õ�3,4�ľ���
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05||d1==0||d2==0)//���ϸ�
					{
						cnt=0;
 				    TP_Drow_Touch_Point(LCD_W-20,LCD_H-20,WHITE);	//�����4
   	 				TP_Drow_Touch_Point(20,20,RED);								//����1
						LCD_ShowString(5,40,"Touch Adjust Filed!          ",RED,WHITE,16,0);//У��ʧ����ʾ
						LCD_ShowString(5,60,"Please Adjust Again!         ",RED,WHITE,16,0);//У��ʧ����ʾ
						delay_ms(1000);
 						continue;
					}
					tem1=abs(pos_temp[0][0]-pos_temp[2][0]);//x1-x3
					tem2=abs(pos_temp[0][1]-pos_temp[2][1]);//y1-y3
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);//�õ�1,3�ľ���
					
					tem1=abs(pos_temp[1][0]-pos_temp[3][0]);//x2-x4
					tem2=abs(pos_temp[1][1]-pos_temp[3][1]);//y2-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);//�õ�2,4�ľ���
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05)//���ϸ�
					{
						cnt=0;
 				    TP_Drow_Touch_Point(LCD_W-20,LCD_H-20,WHITE);	//�����4
   	 				TP_Drow_Touch_Point(20,20,RED);								//����1
						LCD_ShowString(5,40,"Touch Adjust Filed!          ",RED,WHITE,16,0);//У��ʧ����ʾ
						LCD_ShowString(5,60,"Please Adjust Again!         ",RED,WHITE,16,0);//У��ʧ����ʾ
						delay_ms(1000);
						continue;
					}//��ȷ��
								   
					//�Խ������
					tem1=abs(pos_temp[1][0]-pos_temp[2][0]);//x1-x3
					tem2=abs(pos_temp[1][1]-pos_temp[2][1]);//y1-y3
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);//�õ�1,4�ľ���
	
					tem1=abs(pos_temp[0][0]-pos_temp[3][0]);//x2-x4
					tem2=abs(pos_temp[0][1]-pos_temp[3][1]);//y2-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);//�õ�2,3�ľ���
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05)//���ϸ�
					{
						cnt=0;
 				    TP_Drow_Touch_Point(LCD_W-20,LCD_H-20,WHITE);	//�����4
   	 				TP_Drow_Touch_Point(20,20,RED);								//����1
						LCD_ShowString(5,40,"Touch Adjust Filed!          ",RED,WHITE,16,0);//У��ʧ����ʾ
						LCD_ShowString(5,60,"Please Adjust Again!         ",RED,WHITE,16,0);//У��ʧ����ʾ
						delay_ms(1000);
 							continue;
					}//��ȷ��
					//������
					tp_dev.xfac=(float)(LCD_W-40)/(pos_temp[1][0]-pos_temp[0][0]);//�õ�xfac		 
					tp_dev.xoff=(LCD_W-tp_dev.xfac*(pos_temp[1][0]+pos_temp[0][0]))/2;//�õ�xoff
						  
					tp_dev.yfac=(float)(LCD_H-40)/(pos_temp[2][1]-pos_temp[0][1]);//�õ�yfac
					tp_dev.yoff=(LCD_H-tp_dev.yfac*(pos_temp[2][1]+pos_temp[0][1]))/2;//�õ�yoff  

					TP_Drow_Touch_Point(LCD_W-20,LCD_H-20,WHITE);	//�����4
					LCD_ShowString(5,40,"Touch Screen Adjust OK!      ",RED,WHITE,16,0);//У�����
					LCD_ShowString(5,60,"                             ",RED,WHITE,16,0);//�������
					delay_ms(1000);
 					LCD_ShowString(5,40,"                       ",RED,WHITE,16,0);//�������
					return;//У�����				 
			}
		}
 	}
}	
//��������ʼ��  		    
u8 TP_Init(void)
{
	u8 fac[20];
	if(Adujust)
	{
		if(USE_HORIZONTAL==0)
		{//***���������β�ͬ��ԭ��Ĭ�ϵ�У׼����ֵ���ܻ�������ʶ��׼������У׼����ʹ�ã�������ʹ�ù̶���Ĭ��У׼����
			tp_dev.xfac=0.130623;
			tp_dev.xoff=-9;
			tp_dev.yfac=0.179869;
			tp_dev.yoff=-14;
		}else if(USE_HORIZONTAL==1)
		{//***���������β�ͬ��ԭ��Ĭ�ϵ�У׼����ֵ���ܻ�������ʶ��׼������У׼����ʹ�ã�������ʹ�ù̶���Ĭ��У׼����
			tp_dev.xfac=-0.130450;
			tp_dev.xoff=249;
			tp_dev.yfac=-0.181218;
			tp_dev.yoff=335;
		}else if(USE_HORIZONTAL==2)
		{//***���������β�ͬ��ԭ��Ĭ�ϵ�У׼����ֵ���ܻ�������ʶ��׼������У׼����ʹ�ã�������ʹ�ù̶���Ĭ��У׼����
			tp_dev.xfac=0.181279;
			tp_dev.xoff=-16;
			tp_dev.yfac=-0.132244;
			tp_dev.yoff=249;
		}else
		{//***���������β�ͬ��ԭ��Ĭ�ϵ�У׼����ֵ���ܻ�������ʶ��׼������У׼����ʹ�ã�������ʹ�ù̶���Ĭ��У׼����
			tp_dev.xfac=-0.179053;
			tp_dev.xoff=333;
			tp_dev.yfac=0.130295;
			tp_dev.yoff=-11;
		}
		return 0;
	}else
	{
		TP_Adjust();//��ĻУ׼  
		//У׼������ʾ��LCD�ϣ����Ĭ�ϲ�����׼�����ֶ�У���������޸�
		sprintf((char*)fac,"tp_dev.xfac:%10.6f",tp_dev.xfac);
		LCD_ShowString(10,LCD_H-140,fac,RED,WHITE,16,0);
		sprintf((char*)fac,"tp_dev.xoff:%d",tp_dev.xoff);
		LCD_ShowString(10,LCD_H-120,fac,RED,WHITE,16,0);
		sprintf((char*)fac,"tp_dev.yfac:%10.6f",tp_dev.yfac);
		LCD_ShowString(10,LCD_H-100,fac,RED,WHITE,16,0);
		sprintf((char*)fac,"tp_dev.yoff:%d",tp_dev.yoff);
		LCD_ShowString(10,LCD_H-80,fac,RED,WHITE,16,0);
		return 1;
	}
}

