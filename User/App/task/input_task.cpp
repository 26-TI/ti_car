/**
 * @file input_task.cpp
 * @brief EXTI 输入：编码器 SA/SB（4 倍频正交解码）、SS 与 KEY（松开触发提示音）
 *
 * 硬件层面（SysConfig 已配置，外部上拉）：
 *   - SA(PA9) / SB(PA7)：双边沿 → 一个电气周期 4 个计数（4 倍频）
 *   - SS(PA8) / KEY(PA31)：上升沿（松开）
 *
 * 中断上下文只做"读电平查表累加 / 投递事件"，响蜂鸣器交给 input_task。
 */

#include "task/app_tasks.hpp"

#include "FreeRTOS.h" // IWYU pragma: keep
#include "queue.h"
#include "task.h"

#include "bsp/bsp_encoder_exti.hpp"
#include "bsp/bsp_gpio_irq.hpp"
#include "board_cfg.hpp"

extern "C"
{
  /** @brief SS + KEY 松开次数（中断里累加，OLED 显示用） */
  volatile uint32_t g_switch_press_count = 0U;
}

namespace
{
  /* ---- 本任务的业务配置 ---- */
  constexpr uint32_t kBeepFreqHz = 2700U; ///< 松开提示音频率
  constexpr uint32_t kBeepMs     = 60U;   ///< 提示音时长

  /* 编码器极性/方向：接线反了就改这三个 */
  constexpr bool kEncoderInvertA   = false; ///< A 相电平取反
  constexpr bool kEncoderInvertB   = false; ///< B 相电平取反
  constexpr bool kEncoderInvertDir = false; ///< 计数方向翻转

  QueueHandle_t  s_events = nullptr;
  BspEncoderExti s_encoder;

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

  /** @brief A/B 任一相跳变：4 倍频查表累加 */
  void on_encoder_edge(void *ctx)
  {
    static_cast<BspEncoderExti *>(ctx)->on_edge();
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

int32_t input_encoder_count(void)
{
  return s_encoder.count();
}

int32_t input_encoder_take_delta(void)
{
  return s_encoder.take_delta();
}

void input_encoder_reset(void)
{
  s_encoder.reset(0);
}

void input_init(void)
{
  s_events = xQueueCreate(8U, sizeof(uint8_t));

  /* 编码器：A/B 同端口、双边沿中断；极性/方向由 hardware_config.h 的宏选择 */
  s_encoder.init({KEY_PORT,
                  KEY_PIN_ENCODER_SA_PIN,
                  KEY_PIN_ENCODER_SB_PIN,
                  kEncoderInvertA,
                  kEncoderInvertB,
                  kEncoderInvertDir});

  BspGpioIrq::attach({KEY_PORT, KEY_PIN_ENCODER_SA_PIN, on_encoder_edge, &s_encoder});
  BspGpioIrq::attach({KEY_PORT, KEY_PIN_ENCODER_SB_PIN, on_encoder_edge, &s_encoder});
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
    device_buzzer.set_frequency(kBeepFreqHz);
    device_buzzer.on();
    vTaskDelay(pdMS_TO_TICKS(kBeepMs));
    device_buzzer.off();
  }
}
