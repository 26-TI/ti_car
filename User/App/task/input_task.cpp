/**
 * @file input_task.cpp
 * @brief EXTI 输入：编码器 SA/SB（只计数）、SS 与 KEY（松开触发提示音）
 *
 * 硬件层面（SysConfig 已配置，外部上拉）：
 *   - SA(PA9) / SB(PA7)：双边沿
 *   - SS(PA8) / KEY(PA31)：上升沿（松开）
 *
 * 中断上下文只做"计数 + 投递事件"，响蜂鸣器交给 input_task。
 */

#include "task/app_tasks.hpp"

#include "FreeRTOS.h" // IWYU pragma: keep
#include "queue.h"
#include "task.h"

#include "bsp/bsp_gpio_irq.hpp"
#include "board_cfg.hpp"
#include "hardware_config.h"

extern "C"
{
  volatile uint32_t g_encoder_count      = 0U;
  volatile uint32_t g_switch_press_count = 0U;
}

namespace
{
  QueueHandle_t s_events = nullptr;

  enum : uint8_t
  {
    EVT_ENCODER_SW = 1U, ///< 编码器按下开关松开
    EVT_KEY        = 2U, ///< 独立按键松开
  };

  void push_event(uint8_t evt)
  {
    if (s_events == nullptr)
    {
      return;
    }

    BaseType_t woken = pdFALSE;
    xQueueSendFromISR(s_events, &evt, &woken);
    portYIELD_FROM_ISR(woken);
  }

  /* ---- 中断回调（中断上下文） ---- */

  void on_encoder_a(void *)
  {
    g_encoder_count = g_encoder_count + 1U;
  }

  void on_encoder_b(void *)
  {
    g_encoder_count = g_encoder_count + 1U;
  }

  void on_encoder_sw(void *)
  {
    g_switch_press_count = g_switch_press_count + 1U;
    push_event(EVT_ENCODER_SW);
  }

  void on_key(void *)
  {
    g_switch_press_count = g_switch_press_count + 1U;
    push_event(EVT_KEY);
  }
} // namespace

void input_init(void)
{
  s_events = xQueueCreate(8U, sizeof(uint8_t));

  BspGpioIrq::attach({KEY_PORT, KEY_PIN_ENCODER_SA_PIN, on_encoder_a, nullptr});
  BspGpioIrq::attach({KEY_PORT, KEY_PIN_ENCODER_SB_PIN, on_encoder_b, nullptr});
  BspGpioIrq::attach({KEY_PORT, KEY_PIN_ENCODER_SS_PIN, on_encoder_sw, nullptr});
  BspGpioIrq::attach({KEY_PORT, KEY_PIN_KEY_PIN, on_key, nullptr});

  /* 外设级中断已在 SYSCFG_DL_GPIO_init() 里打开，这里补 NVIC */
  NVIC_EnableIRQ(KEY_INT_IRQN);
}

void input_task(void *arg)
{
  (void)arg;

  for (;;)
  {
    uint8_t evt = 0U;
    if (xQueueReceive(s_events, &evt, portMAX_DELAY) != pdTRUE)
    {
      continue;
    }

    /* SS / KEY 松开：短鸣一声（SA/SB 只计数，不响） */
    device_buzzer.set_frequency(M0BSP_BEEP_FREQ_HZ);
    device_buzzer.on();
    vTaskDelay(pdMS_TO_TICKS(M0BSP_BEEP_MS));
    device_buzzer.off();
  }
}
