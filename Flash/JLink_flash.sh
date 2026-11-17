#!/usr/bin/env bash
# ------------------------------------------------------------------
# J-Link 烧录（Linux）—— 直接用 SEGGER JLinkExe，不经 OpenOCD
#
#   - 自动定位工程根目录与 build/<BUILD_TYPE>/<工程名>.elf，可在任意目录调用
#   - MSPM0 推荐用 J-Link：OpenOCD 对 MSPM0 偶发锁芯片（见 Flash/README.md）
#
# 用法：bash Flash/JLink_flash.sh [Debug|Release|RelWithDebInfo]
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

if ! command -v JLinkExe >/dev/null 2>&1; then
    echo "❌ 未找到 JLinkExe，请安装 SEGGER J-Link Software 并加入 PATH"
    exit 1
fi

mkdir -p build
TEMP_SCRIPT="build/jlink_flash.jlink"
cat > "$TEMP_SCRIPT" <<EOF
device MSPM0G3519
if SWD
speed 4000
halt
loadfile $ELF_FILE
r
go
exit
EOF

echo "🔧 项目：$PROJECT_NAME（$BUILD_TYPE）"
echo "📦 ELF ：$ELF_FILE"
echo "⚡ J-Link 烧录中（SWD 4 MHz，device MSPM0G3519）..."
JLinkExe -CommanderScript "$TEMP_SCRIPT"
rm -f "$TEMP_SCRIPT"
echo "✅ 烧录完成"
