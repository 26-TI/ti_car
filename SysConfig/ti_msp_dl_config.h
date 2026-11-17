/*
 * Copyright (c) 2023, Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ============ ti_msp_dl_config.h =============
 *  Configured MSPM0 DriverLib module declarations
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G351X
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_MSPM0G351X
#define CONFIG_MSPM0G3519

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform all required MSP DL initialization
 *
 *  This function should be called once at a point before any use of
 *  MSP DL.
 */


/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)


#define GPIO_HFXT_PORT                                                     GPIOA
#define GPIO_HFXIN_PIN                                             DL_GPIO_PIN_5
#define GPIO_HFXIN_IOMUX                                         (IOMUX_PINCM10)
#define GPIO_HFXOUT_PIN                                            DL_GPIO_PIN_6
#define GPIO_HFXOUT_IOMUX                                        (IOMUX_PINCM11)
#define CPUCLK_FREQ                                                     80000000
/* Defines for SYSPLL_ERR_01 Workaround */
/* Represent 1.000 as 1000 */
#define FLOAT_TO_INT_SCALE                                               (1000U)
#define FCC_EXPECTED_RATIO                                                  2000
#define FCC_UPPER_BOUND                       (FCC_EXPECTED_RATIO * (1 + 0.003))
#define FCC_LOWER_BOUND                       (FCC_EXPECTED_RATIO * (1 - 0.003))

bool SYSCFG_DL_SYSCTL_SYSPLL_init(void);


/* Defines for PWM_A0 */
#define PWM_A0_INST                                                        TIMA0
#define PWM_A0_INST_IRQHandler                                  TIMA0_IRQHandler
#define PWM_A0_INST_INT_IRQN                                    (TIMA0_INT_IRQn)
#define PWM_A0_INST_CLK_FREQ                                            80000000
/* GPIO defines for channel 0 */
#define GPIO_PWM_A0_C0_PORT                                                GPIOB
#define GPIO_PWM_A0_C0_PIN                                         DL_GPIO_PIN_8
#define GPIO_PWM_A0_C0_IOMUX                                     (IOMUX_PINCM25)
#define GPIO_PWM_A0_C0_IOMUX_FUNC                    IOMUX_PINCM25_PF_TIMA0_CCP0
#define GPIO_PWM_A0_C0_IDX                                   DL_TIMER_CC_0_INDEX
/* GPIO defines for channel 1 */
#define GPIO_PWM_A0_C1_PORT                                                GPIOB
#define GPIO_PWM_A0_C1_PIN                                        DL_GPIO_PIN_12
#define GPIO_PWM_A0_C1_IOMUX                                     (IOMUX_PINCM29)
#define GPIO_PWM_A0_C1_IOMUX_FUNC                    IOMUX_PINCM29_PF_TIMA0_CCP1
#define GPIO_PWM_A0_C1_IDX                                   DL_TIMER_CC_1_INDEX
/* GPIO defines for channel 2 */
#define GPIO_PWM_A0_C2_PORT                                                GPIOA
#define GPIO_PWM_A0_C2_PIN                                        DL_GPIO_PIN_10
#define GPIO_PWM_A0_C2_IOMUX                                     (IOMUX_PINCM21)
#define GPIO_PWM_A0_C2_IOMUX_FUNC                    IOMUX_PINCM21_PF_TIMA0_CCP2
#define GPIO_PWM_A0_C2_IDX                                   DL_TIMER_CC_2_INDEX
/* GPIO defines for channel 3 */
#define GPIO_PWM_A0_C3_PORT                                                GPIOB
#define GPIO_PWM_A0_C3_PIN                                        DL_GPIO_PIN_13
#define GPIO_PWM_A0_C3_IOMUX                                     (IOMUX_PINCM30)
#define GPIO_PWM_A0_C3_IOMUX_FUNC                    IOMUX_PINCM30_PF_TIMA0_CCP3
#define GPIO_PWM_A0_C3_IDX                                   DL_TIMER_CC_3_INDEX

