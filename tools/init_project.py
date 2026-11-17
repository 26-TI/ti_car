#!/usr/bin/env python3
"""
跨平台工程初始化 / 自检脚本（幂等，可反复运行）

    python3 tools/init_project.py            # 补齐缺失项
    python3 tools/init_project.py --dry-run  # 只报告，不写文件
    python3 tools/init_project.py --check    # 只自检（缺东西时退出码 1）

规则：
  1. **已存在的文件一律不改内容**（只做存在性与关键字检查）
  2. 缺失的目录 / .gitignore 区块 / 模板文件才创建
  3. 任何检查失败都只报告，不会覆盖你手改过的配置
  4. 连续运行两次，第二次必须零变更

生成/补齐的内容：
  - 目录：User/Board、User/App、Middleware/FreeRTOS、SysConfig、Flash、tools、docs、.vscode
  - .gitignore 的 init_project 区块（缺行才追加）
  - .clangd / .clang-format（缺失时从 Middleware/M0BSP 复制或写最小版本）
  - .vscode/tasks.json、settings.json、launch.json（缺失时写模板）
  - CMakePresets.json、mspm0g351x_base.cmake、Flash 脚本（缺失时报告，它们内容较多，请从仓库恢复）
"""

from __future__ import annotations

import argparse
import json
import re
import shutil
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
BEGIN = "# >>> init_project"
END = "# <<< init_project"

GITIGNORE_LINES = [
    "build/",
    "memory.map",
    "compile_commands.json",
    ".cache/",
    "__pycache__/",
    ".vscode/.cortex-debug*",
    "Flash/*.log",
    "/example/",
]

REQUIRED_DIRS = [
    "User/Board",
    "User/App",
    "Middleware/FreeRTOS",
    "SysConfig",
    "Flash",
    "tools",
    "docs",
    ".vscode",
]

REQUIRED_FILES = [
    "CMakeLists.txt",
    "CMakePresets.json",
    "mspm0g351x_base.cmake",
    ".clangd",
    ".clang-format",
    "Flash/daplink.cfg",
    "Flash/JLink_flash.sh",
    "Flash/JLink_flash.bat",
    "Flash/OpenOCD_flash.sh",
    "Flash/OpenOCD_flash.bat",
    "SysConfig/device.opt",
    "Middleware/FreeRTOS/portable/GCC/ARM_CM0/port.c",
]

REQUIRED_TASK_LABELS = ["Configure", "Build", "Clean_Rebuild", "Clean",
                        "JLink_flash", "OpenOCD_flash", "SysConfig", "SysConfig_Gen"]
REQUIRED_LAUNCH_NAMES = ["jlink-debug", "cmsis-dap-debug"]
REQUIRED_SETTINGS_KEYS = ["clangd.arguments", "VsCodeTaskButtons.tasks", "cmake.useCMakePresets"]

MIN_CLANGD = """# .clangd —— clangd 用 CMake 编译数据库（init_project.py 生成）
CompileFlags:
  CompilationDatabase: build/Debug
  Add:
    - -Wno-unknown-warning-option

Diagnostics:
  Suppress:
    - unknown-argument
"""

MIN_TASKS = {
    "version": "2.0.0",
    "tasks": [
        {"label": "Configure", "type": "shell", "command": "cmake",
         "args": ["--preset", "Debug"], "options": {"cwd": "${workspaceFolder}"},
         "group": "build", "problemMatcher": []},
        {"label": "Build", "type": "shell", "command": "cmake",
         "args": ["--build", "--preset", "Debug"], "options": {"cwd": "${workspaceFolder}"},
         "group": {"kind": "build", "isDefault": True}, "problemMatcher": []},
    ],
}

created: list[str] = []
skipped: list[str] = []
missing: list[str] = []


def strip_jsonc(text: str) -> str:
    """去掉 // 注释与尾逗号，用于解析 VS Code 的 JSONC。"""
    text = re.sub(r'^\s*//.*$', '', text, flags=re.M)
    text = re.sub(r'/\*.*?\*/', '', text, flags=re.S)
    text = re.sub(r',(\s*[}\]])', r'\1', text)
    return text


def load_jsonc(path: Path) -> dict | None:
    try:
        return json.loads(strip_jsonc(path.read_text(encoding="utf-8")))
    except Exception as exc:  # noqa: BLE001
        print(f"[错误] 解析 {path.relative_to(ROOT)} 失败：{exc}")
        return None


def ensure_dirs(dry_run: bool) -> None:
    for rel in REQUIRED_DIRS:
        d = ROOT / rel
        if d.is_dir():
            skipped.append(rel + "/")
            continue
        created.append(rel + "/")
        if not dry_run:
            d.mkdir(parents=True, exist_ok=True)
            gitkeep = d / ".gitkeep"
            if not gitkeep.exists():
                gitkeep.write_text("", encoding="utf-8")


