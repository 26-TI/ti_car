/**
 * @file main.cpp
 * @brief 系统入口 —— 硬件初始化 + 启动调度器
 *
 * 初始化流程：
 *   1. SYSCFG_DL_init()   SysConfig 生成的时钟 / GPIO / 外设初始化
 *   2. board_init()       M0BSP 驱动对象绑定 + NVIC 使能（必须在调度器启动前）
 *   3. app_init()         业务/系统任务创建（工程默认只建系统任务）
 *   4. vTaskStartScheduler() 启动调度器（永不返回）
 */

#include "ti_msp_dl_config.h" // IWYU pragma: keep

#include "FreeRTOS.h" // IWYU pragma: keep
#include "task.h"

#include "app_task.hpp"
#include "board_cfg.hpp"
#include "board_log.hpp"

int main(void)
{
  SYSCFG_DL_init();
  board_trace("\r\n[boot] syscfg ok\r\n");

  board_init();
  board_trace("[boot] board ok, fault=");
  board_trace_hex((unsigned int)g_board_fault);
  board_trace("\r\n");

  app_init();
  board_trace("[boot] app ok, heap=");
  board_trace_hex((unsigned int)xPortGetFreeHeapSize());
  board_trace("\r\n");

  vTaskStartScheduler();

  /* 正常不会返回；返回说明空闲/定时器任务创建失败（通常是堆不够） */
  board_trace("[boot] SCHEDULER RETURNED, heap=");
  board_trace_hex((unsigned int)xPortGetFreeHeapSize());
  board_trace("\r\n");

  while (1)
  {
  }
}
