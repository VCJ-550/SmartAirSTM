#include "fontupd.h"  
#include "exw25q128.h"   
//#include "string.h"
#include "delay.h"	 

//�ֿ�����ռ�õ�����������С(3���ֿ�+unigbk��+�ֿ���Ϣ=3238700�ֽ�,Լռ791��W25QXX����)
#define FONTSECSIZE	 	791
//�ֿ�����ʼ��ַ 
//#define FONTINFOADDR 	1024*1024*12 					//WarShip STM32F103 V3�Ǵ�12M��ַ�Ժ�ʼ����ֿ�
														//ǰ��12M��fatfsռ����.
														//12M�Ժ����3���ֿ�+UNIGBK.BIN,�ܴ�С3.09M,���ֿ�ռ����,���ܶ�!
														//15.10M�Ժ�,�û���������ʹ��.����������100K�ֽڱȽϺ�.
#define FONTINFOADDR 	0					//WarShip STM32F103 V3�Ǵ�12M��ַ�Ժ�ʼ����ֿ�
														
//���������ֿ������Ϣ����ַ����С��
_font_info ftinfo;

//��ʼ������
//����ֵ:0,�ֿ����.
//		 ����,�ֿⶪʧ
uint8_t font_init(void)
{		
	uint8_t t=0;
//	W25QXX_Init();  
	while(t<10)//������ȡ10��,���Ǵ���,˵��ȷʵ��������,�ø����ֿ���
	{
		t++;
		EXW25QXX_Read((u8*)&ftinfo,FONTINFOADDR,sizeof(ftinfo));//����ftinfo�ṹ������
		if(ftinfo.fontok==0XAA)break;
		delay_ms(20);
	}
	if(ftinfo.fontok!=0XAA)return 1;
	return 0;		    
}





























