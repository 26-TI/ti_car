/**
 * @file hardware_config.h
 * @brief 本板唯一配置入口：外设参数、功能开关、任务参数
 *
 * @note 这里出现的每个宏都必须与 SysConfig（SysConfig/MSPM0G3519_CAR.syscfg）
 *       以及 board_instances.hpp 的实例清单保持一致：
 *       - M0BSP_UART_BUF_SIZE 必须等于 board_instances.hpp 里 X(pfx, bufsize) 的 bufsize
 *       - M0BSP_PWM_A0_PERIOD 是 TIMA0 的装载值，改它等于改 PWM 频率
 */

#ifndef __HARDWARE_CONFIG_H__
#define __HARDWARE_CONFIG_H__

/* ==================== UART ==================== */

/** @brief UART TX DMA 缓冲区大小（与 board_instances.hpp 一致） */
#define M0BSP_UART_BUF_SIZE 64U

/* ==================== PWM ==================== */

/** @brief TIMA0（PWM_A0）装载值：频率 = 80 MHz / (period) */
#define M0BSP_PWM_A0_PERIOD 5000U
/** @brief PWM_A0 使用的通道数（SysConfig 里配置了 CC0~CC3） */
#define M0BSP_PWM_A0_CHANNELS 4U

/* ==================== 蜂鸣器（device 层，TIMG12） ==================== */

/** @brief 蜂鸣器发声频率 (Hz) */
#define M0BSP_BUZZER_FREQ_HZ 2700U
/** @brief 蜂鸣器 PWM 装载值 = 定时器时钟 / 发声频率 */
#define M0BSP_BUZZER_PERIOD (80000000U / M0BSP_BUZZER_FREQ_HZ)

/* ==================== 系统任务 ==================== */

/** @brief CPU 监控任务栈深度（单位：StackType_t） */
#define M0BSP_CPU_MON_STACK 0x100U
/** @brief CPU 监控任务优先级（建议只高于空闲任务） */
#define M0BSP_CPU_MON_PRIO 1U

/* ==================== 功能开关 ==================== */

/** @brief 1=启用 CPU 监控任务 */
#define M0BSP_ENABLE_CPU_MONITOR 1
/** @brief 1=启用 CAN（MCAN0） */
#define M0BSP_ENABLE_CAN 1

#endif /* __HARDWARE_CONFIG_H__ */
