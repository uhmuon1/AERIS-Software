# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# compile ASM with /usr/bin/arm-none-eabi-gcc
# compile C with /usr/bin/arm-none-eabi-gcc
# compile CXX with /usr/bin/arm-none-eabi-g++
ASM_DEFINES = -DLIB_BOOT_STAGE2_HEADERS=1 -DLIB_PICO_ATOMIC=1 -DLIB_PICO_BIT_OPS=1 -DLIB_PICO_BIT_OPS_PICO=1 -DLIB_PICO_CLIB_INTERFACE=1 -DLIB_PICO_CRT0=1 -DLIB_PICO_CXX_OPTIONS=1 -DLIB_PICO_DIVIDER=1 -DLIB_PICO_DIVIDER_HARDWARE=1 -DLIB_PICO_DOUBLE=1 -DLIB_PICO_DOUBLE_PICO=1 -DLIB_PICO_FLASH=1 -DLIB_PICO_FLOAT=1 -DLIB_PICO_FLOAT_PICO=1 -DLIB_PICO_INT64_OPS=1 -DLIB_PICO_INT64_OPS_PICO=1 -DLIB_PICO_MALLOC=1 -DLIB_PICO_MEM_OPS=1 -DLIB_PICO_MEM_OPS_PICO=1 -DLIB_PICO_NEWLIB_INTERFACE=1 -DLIB_PICO_PLATFORM=1 -DLIB_PICO_PLATFORM_COMPILER=1 -DLIB_PICO_PLATFORM_PANIC=1 -DLIB_PICO_PLATFORM_SECTIONS=1 -DLIB_PICO_PRINTF=1 -DLIB_PICO_PRINTF_PICO=1 -DLIB_PICO_RUNTIME=1 -DLIB_PICO_RUNTIME_INIT=1 -DLIB_PICO_STANDARD_BINARY_INFO=1 -DLIB_PICO_STANDARD_LINK=1 -DLIB_PICO_STDIO=1 -DLIB_PICO_STDIO_UART=1 -DLIB_PICO_STDLIB=1 -DLIB_PICO_SYNC=1 -DLIB_PICO_SYNC_CRITICAL_SECTION=1 -DLIB_PICO_SYNC_MUTEX=1 -DLIB_PICO_SYNC_SEM=1 -DLIB_PICO_TIME=1 -DLIB_PICO_TIME_ADAPTER=1 -DLIB_PICO_UTIL=1 -DPICO_32BIT=1 -DPICO_BOARD=\"pico\" -DPICO_BUILD=1 -DPICO_CMAKE_BUILD_TYPE=\"Release\" -DPICO_COPY_TO_RAM=0 -DPICO_CXX_ENABLE_EXCEPTIONS=0 -DPICO_NO_FLASH=0 -DPICO_NO_HARDWARE=0 -DPICO_ON_DEVICE=1 -DPICO_PROGRAM_URL=\"https://github.com/raspberrypi/pico-examples/tree/HEAD/pio/st7789_lcd\" -DPICO_RP2040=1 -DPICO_TARGET_NAME=\"pio_st7789_lcd\" -DPICO_USE_BLOCKED_RAM=0

ASM_INCLUDES = -I/home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/pio/st7789_lcd -I/home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_atomic/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/common/pico_stdlib_headers/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_gpio/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/common/pico_base_headers/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/generated/pico_base -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/boards/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2040/pico_platform/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2040/hardware_regs/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_base/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_platform_compiler/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_platform_panic/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_platform_sections/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2040/hardware_structs/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/common/hardware_claim/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_sync/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_sync_spin_lock/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_irq/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_uart/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_resets/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_clocks/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_pll/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_vreg/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_watchdog/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_ticks/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_bootrom/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/common/boot_picoboot_headers/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/boot_bootrom_headers/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_boot_lock/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_flash/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/common/pico_time/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_timer/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/common/pico_sync/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/common/pico_util/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_time_adapter/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_xosc/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_divider/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_runtime/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_runtime_init/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/common/pico_bit_ops_headers/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/common/pico_divider_headers/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_double/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_float/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_malloc/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/common/pico_binary_info/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_printf/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_stdio/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_stdio_uart/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_multicore/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/common/boot_picobin_headers/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_int64_ops/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_mem_ops/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2040/boot_stage2/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_pio/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_interp/include

ASM_FLAGS = -mcpu=cortex-m0plus -mthumb -g -O3 -DNDEBUG -Wall -Wno-format -Wno-unused-function -Wno-maybe-uninitialized -ffunction-sections -fdata-sections

