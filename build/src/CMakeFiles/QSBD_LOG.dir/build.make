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
include src/CMakeFiles/QSBD_LOG.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/QSBD_LOG.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/QSBD_LOG.dir/flags.make

src/CMakeFiles/QSBD_LOG.dir/logger/logger.cpp.o: src/CMakeFiles/QSBD_LOG.dir/flags.make
src/CMakeFiles/QSBD_LOG.dir/logger/logger.cpp.o: ../src/logger/logger.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mhco/projects/Big-Data-IC/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/QSBD_LOG.dir/logger/logger.cpp.o"
	cd /home/mhco/projects/Big-Data-IC/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/QSBD_LOG.dir/logger/logger.cpp.o -c /home/mhco/projects/Big-Data-IC/src/logger/logger.cpp

src/CMakeFiles/QSBD_LOG.dir/logger/logger.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/QSBD_LOG.dir/logger/logger.cpp.i"
	cd /home/mhco/projects/Big-Data-IC/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mhco/projects/Big-Data-IC/src/logger/logger.cpp > CMakeFiles/QSBD_LOG.dir/logger/logger.cpp.i

src/CMakeFiles/QSBD_LOG.dir/logger/logger.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/QSBD_LOG.dir/logger/logger.cpp.s"
	cd /home/mhco/projects/Big-Data-IC/build/src && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mhco/projects/Big-Data-IC/src/logger/logger.cpp -o CMakeFiles/QSBD_LOG.dir/logger/logger.cpp.s

# Object files for target QSBD_LOG
QSBD_LOG_OBJECTS = \
"CMakeFiles/QSBD_LOG.dir/logger/logger.cpp.o"

# External object files for target QSBD_LOG
QSBD_LOG_EXTERNAL_OBJECTS =

lib/libQSBD_LOG.a: src/CMakeFiles/QSBD_LOG.dir/logger/logger.cpp.o
lib/libQSBD_LOG.a: src/CMakeFiles/QSBD_LOG.dir/build.make
lib/libQSBD_LOG.a: src/CMakeFiles/QSBD_LOG.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/mhco/projects/Big-Data-IC/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library ../lib/libQSBD_LOG.a"
	cd /home/mhco/projects/Big-Data-IC/build/src && $(CMAKE_COMMAND) -P CMakeFiles/QSBD_LOG.dir/cmake_clean_target.cmake
	cd /home/mhco/projects/Big-Data-IC/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/QSBD_LOG.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/QSBD_LOG.dir/build: lib/libQSBD_LOG.a

.PHONY : src/CMakeFiles/QSBD_LOG.dir/build

src/CMakeFiles/QSBD_LOG.dir/clean:
	cd /home/mhco/projects/Big-Data-IC/build/src && $(CMAKE_COMMAND) -P CMakeFiles/QSBD_LOG.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/QSBD_LOG.dir/clean

src/CMakeFiles/QSBD_LOG.dir/depend:
	cd /home/mhco/projects/Big-Data-IC/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mhco/projects/Big-Data-IC /home/mhco/projects/Big-Data-IC/src /home/mhco/projects/Big-Data-IC/build /home/mhco/projects/Big-Data-IC/build/src /home/mhco/projects/Big-Data-IC/build/src/CMakeFiles/QSBD_LOG.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/QSBD_LOG.dir/depend

