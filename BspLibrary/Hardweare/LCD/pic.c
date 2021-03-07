#include "pic.h"
#include "lcd_init.h"
#include "show.h"
#include "lcd.h"
#include "exw25q128.h"
#include "delay.h"

MapDepotDef MapDepotStr;
 uint8_t RunPage;  //��ǰ��ҳ��

uint8_t PicBuff[640]={0}; 

//ͼ���ʼ��
uint8_t MapDepotInit()
{
  uint8_t t=0;
 	while(t<10)//������ȡ10��,���Ǵ���,˵��ȷʵ��������,�ø����ֿ���
	{
		EXW25QXX_Read((u8*)&MapDepotStr,MapDepotAddr,sizeof(MapDepotStr));//����ftinfo�ṹ������
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
		EXW25QXX_Read((u8*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));//����ftinfo�ṹ������
		if(ftinfo.fontok==0XAA)break;
		delay_ms(20);
	}
	if(ftinfo.fontok!=0XAA)return 1;
	return 0;		    
}

*/

//���ֿ��м���ͼƬ
void LCD_ShowFPicture(u16 x,u16 y,u16 length,u16 width,uint32_t addr)
{
  u16 i,j;
	u32 k=0;
	LCD_Address_Set(x,y,x+length-1,y+width-1);
	//��ȡͼ������ PicBuff 
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

//��������е�����
//ע�⣺����Ŀ�Ⱥ͸߶��ڴ���ʱҪ����
void DeleteFrame(u16 x,u16 y,u16 length,u16 width)
{
	u16 i,j,w;
	u32 k=0,t=0,t1=0,addr=0;
	uint32_t Stastrcount,EndCount;
	
	LCD_Address_Set(x,y,x+length-1,y+width-1);
	Stastrcount = y*240; //����ָ���������ĵ�����ʼλ��
	EndCount = (length*240)+Stastrcount;//����ָ���������ĵ������λ��
	w = x+length;//����Ƚ�����
	k = Stastrcount;  //ָ�������ʼλ��
	addr = MapDepotStr.mainpage+((Stastrcount)*2);
//	addr = MapDepotAddr;
	
	
	for(i=0;i<240;i++)  //�м���   320  100
	{		
		for(j=0;j<width;j++)  //240  20
		{
			if((k>=Stastrcount)&&(k<=EndCount))   //�����������
			{		  
				if(t1>=length)  //�ж�ˢ��λ��				
					t=k-((t1/length)*240)-Stastrcount;
				else
				  t=k-Stastrcount;
			
				if((t>=x)&&(t<=w)) //�����ָ��λ��
				{
					t1++;	
					EXW25QXX_Read(PicBuff,addr,2);  //���ﲻ�ǰ���˳�����е�
		      addr = MapDepotStr.mainpage+(k*2);
				  LCD_WR_DATA8(PicBuff[0]);
			    LCD_WR_DATA8(PicBuff[1]);	
				}
			 //δ�����ָ��λ�ã��޲���	
		  }
		  //��������⣬�޲���
			k++;
		}
	}
 }	

//�ڱ���ͼ����ʾһ���򣬿������ַ������������
//mode : 0 �ַ�  1��ͼƬ
//
void ShowFrame(uint16_t x,uint16_t y,\
						   uint16_t width,uint16_t height,\
							 uint16_t Zcolor,uint16_t Fcolor, \
							 u8*str,u8 size,\
							 uint8_t mode,uint8_t oper)
{
	if(oper) //��ʾ
	{	
	 if(mode) //�ַ� Show_Str(10,10,200,24,"�о�԰����",24,0);	
	 {
		//����
		LCD_DrawRectangle(x,y,x+width-1,y+height-1,Fcolor);
	  //��������
 	  LCD_Fill(x,y,width+x,height+y,Fcolor);
		//��ʾ����
		Show_Str(x,y,width+x,height+y,str,size,1);
	 } 
	 else  //ͼƬ
	 {
    //LCD_ShowPicture(0,0,240,320,gImage_ymd);
	 }	
 }
 else  //ɾ��	
		DeleteFrame(x,y,width,height);
}
