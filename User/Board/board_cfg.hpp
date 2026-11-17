/**
 * @file board_cfg.hpp
 * @brief 板级绑定：M0BSP 驱动对象与 SysConfig 实例的连接点
 *
 * @note 本文件 + board_cfg.cpp + board_instances.hpp 是**唯一**出现 SysConfig 实例名的地方。
 *       M0BSP 库内不出现任何板级宏；在 SysConfig GUI 里改名时只改这三处
 *       （清单在 board_instances.hpp，绑定在 board_cfg.cpp）。
 *
 * @note 只声明板上真实存在、且 SysConfig 里已配置的外设。
 *       本板 SysConfig 没有 LED / 按键 / MEN 的 GPIO 分组，因此不实例化 bsp_gpio / bsp_key。
 */

#ifndef __BOARD_CFG_HPP__
#define __BOARD_CFG_HPP__

#include "hardware_config.h"  // IWYU pragma: keep
#include "ti_msp_dl_config.h" // IWYU pragma: keep

#include "bsp/bsp_can.hpp"
#include "bsp/bsp_gpio.hpp"
#include "bsp/bsp_i2c.hpp"
#include "bsp/bsp_pwm.hpp"
#include "bsp/bsp_qei.hpp"
#include "bsp/bsp_uart.hpp"

#include "device/buzzer.hpp"
#include "device/iic_oled.hpp"

/* UART 对象模板实参必须与 board_instances.hpp 的 X(pfx, bufsize) 一致 */
static_assert(M0BSP_UART_BUF_SIZE == 64U,
              "M0BSP_UART_BUF_SIZE 必须等于 board_instances.hpp 里的 X(pfx, 64)");

/* ==================== UART（5 路，全部 TX DMA） ==================== */

extern BspUart<M0BSP_UART_BUF_SIZE> bsp_uart0; ///< UART_0：PA0/PA1
extern BspUart<M0BSP_UART_BUF_SIZE> bsp_uart1; ///< UART_1：PB4/PB5
extern BspUart<M0BSP_UART_BUF_SIZE> bsp_uart3; ///< UART_3：PB2/PB3（调试打印）
extern BspUart<M0BSP_UART_BUF_SIZE> bsp_uart4; ///< UART_4：PB10/PB11
extern BspUart<M0BSP_UART_BUF_SIZE> bsp_uart7; ///< UART_7：PB17/PB18

/* ==================== PWM ==================== */

extern BspPwm bsp_pwm_a0;     ///< PWM_A0（TIMA0，4 通道）
extern BspPwm bsp_pwm_buzzer; ///< PWM_G12_BUZZER（TIMG12，CC1 → PB14）

/* ==================== 蜂鸣器（device 层） ==================== */

extern DeviceBuzzer device_buzzer;

/* ==================== 编码器 ==================== */

extern BspQei bsp_qei_g8; ///< QEI_G8（TIMG8，PB15/PB16）
extern BspQei bsp_qei_g9; ///< QEI_G9（TIMG9，PB7/PB9）

/* ==================== I2C ==================== */

extern BspI2c bsp_i2c_oled; ///< I2C_0_OLED（实际控制器 I2C0，PB0/PB1）
extern BspI2c bsp_i2c_1;    ///< I2C_1（I2C1，PA3/PA4）
extern BspI2c bsp_i2c_2;    ///< I2C_2（I2C2，PA29/PA30）

/* ==================== CAN ==================== */

extern BspCan bsp_can; ///< MCAN0（CANFD0，PA12/PA13，经典 CAN 1 Mbps）

/* ==================== GPIO 输出 ==================== */

extern BspGpio bsp_led;    ///< LED（PB19，高电平点亮）
extern BspGpio bsp_sw_5v;  ///< 5V 电子开关（PA11，高电平导通）
extern BspGpio bsp_sw_vcc; ///< VCC 电子开关（PB6，高电平导通）

/* ==================== OLED（I2C0） ==================== */

extern IicOled device_oled; ///< SSD1306，地址 0x3C

/**
 * @brief 绑定全部驱动对象到 SysConfig 实例
 *
 * @note 必须在 SYSCFG_DL_init() 之后、vTaskStartScheduler() 之前调用：
 *       内部会创建 FreeRTOS StreamBuffer 并使能 NVIC。
 *       任何一步失败只记录到 g_board_fault，不阻塞启动。
 */
void board_init();

/* ==================== 初始化故障位图 ==================== */

/** @brief board_init() 失败项；调试器直接看 g_board_fault 即可 */
enum BoardFaultBits : uint32_t
{
  BOARD_FAULT_UART0     = 1U << 0,
  BOARD_FAULT_UART1     = 1U << 1,
  BOARD_FAULT_UART3     = 1U << 2,
  BOARD_FAULT_UART4     = 1U << 3,
  BOARD_FAULT_UART7     = 1U << 4,
  BOARD_FAULT_PWM_A0    = 1U << 5,
  BOARD_FAULT_BUZZER    = 1U << 6,
  BOARD_FAULT_QEI_G8    = 1U << 7,
  BOARD_FAULT_QEI_G9    = 1U << 8,
  BOARD_FAULT_I2C_OLED  = 1U << 9,
  BOARD_FAULT_I2C_1     = 1U << 10,
  BOARD_FAULT_I2C_2     = 1U << 11,
};

/** @brief 0 = 全部成功；位定义见 BoardFaultBits */
extern volatile uint32_t g_board_fault;

#endif /* __BOARD_CFG_HPP__ */
