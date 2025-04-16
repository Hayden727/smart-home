/*
 * @Description: 
 * @FilePath: /LQ_TC26xB_LIBtasking/src/AppSw/Tricore/MY/API/MPU6050.h
 * @Author: Wei Zhou
 * @Github: https://github.com/zromyk
 * @Date: 1970-01-01 08:00:00
 * @LastEditors: Wei Zhou
 * @LastEditTime: 2020-05-22 19:46:09
 * @Copyright: Copyright © 2017 muyiro. All rights reserved.
 */ 

#ifndef __MPU6050_H
#define __MPU6050_H

#include "stdint.h"


//#define STM32
#define K60

#ifdef STM32
#define MPU6050_SCL_RCC     RCC_AHB1Periph_GPIOB
#define MPU6050_SCL_PORT    GPIOB
#define MPU6050_SCL_PIN     GPIO_Pin_10

#define MPU6050_SDA_RCC     RCC_AHB1Periph_GPIOB
#define MPU6050_SDA_PORT    GPIOB
#define MPU6050_SDA_PIN     GPIO_Pin_11

#define MPU6050_GPIO_RCC    MPU6050_SCL_RCC|MPU6050_SDA_RCC

// gpio读写设置
#define MPU6050_READ_SDA()       GPIO_ReadInputDataBit (MPU6050_SDA_PORT, MPU6050_SDA_PIN)
#define MPU6050_SDA_L()          GPIO_ResetBits	(MPU6050_SDA_PORT, MPU6050_SDA_PIN)     //IO口输出低电平
#define MPU6050_SDA_H()          GPIO_SetBits	(MPU6050_SDA_PORT, MPU6050_SDA_PIN)		//IO口输出高电平
#define MPU6050_SCL_L()          GPIO_ResetBits	(MPU6050_SCL_PORT, MPU6050_SCL_PIN)     //IO口输出低电平
#define MPU6050_SCL_H()          GPIO_SetBits   (MPU6050_SCL_PORT, MPU6050_SCL_PIN)		//IO口输出高电平

#define MPU6050_SDA_OUT()           do {                                                    \
                                        GPIO_InitTypeDef GPIO_InitStructure;                \
                                        RCC_AHB1PeriphClockCmd(MPU6050_GPIO_RCC, ENABLE);   \
                                        GPIO_InitStructure.GPIO_Pin = MPU6050_SDA_PIN;      \
                                        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;       \
					GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      \
                                        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   \
                                        GPIO_Init(MPU6050_SDA_PORT, &GPIO_InitStructure);   \
                                    } while(0)
#define MPU6050_SDA_IN()            do {                                                    \
                                        GPIO_InitTypeDef GPIO_InitStructure;                \
                                        RCC_AHB1PeriphClockCmd(MPU6050_GPIO_RCC, ENABLE);   \
                                        GPIO_InitStructure.GPIO_Pin = MPU6050_SDA_PIN;      \
                                        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;        \
					GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        \
                                        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   \
                                        GPIO_Init(MPU6050_SDA_PORT, &GPIO_InitStructure);   \
                                    } while(0)
#define MPU6050_SCL_OUT()            do {                                                   \
                                        GPIO_InitTypeDef GPIO_InitStructure;                \
                                        RCC_AHB1PeriphClockCmd(MPU6050_GPIO_RCC, ENABLE);   \
                                        GPIO_InitStructure.GPIO_Pin = MPU6050_SCL_PIN;      \
                                        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;       \
					GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      \
                                        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   \
                                        GPIO_Init(MPU6050_SCL_PORT, &GPIO_InitStructure);   \
                                    } while(0)
                       
#endif
                                      
#ifdef K60

#define MPU6050_SCL_PIN        PTC12
#define MPU6050_SDA_PIN        PTC14
                                      
//gpio读写设置                                      
#define MPU6050_READ_SDA()       gpio_get(MPU6050_SDA_PIN)
#define MPU6050_SDA_L()          GPIO_SET(MPU6050_SDA_PIN,0)    //IO口输出低电平
#define MPU6050_SDA_H()          GPIO_SET(MPU6050_SDA_PIN,1)    //IO口输出高电平
#define MPU6050_SCL_L()          GPIO_SET(MPU6050_SCL_PIN,0)    //IO口输出低电平
#define MPU6050_SCL_H()          GPIO_SET(MPU6050_SCL_PIN,1)	//IO口输出高电平
                                      
#define MPU6050_SDA_OUT()        gpio_init(MPU6050_SDA_PIN,GPO,1)
#define MPU6050_SDA_IN()         gpio_init(MPU6050_SDA_PIN,GPI,1)
#define MPU6050_SCL_OUT()        gpio_init(MPU6050_SCL_PIN,GPO,1)
                                      
#endif

                                      
                                      
// 四元数数据结构体 //难懂呵呵
typedef struct
{
    float kp;           /*比例增益*/
    float ki;           /*积分增益*/
    float exInt;
    float eyInt;
    float ezInt;        /*积分误差累计*/

    float q0;           /*四元数*/
    float q1;
    float q2;
    float q3;   
    float rMat[3][3];   /*旋转矩阵*/
} ElemNode;

typedef struct
{
    struct 
    {
        int16_t     x;
        int16_t     y;
        int16_t     z;
    } acc, gyro;
    
    struct 
    {
        float     x;
        float     y;
        float     z;
    } accRaw, gyroRaw;

    struct
    {
        float       angle;
        float       gyro;
    } pitch, yaw, roll;

    ElemNode elem;
} MPUNode;

extern MPUNode mpu;

extern void MPU6050_Init(void);        //初始化MPU6050
extern void MPU6050_GetInfo(float dt);

#endif
