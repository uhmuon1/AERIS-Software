cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)
include(pico_extras_import_optional.cmake)

project(i2c_scan C CXX ASM)


set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()


add_executable(i2c_scan
    bus_scan.c
    )

target_link_libraries(i2c_scan pico_stdlib hardware_i2c)

pico_enable_stdio_usb(i2c_scan 1)
pico_enable_stdio_uart(i2c_scan 0)

# Create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(i2c_scan)