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
CMAKE_SOURCE_DIR = /home/mhco/projects/Big-Data-IC

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mhco/projects/Big-Data-IC/build

# Include any dependencies generated for this target.
include src/CMakeFiles/QSBD_STREAM_MAKER.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/QSBD_STREAM_MAKER.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/QSBD_STREAM_MAKER.dir/flags.make

src/CMakeFiles/QSBD_STREAM_MAKER.dir/stream_maker/stream_maker.cpp.o: src/CMakeFiles/QSBD_STREAM_MAKER.dir/flags.make
src/CMakeFiles/QSBD_STREAM_MAKER.dir/stream_maker/stream_maker.cpp.o: ../src/stream_maker/stream_maker.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mhco/projects/Big-Data-IC/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/QSBD_STREAM_MAKER.dir/stream_maker/stream_maker.cpp.o"
	cd /home/mhco/projects/Big-Data-IC/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/QSBD_STREAM_MAKER.dir/stream_maker/stream_maker.cpp.o -c /home/mhco/projects/Big-Data-IC/src/stream_maker/stream_maker.cpp

src/CMakeFiles/QSBD_STREAM_MAKER.dir/stream_maker/stream_maker.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/QSBD_STREAM_MAKER.dir/stream_maker/stream_maker.cpp.i"
	cd /home/mhco/projects/Big-Data-IC/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mhco/projects/Big-Data-IC/src/stream_maker/stream_maker.cpp > CMakeFiles/QSBD_STREAM_MAKER.dir/stream_maker/stream_maker.cpp.i

src/CMakeFiles/QSBD_STREAM_MAKER.dir/stream_maker/stream_maker.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/QSBD_STREAM_MAKER.dir/stream_maker/stream_maker.cpp.s"
	cd /home/mhco/projects/Big-Data-IC/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mhco/projects/Big-Data-IC/src/stream_maker/stream_maker.cpp -o CMakeFiles/QSBD_STREAM_MAKER.dir/stream_maker/stream_maker.cpp.s

# Object files for target QSBD_STREAM_MAKER
QSBD_STREAM_MAKER_OBJECTS = \
"CMakeFiles/QSBD_STREAM_MAKER.dir/stream_maker/stream_maker.cpp.o"

# External object files for target QSBD_STREAM_MAKER
QSBD_STREAM_MAKER_EXTERNAL_OBJECTS =

lib/libQSBD_STREAM_MAKER.a: src/CMakeFiles/QSBD_STREAM_MAKER.dir/stream_maker/stream_maker.cpp.o
lib/libQSBD_STREAM_MAKER.a: src/CMakeFiles/QSBD_STREAM_MAKER.dir/build.make
lib/libQSBD_STREAM_MAKER.a: src/CMakeFiles/QSBD_STREAM_MAKER.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/mhco/projects/Big-Data-IC/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library ../lib/libQSBD_STREAM_MAKER.a"
	cd /home/mhco/projects/Big-Data-IC/build/src && $(CMAKE_COMMAND) -P CMakeFiles/QSBD_STREAM_MAKER.dir/cmake_clean_target.cmake
	cd /home/mhco/projects/Big-Data-IC/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/QSBD_STREAM_MAKER.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/QSBD_STREAM_MAKER.dir/build: lib/libQSBD_STREAM_MAKER.a

.PHONY : src/CMakeFiles/QSBD_STREAM_MAKER.dir/build

src/CMakeFiles/QSBD_STREAM_MAKER.dir/clean:
	cd /home/mhco/projects/Big-Data-IC/build/src && $(CMAKE_COMMAND) -P CMakeFiles/QSBD_STREAM_MAKER.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/QSBD_STREAM_MAKER.dir/clean

src/CMakeFiles/QSBD_STREAM_MAKER.dir/depend:
	cd /home/mhco/projects/Big-Data-IC/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mhco/projects/Big-Data-IC /home/mhco/projects/Big-Data-IC/src /home/mhco/projects/Big-Data-IC/build /home/mhco/projects/Big-Data-IC/build/src /home/mhco/projects/Big-Data-IC/build/src/CMakeFiles/QSBD_STREAM_MAKER.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/QSBD_STREAM_MAKER.dir/depend

