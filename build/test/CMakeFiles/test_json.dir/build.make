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
include test/CMakeFiles/test_json.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/test_json.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/test_json.dir/flags.make

test/CMakeFiles/test_json.dir/test_json.cpp.o: test/CMakeFiles/test_json.dir/flags.make
test/CMakeFiles/test_json.dir/test_json.cpp.o: ../test/test_json.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/test_json.dir/test_json.cpp.o"
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/test" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_json.dir/test_json.cpp.o -c "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/test/test_json.cpp"

test/CMakeFiles/test_json.dir/test_json.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_json.dir/test_json.cpp.i"
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/test" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/test/test_json.cpp" > CMakeFiles/test_json.dir/test_json.cpp.i

test/CMakeFiles/test_json.dir/test_json.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_json.dir/test_json.cpp.s"
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/test" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/test/test_json.cpp" -o CMakeFiles/test_json.dir/test_json.cpp.s

# Object files for target test_json
test_json_OBJECTS = \
"CMakeFiles/test_json.dir/test_json.cpp.o"

# External object files for target test_json
test_json_EXTERNAL_OBJECTS =

bin/test_json: test/CMakeFiles/test_json.dir/test_json.cpp.o
bin/test_json: test/CMakeFiles/test_json.dir/build.make
bin/test_json: lib/libnlohmann_json.a
bin/test_json: test/CMakeFiles/test_json.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/test_json"
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/test" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_json.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/test_json.dir/build: bin/test_json

.PHONY : test/CMakeFiles/test_json.dir/build

test/CMakeFiles/test_json.dir/clean:
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/test" && $(CMAKE_COMMAND) -P CMakeFiles/test_json.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/test_json.dir/clean

test/CMakeFiles/test_json.dir/depend:
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/test" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/test" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/test/CMakeFiles/test_json.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : test/CMakeFiles/test_json.dir/depend

