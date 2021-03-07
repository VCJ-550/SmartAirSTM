#include "show.h"
#include "lcd.h"
#include "pic.h"
#include "lcd_init.h"
#include "delay.h"
#include "Sensor.h"
#include "app.h"
#include "pc.h"

//ShowTidDef ShowTidStr;

uint8_t strt[4096];

//清全屏
//默认为背景色
void LCD_Clear()
{
 LCD_Fill(0,0,LCD_W,LCD_H,BACK_COLOR);
}
	
//显示欢迎页面
void Show_Welcome(void)
{
 //显示字符
//	uint8_t i,j;
	float t=0;
	POINT_COLOR=RED;
	BACK_COLOR = YELLOW;
  Show_Str(10,10,200,24,"中景园电子",24,0);	
	BACK_COLOR = WHITE;
	LCD_ShowString(0,40,"LCD_W:",RED,WHITE,16,0);
	LCD_ShowIntNum(48,40,LCD_W,3,RED,WHITE,16);
	LCD_ShowString(80,40,"LCD_H:",RED,WHITE,16,0);
	LCD_ShowIntNum(128,40,LCD_H,3,RED,WHITE,16);
	LCD_ShowString(80,40,"LCD_H:",RED,WHITE,16,0);
	LCD_ShowString(0,70,"Increaseing Nun:",RED,WHITE,16,0);
	LCD_ShowFloatNum1(128,70,t,4,RED,WHITE,16);
	t+=0.11;
//	for(j=0;j<5;j++)
//	{
//		for(i=0;i<6;i++)
//		{
//			LCD_ShowPicture(40*i,120+j*40,40,40,gImage_1);
//		}
//	}
//	delay_ms(1000);
//	LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	//显示图片
// 	LCD_ShowPicture(0,0,240,320,gImage_ymd);
//////	LCD_ShowPicture(0,0,240,320,gImage_pc);
 LCD_ShowFPicture(0,0,240,320,MapDepotStr.mainpage);
//	delay_ms(500);
//		LCD_Clear();
}

