@echo off
setlocal enabledelayedexpansion
REM ------------------------------------------------------------------
REM J-Link 烧录（Windows）—— 直接用 SEGAR JLink.exe，不经 OpenOCD
REM
REM   JLink 安装目录可用环境变量 JLINK_HOME 覆盖，默认 D:\Toolchain\Jlink\JLink_V958
REM
REM 用法：Flash\JLink_flash.bat [Debug^|Release^|RelWithDebInfo]
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

REM 正斜杠：J-Link Commander 对反斜杠也能吃，但统一一下更稳
set "ELF_JLINK=%ELF_FILE:\=/%"

if not defined JLINK_HOME set "JLINK_HOME=D:\Toolchain\Jlink\JLink_V958"
set "JLINK_EXE=%JLINK_HOME%\JLink.exe"
if not exist "%JLINK_EXE%" set "JLINK_EXE=JLink.exe"

set "TEMP_SCRIPT=%PROJECT_DIR%\build\jlink_flash.jlink"
if not exist "%PROJECT_DIR%\build" mkdir "%PROJECT_DIR%\build"

(
echo device MSPM0G3519
echo if SWD
echo speed 4000
echo halt
echo loadfile %ELF_JLINK%
echo r
echo go
echo exit
) > "%TEMP_SCRIPT%"

echo Project : %PROJECT_NAME% ^(%BUILD_TYPE%^)
echo ELF     : %ELF_FILE%
echo J-Link  : %JLINK_EXE%
"%JLINK_EXE%" -CommanderScript "%TEMP_SCRIPT%"

del "%TEMP_SCRIPT%" 2>nul
