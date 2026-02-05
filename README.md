# 大疆A板基础框架

该项目为RoboMaster大赛，昆明理工大学昆蓬南冥战队的大疆A板的基础框架。

## 硬件平台

- **MCU**: STM32F427IIH6 (180MHz, Cortex-M4)

## 项目结构

```
├── Core/                    # STM32 核心代码
├── Drivers/                 # STM32 HAL 驱动库
├── User_Algorithm/          # 算法模块
│   ├── user_coord.c/h       # 坐标变换
│   └── user_pid.c/h         # PID 控制器
├── User_Application/        # 应用层
│   └── swerve_chassis.c/h   # 舵轮底盘控制
├── User_Architect/          # 架构组件
│   ├── user_delay.c/h       # 延时功能
│   ├── user_queue.c/h       # 无锁队列
│   └── user_ring_buffe.c/h  # 环形缓冲区
└── User_Drives/             # 驱动层
    ├── user_can.c/h         # CAN 总线
    ├── user_dji_bus.c/h     # DBUS 协议
    ├── user_dji_motor.c/h   # 大疆电机驱动
    ├── user_led.c/h         # LED 控制
    ├── user_pwm.c/h         # PWM 输出
    └── user_uart.c/h        # UART 通信
```

## 许可证

本项目基于 STMicroelectronics 提供的 HAL 库开发。
