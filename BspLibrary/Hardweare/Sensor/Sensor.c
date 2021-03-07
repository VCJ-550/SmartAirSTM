#include "Sensor.h"
#include "usart.h"
#include "delay.h"
#include "hdc.h"
#include "bh1750.h"
#include "adc.h"

SensorDataDef SensorDataStr;

uint8_t CO2ReadBuff[8]={0x15,0x04,0x13,0x8B,0x00,0x01,0x46,0x70};   //CO2��������ѯ����
uint8_t ReadBuff[9]={0xFF,0x01,0x86,0x00,0x00,0x00,0x00,0x00,0x79}; //�۳���������ѯ����
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
 //SET-->PC1   RST->PC2  ��Դ����->PC0
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_0;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
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
 //��Դ����->PA11
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOA, &GPIO_InitStructure);	
 uart_init(9600);	 	//���ڳ�ʼ��Ϊ	
 WZSPower_ON;
 USART1_RX_STA = RESET;
}

void SensorCO2Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	  
 //��Դ����->PC9
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_Init(GPIOC, &GPIO_InitStructure);	
 usart2_init(19200);	 	//���ڳ�ʼ��Ϊ19200	
 CO2Power_ON;    //��������
 USART2_RX_STA = RESET;
}

void ReadSensorData(void)
{	
  //PMS
	if(UART5_RX_STA&0x8000)
	{
	  UART5_RX_STA = RESET;
    USART_ITConfig(UART5, USART_IT_RXNE, DISABLE);//���ж� 		
		PMSDataValueUN.PMSValueBuff[1]=PMSReadDataBuff[4];
		PMSDataValueUN.PMSValueBuff[0]=PMSReadDataBuff[5];
		SensorDataStr.PMSStr.PM1CFValue = PMSDataValueUN.PMSValue; //PM1.0 Ũ�ȣ�CF=1����׼�����		
		PMSDataValueUN.PMSValueBuff[1]=PMSReadDataBuff[6];
		PMSDataValueUN.PMSValueBuff[0]=PMSReadDataBuff[7];
		SensorDataStr.PMSStr.PM2_5CFValue = PMSDataValueUN.PMSValue;//PM2.5 Ũ�ȣ�CF=1����׼�����		
		PMSDataValueUN.PMSValueBuff[1]=PMSReadDataBuff[8];
		PMSDataValueUN.PMSValueBuff[0]=PMSReadDataBuff[9];
		SensorDataStr.PMSStr.PM10CFValue = PMSDataValueUN.PMSValue; //PM10 Ũ�ȣ�CF=1����׼�����				
		PMSDataValueUN.PMSValueBuff[1]=PMSReadDataBuff[10];
		PMSDataValueUN.PMSValueBuff[0]=PMSReadDataBuff[11];
		SensorDataStr.PMSStr.PM1Value = PMSDataValueUN.PMSValue;//PM1.0 Ũ�ȣ����������£�		
		PMSDataValueUN.PMSValueBuff[1]=PMSReadDataBuff[12];
		PMSDataValueUN.PMSValueBuff[0]=PMSReadDataBuff[13];
		SensorDataStr.PMSStr.PM2_5Value = PMSDataValueUN.PMSValue;//PM2.5 Ũ�ȣ����������£�		
		PMSDataValueUN.PMSValueBuff[1]=PMSReadDataBuff[14];
		PMSDataValueUN.PMSValueBuff[0]=PMSReadDataBuff[15];
		SensorDataStr.PMSStr.PM10Value = PMSDataValueUN.PMSValue;//PM10 Ũ�ȣ����������£�		
		PMSDataValueUN.PMSValueBuff[1]=PMSReadDataBuff[16];
		PMSDataValueUN.PMSValueBuff[0]=PMSReadDataBuff[17];
		SensorDataStr.PMSStr.PM0_3umValue = PMSDataValueUN.PMSValue; //0.1 ��������ֱ���� 0.3um ���Ͽ��������
		PMSDataValueUN.PMSValueBuff[1]=PMSReadDataBuff[18];
		PMSDataValueUN.PMSValueBuff[0]=PMSReadDataBuff[19];
		SensorDataStr.PMSStr.PM0_5umValue = PMSDataValueUN.PMSValue; 		//0.1 ��������ֱ���� 0.5um ���Ͽ��������
		PMSDataValueUN.PMSValueBuff[1]=PMSReadDataBuff[20];
		PMSDataValueUN.PMSValueBuff[0]=PMSReadDataBuff[21];
		SensorDataStr.PMSStr.PM1_0umValue = PMSDataValueUN.PMSValue; 		//0.1 ��������ֱ���� 1.0um ���Ͽ��������
		PMSDataValueUN.PMSValueBuff[1]=PMSReadDataBuff[22];
		PMSDataValueUN.PMSValueBuff[0]=PMSReadDataBuff[23];
		SensorDataStr.PMSStr.PM2_5umValue = PMSDataValueUN.PMSValue; 		//0.1 ��������ֱ���� 2.5um ���Ͽ��������
		PMSDataValueUN.PMSValueBuff[1]=PMSReadDataBuff[24];
		PMSDataValueUN.PMSValueBuff[0]=PMSReadDataBuff[25];
		SensorDataStr.PMSStr.PM5_0umValue = PMSDataValueUN.PMSValue; 		//0.1 ��������ֱ���� 5.0um ���Ͽ��������
		PMSDataValueUN.PMSValueBuff[1]=PMSReadDataBuff[26];
		PMSDataValueUN.PMSValueBuff[0]=PMSReadDataBuff[27];
		SensorDataStr.PMSStr.PM10umValue = PMSDataValueUN.PMSValue;		//0.1 ��������ֱ���� 10um ���Ͽ��������		
		SensorDataStr.PMSStr.PMVersion = PMSReadDataBuff[28]; //�汾��		
		SensorDataStr.PMSStr.PMErrorCode = PMSReadDataBuff[29]; //�������
	}	
	
 //WZS
 if(USART1_RX_STA&0x8000)
 {
		USART1_RX_STA = RESET;				
		PMSDataValueUN.PMSValueBuff[1]=WZSReadDataBuff[4];
		PMSDataValueUN.PMSValueBuff[0]=WZSReadDataBuff[5];
	  SensorDataStr.WZSStr.WZSCHValue = PMSDataValueUN.PMSValue;//��ȩ����Ũ��		
		PMSDataValueUN.PMSValueBuff[1]=WZSReadDataBuff[6];
		PMSDataValueUN.PMSValueBuff[0]=WZSReadDataBuff[7];
	  SensorDataStr.WZSStr.WZSCHRangeValue = PMSDataValueUN.PMSValue;//��ȩ����������	
	  SensorDataStr.WZSStr.WZSCHDecValue = WZSReadDataBuff[3];//С��λ��
 }
 
	if(USART2_RX_STA&0x8000)
	{
		USART2_RX_STA = RESET;				
		PMSDataValueUN.PMSValueBuff[1]=CO2ReadDataBuff[3];
		PMSDataValueUN.PMSValueBuff[0]=CO2ReadDataBuff[4];
	  SensorDataStr.CO2Value = PMSDataValueUN.PMSValue;  //CO2����Ũ��
	}
}

//��������������
void QuerySensorData(void)
{  
//	 USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
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
	//����
	adcx=Get_Adc_Average(ADC_Channel_4,10); 
	SensorDataStr.Noise = adcx*0.0008;
	//��ص�ѹ 
		adcx=Get_Adc_Average(ADC_Channel_1,10); 
		SensorDataStr.BTVotValue = adcx*0.0008;
}