/* Defines for PWM_G12_BUZZER */
#define PWM_G12_BUZZER_INST                                               TIMG12
#define PWM_G12_BUZZER_INST_IRQHandler                         TIMG12_IRQHandler
#define PWM_G12_BUZZER_INST_INT_IRQN                           (TIMG12_INT_IRQn)
#define PWM_G12_BUZZER_INST_CLK_FREQ                                    80000000
/* GPIO defines for channel 1 */
#define GPIO_PWM_G12_BUZZER_C1_PORT                                        GPIOB
#define GPIO_PWM_G12_BUZZER_C1_PIN                                DL_GPIO_PIN_14
#define GPIO_PWM_G12_BUZZER_C1_IOMUX                             (IOMUX_PINCM31)
#define GPIO_PWM_G12_BUZZER_C1_IOMUX_FUNC            IOMUX_PINCM31_PF_TIMG12_CCP1
#define GPIO_PWM_G12_BUZZER_C1_IDX                           DL_TIMER_CC_1_INDEX




/* Defines for QEI_G8 */
#define QEI_G8_INST                                                        TIMG8
#define QEI_G8_INST_IRQHandler                                  TIMG8_IRQHandler
#define QEI_G8_INST_INT_IRQN                                    (TIMG8_INT_IRQn)
/* Pin configuration defines for QEI_G8 PHA Pin */
#define GPIO_QEI_G8_PHA_PORT                                               GPIOB
#define GPIO_QEI_G8_PHA_PIN                                       DL_GPIO_PIN_15
#define GPIO_QEI_G8_PHA_IOMUX                                    (IOMUX_PINCM32)
#define GPIO_QEI_G8_PHA_IOMUX_FUNC                   IOMUX_PINCM32_PF_TIMG8_CCP0
/* Pin configuration defines for QEI_G8 PHB Pin */
#define GPIO_QEI_G8_PHB_PORT                                               GPIOB
#define GPIO_QEI_G8_PHB_PIN                                       DL_GPIO_PIN_16
#define GPIO_QEI_G8_PHB_IOMUX                                    (IOMUX_PINCM33)
#define GPIO_QEI_G8_PHB_IOMUX_FUNC                   IOMUX_PINCM33_PF_TIMG8_CCP1

/* Defines for QEI_G9 */
#define QEI_G9_INST                                                        TIMG9
#define QEI_G9_INST_IRQHandler                                  TIMG9_IRQHandler
#define QEI_G9_INST_INT_IRQN                                    (TIMG9_INT_IRQn)
/* Pin configuration defines for QEI_G9 PHA Pin */
#define GPIO_QEI_G9_PHA_PORT                                               GPIOB
#define GPIO_QEI_G9_PHA_PIN                                        DL_GPIO_PIN_7
#define GPIO_QEI_G9_PHA_IOMUX                                    (IOMUX_PINCM24)
#define GPIO_QEI_G9_PHA_IOMUX_FUNC                   IOMUX_PINCM24_PF_TIMG9_CCP0
/* Pin configuration defines for QEI_G9 PHB Pin */
#define GPIO_QEI_G9_PHB_PORT                                               GPIOB
#define GPIO_QEI_G9_PHB_PIN                                        DL_GPIO_PIN_9
#define GPIO_QEI_G9_PHB_IOMUX                                    (IOMUX_PINCM26)
#define GPIO_QEI_G9_PHB_IOMUX_FUNC                   IOMUX_PINCM26_PF_TIMG9_CCP1



/* Defines for I2C_0_OLED */
#define I2C_0_OLED_INST                                                     I2C0
#define I2C_0_OLED_INST_IRQHandler                               I2C0_IRQHandler
#define I2C_0_OLED_INST_INT_IRQN                                   I2C0_INT_IRQn
#define GPIO_I2C_0_OLED_SDA_PORT                                           GPIOB
#define GPIO_I2C_0_OLED_SDA_PIN                                    DL_GPIO_PIN_1
#define GPIO_I2C_0_OLED_IOMUX_SDA                                (IOMUX_PINCM13)
#define GPIO_I2C_0_OLED_IOMUX_SDA_FUNC                 IOMUX_PINCM13_PF_I2C0_SDA
#define GPIO_I2C_0_OLED_SCL_PORT                                           GPIOB
#define GPIO_I2C_0_OLED_SCL_PIN                                    DL_GPIO_PIN_0
#define GPIO_I2C_0_OLED_IOMUX_SCL                                (IOMUX_PINCM12)
#define GPIO_I2C_0_OLED_IOMUX_SCL_FUNC                 IOMUX_PINCM12_PF_I2C0_SCL

