/*
 * @Description: 
 * @FilePath: oled.c
 * @Author: Wei Zhou
 * @Github: https://github.com/zromyk
 * @Date: 2020-06-19 21:19:49
 * @LastEditors: Wei Zhou
 * @LastEditTime: 2020-06-19 21:21:49
 * @Copyright: Copyright © 2017 muyiro. All rights reserved.
 */ 

#ifndef __OLED_H
#define __OLED_H
#include "stm32f4xx.h"

//此OLED程序适合6/7脚OLED（7脚OLED的CS脚需要设置为低电平才使能有效）
//此处选择使用的是K60还是STM32
//#define K60
#define STM32

#ifdef K60
//初始化引脚定义
//直接在这里更改IO口即可
#define SCL_PIN PTA29
#define SDA_PIN PTA28
#define RST_PIN PTA27
#define DC_PIN  PTA26

#define SCL_OUT PTA29_OUT
#define SDA_OUT PTA28_OUT
#define RST_OUT PTA27_OUT
#define DC_OUT  PTA26_OUT

#define OLED_SCL_Init  gpio_init(SCL_PIN,GPO,INIT) // 时钟初始化定义
#define OLED_SDA_Init  gpio_init(SDA_PIN,GPO,INIT) // 数据口D0
#define OLED_RST_Init  gpio_init(RST_PIN,GPO,INIT) // 复位低能电平
#define OLED_DC_Init   gpio_init(DC_PIN,GPO,INIT)  // 偏置常低

//IO口定义OUTPUT
//直接在这里改IO就可以了
#define OLED_SCLH  (SCL_OUT = 1)// 时钟定义
#define OLED_SCLL  (SCL_OUT = 0)

#define OLED_SDAH  (SDA_OUT = 1)// 数据口D0
#define OLED_SDAL  (SDA_OUT = 0)

#define OLED_RSTH  (RST_OUT = 1)// 复位低能电平
#define OLED_RSTL  (RST_OUT = 0)

#define OLED_DCH   (DC_OUT = 1)
#define OLED_DCL   (DC_OUT = 0) // 偏置常低

#else
#ifdef STM32

#define	SCL_GPIO_RCC	RCC_AHB1Periph_GPIOE
#define	SCL_GPIO_PORT	GPIOE
#define	SCL_GPIO_PIN	GPIO_Pin_15

#define	SDA_GPIO_RCC	RCC_AHB1Periph_GPIOE
#define	SDA_GPIO_PORT	GPIOE
#define	SDA_GPIO_PIN	GPIO_Pin_14

#define	RES_GPIO_RCC	RCC_AHB1Periph_GPIOE
#define	RES_GPIO_PORT	GPIOE
#define	RES_GPIO_PIN	GPIO_Pin_13

#define	DC_GPIO_RCC		RCC_AHB1Periph_GPIOE
#define	DC_GPIO_PORT	GPIOE
#define	DC_GPIO_PIN		GPIO_Pin_12

#define OLED_GPIO_RCC 	SCL_GPIO_RCC|SDA_GPIO_RCC|RES_GPIO_RCC|DC_GPIO_RCC

//OLED端口定义
//OLED的时钟引脚(SCL)
#define OLED_SCLL 	GPIO_ResetBits(SCL_GPIO_PORT,SCL_GPIO_PIN)
#define OLED_SCLH 	GPIO_SetBits(SCL_GPIO_PORT,SCL_GPIO_PIN)
//OLED的数据引脚(SDA)
#define OLED_SDAL 	GPIO_ResetBits(SDA_GPIO_PORT,SDA_GPIO_PIN)
#define OLED_SDAH 	GPIO_SetBits(SDA_GPIO_PORT,SDA_GPIO_PIN)
//OLED的复位引脚(RES)
#define OLED_RSTL 	GPIO_ResetBits(RES_GPIO_PORT,RES_GPIO_PIN)
#define OLED_RSTH 	GPIO_SetBits(RES_GPIO_PORT,RES_GPIO_PIN)
//OLED的数据/命令控制引脚(DC)
#define OLED_DCL 	GPIO_ResetBits(DC_GPIO_PORT,DC_GPIO_PIN)
#define OLED_DCH 	GPIO_SetBits(DC_GPIO_PORT,DC_GPIO_PIN)

#endif
#endif
/*
* @func OLED初始化
*/
extern void OLED_Init(void);
/*
* @func 清屏函数
*/
extern void OLED_Cls(void);
/*
* @func 写入一组6x8标准ASCII字符串
* @para y 行数
* @para x 列数
* @para ch[] 要显示的字符串
*/
extern void OLED_P6x8Str(unsigned char y, unsigned char x, char ch[]);
/*
* @func 显示一个int类型的数据
* @para y 行数
* @para x 列数
* @para length 设定的整个数据长度（右对齐）
* @para ival 数据
*/

extern void OLED_P6x8Char(unsigned char y, unsigned char x, char ch);



extern void OLED_P6x8Int(unsigned char y, unsigned char x, int32_t ival, unsigned char length);
/*
* @func 显示一个float类型的数据
* @para y 行数
* @para x 列数
* @para length 设定的整个数据长度（右对齐）
* @para lenFloat 小数位数
* @para dval 数据
*/
extern void OLED_P6x8Float(unsigned char y, unsigned char x, float dval, unsigned char length, unsigned char lenFloat);
/*
* @func 显示一副图像
* @para unsigned char 类型的数组（保存了图像信息）
* @para rowNum, columnNum
*/
extern void OLED_PrintImage(unsigned char *pucTable, unsigned char rowNum,  unsigned char columnNum);


#endif
