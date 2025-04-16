#ifndef PID_
#define PID_

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"

// int类型信号的pid
typedef struct
{
 float Target; // 期望
 float P; // 比例系数
 float I; // 积分系数
 float D; // 微分系数
 float T_Limit; // 积分限幅
 float Limit; // 计算结果限幅
 float Error; // 偏差 
 float LastError; // 上次的偏差值
 float PrevError; // 上上次偏差
 float SumError; // 累计误差
 float Realize; // 计算出的差值
} PidNode;

extern PidNode pid;
void PID_ParameterInit(PidNode *pid, float P, float I, float D, float Target, float T_Limit, float Limit);
float PID_PlaceFloat(PidNode *pid, float target, float now);
float PID_IncrFloat(PidNode *pid, float target, float now);


#endif
