# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/guillaume/cpp_work_space/pcom

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/guillaume/cpp_work_space/pcom/build

# Include any dependencies generated for this target.
include examples/ipc_posix_circular_buffer_example/CMakeFiles/ipc_posix_consumer.dir/depend.make

# Include the progress variables for this target.
include examples/ipc_posix_circular_buffer_example/CMakeFiles/ipc_posix_consumer.dir/progress.make

# Include the compile flags for this target's objects.
include examples/ipc_posix_circular_buffer_example/CMakeFiles/ipc_posix_consumer.dir/flags.make

examples/ipc_posix_circular_buffer_example/CMakeFiles/ipc_posix_consumer.dir/src/consumer.cpp.o: examples/ipc_posix_circular_buffer_example/CMakeFiles/ipc_posix_consumer.dir/flags.make
examples/ipc_posix_circular_buffer_example/CMakeFiles/ipc_posix_consumer.dir/src/consumer.cpp.o: ../examples/ipc_posix_circular_buffer_example/src/consumer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/guillaume/cpp_work_space/pcom/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object examples/ipc_posix_circular_buffer_example/CMakeFiles/ipc_posix_consumer.dir/src/consumer.cpp.o"
	cd /home/guillaume/cpp_work_space/pcom/build/examples/ipc_posix_circular_buffer_example && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ipc_posix_consumer.dir/src/consumer.cpp.o -c /home/guillaume/cpp_work_space/pcom/examples/ipc_posix_circular_buffer_example/src/consumer.cpp

examples/ipc_posix_circular_buffer_example/CMakeFiles/ipc_posix_consumer.dir/src/consumer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ipc_posix_consumer.dir/src/consumer.cpp.i"
	cd /home/guillaume/cpp_work_space/pcom/build/examples/ipc_posix_circular_buffer_example && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/guillaume/cpp_work_space/pcom/examples/ipc_posix_circular_buffer_example/src/consumer.cpp > CMakeFiles/ipc_posix_consumer.dir/src/consumer.cpp.i

examples/ipc_posix_circular_buffer_example/CMakeFiles/ipc_posix_consumer.dir/src/consumer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ipc_posix_consumer.dir/src/consumer.cpp.s"
	cd /home/guillaume/cpp_work_space/pcom/build/examples/ipc_posix_circular_buffer_example && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/guillaume/cpp_work_space/pcom/examples/ipc_posix_circular_buffer_example/src/consumer.cpp -o CMakeFiles/ipc_posix_consumer.dir/src/consumer.cpp.s

examples/ipc_posix_circular_buffer_example/CMakeFiles/ipc_posix_consumer.dir/src/consumer.cpp.o.requires:

.PHONY : examples/ipc_posix_circular_buffer_example/CMakeFiles/ipc_posix_consumer.dir/src/consumer.cpp.o.requires

examples/ipc_posix_circular_buffer_example/CMakeFiles/ipc_posix_consumer.dir/src/consumer.cpp.o.provides: examples/ipc_posix_circular_buffer_example/CMakeFiles/ipc_posix_consumer.dir/src/consumer.cpp.o.requires
	$(MAKE) -f examples/ipc_posix_circular_buffer_example/CMakeFiles/ipc_posix_consumer.dir/build.make examples/ipc_posix_circular_buffer_example/CMakeFiles/ipc_posix_consumer.dir/src/consumer.cpp.o.provides.build
.PHONY : examples/ipc_posix_circular_buffer_example/CMakeFiles/ipc_posix_consumer.dir/src/consumer.cpp.o.provides

examples/ipc_posix_circular_buffer_example/CMakeFiles/ipc_posix_consumer.dir/src/consumer.cpp.o.provides.build: examples/ipc_posix_circular_buffer_example/CMakeFiles/ipc_posix_consumer.dir/src/consumer.cpp.o


# Object files for target ipc_posix_consumer
ipc_posix_consumer_OBJECTS = \
"CMakeFiles/ipc_posix_consumer.dir/src/consumer.cpp.o"

# External object files for target ipc_posix_consumer
ipc_posix_consumer_EXTERNAL_OBJECTS =

examples/ipc_posix_circular_buffer_example/ipc_posix_consumer: examples/ipc_posix_circular_buffer_example/CMakeFiles/ipc_posix_consumer.dir/src/consumer.cpp.o
examples/ipc_posix_circular_buffer_example/ipc_posix_consumer: examples/ipc_posix_circular_buffer_example/CMakeFiles/ipc_posix_consumer.dir/build.make
examples/ipc_posix_circular_buffer_example/ipc_posix_consumer: /usr/lib/x86_64-linux-gnu/libboost_system.so
examples/ipc_posix_circular_buffer_example/ipc_posix_consumer: examples/ipc_posix_circular_buffer_example/CMakeFiles/ipc_posix_consumer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/guillaume/cpp_work_space/pcom/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ipc_posix_consumer"
	cd /home/guillaume/cpp_work_space/pcom/build/examples/ipc_posix_circular_buffer_example && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ipc_posix_consumer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/ipc_posix_circular_buffer_example/CMakeFiles/ipc_posix_consumer.dir/build: examples/ipc_posix_circular_buffer_example/ipc_posix_consumer

.PHONY : examples/ipc_posix_circular_buffer_example/CMakeFiles/ipc_posix_consumer.dir/build

examples/ipc_posix_circular_buffer_example/CMakeFiles/ipc_posix_consumer.dir/requires: examples/ipc_posix_circular_buffer_example/CMakeFiles/ipc_posix_consumer.dir/src/consumer.cpp.o.requires

.PHONY : examples/ipc_posix_circular_buffer_example/CMakeFiles/ipc_posix_consumer.dir/requires

examples/ipc_posix_circular_buffer_example/CMakeFiles/ipc_posix_consumer.dir/clean:
	cd /home/guillaume/cpp_work_space/pcom/build/examples/ipc_posix_circular_buffer_example && $(CMAKE_COMMAND) -P CMakeFiles/ipc_posix_consumer.dir/cmake_clean.cmake
.PHONY : examples/ipc_posix_circular_buffer_example/CMakeFiles/ipc_posix_consumer.dir/clean

examples/ipc_posix_circular_buffer_example/CMakeFiles/ipc_posix_consumer.dir/depend:
	cd /home/guillaume/cpp_work_space/pcom/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/guillaume/cpp_work_space/pcom /home/guillaume/cpp_work_space/pcom/examples/ipc_posix_circular_buffer_example /home/guillaume/cpp_work_space/pcom/build /home/guillaume/cpp_work_space/pcom/build/examples/ipc_posix_circular_buffer_example /home/guillaume/cpp_work_space/pcom/build/examples/ipc_posix_circular_buffer_example/CMakeFiles/ipc_posix_consumer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/ipc_posix_circular_buffer_example/CMakeFiles/ipc_posix_consumer.dir/depend

