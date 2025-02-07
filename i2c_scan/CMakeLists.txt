cmake_minimum_required(VERSION 3.12)

set(CMAKE_C_COMPILER /usr/bin/arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER /usr/bin/arm-none-eabi-g++)

set(PICO_SDK_PATH "../../AERIS-Software/pico-sdk")

include(${PICO_SDK_PATH}/pico_sdk_init.cmake)

include_directories($(PICO_SDK_PATH)/src)

include_directories((${PICO_SDK_PATH})/src/rp2_common/hardware_i2c/include)

# Set the project name
project(i2c_scan)

# Initialize the SDK
pico_sdk_init()

# Specify the source files
add_executable(i2c_scan bus_scan.c)

# Link the Pico SDK to the project
target_link_libraries(i2c_scan pico_stdlib)
target_link_libraries(i2c_scan pico_stdlib hardware_i2c)

pico_enable_stdio_usb(i2c_scan 1)
pico_enable_stdio_uart(i2c_scan 0)

# Create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(i2c_scan)