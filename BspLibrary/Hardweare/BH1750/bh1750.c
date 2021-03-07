#include "sys.h"
#include "delay.h"
#include "bh1750.h"
#include "iic.h"

#define	 SlaveAddress  0xB8 // 0x46 //定义器件在IIC总线中的从地址
uint8_t  BUF[4];                  //接收数据缓存区

/***************************************************************
** 功能：     向bh1750写入命令
** 参数：	  无参数
** 返回值：   无
****************************************************************/
void Single_Write_BH1750(u8 REG_Address)
{
    I2C_Start();                  //起始信号
    I2C_SendByte(SlaveAddress);   //发送设备地址+写信号
    I2C_WaitAck();
    I2C_SendByte(REG_Address);    //内部寄存器地址，
	  I2C_WaitAck();
    I2C_Stop();                   //发送停止信号
}

/***************************************************************
** 功能：     连续读出BH1750内部数据 
** 参数：	  无参数
** 返回值：   无
****************************************************************/
void Multiple_Read_BH1750(void)
{   u8 i;	
    I2C_Start();                          //起始信号
    I2C_SendByte(SlaveAddress+1);         //发送设备地址+读信号
	  I2C_WaitAck();
	for (i=0; i<3; i++)                      //连续读取2个地址数据，存储中BUF
    {
        BUF[i] = I2C_RecvByte();          //BUF[0]存储0x32地址中的数据
			  I2C_SendACK(0); 
   }
    I2C_Stop();                          //停止信号
 //   delay_ms(150);
}

/***************************************************************
** 功能：     初始化BH1750
** 参数：	  无参数
** 返回值：   无
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
	
	ADDR = 0;  						//将ADDR位初始化拉低
	
	I2C_GPIO_Config();
	
  Single_Write_BH1750(0x01);  
   
}

/***************************************************************
** 功能：     读取光照度
** 参数：	  无参数
** 返回值：   data：返回光照度值
****************************************************************/
uint16_t Get_Bh_Value(void)
{  
	static float temp;
	unsigned int data;
	int dis_data ;
    Single_Write_BH1750(0x01);   // power on
    Single_Write_BH1750(0x10);   // H- resolution mode
//    delay_ms(200);              //延时200ms
    Multiple_Read_BH1750();       //连续读出数据，存储在BUF中
    dis_data=BUF[0];
    dis_data=(dis_data<<8)+BUF[1];//合成数据，即光照数据
    temp=(float)dis_data/1.2;
	data=(int)temp;
    return data;        
}




