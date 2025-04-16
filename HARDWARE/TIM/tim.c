#include "tim.h"
#include "led.h"
#include "sys.h"
#include "key.h"
#include "Beep.h"
#include "DHT11.h"


//时钟变量初始化
ClockNode CLK_interface = 
{
	.year = 2021,    //闰年被四整除不被100整除
	.month = 11,    //月份数组
	.date = 17,
	.weekday = 3,
	.h = 10,
	.min = 0,
	.s = 0,
  .tomorrow_flag = 0
};

int month_days;
int month_1[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};      //闰年/非闰年每月天数
int month_2[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};      
int timer_flag = 0;   //计时器开始与结束

//计时器初始化(定时器2---0.01s)
ClockNode Millisecond = 
{
  .h = 0,
  .min = 0,
  .s = 0,
  .ms_10 = 0
};


//闹钟初始化
ClockNode Alarm = 
{
  .h = 0,
  .min = 0, 
  .s = 0
  
};


/*
--------------------定时器3初始化--------------------------------
*/
void TIM3_Init(u16 arr,u16 psc)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	TIM_TimeBaseInitStruct.TIM_Period=arr - 1;
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc - 1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//使能定时器3 中断为更新中断
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM3,ENABLE);
}	
/*
--------------------定时器2初始化--------------------------------
*/
void TIM2_Init(u16 arr,u16 psc)
{ 
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef        NVIC_InitTypeStructure;
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

	TIM_TimeBaseInitStructure.TIM_Period=arr-1;   
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc-1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	TIM_Cmd(TIM2,ENABLE);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); 
	
	NVIC_InitTypeStructure.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitTypeStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitTypeStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitTypeStructure.NVIC_IRQChannelSubPriority=3;
	NVIC_Init(&NVIC_InitTypeStructure);
}


void clock_24(void)
{
	CLK_interface.s += 1;
	if(CLK_interface.s > 59) 
	{
		CLK_interface.min += 1;
		CLK_interface.s  = 0;
		if(CLK_interface.min > 59)
		{
			CLK_interface.h += 1; 
			CLK_interface.min = 0;
      Beep_Bling_once();
			if(CLK_interface.h > 23)
			{
				CLK_interface.date += 1;
				CLK_interface.h = 0;
				if(CLK_interface.year % 4 == 0 && CLK_interface.year % 100 != 0)  //判断是否为闰年
				{
					month_days = month_2[CLK_interface.month - 1];
				}
				else month_days = month_1[CLK_interface.month - 1];
				if(CLK_interface.date > month_days)
				{
					CLK_interface.month += 1;
					CLK_interface.date = 1;
					if(CLK_interface.month > 12)
					{
						CLK_interface.year += 1;
						CLK_interface.month = 1;
					}
				}
			}
		}
		
	}
}

void clock_12(void)
{
	CLK_interface.s += 1;
	if(CLK_interface.s > 59) 
	{
		CLK_interface.min += 1;
		CLK_interface.s  = 0;
		if(CLK_interface.min > 59)
		{
			CLK_interface.h  += 1; 
			CLK_interface.min = 0;
      Beep_Bling_once();
			if(CLK_interface.h > 11)
			{				
				CLK_interface.h = 0;
				if(CLK_interface.tomorrow_flag % 2 == 1)
				{
					CLK_interface.date += 1;
					CLK_interface.tomorrow_flag = 0;
				}					
				if(CLK_interface.year % 4 == 0 && CLK_interface.year % 100 != 0)  //判断是否为闰年
				{
					month_days = month_2[CLK_interface.month - 1];
				}
				else month_days = month_1[CLK_interface.month - 1];
				if(CLK_interface.date > month_days)
				{
					CLK_interface.month += 1;
					CLK_interface.date = 1;
					if(CLK_interface.month > 12)
					{
						CLK_interface.year += 1;
						CLK_interface.month = 1;
					}
				}
			}
		}
		
	}
}


void Millisecond_clock(void)
{
  if(timer_flag % 2 ==1 ) Millisecond.ms_10 += 1;  
  if(Millisecond.ms_10 > 99)
  {
    Millisecond.s += 1;
    Millisecond.ms_10 = 0;
    if(Millisecond.s > 59)
    {
      Millisecond.min += 1;
      Millisecond.s = 0;
      if(Millisecond.min > 59)
      {
        Millisecond.h += 1;
        Millisecond.min = 0;
      }
    }
  }
}


int time_conv_flag = 1;

void TIM3_IRQHandler(void)
{
	month_days = month_1[CLK_interface.month - 1];
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
    //time_conv_flag = SWITCH6_READ;   //标记拨码开关状态反转
    if(CLK_interface.h >= 12) CLK_interface.tomorrow_flag = 1;
    if(CLK_interface.tomorrow_flag == 0 && CLK_interface.h == 11 && CLK_interface.min == 59 && CLK_interface.s == 59)
    {
      CLK_interface.tomorrow_flag = 1;
    }
    if(SWITCH6_READ == 0)
    {
      if(CLK_interface.tomorrow_flag%2 == 1 && SWITCH6_READ != time_conv_flag && CLK_interface.h > 12)
      {
        CLK_interface.h -= 12;
        time_conv_flag = SWITCH6_READ;   //记忆上一次拨码开关的状态
      }
    clock_12();
    }
    else
    {
      if(CLK_interface.tomorrow_flag%2 == 1 && SWITCH6_READ != time_conv_flag && CLK_interface.h < 12)
      {
        CLK_interface.h += 12;
        time_conv_flag = SWITCH6_READ;   //记忆上一次拨码开关的状态
      }
     clock_24();
     if(CLK_interface.h == Alarm.h && CLK_interface.min == Alarm.min && CLK_interface.s == Alarm.s) Beep_Bling();
    }
    int alarm_flag = 0;
    getTem_Humi();
    if(DHT11.tempereture >= tempereture || DHT11.humidity >= humidity)
    {
      alarm_flag = 1;
      if(DHT11.tempereture >= tempereture && DHT11.humidity >= humidity)
      {
        alarm_flag = 2;
      }
    }
    if(alarm_flag == 1) LED_Bling(1);
    if(alarm_flag == 2) Beep_Bling_once();
  }
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);   //清除TIM3更新中断标志
}


void TIM2_IRQHandler(void)
{
  if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
  {   
    if(Millisecond.h > 12)
    {
      Millisecond.h = 0;
    }      
    Millisecond_clock(); 
  }
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);   //清除TIM2更新中断标志
}


