#ifndef _DHT11_H
#define _DHT11_H
#include "sys.h"
#include "delay.h"
#include "usart.h"

#define DHT11_GPIO_RCC      RCC_AHB1Periph_GPIOD
#define GPIO_DHT11          GPIOD
#define IO_DHT11            GPIO_Pin_6

#define DHT11_DQ_High     GPIO_SetBits(GPIO_DHT11,IO_DHT11)
#define DHT11_DQ_Low      GPIO_ResetBits(GPIO_DHT11,IO_DHT11)

typedef struct
{
  u8 tempereture;
  u8 humidity;
  
}DHT11Node;


extern DHT11Node DHT11;
//extern DHT11Node DHT11_threshold;
extern int tempereture;
extern int humidity;

void DHT11_IO_OUT(void);
void DHT11_IO_IN (void);
void DHT11_Init(void);
void DHT11_Rst(void);
void getTem_Humi(void);

u8 DHT11_Read_Data(u8 *temp,u8 *humi);
u8 DHT11_Read_Byte(void);
u8 DHT11_Read_Bit(void);
u8 DHT11_Check(void);



#endif



