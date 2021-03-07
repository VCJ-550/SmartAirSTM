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
 	 			    					   
//SPI写数据
//向触摸屏IC写入1byte数据    
//num:要写入的数据
void TP_Write_Byte(u8 num)    
{  
 SPI2_ReadWriteByte(num);
} 		 
//SPI读数据 
//从触摸屏IC读取adc值
//CMD:指令
//返回值:读到的数据	   
u16 TP_Read_AD(u8 CMD)	  
{
	u16 Num=0; 
	TCLK=0;		//先拉低时钟 	 
	TDIN=0; 	//拉低数据线
	TCS=0; 		//选中触摸屏IC
	TP_Write_Byte(CMD);//发送命令字
	delay_us(6);
	TCLK=0;  	     	    
	delay_us(1);    	   
	TCLK=1; 		//给1个时钟，清除BUSY
	delay_us(1);    
	TCLK=0;  	     	    
  Num=SPI2_ReadWriteByte(0xff)<<8;
	Num|=SPI2_ReadWriteByte(0xff);
	Num>>=4;   	//只有高12位有效.
	TCS=1;		//释放片选	 
	return(Num);   
}
//读取一个坐标值(x或者y)
//连续读取READ_TIMES次数据,对这些数据升序排列,
//然后去掉最低和最高LOST_VAL个数,取平均值 
//xy:指令（CMD_RDX/CMD_RDY）
//返回值:读到的数据
#define READ_TIMES 5 	//读取次数
#define LOST_VAL 1	  	//丢弃值
u16 TP_Read_XOY(u8 xy)
{
	u16 i, j;
	u16 buf[READ_TIMES];
	u16 sum=0;
	u16 temp;
	for(i=0;i<READ_TIMES;i++)buf[i]=TP_Read_AD(xy);
	for(i=0;i<READ_TIMES-1; i++)//排序
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])//升序排列
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
//读取x,y坐标
//最小值不能少于100.
//x,y:读取到的坐标值
//返回值:0,失败;1,成功。
u8 TP_Read_XY(u16 *x,u16 *y)
{
	u16 xtemp,ytemp;
	xtemp=TP_Read_XOY(CMD_RDX);
	ytemp=TP_Read_XOY(CMD_RDY);
	*x=xtemp;
	*y=ytemp;
	return 1;//读数成功
}
//连续2次读取触摸屏IC,且这两次的偏差不能超过
//ERR_RANGE,满足条件,则认为读数正确,否则读数错误.	   
//该函数能大大提高准确度
//x,y:读取到的坐标值
//返回值:0,失败;1,成功。
#define ERR_RANGE 50 //误差范围 
u8 TP_Read_XY2(u16 *x,u16 *y) 
{
	u16 x1,y1;
 	u16 x2,y2;
 	u8 flag;
	SPI2_SetSpeed(SPI_BaudRatePrescaler_32);//SPI 32分频
	flag=TP_Read_XY(&x1,&y1);   
	if(flag==0)return(0);
	flag=TP_Read_XY(&x2,&y2);	   
	if(flag==0)return(0);
	SPI2_SetSpeed(SPI_BaudRatePrescaler_4);//SP1  4分频
	if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//前后两次采样在+-50内
	&&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
	{
		*x=(x1+x2)/2;
		*y=(y1+y2)/2;
		return 1;
	}else return 0;	  
}  
//////////////////////////////////////////////////////////////////////////////////		  
//与LCD部分有关的函数  
//画一个触摸点
//用来校准用的
//x,y:坐标
//color:颜色
void TP_Drow_Touch_Point(u16 x,u16 y,u16 color)
{
	LCD_DrawLine(x-12,y,x+13,y,color);//横线
	LCD_DrawLine(x,y-12,x,y+13,color);//竖线
	LCD_DrawPoint(x+1,y+1,color);
	LCD_DrawPoint(x-1,y+1,color);
	LCD_DrawPoint(x+1,y-1,color);
	LCD_DrawPoint(x-1,y-1,color);
	Draw_Circle(x,y,6,color);//画中心圈
}	  
//画一个大点(2*2的点)		   
//x,y:坐标
//color:颜色
void TP_Draw_Big_Point(u16 x,u16 y,u16 color)
{	    
	LCD_DrawPoint(x,y,color);//中心点 
	LCD_DrawPoint(x+1,y,color);
	LCD_DrawPoint(x,y+1,color);
	LCD_DrawPoint(x+1,y+1,color);	 	  	
}

