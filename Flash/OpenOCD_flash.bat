@echo off
setlocal enabledelayedexpansion
REM ------------------------------------------------------------------
REM OpenOCD + CMSIS-DAP 烧录（Windows）
REM
REM   OpenOCD 路径可用环境变量 OPENOCD_HOME 覆盖，默认 D:\Toolchain\OpenOCD-20260302
REM   需要 OpenOCD 自带 target/ti/mspm0.cfg；MSPM0 优先用 J-Link（Flash\JLink_flash.bat）
REM
REM 用法：Flash\OpenOCD_flash.bat [Debug^|Release^|RelWithDebInfo]
REM ------------------------------------------------------------------

set "BUILD_TYPE=%~1"
if "%BUILD_TYPE%"=="" set "BUILD_TYPE=Debug"

for %%i in ("%~dp0..") do (
    set "PROJECT_DIR=%%~fi"
    set "PROJECT_NAME=%%~nxi"
)

set "ELF_FILE=%PROJECT_DIR%\build\%BUILD_TYPE%\%PROJECT_NAME%.elf"
if not exist "%ELF_FILE%" (
    echo [WARN] 没找到 %ELF_FILE%，在 build 下兜底搜索...
    for /r "%PROJECT_DIR%\build" %%f in (*.elf) do set "ELF_FILE=%%f"
)
if not exist "%ELF_FILE%" (
    echo [ERROR] 找不到 .elf，请先编译
    exit /b 1
)
set "ELF_FILE=%ELF_FILE:\=/%"

if not defined OPENOCD_HOME set "OPENOCD_HOME=D:\Toolchain\OpenOCD-20260302"
set "OPENOCD_EXE=%OPENOCD_HOME%\bin\openocd.exe"
if not exist "%OPENOCD_EXE%" set "OPENOCD_EXE=openocd"

echo Project : %PROJECT_NAME% ^(%BUILD_TYPE%^)
echo ELF     : %ELF_FILE%
echo OpenOCD : %OPENOCD_EXE%

"%OPENOCD_EXE%" -f "%~dp0daplink.cfg" -c "program \"%ELF_FILE%\" verify reset exit"
