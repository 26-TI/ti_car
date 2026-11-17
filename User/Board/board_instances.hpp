/**
 * @file board_instances.hpp
 * @brief M0BSP 实例清单（**唯一**出现 SysConfig 实例名的地方）
 *
 * @note 中断入口由 M0BSP 的 bsp_instances.hpp 按这里的清单展开，所以：
 *       - 增删外设：改这两份清单即可
 *       - 在 SysConfig GUI 里改名：改这里的 pfx，库代码不用动
 *       - pfx 必须是 SysConfig 的实例前缀（不含 _INST），例如 UART_3
 *       - bufsize 必须与下方 board_cfg.cpp 里 BspUart<> 的模板实参一致
 */

#ifndef __BOARD_INSTANCES_HPP__
#define __BOARD_INSTANCES_HPP__

/* ---- UART：X(实例前缀, TX DMA 缓冲区大小) ---- */
#define M0BSP_UART_INSTANCES(X) \
  X(UART_0, 64)                 \
  X(UART_1, 64)                 \
  X(UART_3, 64)                 \
  X(UART_4, 64)                 \
  X(UART_7, 64)

/* ---- CAN：X(实例前缀) ---- */
#define M0BSP_CAN_INSTANCES(X) \
  X(MCAN0)

#endif /* __BOARD_INSTANCES_HPP__ */
