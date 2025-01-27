cmake_minimum_required(VERSION 3.12)

#set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
#set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
#set(DCMAKE_C_COMPILER "/usr/bin/gcc")
#set(DCMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_C_COMPILER /usr/bin/arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER /usr/bin/arm-none-eabi-g++)

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
project(gnss_test)

# Initialize the SDK
pico_sdk_init()

# Specify the source files
add_executable(gnss_test gnss.c)

# Link the Pico SDK to the project
target_link_libraries(gnss_test pico_stdlib)
target_link_libraries(gnss_test pico_stdlib hardware_i2c)

pico_enable_stdio_usb(gnss_test 1)
pico_enable_stdio_uart(gnss_test 0)

# Create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(gnss_test)


#cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
#include(pico_sdk_import.cmake)
#include(pico_extras_import_optional.cmake)

#project(pico_examples C CXX ASM)

#set(CMAKE_C_STANDARD 11)
#set(CMAKE_CXX_STANDARD 17)

#if (PICO_SDK_VERSION_STRING VERSION_LESS "2.1.0")
#    message(FATAL_ERROR "Raspberry Pi Pico SDK version 2.1.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
#endif()

#set(PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})

# If you want debug output from USB (pass -DPICO_STDIO_USB=1) this ensures you don't lose any debug output while USB is set up
#if (NOT DEFINED PICO_STDIO_USB_CONNECT_WAIT_TIMEOUT_MS)
#    set(PICO_STDIO_USB_CONNECT_WAIT_TIMEOUT_MS 3000)
#endif()

# Initialize the SDK
#pico_sdk_init()

#include(example_auto_set_url.cmake)

#function(add_subdirectory_exclude_platforms NAME)
#    if (ARGN)
#        if (PICO_PLATFORM IN_LIST ARGN)
#            message("Skipping ${NAME} example which is unsupported on this platform")
#            return()
#        endif()
#        foreach(PATTERN IN LISTS ARGN)
#            string(REGEX MATCH "${PATTERN}" MATCHED "${PICO_PLATFORM}")
#            if (MATCHED)
#                message("Skipping ${NAME} example which is unsupported on this platform")
#                return()
#            endif()
#        endforeach()
#    endif()
#    add_subdirectory(${NAME})
#endfunction()

#add_executable(gnss
#    gnss.c
#    )

# pull in common dependencies
#target_link_libraries(gnss pico_stdlib)

#if (PICO_CYW43_SUPPORTED)
#    target_link_libraries(gnss pico_cyw43_arch_none)
#endif()

# create map/bin/hex file etc.
#pico_add_extra_outputs(gnss)

# add url via pico_set_program_url
#example_auto_set_url(gnss)