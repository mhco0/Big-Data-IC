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
include app/CMakeFiles/quantile_quadtree_experiment.dir/depend.make

# Include the progress variables for this target.
include app/CMakeFiles/quantile_quadtree_experiment.dir/progress.make

# Include the compile flags for this target's objects.
include app/CMakeFiles/quantile_quadtree_experiment.dir/flags.make

app/CMakeFiles/quantile_quadtree_experiment.dir/quantile_quadtree_experiment/quantile_quadtree_experiment.cpp.o: app/CMakeFiles/quantile_quadtree_experiment.dir/flags.make
app/CMakeFiles/quantile_quadtree_experiment.dir/quantile_quadtree_experiment/quantile_quadtree_experiment.cpp.o: ../app/quantile_quadtree_experiment/quantile_quadtree_experiment.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object app/CMakeFiles/quantile_quadtree_experiment.dir/quantile_quadtree_experiment/quantile_quadtree_experiment.cpp.o"
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/app" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/quantile_quadtree_experiment.dir/quantile_quadtree_experiment/quantile_quadtree_experiment.cpp.o -c "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/app/quantile_quadtree_experiment/quantile_quadtree_experiment.cpp"

app/CMakeFiles/quantile_quadtree_experiment.dir/quantile_quadtree_experiment/quantile_quadtree_experiment.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/quantile_quadtree_experiment.dir/quantile_quadtree_experiment/quantile_quadtree_experiment.cpp.i"
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/app" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/app/quantile_quadtree_experiment/quantile_quadtree_experiment.cpp" > CMakeFiles/quantile_quadtree_experiment.dir/quantile_quadtree_experiment/quantile_quadtree_experiment.cpp.i

app/CMakeFiles/quantile_quadtree_experiment.dir/quantile_quadtree_experiment/quantile_quadtree_experiment.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/quantile_quadtree_experiment.dir/quantile_quadtree_experiment/quantile_quadtree_experiment.cpp.s"
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/app" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/app/quantile_quadtree_experiment/quantile_quadtree_experiment.cpp" -o CMakeFiles/quantile_quadtree_experiment.dir/quantile_quadtree_experiment/quantile_quadtree_experiment.cpp.s

# Object files for target quantile_quadtree_experiment
quantile_quadtree_experiment_OBJECTS = \
"CMakeFiles/quantile_quadtree_experiment.dir/quantile_quadtree_experiment/quantile_quadtree_experiment.cpp.o"

# External object files for target quantile_quadtree_experiment
quantile_quadtree_experiment_EXTERNAL_OBJECTS =

bin/quantile_quadtree_experiment: app/CMakeFiles/quantile_quadtree_experiment.dir/quantile_quadtree_experiment/quantile_quadtree_experiment.cpp.o
bin/quantile_quadtree_experiment: app/CMakeFiles/quantile_quadtree_experiment.dir/build.make
bin/quantile_quadtree_experiment: lib/libQSBD_TIMER.a
bin/quantile_quadtree_experiment: lib/libQSBD_QUANTILE_QUADTREE.a
bin/quantile_quadtree_experiment: lib/libQSBD_GK_FACTORY.a
bin/quantile_quadtree_experiment: lib/libQSBD_KLL_FACTORY.a
bin/quantile_quadtree_experiment: lib/libQSBD_DCS_FACTORY.a
bin/quantile_quadtree_experiment: lib/libQSBD_Q_DIGEST_FACTORY.a
bin/quantile_quadtree_experiment: lib/libQSBD_UTILS.a
bin/quantile_quadtree_experiment: lib/libQSBD_LOG.a
bin/quantile_quadtree_experiment: lib/libQSBD_DEBUG.a
bin/quantile_quadtree_experiment: lib/libQSBD_MEMORY_TRACKER.a
bin/quantile_quadtree_experiment: lib/libQSBD_AABB.a
bin/quantile_quadtree_experiment: lib/libQSBD_POINT.a
bin/quantile_quadtree_experiment: lib/libQSBD_GK.a
bin/quantile_quadtree_experiment: lib/libQSBD_KLL.a
bin/quantile_quadtree_experiment: lib/libQSBD_DCS.a
bin/quantile_quadtree_experiment: lib/libQSBD_COUNT_SKETCH.a
bin/quantile_quadtree_experiment: lib/libQSBD_COMMUM_HEADER.a
bin/quantile_quadtree_experiment: lib/libQSBD_TWO_WISE_FAMILY.a
bin/quantile_quadtree_experiment: lib/libQSBD_GLOBAL_GEN.a
bin/quantile_quadtree_experiment: lib/libQSBD_SKETCH_FACTORY.a
bin/quantile_quadtree_experiment: lib/libQSBD_Q_DIGEST.a
bin/quantile_quadtree_experiment: lib/libQSBD_UTILS.a
bin/quantile_quadtree_experiment: lib/libQSBD_QUANTILE_SKETCH.a
bin/quantile_quadtree_experiment: lib/libQSBD_DEBUG.a
bin/quantile_quadtree_experiment: lib/libnlohmann_json.a
bin/quantile_quadtree_experiment: app/CMakeFiles/quantile_quadtree_experiment.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/quantile_quadtree_experiment"
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/app" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/quantile_quadtree_experiment.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
app/CMakeFiles/quantile_quadtree_experiment.dir/build: bin/quantile_quadtree_experiment

.PHONY : app/CMakeFiles/quantile_quadtree_experiment.dir/build

app/CMakeFiles/quantile_quadtree_experiment.dir/clean:
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/app" && $(CMAKE_COMMAND) -P CMakeFiles/quantile_quadtree_experiment.dir/cmake_clean.cmake
.PHONY : app/CMakeFiles/quantile_quadtree_experiment.dir/clean

app/CMakeFiles/quantile_quadtree_experiment.dir/depend:
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/app" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/app" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/app/CMakeFiles/quantile_quadtree_experiment.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : app/CMakeFiles/quantile_quadtree_experiment.dir/depend

