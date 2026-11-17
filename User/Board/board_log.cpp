/**
 * @file board_log.cpp
 * @brief 调试打印实现：格式化后从 UART_7 的 TX DMA 发出
 */

#include "board_log.hpp"

#include <stdarg.h>

#include "board_cfg.hpp"
#include "service/user_system.hpp"

void board_trace(const char *s)
{
  if (s == nullptr)
  {
    return;
  }

  while (*s != '\0')
  {
    while (DL_UART_Main_isBusy(UART_7_INST))
    {
    }
    DL_UART_Main_transmitDataBlocking(UART_7_INST, static_cast<uint8_t>(*s));
    ++s;
  }
}

void board_trace_hex(unsigned int v)
{
  static const char kDigits[] = "0123456789ABCDEF";
  char              buf[11];

  buf[0] = '0';
  buf[1] = 'x';
  for (int i = 0; i < 8; ++i)
  {
    buf[2 + i] = kDigits[(v >> ((7 - i) * 4)) & 0xFU];
  }
  buf[10] = '\0';

  board_trace(buf);
}

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