C_DEFINES = -DLIB_BOOT_STAGE2_HEADERS=1 -DLIB_PICO_ATOMIC=1 -DLIB_PICO_BIT_OPS=1 -DLIB_PICO_BIT_OPS_PICO=1 -DLIB_PICO_CLIB_INTERFACE=1 -DLIB_PICO_CRT0=1 -DLIB_PICO_CXX_OPTIONS=1 -DLIB_PICO_DIVIDER=1 -DLIB_PICO_DIVIDER_HARDWARE=1 -DLIB_PICO_DOUBLE=1 -DLIB_PICO_DOUBLE_PICO=1 -DLIB_PICO_FLASH=1 -DLIB_PICO_FLOAT=1 -DLIB_PICO_FLOAT_PICO=1 -DLIB_PICO_INT64_OPS=1 -DLIB_PICO_INT64_OPS_PICO=1 -DLIB_PICO_MALLOC=1 -DLIB_PICO_MEM_OPS=1 -DLIB_PICO_MEM_OPS_PICO=1 -DLIB_PICO_NEWLIB_INTERFACE=1 -DLIB_PICO_PLATFORM=1 -DLIB_PICO_PLATFORM_COMPILER=1 -DLIB_PICO_PLATFORM_PANIC=1 -DLIB_PICO_PLATFORM_SECTIONS=1 -DLIB_PICO_PRINTF=1 -DLIB_PICO_PRINTF_PICO=1 -DLIB_PICO_RUNTIME=1 -DLIB_PICO_RUNTIME_INIT=1 -DLIB_PICO_STANDARD_BINARY_INFO=1 -DLIB_PICO_STANDARD_LINK=1 -DLIB_PICO_STDIO=1 -DLIB_PICO_STDIO_UART=1 -DLIB_PICO_STDLIB=1 -DLIB_PICO_SYNC=1 -DLIB_PICO_SYNC_CRITICAL_SECTION=1 -DLIB_PICO_SYNC_MUTEX=1 -DLIB_PICO_SYNC_SEM=1 -DLIB_PICO_TIME=1 -DLIB_PICO_TIME_ADAPTER=1 -DLIB_PICO_UTIL=1 -DPICO_32BIT=1 -DPICO_BOARD=\"pico\" -DPICO_BUILD=1 -DPICO_CMAKE_BUILD_TYPE=\"Release\" -DPICO_COPY_TO_RAM=0 -DPICO_CXX_ENABLE_EXCEPTIONS=0 -DPICO_NO_FLASH=0 -DPICO_NO_HARDWARE=0 -DPICO_ON_DEVICE=1 -DPICO_PROGRAM_URL=\"https://github.com/raspberrypi/pico-examples/tree/HEAD/pio/st7789_lcd\" -DPICO_RP2040=1 -DPICO_TARGET_NAME=\"pio_st7789_lcd\" -DPICO_USE_BLOCKED_RAM=0

C_INCLUDES = -I/home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/pio/st7789_lcd -I/home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_atomic/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/common/pico_stdlib_headers/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_gpio/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/common/pico_base_headers/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/generated/pico_base -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/boards/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2040/pico_platform/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2040/hardware_regs/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_base/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_platform_compiler/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_platform_panic/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_platform_sections/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2040/hardware_structs/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/common/hardware_claim/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_sync/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_sync_spin_lock/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_irq/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_uart/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_resets/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_clocks/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_pll/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_vreg/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_watchdog/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_ticks/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_bootrom/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/common/boot_picoboot_headers/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/boot_bootrom_headers/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_boot_lock/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_flash/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/common/pico_time/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_timer/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/common/pico_sync/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/common/pico_util/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_time_adapter/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_xosc/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_divider/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_runtime/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_runtime_init/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/common/pico_bit_ops_headers/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/common/pico_divider_headers/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_double/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_float/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_malloc/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/common/pico_binary_info/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_printf/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_stdio/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_stdio_uart/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_multicore/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/common/boot_picobin_headers/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_int64_ops/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_mem_ops/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2040/boot_stage2/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_pio/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_interp/include

C_FLAGS = -mcpu=cortex-m0plus -mthumb -g -O3 -DNDEBUG -Wall -Wno-format -Wno-unused-function -Wno-maybe-uninitialized -ffunction-sections -fdata-sections -std=gnu11

