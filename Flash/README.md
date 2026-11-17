# Flash 烧录脚本

工程名、构建类型都不写死：脚本自己算 `build/<BUILD_TYPE>/<工程名>.elf`，找不到就在 `build/` 下兜底搜索。

| 探针 | Linux | Windows | 说明 |
| --- | --- | --- | --- |
| **SEGGER J-Link（推荐）** | `JLink_flash.sh` | `JLink_flash.bat` | 直接用 `JLinkExe` / `JLink.exe`，`device MSPM0G3519`、SWD 4 MHz |
| CMSIS-DAP | `OpenOCD_flash.sh` | `OpenOCD_flash.bat` | 经 OpenOCD，需要带 `target/ti/mspm0.cfg` 的版本 |

用法：

```bash
bash Flash/JLink_flash.sh            # 默认 build/Debug
bash Flash/JLink_flash.sh Release
```

```bat
Flash\JLink_flash.bat
Flash\OpenOCD_flash.bat Debug
```

## 为什么优先 J-Link

- MSPM0 + OpenOCD 偶发锁芯片，解锁麻烦；J-Link 更稳定。
- Ubuntu 源里的 OpenOCD 0.12.0 **没有** `target/ti/mspm0.cfg`，`OpenOCD_flash.sh` 会先探测并给出明确提示，
  而不是抛一堆 TCL 错误。
- J-Link 的临时命令脚本写在 `build/jlink_flash.jlink`，不污染仓库。

## 可覆盖的环境变量

| 变量 | 作用 | 默认 |
| --- | --- | --- |
| `JLINK_HOME` | Windows 下 J-Link 安装目录 | `D:\Toolchain\Jlink\JLink_V958` |
| `OPENOCD_HOME` | Windows 下 OpenOCD 安装目录 | `D:\Toolchain\OpenOCD-20260302` |

## 本目录其他文件

| 文件 | 用途 |
| --- | --- |
| `daplink.cfg` | OpenOCD：CMSIS-DAP + SWD 2 MHz + `target/ti/mspm0.cfg` |
| `jlink.cfg` | OpenOCD：J-Link 适配器 + SWD 4 MHz + `target/ti/mspm0.cfg`（备用） |
| `MSPM0G351X.svd` | 外设寄存器描述，Cortex-Debug 查看外设用 |
