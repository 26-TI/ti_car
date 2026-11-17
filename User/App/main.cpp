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

/**
 * @brief 时钟初始化之前的探针：用复位默认时钟（SYSOSC）直接闪 LED
 *
 * 用途：冷启动（按复位/断电重上电）后若连 main 都没跑到，
 *       这里也不会闪；若能闪但 UART 没输出，说明卡在 SYSCFG_DL_SYSCTL_init()
 *       的晶振/PLL 等待里。
 *
 * @note 只依赖 IOMUX + GPIO，不需要任何外设时钟初始化。
 */
static void early_blink(unsigned times)
{
  DL_GPIO_initDigitalOutput(LED_PIN_LED_IOMUX);
  DL_GPIO_enableOutput(LED_PORT, LED_PIN_LED_PIN);

  for (unsigned i = 0U; i < times; ++i)
  {
    DL_GPIO_setPins(LED_PORT, LED_PIN_LED_PIN);
    DL_Common_delayCycles(8000000U); /* 复位默认 SYSOSC 32 MHz 下约 250 ms */
    DL_GPIO_clearPins(LED_PORT, LED_PIN_LED_PIN);
    DL_Common_delayCycles(8000000U);
  }
}

int main(void)
{
  /* 冷启动探针：上电后应先闪 3 下 */
  early_blink(3U);

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