/* Defines for I2C_2 */
#define I2C_2_INST                                                          I2C2
#define I2C_2_INST_IRQHandler                                    I2C2_IRQHandler
#define I2C_2_INST_INT_IRQN                                        I2C2_INT_IRQn
#define GPIO_I2C_2_SDA_PORT                                                GPIOA
#define GPIO_I2C_2_SDA_PIN                                        DL_GPIO_PIN_30
#define GPIO_I2C_2_IOMUX_SDA                                      (IOMUX_PINCM5)
#define GPIO_I2C_2_IOMUX_SDA_FUNC                       IOMUX_PINCM5_PF_I2C2_SDA
#define GPIO_I2C_2_SCL_PORT                                                GPIOA
#define GPIO_I2C_2_SCL_PIN                                        DL_GPIO_PIN_29
#define GPIO_I2C_2_IOMUX_SCL                                      (IOMUX_PINCM4)
#define GPIO_I2C_2_IOMUX_SCL_FUNC                       IOMUX_PINCM4_PF_I2C2_SCL

/* Defines for I2C_1 */
#define I2C_1_INST                                                          I2C1
#define I2C_1_INST_IRQHandler                                    I2C1_IRQHandler
#define I2C_1_INST_INT_IRQN                                        I2C1_INT_IRQn
#define GPIO_I2C_1_SDA_PORT                                                GPIOA
#define GPIO_I2C_1_SDA_PIN                                         DL_GPIO_PIN_3
#define GPIO_I2C_1_IOMUX_SDA                                      (IOMUX_PINCM8)
#define GPIO_I2C_1_IOMUX_SDA_FUNC                       IOMUX_PINCM8_PF_I2C1_SDA
#define GPIO_I2C_1_SCL_PORT                                                GPIOA
#define GPIO_I2C_1_SCL_PIN                                         DL_GPIO_PIN_4
#define GPIO_I2C_1_IOMUX_SCL                                      (IOMUX_PINCM9)
#define GPIO_I2C_1_IOMUX_SCL_FUNC                       IOMUX_PINCM9_PF_I2C1_SCL


