#include "Sensor.h"
#include "usart.h"
#include "delay.h"
#include "hdc.h"
#include "bh1750.h"
#include "adc.h"

SensorDataDef SensorDataStr;

uint8_t CO2ReadBuff[8]={0x15,0x04,0x13,0x8B,0x00,0x01,0x46,0x70};   //CO2传感器查询命令
uint8_t ReadBuff[9]={0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79}; //粉尘传感器查询命令
uint16_t adcx;

union PMSDataValue
{
  uint16_t PMSValue;
	uint8_t PMSValueBuff[2];
}PMSDataValueUN;


void SensorPMSInit(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	  
 //SET-->PC1   RST->PC2  电源控制->PC0
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_0;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOC, &GPIO_InitStructure);		 
 UART5_Init(9600);	
 PMSSET_SET; 
 PMSRST_SET; 
 PMSPower_ON; 
}

void SensorWZSInit(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	  
 //电源控制->PA11
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOA, &GPIO_InitStructure);	
 uart_init(9600);	 	//串口初始化为	
 WZSPower_ON;
 USART1_RX_STA = RESET;
}

void SensorCO2Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	  
 //电源控制->PC9
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOC, &GPIO_InitStructure);	
 usart2_init(19200);	 	//串口初始化为19200	
 CO2Power_ON;    //开启供电
 USART2_RX_STA = RESET;
}

