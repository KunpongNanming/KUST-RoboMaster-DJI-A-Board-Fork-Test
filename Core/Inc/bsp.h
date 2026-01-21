#ifndef __USER_BSP_H__
#define __USER_BSP_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "main.h"
#include "../../User_Algorithm/Inc/user_coord.h"

/* 全局注册表 ----------------------------------------------------------------*/
#define MAX_LOOP_EVENT 32
void LOOP_EVENT_Handle(void);
typedef void (*LOOP_Event)(void);
extern LOOP_Event loop_event[MAX_LOOP_EVENT];
extern uint8_t loop_event_num;

/* 接口定义 ------------------------------------------------------------------*/

// 状态灯
#include "../../User_Drives/Inc/user_led.h"
extern LED_DRIVES red_led;
extern LED_DRIVES green_led;



#endif //__USER_BSP_H__