/* Defines for UART_3 */
#define UART_3_INST                                                        UART3
#define UART_3_INST_FREQUENCY                                           80000000
#define UART_3_INST_IRQHandler                                  UART3_IRQHandler
#define UART_3_INST_INT_IRQN                                      UART3_INT_IRQn
#define GPIO_UART_3_RX_PORT                                                GPIOB
#define GPIO_UART_3_TX_PORT                                                GPIOB
#define GPIO_UART_3_RX_PIN                                         DL_GPIO_PIN_3
#define GPIO_UART_3_TX_PIN                                         DL_GPIO_PIN_2
#define GPIO_UART_3_IOMUX_RX                                     (IOMUX_PINCM16)
#define GPIO_UART_3_IOMUX_TX                                     (IOMUX_PINCM15)
#define GPIO_UART_3_IOMUX_RX_FUNC                      IOMUX_PINCM16_PF_UART3_RX
#define GPIO_UART_3_IOMUX_TX_FUNC                      IOMUX_PINCM15_PF_UART3_TX
#define UART_3_BAUD_RATE                                                (115200)
#define UART_3_IBRD_80_MHZ_115200_BAUD                                      (43)
#define UART_3_FBRD_80_MHZ_115200_BAUD                                      (26)
/* Defines for UART_1 */
#define UART_1_INST                                                        UART1
#define UART_1_INST_FREQUENCY                                           40000000
#define UART_1_INST_IRQHandler                                  UART1_IRQHandler
#define UART_1_INST_INT_IRQN                                      UART1_INT_IRQn
#define GPIO_UART_1_RX_PORT                                                GPIOB
#define GPIO_UART_1_TX_PORT                                                GPIOB
#define GPIO_UART_1_RX_PIN                                         DL_GPIO_PIN_5
#define GPIO_UART_1_TX_PIN                                         DL_GPIO_PIN_4
#define GPIO_UART_1_IOMUX_RX                                     (IOMUX_PINCM18)
#define GPIO_UART_1_IOMUX_TX                                     (IOMUX_PINCM17)
#define GPIO_UART_1_IOMUX_RX_FUNC                      IOMUX_PINCM18_PF_UART1_RX
#define GPIO_UART_1_IOMUX_TX_FUNC                      IOMUX_PINCM17_PF_UART1_TX
#define UART_1_BAUD_RATE                                                (115200)
#define UART_1_IBRD_40_MHZ_115200_BAUD                                      (21)
#define UART_1_FBRD_40_MHZ_115200_BAUD                                      (45)
/* Defines for UART_4 */
#define UART_4_INST                                                        UART4
#define UART_4_INST_FREQUENCY                                           80000000
#define UART_4_INST_IRQHandler                                  UART4_IRQHandler
#define UART_4_INST_INT_IRQN                                      UART4_INT_IRQn
#define GPIO_UART_4_RX_PORT                                                GPIOB
#define GPIO_UART_4_TX_PORT                                                GPIOB
#define GPIO_UART_4_RX_PIN                                        DL_GPIO_PIN_11
#define GPIO_UART_4_TX_PIN                                        DL_GPIO_PIN_10
#define GPIO_UART_4_IOMUX_RX                                     (IOMUX_PINCM28)
#define GPIO_UART_4_IOMUX_TX                                     (IOMUX_PINCM27)
#define GPIO_UART_4_IOMUX_RX_FUNC                      IOMUX_PINCM28_PF_UART4_RX
#define GPIO_UART_4_IOMUX_TX_FUNC                      IOMUX_PINCM27_PF_UART4_TX
#define UART_4_BAUD_RATE                                                (115200)
#define UART_4_IBRD_80_MHZ_115200_BAUD                                      (43)
#define UART_4_FBRD_80_MHZ_115200_BAUD                                      (26)
/* Defines for UART_0 */
#define UART_0_INST                                                        UART0
#define UART_0_INST_FREQUENCY                                           40000000
#define UART_0_INST_IRQHandler                                  UART0_IRQHandler
#define UART_0_INST_INT_IRQN                                      UART0_INT_IRQn
#define GPIO_UART_0_RX_PORT                                                GPIOA
#define GPIO_UART_0_TX_PORT                                                GPIOA
#define GPIO_UART_0_RX_PIN                                         DL_GPIO_PIN_1
#define GPIO_UART_0_TX_PIN                                         DL_GPIO_PIN_0
#define GPIO_UART_0_IOMUX_RX                                      (IOMUX_PINCM2)
#define GPIO_UART_0_IOMUX_TX                                      (IOMUX_PINCM1)
#define GPIO_UART_0_IOMUX_RX_FUNC                       IOMUX_PINCM2_PF_UART0_RX
#define GPIO_UART_0_IOMUX_TX_FUNC                       IOMUX_PINCM1_PF_UART0_TX
#define UART_0_BAUD_RATE                                                (115200)
#define UART_0_IBRD_40_MHZ_115200_BAUD                                      (21)
#define UART_0_FBRD_40_MHZ_115200_BAUD                                      (45)
/* Defines for UART_7 */
#define UART_7_INST                                                        UART7
#define UART_7_INST_FREQUENCY                                           40000000
#define UART_7_INST_IRQHandler                                  UART7_IRQHandler
#define UART_7_INST_INT_IRQN                                      UART7_INT_IRQn
#define GPIO_UART_7_RX_PORT                                                GPIOB
#define GPIO_UART_7_TX_PORT                                                GPIOB
#define GPIO_UART_7_RX_PIN                                        DL_GPIO_PIN_18
#define GPIO_UART_7_TX_PIN                                        DL_GPIO_PIN_17
#define GPIO_UART_7_IOMUX_RX                                     (IOMUX_PINCM44)
#define GPIO_UART_7_IOMUX_TX                                     (IOMUX_PINCM43)
#define GPIO_UART_7_IOMUX_RX_FUNC                      IOMUX_PINCM44_PF_UART7_RX
#define GPIO_UART_7_IOMUX_TX_FUNC                      IOMUX_PINCM43_PF_UART7_TX
#define UART_7_BAUD_RATE                                                (115200)
#define UART_7_IBRD_40_MHZ_115200_BAUD                                      (21)
#define UART_7_FBRD_40_MHZ_115200_BAUD                                      (45)




