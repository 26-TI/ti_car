/**
 * @file heartbeat_task.cpp
 * @brief 心跳任务：LED 500 ms 翻转，两路电子开关 3 s 高 / 3 s 低
 *
 * 时序都在本文件顶部的常量里，改这里即可。
 */

#include "task/app_tasks.hpp"

#include "FreeRTOS.h" // IWYU pragma: keep
#include "task.h"

#include "board_cfg.hpp"
#include "board_log.hpp"

namespace
{
  constexpr uint32_t kTickMs        = 10U;   ///< 任务节拍
  constexpr uint32_t kLedToggleMs   = 500U;  ///< LED 翻转周期
  constexpr uint32_t kSwitchPhaseMs = 3000U; ///< 电子开关高/低相位时长
} // namespace

void heartbeat_task(void *arg)
{
  (void)arg;

  uint32_t led_ms    = 0U;
  uint32_t switch_ms = 0U;
  bool     led_on    = false;
  bool     sw_high   = false;

  bsp_led.write(led_on);
  bsp_sw_5v.write(sw_high);
  bsp_sw_vcc.write(sw_high);

  /* 打印口自检：看到这一行说明 UART_7 与 TX DMA 都通了 */
  board_printf("\r\n[MSPM0G3519_CAR] boot ok, build %s %s\r\n", __DATE__, __TIME__);

  TickType_t last = xTaskGetTickCount();

  for (;;)
  {
    vTaskDelayUntil(&last, pdMS_TO_TICKS(kTickMs));

    /* ---- LED：每 kLedToggleMs 翻转一次 ---- */
    led_ms += kTickMs;
    if (led_ms >= kLedToggleMs)
    {
      led_ms = 0U;
      led_on = !led_on;
      bsp_led.write(led_on);
    }

    /* ---- 电子开关：高 kSwitchPhaseMs → 低 kSwitchPhaseMs ---- */
    switch_ms += kTickMs;
    if (switch_ms >= kSwitchPhaseMs)
    {
      switch_ms = 0U;
      sw_high   = !sw_high;
      bsp_sw_5v.write(sw_high);
      bsp_sw_vcc.write(sw_high);
    }
  }
}
