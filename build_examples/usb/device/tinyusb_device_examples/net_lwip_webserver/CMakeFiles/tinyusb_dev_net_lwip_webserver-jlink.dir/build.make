# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/connerparker/ASEN_4013/AERIS-Software/pico-examples

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build

# Utility rule file for tinyusb_dev_net_lwip_webserver-jlink.

# Include any custom commands dependencies for this target.
include usb/device/tinyusb_device_examples/net_lwip_webserver/CMakeFiles/tinyusb_dev_net_lwip_webserver-jlink.dir/compiler_depend.make

# Include the progress variables for this target.
include usb/device/tinyusb_device_examples/net_lwip_webserver/CMakeFiles/tinyusb_dev_net_lwip_webserver-jlink.dir/progress.make

usb/device/tinyusb_device_examples/net_lwip_webserver/CMakeFiles/tinyusb_dev_net_lwip_webserver-jlink: usb/device/tinyusb_device_examples/net_lwip_webserver/tinyusb_dev_net_lwip_webserver.elf
	cd /home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/usb/device/tinyusb_device_examples/net_lwip_webserver && JLinkExe -device -if swd -JTAGConf -1,-1 -speed auto -CommandFile /home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/usb/device/tinyusb_device_examples/net_lwip_webserver/tinyusb_dev_net_lwip_webserver.jlink

tinyusb_dev_net_lwip_webserver-jlink: usb/device/tinyusb_device_examples/net_lwip_webserver/CMakeFiles/tinyusb_dev_net_lwip_webserver-jlink
tinyusb_dev_net_lwip_webserver-jlink: usb/device/tinyusb_device_examples/net_lwip_webserver/CMakeFiles/tinyusb_dev_net_lwip_webserver-jlink.dir/build.make
.PHONY : tinyusb_dev_net_lwip_webserver-jlink

# Rule to build all files generated by this target.
usb/device/tinyusb_device_examples/net_lwip_webserver/CMakeFiles/tinyusb_dev_net_lwip_webserver-jlink.dir/build: tinyusb_dev_net_lwip_webserver-jlink
.PHONY : usb/device/tinyusb_device_examples/net_lwip_webserver/CMakeFiles/tinyusb_dev_net_lwip_webserver-jlink.dir/build

usb/device/tinyusb_device_examples/net_lwip_webserver/CMakeFiles/tinyusb_dev_net_lwip_webserver-jlink.dir/clean:
	cd /home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/usb/device/tinyusb_device_examples/net_lwip_webserver && $(CMAKE_COMMAND) -P CMakeFiles/tinyusb_dev_net_lwip_webserver-jlink.dir/cmake_clean.cmake
.PHONY : usb/device/tinyusb_device_examples/net_lwip_webserver/CMakeFiles/tinyusb_dev_net_lwip_webserver-jlink.dir/clean

usb/device/tinyusb_device_examples/net_lwip_webserver/CMakeFiles/tinyusb_dev_net_lwip_webserver-jlink.dir/depend:
	cd /home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/connerparker/ASEN_4013/AERIS-Software/pico-examples /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/lib/tinyusb/examples/device/net_lwip_webserver /home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build /home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/usb/device/tinyusb_device_examples/net_lwip_webserver /home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/usb/device/tinyusb_device_examples/net_lwip_webserver/CMakeFiles/tinyusb_dev_net_lwip_webserver-jlink.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : usb/device/tinyusb_device_examples/net_lwip_webserver/CMakeFiles/tinyusb_dev_net_lwip_webserver-jlink.dir/depend

