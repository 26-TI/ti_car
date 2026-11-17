/**
 * @file app_task.cpp
 * @brief 应用层任务创建（工程默认只创建 M0BSP 的系统任务）
 */

#include "app_task.hpp"

#include "FreeRTOS.h" // IWYU pragma: keep
#include "task.h"

#include "hardware_config.h"
#include "tasks/task.hpp"

void app_init(void)
{
#if M0BSP_ENABLE_CPU_MONITOR
  xTaskCreate(cpu_monitor_task, "cpu_mon", M0BSP_CPU_MON_STACK, nullptr,
              M0BSP_CPU_MON_PRIO, nullptr);
#endif

  /* 业务任务请在 User/App/task/ 下实现，并在这里创建 */
}
