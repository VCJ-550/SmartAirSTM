#include "hdc.h"
#include "iic.h"
#include "delay.h"

HDC1080MenTDef HDC1080MenTStr;

//-----------------------------------------------------------------//
//HDC1080

u8 HDC1080_Write_Buffer(uint8_t addr, uint8_t *buffer, uint8_t len)
{
    if(I2C_Start()==0)  //起始信号
		{I2C_Stop(); return RESET;}  
	
		
    I2C_SendByte(HDC1080_Device_Adderss);
    if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
		
    I2C_SendByte(addr);
		if(!I2C_WaitAck()){I2C_Stop();return RESET;}
		
    while ( len-- )
    {
    	I2C_SendByte(*buffer);
			if(!I2C_WaitAck()){I2C_Stop();return RESET;}
    	buffer++;
    }
    I2C_Stop();
		return SET;
}

void HDC1080_Soft_Reset(void)
{
    u8 temp[2];
    temp[0] = 0x80; 
    temp[1] = 0x00;
    HDC1080_Write_Buffer(HDC1080_Read_Config, temp, 2);
    delay_ms(20);//there should be waiting for more than 15 ms.
}

void HDC1080_Setting(void)
{
    uint16_t tempcom = 0;
    uint8_t temp[2];
	
    tempcom |= 1<<HDC1080_Mode ;//连续读取数据使能
    // 温度与温度都为14bit
    temp[0] = (uint8_t)(tempcom >> 8); 
    temp[1] = (uint8_t)tempcom;
    HDC1080_Write_Buffer(0x02, temp, 2);
}



void HDC1080_Init(void)
{
//		u8 temp = 0;
	I2C_GPIO_Config();
    HDC1080_Soft_Reset();
    HDC1080_Setting();
}


u8 HDC1080_Read_Buffer(uint8_t addr, uint8_t *buffer, uint8_t len)
{	
//    uint8_t temp = 0;
	
		if(I2C_Start()==0)  //起始信号
		{I2C_Stop(); return RESET;}  
		
    I2C_SendByte(HDC1080_Device_Adderss);
    if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
		
    I2C_SendByte(addr);
    if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
		 
    I2C_Stop();
//    
//    while( pin_SCL_READ())
//    {
//        Delay_us(20);
//	temp++;
//	if ( temp >= 100 )
//	{
//	    break;
//	}
//    }
		delay_ms(1);
    I2C_Start();
		delay_ms(10);
    I2C_SendByte(HDC1080_Device_Adderss|0x01);    //read
     if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
		 
    while ( len-- )
    {
    	*buffer = I2C_RecvByte();
    	buffer++;
    	if ( len == 0 )
    	   I2C_SendACK(1);
    	else
          I2C_SendACK(0);   	
    	
    }
    I2C_Stop();
		return SET;
}
/*******************************************************************************
  * @brief  HDC1080_Read_MBuffer becareful between triger and read there is a wait.
  * @param  uint8_t addr is point register
  * @param  uint8_t *buffer is the need to read data point
  * @param  uint8_t len is the read data length
  * @retval void
 *******************************************************************************/
u8 HDC1080_Read_MBuffer(uint8_t addr, uint8_t *buffer, uint8_t len)
{
   // uint8_t temp = 0;
	
   
		if(I2C_Start()==0)  //起始信号
		{I2C_Stop(); return RESET;}  
		
     I2C_SendByte(HDC1080_Device_Adderss);
    if(!I2C_WaitAck()){I2C_Stop(); return RESET;}

    
    I2C_SendByte(0X00);
    if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
		
		I2C_Stop();
    
//    while( pin_SCL_READ())
//    {
//        Delay_us(20);
//	temp++;
//	if ( temp >= 100 )
//	{
//	    break;
//	}
//    }
    delay_ms(10);      // after triger should wait at least 6.5ms
		if(I2C_Start()==0)  //起始信号
		{I2C_Stop();return RESET;}  
		delay_ms(10);         //14位的转换时间，必须的
    I2C_SendByte(HDC1080_Device_Adderss|0x01);    //read
		if(!I2C_WaitAck()){I2C_Stop();return RESET;}
	
    while ( len-- )
    {
    	*buffer = I2C_RecvByte();
    	buffer++;
    	if ( len == 0 )
    	     I2C_SendACK(1);
    	else
           I2C_SendACK(0);
    	
    }
    I2C_Stop();
		return SET;
}

u16 HDC1080_Read_Temper(void)
{
    uint8_t buffer[2];

    HDC1080_Read_MBuffer(0x00, buffer, 2);
    //return ((buffer[0]<<8)|buffer[1]);
    return (u16)(((((buffer[0]<<8)|buffer[1])/65536.0)*165-40)*100);//Temper*100
}
/*******************************************************************************
  * @brief  HDC1080_Read_Humidity 
  * @param  void
  * @retval int16_t Humidity value
 *******************************************************************************/
u16 HDC1080_Read_Humidi(void)
{
    uint8_t buffer[2];
	
    HDC1080_Read_MBuffer(HDC1080_Read_Humidity, buffer, 2);

   // return (buffer[0]<<8)|buffer[1];
    return (u16)((((buffer[0]<<8)|buffer[1])/65536.0)*10000);//Humidi*100
}
/*******************************************************************************
  * @brief  HDC1080_Read_Configuration 
  * @param  void
  * @retval Config value
 *******************************************************************************/
u16 HDC1080_Read_Conf(void)
{
    uint8_t buffer[2];
	
    HDC1080_Read_Buffer(HDC1080_Read_Config, buffer, 2);

    return ((buffer[0]<<8)|buffer[1]);
}
/*******************************************************************************
  * @brief  HDC1080_Read_ManufacturerID 
  * @param  void
  * @retval ManufacturerID 
 *******************************************************************************/
u16 HDC1080_Read_ManufacturerID(void)
{
    uint8_t buffer[2];
	
    HDC1080_Read_Buffer(0xfe, buffer, 2);

    return ((buffer[0]<<8)|buffer[1]);
}
/*******************************************************************************
  * @brief  HDC1080_Read_DeviceID
  * @param  void
  * @retval DeviceID
 *******************************************************************************/
u16 HDC1080_Read_DeviceID(void)
{
    uint8_t buffer[2];
	
    HDC1080_Read_Buffer(0xff, buffer, 2);

    return ((buffer[0]<<8)|buffer[1]);
}


