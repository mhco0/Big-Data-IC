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
CMAKE_SOURCE_DIR = "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build"

# Include any dependencies generated for this target.
include src/CMakeFiles/PMQ.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/PMQ.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/PMQ.dir/flags.make

src/CMakeFiles/PMQ.dir/pmq/pmq.cpp.o: src/CMakeFiles/PMQ.dir/flags.make
src/CMakeFiles/PMQ.dir/pmq/pmq.cpp.o: ../src/pmq/pmq.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/PMQ.dir/pmq/pmq.cpp.o"
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/src" && /bin/x86_64-linux-gnu-g++-9  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/PMQ.dir/pmq/pmq.cpp.o -c "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/src/pmq/pmq.cpp"

src/CMakeFiles/PMQ.dir/pmq/pmq.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PMQ.dir/pmq/pmq.cpp.i"
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/src" && /bin/x86_64-linux-gnu-g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/src/pmq/pmq.cpp" > CMakeFiles/PMQ.dir/pmq/pmq.cpp.i

src/CMakeFiles/PMQ.dir/pmq/pmq.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PMQ.dir/pmq/pmq.cpp.s"
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/src" && /bin/x86_64-linux-gnu-g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/src/pmq/pmq.cpp" -o CMakeFiles/PMQ.dir/pmq/pmq.cpp.s

# Object files for target PMQ
PMQ_OBJECTS = \
"CMakeFiles/PMQ.dir/pmq/pmq.cpp.o"

# External object files for target PMQ
PMQ_EXTERNAL_OBJECTS =

lib/libPMQ.a: src/CMakeFiles/PMQ.dir/pmq/pmq.cpp.o
lib/libPMQ.a: src/CMakeFiles/PMQ.dir/build.make
lib/libPMQ.a: src/CMakeFiles/PMQ.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library ../lib/libPMQ.a"
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/src" && $(CMAKE_COMMAND) -P CMakeFiles/PMQ.dir/cmake_clean_target.cmake
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/src" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/PMQ.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/PMQ.dir/build: lib/libPMQ.a

.PHONY : src/CMakeFiles/PMQ.dir/build

src/CMakeFiles/PMQ.dir/clean:
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/src" && $(CMAKE_COMMAND) -P CMakeFiles/PMQ.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/PMQ.dir/clean

src/CMakeFiles/PMQ.dir/depend:
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/src" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/src" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/src/CMakeFiles/PMQ.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : src/CMakeFiles/PMQ.dir/depend

