/**
 * @file app_task.cpp
 * @brief 应用层任务创建（工程默认只创建 M0BSP 的系统任务）
 */

#include "app_task.hpp"

#include "FreeRTOS.h" // IWYU pragma: keep
#include "task.h"

#include "hardware_config.h"
#include "task/app_tasks.hpp"
#include "tasks/task.hpp"

void app_init(void)
{
#if APP_ENABLE_INPUT
  /* 上电变调提示音（阻塞约 360 ms，此时还没起调度器） */
  boot_tune_play();
  /* EXTI 注册 + 队列 + NVIC：放在响铃之后，避免播放期间进中断 */
  input_init();
#endif

#if M0BSP_ENABLE_CPU_MONITOR
  xTaskCreate(cpu_monitor_task, "cpu_mon", M0BSP_CPU_MON_STACK, nullptr,
              M0BSP_CPU_MON_PRIO, nullptr);
#endif

#if APP_ENABLE_HEARTBEAT
  xTaskCreate(heartbeat_task, "heart", APP_HEARTBEAT_STACK, nullptr,
              APP_HEARTBEAT_PRIO, nullptr);
#endif

#if APP_ENABLE_INPUT
  xTaskCreate(input_task, "input", APP_INPUT_STACK, nullptr,
              APP_INPUT_PRIO, nullptr);
#endif

#if APP_ENABLE_OLED
  xTaskCreate(oled_task, "oled", APP_OLED_STACK, nullptr,
              APP_OLED_PRIO, nullptr);
#endif

  /* 业务任务请在 User/App/task/ 下实现，并在这里创建 */
}
