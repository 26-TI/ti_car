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
#include "service/user_system.hpp"

namespace
{
  /* ---- 显示屏配置 ---- */
  constexpr uint8_t  kOledAddr  = 0x3CU; ///< SSD1306 从机地址
  constexpr uint32_t kRefreshMs = 200U;  ///< 刷新周期

  /**
   * @brief 屏幕方向（面板级：段重映射 + COM 扫描）
   *
   * 以 FPC 在产品左侧、折到背面焊接的安装方向为准（实机照片确认）：
   * 画面相对目标角度整体 180° → 面板取 A1 + C8（ROT_180）。
   * 若换屏或改成 FPC 在右侧，试 ROT_0 / MIRROR_X / MIRROR_Y。
   */
  constexpr IicOled::Rotation kOledRotation = IicOled::Rotation::ROT_180;

  /**
   * @brief 字模镜像（只影响单个字形，不影响字符位置）
   *
   * 面板方向调好后字形即为正，默认 NONE；
   * 仅当单字还反时再改成 H / V / HV。
   */
  constexpr IicOled::FontFlip kFontFlip = IicOled::FontFlip::NONE;

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

  /** @brief 是否显示方向自检页（确认方向后改 false） */
  constexpr bool kShowOrientationTest = true;

  /**
   * @brief 方向自检页：只用几何图形，不依赖字形，便于照片判定
   *
   * 缓冲里的布局（屏幕应有的正方向）：
   *   ━━━━━━━━━━━━━━━━━━━━━  ← 第 0 页整条横粗线（上边）
   *   ▌████                     ← 左边竖粗线（第 0 列）+ 左上大块(32x16)
   *   ▌
   *   ▌                     █  ← 右下小块(8x16)
   *
   * 判定：
   *   - 横粗线在上面 → 上下正常；在下面 → 上下镜像
   *   - 竖粗线在左边 → 左右正常；在右边 → 左右镜像
   *   - 大块在左上、小块在右下 → 与上面两条一致（交叉验证）
   */
  void draw_orientation_test(IicOled &o)
  {
    o.clear();

    /* 1) 上边整条横粗线（第 0 页） */
    for (uint8_t col = 0U; col < 128U; ++col)
    {
      o.set_pixel(col, 0U, 0xFFU);
    }

    /* 2) 左边竖粗线（第 0~1 列，第 1~7 页） */
    for (uint8_t page = 1U; page < 8U; ++page)
    {
      o.set_pixel(0U, page, 0xFFU);
      o.set_pixel(1U, page, 0xFFU);
    }

    /* 3) 左上大块：第 4~35 列 × 第 1~2 页（32 x 16） */
    for (uint8_t col = 4U; col < 36U; ++col)
    {
      o.set_pixel(col, 1U, 0xFFU);
      o.set_pixel(col, 2U, 0xFFU);
    }

    /* 4) 右下小块：第 112~119 列 × 第 6~7 页（8 x 16） */
    for (uint8_t col = 112U; col < 120U; ++col)
    {
      o.set_pixel(col, 6U, 0xFFU);
      o.set_pixel(col, 7U, 0xFFU);
    }

    o.refresh();
  }
} // namespace

void oled_task(void *arg)
{
  (void)arg;

  if (!device_oled.init(&bsp_i2c_oled, kOledAddr, kOledRotation))
  {
    vTaskDelete(nullptr);
    return;
  }

  device_oled.set_font_flip(kFontFlip);

  if (kShowOrientationTest)
  {
    /* 方向自检：四角标注 + 边框，拍一张照片即可确定 Rotation/FontFlip */
    draw_orientation_test(device_oled);
    for (;;)
    {
      vTaskDelay(pdMS_TO_TICKS(1000));
    }
  }

  device_oled.clear();
  device_oled.show_string(0U, 0U, "helloworld");
  device_oled.refresh();

  char line[17];

  for (;;)
  {
    user_system::format(line, sizeof(line), "ENC: %ld", (long)input_encoder_count());
    pad_line(line);
    device_oled.show_string(0U, 1U, line);

    user_system::format(line, sizeof(line), "SS/KEY: %lu", (unsigned long)g_switch_press_count);
    pad_line(line);
    device_oled.show_string(0U, 2U, line);

    device_oled.refresh();

    vTaskDelay(pdMS_TO_TICKS(kRefreshMs));
  }
}
