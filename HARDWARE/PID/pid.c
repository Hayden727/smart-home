#include "pid.h"

#define _limit(a, b, c) (a > b ? (a < c ? a : c) : b) //注意：b < c
/*
* @func PID参数初始化
*/

PidNode pid;

void PID_ParameterInit(PidNode *pid, float P, float I, float D, float Target, float T_Limit, float Limit)//PID参数的初始化
{
 //指针对应
 pid->P = P;
 pid->I = I;
 pid->D = D;
 pid->Target = Target;
 pid->T_Limit = T_Limit;
 pid->Limit = Limit;

 pid->LastError = 0;
 pid->PrevError = 0;
 pid->SumError = 0;
}
// 位置式PID控制
float PID_PlaceFloat(PidNode *pid, float target, float now)
{
 pid->Error = target - now; // 计算当前误差
 pid->SumError += pid->Error; // 误差积分
 pid->SumError = _limit(pid->SumError, -pid->T_Limit, pid->T_Limit);

 pid->Realize = (pid->P * pid->Error
 + pid->I * pid->SumError
 + pid->D * (pid->Error - pid->LastError));
 pid->LastError = pid->Error; // 更新上次误差

 pid->Realize = _limit(pid->Realize, -pid->Limit, pid->Limit);
 return pid->Realize; // 返回输出实际值
}
// 增量式PID电机控制
float PID_IncrFloat(PidNode *pid, float target, float now)
{
 pid->Error = target - now; // 计算当前误差
 pid->Realize += (pid->P * (pid->Error - pid->LastError)
 + pid->I * pid->Error
 + pid->D * (pid->Error - 2 * pid->LastError + pid->PrevError));
 pid->PrevError = pid->LastError;// 更新前次误差
 pid->LastError = pid->Error; // 更新上次误差

 pid->Realize = _limit(pid->Realize, -pid->Limit, pid->Limit);
 return pid->Realize; // 返回输出实际值
}
