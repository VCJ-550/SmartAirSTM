#include "bord.h"
#include "show.h"
#include "wifi.h"
#include "app.h"
#include "pic.h"

u8 datatemp[32];
u8 linkok=0;

 int main(void)
 {	
	u16 lastpos[2];//���һ�ε����� 
	delay_init();	    	 //�����δ�ʱ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
  BordHardweardInit();  //�弶Ӳ����ʼ��
////	wifi_protocol_init(); //Э���ʼ��
//	 mcu_reset_wifi();  //����WIFI
//	 delay_ms(500);
//	 mcu_set_wifi_mode(0);  //����Ϊsmartconfig����
	lastpos[0]=0XFFFF;
	LED=1;
	  while(font_init()) //�ֿ���
		{
	    LCD_ShowString(0,70,"There is no word stock!",RED,WHITE,16,0); 
			delay_ms(200);
			LCD_Fill(30,70,200+30,70+16,WHITE);
			delay_ms(200);		    
	  }
		
		while(MapDepotInit()) //ͼ����
		{
		delay_ms(200);  //ͼ�����
			LED =!LED;
		}
 Show_Welcome();   //��ʾ��ӭҳ��
		
//	mcu_get_wifi_connect_status();  //��ȡWIFI״̬
////	ShowWifiPage(1);
	 POINT_COLOR=RED;
//	Show_Str(10,40,200,16,"��ʼ���ɹ���",16,1);	 
	LCD_ShowString(10,80,"There!",RED,WHITE,16,1);
//  mcu_start_wifitest();
	
//	ShowFrame(10,120,200,300,YELLOW,YELLOW,"SYSTEM",16,1,1);
//  delay_ms(1000);
//	ShowFrame(10,120,200,300,YELLOW,YELLOW,"SYSTEM",16,1,0);
		delay_ms(1000);
    LCD_ShowFPicture(0,22,240,276,MapDepotStr.xqaddr); //����ҳ		
		
	while(1)
	{		
//		mcu_reset_wifi();  //����WIFI
//		 
//		if(KEY_Scan())
//		{
//		
//����һ����
//////	ShowFrame(0,22,240,276,YELLOW,YELLOW,"SYSTEM",16,1,1);
//////  delay_ms(1000);
//////	ShowFrame(0,22,240,300,YELLOW,YELLOW,"SYSTEM",16,1,0); 
//////  delay_ms(1000);		
		
		//��ʾ����ҳ��
//////////////		LCD_ShowFPicture(0,22,240,276,MapDepotStr.xqaddr); //����ҳ
//////////////		delay_ms(500);		
//////////////		LCD_ShowFPicture(0,22,240,276,MapDepotStr.zyaddr); //��ҳ
//////////////		delay_ms(500);		
//////////////		LCD_ShowFPicture(0,22,240,276,MapDepotStr.szaddr); //����ҳ
//////////////		delay_ms(500);		
//////////////		LCD_ShowFPicture(0,22,240,276,MapDepotStr.xxddr); //��Ϣҳ
//////////////		delay_ms(500);
//////////////		 LED =!LED;
////    if(mcu_get_reset_wifi_flag())			
////	    mcu_set_wifi_mode(0);  //����Ϊsmartconfig����		
//		}
////		wifi_uart_service();//��ѯ�Ƿ������ݽ���
////    mcu_get_wifi_connect_status();  //��ȡWIFI״̬
		
////		if(KEY_Scan())
////		{
////    mcu_reset_wifi();  //����WIFI
////    mcu_set_wifi_mode(0);  //����Ϊsmartconfig����	
////			ShowWifiPage(1);
////			linkok=0;
////		}			
//// if((TuyaWifiInforStr.WifiRunMode == 0x04)&&(!linkok))  //���ӵ�Ϳѻ
//// {
////    linkok = 1;
////	  ShowSensorPage(1); 
//// } else
//// {
////// ShowWifiPage(0);
//// 
//// }
//// 
//// if(linkok)
//// {
////	 QuerySensorData();	
////	 ReadSensorData();	
////	 ShowSensorPage(0);
//// }else
//// {
////	 ShowWifiPage(0);
//// }
//		
		
//		mcu_get_green_time();   //��ȡ����ʱ��	
	

//	LCD_ShowString(10,LCD_H-40,"X:",RED,WHITE,16,0);
//	LCD_ShowIntNum(26,LCD_H-40,0,3,RED,WHITE,16);
//	LCD_ShowString(10,LCD_H-20,"Y:",RED,WHITE,16,0);
//	LCD_ShowIntNum(26,LCD_H-20,0,3,RED,WHITE,16);
//	while(1)
//	{
		QuerySensorData(); //��ѯ����������
		ReadSensorData();  //������������
 		tp_dev.scan(0);//ɨ��
		if(tp_dev.sta&TP_PRES_DOWN)//�а���������
		{	
//      LED =!LED;			
			delay_ms(1);//��Ҫ����ʱ,��������Ϊ�а�������.		    
			if((tp_dev.x[0]<(LCD_W-1)&&tp_dev.x[0]>=1)&&(tp_dev.y[0]<(LCD_H-1)&&tp_dev.y[0]>=1))
			{			 
//				if(lastpos[0]==0XFFFF)
//				{
//					lastpos[0]=tp_dev.x[0];
//					lastpos[1]=tp_dev.y[0];
//				}
//				LCD_DrawRoughLine(lastpos[0],lastpos[1],tp_dev.x[0],tp_dev.y[0],BLUE);//����
				if((tp_dev.y[0]>22)&&(tp_dev.y[0]<62))
				{
					if((tp_dev.x[0]>10)&&(tp_dev.x[0]<60)) 
						LCD_ShowFPicture(0,22,240,276,MapDepotStr.xqaddr); //����ҳ
					if((tp_dev.x[0]>80)&&(tp_dev.x[0]<120)) 
						LCD_ShowFPicture(0,22,240,276,MapDepotStr.zyaddr); //��ҳ
					if((tp_dev.x[0]>140)&&(tp_dev.x[0]<170)) 
						LCD_ShowFPicture(0,22,240,276,MapDepotStr.szaddr); //����ҳ
					if((tp_dev.x[0]>190)&&(tp_dev.x[0]<230)) 
						LCD_ShowFPicture(0,22,240,276,MapDepotStr.xxddr); //��Ϣҳ
//				lastpos[0]=tp_dev.x[0];
//				lastpos[1]=tp_dev.y[0];
//				LCD_ShowString(10,LCD_H-40,"X:",RED,WHITE,16,0);
//				LCD_ShowIntNum(26,LCD_H-40,tp_dev.x[0],3,RED,WHITE,16);
//				LCD_ShowString(10,LCD_H-20,"Y:",RED,WHITE,16,0);
//				LCD_ShowIntNum(26,LCD_H-20,tp_dev.y[0],3,RED,WHITE,16);
				}
			}    
		}
		
//	}
		 
	 } //while end	
 }//main end

