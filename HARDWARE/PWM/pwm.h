#ifndef __PWM_H
#define __PWM_H
#include "sys.h"

//void TIM5_PWM_Init(u32 arr,u32 psc);
//void TIM1_PWM_Init(u32 arr,u32 psc);
void TIM4_SERVOPWM_Init(u32 arr,u32 psc);
void TIM4_LightPWM_Init(u32 arr,u32 psc);
void SetBrightness(uint8_t mode);
#endif


