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
CMAKE_BINARY_DIR = "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC"

# Include any dependencies generated for this target.
include src/CMakeFiles/PBPLOTS_SUPPORT_LIB.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/PBPLOTS_SUPPORT_LIB.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/PBPLOTS_SUPPORT_LIB.dir/flags.make

src/CMakeFiles/PBPLOTS_SUPPORT_LIB.dir/pbplots/supportLib.cpp.o: src/CMakeFiles/PBPLOTS_SUPPORT_LIB.dir/flags.make
src/CMakeFiles/PBPLOTS_SUPPORT_LIB.dir/pbplots/supportLib.cpp.o: src/pbplots/supportLib.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/PBPLOTS_SUPPORT_LIB.dir/pbplots/supportLib.cpp.o"
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/src" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/PBPLOTS_SUPPORT_LIB.dir/pbplots/supportLib.cpp.o -c "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/src/pbplots/supportLib.cpp"

src/CMakeFiles/PBPLOTS_SUPPORT_LIB.dir/pbplots/supportLib.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/PBPLOTS_SUPPORT_LIB.dir/pbplots/supportLib.cpp.i"
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/src" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/src/pbplots/supportLib.cpp" > CMakeFiles/PBPLOTS_SUPPORT_LIB.dir/pbplots/supportLib.cpp.i

src/CMakeFiles/PBPLOTS_SUPPORT_LIB.dir/pbplots/supportLib.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/PBPLOTS_SUPPORT_LIB.dir/pbplots/supportLib.cpp.s"
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/src" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/src/pbplots/supportLib.cpp" -o CMakeFiles/PBPLOTS_SUPPORT_LIB.dir/pbplots/supportLib.cpp.s

# Object files for target PBPLOTS_SUPPORT_LIB
PBPLOTS_SUPPORT_LIB_OBJECTS = \
"CMakeFiles/PBPLOTS_SUPPORT_LIB.dir/pbplots/supportLib.cpp.o"

# External object files for target PBPLOTS_SUPPORT_LIB
PBPLOTS_SUPPORT_LIB_EXTERNAL_OBJECTS =

lib/libPBPLOTS_SUPPORT_LIB.a: src/CMakeFiles/PBPLOTS_SUPPORT_LIB.dir/pbplots/supportLib.cpp.o
lib/libPBPLOTS_SUPPORT_LIB.a: src/CMakeFiles/PBPLOTS_SUPPORT_LIB.dir/build.make
lib/libPBPLOTS_SUPPORT_LIB.a: src/CMakeFiles/PBPLOTS_SUPPORT_LIB.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library ../lib/libPBPLOTS_SUPPORT_LIB.a"
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/src" && $(CMAKE_COMMAND) -P CMakeFiles/PBPLOTS_SUPPORT_LIB.dir/cmake_clean_target.cmake
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/src" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/PBPLOTS_SUPPORT_LIB.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/PBPLOTS_SUPPORT_LIB.dir/build: lib/libPBPLOTS_SUPPORT_LIB.a

.PHONY : src/CMakeFiles/PBPLOTS_SUPPORT_LIB.dir/build

src/CMakeFiles/PBPLOTS_SUPPORT_LIB.dir/clean:
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/src" && $(CMAKE_COMMAND) -P CMakeFiles/PBPLOTS_SUPPORT_LIB.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/PBPLOTS_SUPPORT_LIB.dir/clean

src/CMakeFiles/PBPLOTS_SUPPORT_LIB.dir/depend:
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/src" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/src" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/src/CMakeFiles/PBPLOTS_SUPPORT_LIB.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : src/CMakeFiles/PBPLOTS_SUPPORT_LIB.dir/depend

