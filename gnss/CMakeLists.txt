cmake_minimum_required(VERSION 3.12)

# Set the project name
project(gnss_test)

# Initialize the Pico SDK
include(pico_sdk_import.cmake)

# Specify the source files
add_executable(gnss_test gnss.c)

# Link the Pico SDK to the project
target_link_libraries(gnss_test pico_stdlib)

# Create the UF2 file (used for flashing)
pico_add_extra_outputs(gnss_test)
