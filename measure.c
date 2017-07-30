#include "measure.h"

u8 ic_init(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
  GPIO_InitTypeDef GPIO_PA0;
  GPIO_PA0.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_PA0.GPIO_Mode=GPIO_Mode_IN_FLOATING;
  GPIO_PA0.GPIO_Pin=GPIO_Pin_0;
  GPIO_Init(GPIOA,&GPIO_PA0);
  
  
  TIM_TimeBaseInitTypeDef Base_Parameter;
  Base_Parameter.TIM_Period=2000-1;
  Base_Parameter.TIM_Prescaler=36000-1;
  Base_Parameter.TIM_ClockDivision=TIM_CKD_DIV1;
  Base_Parameter.TIM_CounterMode=TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM2,&Base_Parameter);
  
  
  TIM_ICInitTypeDef IC_Parameter;
  IC_Parameter.TIM_Channel=TIM_Channel_1;
  IC_Parameter.TIM_ICPolarity=TIM_ICPolarity_Rising;
  IC_Parameter.TIM_ICSelection=TIM_ICSelection_DirectTI;
  IC_Parameter.TIM_ICPrescaler=TIM_ICPSC_DIV1;
  IC_Parameter.TIM_ICFilter=0X00;
  TIM_ICInit(TIM2, &IC_Parameter);
  
  TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
  TIM_Cmd(TIM2, ENABLE); 
  
  
  int TIM2IC_ReadValue1,TIM2IC_ReadValue2;//??????????????
  int TIM2CaptureNumber; //????????
  int TIM2Capture;
  u8 Frequency=0x08;
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)//????TIM2??§Ù????§Ø?
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);////???? TIM2 ???§Ø??????¦Ë
  }
  else if(TIM_GetITStatus(TIM2, TIM_IT_CC1) == SET)////????TIM2??§Ù????§Ø?
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);//???? TIM2???§Ø??????¦Ë
    TIM2CaptureNumber=0;
    if(TIM2CaptureNumber== 0)//???????????????
    {
      TIM2IC_ReadValue1 = TIM_GetCapture1(TIM2);//????TIM2?????1???
      TIM2CaptureNumber = 1;//?????????1
    }
    else if(TIM2CaptureNumber == 1)//?????????????1
    {
      TIM2IC_ReadValue2 = TIM_GetCapture1(TIM2);//????TIM2?????2???
      if (TIM2IC_ReadValue2 > TIM2IC_ReadValue1)//§µ??
      {
        TIM2Capture= (TIM2IC_ReadValue2- TIM2IC_ReadValue1);
      }
      else
      {
        TIM2Capture = ((0xFFFF - TIM2IC_ReadValue1) + TIM2IC_ReadValue2); 
      }
      Frequency=TIM2Capture;
    }
  }
  Frequency=(u8)(Frequency);
  return Frequency;
}
