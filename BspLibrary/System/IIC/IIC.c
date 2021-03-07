#include "IIC.h"
#include "delay.h"
#include "usart.h"
#include <math.h>

 void I2C_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* ʹ���� I2C�йص�ʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  

	//SDA-->PB9   SCL-->PB8
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure); 

//	SCL_H;
//	SDA_H;
}

#if (USERED==0)
void SDA_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	// PB9-SDA	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	// PB9-SDA	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
#endif

void delay_1us(uint8_t x)//������ʱ,iic_40K
{
	delay_us(50);
}
////////IIC��ʼ����//////////
/*
IIC��ʼ:��SCL���ڸߵ�ƽ�ڼ䣬SDA�ɸߵ�ƽ��ɵ͵�ƽ����һ���½��أ�Ȼ��SCL����
*/
uint8_t I2C_Start(void)
{
	  SDA_OUT();
		SDA_H; 
		delay_1us(5);	//��ʱ��֤ʱ��Ƶ�ʵ���40K���Ա�ӻ�ʶ��
		SCL_H;
		delay_1us(5);//��ʱ��֤ʱ��Ƶ�ʵ���40K���Ա�ӻ�ʶ��
		//if(!SDA_read) return 0;//SDA��Ϊ�͵�ƽ������æ,�˳�
		SDA_L;   //SCL���ڸߵ�ƽ��ʱ��SDA����
		delay_1us(5);
	  //if(SDA_read) return 0;//SDA��Ϊ�ߵ�ƽ�����߳���,�˳�
		SCL_L;
	  delay_1us(5);
	  return 1;
}
//**************************************
//IICֹͣ�ź�
/*
IICֹͣ:��SCL���ڸߵ�ƽ�ڼ䣬SDA�ɵ͵�ƽ��ɸߵ�ƽ����һ��������
*/
//**************************************
void I2C_Stop(void)
{
		SDA_OUT();
    SDA_L;
		SCL_L;
		delay_1us(5);
		SCL_H;
		delay_1us(5);
		SDA_H;//��SCL���ڸߵ�ƽ�ڼ䣬SDA�ɵ͵�ƽ��ɸߵ�ƽ             //��ʱ
}
//**************************************
//IIC����Ӧ���ź�
//��ڲ���:ack (0:ACK 1:NAK)
/*
Ӧ�𣺵��ӻ����յ����ݺ�����������һ���͵�ƽ�ź�
��׼����SDA��ƽ״̬����SCL�ߵ�ƽʱ����������SDA
*/
//**************************************
void I2C_SendACK(uint8_t i)
{
		SDA_OUT();
    if(1==i)
			SDA_H;	             //׼����SDA��ƽ״̬����Ӧ��
    else 
			SDA_L;  						//׼����SDA��ƽ״̬��Ӧ�� 	
	  SCL_H;                    //����ʱ����
    delay_1us(5);                 //��ʱ
    SCL_L ;                  //����ʱ����
    delay_1us(5);    
} 
///////�ȴ��ӻ�Ӧ��////////
/*
������(����)������һ�����ݺ󣬵ȴ��ӻ�Ӧ��
���ͷ�SDA���ôӻ�ʹ�ã�Ȼ��ɼ�SDA״̬
*/
/////////////////
uint8_t I2C_WaitAck(void) 	 //����Ϊ:=1��ACK,=0��ACK
{
	SDA_IN();
	uint16_t i=0;
	SDA_H;delay_us(1);	        //�ͷ�SDA
	SCL_H;delay_us(1);         //SCL���߽��в���
	while(SDA_read)//�ȴ�SDA����
	{
		i++;      //�ȴ�����
		if(i==500)//��ʱ����ѭ��
		break;
		
	}
	if(SDA_read)//�ٴ��ж�SDA�Ƿ�����
	{
		SCL_L; 
		return RESET;//�ӻ�Ӧ��ʧ�ܣ�����0
	}
  delay_1us(5);//��ʱ��֤ʱ��Ƶ�ʵ���40K��
	SCL_L;
	delay_1us(5); //��ʱ��֤ʱ��Ƶ�ʵ���40K��
	SDA_OUT();
	return SET;//�ӻ�Ӧ��ɹ�������1
}
//**************************************
//��IIC���߷���һ���ֽ�����
/*
һ���ֽ�8bit,��SCL�͵�ƽʱ��׼����SDA��SCL�ߵ�ƽʱ���ӻ�����SDA
*/
//**************************************
void I2C_SendByte(uint8_t dat)
{
	SDA_OUT();
  uint8_t i;
	SCL_L;//SCL���ͣ���SDA׼��
  for (i=0; i<8; i++)         //8λ������
  {
		if(dat&0x80)//SDA׼��
		SDA_H;  
		else 
		SDA_L;
    SCL_H;                //����ʱ�ӣ����ӻ�����
    delay_1us(5);        //��ʱ����IICʱ��Ƶ�ʣ�Ҳ�Ǹ��ӻ������г���ʱ��
    SCL_L;                //����ʱ�ӣ���SDA׼��
    delay_1us(5); 		  //��ʱ����IICʱ��Ƶ��
		dat <<= 1;          //�Ƴ����ݵ����λ  
  }
	delay_1us(10);
}
//**************************************
//��IIC���߽���һ���ֽ�����
//**************************************
uint8_t I2C_RecvByte(void)
{
    uint8_t i;
    uint8_t dat = 0;
		SDA_IN();
    SDA_H;//�ͷ�SDA�����ӻ�ʹ��
    delay_1us(5);         //��ʱ���ӻ�׼��SDAʱ��            
    for (i=0; i<8; i++)         //8λ������
    { 
		  dat <<= 1;
			
      SCL_H;                //����ʱ���ߣ������ӻ�SDA
     
		  if(SDA_read) //������    
		   dat |=0x01;      
       delay_1us(5);     //��ʱ����IICʱ��Ƶ��		
       SCL_L;           //����ʱ���ߣ�������յ�������
       delay_1us(5);   //��ʱ���ӻ�׼��SDAʱ��
    } 
    return dat;
}
//**************************************
//��IIC�豸д��һ���ֽ�����
//**************************************
uint8_t Single_WriteI2C_byte(uint8_t Slave_Address,uint8_t REG_Address,uint8_t data)
{
	  if(I2C_Start()==0)  //��ʼ�ź�
		{I2C_Stop(); return RESET;}           

    I2C_SendByte(Slave_Address);   //�����豸��ַ+д�ź�
 	  if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
   
		I2C_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ��
 	  if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
   
		I2C_SendByte(data);       //�ڲ��Ĵ������ݣ�
	  if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
		
		I2C_Stop();   //����ֹͣ�ź�
		
		return SET;
}

