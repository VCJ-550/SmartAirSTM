#ifndef _IIC_H
#define _IIC_H
#include "sys.h" 

#define USERED 0

//IO操作函数
#define SCL_H          GPIO_SetBits(GPIOB,GPIO_Pin_8)
#define SCL_L          GPIO_ResetBits(GPIOB,GPIO_Pin_8)    
#define SDA_H          GPIO_SetBits(GPIOB,GPIO_Pin_9)
#define SDA_L          GPIO_ResetBits(GPIOB,GPIO_Pin_9) 
#define SDA_read       PBin(9)

//IO方向设置 
#if USERED
#define SDA_IN()       {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=0x00000080;}	//PB9输入模式
#define SDA_OUT()      {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=0x00000030;} //PB9输出模式
#else
void SDA_OUT(void);
void SDA_IN(void);
#endif

void I2C_GPIO_Config(void);
uint8_t I2C_Start(void);
void I2C_Stop(void);
void I2C_SendACK(uint8_t i);
uint8_t I2C_WaitAck(void);
void I2C_SendByte(uint8_t dat);
uint8_t I2C_RecvByte(void);
uint8_t Single_WriteI2C_byte(uint8_t Slave_Address,uint8_t REG_Address,uint8_t data);
uint8_t Single_MWriteI2C_byte(uint8_t Slave_Address,uint8_t REG_Address,uint8_t const *data,uint8_t length);
uint8_t Single_ReadI2C(uint8_t Slave_Address,uint8_t REG_Address,uint8_t *REG_data,uint8_t length);


#endif


