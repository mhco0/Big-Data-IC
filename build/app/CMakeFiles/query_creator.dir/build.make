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
include app/CMakeFiles/query_creator.dir/depend.make

# Include the progress variables for this target.
include app/CMakeFiles/query_creator.dir/progress.make

# Include the compile flags for this target's objects.
include app/CMakeFiles/query_creator.dir/flags.make

app/CMakeFiles/query_creator.dir/query_creator/query_creator.cpp.o: app/CMakeFiles/query_creator.dir/flags.make
app/CMakeFiles/query_creator.dir/query_creator/query_creator.cpp.o: ../app/query_creator/query_creator.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object app/CMakeFiles/query_creator.dir/query_creator/query_creator.cpp.o"
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/app" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/query_creator.dir/query_creator/query_creator.cpp.o -c "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/app/query_creator/query_creator.cpp"

app/CMakeFiles/query_creator.dir/query_creator/query_creator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/query_creator.dir/query_creator/query_creator.cpp.i"
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/app" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/app/query_creator/query_creator.cpp" > CMakeFiles/query_creator.dir/query_creator/query_creator.cpp.i

app/CMakeFiles/query_creator.dir/query_creator/query_creator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/query_creator.dir/query_creator/query_creator.cpp.s"
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/app" && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/app/query_creator/query_creator.cpp" -o CMakeFiles/query_creator.dir/query_creator/query_creator.cpp.s

# Object files for target query_creator
query_creator_OBJECTS = \
"CMakeFiles/query_creator.dir/query_creator/query_creator.cpp.o"

# External object files for target query_creator
query_creator_EXTERNAL_OBJECTS =

bin/query_creator: app/CMakeFiles/query_creator.dir/query_creator/query_creator.cpp.o
bin/query_creator: app/CMakeFiles/query_creator.dir/build.make
bin/query_creator: lib/libnlohmann_json.a
bin/query_creator: lib/libQSBD_STREAM_MAKER.a
bin/query_creator: lib/libQSBD_UTILS.a
bin/query_creator: lib/libQSBD_LOG.a
bin/query_creator: lib/libQSBD_DEBUG.a
bin/query_creator: lib/libQSBD_COMMUM_HEADER.a
bin/query_creator: lib/libQSBD_GLOBAL_GEN.a
bin/query_creator: app/CMakeFiles/query_creator.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/query_creator"
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/app" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/query_creator.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
app/CMakeFiles/query_creator.dir/build: bin/query_creator

.PHONY : app/CMakeFiles/query_creator.dir/build

app/CMakeFiles/query_creator.dir/clean:
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/app" && $(CMAKE_COMMAND) -P CMakeFiles/query_creator.dir/cmake_clean.cmake
.PHONY : app/CMakeFiles/query_creator.dir/clean

app/CMakeFiles/query_creator.dir/depend:
	cd "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/app" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/app" "/mnt/c/Users/Marcos/Desktop/Meus projetos/Big-Data-IC/build/app/CMakeFiles/query_creator.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : app/CMakeFiles/query_creator.dir/depend

