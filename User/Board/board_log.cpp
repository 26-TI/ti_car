/**
 * @file board_log.cpp
 * @brief 调试打印实现：格式化后从 UART_7 的 TX DMA 发出
 */

#include "board_log.hpp"

#include <stdarg.h>

#include "board_cfg.hpp"
#include "service/user_system.hpp"

namespace
{
  /** @brief 单次打印的最大长度（含 '\0'） */
  constexpr size_t kLogBufSize = 128U;
}

int board_printf(const char *fmt, ...)
{
  if (fmt == nullptr)
  {
    return -1;
  }

  char buf[kLogBufSize];

  va_list args;
  va_start(args, fmt);
  const int n = user_system::format_v(buf, sizeof(buf), fmt, args);
  va_end(args);

  if (n <= 0)
  {
    return n;
  }

  /* BspUart::send() 内部走 StreamBuffer + TX DMA；必须处于任务上下文（调度器已启动） */
  const size_t sent = bsp_uart7.send(reinterpret_cast<const uint8_t *>(buf),
                                     static_cast<size_t>(n));
  return (sent == static_cast<size_t>(n)) ? n : -1;
}
