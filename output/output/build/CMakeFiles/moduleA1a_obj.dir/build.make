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
CMAKE_SOURCE_DIR = /mnt/c/Users/LIUHY/Desktop/output/output

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/LIUHY/Desktop/output/output/build

# Utility rule file for moduleA1a_obj.

# Include any custom commands dependencies for this target.
include CMakeFiles/moduleA1a_obj.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/moduleA1a_obj.dir/progress.make

CMakeFiles/moduleA1a_obj: ../src/moduleA/moduleA1/moduleA1a/moduleA1a.o

../src/moduleA/moduleA1/moduleA1a/moduleA1a.o: ../src/moduleA/moduleA1/moduleA1a/moduleA1a.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/mnt/c/Users/LIUHY/Desktop/output/output/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Compiling /mnt/c/Users/LIUHY/Desktop/output/output/src/moduleA/moduleA1/moduleA1a/moduleA1a.c to /mnt/c/Users/LIUHY/Desktop/output/output/src/moduleA/moduleA1/moduleA1a/moduleA1a.o"
	/usr/bin/cc -c /mnt/c/Users/LIUHY/Desktop/output/output/src/moduleA/moduleA1/moduleA1a/moduleA1a.c -o /mnt/c/Users/LIUHY/Desktop/output/output/src/moduleA/moduleA1/moduleA1a/moduleA1a.o -I/mnt/c/Users/LIUHY/Desktop/output/output/src/moduleA/moduleA1/moduleA1a -I/mnt/c/Users/LIUHY/Desktop/output/output/src

moduleA1a_obj: CMakeFiles/moduleA1a_obj
moduleA1a_obj: ../src/moduleA/moduleA1/moduleA1a/moduleA1a.o
moduleA1a_obj: CMakeFiles/moduleA1a_obj.dir/build.make
.PHONY : moduleA1a_obj

# Rule to build all files generated by this target.
CMakeFiles/moduleA1a_obj.dir/build: moduleA1a_obj
.PHONY : CMakeFiles/moduleA1a_obj.dir/build

CMakeFiles/moduleA1a_obj.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/moduleA1a_obj.dir/cmake_clean.cmake
.PHONY : CMakeFiles/moduleA1a_obj.dir/clean

CMakeFiles/moduleA1a_obj.dir/depend:
	cd /mnt/c/Users/LIUHY/Desktop/output/output/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/LIUHY/Desktop/output/output /mnt/c/Users/LIUHY/Desktop/output/output /mnt/c/Users/LIUHY/Desktop/output/output/build /mnt/c/Users/LIUHY/Desktop/output/output/build /mnt/c/Users/LIUHY/Desktop/output/output/build/CMakeFiles/moduleA1a_obj.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/moduleA1a_obj.dir/depend

