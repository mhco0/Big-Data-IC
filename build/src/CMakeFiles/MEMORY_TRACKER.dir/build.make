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
include src/CMakeFiles/MEMORY_TRACKER.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/MEMORY_TRACKER.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/MEMORY_TRACKER.dir/flags.make

src/CMakeFiles/MEMORY_TRACKER.dir/memory_tracker/memory_tracker.cpp.o: src/CMakeFiles/MEMORY_TRACKER.dir/flags.make
src/CMakeFiles/MEMORY_TRACKER.dir/memory_tracker/memory_tracker.cpp.o: ../src/memory_tracker/memory_tracker.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/MEMORY_TRACKER.dir/memory_tracker/memory_tracker.cpp.o"
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/src" && /bin/x86_64-linux-gnu-g++-9  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/MEMORY_TRACKER.dir/memory_tracker/memory_tracker.cpp.o -c "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/src/memory_tracker/memory_tracker.cpp"

src/CMakeFiles/MEMORY_TRACKER.dir/memory_tracker/memory_tracker.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MEMORY_TRACKER.dir/memory_tracker/memory_tracker.cpp.i"
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/src" && /bin/x86_64-linux-gnu-g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/src/memory_tracker/memory_tracker.cpp" > CMakeFiles/MEMORY_TRACKER.dir/memory_tracker/memory_tracker.cpp.i

src/CMakeFiles/MEMORY_TRACKER.dir/memory_tracker/memory_tracker.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MEMORY_TRACKER.dir/memory_tracker/memory_tracker.cpp.s"
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/src" && /bin/x86_64-linux-gnu-g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/src/memory_tracker/memory_tracker.cpp" -o CMakeFiles/MEMORY_TRACKER.dir/memory_tracker/memory_tracker.cpp.s

# Object files for target MEMORY_TRACKER
MEMORY_TRACKER_OBJECTS = \
"CMakeFiles/MEMORY_TRACKER.dir/memory_tracker/memory_tracker.cpp.o"

# External object files for target MEMORY_TRACKER
MEMORY_TRACKER_EXTERNAL_OBJECTS =

lib/libMEMORY_TRACKER.a: src/CMakeFiles/MEMORY_TRACKER.dir/memory_tracker/memory_tracker.cpp.o
lib/libMEMORY_TRACKER.a: src/CMakeFiles/MEMORY_TRACKER.dir/build.make
lib/libMEMORY_TRACKER.a: src/CMakeFiles/MEMORY_TRACKER.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library ../lib/libMEMORY_TRACKER.a"
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/src" && $(CMAKE_COMMAND) -P CMakeFiles/MEMORY_TRACKER.dir/cmake_clean_target.cmake
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/src" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MEMORY_TRACKER.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/MEMORY_TRACKER.dir/build: lib/libMEMORY_TRACKER.a

.PHONY : src/CMakeFiles/MEMORY_TRACKER.dir/build

src/CMakeFiles/MEMORY_TRACKER.dir/clean:
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/src" && $(CMAKE_COMMAND) -P CMakeFiles/MEMORY_TRACKER.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/MEMORY_TRACKER.dir/clean

src/CMakeFiles/MEMORY_TRACKER.dir/depend:
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/src" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/src" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/src/CMakeFiles/MEMORY_TRACKER.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : src/CMakeFiles/MEMORY_TRACKER.dir/depend

