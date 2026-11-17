/**
 * @file heartbeat_task.cpp
 * @brief 心跳任务：LED 500 ms 翻转，两路电子开关 3 s 高 / 3 s 低
 *
 * 所有时间都从 hardware_config.h 取，改那里即可，不用动逻辑。
 */

#include "task/app_tasks.hpp"

#include "FreeRTOS.h" // IWYU pragma: keep
#include "task.h"

#include "board_cfg.hpp"
#include "hardware_config.h"

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

  TickType_t last = xTaskGetTickCount();

  for (;;)
  {
    vTaskDelayUntil(&last, pdMS_TO_TICKS(M0BSP_HEARTBEAT_TICK_MS));

    /* ---- LED：每 M0BSP_LED_TOGGLE_MS 翻转一次 ---- */
    led_ms += M0BSP_HEARTBEAT_TICK_MS;
    if (led_ms >= M0BSP_LED_TOGGLE_MS)
    {
      led_ms = 0U;
      led_on = !led_on;
      bsp_led.write(led_on);
    }

    /* ---- 电子开关：高 M0BSP_SWITCH_PHASE_MS → 低 M0BSP_SWITCH_PHASE_MS ---- */
    switch_ms += M0BSP_HEARTBEAT_TICK_MS;
    if (switch_ms >= M0BSP_SWITCH_PHASE_MS)
    {
      switch_ms = 0U;
      sw_high   = !sw_high;
      bsp_sw_5v.write(sw_high);
      bsp_sw_vcc.write(sw_high);
    }
  }
}
