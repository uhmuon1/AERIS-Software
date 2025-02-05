cmake_minimum_required(VERSION 3.12)

#set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
#set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
#set(DCMAKE_C_COMPILER "/usr/bin/gcc")
#set(DCMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_C_COMPILER /usr/bin/arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER /usr/bin/arm-none-eabi-g++)

# include(pico_sdk_import.cmake)
# include(pico_extras_import_optional.cmake)

#set(CMAKE_MAKE_PROGRAM "/usr/bin/make")

# Specify the Pico SDK location (adjust path if necessary)
#set(PICO_SDK_PATH "../../pico-sdk")
set(PICO_SDK_PATH "../../AERIS-Software/pico-sdk")

# Initialize the Raspberry Pi Pico SDK
include(${PICO_SDK_PATH}/pico_sdk_init.cmake)

include_directories($(PICO_SDK_PATH)/src)

# Set the project name
project(bus_scan)

# Initialize the SDK
pico_sdk_init()

# Specify the source files
add_executable(bus_scan bus_scan.c)

# Link the Pico SDK to the project
target_link_libraries(bus_scan pico_stdlib)
target_link_libraries(bus_scan pico_stdlib hardware_i2c)

pico_enable_stdio_usb(bus_scan 1)
pico_enable_stdio_uart(bus_scan 0)

# Create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(bus_scan)