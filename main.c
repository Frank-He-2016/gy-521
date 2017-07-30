#include "usart.h"
#include "measure.h"
#include "GY521.h"
int main()
{
  short T_X,T_Y,T_Z,T_T;
  unsigned char TX_DATA[4];
  RCC_Configuration();
  gpio_init();
  usart_init();
  ic_init();
  //USART1_Configuration();
  I2C_GPIO_Config();
  Delayms(10);
  Init_MPU3050();
  while(1)
 {
  char *a;
  a="ÍÓÂÝÒÇÊý¾ÝÎª";
  while(*a)
  {
    USART_SendData(USART1,*a);
    while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
    *a++;
  }
        READ_MPU3050();	         
        DATA_printf(TX_DATA,T_X);
	Send_data('X');			
	DATA_printf(TX_DATA,T_Y);
	Send_data('Y');			
	DATA_printf(TX_DATA,T_Z);
	Send_data('Z');		
	DATA_printf(TX_DATA,T_T);
	Send_data('T');	
	USART1_SendData(0X0D);
	USART1_SendData(0X0A);
	Delayms(5);
  }
/*while(1)
{
  char *a;
  a="abcdefg\r\n";
  while(*a)
  {
    USART_SendData(USART1,*a);
    while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
    *a++;
  }
  u8 f;
  f=ic_init();
  USART_SendData(USART1,f);
  while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
}*/
}
