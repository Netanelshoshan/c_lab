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
CMAKE_COMMAND = "/Users/mbp/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/201.7846.88/CLion.app/Contents/bin/cmake/mac/bin/cmake"

# The command to remove a file.
RM = "/Users/mbp/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/201.7846.88/CLion.app/Contents/bin/cmake/mac/bin/cmake" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/mbp/Documents/cs/c_lab/exams

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/mbp/Documents/cs/c_lab/exams/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/exams.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/exams.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/exams.dir/flags.make

CMakeFiles/exams.dir/main.c.o: CMakeFiles/exams.dir/flags.make
CMakeFiles/exams.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mbp/Documents/cs/c_lab/exams/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/exams.dir/main.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/exams.dir/main.c.o   -c /Users/mbp/Documents/cs/c_lab/exams/main.c

CMakeFiles/exams.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/exams.dir/main.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/mbp/Documents/cs/c_lab/exams/main.c > CMakeFiles/exams.dir/main.c.i

CMakeFiles/exams.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/exams.dir/main.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/mbp/Documents/cs/c_lab/exams/main.c -o CMakeFiles/exams.dir/main.c.s

# Object files for target exams
exams_OBJECTS = \
"CMakeFiles/exams.dir/main.c.o"

# External object files for target exams
exams_EXTERNAL_OBJECTS =

exams: CMakeFiles/exams.dir/main.c.o
exams: CMakeFiles/exams.dir/build.make
exams: CMakeFiles/exams.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/mbp/Documents/cs/c_lab/exams/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable exams"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/exams.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/exams.dir/build: exams

.PHONY : CMakeFiles/exams.dir/build

CMakeFiles/exams.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/exams.dir/cmake_clean.cmake
.PHONY : CMakeFiles/exams.dir/clean

CMakeFiles/exams.dir/depend:
	cd /Users/mbp/Documents/cs/c_lab/exams/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/mbp/Documents/cs/c_lab/exams /Users/mbp/Documents/cs/c_lab/exams /Users/mbp/Documents/cs/c_lab/exams/cmake-build-debug /Users/mbp/Documents/cs/c_lab/exams/cmake-build-debug /Users/mbp/Documents/cs/c_lab/exams/cmake-build-debug/CMakeFiles/exams.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/exams.dir/depend

