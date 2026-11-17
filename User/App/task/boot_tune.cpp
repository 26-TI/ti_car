/**
 * @file boot_tune.cpp
 * @brief 上电变调提示音（调度器启动前阻塞播放）
 *
 * 1000 → 1500 → 2000 Hz，每段 120 ms，由 DL_Common_delayCycles 精确延时。
 */

#include "task/app_tasks.hpp"

#include "ti_msp_dl_config.h" // IWYU pragma: keep

#include "board_cfg.hpp"
#include "hardware_config.h"

namespace
{
  /* ---- 上电变调音型（TIMG12 是 16 位/80 MHz，最低约 1220 Hz） ---- */
  constexpr uint32_t kNoteHz[3] = {1500U, 2000U, 2700U};
  constexpr uint32_t kNoteMs    = 120U;

  /** @brief 发一个音，阻塞 ms 毫秒 */
  void note(uint32_t hz, uint32_t ms)
  {
    device_buzzer.set_frequency(hz);
    device_buzzer.on();

    const uint32_t cycles = (M0BSP_CPU_HZ / 1000U) * ms;
    DL_Common_delayCycles(cycles);

    device_buzzer.off();
  }
} // namespace

void boot_tune_play(void)
{
  for (uint32_t hz : kNoteHz)
  {
    note(hz, kNoteMs);
  }
}
