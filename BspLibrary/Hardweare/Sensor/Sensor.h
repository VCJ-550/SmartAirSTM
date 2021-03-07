#ifndef __SERVO_H_
#define __SERVO_H_
#include "sys.h"

/*PMS������*/
//SET-->PC1
#define PMSSET_SET  	GPIO_SetBits(GPIOC,GPIO_Pin_1)    //SET->SET    
#define PMSSET_RESET  GPIO_ResetBits(GPIOC,GPIO_Pin_1)  //SET->RESET
//RST->PC2
#define PMSRST_SET  	GPIO_SetBits(GPIOC,GPIO_Pin_2)	  //RST->SET
#define PMSRST_RESET  GPIO_ResetBits(GPIOC,GPIO_Pin_2)  //RST->RESET
//��Դ����->PC0
#define PMSPower_ON  	GPIO_SetBits(GPIOC,GPIO_Pin_0)    //Power->SET
#define PMSPower_OFF  GPIO_ResetBits(GPIOC,GPIO_Pin_0)  //Power->RESET

/*WZS������*/
#define WZSPower_ON  	GPIO_SetBits(GPIOA,GPIO_Pin_11)    //Power->SET
#define WZSPower_OFF  GPIO_ResetBits(GPIOA,GPIO_Pin_11)  //Power->RESET

/*CO2������*/
#define CO2Power_ON  	GPIO_SetBits(GPIOC,GPIO_Pin_9)    //Power->SET
#define CO2Power_OFF  GPIO_ResetBits(GPIOC,GPIO_Pin_9)  //Power->RESET

typedef struct PMS
{
 uint16_t PM1CFValue;
 uint16_t PM2_5CFValue;
 uint16_t PM10CFValue;
 uint16_t PM1Value;
 uint16_t PM2_5Value;
 uint16_t PM10Value;
 uint16_t PM0_3umValue;
 uint16_t PM0_5umValue;
 uint16_t PM1_0umValue;
 uint16_t PM2_5umValue;
 uint16_t PM5_0umValue;
 uint16_t PM10umValue;
 uint8_t PMVersion;
 uint8_t PMErrorCode;
}PMSDef;

typedef struct WZS
{
 uint16_t WZSCHValue;
 uint16_t WZSCHRangeValue;
 uint16_t WZSCHDecValue;
}WZSDef;

typedef struct HDC
{
 uint16_t Temperature;
 uint16_t	Humidity;
 uint16_t hdcid; 
}HDCDef;

typedef struct SensorData
{
  PMSDef PMSStr;  //�۳��������ṹ��
	WZSDef WZSStr;  //��ȩ�������ṹ��
	HDCDef HDCStr;  //��ʪ�ȴ�����
	uint16_t CO2Value;  //������ֵ̼
	uint16_t Light; //����ǿ��
  float Noise; //����
  float BTVotValue;  //��ص�ѹ 	
}SensorDataDef;

extern SensorDataDef SensorDataStr;


void SensorPMSInit(void);
void SensorWZSInit(void);
void SensorCO2Init(void);

void ReadSensorData(void);
void QuerySensorData(void);


#endif

