#include "GY521.h"

void RCC_Configuration(void)
{   
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
 RCC_HSEConfig(RCC_HSE_ON);
  
u8 HSEStartUpStatus;
  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if(HSEStartUpStatus == SUCCESS)
  {
    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1); 
  
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1); 

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

    /* Enable PLL */ 
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
  } 
   /* Enable GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG and AFIO clocks */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG | RCC_APB2Periph_AFIO  , ENABLE);  
}

void I2C_GPIO_Config(void)
{
  
  GPIO_InitTypeDef  GPIO_InitStructure; 
 
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void delay5ms(void)
{
		
   int i=5000;  
   while(i) 
   { 
     i--; 
   }  
}
void Delay(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}
 void Delayms(vu32 m)
{
  u32 i;
  
  for(; m != 0; m--)	
       for (i=0; i<50000; i++);
}
void Init_MPU3050(void)
{
   
  Single_Write(MPU3050_Addr,PWR_M, 0x80);   //
   Single_Write(MPU3050_Addr,SMPL, 0x07);    //
   Single_Write(MPU3050_Addr,DLPF, 0x1E);    
   Single_Write(MPU3050_Addr,INT_C, 0x00 );  //
   Single_Write(MPU3050_Addr,PWR_M, 0x00);   //
}
u8 Single_Write(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data)		     //void
{
    if(!I2C_Start())return 0;
    I2C_SendByte(SlaveAddress);   //?????υτ???+§Υ???//I2C_SendByte(((REG_Address & 0x0700) >>7) | SlaveAddress & 0xFFFE);//???????????+??????? 
    if(!I2C_WaitAck()){I2C_Stop(); return 0;}
    I2C_SendByte(REG_Address );   //???????????      
    I2C_WaitAck();	
    I2C_SendByte(REG_data);
    I2C_WaitAck();   
    I2C_Stop(); 
    delay5ms();
    return 1;
}

u8 I2C_Start(void)
{
	SDA_H;
	SCL_H;
	I2C_delay();
	if(!SDA_read)return 0;	//SDA??????????????,???
	SDA_L;
	I2C_delay();
	if(SDA_read) return 0;	//SDA????????????????,???
	SDA_L;
	I2C_delay();
	return 1;
}
	
void I2C_SendByte(u8 SendByte) //??????¦Λ????¦Λ//
{
    u8 i=8;
    while(i--)
    {
        SCL_L;
        I2C_delay();
      if(SendByte&0x80)
        SDA_H;  
      else 
        SDA_L;   
        SendByte<<=1;
        I2C_delay();
		SCL_H;
        I2C_delay();
    }
    SCL_L;
}  
void I2C_Ack(void)
{	
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
}   
u8 I2C_WaitAck(void) 	 //?????:=1??ACK,=0??ACK
{
	SCL_L;
	I2C_delay();
	SDA_H;			
	I2C_delay();
	SCL_H;
	I2C_delay();
	if(SDA_read)
	{
      SCL_L;
	  I2C_delay();
      return 0;
	}
	SCL_L;
	I2C_delay();
	return 1;
}
void I2C_Stop(void)
{
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SDA_H;
	I2C_delay();
} 
void I2C_delay(void)
{
		
   u8 i=30; //??????????????	?????????????5????§Υ??
   while(i) 
   { 
     i--; 
   }  
}
void READ_MPU3050(void)
{
   short T_X,T_Y,T_Z,T_T;
   unsigned char BUF[10];
   
   BUF[0]=Single_Read(MPU3050_Addr,GX_L); 
   BUF[1]=Single_Read(MPU3050_Addr,GX_H);
   T_X=	(BUF[1]<<8)|BUF[0];
   T_X/=16.4; 						   //???????X??????

   BUF[2]=Single_Read(MPU3050_Addr,GY_L);
   BUF[3]=Single_Read(MPU3050_Addr,GY_H);
   T_Y=	(BUF[3]<<8)|BUF[2];
   T_Y/=16.4; 						   //???????Y??????
   BUF[4]=Single_Read(MPU3050_Addr,GZ_L);
   BUF[5]=Single_Read(MPU3050_Addr,GZ_H);
   T_Z=	(BUF[5]<<8)|BUF[4];
   T_Z/=16.4; 					       //???????Z??????

   BUF[6]=Single_Read(MPU3050_Addr,TMP_L); 
   BUF[7]=Single_Read(MPU3050_Addr,TMP_H); 
   T_T=(BUF[7]<<8)|BUF[6];
   T_T = 35+ ((double) (T_T + 13200)) / 280;// ????????????
}
void Send_data(uchar axis)
 {
  unsigned char TX_DATA[4];
  uchar i;
  USART1_SendData(axis);
  USART1_SendData(':');
  for(i=0;i<4;i++)USART1_SendData(TX_DATA[i]);
  USART1_SendData(' ');
  USART1_SendData(' ');
 }

void DATA_printf(uchar *s,short temp_data)
{
	if(temp_data<0){
	temp_data=-temp_data;
    *s='-';
	}
	else *s=' ';
    *++s =temp_data/100+0x30;
    temp_data=temp_data%100;     //???????
    *++s =temp_data/10+0x30;
    temp_data=temp_data%10;      //???????
    *++s =temp_data+0x30; 	
}

void  USART1_SendData(uchar SendData)
{
USART_SendData(USART1, SendData);
Delayms(1);
}
unsigned char Single_Read(unsigned char SlaveAddress,unsigned char REG_Address)
{   
  char  test; 
  unsigned char REG_data;     	
  if(!I2C_Start())return FALSE;
    I2C_SendByte(SlaveAddress); //I2C_SendByte(((REG_Address & 0x0700) >>7) | REG_Address & 0xFFFE);//???????????+??????? 
    if(!I2C_WaitAck()){I2C_Stop();test=1; return FALSE;}
    I2C_SendByte((u8) REG_Address);   //???????????      
    I2C_WaitAck();
    I2C_Start();
    I2C_SendByte(SlaveAddress+1);
    I2C_WaitAck();

	REG_data= I2C_RadeByte();
    I2C_NoAck();
    I2C_Stop();
    //return TRUE;
	return REG_data;

}
unsigned char I2C_RadeByte(void)  //??????¦Λ????¦Λ//
{ 
    u8 i=8;
    u8 ReceiveByte=0;

    SDA_H;				
    while(i--)
    {
      ReceiveByte<<=1;      
      SCL_L;
      I2C_delay();
	  SCL_H;
      I2C_delay();	
      if(SDA_read)
      {
        ReceiveByte|=0x01;
      }
    }
    SCL_L;
    return ReceiveByte;
} 
void I2C_NoAck(void)
{	
	SCL_L;
	I2C_delay();
	SDA_H;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
} 
/*void GY521_init(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB1Periph_I2C1|RCC_APB2Periph_GPIOB,ENABLE);
  
  GPIO_InitTypeDef I2C1_GPIO_Psra;
  I2C1_GPIO_Psra.GPIO_Mode=GPIO_Mode_AF_OD;
  I2C1_GPIO_Psra.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;
  I2C1_GPIO_Psra.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Init(GPIOB,&I2C1_GPIO_Psra);
  
  I2C_InitTypeDef I2C1_Para;
  I2C1_Para.I2C_Ack=I2C_Ack_Enable;
  I2C1_Para.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit;
  I2C1_Para.I2C_ClockSpeed=200000;
  I2C1_Para.I2C_DutyCycle=I2C_DutyCycle_2;
  I2C1_Para.I2C_Mode=I2C_Mode_SMBusHost;
  I2C1_Para.I2C_OwnAddress1=0xA0;
  I2C_Init(I2C1,&I2C1_Para);
  
  I2C_Cmd(I2C1,ENABLE);
  
}*/