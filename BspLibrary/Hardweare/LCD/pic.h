#ifndef __PIC_H_
#define __PIC_H_
#include "sys.h"

#define MapDepotAddr   0x00317001//ͼ��λ��

 extern uint8_t RunPage;  //��ǰ��ҳ��

__packed typedef struct MapDepot
{
  uint8_t Depotok;				//���ڱ�־��0XBB:������������������
	uint32_t mainpage; 			//��ҳ��ĵ�ַ
	uint32_t mainpagesize;			//��ҳ��Ĵ�С	 
	uint32_t xqaddr;			//����ҳ��ַ	
	uint32_t xqsize;			//����ҳ��С	 
	uint32_t zyaddr;			//��ҳ��ַ
	uint32_t zysize;			//��ҳ��С		 
	uint32_t szaddr;			//����ҳ��ַ
	uint32_t szsize;			//����ҳ��С
	uint32_t xxddr;			  //��Ϣҳ��ַ
	uint32_t xxsize;			//��Ϣҳ��С
}MapDepotDef;

extern MapDepotDef MapDepotStr;

uint8_t MapDepotInit(void);
void LCD_ShowFPicture(u16 x,u16 y,u16 length,u16 width,uint32_t addr);
//�ڱ���ͼ����ʾһ���򣬿������ַ������������
void ShowFrame(uint16_t x,uint16_t y,\
						   uint16_t width,uint16_t height,\
							 uint16_t Zcolor,uint16_t Fcolor, \
							 u8*str,u8 size,\
							 uint8_t mode,uint8_t oper);

#endif

