# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/ubuntu/Desktop/CSALEX2022

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ubuntu/Desktop/CSALEX2022/build

# Utility rule file for NightlyTest.

# Include the progress variables for this target.
include lib/yaml-cpp/CMakeFiles/NightlyTest.dir/progress.make

lib/yaml-cpp/CMakeFiles/NightlyTest:
	cd /home/ubuntu/Desktop/CSALEX2022/build/lib/yaml-cpp && /usr/bin/ctest -D NightlyTest

NightlyTest: lib/yaml-cpp/CMakeFiles/NightlyTest
NightlyTest: lib/yaml-cpp/CMakeFiles/NightlyTest.dir/build.make

.PHONY : NightlyTest

# Rule to build all files generated by this target.
lib/yaml-cpp/CMakeFiles/NightlyTest.dir/build: NightlyTest

.PHONY : lib/yaml-cpp/CMakeFiles/NightlyTest.dir/build

lib/yaml-cpp/CMakeFiles/NightlyTest.dir/clean:
	cd /home/ubuntu/Desktop/CSALEX2022/build/lib/yaml-cpp && $(CMAKE_COMMAND) -P CMakeFiles/NightlyTest.dir/cmake_clean.cmake
.PHONY : lib/yaml-cpp/CMakeFiles/NightlyTest.dir/clean

lib/yaml-cpp/CMakeFiles/NightlyTest.dir/depend:
	cd /home/ubuntu/Desktop/CSALEX2022/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ubuntu/Desktop/CSALEX2022 /home/ubuntu/Desktop/CSALEX2022/lib/yaml-cpp /home/ubuntu/Desktop/CSALEX2022/build /home/ubuntu/Desktop/CSALEX2022/build/lib/yaml-cpp /home/ubuntu/Desktop/CSALEX2022/build/lib/yaml-cpp/CMakeFiles/NightlyTest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/yaml-cpp/CMakeFiles/NightlyTest.dir/depend