/* Defines for SPI_1 */
#define SPI_1_INST                                                         SPI1
#define SPI_1_INST_IRQHandler                                   SPI1_IRQHandler
#define SPI_1_INST_INT_IRQN                                       SPI1_INT_IRQn
#define GPIO_SPI_1_PICO_PORT                                              GPIOB
#define GPIO_SPI_1_PICO_PIN                                      DL_GPIO_PIN_22
#define GPIO_SPI_1_IOMUX_PICO                                   (IOMUX_PINCM50)
#define GPIO_SPI_1_IOMUX_PICO_FUNC                   IOMUX_PINCM50_PF_SPI1_PICO
#define GPIO_SPI_1_POCI_PORT                                              GPIOB
#define GPIO_SPI_1_POCI_PIN                                      DL_GPIO_PIN_21
#define GPIO_SPI_1_IOMUX_POCI                                   (IOMUX_PINCM49)
#define GPIO_SPI_1_IOMUX_POCI_FUNC                   IOMUX_PINCM49_PF_SPI1_POCI
/* GPIO configuration for SPI_1 */
#define GPIO_SPI_1_SCLK_PORT                                              GPIOB
#define GPIO_SPI_1_SCLK_PIN                                      DL_GPIO_PIN_23
#define GPIO_SPI_1_IOMUX_SCLK                                   (IOMUX_PINCM51)
#define GPIO_SPI_1_IOMUX_SCLK_FUNC                   IOMUX_PINCM51_PF_SPI1_SCLK
#define GPIO_SPI_1_CS0_PORT                                               GPIOB
#define GPIO_SPI_1_CS0_PIN                                       DL_GPIO_PIN_20
#define GPIO_SPI_1_IOMUX_CS0                                    (IOMUX_PINCM48)
#define GPIO_SPI_1_IOMUX_CS0_FUNC                     IOMUX_PINCM48_PF_SPI1_CS0



/* Defines for DMA_CH2 */
#define DMA_CH2_CHAN_ID                                                      (2)
#define UART_3_INST_DMA_TRIGGER                              (DMA_UART3_TX_TRIG)
/* Defines for DMA_CH1 */
#define DMA_CH1_CHAN_ID                                                      (1)
#define UART_1_INST_DMA_TRIGGER                              (DMA_UART1_TX_TRIG)
/* Defines for DMA_CH3 */
#define DMA_CH3_CHAN_ID                                                      (3)
#define UART_4_INST_DMA_TRIGGER                              (DMA_UART4_TX_TRIG)
/* Defines for DMA_CH0 */
#define DMA_CH0_CHAN_ID                                                      (0)
#define UART_0_INST_DMA_TRIGGER                              (DMA_UART0_TX_TRIG)
/* Defines for DMA_CH4 */
#define DMA_CH4_CHAN_ID                                                      (4)
#define UART_7_INST_DMA_TRIGGER                              (DMA_UART7_TX_TRIG)


/* Port definition for Pin Group LED */
#define LED_PORT                                                         (GPIOB)

