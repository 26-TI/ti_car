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

  board_init();

  app_init();

  vTaskStartScheduler();

  while (1)
  {
  }
}
