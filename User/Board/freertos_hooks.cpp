/**
 * @file freertos_hooks.cpp
 * @brief FreeRTOS 故障钩子（堆分配失败 / 栈溢出）
 *
 * @note 发生故障时把现场留在 g_rtos_fault 里并停住，调试器可以直接连上看变量。
 *       不要在钩子里调用可能再次申请内存的接口（例如带 %f 的 printf）。
 */

#include <stdint.h>

#include "FreeRTOS.h" // IWYU pragma: keep
#include "task.h"

extern "C"
{

  /** @brief RTOS 故障码：0=无, 1=堆分配失败, 2=栈溢出 */
  volatile uint32_t g_rtos_fault = 0U;
  /** @brief 故障任务句柄（栈溢出时记录肇事任务） */
  volatile void *g_rtos_fault_task = nullptr;

  /**
   * @brief 运行时间统计计数器（FreeRTOSConfig.h 的 portGET_RUN_TIME_COUNTER_VALUE）
   *
   * @note 1 tick = 1000 单位，由 tick hook 驱动，见 cpu_monitor_task 的使用。
   */
  volatile unsigned long CPU_RunTime = 0UL;

  /**
   * @brief 节拍钩子（configUSE_TICK_HOOK = 1）
   *
   * @note 每 tick +1000，使 CPU_RunTime 的单位是 µs（与 FreeRTOSConfig.h 的说明一致）。
   */
  void vApplicationTickHook(void)
  {
    CPU_RunTime = CPU_RunTime + 1000UL;
  }

  /**
   * @brief 堆分配失败钩子
   */
  void vApplicationMallocFailedHook(void)
  {
    g_rtos_fault = 1U;
    taskDISABLE_INTERRUPTS();
    for (;;)
    {
    }
  }

  /**
   * @brief 栈溢出钩子
   * @param xTask 发生溢出的任务句柄
   * @param pcTaskName 任务名
   */
  void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
  {
    (void)pcTaskName;
    g_rtos_fault = 2U;
    g_rtos_fault_task = (void *)xTask;
    taskDISABLE_INTERRUPTS();
    for (;;)
    {
    }
  }
}
