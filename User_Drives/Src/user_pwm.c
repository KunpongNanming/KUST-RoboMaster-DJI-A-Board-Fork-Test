/* 包含头文件 ----------------------------------------------------------------*/
#include "../Inc/user_pwm.h"
#include "../../Core/Inc/bsp.h"

/* 函数体 --------------------------------------------------------------------*/
void PWM_Init(PWM_DRIVES *pwm, TIM_HandleTypeDef *htim, const uint32_t channel, const uint32_t tim_clock){
    pwm->htim = htim;
    pwm->channel = channel;
    pwm->clock = tim_clock / (htim->Init.Prescaler + 1);
    HAL_TIM_PWM_Start(htim,channel);
}

uint32_t PWM_SetDuty(PWM_DRIVES *pwm, const float duty){
    pwm->duty = duty;
    const uint32_t compare_reg = (uint32_t)((float)(pwm->htim->Init.Period + 1) * duty);
    __HAL_TIM_SET_COMPARE(pwm->htim, pwm->channel, compare_reg);
    return compare_reg;
}

uint32_t PWM_SetFreq(PWM_DRIVES *pwm, const uint32_t freq){
    pwm->freq = freq;
    const uint32_t reload_reg = pwm->clock / freq - 1;
    __HAL_TIM_SET_AUTORELOAD(pwm->htim, reload_reg);
    return reload_reg;
}

