#ifndef __PIC_H_
#define __PIC_H_
#include "sys.h"

#define MapDepotAddr   0x00317001//图库位置

 extern uint8_t RunPage;  //当前的页面

__packed typedef struct MapDepot
{
  uint8_t Depotok;				//存在标志，0XBB:正常；其他，不存在
	uint32_t mainpage; 			//主页面的地址
	uint32_t mainpagesize;			//主页面的大小	 
	uint32_t xqaddr;			//详情页地址	
	uint32_t xqsize;			//详情页大小	 
	uint32_t zyaddr;			//主页地址
	uint32_t zysize;			//主页大小		 
	uint32_t szaddr;			//设置页地址
	uint32_t szsize;			//设置页大小
	uint32_t xxddr;			  //信息页地址
	uint32_t xxsize;			//信息页大小
}MapDepotDef;

extern MapDepotDef MapDepotStr;

uint8_t MapDepotInit(void);
void LCD_ShowFPicture(u16 x,u16 y,u16 length,u16 width,uint32_t addr);
//在背景图上显示一个框，可以是字符，可以是填充
void ShowFrame(uint16_t x,uint16_t y,\
						   uint16_t width,uint16_t height,\
							 uint16_t Zcolor,uint16_t Fcolor, \
							 u8*str,u8 size,\
							 uint8_t mode,uint8_t oper);

#endif

