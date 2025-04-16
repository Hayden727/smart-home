#include "Beep.h"
#include "stm32f4xx.h"
#include "delay.h"

	void BEEP_Init(void)
{	
	
	GPIO_InitTypeDef  GPIO_InitStructure;  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //蜂鸣器引脚为PB0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
}




//蜂鸣器响应两次，作为门铃

void Beep_Bling(void)
{
  Beep = !Beep;
  delay_ms(100);
  Beep = !Beep;
  delay_ms(100);
  Beep = !Beep;
  delay_ms(100);
  Beep = !Beep;
}

void Beep_Bling_once(void)
{
  Beep = !Beep;
  delay_ms(100);
  Beep = !Beep;
}

