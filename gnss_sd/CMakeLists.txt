cmake_minimum_required(VERSION 3.12)

#set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
#set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
#set(DCMAKE_C_COMPILER "/usr/bin/gcc")
#set(DCMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_C_COMPILER /usr/bin/arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER /usr/bin/arm-none-eabi-g++)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

#set(CMAKE_MAKE_PROGRAM "/usr/bin/make")

# Specify the Pico SDK location (adjust path if necessary)
#set(PICO_SDK_PATH "../../pico-sdk")
set(PICO_SDK_PATH "../../AERIS-Software/pico-sdk")

# Initialize the Raspberry Pi Pico SDK
include(${PICO_SDK_PATH}/pico_sdk_init.cmake)

include_directories($(PICO_SDK_PATH)/src)

#include_directories((${PICO_SDK_PATH})/src/common/pico_stdlib_headers/include)
include_directories((${PICO_SDK_PATH})/src/rp2_common/hardware_i2c/include)

# Set the project name
project(gnss_logging)

# Initialize the SDK
pico_sdk_init()

add_subdirectory(FatFs_SPI)

# Specify the source files
add_executable(gnss_logging gnss_logging.c hw_config.c)

# Link the Pico SDK to the project

target_link_libraries(gnss_logging
    pico_stdlib
    hardware_i2c
    hardware_spi
    FatFs_SPI
    hardware_pio
    hardware_clocks
    hardware_gpio
    pico_binary_info
    pico_multicore
)

pico_enable_stdio_usb(gnss_logging 1)
pico_enable_stdio_uart(gnss_logging 0)

# Create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(gnss_logging)