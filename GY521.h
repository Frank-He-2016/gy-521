#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_i2c.h"

#define _I2C
#define _I2C1
#define _I2C2

#define WHO	    0x00
#define	SMPL	0x15
#define DLPF	0x16
#define INT_C	0x17
#define INT_S	0x1A
#define	TMP_H	0x1B
#define	TMP_L	0x1C
#define	GX_H	0x1D
#define	GX_L	0x1E
#define	GY_H	0x1F
#define	GY_L	0x20
#define GZ_H	0x21
#define GZ_L	0x22
#define PWR_M	0x3E

#define	MPU3050_Addr   0xD0	  //??????????IIC?????§Ö????,????ALT  ADDRESS????????????

#define   uchar unsigned char
#define   uint unsigned int	

extern unsigned char TX_DATA[4];  	 //??????????
extern unsigned char BUF[10];       //?????????????
extern char  test; 				 //IIC???
extern short T_X,T_Y,T_Z,T_T;		 //X,Y,Z?????

//************************************
/*???IIC?????????????*/
#define SCL_H         GPIOB->BSRR = GPIO_Pin_6
#define SCL_L         GPIOB->BRR  = GPIO_Pin_6 
   
#define SDA_H         GPIOB->BSRR = GPIO_Pin_7
#define SDA_L         GPIOB->BRR  = GPIO_Pin_7

#define SCL_read      GPIOB->IDR  & GPIO_Pin_6
#define SDA_read      GPIOB->IDR  & GPIO_Pin_7
#define FALSE 0

void RCC_Configuration(void);
void I2C_GPIO_Config(void);
void delay5ms(void);
void Delay(vu32 nCount);
void Delayms(vu32 m);
void Init_MPU3050(void);
u8 Single_Write(unsigned char SlaveAddress,unsigned char REG_Address,unsigned char REG_data);
u8 I2C_Start(void);
void I2C_SendByte(u8 SendByte);
void I2C_Ack(void);
u8 I2C_WaitAck(void);
void I2C_Stop(void);
void I2C_delay(void);
void READ_MPU3050(void);
void Send_data(uchar axis);
unsigned char Single_Read(unsigned char SlaveAddress,unsigned char REG_Address);
void DATA_printf(uchar *s,short temp_data);
void  USART1_SendData(uchar SendData);
unsigned char I2C_RadeByte(void);
void I2C_NoAck(void);
//void GY521_init(void);