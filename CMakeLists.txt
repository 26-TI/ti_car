# ==============================================================================
# MSPM0G3519_CAR —— MSPM0G3519 (Cortex-M0+) + FreeRTOS + M0BSP
#
# 构建：cmake --preset Debug && cmake --build --preset Debug
# 产物：build/<preset>/MSPM0G3519_CAR.elf
# ==============================================================================
cmake_minimum_required(VERSION 3.27)

include(mspm0g351x_base.cmake)

project(MSPM0G3519_CAR C CXX ASM)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(FREERTOS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/FreeRTOS)

# ------------------------------------------------------------------------------
# FreeRTOS（Middlewares/FreeRTOS 直接放置，非 submodule、无子目录）
# ------------------------------------------------------------------------------
file(GLOB FREERTOS_C_SOURCES "${FREERTOS_DIR}/*.c")
file(GLOB FREERTOS_MEMMANG_C_SOURCES "${FREERTOS_DIR}/portable/MemMang/heap_4.c")
file(GLOB_RECURSE FREERTOS_PORTABLE_C_SOURCES "${FREERTOS_DIR}/portable/GCC/ARM_CM0/*.c")

# ------------------------------------------------------------------------------
# 宿主源码（User/ 下增删文件后直接构建，不用改这里）
# ------------------------------------------------------------------------------
file(GLOB_RECURSE USER_SOURCES
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    CONFIGURE_DEPENDS
    User/*.c
    User/*.cpp
)

add_executable(${CMAKE_PROJECT_NAME}.elf
    ${SYSCONFIG_SOURCES}
    ${MSPM0_SDK_SOURCES}
    ${MSPM0_STARTUP}
    ${FREERTOS_C_SOURCES}
    ${FREERTOS_PORTABLE_C_SOURCES}
    ${FREERTOS_MEMMANG_C_SOURCES}
    ${USER_SOURCES}
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/User/Board
    ${CMAKE_CURRENT_SOURCE_DIR}/User/App
)

# FreeRTOS 也是库：它内部的告警不刷屏，我们自己的代码（Middlewares/M0BSP、User/）照常 -Wall
include_directories(SYSTEM
    ${FREERTOS_DIR}/include
    ${FREERTOS_DIR}/portable/GCC/ARM_CM0
)

# ------------------------------------------------------------------------------
# M0BSP（git submodule：Middlewares/M0BSP → Rh-i/M0BSP）
# 未初始化 submodule 时给出明确提示，而不是一堆未定义符号。
# ------------------------------------------------------------------------------
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/M0BSP/CMakeLists.txt)
    add_subdirectory(Middlewares/M0BSP)
    target_link_libraries(${CMAKE_PROJECT_NAME}.elf m0bsp)
    target_compile_features(${CMAKE_PROJECT_NAME}.elf PRIVATE cxx_std_20)
else()
    message(WARNING
        "Middlewares/M0BSP 不存在：请先执行 git submodule update --init --recursive 再配置。")
endif()
