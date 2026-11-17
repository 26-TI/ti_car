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

/* ==================== 指示灯 / 电子开关 ==================== */

/** @brief LED 翻转周期 (ms)：500 ms 翻转一次 */
#define M0BSP_LED_TOGGLE_MS 500U
/** @brief 电子开关相位时间 (ms)：高 3 s → 低 3 s */
#define M0BSP_SWITCH_PHASE_MS 3000U
/** @brief 心跳任务节拍 (ms)，LED 与开关的时间基准 */
#define M0BSP_HEARTBEAT_TICK_MS 10U

/* ==================== 蜂鸣器音型 ==================== */

/** @brief 按键/编码器按下触发的提示音频率 (Hz) */
#define M0BSP_BEEP_FREQ_HZ 2700U
/** @brief 提示音时长 (ms) */
#define M0BSP_BEEP_MS 60U
/** @brief 上电变调：三个音高 (Hz)
 *  @note TIMG12 是 16 位、80 MHz 不分频，周期上限 65535 → 最低约 1220 Hz，
 *        所以音高不能低于 1.3 kHz。 */
#define M0BSP_BOOT_TUNE_F1 1500U
#define M0BSP_BOOT_TUNE_F2 2000U
#define M0BSP_BOOT_TUNE_F3 2700U
/** @brief 上电变调：每个音的时长 (ms) */
#define M0BSP_BOOT_TUNE_NOTE_MS 120U

/* ==================== OLED ==================== */

/** @brief OLED I2C 从机地址 */
#define M0BSP_OLED_ADDR 0x3CU
/** @brief OLED 刷新周期 (ms) */
#define M0BSP_OLED_REFRESH_MS 200U

/* ==================== 任务参数 ==================== */

/** @brief CPU 主频 (Hz)，用于调度器启动前的阻塞延时 */
#define M0BSP_CPU_HZ 80000000U

#define M0BSP_HEARTBEAT_STACK 0x100U
#define M0BSP_HEARTBEAT_PRIO 2U
#define M0BSP_INPUT_STACK 0x100U
#define M0BSP_INPUT_PRIO 2U
#define M0BSP_OLED_STACK 0x180U
#define M0BSP_OLED_PRIO 1U

/* ==================== 功能开关（新增） ==================== */

/** @brief 1=启用 LED/电子开关心跳任务 */
#define M0BSP_ENABLE_HEARTBEAT 1
/** @brief 1=启用 EXTI 输入（KEY/编码器）与提示音任务 */
#define M0BSP_ENABLE_INPUT 1
/** @brief 1=启用 OLED 显示任务 */
#define M0BSP_ENABLE_OLED 1

#endif /* __HARDWARE_CONFIG_H__ */
