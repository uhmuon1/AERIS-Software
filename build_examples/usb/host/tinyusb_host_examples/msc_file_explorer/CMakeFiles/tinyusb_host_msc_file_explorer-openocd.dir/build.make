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

# Utility rule file for tinyusb_host_msc_file_explorer-openocd.

# Include any custom commands dependencies for this target.
include usb/host/tinyusb_host_examples/msc_file_explorer/CMakeFiles/tinyusb_host_msc_file_explorer-openocd.dir/compiler_depend.make

# Include the progress variables for this target.
include usb/host/tinyusb_host_examples/msc_file_explorer/CMakeFiles/tinyusb_host_msc_file_explorer-openocd.dir/progress.make

usb/host/tinyusb_host_examples/msc_file_explorer/CMakeFiles/tinyusb_host_msc_file_explorer-openocd: usb/host/tinyusb_host_examples/msc_file_explorer/tinyusb_host_msc_file_explorer.elf
	cd /home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/usb/host/tinyusb_host_examples/msc_file_explorer && openocd -c "tcl_port disabled" -c "gdb_port disabled" -c init -c halt -c "program /home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/usb/host/tinyusb_host_examples/msc_file_explorer/tinyusb_host_msc_file_explorer.elf" -c reset -c exit

tinyusb_host_msc_file_explorer-openocd: usb/host/tinyusb_host_examples/msc_file_explorer/CMakeFiles/tinyusb_host_msc_file_explorer-openocd
tinyusb_host_msc_file_explorer-openocd: usb/host/tinyusb_host_examples/msc_file_explorer/CMakeFiles/tinyusb_host_msc_file_explorer-openocd.dir/build.make
.PHONY : tinyusb_host_msc_file_explorer-openocd

# Rule to build all files generated by this target.
usb/host/tinyusb_host_examples/msc_file_explorer/CMakeFiles/tinyusb_host_msc_file_explorer-openocd.dir/build: tinyusb_host_msc_file_explorer-openocd
.PHONY : usb/host/tinyusb_host_examples/msc_file_explorer/CMakeFiles/tinyusb_host_msc_file_explorer-openocd.dir/build

usb/host/tinyusb_host_examples/msc_file_explorer/CMakeFiles/tinyusb_host_msc_file_explorer-openocd.dir/clean:
	cd /home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/usb/host/tinyusb_host_examples/msc_file_explorer && $(CMAKE_COMMAND) -P CMakeFiles/tinyusb_host_msc_file_explorer-openocd.dir/cmake_clean.cmake
.PHONY : usb/host/tinyusb_host_examples/msc_file_explorer/CMakeFiles/tinyusb_host_msc_file_explorer-openocd.dir/clean

usb/host/tinyusb_host_examples/msc_file_explorer/CMakeFiles/tinyusb_host_msc_file_explorer-openocd.dir/depend:
	cd /home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/connerparker/ASEN_4013/AERIS-Software/pico-examples /home/connerparker/ASEN_4013/AERIS-Software/pico-sdk/lib/tinyusb/examples/host/msc_file_explorer /home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build /home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/usb/host/tinyusb_host_examples/msc_file_explorer /home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/usb/host/tinyusb_host_examples/msc_file_explorer/CMakeFiles/tinyusb_host_msc_file_explorer-openocd.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : usb/host/tinyusb_host_examples/msc_file_explorer/CMakeFiles/tinyusb_host_msc_file_explorer-openocd.dir/depend

