/**
 * @file oled_task.cpp
 * @brief OLED（I2C0）显示：helloworld + 编码器/按键计数
 *
 * 坐标约定（8x16 字模）：x = 字符列 (0~15)，y = 文本行 (0~3)。
 */

#include "task/app_tasks.hpp"

#include "FreeRTOS.h" // IWYU pragma: keep
#include "task.h"

#include "board_cfg.hpp"
#include "hardware_config.h"
#include "service/user_system.hpp"

namespace
{
  /** @brief 把一行补满 16 字符，避免数字变短时残留旧字符 */
  void pad_line(char *line)
  {
    size_t len = 0U;
    while (line[len] != '\0' && len < 16U)
    {
      ++len;
    }
    while (len < 16U)
    {
      line[len++] = ' ';
    }
    line[16] = '\0';
  }
} // namespace

void oled_task(void *arg)
{
  (void)arg;

  if (!device_oled.init(&bsp_i2c_oled, M0BSP_OLED_ADDR))
  {
    vTaskDelete(nullptr);
    return;
  }

  device_oled.clear();
  device_oled.show_string(0U, 0U, "helloworld");
  device_oled.refresh();

  char line[17];

  for (;;)
  {
    user_system::format(line, sizeof(line), "SA/SB: %lu", (unsigned long)g_encoder_count);
    pad_line(line);
    device_oled.show_string(0U, 1U, line);

    user_system::format(line, sizeof(line), "SS/KEY: %lu", (unsigned long)g_switch_press_count);
    pad_line(line);
    device_oled.show_string(0U, 2U, line);

    device_oled.refresh();

    vTaskDelay(pdMS_TO_TICKS(M0BSP_OLED_REFRESH_MS));
  }
}
