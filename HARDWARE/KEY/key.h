#ifndef   _KEY_H
#define   _KEY_H
#include  "stm32f4xx.h"

#define _limit(a, b, c) 				(a > b ? (a < c ? a : c) : b) //注意：b < c
 
/**   按键部分   **/

#define    KEY1_GPIO_RCC      RCC_AHB1Periph_GPIOE
#define    KEY1_GPIO_PORT     GPIOE
#define    KEY1_GPIO_PIN      GPIO_Pin_2

#define    KEY2_GPIO_RCC      RCC_AHB1Periph_GPIOE
#define    KEY2_GPIO_PORT     GPIOE
#define    KEY2_GPIO_PIN      GPIO_Pin_3

#define    KEY3_GPIO_RCC      RCC_AHB1Periph_GPIOE
#define    KEY3_GPIO_PORT     GPIOE
#define    KEY3_GPIO_PIN      GPIO_Pin_4

#define    KEY4_GPIO_RCC      RCC_AHB1Periph_GPIOE
#define    KEY4_GPIO_PORT     GPIOE
#define    KEY4_GPIO_PIN      GPIO_Pin_5

#define    KEY5_GPIO_RCC      RCC_AHB1Periph_GPIOE
#define    KEY5_GPIO_PORT     GPIOE
#define    KEY5_GPIO_PIN      GPIO_Pin_6

#define    KEY6_GPIO_RCC      RCC_AHB1Periph_GPIOC
#define    KEY6_GPIO_PORT     GPIOC
#define    KEY6_GPIO_PIN      GPIO_Pin_13


//定义按键状态
#define KEY_ON		0	//按键按下
#define KEY_OFF		1	//按键松开

#define KEY1_READ  GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY1_GPIO_PIN)
#define KEY2_READ  GPIO_ReadInputDataBit(KEY2_GPIO_PORT,KEY2_GPIO_PIN)
#define KEY3_READ  GPIO_ReadInputDataBit(KEY3_GPIO_PORT,KEY3_GPIO_PIN)
#define KEY4_READ  GPIO_ReadInputDataBit(KEY4_GPIO_PORT,KEY4_GPIO_PIN)
#define KEY5_READ  GPIO_ReadInputDataBit(KEY5_GPIO_PORT,KEY5_GPIO_PIN)
#define KEY6_READ  GPIO_ReadInputDataBit(KEY6_GPIO_PORT,KEY6_GPIO_PIN)



//按键功能
typedef enum
{
	KEY_NULL = 0,				//无按键按下
	KEY_START_STOP,	        //计时器开
	KEY_CLR,		        //计时器关
	KEY_PARA,		        //切换调节的参数
	KEY_PAGE,	          //切换页面
	KEY_ADD,						//加
	KEY_DECREASE,				//减
}KeyModeNode;

//按键结构体
typedef struct
{
	unsigned char long_press_mode;	//按键长按模式启动,1为长按启动，0为长按关闭
	unsigned char long_press_check;	//按键长按检查
	unsigned char long_press_time;	//按键长按检测时间
	
	unsigned char long_press_span;						
	
	KeyModeNode KeyValue;
}KeyMessageNode;

extern KeyMessageNode KeyMessage;

void Key_Init(void); //按键初始化

KeyModeNode Key_Scan(void); //全按键扫描

void DebugI(int *Regulated_quantity,int number);
void DebugC(char *Regulated_quantity,char number);


/**  拨码开关部分  **/

#define    SWITCH1_GPIO_RCC      RCC_AHB1Periph_GPIOD
#define    SWITCH1_GPIO_PORT     GPIOD
#define    SWITCH1_GPIO_PIN      GPIO_Pin_0

#define    SWITCH2_GPIO_RCC      RCC_AHB1Periph_GPIOD
#define    SWITCH2_GPIO_PORT     GPIOD
#define    SWITCH2_GPIO_PIN      GPIO_Pin_1

#define    SWITCH3_GPIO_RCC      RCC_AHB1Periph_GPIOD
#define    SWITCH3_GPIO_PORT     GPIOD
#define    SWITCH3_GPIO_PIN      GPIO_Pin_2

#define    SWITCH4_GPIO_RCC      RCC_AHB1Periph_GPIOD
#define    SWITCH4_GPIO_PORT     GPIOD
#define    SWITCH4_GPIO_PIN      GPIO_Pin_3

#define    SWITCH5_GPIO_RCC      RCC_AHB1Periph_GPIOD
#define    SWITCH5_GPIO_PORT     GPIOD
#define    SWITCH5_GPIO_PIN      GPIO_Pin_4

#define    SWITCH6_GPIO_RCC      RCC_AHB1Periph_GPIOD
#define    SWITCH6_GPIO_PORT     GPIOD
#define    SWITCH6_GPIO_PIN      GPIO_Pin_5


//读取拨码开关的状态（低电平为拨码ON）
#define    SWITCH1_READ    GPIO_ReadInputDataBit(SWITCH1_GPIO_PORT, SWITCH1_GPIO_PIN)
#define    SWITCH2_READ    GPIO_ReadInputDataBit(SWITCH2_GPIO_PORT, SWITCH2_GPIO_PIN)
#define    SWITCH3_READ    GPIO_ReadInputDataBit(SWITCH3_GPIO_PORT, SWITCH3_GPIO_PIN)
#define    SWITCH4_READ    GPIO_ReadInputDataBit(SWITCH4_GPIO_PORT, SWITCH4_GPIO_PIN)
#define    SWITCH5_READ    GPIO_ReadInputDataBit(SWITCH5_GPIO_PORT, SWITCH5_GPIO_PIN)
#define    SWITCH6_READ    GPIO_ReadInputDataBit(SWITCH6_GPIO_PORT, SWITCH6_GPIO_PIN)


void switch_Init(void);


#endif