uint8_t Single_MWriteI2C_byte(uint8_t Slave_Address,uint8_t REG_Address,uint8_t const *data,uint8_t length)
{
	  if(I2C_Start()==0)  //��ʼ�ź�
		{I2C_Stop();return RESET;}           

    I2C_SendByte(Slave_Address);   //�����豸��ַ+д�ź�
 	  if(!I2C_WaitAck()){I2C_Stop();return RESET;}
   
		I2C_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ��
 	  if(!I2C_WaitAck()){I2C_Stop();return RESET;}
 
	while(length)
	{
		I2C_SendByte(*data++);       //�ڲ��Ĵ������ݣ�
	   if(!I2C_WaitAck()){I2C_Stop(); return RESET;}           //Ӧ��
		length--;
	}
	//	I2C_SendByte(*data);       //�ڲ��Ĵ������ݣ�
 	//	if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
		I2C_Stop();   //����ֹͣ�ź�		
		return SET;
}

//**************************************
//��IIC�豸��ȡһ���ֽ�����
//**************************************
uint8_t Single_ReadI2C(uint8_t Slave_Address,uint8_t REG_Address,uint8_t *REG_data,uint8_t length)
{
 if(I2C_Start()==0)  //��ʼ�ź�
		{I2C_Stop();return RESET;}          
	 
	I2C_SendByte(Slave_Address);    //�����豸��ַ+д�ź�
		if(!I2C_WaitAck()){I2C_Stop()	;return RESET;} 
	
	I2C_SendByte(REG_Address);     //���ʹ洢��Ԫ��ַ
 	if(!I2C_WaitAck()){I2C_Stop();return RESET;} 
	
	if(I2C_Start()==0)  //��ʼ�ź�
			{I2C_Stop(); return RESET;}            

	I2C_SendByte(Slave_Address+1);  //�����豸��ַ+���ź�
 	if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
	
	while(length-1)
	{
		*REG_data++=I2C_RecvByte();       //�����Ĵ�������
		I2C_SendACK(0);               //Ӧ��
		length--;
	}
	*REG_data=I2C_RecvByte();  
	I2C_SendACK(1);     //����ֹͣ�����ź�
	I2C_Stop();                    //ֹͣ�ź�
	return SET;
}






