#include "headfile.h"
#include "User-interface.h"

// 硬件初始化
void BoardInit()
{
//  GPIO_InitTypeDef GPIO_InitStructure; //GPIO的初始化
  BEEP_Init();  //蜂鸣器初始化
  TIM4_LightPWM_Init(10000,168);   //照明装置初始化--频率50HZ
  TIM4_SERVOPWM_Init(10000,168);   //舵机初始化--频率50HZ
  uart_init(115200);   //串口1初始化，波特率为115200
  delay_init(168);   //delay函数的初始化
  OLED_Init();   //OLED的初始化
  LED_GPIO_Config();  //LED初始化
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);   //中断优先级组别为2（抢占优先级和响应优先级均为4位）
  //EXTIX_Init();   //外部中断初始化
	TIM3_Init(10000, 8400);   //定时器3初始化-----1s
  TIM2_Init(10000, 84);      //定时器2初始化----0.01s
	Key_Init();    //按键初始化
  switch_Init();    //拨码开关初始化
  DHT11_Init();     //温湿度模块初始化
} 


int main()
{
  BoardInit();
  TIM_SetCompare2(TIM4,300);
  TIM_SetCompare4(TIM4,300);
//  TIM_SetCompare1(TIM4,10000);
//  TIM_SetCompare3(TIM4,10000);
  while(1)
  {
		//oled_flag = 1;
    if(oled_flag != 0)
    {
      //OLED_P6x8Str(3, 2,"SMART HOME!");
			Key_Hub();
    }
    else
    {
      OLED_Cls();
    }
  }
}


