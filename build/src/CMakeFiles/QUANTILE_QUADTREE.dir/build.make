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
include src/CMakeFiles/QUANTILE_QUADTREE.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/QUANTILE_QUADTREE.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/QUANTILE_QUADTREE.dir/flags.make

# Object files for target QUANTILE_QUADTREE
QUANTILE_QUADTREE_OBJECTS =

# External object files for target QUANTILE_QUADTREE
QUANTILE_QUADTREE_EXTERNAL_OBJECTS =

lib/libQUANTILE_QUADTREE.a: src/CMakeFiles/QUANTILE_QUADTREE.dir/build.make
lib/libQUANTILE_QUADTREE.a: src/CMakeFiles/QUANTILE_QUADTREE.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Linking CXX static library ../lib/libQUANTILE_QUADTREE.a"
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/src" && $(CMAKE_COMMAND) -P CMakeFiles/QUANTILE_QUADTREE.dir/cmake_clean_target.cmake
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/src" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/QUANTILE_QUADTREE.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/QUANTILE_QUADTREE.dir/build: lib/libQUANTILE_QUADTREE.a

.PHONY : src/CMakeFiles/QUANTILE_QUADTREE.dir/build

src/CMakeFiles/QUANTILE_QUADTREE.dir/clean:
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/src" && $(CMAKE_COMMAND) -P CMakeFiles/QUANTILE_QUADTREE.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/QUANTILE_QUADTREE.dir/clean

src/CMakeFiles/QUANTILE_QUADTREE.dir/depend:
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/src" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/src" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/src/CMakeFiles/QUANTILE_QUADTREE.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : src/CMakeFiles/QUANTILE_QUADTREE.dir/depend

