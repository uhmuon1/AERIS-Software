cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)
include(pico_extras_import_optional.cmake)

project(motor_test C CXX ASM)


set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()


add_executable(motor_test
    motor_test.c
    )

target_link_libraries(motor_test pico_stdlib hardware_i2c)

pico_enable_stdio_usb(motor_test 1)
pico_enable_stdio_uart(motor_test 0)

# Create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(motor_test)