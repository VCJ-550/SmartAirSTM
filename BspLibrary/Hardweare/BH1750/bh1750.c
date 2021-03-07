#include "sys.h"
#include "delay.h"
#include "bh1750.h"
#include "iic.h"

#define	 SlaveAddress  0xB8 // 0x46 //����������IIC�����еĴӵ�ַ
uint8_t  BUF[4];                  //�������ݻ�����

/***************************************************************
** ���ܣ�     ��bh1750д������
** ������	  �޲���
** ����ֵ��   ��
****************************************************************/
void Single_Write_BH1750(u8 REG_Address)
{
    I2C_Start();                  //��ʼ�ź�
    I2C_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�
    I2C_WaitAck();
    I2C_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ��
	  I2C_WaitAck();
    I2C_Stop();                   //����ֹͣ�ź�
}

/***************************************************************
** ���ܣ�     ��������BH1750�ڲ����� 
** ������	  �޲���
** ����ֵ��   ��
****************************************************************/
void Multiple_Read_BH1750(void)
{   u8 i;	
    I2C_Start();                          //��ʼ�ź�
    I2C_SendByte(SlaveAddress+1);         //�����豸��ַ+���ź�
	  I2C_WaitAck();
	for (i=0; i<3; i++)                      //������ȡ2����ַ���ݣ��洢��BUF
    {
        BUF[i] = I2C_RecvByte();          //BUF[0]�洢0x32��ַ�е�����
			  I2C_SendACK(0); 
   }
    I2C_Stop();                          //ֹͣ�ź�
 //   delay_ms(150);
}

/***************************************************************
** ���ܣ�     ��ʼ��BH1750
** ������	  �޲���
** ����ֵ��   ��
****************************************************************/
void BH1750_Configure(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	// PA12-ADDR	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	ADDR = 0;  						//��ADDRλ��ʼ������
	
	I2C_GPIO_Config();
	
  Single_Write_BH1750(0x01);  
   
}

/***************************************************************
** ���ܣ�     ��ȡ���ն�
** ������	  �޲���
** ����ֵ��   data�����ع��ն�ֵ
****************************************************************/
uint16_t Get_Bh_Value(void)
{  
	static float temp;
	unsigned int data;
	int dis_data ;
    Single_Write_BH1750(0x01);   // power on
    Single_Write_BH1750(0x10);   // H- resolution mode
//    delay_ms(200);              //��ʱ200ms
    Multiple_Read_BH1750();       //�����������ݣ��洢��BUF��
    dis_data=BUF[0];
    dis_data=(dis_data<<8)+BUF[1];//�ϳ����ݣ�����������
    temp=(float)dis_data/1.2;
	data=(int)temp;
    return data;        
}




