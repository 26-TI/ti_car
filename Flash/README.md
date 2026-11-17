# Flash 烧录脚本

工程名、构建类型都不写死：脚本自己算 `build/<BUILD_TYPE>/<工程名>.elf`，找不到就在 `build/` 下兜底搜索。

| 探针 | Linux | Windows | 说明 |
| --- | --- | --- | --- |
| **SEGGER J-Link（推荐）** | `JLink_flash.sh` | `JLink_flash.bat` | 直接用 `JLinkExe` / `JLink.exe`，`device MSPM0G3519`、SWD 4 MHz |
| CMSIS-DAP | `OpenOCD_flash.sh` | `OpenOCD_flash.bat` | 经 OpenOCD，**必须用支持 MSPM0 的版本**（见下） |

```bash
bash Flash/JLink_flash.sh                 # 默认 build/Debug
bash Flash/JLink_flash.sh Release
bash Flash/OpenOCD_flash.sh
```

```bat
Flash\JLink_flash.bat
Flash\OpenOCD_flash.bat Debug
```

## OpenOCD 必须支持 MSPM0（重点）

MSPM0 的烧录依赖 **`mspm0` flash 驱动**。Ubuntu 源里的 `openocd 0.12.0` **没有**这个驱动，也没有
`target/ti/mspm0.cfg`，直接 `openocd -f Flash/daplink.cfg` 会立刻退出——Cortex-Debug 中的表现是
`OpenOCD: GDB Server Quit Unexpectedly`。

本工程的做法：

| 机制 | 说明 |
| --- | --- |
| `Flash/openocd-server` | 启动器：按以下顺序挑**支持 MSPM0** 的 OpenOCD，然后原样透传参数<br>① `$OPENOCD_BIN` → ② EmberProbe 扩展自带（`~/.config/Code/User/globalStorage/bakesheep.emberprobe/.../openocd-*/bin/openocd`）→ ③ `/usr/local/bin/openocd` → ④ `PATH` 里的 `openocd`<br>判定依据：二进制内含 `mspm0` 驱动符号 |
| `Flash/mspm0.cfg` | **随工程附带**的 TI 目标配置（GPL-2.0-or-later，Copyright TI），`daplink.cfg` / `jlink.cfg` 用 `[file dirname [info script]]` 相对引用，不再依赖发行版是否自带 |
| `.vscode/launch.json` | `cmsis-dap-debug` 的 `serverpath` 指向 `Flash/openocd-server`，所以 Cortex-Debug 也能自动选对版本 |

调试/烧录前可用环境变量覆盖：

```bash
export OPENOCD_BIN=/path/to/openocd     # 指定可执行
export OPENOCD_SCRIPTS=/path/to/scripts # 指定脚本根目录（一般不用）
OPENOCD_VERBOSE=1 bash Flash/OpenOCD_flash.sh   # 打印实际使用的 openocd 与脚本目录
```

**Windows**：`launch.json` 里的 `serverpath` 指向 `.sh`，Windows 上请改成你的 `openocd.exe` 路径，
或删掉该行并保证 `PATH` 中的 OpenOCD 支持 MSPM0（TI 官方 XDS110/CMSIS-DAP 版本即可）。

## 常见报错

| 报错 | 原因 | 处理 |
| --- | --- | --- |
| `Can't find target/ti/mspm0.cfg` | 用的是不带 MSPM0 脚本的 OpenOCD | 用 `Flash/openocd-server`（已改为引用本目录 `mspm0.cfg`） |
| `unable to find a matching CMSIS-DAP device` | 探针没插 / 被别的进程占用 | 插好 DAPLink；关闭占用它的 OpenOCD（EmberProbe、CubeIDE 等） |
| `OpenOCD: GDB Server Quit Unexpectedly`（Cortex-Debug） | 同上两类，或 `serverpath` 未生效 | 看 `gdb-server` 终端输出；确认 `serverpath` |
| `invalid command name "mspm0_board_reset"` 之类 | 目标配置没加载成功 | 确认 `Flash/mspm0.cfg` 存在 |

## 为什么优先 J-Link

- MSPM0 + OpenOCD 偶发锁芯片，解锁麻烦；J-Link 更稳定。
- J-Link 的临时命令脚本写在 `build/jlink_flash.jlink`，不污染仓库。
- 仓库内已不再需要额外配置（`device MSPM0G3519` 写在脚本里）。

## 可覆盖的环境变量

| 变量 | 作用 | 默认 |
| --- | --- | --- |
| `OPENOCD_BIN` | 指定 OpenOCD 可执行 | 自动探测 |
| `OPENOCD_SCRIPTS` | 指定 OpenOCD 脚本根 | 按二进制位置推断 |
| `JLINK_HOME` | Windows 下 J-Link 安装目录 | `D:\Toolchain\Jlink\JLink_V958` |
| `OPENOCD_HOME` | Windows 下 OpenOCD 安装目录 | `D:\Toolchain\OpenOCD-20260302` |

## 本目录其他文件

| 文件 | 用途 |
| --- | --- |
| `openocd-server` | OpenOCD 启动器（自动挑选支持 MSPM0 的版本） |
| `mspm0.cfg` | TI MSPM0 target 配置（随工程附带） |
| `daplink.cfg` | OpenOCD：CMSIS-DAP + SWD 2 MHz + `mspm0.cfg` |
| `jlink.cfg` | OpenOCD：J-Link 适配器 + SWD 4 MHz + `mspm0.cfg`（备用） |
| `MSPM0G351X.svd` | 外设寄存器描述，Cortex-Debug 查看外设用 |
