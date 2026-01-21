#ifndef __USER_PWM_H__
#define __USER_PWM_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "main.h"

/* 类型定义 ------------------------------------------------------------------*/

typedef struct {
    TIM_HandleTypeDef *htim;
    uint32_t channel;
    uint32_t clock;
    uint32_t freq;
    float duty;
} PWM_DRIVES;


/* 函数声明 ------------------------------------------------------------------*/
void PWM_Init(PWM_DRIVES *pwm, TIM_HandleTypeDef *htim, uint32_t channel, uint32_t tim_clock);
uint32_t PWM_SetDuty(PWM_DRIVES *pwm, float duty);
uint32_t PWM_SetFreq(PWM_DRIVES *pwm, const uint32_t freq);


#endif  // __USER_PWM_H__