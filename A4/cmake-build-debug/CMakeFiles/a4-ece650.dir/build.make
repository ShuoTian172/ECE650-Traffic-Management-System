# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/shuotian/Desktop/Software ECE650/ece650-a4-submit"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/shuotian/Desktop/Software ECE650/ece650-a4-submit/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/a4-ece650.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/a4-ece650.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/a4-ece650.dir/flags.make

CMakeFiles/a4-ece650.dir/a4-ece650.cpp.o: CMakeFiles/a4-ece650.dir/flags.make
CMakeFiles/a4-ece650.dir/a4-ece650.cpp.o: ../a4-ece650.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/shuotian/Desktop/Software ECE650/ece650-a4-submit/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/a4-ece650.dir/a4-ece650.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/a4-ece650.dir/a4-ece650.cpp.o -c "/Users/shuotian/Desktop/Software ECE650/ece650-a4-submit/a4-ece650.cpp"

CMakeFiles/a4-ece650.dir/a4-ece650.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/a4-ece650.dir/a4-ece650.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/shuotian/Desktop/Software ECE650/ece650-a4-submit/a4-ece650.cpp" > CMakeFiles/a4-ece650.dir/a4-ece650.cpp.i

CMakeFiles/a4-ece650.dir/a4-ece650.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/a4-ece650.dir/a4-ece650.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/shuotian/Desktop/Software ECE650/ece650-a4-submit/a4-ece650.cpp" -o CMakeFiles/a4-ece650.dir/a4-ece650.cpp.s

# Object files for target a4-ece650
a4__ece650_OBJECTS = \
"CMakeFiles/a4-ece650.dir/a4-ece650.cpp.o"

# External object files for target a4-ece650
a4__ece650_EXTERNAL_OBJECTS =

a4-ece650: CMakeFiles/a4-ece650.dir/a4-ece650.cpp.o
a4-ece650: CMakeFiles/a4-ece650.dir/build.make
a4-ece650: minisat/libminisat.a
a4-ece650: /usr/lib/libz.dylib
a4-ece650: CMakeFiles/a4-ece650.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/Users/shuotian/Desktop/Software ECE650/ece650-a4-submit/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable a4-ece650"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/a4-ece650.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/a4-ece650.dir/build: a4-ece650

.PHONY : CMakeFiles/a4-ece650.dir/build

CMakeFiles/a4-ece650.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/a4-ece650.dir/cmake_clean.cmake
.PHONY : CMakeFiles/a4-ece650.dir/clean

CMakeFiles/a4-ece650.dir/depend:
	cd "/Users/shuotian/Desktop/Software ECE650/ece650-a4-submit/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/shuotian/Desktop/Software ECE650/ece650-a4-submit" "/Users/shuotian/Desktop/Software ECE650/ece650-a4-submit" "/Users/shuotian/Desktop/Software ECE650/ece650-a4-submit/cmake-build-debug" "/Users/shuotian/Desktop/Software ECE650/ece650-a4-submit/cmake-build-debug" "/Users/shuotian/Desktop/Software ECE650/ece650-a4-submit/cmake-build-debug/CMakeFiles/a4-ece650.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/a4-ece650.dir/depend
