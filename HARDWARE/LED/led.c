#include "led.h"
#include "stm32f4xx.h"
#include "delay.h"

	void LED_GPIO_Config(void)
{	
	//定义一个GPIO_InitTypeDef 类型的结构体，名字叫GPIO_InitStructure 
	GPIO_InitTypeDef  GPIO_InitStructure;  
	//使能GPIOA的外设时钟，使用外设的时候都要使能时钟，不同外设使能的时钟可能不一样
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE,ENABLE);
	//选择要用的GPIO引脚		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//针脚1
	//设置引脚模式为普通输出模式			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 	
	//推挽输出，意思是可以明确的输出高低电平两种状态
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
	//设置引脚速度为100MHZ(可以从这里来理解为啥要使能时钟，否则单片机怎么控制频率)
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	//调用库函数，初始化GPIO(就是应用上面的配置)
	GPIO_Init(GPIOA, &GPIO_InitStructure);//GPIOA 表示初始化的是端口(port)A，即PA1
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
	GPIO_ResetBits(GPIOE,GPIO_Pin_8);
	GPIO_ResetBits(GPIOE,GPIO_Pin_9);
	GPIO_ResetBits(GPIOE,GPIO_Pin_10);
	GPIO_ResetBits(GPIOE,GPIO_Pin_11);
}


void LED_Bling(int times)
{
  int i = 0; 
  for(i = 0; i < times; i++)
  {
    GPIO_ResetBits(GPIOE,GPIO_Pin_8);
    GPIO_ResetBits(GPIOE,GPIO_Pin_9);
    GPIO_ResetBits(GPIOE,GPIO_Pin_10);
    GPIO_ResetBits(GPIOE,GPIO_Pin_11);
    delay_ms(100);
    GPIO_SetBits(GPIOE,GPIO_Pin_8);
    GPIO_SetBits(GPIOE,GPIO_Pin_9);
    GPIO_SetBits(GPIOE,GPIO_Pin_10);
    GPIO_SetBits(GPIOE,GPIO_Pin_11);
    delay_ms(100);
    GPIO_ResetBits(GPIOE,GPIO_Pin_8);
    GPIO_ResetBits(GPIOE,GPIO_Pin_9);
    GPIO_ResetBits(GPIOE,GPIO_Pin_10);
    GPIO_ResetBits(GPIOE,GPIO_Pin_11);
  }
}
  


	

	
	
	



