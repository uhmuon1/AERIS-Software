# Install script for directory: /home/connerparker/ASEN_4013/AERIS-Software/pico-examples

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/arm-none-eabi-objdump")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/pico-sdk/cmake_install.cmake")
  include("/home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/blink/cmake_install.cmake")
  include("/home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/blink_simple/cmake_install.cmake")
  include("/home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/hello_world/cmake_install.cmake")
  include("/home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/adc/cmake_install.cmake")
  include("/home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/binary_info/cmake_install.cmake")
  include("/home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/bootloaders/cmake_install.cmake")
  include("/home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/clocks/cmake_install.cmake")
  include("/home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/cmake/cmake_install.cmake")
  include("/home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/dcp/cmake_install.cmake")
  include("/home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/divider/cmake_install.cmake")
  include("/home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/dma/cmake_install.cmake")
  include("/home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/flash/cmake_install.cmake")
  include("/home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/gpio/cmake_install.cmake")
  include("/home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/hstx/cmake_install.cmake")
  include("/home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/i2c/cmake_install.cmake")
  include("/home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/interp/cmake_install.cmake")
  include("/home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/multicore/cmake_install.cmake")
  include("/home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/otp/cmake_install.cmake")
  include("/home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/picoboard/cmake_install.cmake")
  include("/home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/pico_w/cmake_install.cmake")
  include("/home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/pio/cmake_install.cmake")
  include("/home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/pwm/cmake_install.cmake")
  include("/home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/reset/cmake_install.cmake")
  include("/home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/rtc/cmake_install.cmake")
  include("/home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/spi/cmake_install.cmake")
  include("/home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/system/cmake_install.cmake")
  include("/home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/timer/cmake_install.cmake")
  include("/home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/uart/cmake_install.cmake")
  include("/home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/universal/cmake_install.cmake")
  include("/home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/usb/cmake_install.cmake")
  include("/home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/watchdog/cmake_install.cmake")
  include("/home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/sha/cmake_install.cmake")
  include("/home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/freertos/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