/* Defines for PIN_LED: GPIOB.19 with pinCMx 45 on package pin 16 */
#define LED_PIN_LED_PIN                                         (DL_GPIO_PIN_19)
#define LED_PIN_LED_IOMUX                                        (IOMUX_PINCM45)
/* Defines for PIN_VCC_BREAKER: GPIOB.6 with pinCMx 23 on package pin 58 */
#define BREAKER_PIN_VCC_BREAKER_PORT                                     (GPIOB)
#define BREAKER_PIN_VCC_BREAKER_PIN                              (DL_GPIO_PIN_6)
#define BREAKER_PIN_VCC_BREAKER_IOMUX                            (IOMUX_PINCM23)
/* Defines for PIN_5V_BREAKER: GPIOA.11 with pinCMx 22 on package pin 57 */
#define BREAKER_PIN_5V_BREAKER_PORT                                      (GPIOA)
#define BREAKER_PIN_5V_BREAKER_PIN                              (DL_GPIO_PIN_11)
#define BREAKER_PIN_5V_BREAKER_IOMUX                             (IOMUX_PINCM22)
/* Defines for PIN_0: GPIOA.24 with pinCMx 54 on package pin 25 */
#define IO_PIN_0_PORT                                                    (GPIOA)
#define IO_PIN_0_PIN                                            (DL_GPIO_PIN_24)
#define IO_PIN_0_IOMUX                                           (IOMUX_PINCM54)
/* Defines for PIN_1: GPIOA.25 with pinCMx 55 on package pin 26 */
#define IO_PIN_1_PORT                                                    (GPIOA)
#define IO_PIN_1_PIN                                            (DL_GPIO_PIN_25)
#define IO_PIN_1_IOMUX                                           (IOMUX_PINCM55)
/* Defines for PIN_2: GPIOB.25 with pinCMx 56 on package pin 27 */
#define IO_PIN_2_PORT                                                    (GPIOB)
#define IO_PIN_2_PIN                                            (DL_GPIO_PIN_25)
#define IO_PIN_2_IOMUX                                           (IOMUX_PINCM56)
/* Defines for PIN_3: GPIOB.26 with pinCMx 57 on package pin 28 */
#define IO_PIN_3_PORT                                                    (GPIOB)
#define IO_PIN_3_PIN                                            (DL_GPIO_PIN_26)
#define IO_PIN_3_IOMUX                                           (IOMUX_PINCM57)
/* Defines for PIN_4: GPIOB.27 with pinCMx 58 on package pin 29 */
#define IO_PIN_4_PORT                                                    (GPIOB)
#define IO_PIN_4_PIN                                            (DL_GPIO_PIN_27)
#define IO_PIN_4_IOMUX                                           (IOMUX_PINCM58)
/* Defines for PIN_5: GPIOA.26 with pinCMx 59 on package pin 30 */
#define IO_PIN_5_PORT                                                    (GPIOA)
#define IO_PIN_5_PIN                                            (DL_GPIO_PIN_26)
#define IO_PIN_5_IOMUX                                           (IOMUX_PINCM59)
/* Defines for PIN_6: GPIOA.27 with pinCMx 60 on package pin 31 */
#define IO_PIN_6_PORT                                                    (GPIOA)
#define IO_PIN_6_PIN                                            (DL_GPIO_PIN_27)
#define IO_PIN_6_IOMUX                                           (IOMUX_PINCM60)
/* Defines for PIN_7: GPIOA.28 with pinCMx 3 on package pin 35 */
#define IO_PIN_7_PORT                                                    (GPIOA)
#define IO_PIN_7_PIN                                            (DL_GPIO_PIN_28)
#define IO_PIN_7_IOMUX                                            (IOMUX_PINCM3)
/* Port definition for Pin Group KEY */
#define KEY_PORT                                                         (GPIOA)

/* Defines for PIN_KEY: GPIOA.31 with pinCMx 6 on package pin 39 */
// pins affected by this interrupt request:["PIN_KEY","PIN_ENCODER_SA","PIN_ENCODER_SS","PIN_ENCODER_SB"]
#define KEY_INT_IRQN                                            (GPIOA_INT_IRQn)
#define KEY_INT_IIDX                            (DL_INTERRUPT_GROUP1_IIDX_GPIOA)
#define KEY_PIN_KEY_IIDX                                    (DL_GPIO_IIDX_DIO31)
#define KEY_PIN_KEY_PIN                                         (DL_GPIO_PIN_31)
#define KEY_PIN_KEY_IOMUX                                         (IOMUX_PINCM6)
/* Defines for PIN_ENCODER_SA: GPIOA.9 with pinCMx 20 on package pin 55 */
#define KEY_PIN_ENCODER_SA_IIDX                              (DL_GPIO_IIDX_DIO9)
#define KEY_PIN_ENCODER_SA_PIN                                   (DL_GPIO_PIN_9)
#define KEY_PIN_ENCODER_SA_IOMUX                                 (IOMUX_PINCM20)
/* Defines for PIN_ENCODER_SS: GPIOA.8 with pinCMx 19 on package pin 54 */
#define KEY_PIN_ENCODER_SS_IIDX                              (DL_GPIO_IIDX_DIO8)
#define KEY_PIN_ENCODER_SS_PIN                                   (DL_GPIO_PIN_8)
#define KEY_PIN_ENCODER_SS_IOMUX                                 (IOMUX_PINCM19)
/* Defines for PIN_ENCODER_SB: GPIOA.7 with pinCMx 14 on package pin 49 */
#define KEY_PIN_ENCODER_SB_IIDX                              (DL_GPIO_IIDX_DIO7)
#define KEY_PIN_ENCODER_SB_PIN                                   (DL_GPIO_PIN_7)
#define KEY_PIN_ENCODER_SB_IOMUX                                 (IOMUX_PINCM14)


