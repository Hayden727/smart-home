#include "User-interface.h"
#include "oled.h"
#include "tim.h"
#include "key.h"
#include "delay.h"
#include "DHT11.h"

UserinterfaceNode UserInterface = 
{
  .PageNum = 4,
  .paraNum = 5
};

int Para = 0;

//时钟界面
void Clock_Interface(void)
{
	OLED_P6x8Str(1, 3, "Custom_clock!");
	OLED_P6x8Str(3, 1, "Date:");
	OLED_P6x8Int(4, 2, CLK_interface.year, 4);
	OLED_P6x8Str(4, 8, "-");
	OLED_P6x8Int(4, 9, CLK_interface.month , 2);
	OLED_P6x8Str(4, 13, "-");
	OLED_P6x8Int(4, 14, CLK_interface.date, 2);
	OLED_P6x8Str(5, 1, "Time:");
	OLED_P6x8Int(6, 2, CLK_interface.h, 2);
	OLED_P6x8Str(6, 5, ":");
	OLED_P6x8Int(6, 6, CLK_interface.min, 2);
	OLED_P6x8Str(6, 9, ":");
	OLED_P6x8Int(6, 10, CLK_interface.s, 2);
  OLED_P6x8Int(7, 15, Para % UserInterface.paraNum, 1);
}

void timer_clock(void)
{
  
  OLED_P6x8Str(1, 5, "TIMER!!");
  OLED_P6x8Str(3, 0, "Hour:");
  OLED_P6x8Int(3, 5, Millisecond.h, 2);
  //OLED_P6x8Str(5, 5, ":");
  OLED_P6x8Int(5, 2, Millisecond.min, 2);
  OLED_P6x8Str(5, 5, ":");
  OLED_P6x8Int(5, 6, Millisecond.s, 2);
  OLED_P6x8Str(5, 9, ":");
  OLED_P6x8Int(5, 10, Millisecond.ms_10, 2); 
  OLED_P6x8Int(7, 15, Para % UserInterface.paraNum, 1);
}

void Alarm_clock(void)
{
  OLED_P6x8Str(1, 5, "Alarm clock!!");
  OLED_P6x8Int(5, 2, Alarm.h, 2);
	OLED_P6x8Str(5, 5, ":");
	OLED_P6x8Int(5, 6, Alarm.min, 2);
	OLED_P6x8Str(5, 9, ":");
	OLED_P6x8Int(5, 10, Alarm.s, 2);
  OLED_P6x8Int(7, 15, Para % UserInterface.paraNum, 1);
}

void Tem_Hum_DBG(void)
{
  OLED_P6x8Str(1, 3, "TEM&HUMI-Debug");
  OLED_P6x8Str(3, 0, "Temp:");
  OLED_P6x8Int(3, 4, (int)DHT11.tempereture, 2);
  OLED_P6x8Str(4, 0, "Humi:");
  OLED_P6x8Int(4, 4, (int)DHT11.humidity, 2);  
  OLED_P6x8Str(5, 0, "Threshold:");
  OLED_P6x8Str(6, 0, "Tem:");
  OLED_P6x8Int(6, 4, tempereture, 2);
  OLED_P6x8Str(6, 9, "Humi:");
  OLED_P6x8Int(6, 14, humidity, 2);
  OLED_P6x8Int(7, 15, Para % UserInterface.paraNum, 1);
}

void Key_Hub(void)
{

  delay_ms(168);
  if((KeyMessage.KeyValue = Key_Scan()) == KEY_PAGE)
  {   
    ++UserInterface.PageStatus;
    UserInterface.PageStatus = (PAGE_STATUS)(UserInterface.PageStatus % UserInterface.PageNum);
    OLED_Cls();
  }
  if(KeyMessage.KeyValue == KEY_PARA) Para += 1;
    switch (UserInterface.PageStatus)
    {
      case Custom_clock:
      {
        UserInterface.paraNum = 6;     
        Clock_Interface();
        switch(Para % UserInterface.paraNum)
        {
          case(0):
          {
            DebugI(&CLK_interface.year, 1);
            break;               
          }
          case(1):
          {
            if(CLK_interface.month <= 12 && CLK_interface.month > 0) DebugI(&CLK_interface.month, 1); 
            else CLK_interface.month = 1;
            break;
          }
          case(2):
          {
            if(CLK_interface.year % 4 == 0 && CLK_interface.year % 100 != 0)  //判断是否为闰年
            {
              month_days = month_2[CLK_interface.month - 1];
            }
            else month_days = month_1[CLK_interface.month - 1];
            if(CLK_interface.date <= month_days && CLK_interface.date > 0) DebugI(&CLK_interface.date, 1); 
            else CLK_interface.date = 1;
            break;
          }
          case(3):
          {
            if(CLK_interface.h < 24 && CLK_interface.h >= 0) DebugI(&CLK_interface.h, 1); 
            else CLK_interface.h = 0;
            break;
          }
          case(4):
          {
            if(CLK_interface.min < 60 && CLK_interface.min >= 0) DebugI(&CLK_interface.min, 1); 
            else CLK_interface.min = 0;
            break;
          }
          case(5):
          {
            if(CLK_interface.s < 60 && CLK_interface.s > 0) DebugI(&CLK_interface.s, 1); 
            else CLK_interface.s = 0;
            break;
          }         
       }
         break;
      }
      case Timer_clock:
      {
        timer_clock();
        if(KeyMessage.KeyValue == KEY_START_STOP) 
        {
          timer_flag += 1;    //决定定时器的使能与失能
        }
        if(timer_flag > 8)
        {
          Millisecond.h = 0;
          Millisecond.min = 0;
          Millisecond.s = 0;
          Millisecond.ms_10 = 0;
          timer_flag = 0;
        }
        break;
      }
      case Alarm_Clock:
      {
        UserInterface.paraNum = 3;
        Alarm_clock();
        switch(Para % UserInterface.paraNum)
        {
          case(0):
          {
            if(Alarm.h < 24 && Alarm.h >= 0) DebugI(&Alarm.h, 1); 
            else Alarm.h = 0;
            break;
          }
          case(1):
          {
            if(Alarm.min < 60 && Alarm.min >= 0) DebugI(&Alarm.min, 1); 
            else Alarm.min = 0;
            break;
          }
          case(2):
          {
            if(Alarm.s < 60 && Alarm.s >= 0) DebugI(&Alarm.s, 1); 
            else Alarm.s = 0;
            break;
          }
        }
        break;
      }
      case Tem_Hum:
      {
        UserInterface.paraNum = 2;
        Tem_Hum_DBG();
        switch(Para % UserInterface.paraNum)
        {
          case(0):
          {
            DebugI(&tempereture, 1);
            break;
          }
          case(1):
          {
            DebugI(&humidity, 2);
            break;
          }
        }
        break;
      }
        
    } 
}


