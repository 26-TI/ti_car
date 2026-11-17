/**
 * @file app_tasks.hpp
 * @brief 应用层任务入口（心跳 / 输入事件 / OLED / 上电音）
 */

#ifndef __APP_TASKS_HPP__
#define __APP_TASKS_HPP__

#include <stdint.h>

/* ==================== 任务开关（应用层配置） ==================== */

#define APP_ENABLE_HEARTBEAT 1 ///< LED + 两路电子开关心跳
#define APP_ENABLE_INPUT 1     ///< EXTI 输入（编码器/按键）与提示音
#define APP_ENABLE_OLED 1      ///< OLED 显示

/* ==================== 任务参数 ==================== */

#define APP_HEARTBEAT_STACK 0x100U
#define APP_HEARTBEAT_PRIO 2U
#define APP_INPUT_STACK 0x100U
#define APP_INPUT_PRIO 2U
#define APP_OLED_STACK 0x180U
#define APP_OLED_PRIO 1U

#ifdef __cplusplus
extern "C"
{
#endif

  /** @brief 心跳任务：LED 500 ms 翻转 + 5V/VCC 电子开关 3 s 高 / 3 s 低 */
  void heartbeat_task(void *arg);

  /**
   * @brief 输入初始化（调度器启动前调用）
   *
   * 注册 SA/SB/SS/KEY 的 EXTI 回调、创建事件队列、使能 GPIO 中断。
   */
  void input_init(void);

  /** @brief 输入事件任务：SS/KEY 松开 → 蜂鸣器短鸣 */
  void input_task(void *arg);

  /** @brief OLED 任务：显示 helloworld 与 SA/SB、SS/KEY 计数 */
  void oled_task(void *arg);

  /** @brief 上电变调提示音（阻塞，调度器启动前调用） */
  void boot_tune_play(void);

  /** @brief 编码器 4 倍频累计位置（有符号，正转增/反转减） */
  int32_t input_encoder_count(void);

  /** @brief 取编码器增量（读后清零，任务里用） */
  int32_t input_encoder_take_delta(void);

  /** @brief 编码器计数清零 */
  void input_encoder_reset(void);

  /** @brief SS + KEY 松开次数（中断里累加） */
  extern volatile uint32_t g_switch_press_count;

#ifdef __cplusplus
}
#endif

#endif /* __APP_TASKS_HPP__ */
