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
include src/CMakeFiles/QSBD_AABB.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/QSBD_AABB.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/QSBD_AABB.dir/flags.make

src/CMakeFiles/QSBD_AABB.dir/aabb/aabb.cpp.o: src/CMakeFiles/QSBD_AABB.dir/flags.make
src/CMakeFiles/QSBD_AABB.dir/aabb/aabb.cpp.o: ../src/aabb/aabb.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/QSBD_AABB.dir/aabb/aabb.cpp.o"
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/src" && /bin/x86_64-linux-gnu-g++-9  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/QSBD_AABB.dir/aabb/aabb.cpp.o -c "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/src/aabb/aabb.cpp"

src/CMakeFiles/QSBD_AABB.dir/aabb/aabb.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/QSBD_AABB.dir/aabb/aabb.cpp.i"
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/src" && /bin/x86_64-linux-gnu-g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/src/aabb/aabb.cpp" > CMakeFiles/QSBD_AABB.dir/aabb/aabb.cpp.i

src/CMakeFiles/QSBD_AABB.dir/aabb/aabb.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/QSBD_AABB.dir/aabb/aabb.cpp.s"
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/src" && /bin/x86_64-linux-gnu-g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/src/aabb/aabb.cpp" -o CMakeFiles/QSBD_AABB.dir/aabb/aabb.cpp.s

# Object files for target QSBD_AABB
QSBD_AABB_OBJECTS = \
"CMakeFiles/QSBD_AABB.dir/aabb/aabb.cpp.o"

# External object files for target QSBD_AABB
QSBD_AABB_EXTERNAL_OBJECTS =

lib/libQSBD_AABB.a: src/CMakeFiles/QSBD_AABB.dir/aabb/aabb.cpp.o
lib/libQSBD_AABB.a: src/CMakeFiles/QSBD_AABB.dir/build.make
lib/libQSBD_AABB.a: src/CMakeFiles/QSBD_AABB.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library ../lib/libQSBD_AABB.a"
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/src" && $(CMAKE_COMMAND) -P CMakeFiles/QSBD_AABB.dir/cmake_clean_target.cmake
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/src" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/QSBD_AABB.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/QSBD_AABB.dir/build: lib/libQSBD_AABB.a

.PHONY : src/CMakeFiles/QSBD_AABB.dir/build

src/CMakeFiles/QSBD_AABB.dir/clean:
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/src" && $(CMAKE_COMMAND) -P CMakeFiles/QSBD_AABB.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/QSBD_AABB.dir/clean

src/CMakeFiles/QSBD_AABB.dir/depend:
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/src" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/src" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/src/CMakeFiles/QSBD_AABB.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : src/CMakeFiles/QSBD_AABB.dir/depend

