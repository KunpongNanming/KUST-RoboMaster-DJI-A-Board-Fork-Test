/* 包含头文件 ----------------------------------------------------------------*/
#include "../Inc/user_pwm.h"
#include "../../Core/Inc/bsp.h"

/* 函数体 --------------------------------------------------------------------*/

/**
* @brief 初始化 PWM 驱动
* @param pwm       PWM 驱动结构体指针
* @param htim      定时器硬件句柄
* @param channel   PWM 通道
* @param tim_clock 定时器时钟频率  (总线时钟频率)
*/
void PWM_Init(PWM_DRIVES *pwm, TIM_HandleTypeDef *htim, const uint32_t channel, const uint32_t tim_clock){
    pwm->htim = htim;
    pwm->channel = channel;
    pwm->clock = tim_clock / (htim->Init.Prescaler + 1);
    pwm->freq = pwm->clock / (__HAL_TIM_GET_AUTORELOAD(pwm->htim) + 1);
    pwm->duty = (float) __HAL_TIM_GET_COMPARE(pwm->htim, pwm->channel) / (float) (pwm->htim->Init.Period + 1);
    
    HAL_TIM_PWM_Start(htim, channel);
}

/**
* @brief 设置 PWM 占空比
* @param pwm  PWM 驱动结构体指针
* @param duty PWM 占空比 (0.0 ~ 1.0)
* @return 比较寄存器的值
*/
uint32_t PWM_SetDuty(PWM_DRIVES *pwm, const float duty){
    pwm->duty = duty;
    const uint32_t compare_reg = (uint32_t)((float)(pwm->htim->Init.Period + 1) * duty);
    __HAL_TIM_SET_COMPARE(pwm->htim, pwm->channel, compare_reg);
    return compare_reg;
}

/**
* @brief 设置 PWM 频率
* @param pwm  PWM 驱动结构体指针
* @param freq PWM 输出频率 单位: Hz
* @return 自动重载寄存器的值
*/
uint32_t PWM_SetFreq(PWM_DRIVES *pwm, const uint32_t freq){
    pwm->freq = freq;
    const uint32_t reload_reg = pwm->clock / freq - 1;
    __HAL_TIM_SET_AUTORELOAD(pwm->htim, reload_reg);
    return reload_reg;
}

