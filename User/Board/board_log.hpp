/**
 * @file board_log.hpp
 * @brief 调试打印出口 —— 固定走 UART_7（PB17 TX / PB18 RX，115200 8N1，TX DMA）
 *
 * 为什么单独包一层：
 *   - 打印口只有一个固定选择（UART_7），不要在业务里散落 `bsp_uart?`
 *   - 以后换打印口（改 SysConfig 实例名 / 换 UART）只改 board_log.cpp 一处
 *
 * @note 内部使用 `portMAX_DELAY` 等 FreeRTOS 阻塞 API，**只能在调度器启动后的任务上下文调用**；
 *       不要在中断、也不要在 `app_init()`（调度器启动前）里用。
 *
 * @note 缓冲区为 128 字节，超长内容会被截断（`format_v` 保证 '\0' 结尾）。
 */

#ifndef __BOARD_LOG_HPP__
#define __BOARD_LOG_HPP__

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * @brief 向打印串口输出一行格式化文本
   * @param fmt printf 子集格式串（支持 %d/%u/%x/%s/%c/%f，见 service/user_system）
   * @return 成功写出的字节数；<0 表示格式化失败
   */
  int board_printf(const char *fmt, ...);

  /**
   * @brief 启动早期跟踪：直接轮询写 UART_7（不用 DMA / StreamBuffer / 堆）
   *
   * @note 可在调度器启动之前、甚至怀疑堆失败时调用；只依赖 SysConfig 已初始化 UART_7。
   *       用于定位“复位后卡在哪一步”。
   */
  void board_trace(const char *s);

  /** @brief 以 0xXXXXXXXX 形式输出一个 32 位值（配合 board_trace 使用） */
  void board_trace_hex(unsigned int v);

#ifdef __cplusplus
}
#endif

#endif /* __BOARD_LOG_HPP__ */