def ensure_gitignore(dry_run: bool) -> None:
    path = ROOT / ".gitignore"
    if not path.exists():
        block = BEGIN + "\n" + "\n".join(GITIGNORE_LINES) + "\n" + END + "\n"
        created.append(".gitignore")
        if not dry_run:
            path.write_text(block, encoding="utf-8")
        return

    text = path.read_text(encoding="utf-8")
    if BEGIN in text and END in text:
        head, rest = text.split(BEGIN, 1)
        body, tail = rest.split(END, 1)
        lines = [ln.strip() for ln in body.splitlines() if ln.strip()]
        add = [ln for ln in GITIGNORE_LINES if ln not in lines]
        if not add:
            skipped.append(".gitignore 区块")
            return
        new_body = body.rstrip("\n") + "\n" + "\n".join(add) + "\n"
        created.append(f".gitignore（补 {len(add)} 行）")
        if not dry_run:
            path.write_text(head + BEGIN + new_body + END + tail, encoding="utf-8")
        return

    add = [ln for ln in GITIGNORE_LINES if ln not in text.splitlines()]
    if not add:
        skipped.append(".gitignore")
        return
    created.append(f".gitignore（追加 init_project 区块，{len(add)} 行）")
    if not dry_run:
        block = "\n" + BEGIN + "\n" + "\n".join(add) + "\n" + END + "\n"
        path.write_text(text.rstrip("\n") + "\n" + block, encoding="utf-8")


def ensure_clang_files(dry_run: bool) -> None:
    clangd = ROOT / ".clangd"
    if clangd.exists():
        skipped.append(".clangd")
    else:
        created.append(".clangd")
        if not dry_run:
            clangd.write_text(MIN_CLANGD, encoding="utf-8")

    fmt = ROOT / ".clang-format"
    if fmt.exists():
        skipped.append(".clang-format")
        return
    src = ROOT / "Middleware/M0BSP/.clang-format"
    created.append(".clang-format")
    if dry_run:
        return
    if src.is_file():
        shutil.copyfile(src, fmt)
    else:
        fmt.write_text("---\nBasedOnStyle: LLVM\nIndentWidth: 2\n", encoding="utf-8")


def ensure_json(rel: str, template: dict | None, check) -> None:
    path = ROOT / rel
    if not path.exists():
        if template is None:
            missing.append(rel)
            return
        created.append(rel)
        if not dry_run:
            path.write_text(json.dumps(template, indent=4, ensure_ascii=False) + "\n",
                            encoding="utf-8")
        return

    data = load_jsonc(path)
    if data is None:
        missing.append(rel + "（无法解析）")
        return
    lacks = check(data)
    if lacks:
        missing.append(f"{rel}（缺少：{', '.join(lacks)}）")
    else:
        skipped.append(rel)


def main() -> int:
    ap = argparse.ArgumentParser(description="初始化 / 自检 MSPM0G3519_CAR 工程")
    ap.add_argument("--dry-run", action="store_true", help="只报告，不写文件")
    ap.add_argument("--check", action="store_true", help="只自检，缺项时退出码 1")
    args = ap.parse_args()
    global dry_run
    dry_run = args.dry_run or args.check

    ensure_dirs(dry_run)
    ensure_gitignore(dry_run)
    ensure_clang_files(dry_run)

    ensure_json(".vscode/tasks.json", MIN_TASKS,
                lambda d: [lb for lb in REQUIRED_TASK_LABELS
                           if lb not in {t.get("label") for t in d.get("tasks", [])}])
    ensure_json(".vscode/launch.json", None,
                lambda d: [n for n in REQUIRED_LAUNCH_NAMES
                           if n not in {c.get("name") for c in d.get("configurations", [])}])
    ensure_json(".vscode/settings.json", None,
                lambda d: [k for k in REQUIRED_SETTINGS_KEYS if k not in d])

    for rel in REQUIRED_FILES:
        if not (ROOT / rel).exists():
            missing.append(rel)

    if not (ROOT / "Middleware/M0BSP/CMakeLists.txt").exists():
        missing.append("Middleware/M0BSP（submodule 未初始化：git submodule update --init）")

    print("=" * 60)
    print(f"工程根：{ROOT}")
    print(f"模式  ：{'检查' if args.check else ('预演' if args.dry_run else '执行')}")
    print("=" * 60)
    for label, items in (("已创建 / 补齐", created), ("已存在，跳过", skipped), ("缺失 / 需处理", missing)):
        print(f"\n[{label}] {len(items)} 项")
        for it in items:
            print(f"  - {it}")

    if missing:
        print("\n结果：有缺失项，请按上面列表补齐（本脚本不覆盖已有文件）")
        return 1
    print("\n结果：工程结构完整" + ("" if args.check else "（重复运行不会产生变更）"))
    return 0


if __name__ == "__main__":
    sys.exit(main())
