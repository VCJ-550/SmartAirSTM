#include "fontupd.h"  
#include "exw25q128.h"   
//#include "string.h"
#include "delay.h"	 

//字库区域占用的总扇区数大小(3个字库+unigbk表+字库信息=3238700字节,约占791个W25QXX扇区)
#define FONTSECSIZE	 	791
//字库存放起始地址 
//#define FONTINFOADDR 	1024*1024*12 					//WarShip STM32F103 V3是从12M地址以后开始存放字库
														//前面12M被fatfs占用了.
														//12M以后紧跟3个字库+UNIGBK.BIN,总大小3.09M,被字库占用了,不能动!
														//15.10M以后,用户可以自由使用.建议用最后的100K字节比较好.
#define FONTINFOADDR 	0					//WarShip STM32F103 V3是从12M地址以后开始存放字库
														
//用来保存字库基本信息，地址，大小等
_font_info ftinfo;

//初始化字体
//返回值:0,字库完好.
//		 其他,字库丢失
uint8_t font_init(void)
{		
	uint8_t t=0;
//	W25QXX_Init();  
	while(t<10)//连续读取10次,都是错误,说明确实是有问题,得更新字库了
	{
		t++;
		EXW25QXX_Read((u8*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));//读出ftinfo结构体数据
		if(ftinfo.fontok==0XAA)break;
		delay_ms(20);
	}
	if(ftinfo.fontok!=0XAA)return 1;
	return 0;		    
}





























