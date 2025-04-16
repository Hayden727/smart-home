#ifndef User_INTERFACE_H
#define User_INTERFACE_H



typedef enum
{
		Custom_clock = 0,     
		Timer_clock,      
		Alarm_Clock, 
    Tem_Hum
  
}PAGE_STATUS;

typedef struct
{
  PAGE_STATUS PageStatus;
  int PageNum;
  int paraNum;

}UserinterfaceNode;


void Clock_Interface(void);
void timer_clock(void);
void Alarm_clock(void);
void Key_Hub(void);
void Tem_Hum_DBG(void);


#endif

