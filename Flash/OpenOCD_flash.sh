#!/usr/bin/env bash
# ------------------------------------------------------------------
# OpenOCD + CMSIS-DAP 烧录（Linux）
#
#   注意：MSPM0 需要 OpenOCD 自带 target/ti/mspm0.cfg。
#         Ubuntu 源里的 0.12.0 没有这个文件，脚本会先探测并给出明确提示。
#         MSPM0 优先用 J-Link：bash Flash/JLink_flash.sh
#
# 用法：bash Flash/OpenOCD_flash.sh [Debug|Release|RelWithDebInfo]
# ------------------------------------------------------------------
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"
PROJECT_NAME="$(basename "$PROJECT_DIR")"
BUILD_TYPE="${1:-Debug}"

cd "$PROJECT_DIR"

ELF_FILE="build/${BUILD_TYPE}/${PROJECT_NAME}.elf"
if [ ! -f "$ELF_FILE" ]; then
    ELF_FILE="$(find build -name '*.elf' -type f 2>/dev/null | head -n 1 || true)"
fi
if [ -z "${ELF_FILE}" ] || [ ! -f "$ELF_FILE" ]; then
    echo "❌ 找不到 .elf，请先编译（cmake --build --preset Debug）"
    exit 1
fi
ELF_FILE="$(realpath "$ELF_FILE")"

if ! command -v openocd >/dev/null 2>&1; then
    echo "❌ 未找到 openocd，请先安装（或改用 J-Link：bash Flash/JLink_flash.sh）"
    exit 1
fi

if ! openocd -c "find target/ti/mspm0.cfg" -c "shutdown" >/dev/null 2>&1; then
    echo "❌ 当前 OpenOCD 缺少 target/ti/mspm0.cfg（$(openocd --version 2>&1 | head -n 1)）"
    echo "   MSPM0 建议直接用 J-Link 烧录：bash Flash/JLink_flash.sh"
    echo "   或改装带 TI MSPM0 target 的 OpenOCD 后重试。"
    exit 1
fi

echo "🔧 项目：$PROJECT_NAME（$BUILD_TYPE）"
echo "📦 ELF ：$ELF_FILE"
echo "⚡ OpenOCD 烧录中（CMSIS-DAP / SWD 2 MHz）..."
openocd -f "$SCRIPT_DIR/daplink.cfg" \
        -c "gdb_port disabled" \
        -c "tcl_port disabled" \
        -c "telnet_port disabled" \
        -c "program \"$ELF_FILE\" verify reset exit"

echo "✅ 烧录完成"
