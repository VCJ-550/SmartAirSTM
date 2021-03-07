#include "spi.h"



void SPI2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);//ʹ��SPI2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//ʹ��GPIOA

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�ٶ�50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	 //�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��GPIOA
	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//ȫ˫��ģʽ
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//����SPI����ģʽ������ģʽ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//����SPI���ݴ�С��8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//����ͬ��ʱ�ӿ���ʱSCLKλ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//����ͬ��ʱ�ӿյڶ���ʱ���ز���
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//NSS�ź����������
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;//������Ԥ��Ƶֵ��������Ԥ��ƵֵΪ4
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//���ݴ����λ����
	SPI_InitStructure.SPI_CRCPolynomial = 7;//CRCֵ����Ķ���ʽ
	SPI_Init(SPI2,&SPI_InitStructure);//��ʼ��SPI
	SPI_Cmd(SPI2, ENABLE);//ʹ��SPI
}


//SPI �ٶ����ú���
//SpeedSet:
//SPI_BaudRatePrescaler_2   2��Ƶ   
//SPI_BaudRatePrescaler_8   8��Ƶ   
//SPI_BaudRatePrescaler_16  16��Ƶ  
//SPI_BaudRatePrescaler_256 256��Ƶ 
  
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI2->CR1&=0XFFC7;
	SPI2->CR1|=SPI_BaudRatePrescaler;	//����SPI1�ٶ� 
	SPI_Cmd(SPI2,ENABLE); 

} 

//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI2_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
		{
		retry++;
		if(retry>200)return 0;
		}			  
	SPI_I2S_SendData(SPI2, TxData); //ͨ������SPIx����һ������
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
	return SPI_I2S_ReceiveData(SPI2); //����ͨ��SPIx������յ�����					    
}

/***************************************************************************************************/ 

//�������Ƕ�SPI3�ĳ�ʼ��
void SPI3_Init(void)
{	 
  GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);//ʹ��SPI3ʱ��
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
  //PB3->SCLK PB5->MOSI PB4->MISO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;	 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 		 //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_SetBits(GPIOB,GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5);

	//����ֻ���SP3�ڳ�ʼ��
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3,ENABLE);//��λSPI3
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3,DISABLE);//ֹͣ��λSPI3

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI3, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
	SPI_Cmd(SPI3, ENABLE); //ʹ��SPI����

	SPI3_ReadWriteByte(0xff);//��������		 
}   
//SPI3�ٶ����ú���
//SPI�ٶ�=fAPB2/��Ƶϵ��
//@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
//fAPB2ʱ��һ��Ϊ84Mhz��
void SPI3_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//�ж���Ч��
	SPI3->CR1&=0XFFC7;//λ3-5���㣬�������ò�����
	SPI3->CR1|=SPI_BaudRatePrescaler;	//����SPI1�ٶ� 
	SPI_Cmd(SPI3,ENABLE); //ʹ��SPI3
} 
//SPI1 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI3_ReadWriteByte(u8 TxData)
{		 			 
 
  while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET){}//�ȴ���������  
	
	SPI_I2S_SendData(SPI3, TxData); //ͨ������SPIx����һ��byte  ����
		
  while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET){} //�ȴ�������һ��byte  
 
	return SPI_I2S_ReceiveData(SPI3); //����ͨ��SPIx������յ�����	
 		    
}

