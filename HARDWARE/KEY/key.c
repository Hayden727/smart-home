#include "key.h"
#include "delay.h"

/********按键部分********/

KeyMessageNode KeyMessage;

//按键初始化函数
void Key_Init()
{
	GPIO_InitTypeDef GPIO_InitTypeStructure;
	 //key1<----->key5
  RCC_AHB1PeriphClockCmd(KEY1_GPIO_RCC,ENABLE);
  
  GPIO_InitTypeStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitTypeStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitTypeStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitTypeStructure.GPIO_Pin=KEY1_GPIO_PIN|KEY2_GPIO_PIN|KEY3_GPIO_PIN|KEY4_GPIO_PIN|KEY5_GPIO_PIN;
	GPIO_Init(KEY1_GPIO_PORT,&GPIO_InitTypeStructure);
	 //key6时钟使能	
  RCC_AHB1PeriphClockCmd(KEY6_GPIO_PIN,ENABLE);
  
	GPIO_InitTypeStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitTypeStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitTypeStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitTypeStructure.GPIO_Pin=KEY6_GPIO_PIN;
	GPIO_Init(KEY6_GPIO_PORT,&GPIO_InitTypeStructure);
	
}

void switch_Init(void)
{
  GPIO_InitTypeDef GPIO_InitTypeStructure;
  RCC_AHB1PeriphClockCmd(SWITCH1_GPIO_RCC,ENABLE);
  
  GPIO_InitTypeStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitTypeStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitTypeStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitTypeStructure.GPIO_Pin=SWITCH1_GPIO_PIN|SWITCH2_GPIO_PIN|SWITCH3_GPIO_PIN|SWITCH4_GPIO_PIN|SWITCH5_GPIO_PIN|SWITCH6_GPIO_PIN;
  
	GPIO_Init(SWITCH1_GPIO_PORT,&GPIO_InitTypeStructure);
}

uint8_t Key_Scan_1(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON)
	{
		delay_ms(10);
		if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)==KEY_ON) return KEY_ON;		
	} 
	return KEY_OFF;
}	

//全按键扫描(进行功能返回)
KeyModeNode Key_Scan(void)
{
//  static u8 key_scan_flag = 1;	//按键扫描标志
//	if(key_scan_flag && (KEY1_READ==0 || KEY1_READ==0 || KEY1_READ==0 || KEY1_READ==0 || KEY1_READ==0 || KEY1_READ==0))
//{
//		delay_ms(10);	//消除抖动
//		key_scan_flag = 0;	//按键扫描关闭
//		//按键返回键值		
         if(KEY1_READ==KEY_ON)		return KEY_PARA;
		else if(KEY2_READ==KEY_ON)		return KEY_PAGE;
		else if(KEY3_READ==KEY_ON)		return KEY_ADD;
		else if(KEY4_READ==KEY_ON)		return KEY_START_STOP;
		else if(KEY5_READ==KEY_ON)		return KEY_DECREASE;
		else if(KEY6_READ==KEY_ON)		return KEY_CLR;
//	}
//	else if(KEY1_READ==1&&KEY2_READ==1&&KEY3_READ==1&&KEY4_READ==1&&KEY5_READ==1&&KEY6_READ==1) key_scan_flag = 1;
	return KEY_NULL;	
}


//调试函数      
//Regulated_quantity:被调量
//number：调节量

void DebugI(int *Regulated_quantity,int number) 
{
    if(KeyMessage.KeyValue==KEY_ADD)
      *Regulated_quantity+=number;
    if(KeyMessage.KeyValue==KEY_DECREASE)
      *Regulated_quantity-=number;
}

void DebugC(char *Regulated_quantity,char number) 
{
    if(KeyMessage.KeyValue==KEY_ADD)
      *Regulated_quantity+=number;
    if(KeyMessage.KeyValue==KEY_DECREASE)
      *Regulated_quantity-=number;
}




