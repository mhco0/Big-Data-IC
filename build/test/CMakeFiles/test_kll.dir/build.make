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
include test/CMakeFiles/test_kll.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/test_kll.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/test_kll.dir/flags.make

test/CMakeFiles/test_kll.dir/test_kll.cpp.o: test/CMakeFiles/test_kll.dir/flags.make
test/CMakeFiles/test_kll.dir/test_kll.cpp.o: ../test/test_kll.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/test_kll.dir/test_kll.cpp.o"
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/test" && /bin/x86_64-linux-gnu-g++-9  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_kll.dir/test_kll.cpp.o -c "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/test/test_kll.cpp"

test/CMakeFiles/test_kll.dir/test_kll.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_kll.dir/test_kll.cpp.i"
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/test" && /bin/x86_64-linux-gnu-g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/test/test_kll.cpp" > CMakeFiles/test_kll.dir/test_kll.cpp.i

test/CMakeFiles/test_kll.dir/test_kll.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_kll.dir/test_kll.cpp.s"
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/test" && /bin/x86_64-linux-gnu-g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/test/test_kll.cpp" -o CMakeFiles/test_kll.dir/test_kll.cpp.s

# Object files for target test_kll
test_kll_OBJECTS = \
"CMakeFiles/test_kll.dir/test_kll.cpp.o"

# External object files for target test_kll
test_kll_EXTERNAL_OBJECTS =

bin/test_kll: test/CMakeFiles/test_kll.dir/test_kll.cpp.o
bin/test_kll: test/CMakeFiles/test_kll.dir/build.make
bin/test_kll: lib/libgtest_maind.a
bin/test_kll: lib/libQSBD_KLL.a
bin/test_kll: lib/libQSBD_STREAM_MAKER.a
bin/test_kll: lib/libQSBD_KLL_FACTORY.a
bin/test_kll: lib/libQSBD_UTILS.a
bin/test_kll: lib/libgtestd.a
bin/test_kll: lib/libQSBD_KLL.a
bin/test_kll: lib/libQSBD_COMMUM_HEADER.a
bin/test_kll: lib/libQSBD_GLOBAL_GEN.a
bin/test_kll: lib/libQSBD_SKETCH_FACTORY.a
bin/test_kll: test/CMakeFiles/test_kll.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/test_kll"
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/test" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_kll.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/test_kll.dir/build: bin/test_kll

.PHONY : test/CMakeFiles/test_kll.dir/build

test/CMakeFiles/test_kll.dir/clean:
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/test" && $(CMAKE_COMMAND) -P CMakeFiles/test_kll.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/test_kll.dir/clean

test/CMakeFiles/test_kll.dir/depend:
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/test" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/test" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/test/CMakeFiles/test_kll.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : test/CMakeFiles/test_kll.dir/depend

