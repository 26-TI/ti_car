# MSPM0G3519_CAR

TI **MSPM0G3519**（Cortex-M0+，LQFP-64 PM）车辆控制板工程：FreeRTOS + M0BSP 板级库 + TI SysConfig 生成代码，
一套 CMake presets 同时支持 **Linux / Windows** 的编译、烧录、调试。

---

## 1. 目录结构

```
MSPM0G3519_CAR/
├─ CMakeLists.txt              构建入口（CMake ≥ 3.27 + Ninja）
├─ CMakePresets.json           Debug / Release / RelWithDebInfo（产物 build/<preset>/）
├─ mspm0g351x_base.cmake       MSPM0 工具链/编译/链接集中配置（SDK 路径三级回退）
├─ SysConfig/                  SysConfig 工程：*.syscfg + 生成代码（唯一外设配置源）
├─ Middlewares/
│  ├─ FreeRTOS/                直接放置，非 submodule
│  └─ M0BSP/                   git submodule → https://github.com/Rh-i/M0BSP.git
│     └─ tools/                init_project.py（结构自检/补齐）、sysconfig_ui.py（开界面）
│                              check_layers.py（分层检查）
├─ User/
│  ├─ Board/                   板级配置：board_cfg / board_instances / hardware_config（只放硬件宏）
│  │                           / FreeRTOSConfig.h / freertos_hooks.cpp / sysmem.c
│  └─ App/                     main.cpp + app_init()（默认只创建系统任务）
│      └─ task/                业务任务，各自时序/音型/地址配置写在本文件顶部
├─ Flash/                      烧录脚本与 OpenOCD 配置、SVD
├─ .vscode/                    tasks / launch / settings（含 Task Button）
└─ example/                    ★ 仅作迁移参考，已 gitignore，不属于本工程
```

### 分层

```
User/App                        业务（本工程默认没有业务任务）
    ↑
Middlewares/M0BSP/code/tasks     系统任务（cpu_monitor）
    ↑
Middlewares/M0BSP/code/device    器件（oled / track / ins / gyro / motor / buzzer）
    ↑
Middlewares/M0BSP/code/service   服务（user_system 格式化输出）
    ↑
Middlewares/M0BSP/code/bsp       驱动（uart / pwm / qei / can / i2c / gpio / key）
    ↑
Middlewares/M0BSP/code/algorithm 算法（pid）
```

分层由 `python3 Middlewares/M0BSP/tools/check_layers.py` 强制检查。
**库内不出现任何 SysConfig 实例名**：实例名只写在 `User/Board/board_instances.hpp` 与 `board_cfg.cpp`。

---

## 2. 环境要求

| 组件 | Linux | Windows |
| --- | --- | --- |
| 编译 | `arm-none-eabi-gcc/g++`（本机 15.2.rel1） | 同（TI 提供的 GCC 或 ARM GNU Toolchain） |
| 构建 | CMake ≥ 3.27、Ninja | 同 |
| SDK | MSPM0 SDK **2.10.00.04** | 同 |
| SysConfig | **1.27.0**（与 `.syscfg` 的 `@versions` 一致） | 同 |
| 烧录/调试 | SEGGER J-Link（推荐）、OpenOCD（可选） | J-Link / OpenOCD |
| VS Code 扩展 | CMake Tools、Cortex-Debug、clangd、VsCodeTaskButtons | 同 |

SDK / SysConfig 路径不必改文件，三种方式任选：

1. `cmake --preset Debug -DMSPM0_SDK_PATH=/path/to/sdk`
2. 环境变量 `MSPM0_SDK_ROOT`（SysConfig 用 `TI_SYSCONFIG_DIR`）
3. 不改：使用平台默认路径
   - Linux：`~/ti/mspm0_sdk_2_10_00_04`、`~/ti/sysconfig_1.27.0`
   - Windows：`C:/ti/mspm0_sdk_2_10_00_04`、`D:\CCS\sysconfig`

---

## 3. 首次获取与配置

```bash
git clone <本仓库> && cd MSPM0G3519_CAR
git submodule update --init --recursive      # 拉取 Middlewares/M0BSP
python3 Middlewares/M0BSP/tools/init_project.py   # 结构自检 / 补齐（幂等，可重复运行）
```

## 4. 编译

Linux / Windows 命令相同：

```bash
cmake --preset Debug
cmake --build --preset Debug
```

产物：`build/Debug/MSPM0G3519_CAR.elf`，链接时会打印内存占用（`memory.map` 同目录）。

