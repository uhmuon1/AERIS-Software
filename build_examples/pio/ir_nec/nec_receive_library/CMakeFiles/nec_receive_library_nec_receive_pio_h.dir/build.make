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

# Utility rule file for nec_receive_library_nec_receive_pio_h.

# Include any custom commands dependencies for this target.
include pio/ir_nec/nec_receive_library/CMakeFiles/nec_receive_library_nec_receive_pio_h.dir/compiler_depend.make

# Include the progress variables for this target.
include pio/ir_nec/nec_receive_library/CMakeFiles/nec_receive_library_nec_receive_pio_h.dir/progress.make

pio/ir_nec/nec_receive_library/CMakeFiles/nec_receive_library_nec_receive_pio_h: pio/ir_nec/nec_receive_library/nec_receive.pio.h

pio/ir_nec/nec_receive_library/nec_receive.pio.h: ../pio/ir_nec/nec_receive_library/nec_receive.pio
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating nec_receive.pio.h"
	cd /home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/pio/ir_nec/nec_receive_library && ../../../pioasm-install/pioasm/pioasm -o c-sdk -v 0 /home/connerparker/ASEN_4013/AERIS-Software/pico-examples/pio/ir_nec/nec_receive_library/nec_receive.pio /home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/pio/ir_nec/nec_receive_library/nec_receive.pio.h

nec_receive_library_nec_receive_pio_h: pio/ir_nec/nec_receive_library/CMakeFiles/nec_receive_library_nec_receive_pio_h
nec_receive_library_nec_receive_pio_h: pio/ir_nec/nec_receive_library/nec_receive.pio.h
nec_receive_library_nec_receive_pio_h: pio/ir_nec/nec_receive_library/CMakeFiles/nec_receive_library_nec_receive_pio_h.dir/build.make
.PHONY : nec_receive_library_nec_receive_pio_h

# Rule to build all files generated by this target.
pio/ir_nec/nec_receive_library/CMakeFiles/nec_receive_library_nec_receive_pio_h.dir/build: nec_receive_library_nec_receive_pio_h
.PHONY : pio/ir_nec/nec_receive_library/CMakeFiles/nec_receive_library_nec_receive_pio_h.dir/build

pio/ir_nec/nec_receive_library/CMakeFiles/nec_receive_library_nec_receive_pio_h.dir/clean:
	cd /home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/pio/ir_nec/nec_receive_library && $(CMAKE_COMMAND) -P CMakeFiles/nec_receive_library_nec_receive_pio_h.dir/cmake_clean.cmake
.PHONY : pio/ir_nec/nec_receive_library/CMakeFiles/nec_receive_library_nec_receive_pio_h.dir/clean

pio/ir_nec/nec_receive_library/CMakeFiles/nec_receive_library_nec_receive_pio_h.dir/depend:
	cd /home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/connerparker/ASEN_4013/AERIS-Software/pico-examples /home/connerparker/ASEN_4013/AERIS-Software/pico-examples/pio/ir_nec/nec_receive_library /home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build /home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/pio/ir_nec/nec_receive_library /home/connerparker/ASEN_4013/AERIS-Software/pico-examples/build/pio/ir_nec/nec_receive_library/CMakeFiles/nec_receive_library_nec_receive_pio_h.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : pio/ir_nec/nec_receive_library/CMakeFiles/nec_receive_library_nec_receive_pio_h.dir/depend

