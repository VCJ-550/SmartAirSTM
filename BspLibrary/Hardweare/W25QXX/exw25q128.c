#include "exw25q128.h" 
#include "spi.h"
#include "delay.h"	   
#include "usart.h"	

 
u16 EXW25QXX_TYPE=EXW25Q128;	//Ĭ����W25Q128

//4KbytesΪһ��Sector
//16������Ϊ1��Block
//W25Q128
//����Ϊ16M�ֽ�,����128��Block,4096��Sector 
													 
//��ʼ��SPI FLASH��IO��
void EXW25QXX_Init(void)
{ 
	
 GPIO_InitTypeDef  GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	
	//PB1->CS
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	

	EXW25QXX_CS=1;			//SPI FLASH��ѡ��
	SPI3_Init();		   			//��ʼ��SPI
	SPI3_SetSpeed(SPI_BaudRatePrescaler_2);		//����Ϊ42Mʱ��,����ģʽ 
	EXW25QXX_TYPE=EXW25QXX_ReadID();	//��ȡFLASH ID.
}  

//��ȡW25QXX��״̬�Ĵ���
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
//TB,BP2,BP1,BP0:FLASH����д��������
//WEL:дʹ������
//BUSY:æ���λ(1,æ;0,����)
//Ĭ��:0x00
u8 EXW25QXX_ReadSR(void)   
{  
	u8 exbyte=0;   
	EXW25QXX_CS=0;                            //ʹ������   
	SPI3_ReadWriteByte(EXW25X_ReadStatusReg);    //���Ͷ�ȡ״̬�Ĵ�������    
	exbyte=SPI3_ReadWriteByte(0Xff);             //��ȡһ���ֽ�  
	EXW25QXX_CS=1;                            //ȡ��Ƭѡ     
	return exbyte;   
} 
//дW25QXX״̬�Ĵ���
//ֻ��SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)����д!!!
void EXW25QXX_Write_SR(u8 sr)   
{   
	EXW25QXX_CS=0;                            //ʹ������   
	SPI3_ReadWriteByte(EXW25X_WriteStatusReg);   //����дȡ״̬�Ĵ�������    
	SPI3_ReadWriteByte(sr);               //д��һ���ֽ�  
	EXW25QXX_CS=1;                            //ȡ��Ƭѡ     	      
}   
//W25QXXдʹ��	
//��WEL��λ   
void EXW25QXX_Write_Enable(void)   
{
	EXW25QXX_CS=0;                            //ʹ������   
    SPI3_ReadWriteByte(EXW25X_WriteEnable);      //����дʹ��  
	EXW25QXX_CS=1;                            //ȡ��Ƭѡ     	      
} 
//W25QXXд��ֹ	
//��WEL����  
void EXW25QXX_Write_Disable(void)   
{  
	EXW25QXX_CS=0;                            //ʹ������   
    SPI3_ReadWriteByte(EXW25X_WriteDisable);     //����д��ָֹ��    
	EXW25QXX_CS=1;                            //ȡ��Ƭѡ     	      
} 		
//��ȡоƬID
//����ֵ����:				   
//0XEF13,��ʾоƬ�ͺ�ΪW25Q80  
//0XEF14,��ʾоƬ�ͺ�ΪW25Q16    
//0XEF15,��ʾоƬ�ͺ�ΪW25Q32  
//0XEF16,��ʾоƬ�ͺ�ΪW25Q64 
//0XEF17,��ʾоƬ�ͺ�ΪW25Q128 	  
u16 EXW25QXX_ReadID(void)
{
	u16 exTemp = 0;	  
	EXW25QXX_CS=0;				    
	SPI3_ReadWriteByte(0x90);//���Ͷ�ȡID����	    
	SPI3_ReadWriteByte(0x00); 	    
	SPI3_ReadWriteByte(0x00); 	    
	SPI3_ReadWriteByte(0x00); 	 			   
	exTemp|=SPI3_ReadWriteByte(0xFF)<<8;  
	exTemp|=SPI3_ReadWriteByte(0xFF);	 
	EXW25QXX_CS=1;	
//  printf("\r\n�ⲿ������ID:0x%0x\r\n",exTemp);//��ӡ������ID		
	return exTemp;
}   		    
//��ȡSPI FLASH  
//��ָ����ַ��ʼ��ȡָ�����ȵ�����
//pBuffer:���ݴ洢��
//ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
//NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
void EXW25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{ 
 	u16 i;   										    
	EXW25QXX_CS=0;                            //ʹ������   
    SPI3_ReadWriteByte(EXW25X_ReadData);         //���Ͷ�ȡ����   
    SPI3_ReadWriteByte((u8)((ReadAddr)>>16));  //����24bit��ַ    
    SPI3_ReadWriteByte((u8)((ReadAddr)>>8));   
    SPI3_ReadWriteByte((u8)ReadAddr);   
    for(i=0;i<NumByteToRead;i++)
	{ 
        pBuffer[i]=SPI3_ReadWriteByte(0XFF);   //ѭ������  
    }
	//printf("\r\nд�������ַ:0x%0x\r\n",NumByteToRead);
	EXW25QXX_CS=1;  				    	      
}  
//SPI��һҳ(0~65535)��д������256���ֽڵ�����
//��ָ����ַ��ʼд�����256�ֽڵ�����
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!	 
void EXW25QXX_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
 	u16 i;  
    EXW25QXX_Write_Enable();                  //SET WEL 
	EXW25QXX_CS=0;                            //ʹ������   
    SPI3_ReadWriteByte(EXW25X_PageProgram);      //����дҳ����   
    SPI3_ReadWriteByte((u8)((WriteAddr)>>16)); //����24bit��ַ    
    SPI3_ReadWriteByte((u8)((WriteAddr)>>8));   
    SPI3_ReadWriteByte((u8)WriteAddr);   
    for(i=0;i<NumByteToWrite;i++)SPI3_ReadWriteByte(pBuffer[i]);//ѭ��д��  
	EXW25QXX_CS=1;                            //ȡ��Ƭѡ 
	EXW25QXX_Wait_Busy();					   //�ȴ�д�����
} 
//�޼���дSPI FLASH 
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ���� 
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
//CHECK OK
void EXW25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain=256-WriteAddr%256; //��ҳʣ����ֽ���		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//������256���ֽ�
	while(1)
	{	   
		EXW25QXX_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//д�������
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //��ȥ�Ѿ�д���˵��ֽ���
			if(NumByteToWrite>256)pageremain=256; //һ�ο���д��256���ֽ�
			else pageremain=NumByteToWrite; 	  //����256���ֽ���
		}
	};	    
} 
//дSPI FLASH  
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ú�������������!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)						
//NumByteToWrite:Ҫд����ֽ���(���65535)   
u8 EXW25QXX_BUFFER[4096];		 
void EXW25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    
	u8 * EXW25QXX_BUF;	  
   	EXW25QXX_BUF=EXW25QXX_BUFFER;	     
 	secpos=WriteAddr/4096;//������ַ  
	secoff=WriteAddr%4096;//�������ڵ�ƫ��
	secremain=4096-secoff;//����ʣ��ռ��С   
