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
   * @brief 方向自检页：边框 + 四角标注
   *
   * 画面含义（屏幕应当的“正方向”）：
   *   TL(左上)           TR(右上)
   *
   *   BL(左下)           BR(右下)
   * 另有上/下边框、左右边框，可看出是否镜像。
   */
  void draw_orientation_test(IicOled &o)
  {
    o.clear();

    /* 上/下边框（第 0 页与第 7 页整行） */
    for (uint8_t col = 0U; col < 128U; ++col)
    {
      o.set_pixel(col, 0U, 0xFFU);
      o.set_pixel(col, 7U, 0xFFU);
    }
    /* 左/右边框（第 0 列与第 127 列所有页） */
    for (uint8_t page = 1U; page < 7U; ++page)
    {
      o.set_pixel(0U, page, 0xFFU);
      o.set_pixel(127U, page, 0xFFU);
    }

    /* 四角文字（y=1 占第 2~3 页，y=2 占第 4~5 页，避开边框） */
    o.show_string(2U, 1U, "TL");
    o.show_string(13U, 1U, "TR");
    o.show_string(2U, 2U, "BL");
    o.show_string(13U, 2U, "BR");

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
