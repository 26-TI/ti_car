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
  note(M0BSP_BOOT_TUNE_F1, M0BSP_BOOT_TUNE_NOTE_MS);
  note(M0BSP_BOOT_TUNE_F2, M0BSP_BOOT_TUNE_NOTE_MS);
  note(M0BSP_BOOT_TUNE_F3, M0BSP_BOOT_TUNE_NOTE_MS);
}
