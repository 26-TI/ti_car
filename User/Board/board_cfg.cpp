/**
 * @file board_cfg.cpp
 * @brief 板级绑定：把 M0BSP 驱动对象绑到 SysConfig 生成的外设实例
 *
 * 约定：
 *   - 参数一律取自 SysConfig 生成的宏（UART_0_INST / DMA_CH0_CHAN_ID …），不写裸数字
 *   - init() 只做软件校验与引用保存，不碰硬件寄存器（硬件由 SYSCFG_DL_init() 配好）
 *   - 只绑定 SysConfig 里真实存在的外设；板子没有的东西不初始化
 */

#include "board_cfg.hpp"

/* ==================== 对象定义 ==================== */

BspUart<M0BSP_UART_BUF_SIZE> bsp_uart0;
BspUart<M0BSP_UART_BUF_SIZE> bsp_uart1;
BspUart<M0BSP_UART_BUF_SIZE> bsp_uart3;
BspUart<M0BSP_UART_BUF_SIZE> bsp_uart4;
BspUart<M0BSP_UART_BUF_SIZE> bsp_uart7;

BspPwm     bsp_pwm_a0;
BspPwm     bsp_pwm_buzzer;
BspQei     bsp_qei_g8;
BspQei     bsp_qei_g9;
BspI2c     bsp_i2c_oled;
BspI2c     bsp_i2c_1;
BspI2c     bsp_i2c_2;
BspCan     bsp_can;
DeviceBuzzer device_buzzer;

BspGpio  bsp_led;
BspGpio  bsp_sw_5v;
BspGpio  bsp_sw_vcc;
IicOled  device_oled;

/* ==================== 绑定 ==================== */

void board_init()
{
  /* ---- UART：115200 8N1，RX 中断 + TX DMA ---- */
  bsp_uart0.init({UART_0_INST, UART_0_INST_INT_IRQN, DMA_CH0_CHAN_ID, UART_0_INST_DMA_TRIGGER});
  bsp_uart1.init({UART_1_INST, UART_1_INST_INT_IRQN, DMA_CH1_CHAN_ID, UART_1_INST_DMA_TRIGGER});
  bsp_uart3.init({UART_3_INST, UART_3_INST_INT_IRQN, DMA_CH2_CHAN_ID, UART_3_INST_DMA_TRIGGER});
  bsp_uart4.init({UART_4_INST, UART_4_INST_INT_IRQN, DMA_CH3_CHAN_ID, UART_4_INST_DMA_TRIGGER});
  bsp_uart7.init({UART_7_INST, UART_7_INST_INT_IRQN, DMA_CH4_CHAN_ID, UART_7_INST_DMA_TRIGGER});

  /* ---- PWM：TIMA0 四通道 ---- */
  bsp_pwm_a0.init({PWM_A0_INST,
                   PWM_A0_INST_CLK_FREQ,
                   M0BSP_PWM_A0_PERIOD,
                   M0BSP_PWM_A0_CHANNELS,
                   {GPIO_PWM_A0_C0_IDX, GPIO_PWM_A0_C1_IDX, GPIO_PWM_A0_C2_IDX, GPIO_PWM_A0_C3_IDX}});

  /* ---- PWM：TIMG12 → 蜂鸣器（device 层封装） ---- */
  bsp_pwm_buzzer.init({PWM_G12_BUZZER_INST,
                       PWM_G12_BUZZER_INST_CLK_FREQ,
                       M0BSP_BUZZER_PERIOD,
                       1U,
                       {GPIO_PWM_G12_BUZZER_C1_IDX}});
  device_buzzer.init({&bsp_pwm_buzzer, 0U, M0BSP_BUZZER_PERIOD, PWM_G12_BUZZER_INST_CLK_FREQ});

  /* ---- 编码器 ---- */
  bsp_qei_g8.init({QEI_G8_INST});
  bsp_qei_g9.init({QEI_G9_INST});

  /* ---- I2C ---- */
  bsp_i2c_oled.init({I2C_0_OLED_INST});
  bsp_i2c_1.init({I2C_1_INST});
  bsp_i2c_2.init({I2C_2_INST});

  /* ---- GPIO 输出：LED 与两路电子开关（初始全低） ---- */
  bsp_led.init({LED_PORT, LED_PIN_LED_PIN});
  bsp_sw_5v.init({BREAKER_PIN_5V_BREAKER_PORT, BREAKER_PIN_5V_BREAKER_PIN});
  bsp_sw_vcc.init({BREAKER_PIN_VCC_BREAKER_PORT, BREAKER_PIN_VCC_BREAKER_PIN});
  bsp_led.reset();
  bsp_sw_5v.reset();
  bsp_sw_vcc.reset();

#if M0BSP_ENABLE_CAN
  /* ---- CAN：经典 CAN 1 Mbps ---- */
  bsp_can.init({MCAN0_INST, MCAN0_INST_INT_IRQN}, BspCan::NORMAL);
#endif
}