// 	printf("��ʼ��ַ:0x%0x,�ֽ���:0x%0x\r\n",WriteAddr,NumByteToWrite);//������
//	printf("������:0x%0x,������ƫ��:0x%0x,����ʣ���С:0x%0x\r\n",secpos,secoff,secremain);//������
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//������4096���ֽ�
	while(1) 
	{	
		EXW25QXX_Read(EXW25QXX_BUF,secpos*4096,4096);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(EXW25QXX_BUF[secoff+i]!=0XFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			EXW25QXX_Erase_Sector(secpos);//�����������
			for(i=0;i<secremain;i++)	   //����
			{
				EXW25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			EXW25QXX_Write_NoCheck(EXW25QXX_BUF,secpos*4096,4096);//д����������  

		}else EXW25QXX_Write_NoCheck(pBuffer,WriteAddr,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumByteToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;//������ַ��1
			secoff=0;//ƫ��λ��Ϊ0 	 

		   	pBuffer+=secremain;  //ָ��ƫ��
			WriteAddr+=secremain;//д��ַƫ��	   
		   	NumByteToWrite-=secremain;				//�ֽ����ݼ�
			if(NumByteToWrite>4096)secremain=4096;	//��һ����������д����
			else secremain=NumByteToWrite;			//��һ����������д����
		}	 
	};	 
}
//��������оƬ		  
//�ȴ�ʱ�䳬��...
void EXW25QXX_Erase_Chip(void)   
{                                   
    EXW25QXX_Write_Enable();                  //SET WEL 
    EXW25QXX_Wait_Busy();   
  	EXW25QXX_CS=0;                            //ʹ������   
    SPI3_ReadWriteByte(EXW25X_ChipErase);        //����Ƭ��������  
	EXW25QXX_CS=1;                            //ȡ��Ƭѡ     	      
	EXW25QXX_Wait_Busy();   				   //�ȴ�оƬ��������
}   
//����һ������
//Dst_Addr:������ַ ����ʵ����������
//����һ��ɽ��������ʱ��:150ms
void EXW25QXX_Erase_Sector(u32 Dst_Addr)   
{  
	//����falsh�������,������   
// 	printf("fe:%x\r\n",Dst_Addr);	  
 	Dst_Addr*=4096;
    EXW25QXX_Write_Enable();                  //SET WEL 	 
    EXW25QXX_Wait_Busy();   
  	EXW25QXX_CS=0;                            //ʹ������   
    SPI3_ReadWriteByte(EXW25X_SectorErase);      //������������ָ�� 
    SPI3_ReadWriteByte((u8)((Dst_Addr)>>16));  //����24bit��ַ    
    SPI3_ReadWriteByte((u8)((Dst_Addr)>>8));   
    SPI3_ReadWriteByte((u8)Dst_Addr);  
	  EXW25QXX_CS=1;                            //ȡ��Ƭѡ     	      
    EXW25QXX_Wait_Busy();   				   //�ȴ��������
}  
//�ȴ�����
void EXW25QXX_Wait_Busy(void)   
{   
	while((EXW25QXX_ReadSR()&0x01)==0x01);   // �ȴ�BUSYλ���
}  
//�������ģʽ
void EXW25QXX_PowerDown(void)   
{ 
  	EXW25QXX_CS=0;                            //ʹ������   
    SPI3_ReadWriteByte(EXW25X_PowerDown);        //���͵�������  
	  EXW25QXX_CS=1;                            //ȡ��Ƭѡ     	      
    delay_us(3);                               //�ȴ�TPD  
}   
//����
void EXW25QXX_WAKEUP(void)   
{  
  	EXW25QXX_CS=0;                            //ʹ������   
    SPI3_ReadWriteByte(EXW25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB    
	  EXW25QXX_CS=1;                            //ȡ��Ƭѡ     	      
    delay_us(3);                               //�ȴ�TRES1
}   


























