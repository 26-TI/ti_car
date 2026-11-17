#!/usr/bin/env bash
# ------------------------------------------------------------------
# OpenOCD + CMSIS-DAP 烧录（Linux）
#
#   OpenOCD 由 Flash/openocd-server 自动挑选"支持 MSPM0"的版本
#   （Ubuntu 源里的 0.12.0 没有 target/ti/mspm0.cfg / mspm0 驱动，会直接失败）。
#   MSPM0 更推荐 J-Link：bash Flash/JLink_flash.sh
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

if ! command -v lsusb >/dev/null 2>&1 || lsusb | grep -qiE "cmsis|dap"; then
    : # 探针存在（或无法检测）
else
    echo "⚠️  lsusb 里没看到 CMSIS-DAP 探针，请先插好 DAPLink（或改用 J-Link）"
fi

echo "🔧 项目：$PROJECT_NAME（$BUILD_TYPE）"
echo "📦 ELF ：$ELF_FILE"
echo "⚡ OpenOCD 烧录中（自动选择支持 MSPM0 的 openocd）..."

OPENOCD_VERBOSE=1 "$SCRIPT_DIR/openocd-server" \
    -f "$SCRIPT_DIR/daplink.cfg" \
    -c "gdb_port disabled" \
    -c "tcl_port disabled" \
    -c "telnet_port disabled" \
    -c "program \"$ELF_FILE\" verify reset exit"

echo "✅ 烧录完成"