void ShowSensorPage(uint8_t tid)
{
 if(tid)  //需要加载界面
 {
	 LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	 LCD_ShowString(10,16,"CO2:     ppm",RED,WHITE,16,1);
   LCD_ShowString(10,32,"Tem:      C",RED,WHITE,16,1);
   LCD_ShowString(10,48,"Hum:       %",RED,WHITE,16,1);
   LCD_ShowString(10,64,"Light:     lx",RED,WHITE,16,1);
	 LCD_ShowString(10,80,"Noise:      V",RED,WHITE,16,1);
   LCD_ShowString(10,96,"CH4O:     ug/m3",RED,WHITE,16,1);
   LCD_ShowString(10,112,"PM1C:     ug/m3",RED,WHITE,16,1); 
   LCD_ShowString(10,128,"PM2.5C:     ug/m3",RED,WHITE,16,1);  
	 LCD_ShowString(10,144,"PM10C:     ug/m3",RED,WHITE,16,1);	 
   LCD_ShowString(10,160,"PM1:     ug/m3",RED,WHITE,16,1); 
   LCD_ShowString(10,176,"PM2.5:     ug/m3",RED,WHITE,16,1);  
	 LCD_ShowString(10,192,"PM10:     ug/m3",RED,WHITE,16,1); 
   LCD_ShowString(10,208,"PM0.3U:     pcs",RED,WHITE,16,1); 
   LCD_ShowString(10,224,"PM0.5U:     pcs",RED,WHITE,16,1);   
	 LCD_ShowString(10,240,"PM1.0U:     pcs",RED,WHITE,16,1); 
   LCD_ShowString(10,256,"PM2.5U:     pcs",RED,WHITE,16,1); 
   LCD_ShowString(10,272,"PM5.0U:     pcs",RED,WHITE,16,1);   
	 LCD_ShowString(10,288,"PM10U:     pcs",RED,WHITE,16,1); 
	 LCD_ShowString(10,304,"BTVot:     V",RED,WHITE,16,1); 
 }
 
 LCD_ShowIntNum(42,16,SensorDataStr.CO2Value,4,RED,WHITE,16);
 LCD_ShowDecimalsNum1(42,32,SensorDataStr.HDCStr.Temperature,2,2,RED,WHITE,16);
 LCD_ShowDecimalsNum1(42,48,SensorDataStr.HDCStr.Humidity,2,2,RED,WHITE,16);
 LCD_ShowIntNum(58,64,SensorDataStr.Light,4,RED,WHITE,16);
 LCD_ShowINntDecNum1(58,80,SensorDataStr.Noise,2,2,RED,WHITE,16); 
 LCD_ShowIntNum(50,96,SensorDataStr.WZSStr.WZSCHValue,4,RED,WHITE,16);
 LCD_ShowIntNum(50,112,SensorDataStr.PMSStr.PM1CFValue,4,RED,WHITE,16);
 LCD_ShowIntNum(74,128,SensorDataStr.PMSStr.PM2_5CFValue,4,RED,WHITE,16); 
 LCD_ShowIntNum(58,144,SensorDataStr.PMSStr.PM10Value,4,RED,WHITE,16); 
 LCD_ShowIntNum(50,160,SensorDataStr.PMSStr.PM1Value,4,RED,WHITE,16);
 LCD_ShowIntNum(58,176,SensorDataStr.PMSStr.PM2_5Value,4,RED,WHITE,16); 
 LCD_ShowIntNum(50,192,SensorDataStr.PMSStr.PM10Value,4,RED,WHITE,16);
 LCD_ShowIntNum(74,208,SensorDataStr.PMSStr.PM0_3umValue,4,RED,WHITE,16);
 LCD_ShowIntNum(74,224,SensorDataStr.PMSStr.PM0_5umValue,4,RED,WHITE,16);
 LCD_ShowIntNum(74,240,SensorDataStr.PMSStr.PM1_0umValue,4,RED,WHITE,16);
 LCD_ShowIntNum(74,256,SensorDataStr.PMSStr.PM2_5umValue,4,RED,WHITE,16);
 LCD_ShowIntNum(74,272,SensorDataStr.PMSStr.PM5_0umValue,4,RED,WHITE,16);
 LCD_ShowIntNum(58,288,SensorDataStr.PMSStr.PM10umValue,4,RED,WHITE,16);
 LCD_ShowINntDecNum1(58,304,SensorDataStr.BTVotValue,2,2,RED,WHITE,16);
}

void ShowWifiPage(uint8_t tid)
{
	
	if(tid)
	{
		LCD_Fill(0,0,LCD_W,LCD_H,WHITE);
	  LCD_ShowString(10,16,"Wifi Mode:",RED,WHITE,16,0); 	
	}
	LCD_ShowString(90,32,"              ",RED,WHITE,16,0);
	switch(TuyaWifiInforStr.WifiRunMode)
	{
        case 0:
          LCD_ShowString(90,32,"smartconfig",RED,WHITE,16,0); 	  //wifi工作状态1
        break;
      
        case 1:
            LCD_ShowString(90,32,"AP ",RED,WHITE,16,0);//wifi工作状态2
        break;
        
        case 2:
            LCD_ShowString(90,32,"WIFI no link !",RED,WHITE,16,0);//wifi工作状态3
        break;
        
        case 3:
           LCD_ShowString(90,32,"WIFI to link !",RED,WHITE,16,0); //wifi工作状态4
        break;
        
        case 4:
           LCD_ShowString(90,32,"link to tuya!",RED,WHITE,16,0); //wifi工作状态5
        break;
        
        case 5:
           LCD_ShowString(90,32,"WIFI Low",RED,WHITE,16,0); //wifi工作状态6
        break;
      
        case 6:
           LCD_ShowString(90,32,"smartconfig&AP",RED,WHITE,16,0); //wifi工作状态7
        break;
        
        default:break;
	}
}

//显示主页
void ShowHomePage(void)
{
  //时间
	
	//天气
	
	//WiFi状态

  //图标数据
}



	



