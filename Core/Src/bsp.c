/* 包含头文件 ----------------------------------------------------------------*/
#include "bsp.h"
#include "bsp_config.h"
#include <stdio.h>

/* 全局注册表 --------------------------------------------------------------*/
void (*loop_event[MAX_LOOP_EVENT])(void) = {0};
uint8_t loop_event_num = 0;

void LOOP_EVENT_Handle(void) {
    for (uint8_t event_index = 0 ; event_index < loop_event_num ; event_index++) {
        loop_event[event_index]();
    }
}

/* 接口定义 --------------------------------------------------------------------*/

// 调试串口
UART_DRIVES user_debug_uart = {0};
void user_debug_uart_callback(void * user_uart) {
    UART_DRIVES *uart = (UART_DRIVES*)user_uart;
    uint8_t message[UART_BUFFER_SIZE] = {0};
    const uint16_t size = RBuffer_GetWithLen(&uart->rx_ringBuffer,message,RBuffer_GetLength(&uart->rx_ringBuffer));
    if (size) {
        char temp[UART_BUFFER_SIZE+32] = {0};
        sprintf(temp, "message: %s\ndata.size: %d\n", (char *)message,size);
        UART_QSend(uart, temp);
    }
}

// 大疆 DR16 接收机
DBUS_DRIVES user_dbus_DR16 = {0};

// 状态灯
LED_DRIVES red_led = {0};
LED_DRIVES green_led = {0};

// can 总线
CAN_DRIVES user_can_1 = {0};
void user_can_1_callback(void * user_can) {
    CAN_DRIVES *can = (CAN_DRIVES*)user_can;
    DJI_Motor_Handle(can);
}

CAN_DRIVES user_can_2 = {0};
CAN_CONNECTION can_connection = {0};
void user_can_2_callback(void * user_can) {
    const CAN_DRIVES *can = (CAN_DRIVES*)user_can;
    if (can->rx_msg.StdId == CAN_CONNET_ID) {
        const uint8_t *receive_data = can->rx_msg.Data;

        can_connection.ch2 = (int16_t) (receive_data[0] << 0 | receive_data[1] << 8);
        can_connection.ch3 = (int16_t) (receive_data[2] << 0 | receive_data[3] << 8);
        can_connection.ch0 = (int16_t) (receive_data[4] << 0 | receive_data[5] << 8);
        can_connection.ch1 = (int16_t) (receive_data[6] << 0 | receive_data[7] << 8);
    }
}

// 大疆电机
DJI_MOTOR_DRIVES FR_GM6020 = {0};
DJI_MOTOR_DRIVES FL_GM6020 = {0};
DJI_MOTOR_DRIVES RR_GM6020 = {0};
DJI_MOTOR_DRIVES RL_GM6020 = {0};

DJI_MOTOR_DRIVES FR_M3508 = {0};
DJI_MOTOR_DRIVES FL_M3508 = {0};
DJI_MOTOR_DRIVES RR_M3508 = {0};
DJI_MOTOR_DRIVES RL_M3508 = {0};

DJI_MOTOR_DRIVES YAW_GM6020 = {0};

// 舵轮底盘
SwerveChassisState user_swerve_chassis = {0};










