cmake_minimum_required(VERSION 3.13)

# Initialize pico-sdk from submodule
# NOTE: This must happen before project()
include(pico_sdk_import.cmake)

project(sd_card_test C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

# Add FatFS_SPI library
add_subdirectory(FatFs_SPI)

# Create the main executable
add_executable(sd_card_test
    main.c
    hw_config.c
)

# Enable USB output, disable UART output
pico_enable_stdio_usb(sd_card_test 1)
pico_enable_stdio_uart(sd_card_test 0)

# Add the required libraries
target_link_libraries(sd_card_test
    pico_stdlib
    hardware_spi
    FatFs_SPI
)

# Create map/bin/hex file etc.
pico_add_extra_outputs(sd_card_test)