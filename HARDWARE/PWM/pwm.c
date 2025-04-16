#include "pwm.h"
#include "usart.h"

void TIM4_SERVOPWM_Init(u32 arr,u32 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	//使能定时器4时钟   
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 	//使能GPIOD的时钟	
  
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13|GPIO_Pin_15;//TIM4_CH2和TIM4_CH4
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;//模拟输出模式
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;//上拉输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_InitStructure);//初始化GPIO
	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_TIM4);
	
	//初始化TIM4
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//TIM向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period=arr-1;//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc-1;//设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	
	
	//初始化TIM5 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;////选择定时器模式:TIM脉冲宽度调制模式1,小于的事有效电平
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;//比较输出使能
//	TIM_OCInitStructure.TIM_Pulse=100;//设置比较难
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//输出极性:TIM输出比较极性低
	TIM_OC2Init(TIM4,&TIM_OCInitStructure);
	TIM_OC4Init(TIM4,&TIM_OCInitStructure);
	
	TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable);// 使能TIM4在CCR2上的预装载寄存器
	TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable);// 使能TIM4在CCR上的预装载寄存器
	
	TIM_ARRPreloadConfig(TIM4,ENABLE); //使能TIM4的计数寄存器
	TIM_Cmd(TIM4,ENABLE); //使能TIM4

}


void TIM4_LightPWM_Init(u32 arr,u32 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	//使能定时器4时钟   
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 	//使能GPIOD的时钟	
  
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_14;//TIM4_CH1和TIM4_CH3
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;//模拟输出模式
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;//上拉输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_InitStructure);//初始化GPIO
	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_TIM4);
	
	//初始化TIM4
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;   //TIM向上计数模式
	TIM_TimeBaseInitStructure.TIM_Period=arr-1;   //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc-1;   //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;   //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	
	
	//初始化TIM5 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;////选择定时器模式:TIM脉冲宽度调制模式1,小于的事有效电平
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;//比较输出使能
//	TIM_OCInitStructure.TIM_Pulse=100;//设置比较难
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//输出极性:TIM输出比较极性低
	TIM_OC1Init(TIM4,&TIM_OCInitStructure);
	TIM_OC3Init(TIM4,&TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);// 使能TIM4在CCR2上的预装载寄存器
	TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);// 使能TIM4在CCR2上的预装载寄存器
	
	TIM_ARRPreloadConfig(TIM4,ENABLE); //使能TIM4的计数寄存器
	TIM_Cmd(TIM4,ENABLE); //使能TIM4

}

