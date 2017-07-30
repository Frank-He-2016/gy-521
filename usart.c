#include "usart.h"

void gpio_init(void)
{
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 |RCC_APB2Periph_GPIOA
|RCC_APB2Periph_AFIO, ENABLE);
  GPIO_InitTypeDef GPIO_Parameter;
  GPIO_Parameter.GPIO_Pin=GPIO_Pin_9;
  GPIO_Parameter.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_Parameter.GPIO_Mode=GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA,&GPIO_Parameter);
}

void usart_init(void)
{
  USART_InitTypeDef USART_Parameter;
  USART_Parameter.USART_BaudRate=9600;
  USART_Parameter.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
  USART_Parameter.USART_Mode=USART_Mode_Tx;
  USART_Parameter.USART_Parity=USART_Parity_No;
  USART_Parameter.USART_StopBits=USART_StopBits_1;
  USART_Parameter.USART_WordLength=USART_WordLength_8b;
  USART_Init(USART1,&USART_Parameter);
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //使能接收中断
  USART_ClearFlag(USART1,USART_FLAG_TC);
  USART_Cmd(USART1, ENABLE);
}