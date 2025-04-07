cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)
include(pico_extras_import_optional.cmake)

project(flight_code C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

# Define source and header files
file(GLOB_RECURSE SOURCES "src/*.c")
file(GLOB_RECURSE HEADERS "include/*.h")

add_executable(flight_code
    ${SOURCES} 
    ${HEADERS}
    main.c
    )

target_link_libraries(flight_code 
    pico_stdlib
    FatFs_SPI
    hardware_spi 
    hardware_i2c
    )

pico_enable_stdio_usb(flight_code 1)
pico_enable_stdio_uart(flight_code 0)

# Create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(flight_code)