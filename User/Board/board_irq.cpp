/**
 * @file board_irq.cpp
 * @brief GPIO 中断入口（MSPM0 的 GPIOA 与 GPIOB 共用 GROUP1 向量）
 *
 * 为什么放在宿主而不是库里：
 *   这个向量是**共享**的，库里再定义一次就会和宿主重复定义。
 *   库只提供 BspGpioIrq::dispatch()，由宿主决定分发哪些端口。
 */

#include "bsp/bsp_gpio_irq.hpp"

extern "C" void GROUP1_IRQHandler(void)
{
  BspGpioIrq::dispatch(GPIOA);
  BspGpioIrq::dispatch(GPIOB);
}
