/* 包含头文件 ----------------------------------------------------------------*/
#include "../Inc/user_led.h"
#include "../../Core/Inc/bsp.h"

/* 函数体 --------------------------------------------------------------------*/

/**
* @brief 初始化 LED
* @param led         LED 驱动结构体指针
* @param GPIO        GPIO 端口
* @param pin         GPIO 引脚
* @param is_reversal 是否反转逻辑, 1 为反转, 0 为正常
*/
void LED_Init(LED_DRIVES* led, GPIO_TypeDef* GPIO, const uint16_t pin, const uint8_t is_reversal){
    led->GPIO = GPIO;
    led->pin = pin;
    led->is_reversal = is_reversal;
}

/**
* @brief 设置 LED 状态
* @param led   LED 驱动结构体指针
* @param state LED 状态
*/
void LED_StateSet(const LED_DRIVES* led, const LED_State state){
    if (led->is_reversal) {
        switch (state) {
            case LED_ON:
                HAL_GPIO_WritePin(led->GPIO, led->pin, GPIO_PIN_RESET);  /*LED 点亮*/
                break;
            case LED_OFF:
                HAL_GPIO_WritePin(led->GPIO, led->pin, GPIO_PIN_SET);    /*LED 熄灭*/
                break;
        }
    } else {
        switch (state) {
            case LED_ON:
                HAL_GPIO_WritePin(led->GPIO, led->pin, GPIO_PIN_SET);    /*LED 点亮*/
                break;
            case LED_OFF:
                HAL_GPIO_WritePin(led->GPIO, led->pin, GPIO_PIN_RESET);  /*LED 熄灭*/
                break;
        }
    }
}

/**
* @brief 点亮 LED
* @param led LED 驱动结构体指针
*/
void LED_On(const LED_DRIVES* led){
    if (led->is_reversal)
        HAL_GPIO_WritePin(led->GPIO, led->pin, GPIO_PIN_RESET);  /*LED 点亮*/
    else
        HAL_GPIO_WritePin(led->GPIO, led->pin, GPIO_PIN_SET);    /*LED 点亮*/
}

/**
* @brief 熄灭 LED
* @param led LED 驱动结构体指针
*/
void LED_Off(const LED_DRIVES* led){
    if (led->is_reversal)
        HAL_GPIO_WritePin(led->GPIO, led->pin, GPIO_PIN_SET);    /*LED 熄灭*/
    else
        HAL_GPIO_WritePin(led->GPIO, led->pin, GPIO_PIN_RESET);  /*LED 熄灭*/
}

/**
* @brief 翻转 LED 状态
* @param led LED 驱动结构体指针
*/
void LED_Toggle(const LED_DRIVES* led){
    HAL_GPIO_TogglePin(led->GPIO, led->pin);
}

/**
* @brief 获取 LED 状态
* @param led LED 驱动结构体指针
* @return LED 状态
*/
LED_State LED_StateGet(const LED_DRIVES* led){
    const GPIO_PinState pinState = HAL_GPIO_ReadPin(led->GPIO, led->pin);

    if (led->is_reversal)
        switch (pinState) {
            case GPIO_PIN_RESET:
                return LED_ON;
            case GPIO_PIN_SET:
                return LED_OFF;
        }
    else
        switch (pinState) {
            case GPIO_PIN_SET:
                return LED_ON;
            case GPIO_PIN_RESET:
                return LED_OFF;
        }

    return LED_OFF;
}