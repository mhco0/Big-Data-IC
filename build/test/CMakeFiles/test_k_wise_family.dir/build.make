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
CMAKE_SOURCE_DIR = "/mnt/c/Users/Marcos/OneDrive/Área de Trabalho/Meus Projetos/Big-Data-IC"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/mnt/c/Users/Marcos/OneDrive/Área de Trabalho/Meus Projetos/Big-Data-IC/build"

# Include any dependencies generated for this target.
include test/CMakeFiles/test_k_wise_family.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/test_k_wise_family.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/test_k_wise_family.dir/flags.make

test/CMakeFiles/test_k_wise_family.dir/test_k_wise_family.cpp.o: test/CMakeFiles/test_k_wise_family.dir/flags.make
test/CMakeFiles/test_k_wise_family.dir/test_k_wise_family.cpp.o: ../test/test_k_wise_family.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/mnt/c/Users/Marcos/OneDrive/Área de Trabalho/Meus Projetos/Big-Data-IC/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/test_k_wise_family.dir/test_k_wise_family.cpp.o"
	cd "/mnt/c/Users/Marcos/OneDrive/Área de Trabalho/Meus Projetos/Big-Data-IC/build/test" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_k_wise_family.dir/test_k_wise_family.cpp.o -c "/mnt/c/Users/Marcos/OneDrive/Área de Trabalho/Meus Projetos/Big-Data-IC/test/test_k_wise_family.cpp"

test/CMakeFiles/test_k_wise_family.dir/test_k_wise_family.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_k_wise_family.dir/test_k_wise_family.cpp.i"
	cd "/mnt/c/Users/Marcos/OneDrive/Área de Trabalho/Meus Projetos/Big-Data-IC/build/test" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/mnt/c/Users/Marcos/OneDrive/Área de Trabalho/Meus Projetos/Big-Data-IC/test/test_k_wise_family.cpp" > CMakeFiles/test_k_wise_family.dir/test_k_wise_family.cpp.i

test/CMakeFiles/test_k_wise_family.dir/test_k_wise_family.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_k_wise_family.dir/test_k_wise_family.cpp.s"
	cd "/mnt/c/Users/Marcos/OneDrive/Área de Trabalho/Meus Projetos/Big-Data-IC/build/test" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/mnt/c/Users/Marcos/OneDrive/Área de Trabalho/Meus Projetos/Big-Data-IC/test/test_k_wise_family.cpp" -o CMakeFiles/test_k_wise_family.dir/test_k_wise_family.cpp.s

# Object files for target test_k_wise_family
test_k_wise_family_OBJECTS = \
"CMakeFiles/test_k_wise_family.dir/test_k_wise_family.cpp.o"

# External object files for target test_k_wise_family
test_k_wise_family_EXTERNAL_OBJECTS =

bin/test_k_wise_family: test/CMakeFiles/test_k_wise_family.dir/test_k_wise_family.cpp.o
bin/test_k_wise_family: test/CMakeFiles/test_k_wise_family.dir/build.make
bin/test_k_wise_family: lib/libQSBD_K_WISE_FAMILY.a
bin/test_k_wise_family: lib/libQSBD_GLOBAL_GEN.a
bin/test_k_wise_family: lib/libQSBD_COMMUM_HEADER.a
bin/test_k_wise_family: lib/libQSBD_UTILS.a
bin/test_k_wise_family: lib/libQSBD_DEBUG.a
bin/test_k_wise_family: test/CMakeFiles/test_k_wise_family.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/mnt/c/Users/Marcos/OneDrive/Área de Trabalho/Meus Projetos/Big-Data-IC/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/test_k_wise_family"
	cd "/mnt/c/Users/Marcos/OneDrive/Área de Trabalho/Meus Projetos/Big-Data-IC/build/test" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_k_wise_family.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/test_k_wise_family.dir/build: bin/test_k_wise_family

.PHONY : test/CMakeFiles/test_k_wise_family.dir/build

test/CMakeFiles/test_k_wise_family.dir/clean:
	cd "/mnt/c/Users/Marcos/OneDrive/Área de Trabalho/Meus Projetos/Big-Data-IC/build/test" && $(CMAKE_COMMAND) -P CMakeFiles/test_k_wise_family.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/test_k_wise_family.dir/clean

test/CMakeFiles/test_k_wise_family.dir/depend:
	cd "/mnt/c/Users/Marcos/OneDrive/Área de Trabalho/Meus Projetos/Big-Data-IC/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/mnt/c/Users/Marcos/OneDrive/Área de Trabalho/Meus Projetos/Big-Data-IC" "/mnt/c/Users/Marcos/OneDrive/Área de Trabalho/Meus Projetos/Big-Data-IC/test" "/mnt/c/Users/Marcos/OneDrive/Área de Trabalho/Meus Projetos/Big-Data-IC/build" "/mnt/c/Users/Marcos/OneDrive/Área de Trabalho/Meus Projetos/Big-Data-IC/build/test" "/mnt/c/Users/Marcos/OneDrive/Área de Trabalho/Meus Projetos/Big-Data-IC/build/test/CMakeFiles/test_k_wise_family.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : test/CMakeFiles/test_k_wise_family.dir/depend

