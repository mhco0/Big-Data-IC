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
include src/CMakeFiles/K_WISE_FAMILY.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/K_WISE_FAMILY.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/K_WISE_FAMILY.dir/flags.make

src/CMakeFiles/K_WISE_FAMILY.dir/k_wise_family/k_wise_family.cpp.o: src/CMakeFiles/K_WISE_FAMILY.dir/flags.make
src/CMakeFiles/K_WISE_FAMILY.dir/k_wise_family/k_wise_family.cpp.o: ../src/k_wise_family/k_wise_family.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/K_WISE_FAMILY.dir/k_wise_family/k_wise_family.cpp.o"
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/src" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/K_WISE_FAMILY.dir/k_wise_family/k_wise_family.cpp.o -c "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/src/k_wise_family/k_wise_family.cpp"

src/CMakeFiles/K_WISE_FAMILY.dir/k_wise_family/k_wise_family.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/K_WISE_FAMILY.dir/k_wise_family/k_wise_family.cpp.i"
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/src" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/src/k_wise_family/k_wise_family.cpp" > CMakeFiles/K_WISE_FAMILY.dir/k_wise_family/k_wise_family.cpp.i

src/CMakeFiles/K_WISE_FAMILY.dir/k_wise_family/k_wise_family.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/K_WISE_FAMILY.dir/k_wise_family/k_wise_family.cpp.s"
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/src" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/src/k_wise_family/k_wise_family.cpp" -o CMakeFiles/K_WISE_FAMILY.dir/k_wise_family/k_wise_family.cpp.s

# Object files for target K_WISE_FAMILY
K_WISE_FAMILY_OBJECTS = \
"CMakeFiles/K_WISE_FAMILY.dir/k_wise_family/k_wise_family.cpp.o"

# External object files for target K_WISE_FAMILY
K_WISE_FAMILY_EXTERNAL_OBJECTS =

lib/libK_WISE_FAMILY.a: src/CMakeFiles/K_WISE_FAMILY.dir/k_wise_family/k_wise_family.cpp.o
lib/libK_WISE_FAMILY.a: src/CMakeFiles/K_WISE_FAMILY.dir/build.make
lib/libK_WISE_FAMILY.a: src/CMakeFiles/K_WISE_FAMILY.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library ../lib/libK_WISE_FAMILY.a"
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/src" && $(CMAKE_COMMAND) -P CMakeFiles/K_WISE_FAMILY.dir/cmake_clean_target.cmake
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/src" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/K_WISE_FAMILY.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/K_WISE_FAMILY.dir/build: lib/libK_WISE_FAMILY.a

.PHONY : src/CMakeFiles/K_WISE_FAMILY.dir/build

src/CMakeFiles/K_WISE_FAMILY.dir/clean:
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/src" && $(CMAKE_COMMAND) -P CMakeFiles/K_WISE_FAMILY.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/K_WISE_FAMILY.dir/clean

src/CMakeFiles/K_WISE_FAMILY.dir/depend:
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/src" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/src" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/src/CMakeFiles/K_WISE_FAMILY.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : src/CMakeFiles/K_WISE_FAMILY.dir/depend

