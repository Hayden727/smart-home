#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"

void TIM3_Init(u16 arr,u16 psc);
void TIM2_Init(u16 arr,u16 psc);

typedef struct
{
	int year;
	int month;
	int date;
	int weekday;
	int h;
	int min;
	int s;
  int ms_10;
  int tomorrow_flag;  //
}ClockNode;

extern ClockNode CLK_interface;
extern ClockNode Millisecond;
extern ClockNode Alarm;
extern int month_1[12];
extern int month_2[12];
extern int month_days;
extern int timer_flag;   //计时器开始与结束
#endif

