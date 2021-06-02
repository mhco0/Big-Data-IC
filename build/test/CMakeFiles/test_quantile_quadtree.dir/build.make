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
include test/CMakeFiles/test_quantile_quadtree.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/test_quantile_quadtree.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/test_quantile_quadtree.dir/flags.make

test/CMakeFiles/test_quantile_quadtree.dir/test_quantile_quadtree.cpp.o: test/CMakeFiles/test_quantile_quadtree.dir/flags.make
test/CMakeFiles/test_quantile_quadtree.dir/test_quantile_quadtree.cpp.o: ../test/test_quantile_quadtree.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mhco/projects/Big-Data-IC/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/test_quantile_quadtree.dir/test_quantile_quadtree.cpp.o"
	cd /home/mhco/projects/Big-Data-IC/build/test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_quantile_quadtree.dir/test_quantile_quadtree.cpp.o -c /home/mhco/projects/Big-Data-IC/test/test_quantile_quadtree.cpp

test/CMakeFiles/test_quantile_quadtree.dir/test_quantile_quadtree.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_quantile_quadtree.dir/test_quantile_quadtree.cpp.i"
	cd /home/mhco/projects/Big-Data-IC/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mhco/projects/Big-Data-IC/test/test_quantile_quadtree.cpp > CMakeFiles/test_quantile_quadtree.dir/test_quantile_quadtree.cpp.i

test/CMakeFiles/test_quantile_quadtree.dir/test_quantile_quadtree.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_quantile_quadtree.dir/test_quantile_quadtree.cpp.s"
	cd /home/mhco/projects/Big-Data-IC/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mhco/projects/Big-Data-IC/test/test_quantile_quadtree.cpp -o CMakeFiles/test_quantile_quadtree.dir/test_quantile_quadtree.cpp.s

# Object files for target test_quantile_quadtree
test_quantile_quadtree_OBJECTS = \
"CMakeFiles/test_quantile_quadtree.dir/test_quantile_quadtree.cpp.o"

# External object files for target test_quantile_quadtree
test_quantile_quadtree_EXTERNAL_OBJECTS =

bin/test_quantile_quadtree: test/CMakeFiles/test_quantile_quadtree.dir/test_quantile_quadtree.cpp.o
bin/test_quantile_quadtree: test/CMakeFiles/test_quantile_quadtree.dir/build.make
bin/test_quantile_quadtree: lib/libQSBD_QUANTILE_QUADTREE.a
bin/test_quantile_quadtree: lib/libQSBD_STREAM_MAKER.a
bin/test_quantile_quadtree: lib/libQSBD_COUNT_SKETCH.a
bin/test_quantile_quadtree: lib/libQSBD_KLL_FACTORY.a
bin/test_quantile_quadtree: lib/libQSBD_GK_FACTORY.a
bin/test_quantile_quadtree: lib/libQSBD_Q_DIGEST_FACTORY.a
bin/test_quantile_quadtree: lib/libQSBD_DCS_FACTORY.a
bin/test_quantile_quadtree: lib/libQSBD_MEMORY_TRACKER.a
bin/test_quantile_quadtree: lib/libQSBD_TIMER.a
bin/test_quantile_quadtree: lib/libQSBD_DEBUG.a
bin/test_quantile_quadtree: lib/libQSBD_UTILS.a
bin/test_quantile_quadtree: lib/libQSBD_AABB.a
bin/test_quantile_quadtree: lib/libQSBD_POINT.a
bin/test_quantile_quadtree: lib/libQSBD_KLL.a
bin/test_quantile_quadtree: lib/libQSBD_GK.a
bin/test_quantile_quadtree: lib/libQSBD_Q_DIGEST.a
bin/test_quantile_quadtree: lib/libQSBD_SKETCH_FACTORY.a
bin/test_quantile_quadtree: lib/libQSBD_DCS.a
bin/test_quantile_quadtree: lib/libQSBD_COUNT_SKETCH.a
bin/test_quantile_quadtree: lib/libQSBD_K_WISE_FAMILY.a
bin/test_quantile_quadtree: lib/libQSBD_DEBUG.a
bin/test_quantile_quadtree: lib/libQSBD_UTILS.a
bin/test_quantile_quadtree: lib/libQSBD_QUANTILE_SKETCH.a
bin/test_quantile_quadtree: lib/libQSBD_COMMUM_HEADER.a
bin/test_quantile_quadtree: lib/libQSBD_GLOBAL_GEN.a
bin/test_quantile_quadtree: lib/libnlohmann_json.a
bin/test_quantile_quadtree: test/CMakeFiles/test_quantile_quadtree.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/mhco/projects/Big-Data-IC/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/test_quantile_quadtree"
	cd /home/mhco/projects/Big-Data-IC/build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_quantile_quadtree.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/test_quantile_quadtree.dir/build: bin/test_quantile_quadtree

.PHONY : test/CMakeFiles/test_quantile_quadtree.dir/build

test/CMakeFiles/test_quantile_quadtree.dir/clean:
	cd /home/mhco/projects/Big-Data-IC/build/test && $(CMAKE_COMMAND) -P CMakeFiles/test_quantile_quadtree.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/test_quantile_quadtree.dir/clean

test/CMakeFiles/test_quantile_quadtree.dir/depend:
	cd /home/mhco/projects/Big-Data-IC/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mhco/projects/Big-Data-IC /home/mhco/projects/Big-Data-IC/test /home/mhco/projects/Big-Data-IC/build /home/mhco/projects/Big-Data-IC/build/test /home/mhco/projects/Big-Data-IC/build/test/CMakeFiles/test_quantile_quadtree.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/test_quantile_quadtree.dir/depend

