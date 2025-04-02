cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)
include(pico_extras_import_optional.cmake)

project(spi_test C CXX ASM)


set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_executable(spi_test
    test_spi.c
    )

target_link_libraries(spi_test pico_stdlib hardware_spi)

pico_enable_stdio_usb(spi_test 1)
pico_enable_stdio_uart(spi_test 0)

# Create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(spi_test)