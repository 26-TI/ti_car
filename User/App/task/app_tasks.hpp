/**
 * @file app_tasks.hpp
 * @brief 应用层任务入口（心跳 / 输入事件 / OLED / 上电音）
 */

#ifndef __APP_TASKS_HPP__
#define __APP_TASKS_HPP__

#include <stdint.h>

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

  /** @brief 编码器 SA/SB 边沿计数（中断里累加） */
  extern volatile uint32_t g_encoder_count;
  /** @brief SS + KEY 松开次数（中断里累加） */
  extern volatile uint32_t g_switch_press_count;

#ifdef __cplusplus
}
#endif

#endif /* __APP_TASKS_HPP__ */