VS Code 里直接按 **`[🔨 编译]`** 按钮，或 `Ctrl+Shift+B`。

> 打开工程时请把 **`MSPM0G3519_CAR/` 目录本身**作为工作区文件夹，
> 否则 `.vscode/tasks.json` 与 Task Button 不会生效。

---

## 5. SysConfig

- 配置文件：`SysConfig/MSPM0G3519_CAR.syscfg`，生成物 `SysConfig/ti_msp_dl_config.{c,h}`、`device*.lds` 等。
- 界面：VS Code 按钮 **`[⚙️ SysConfig]`**（= `python3 Middlewares/M0BSP/tools/sysconfig_ui.py`）：打开 GUI → 保存并关闭 → 自动跑 CLI 重新生成。
- 只生成不进界面：**`[🧩 生成配置]`**（`--gen`）或

```bash
python3 Middlewares/M0BSP/tools/sysconfig_ui.py --gen
```

当前 syscfg 的关键配置（详见 `docs/SysConfig配置说明.md`）：

| 外设 | 配置 |
| --- | --- |
| UART_0 / 1 / 3 / 4 / 7 | 115200、FIFO、RX + RX_TIMEOUT 中断、**TX DMA（DMA_CH0…CH4）** |
| MCAN0 | 经典 CAN 1 Mbps、开启中断（RF0N + TEFN） |
| PWM_A0 | TIMA0 四通道（PB8/PB12/PA10/PB13） |
| PWM_G12_BUZZER | TIMG12 CC1（PB14），由 device 层 `DeviceBuzzer` 使用 |
| QEI_G8 / QEI_G9 | TIMG8（PB15/PB16）/ TIMG9（PB7/PB9） |
| I2C_0_OLED / I2C_1 / I2C_2 | I2C0（PB0/PB1）/ I2C1（PA3/PA4）/ I2C2（PA29/PA30） |

---

## 6. 烧录

| 探针 | Linux | Windows |
| --- | --- | --- |
| **J-Link（推荐）** | `bash Flash/JLink_flash.sh` | `Flash\JLink_flash.bat` |
| CMSIS-DAP | `bash Flash/OpenOCD_flash.sh` | `Flash\OpenOCD_flash.bat` |

- 脚本自动定位 `build/<BUILD_TYPE>/<工程名>.elf`，也可传 `Release` / `RelWithDebInfo`。
- MSPM0 优先 J-Link：OpenOCD 对 MSPM0 偶发锁芯片，且需要带 `target/ti/mspm0.cfg` 的 OpenOCD
  （Ubuntu 源里的 0.12.0 没有，脚本会先探测再给出提示）。
- Windows 下 `JLINK_HOME` / `OPENOCD_HOME` 可覆盖默认安装路径。

## 7. 调试

`F5` 选择配置：

| 配置 | 说明 |
| --- | --- |
| `jlink-debug` | Cortex-Debug + J-Link，`device MSPM0G3519`，SVD 外设寄存器、live watch |
| `cmsis-dap-debug` | Cortex-Debug + OpenOCD（`Flash/daplink.cfg`） |

两个配置都 `runToEntryPoint: main`，SVD 用 `Flash/MSPM0G351X.svd`。

## 8. 常用命令

```bash
python3 Middlewares/M0BSP/tools/init_project.py --check           # 结构自检
python3 Middlewares/M0BSP/tools/check_layers.py                   # 分层检查
python3 Middlewares/M0BSP/tools/sysconfig_ui.py --dry-run         # 看 SysConfig 将执行的命令
cmake --build --preset Debug --clean-first                        # 全量重编
```

## 9. 已知限制（本版）

- 板上**没有** LED / 按键 / MEN 的 GPIO 分组（SysConfig 里未配置），因此 `bsp_gpio` / `bsp_key`
  只保留在库里、未被实例化；相关业务（菜单、按键任务）未迁移。
- 业务任务（巡线 / 电机 / 里程计 / OLED 等）与协议层**未迁移**：`User/App` 默认只有系统任务，
  按 `User/App/task/` 的约定自行添加。
- M0BSP 的 `code/device/fort.h`（8×16 字模）来源待确认，暂随库保留。

## 10. 文档

- `docs/迁移映射.md` —— `MSPM0_FreeRTOS_BDC_Core` → 本工程的逐项映射（旧路径 → 新路径）
- `docs/SysConfig配置说明.md` —— 本次对 `.syscfg` 做的改动与理由
- `Flash/README.md` —— 烧录脚本细节
