#include "bord.h"
#include "show.h"
#include "wifi.h"
#include "app.h"
#include "pic.h"

u8 datatemp[32];
u8 linkok=0;

 int main(void)
 {	
	u16 lastpos[2];//最后一次的数据 
	delay_init();	    	 //启动滴答定时器	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
  BordHardweardInit();  //板级硬件初始化
////	wifi_protocol_init(); //协议初始化
//	 mcu_reset_wifi();  //重置WIFI
//	 delay_ms(500);
//	 mcu_set_wifi_mode(0);  //设置为smartconfig配网
	lastpos[0]=0XFFFF;
	LED=1;
	  while(font_init()) //字库检查
		{
	    LCD_ShowString(0,70,"There is no word stock!",RED,WHITE,16,0); 
			delay_ms(200);
			LCD_Fill(30,70,200+30,70+16,WHITE);
			delay_ms(200);		    
	  }
		
		while(MapDepotInit()) //图库检查
		{
		delay_ms(200);  //图库错误
			LED =!LED;
		}
 Show_Welcome();   //显示欢迎页面
		
//	mcu_get_wifi_connect_status();  //获取WIFI状态
////	ShowWifiPage(1);
	 POINT_COLOR=RED;
//	Show_Str(10,40,200,16,"初始化成功！",16,1);	 
	LCD_ShowString(10,80,"There!",RED,WHITE,16,1);
//  mcu_start_wifitest();
	
//	ShowFrame(10,120,200,300,YELLOW,YELLOW,"SYSTEM",16,1,1);
//  delay_ms(1000);
//	ShowFrame(10,120,200,300,YELLOW,YELLOW,"SYSTEM",16,1,0);
		delay_ms(1000);
    LCD_ShowFPicture(0,22,240,276,MapDepotStr.xqaddr); //详情页		
		
	while(1)
	{		
//		mcu_reset_wifi();  //重置WIFI
//		 
//		if(KEY_Scan())
//		{
//		
//创建一个框
//////	ShowFrame(0,22,240,276,YELLOW,YELLOW,"SYSTEM",16,1,1);
//////  delay_ms(1000);
//////	ShowFrame(0,22,240,300,YELLOW,YELLOW,"SYSTEM",16,1,0); 
//////  delay_ms(1000);		
		
		//显示详情页面
//////////////		LCD_ShowFPicture(0,22,240,276,MapDepotStr.xqaddr); //详情页
//////////////		delay_ms(500);		
//////////////		LCD_ShowFPicture(0,22,240,276,MapDepotStr.zyaddr); //主页
//////////////		delay_ms(500);		
//////////////		LCD_ShowFPicture(0,22,240,276,MapDepotStr.szaddr); //设置页
//////////////		delay_ms(500);		
//////////////		LCD_ShowFPicture(0,22,240,276,MapDepotStr.xxddr); //信息页
//////////////		delay_ms(500);
//////////////		 LED =!LED;
////    if(mcu_get_reset_wifi_flag())			
////	    mcu_set_wifi_mode(0);  //设置为smartconfig配网		
//		}
////		wifi_uart_service();//轮询是否有数据接收
////    mcu_get_wifi_connect_status();  //获取WIFI状态
		
////		if(KEY_Scan())
////		{
////    mcu_reset_wifi();  //重置WIFI
////    mcu_set_wifi_mode(0);  //设置为smartconfig配网	
////			ShowWifiPage(1);
////			linkok=0;
////		}			
//// if((TuyaWifiInforStr.WifiRunMode == 0x04)&&(!linkok))  //连接到涂鸦
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
		
//		mcu_get_green_time();   //获取格林时间	
	

//	LCD_ShowString(10,LCD_H-40,"X:",RED,WHITE,16,0);
//	LCD_ShowIntNum(26,LCD_H-40,0,3,RED,WHITE,16);
//	LCD_ShowString(10,LCD_H-20,"Y:",RED,WHITE,16,0);
//	LCD_ShowIntNum(26,LCD_H-20,0,3,RED,WHITE,16);
//	while(1)
//	{
		QuerySensorData(); //查询传感器数据
		ReadSensorData();  //处理传感器数据
 		tp_dev.scan(0);//扫描
		if(tp_dev.sta&TP_PRES_DOWN)//有按键被按下
		{	
//      LED =!LED;			
			delay_ms(1);//必要的延时,否则老认为有按键按下.		    
			if((tp_dev.x[0]<(LCD_W-1)&&tp_dev.x[0]>=1)&&(tp_dev.y[0]<(LCD_H-1)&&tp_dev.y[0]>=1))
			{			 
//				if(lastpos[0]==0XFFFF)
//				{
//					lastpos[0]=tp_dev.x[0];
//					lastpos[1]=tp_dev.y[0];
//				}
//				LCD_DrawRoughLine(lastpos[0],lastpos[1],tp_dev.x[0],tp_dev.y[0],BLUE);//画线
				if((tp_dev.y[0]>22)&&(tp_dev.y[0]<62))
				{
					if((tp_dev.x[0]>10)&&(tp_dev.x[0]<60)) 
						LCD_ShowFPicture(0,22,240,276,MapDepotStr.xqaddr); //详情页
					if((tp_dev.x[0]>80)&&(tp_dev.x[0]<120)) 
						LCD_ShowFPicture(0,22,240,276,MapDepotStr.zyaddr); //主页
					if((tp_dev.x[0]>140)&&(tp_dev.x[0]<170)) 
						LCD_ShowFPicture(0,22,240,276,MapDepotStr.szaddr); //设置页
					if((tp_dev.x[0]>190)&&(tp_dev.x[0]<230)) 
						LCD_ShowFPicture(0,22,240,276,MapDepotStr.xxddr); //信息页
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