/******************************************************************************
      函数说明：画线
      入口数据：x1,y1   起始坐标
                x2,y2   终止坐标
                color   线的颜色
      返回值：  无
******************************************************************************/
void LCD_DrawRoughLine(u16 x1,u16 y1,u16 x2,u16 y2,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1;
	uRow=x1;//画线起点坐标
	uCol=y1;
	if(delta_x>0)incx=1; //设置单步方向 
	else if (delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;//水平线 
	else {incy=-1;delta_y=-delta_y;}
	if(delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		TP_Draw_Big_Point(uRow,uCol,color);//画点
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
//触摸按键扫描
//tp:0,屏幕坐标;1,物理坐标(校准等特殊场合用)
//返回值:当前触屏状态.
//0,触屏无触摸;1,触屏有触摸
u8 TP_Scan(u8 tp)
{			   
	if(PEN==0)//有按键按下
	{
		if(tp)TP_Read_XY2(&tp_dev.x[0],&tp_dev.y[0]);//读取物理坐标
		else if(TP_Read_XY2(&tp_dev.x[0],&tp_dev.y[0]))//读取屏幕坐标
		{
	 		tp_dev.x[0]=tp_dev.xfac*tp_dev.x[0]+tp_dev.xoff;//将结果转换为屏幕坐标
			tp_dev.y[0]=tp_dev.yfac*tp_dev.y[0]+tp_dev.yoff;  
	 	} 
		if((tp_dev.sta&TP_PRES_DOWN)==0)//之前没有被按下
		{		 
			tp_dev.sta=TP_PRES_DOWN|TP_CATH_PRES;//按键按下  
			tp_dev.x[4]=tp_dev.x[0];//记录第一次按下时的坐标
			tp_dev.y[4]=tp_dev.y[0];  	   			 
		}			   
	}else
	{
		if(tp_dev.sta&TP_PRES_DOWN)//之前是被按下的
		{
			tp_dev.sta&=~(1<<7);//标记按键松开
		}else//之前就没有被按下
		{
			tp_dev.x[4]=0;
			tp_dev.y[4]=0;
			tp_dev.x[0]=0xffff;
			tp_dev.y[0]=0xffff;
		}	    
	}
	return tp_dev.sta&TP_PRES_DOWN;//返回当前的触屏状态
}
		 
//触摸屏校准代码
//得到四个校准参数
void TP_Adjust(void)
{								 
	u16 pos_temp[4][2];//坐标缓存值
	u8  cnt=0;	
	u16 d1,d2;
	u32 tem1,tem2;
	double fac; 	
 	cnt=0;				
  LCD_Fill(0,0,LCD_W,LCD_H,WHITE);//清屏   
	LCD_ShowString(5,40,"Please adjustment the screen.",RED,WHITE,16,0);//显示提示信息
	TP_Drow_Touch_Point(20,20,RED);//画点1 
	tp_dev.sta=0;//消除触发信号 
	tp_dev.xfac=0;//xfac用来标记是否校准过,所以校准之前必须清掉!以免错误	 
	while(1)//如果连续10秒钟没有按下,则自动退出
	{
		tp_dev.scan(1);//扫描物理坐标
		if((tp_dev.sta&0xc0)==TP_CATH_PRES)//按键按下了一次(此时按键松开了.)
		{		
			tp_dev.sta&=~(1<<6);//标记按键已经被处理过了.
						   			   
			pos_temp[cnt][0]=tp_dev.x[0];
			pos_temp[cnt][1]=tp_dev.y[0];
			cnt++;	  
			switch(cnt)
			{			   
				case 1:						 
					TP_Drow_Touch_Point(20,20,WHITE);				//清除点1 
					TP_Drow_Touch_Point(LCD_W-20,20,RED);	//画点2
					break;
				case 2:
 					TP_Drow_Touch_Point(LCD_W-20,20,WHITE);	//清除点2
					TP_Drow_Touch_Point(20,LCD_H-20,RED);	//画点3
					break;
				case 3:
 					TP_Drow_Touch_Point(20,LCD_H-20,WHITE);			//清除点3
 					TP_Drow_Touch_Point(LCD_W-20,LCD_H-20,RED);	//画点4
					break;
				case 4:	 //全部四个点已经得到
	    		    //对边相等
					tem1=abs(pos_temp[0][0]-pos_temp[1][0]);//x1-x2
					tem2=abs(pos_temp[0][1]-pos_temp[1][1]);//y1-y2
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);//得到1,2的距离
					
					tem1=abs(pos_temp[2][0]-pos_temp[3][0]);//x3-x4
					tem2=abs(pos_temp[2][1]-pos_temp[3][1]);//y3-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);//得到3,4的距离
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05||d1==0||d2==0)//不合格
					{
						cnt=0;
 				    TP_Drow_Touch_Point(LCD_W-20,LCD_H-20,WHITE);	//清除点4
   	 				TP_Drow_Touch_Point(20,20,RED);								//画点1
						LCD_ShowString(5,40,"Touch Adjust Filed!          ",RED,WHITE,16,0);//校正失败提示
						LCD_ShowString(5,60,"Please Adjust Again!         ",RED,WHITE,16,0);//校正失败提示
						delay_ms(1000);
 						continue;
					}
					tem1=abs(pos_temp[0][0]-pos_temp[2][0]);//x1-x3
					tem2=abs(pos_temp[0][1]-pos_temp[2][1]);//y1-y3
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);//得到1,3的距离
					
					tem1=abs(pos_temp[1][0]-pos_temp[3][0]);//x2-x4
					tem2=abs(pos_temp[1][1]-pos_temp[3][1]);//y2-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);//得到2,4的距离
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05)//不合格
					{
						cnt=0;
 				    TP_Drow_Touch_Point(LCD_W-20,LCD_H-20,WHITE);	//清除点4
   	 				TP_Drow_Touch_Point(20,20,RED);								//画点1
						LCD_ShowString(5,40,"Touch Adjust Filed!          ",RED,WHITE,16,0);//校正失败提示
						LCD_ShowString(5,60,"Please Adjust Again!         ",RED,WHITE,16,0);//校正失败提示
						delay_ms(1000);
						continue;
					}//正确了
								   
					//对角线相等
					tem1=abs(pos_temp[1][0]-pos_temp[2][0]);//x1-x3
					tem2=abs(pos_temp[1][1]-pos_temp[2][1]);//y1-y3
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);//得到1,4的距离
	
					tem1=abs(pos_temp[0][0]-pos_temp[3][0]);//x2-x4
					tem2=abs(pos_temp[0][1]-pos_temp[3][1]);//y2-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);//得到2,3的距离
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05)//不合格
					{
						cnt=0;
 				    TP_Drow_Touch_Point(LCD_W-20,LCD_H-20,WHITE);	//清除点4
   	 				TP_Drow_Touch_Point(20,20,RED);								//画点1
						LCD_ShowString(5,40,"Touch Adjust Filed!          ",RED,WHITE,16,0);//校正失败提示
						LCD_ShowString(5,60,"Please Adjust Again!         ",RED,WHITE,16,0);//校正失败提示
						delay_ms(1000);
 							continue;
					}//正确了
					//计算结果
					tp_dev.xfac=(float)(LCD_W-40)/(pos_temp[1][0]-pos_temp[0][0]);//得到xfac		 
					tp_dev.xoff=(LCD_W-tp_dev.xfac*(pos_temp[1][0]+pos_temp[0][0]))/2;//得到xoff
						  
					tp_dev.yfac=(float)(LCD_H-40)/(pos_temp[2][1]-pos_temp[0][1]);//得到yfac
					tp_dev.yoff=(LCD_H-tp_dev.yfac*(pos_temp[2][1]+pos_temp[0][1]))/2;//得到yoff  

					TP_Drow_Touch_Point(LCD_W-20,LCD_H-20,WHITE);	//清除点4
					LCD_ShowString(5,40,"Touch Screen Adjust OK!      ",RED,WHITE,16,0);//校正完成
					LCD_ShowString(5,60,"                             ",RED,WHITE,16,0);//清除文字
					delay_ms(1000);
 					LCD_ShowString(5,40,"                       ",RED,WHITE,16,0);//清除文字
					return;//校正完成				 
			}
		}
 	}
}	
//触摸屏初始化  		    
u8 TP_Init(void)
{
	u8 fac[20];
	if(Adujust)
	{
		if(USE_HORIZONTAL==0)
		{//***因触摸屏批次不同等原因，默认的校准参数值可能会引起触摸识别不准，建议校准后再使用，不建议使用固定的默认校准参数
			tp_dev.xfac=0.130623;
			tp_dev.xoff=-9;
			tp_dev.yfac=0.179869;
			tp_dev.yoff=-14;
		}else if(USE_HORIZONTAL==1)
		{//***因触摸屏批次不同等原因，默认的校准参数值可能会引起触摸识别不准，建议校准后再使用，不建议使用固定的默认校准参数
			tp_dev.xfac=-0.130450;
			tp_dev.xoff=249;
			tp_dev.yfac=-0.181218;
			tp_dev.yoff=335;
		}else if(USE_HORIZONTAL==2)
		{//***因触摸屏批次不同等原因，默认的校准参数值可能会引起触摸识别不准，建议校准后再使用，不建议使用固定的默认校准参数
			tp_dev.xfac=0.181279;
			tp_dev.xoff=-16;
			tp_dev.yfac=-0.132244;
			tp_dev.yoff=249;
		}else
		{//***因触摸屏批次不同等原因，默认的校准参数值可能会引起触摸识别不准，建议校准后再使用，不建议使用固定的默认校准参数
			tp_dev.xfac=-0.179053;
			tp_dev.xoff=333;
			tp_dev.yfac=0.130295;
			tp_dev.yoff=-11;
		}
		return 0;
	}else
	{
		TP_Adjust();//屏幕校准  
		//校准参数显示在LCD上，如果默认参数不准，请手动校正后自行修改
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

