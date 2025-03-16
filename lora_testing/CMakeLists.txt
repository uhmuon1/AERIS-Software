cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)
include(pico_extras_import_optional.cmake)

project(lora_tx C CXX ASM)


set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_executable(lora_tx
    main.c
    )

target_link_libraries(lora_tx pico_stdlib hardware_spi)

pico_enable_stdio_usb(lora_tx 1)
pico_enable_stdio_uart(lora_tx 0)

# Create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(lora_tx)