CXX_DEFINES = -DLIB_BOOT_STAGE2_HEADERS=1 -DLIB_PICO_ATOMIC=1 -DLIB_PICO_BIT_OPS=1 -DLIB_PICO_BIT_OPS_PICO=1 -DLIB_PICO_CLIB_INTERFACE=1 -DLIB_PICO_CRT0=1 -DLIB_PICO_CXX_OPTIONS=1 -DLIB_PICO_DIVIDER=1 -DLIB_PICO_DIVIDER_HARDWARE=1 -DLIB_PICO_DOUBLE=1 -DLIB_PICO_DOUBLE_PICO=1 -DLIB_PICO_FLASH=1 -DLIB_PICO_FLOAT=1 -DLIB_PICO_FLOAT_PICO=1 -DLIB_PICO_INT64_OPS=1 -DLIB_PICO_INT64_OPS_PICO=1 -DLIB_PICO_MALLOC=1 -DLIB_PICO_MEM_OPS=1 -DLIB_PICO_MEM_OPS_PICO=1 -DLIB_PICO_NEWLIB_INTERFACE=1 -DLIB_PICO_PLATFORM=1 -DLIB_PICO_PLATFORM_COMPILER=1 -DLIB_PICO_PLATFORM_PANIC=1 -DLIB_PICO_PLATFORM_SECTIONS=1 -DLIB_PICO_PRINTF=1 -DLIB_PICO_PRINTF_PICO=1 -DLIB_PICO_RUNTIME=1 -DLIB_PICO_RUNTIME_INIT=1 -DLIB_PICO_STANDARD_BINARY_INFO=1 -DLIB_PICO_STANDARD_LINK=1 -DLIB_PICO_STDIO=1 -DLIB_PICO_STDIO_UART=1 -DLIB_PICO_STDLIB=1 -DLIB_PICO_SYNC=1 -DLIB_PICO_SYNC_CRITICAL_SECTION=1 -DLIB_PICO_SYNC_MUTEX=1 -DLIB_PICO_SYNC_SEM=1 -DLIB_PICO_TIME=1 -DLIB_PICO_TIME_ADAPTER=1 -DLIB_PICO_UTIL=1 -DPICO_32BIT=1 -DPICO_BOARD=\"pico\" -DPICO_BUILD=1 -DPICO_CMAKE_BUILD_TYPE=\"Release\" -DPICO_COPY_TO_RAM=0 -DPICO_CXX_ENABLE_EXCEPTIONS=0 -DPICO_NO_FLASH=0 -DPICO_NO_HARDWARE=0 -DPICO_ON_DEVICE=1 -DPICO_PROGRAM_URL=\"https://github.com/raspberrypi/pico-examples/tree/HEAD/pio/st7789_lcd\" -DPICO_RP2040=1 -DPICO_TARGET_NAME=\"pio_st7789_lcd\" -DPICO_USE_BLOCKED_RAM=0

CXX_INCLUDES = -I/home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/pio/st7789_lcd -I/home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_atomic/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/common/pico_stdlib_headers/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_gpio/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/common/pico_base_headers/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/generated/pico_base -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/boards/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2040/pico_platform/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2040/hardware_regs/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_base/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_platform_compiler/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_platform_panic/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_platform_sections/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2040/hardware_structs/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/common/hardware_claim/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_sync/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_sync_spin_lock/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_irq/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_uart/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_resets/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_clocks/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_pll/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_vreg/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_watchdog/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_ticks/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_bootrom/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/common/boot_picoboot_headers/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/boot_bootrom_headers/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_boot_lock/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_flash/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/common/pico_time/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_timer/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/common/pico_sync/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/common/pico_util/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_time_adapter/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_xosc/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_divider/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_runtime/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_runtime_init/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/common/pico_bit_ops_headers/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/common/pico_divider_headers/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_double/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_float/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_malloc/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/common/pico_binary_info/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_printf/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_stdio/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_stdio_uart/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_multicore/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/common/boot_picobin_headers/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_int64_ops/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/pico_mem_ops/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2040/boot_stage2/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_pio/include -isystem /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/src/rp2_common/hardware_interp/include

CXX_FLAGS = -mcpu=cortex-m0plus -mthumb -g -O3 -DNDEBUG -Wall -Wno-format -Wno-unused-function -Wno-maybe-uninitialized -fno-exceptions -fno-unwind-tables -fno-rtti -fno-use-cxa-atexit -ffunction-sections -fdata-sections -std=gnu++17