void ReadSensorData(void)
{	
  //PMS
	if(UART5_RX_STA&0x8000)
	{
	  UART5_RX_STA = RESET;
    USART_ITConfig(UART5, USART_IT_RXNE, DISABLE);//关中断 		
		PMSDataValueUN.PMSValueBuff[1]=PMSReadDataBuff[4];
		PMSDataValueUN.PMSValueBuff[0]=PMSReadDataBuff[5];
		SensorDataStr.PMSStr.PM1CFValue = PMSDataValueUN.PMSValue; //PM1.0 浓度（CF=1，标准颗粒物）		
		PMSDataValueUN.PMSValueBuff[1]=PMSReadDataBuff[6];
		PMSDataValueUN.PMSValueBuff[0]=PMSReadDataBuff[7];
		SensorDataStr.PMSStr.PM2_5CFValue = PMSDataValueUN.PMSValue;//PM2.5 浓度（CF=1，标准颗粒物）		
		PMSDataValueUN.PMSValueBuff[1]=PMSReadDataBuff[8];
		PMSDataValueUN.PMSValueBuff[0]=PMSReadDataBuff[9];
		SensorDataStr.PMSStr.PM10CFValue = PMSDataValueUN.PMSValue; //PM10 浓度（CF=1，标准颗粒物）				
		PMSDataValueUN.PMSValueBuff[1]=PMSReadDataBuff[10];
		PMSDataValueUN.PMSValueBuff[0]=PMSReadDataBuff[11];
		SensorDataStr.PMSStr.PM1Value = PMSDataValueUN.PMSValue;//PM1.0 浓度（大气环境下）		
		PMSDataValueUN.PMSValueBuff[1]=PMSReadDataBuff[12];
		PMSDataValueUN.PMSValueBuff[0]=PMSReadDataBuff[13];
		SensorDataStr.PMSStr.PM2_5Value = PMSDataValueUN.PMSValue;//PM2.5 浓度（大气环境下）		
		PMSDataValueUN.PMSValueBuff[1]=PMSReadDataBuff[14];
		PMSDataValueUN.PMSValueBuff[0]=PMSReadDataBuff[15];
		SensorDataStr.PMSStr.PM10Value = PMSDataValueUN.PMSValue;//PM10 浓度（大气环境下）		
		PMSDataValueUN.PMSValueBuff[1]=PMSReadDataBuff[16];
		PMSDataValueUN.PMSValueBuff[0]=PMSReadDataBuff[17];
		SensorDataStr.PMSStr.PM0_3umValue = PMSDataValueUN.PMSValue; //0.1 升空气中直径在 0.3um 以上颗粒物个数
		PMSDataValueUN.PMSValueBuff[1]=PMSReadDataBuff[18];
		PMSDataValueUN.PMSValueBuff[0]=PMSReadDataBuff[19];
		SensorDataStr.PMSStr.PM0_5umValue = PMSDataValueUN.PMSValue; 		//0.1 升空气中直径在 0.5um 以上颗粒物个数
		PMSDataValueUN.PMSValueBuff[1]=PMSReadDataBuff[20];
		PMSDataValueUN.PMSValueBuff[0]=PMSReadDataBuff[21];
		SensorDataStr.PMSStr.PM1_0umValue = PMSDataValueUN.PMSValue; 		//0.1 升空气中直径在 1.0um 以上颗粒物个数
		PMSDataValueUN.PMSValueBuff[1]=PMSReadDataBuff[22];
		PMSDataValueUN.PMSValueBuff[0]=PMSReadDataBuff[23];
		SensorDataStr.PMSStr.PM2_5umValue = PMSDataValueUN.PMSValue; 		//0.1 升空气中直径在 2.5um 以上颗粒物个数
		PMSDataValueUN.PMSValueBuff[1]=PMSReadDataBuff[24];
		PMSDataValueUN.PMSValueBuff[0]=PMSReadDataBuff[25];
		SensorDataStr.PMSStr.PM5_0umValue = PMSDataValueUN.PMSValue; 		//0.1 升空气中直径在 5.0um 以上颗粒物个数
		PMSDataValueUN.PMSValueBuff[1]=PMSReadDataBuff[26];
		PMSDataValueUN.PMSValueBuff[0]=PMSReadDataBuff[27];
		SensorDataStr.PMSStr.PM10umValue = PMSDataValueUN.PMSValue;		//0.1 升空气中直径在 10um 以上颗粒物个数		
		SensorDataStr.PMSStr.PMVersion = PMSReadDataBuff[28]; //版本号		
		SensorDataStr.PMSStr.PMErrorCode = PMSReadDataBuff[29]; //错误代码
	}	
	
 //WZS
 if(USART1_RX_STA&0x8000)
 {
		USART1_RX_STA = RESET;				
		PMSDataValueUN.PMSValueBuff[1]=WZSReadDataBuff[4];
		PMSDataValueUN.PMSValueBuff[0]=WZSReadDataBuff[5];
	  SensorDataStr.WZSStr.WZSCHValue = PMSDataValueUN.PMSValue;//甲醛气体浓度		
		PMSDataValueUN.PMSValueBuff[1]=WZSReadDataBuff[6];
		PMSDataValueUN.PMSValueBuff[0]=WZSReadDataBuff[7];
	  SensorDataStr.WZSStr.WZSCHRangeValue = PMSDataValueUN.PMSValue;//甲醛气体满量程	
	  SensorDataStr.WZSStr.WZSCHDecValue = WZSReadDataBuff[3];//小数位数
 }
 
	if(USART2_RX_STA&0x8000)
	{
		USART2_RX_STA = RESET;				
		PMSDataValueUN.PMSValueBuff[1]=CO2ReadDataBuff[3];
		PMSDataValueUN.PMSValueBuff[0]=CO2ReadDataBuff[4];
	  SensorDataStr.CO2Value = PMSDataValueUN.PMSValue;  //CO2气体浓度
	}
}

//传感器数据请求
void QuerySensorData(void)
{  
//	 USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//开启串口接受中断
//	 //WZS
//   USART_SendDataBuff(USART1, ReadBuff,9);   	
//	 delay_ms(100);		
	 //CO2
   USART_SendDataBuff( USART2, CO2ReadBuff,8);
	//HDC1080
	SensorDataStr.HDCStr.Temperature = HDC1080_Read_Temper(); 
	SensorDataStr.HDCStr.Humidity=HDC1080_Read_Humidi();
	//BH1750
	SensorDataStr.Light = Get_Bh_Value();
	//噪声
	adcx=Get_Adc_Average(ADC_Channel_4,10); 
	SensorDataStr.Noise = adcx*0.0008;
	//电池电压 
		adcx=Get_Adc_Average(ADC_Channel_1,10); 
		SensorDataStr.BTVotValue = adcx*0.0008;
}




