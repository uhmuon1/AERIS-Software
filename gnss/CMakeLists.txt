cmake_minimum_required(VERSION 3.12)

# Set the project name
project(gnss_test)

# Initialize the Pico SDK
include(pico_sdk_import.cmake)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

# Specify the source files
add_executable(gnss_test gnss.c)

# Link the Pico SDK to the project
target_link_libraries(gnss_test pico_stdlib)

# enable usb output, disable uart output
pico_enable_stdio_usb(hello_usb 1)
pico_enable_stdio_uart(hello_usb 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(hello_usb)

# Create the UF2 file (used for flashing)
pico_add_extra_outputs(gnss_test)
