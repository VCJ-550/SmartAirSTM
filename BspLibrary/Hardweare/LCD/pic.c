#include "pic.h"
#include "lcd_init.h"
#include "show.h"
#include "lcd.h"
#include "exw25q128.h"
#include "delay.h"

MapDepotDef MapDepotStr;
 uint8_t RunPage;  //当前的页面

uint8_t PicBuff[640]={0}; 

//图库初始化
uint8_t MapDepotInit()
{
  uint8_t t=0;
 	while(t<10)//连续读取10次,都是错误,说明确实是有问题,得更新字库了
	{
		EXW25QXX_Read((u8*)&MapDepotStr,MapDepotAddr,sizeof(MapDepotStr));//读出ftinfo结构体数据
		if(MapDepotStr.Depotok==0XBB)break;
		delay_ms(20);
	}
	if(MapDepotStr.Depotok!=0XBB)return 1;
	return 0;	
}

/*
 font_init(void)
{		

//	W25QXX_Init();  

		t++;
		EXW25QXX_Read((u8*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));//读出ftinfo结构体数据
		if(ftinfo.fontok==0XAA)break;
		delay_ms(20);
	}
	if(ftinfo.fontok!=0XAA)return 1;
	return 0;		    
}

*/

//从字库中加载图片
void LCD_ShowFPicture(u16 x,u16 y,u16 length,u16 width,uint32_t addr)
{
  u16 i,j;
	u32 k=0;
	LCD_Address_Set(x,y,x+length-1,y+width-1);
	//读取图库数据 PicBuff 
		for(i=0;i<length;i++)
	{
		EXW25QXX_Read(PicBuff,addr,width*2);
		addr = addr+(width*2);
		for(j=0;j<width;j++)
		{	    
			LCD_WR_DATA8(PicBuff[k*2]);
			LCD_WR_DATA8(PicBuff[k*2+1]);
		  k++;
		}
		k=0;
	}
}

//读点阵库中的数据
//注意：这里的宽度和高度在传入时要交换
void DeleteFrame(u16 x,u16 y,u16 length,u16 width)
{
	u16 i,j,w;
	u32 k=0,t=0,t1=0,addr=0;
	uint32_t Stastrcount,EndCount;
	
	LCD_Address_Set(x,y,x+length-1,y+width-1);
	Stastrcount = y*240; //计算指定填充区域的点阵起始位置
	EndCount = (length*240)+Stastrcount;//计算指定填充区域的点阵结束位置
	w = x+length;//填充宽度结束字
	k = Stastrcount;  //指向点阵起始位置
	addr = MapDepotStr.mainpage+((Stastrcount)*2);
//	addr = MapDepotAddr;
	
	
	for(i=0;i<240;i++)  //列计数   320  100
	{		
		for(j=0;j<width;j++)  //240  20
		{
			if((k>=Stastrcount)&&(k<=EndCount))   //处于填充区域
			{		  
				if(t1>=length)  //判断刷新位置				
					t=k-((t1/length)*240)-Stastrcount;
				else
				  t=k-Stastrcount;
			
				if((t>=x)&&(t<=w)) //到填充指定位置
				{
					t1++;	
					EXW25QXX_Read(PicBuff,addr,2);  //这里不是按照顺序排列的
		      addr = MapDepotStr.mainpage+(k*2);
				  LCD_WR_DATA8(PicBuff[0]);
			    LCD_WR_DATA8(PicBuff[1]);	
				}
			 //未到填充指定位置，无操作	
		  }
		  //填充区域外，无操作
			k++;
		}
	}
 }	

//在背景图上显示一个框，可以是字符，可以是填充
//mode : 0 字符  1：图片
//
void ShowFrame(uint16_t x,uint16_t y,\
						   uint16_t width,uint16_t height,\
							 uint16_t Zcolor,uint16_t Fcolor, \
							 u8*str,u8 size,\
							 uint8_t mode,uint8_t oper)
{
	if(oper) //显示
	{	
	 if(mode) //字符 Show_Str(10,10,200,24,"中景园电子",24,0);	
	 {
		//画框
		LCD_DrawRectangle(x,y,x+width-1,y+height-1,Fcolor);
	  //填充这个框
 	  LCD_Fill(x,y,width+x,height+y,Fcolor);
		//显示内容
		Show_Str(x,y,width+x,height+y,str,size,1);
	 } 
	 else  //图片
	 {
    //LCD_ShowPicture(0,0,240,320,gImage_ymd);
	 }	
 }
 else  //删除	
		DeleteFrame(x,y,width,height);
}
