# ==============================================================================
# MSPM0G351x 工程公共配置（由 MSPM0_FreeRTOS_BDC_Core/mspm0g351x_base.cmake 泛化而来）
#
# 与旧版的唯一区别：SDK 路径不再硬编码，改为三级回退
#   1. -DMSPM0_SDK_PATH=<path>
#   2. 环境变量 MSPM0_SDK_ROOT
#   3. 平台默认路径（Linux: /home/rh/ti/mspm0_sdk_2_10_00_04，Windows: C:/ti/mspm0_sdk_2_10_00_04）
# ==============================================================================
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)

if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-Ofast)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-Ofast -g)
elseif("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os)
else()
    message(STATUS "Minimal optimization, debug info included")
    add_compile_options(-O0 -g)
endif()

# ------------------------------------------------------------------------------
# ARM GCC（未显式指定时才用 PATH 里的 arm-none-eabi-*，便于预设或工具链文件覆盖）
# ------------------------------------------------------------------------------
if(NOT CMAKE_C_COMPILER)
    set(CMAKE_C_COMPILER arm-none-eabi-gcc)
endif()
if(NOT CMAKE_CXX_COMPILER)
    set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
endif()
if(NOT CMAKE_ASM_COMPILER)
    set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
endif()
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(CMAKE_SIZE arm-none-eabi-size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# ------------------------------------------------------------------------------
# MSPM0 SDK 路径（三级回退）
# ------------------------------------------------------------------------------
if(NOT MSPM0_SDK_PATH)
    if(DEFINED ENV{MSPM0_SDK_ROOT})
        set(MSPM0_SDK_PATH "$ENV{MSPM0_SDK_ROOT}")
    elseif(WIN32)
        set(MSPM0_SDK_PATH "C:/ti/mspm0_sdk_2_10_00_04")
    else()
        set(MSPM0_SDK_PATH "/home/rh/ti/mspm0_sdk_2_10_00_04")
    endif()
endif()
set(MSPM0_SDK_PATH "${MSPM0_SDK_PATH}" CACHE PATH "MSPM0 SDK 根目录（含 source/ 与 .metadata/）")

if(NOT EXISTS "${MSPM0_SDK_PATH}/source/ti/driverlib")
    message(FATAL_ERROR
        "没找到 MSPM0 SDK：${MSPM0_SDK_PATH}\n"
        "请用 -DMSPM0_SDK_PATH=<SDK 根目录>、或设置环境变量 MSPM0_SDK_ROOT 后重新配置。")
endif()
message(STATUS "MSPM0 SDK: ${MSPM0_SDK_PATH}")

# SysConfig 生成物所在目录（与 MSPM0_FreeRTOS_BDC_Core 同构：<工程根>/SysConfig）
set(SYSCONFIG_PATH ${CMAKE_SOURCE_DIR}/SysConfig)

if(NOT EXISTS "${SYSCONFIG_PATH}/ti_msp_dl_config.c")
    message(FATAL_ERROR "没找到 ${SYSCONFIG_PATH}/ti_msp_dl_config.c（SysConfig 生成物）")
endif()

# ------------------------------------------------------------------------------
# 编译选项（与旧工程保持一致，来自 M0 GCC 例程）
# ------------------------------------------------------------------------------
add_compile_options(-mcpu=cortex-m0plus -march=armv6-m -mthumb -mfloat-abi=soft -Wall -gstrict-dwarf)
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)
add_compile_options($<$<COMPILE_LANGUAGE:ASM>:-x$<SEMICOLON>assembler-with-cpp>)

add_compile_options(
    $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
    $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
)

# ------------------------------------------------------------------------------
# 包含路径
#   - SysConfig 生成代码：普通 include，生成代码的告警照常暴露
#   - TI SDK / CMSIS：系统头（-isystem），第三方头内部的告警不刷屏
#     （例如 C++20 下 dl_adc12.h 把不同枚举按位或的 -Wdeprecated-enum-enum-conversion）
#   - -Wall 对 Middlewares/ 与 User/ 自己的代码仍然完全生效
# ------------------------------------------------------------------------------
include_directories(${SYSCONFIG_PATH})

include_directories(SYSTEM
    ${MSPM0_SDK_PATH}/source
    ${MSPM0_SDK_PATH}/source/third_party/CMSIS/Core/Include
)

# ------------------------------------------------------------------------------
# 设备定义（-D__MSPM0G3519__ -D__USE_SYSCONFIG__）
# ------------------------------------------------------------------------------
file(READ ${SYSCONFIG_PATH}/device.opt DEVICE_OPT)
add_definitions(${DEVICE_OPT})

# ------------------------------------------------------------------------------
# 链接选项
# ------------------------------------------------------------------------------
link_directories(${MSPM0_SDK_PATH}/source)

add_link_options(-T${SYSCONFIG_PATH}/device_linker.lds)
add_link_options(-T${SYSCONFIG_PATH}/device.lds.genlibs)
add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=memory.map)
add_link_options(-mcpu=cortex-m0plus -march=armv6-m -mthumb -static -lgcc -lc -lm -lnosys --specs=nano.specs -nostartfiles)
add_link_options(-Wl,--no-warn-rwx-segments)

# ------------------------------------------------------------------------------
# SDK / SysConfig / 启动文件源码
# ------------------------------------------------------------------------------
file(GLOB_RECURSE MSPM0_SDK_SOURCES
    ${MSPM0_SDK_PATH}/source/ti/driverlib/*.c
)
file(GLOB_RECURSE MSPM0_STARTUP
    ${MSPM0_SDK_PATH}/source/ti/devices/msp/m0p/startup_system_files/gcc/startup_mspm0g351x_gcc.c
)
file(GLOB_RECURSE SYSCONFIG_SOURCES
    ${SYSCONFIG_PATH}/*.c
)
