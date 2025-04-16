#include "DHT11.h"
#include "delay.h"
 
 
//DHT11数据初始化 
DHT11Node DHT11 = 
{
  .tempereture = 0,
  .humidity = 0
};

//DHT11Node DHT11_threshold = 
//{
//  .tempereture = 27,
//  .humidity = 80
//};
 
int tempereture = 26;
int humidity = 65;

 void DHT11_IO_IN (void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(DHT11_GPIO_RCC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = IO_DHT11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIO_DHT11, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIO_DHT11,IO_DHT11);
	
}

void DHT11_IO_OUT (void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(DHT11_GPIO_RCC, ENABLE);
	GPIO_InitStructure.GPIO_Pin =  IO_DHT11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIO_DHT11, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIO_DHT11,IO_DHT11);

}

void DHT11_Rst(void)
{
	DHT11_IO_OUT();
	DHT11_DQ_Low;
	delay_ms(20);
	DHT11_DQ_High;
	delay_us(30);

}

u8 DHT11_Check(void)
{
	u8 retry=0;
	DHT11_IO_IN();
	
	while ((GPIO_ReadInputDataBit(GPIO_DHT11, IO_DHT11)==1) && retry < 100)
	{
		retry++;
		delay_us(1);
	};
	
	if(retry>=100)
		return 1;
	else retry=0;
	
	while ((GPIO_ReadInputDataBit(GPIO_DHT11,IO_DHT11)==0)&&retry<100)
	{
		retry++;
		delay_us(1);
	};	
	if(retry>=100)
		return 1;
		return 0;
	
}

u8 DHT11_Read_Bit(void)
{
	u8 retry=0;
	while ((GPIO_ReadInputDataBit(GPIO_DHT11,IO_DHT11)==1)&&retry<100)
	{
		retry++;
		delay_us(1);
	};
	retry=0;
	while ((GPIO_ReadInputDataBit(GPIO_DHT11,IO_DHT11)==0)&&retry<100)
	{
		retry++;
		delay_us(1);
	};	
	delay_us(50);
	if(GPIO_ReadInputDataBit(GPIO_DHT11,IO_DHT11)==1)
	return 1;
	else 
	return 0;

}

u8 DHT11_Read_Byte(void)
{
	u8 i,dat;
	dat=0;
	for (i=0;i<8;i++)
	{
		dat<<=1;
		dat|=DHT11_Read_Bit();
	}
	return dat;
}

u8 DHT11_Read_Data(u8 *temp,u8 *humi)
{
	u8 buf[5];
	u8 i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for (i=0;i<5;i++)
		{
			buf[i]=DHT11_Read_Byte();		
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*temp=buf[2];
			*humi=buf[0];
		}		
	}
	else return 1;
	return 0;
}

void DHT11_Init(void)
{
	DHT11_Rst();
	DHT11_Check();
}

void getTem_Humi(void)
{
  DHT11_Read_Data(&DHT11.tempereture, &DHT11.humidity);
}


 
