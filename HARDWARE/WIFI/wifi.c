#include "wifi.h"
#include "usart.h"
#include "delay.h"

void WIFI_Init(void)
{
	printf("AT+CWMODE=2/r/n");//AP模式
  printf("AT+RST/r/n");
	printf("AT+CWSAP='ESP8266','12345678',11,0/r/n");//建立热点名称，密码
  printf("AT+CIPMUX=1/r/n");//开启多连接
	printf("AT+CIPSTATUS/r/n");//返回5说明当前为AP模式
  printf("AT+CIPSERVER=1,8080/r/n");//建立客户端
  delay_ms(15000);
	printf("AT+CIPSEND=0,1/r/n");//开始传输
}

