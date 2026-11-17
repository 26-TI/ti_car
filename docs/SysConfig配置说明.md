# SysConfig 配置说明

配置文件：`SysConfig/MSPM0G3519_CAR.syscfg`
生成命令：`python3 tools/sysconfig_ui.py --gen`（或 `[🧩 生成配置]` 按钮）

器件：`MSPM0G3519` / `LQFP-64(PM)` / `mspm0_sdk@2.10.00.04` / SysConfig `1.27.0`。
`SysConfig/device.opt` 定义 `-D__MSPM0G3519__ -D__USE_SYSCONFIG__`。

## 1. 本次改动（相对原始 syscfg）

### 1.1 五路 UART：补波特率 / FIFO / 中断 / TX DMA

原始 syscfg 里 UART 只有引脚分配（没有波特率、没有 FIFO、没有任何中断），
而 `BspUart` 依赖：RX FIFO + RX / RX_TIMEOUT 中断 + TX DMA。

| 属性 | 值 |
| --- | --- |
| `targetBaudRate` | 115200 |
| `enableFIFO` | true |
| `rxFifoThreshold` / `txFifoThreshold` | `DL_UART_RX_FIFO_LEVEL_FULL` / `DL_UART_TX_FIFO_LEVEL_EMPTY` |
| `enabledInterrupts` | `RX`、`RX_TIMEOUT_ERROR`、`DMA_DONE_TX` |
| `enabledDMATXTriggers` | `DL_UART_DMA_INTERRUPT_TX` |
| `DMA_CHANNEL_TX` | `b2f` / `BYTE` / `BYTE` |

DMA 通道分配（不写死数字，代码一律用生成的宏）：

| UART | 实例名 | DMA 通道 |
| --- | --- | --- |
| UART0 | `UART_0`（PA0/PA1） | `DMA_CH0` |
| UART1 | `UART_1`（PB4/PB5） | `DMA_CH1` |
| UART3 | `UART_3`（PB2/PB3，调试） | `DMA_CH2` |
| UART4 | `UART_4`（PB10/PB11） | `DMA_CH3` |
| UART7 | `UART_7`（PB17/PB18） | `DMA_CH4` |

生成后应存在这些宏（可用 `grep` 校验）：

```
DMA_CH0_CHAN_ID … DMA_CH4_CHAN_ID
UART_0..UART_7_INST_DMA_TRIGGER
UART_x_INST_INT_IRQN
```

### 1.2 MCAN0：打开中断

原始 syscfg 的 MCAN0 只有引脚分配，`ti_msp_dl_config.h` 里**没有** `MCAN0_INST_INT_IRQN` /
`MCAN0_INST_IRQHandler`，`BspCan` 编不过。本次补齐：

`fdMode=false`、`desiredNomRate=1000`、标准/扩展滤波器、8 字节元素、
`enableInterrupt=true`、`interruptLine=[LINE0]`、`interruptFlags=[RF0N, TEFN]`、`spPercent=75`。

### 1.3 未改动

实例名、引脚分配、`PWM_A0`、`PWM_G12_BUZZER`、`QEI_G8/G9`、`I2C_*`、`SPI_1`、GPIO 分组全部保持原样。

## 2. 改名的正确姿势

外设实例名只在两个地方出现：

1. `User/Board/board_instances.hpp` —— 中断入口清单（`X(UART_3, 64)` 这种）
2. `User/Board/board_cfg.cpp` —— 对象与实例的绑定（`UART_3_INST`、`DMA_CH2_CHAN_ID` …）

在 SysConfig GUI 里把 `UART_3` 改名成 `UART_DEBUG` 后：

1. 改 `board_instances.hpp`：`X(UART_3, 64)` → `X(UART_DEBUG, 64)`
2. 改 `board_cfg.cpp`：`bsp_uart3.init({UART_DEBUG_INST, …})`
3. 重新生成 + 编译即可，`Middlewares/M0BSP` 一行都不用动

> 说明：中断函数名由 SysConfig 的 `_INST_IRQHandler` 宏决定（`UART_3_INST_IRQHandler → UART3_IRQHandler`），
> 所以改名不会改变真正的中断向量名。

## 3. GUI 与命令行的一致性

- 用 GUI 保存后，SysConfig 会重排属性顺序 → `git diff` 看起来很大，但内容等价。
- 只跑 CLI（`--gen`）不会改 `.syscfg` 本身，只重写 `ti_msp_dl_config.{c,h}` 等生成物。
- 生成物是**提交进仓库**的（与 BDC 一致），所以每次改完 `.syscfg` 记得一起提交。