/* Defines for MCAN0 */
#define MCAN0_INST                                                        CANFD0
#define GPIO_MCAN0_CAN_TX_PORT                                             GPIOA
#define GPIO_MCAN0_CAN_TX_PIN                                     DL_GPIO_PIN_12
#define GPIO_MCAN0_IOMUX_CAN_TX                                  (IOMUX_PINCM34)
#define GPIO_MCAN0_IOMUX_CAN_TX_FUNC               IOMUX_PINCM34_PF_CANFD0_CANTX
#define GPIO_MCAN0_CAN_RX_PORT                                             GPIOA
#define GPIO_MCAN0_CAN_RX_PIN                                     DL_GPIO_PIN_13
#define GPIO_MCAN0_IOMUX_CAN_RX                                  (IOMUX_PINCM35)
#define GPIO_MCAN0_IOMUX_CAN_RX_FUNC               IOMUX_PINCM35_PF_CANFD0_CANRX
#define MCAN0_INST_IRQHandler                                 CANFD0_IRQHandler
#define MCAN0_INST_INT_IRQN                                     CANFD0_INT_IRQn


/* Defines for MCAN0 MCAN RAM configuration */
#define MCAN0_INST_MCAN_STD_ID_FILT_START_ADDR     (0)
#define MCAN0_INST_MCAN_STD_ID_FILTER_NUM          (1)
#define MCAN0_INST_MCAN_EXT_ID_FILT_START_ADDR     (48)
#define MCAN0_INST_MCAN_EXT_ID_FILTER_NUM          (1)
#define MCAN0_INST_MCAN_TX_BUFF_START_ADDR         (148)
#define MCAN0_INST_MCAN_TX_BUFF_SIZE               (2)
#define MCAN0_INST_MCAN_FIFO_1_START_ADDR          (192)
#define MCAN0_INST_MCAN_FIFO_1_NUM                 (2)
#define MCAN0_INST_MCAN_TX_EVENT_START_ADDR        (164)
#define MCAN0_INST_MCAN_TX_EVENT_SIZE              (2)
#define MCAN0_INST_MCAN_EXT_ID_AND_MASK            (0x1FFFFFFFU)
#define MCAN0_INST_MCAN_RX_BUFF_START_ADDR         (208)
#define MCAN0_INST_MCAN_FIFO_0_START_ADDR          (172)
#define MCAN0_INST_MCAN_FIFO_0_NUM                 (3)

#define MCAN0_INST_MCAN_INTERRUPTS (DL_MCAN_INTERRUPT_RF0N | \
						DL_MCAN_INTERRUPT_TEFN)




/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);

bool SYSCFG_DL_SYSCTL_SYSPLL_init(void);
void SYSCFG_DL_PWM_A0_init(void);
void SYSCFG_DL_PWM_G12_BUZZER_init(void);
void SYSCFG_DL_QEI_G8_init(void);
void SYSCFG_DL_QEI_G9_init(void);
void SYSCFG_DL_I2C_0_OLED_init(void);
void SYSCFG_DL_I2C_2_init(void);
void SYSCFG_DL_I2C_1_init(void);
void SYSCFG_DL_UART_3_init(void);
void SYSCFG_DL_UART_1_init(void);
void SYSCFG_DL_UART_4_init(void);
void SYSCFG_DL_UART_0_init(void);
void SYSCFG_DL_UART_7_init(void);
void SYSCFG_DL_SPI_1_init(void);
void SYSCFG_DL_DMA_init(void);

void SYSCFG_DL_MCAN0_init(void);

bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
