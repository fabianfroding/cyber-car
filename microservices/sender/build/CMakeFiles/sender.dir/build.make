# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.14.0/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.14.0/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/olivermanzi/Desktop/PlayGround/School/Cyber_Systems_dit638/repos/cyber-car/microservices/sender

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/olivermanzi/Desktop/PlayGround/School/Cyber_Systems_dit638/repos/cyber-car/microservices/sender/build

# Include any dependencies generated for this target.
include CMakeFiles/sender.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/sender.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/sender.dir/flags.make

CMakeFiles/sender.dir/src/sender.cpp.o: CMakeFiles/sender.dir/flags.make
CMakeFiles/sender.dir/src/sender.cpp.o: ../src/sender.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/olivermanzi/Desktop/PlayGround/School/Cyber_Systems_dit638/repos/cyber-car/microservices/sender/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/sender.dir/src/sender.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/sender.dir/src/sender.cpp.o -c /Users/olivermanzi/Desktop/PlayGround/School/Cyber_Systems_dit638/repos/cyber-car/microservices/sender/src/sender.cpp

CMakeFiles/sender.dir/src/sender.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/sender.dir/src/sender.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/olivermanzi/Desktop/PlayGround/School/Cyber_Systems_dit638/repos/cyber-car/microservices/sender/src/sender.cpp > CMakeFiles/sender.dir/src/sender.cpp.i

CMakeFiles/sender.dir/src/sender.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/sender.dir/src/sender.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/olivermanzi/Desktop/PlayGround/School/Cyber_Systems_dit638/repos/cyber-car/microservices/sender/src/sender.cpp -o CMakeFiles/sender.dir/src/sender.cpp.s

# Object files for target sender
sender_OBJECTS = \
"CMakeFiles/sender.dir/src/sender.cpp.o"

# External object files for target sender
sender_EXTERNAL_OBJECTS =

sender: CMakeFiles/sender.dir/src/sender.cpp.o
sender: CMakeFiles/sender.dir/build.make
sender: CMakeFiles/sender.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/olivermanzi/Desktop/PlayGround/School/Cyber_Systems_dit638/repos/cyber-car/microservices/sender/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable sender"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/sender.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/sender.dir/build: sender

.PHONY : CMakeFiles/sender.dir/build

CMakeFiles/sender.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/sender.dir/cmake_clean.cmake
.PHONY : CMakeFiles/sender.dir/clean

CMakeFiles/sender.dir/depend:
	cd /Users/olivermanzi/Desktop/PlayGround/School/Cyber_Systems_dit638/repos/cyber-car/microservices/sender/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/olivermanzi/Desktop/PlayGround/School/Cyber_Systems_dit638/repos/cyber-car/microservices/sender /Users/olivermanzi/Desktop/PlayGround/School/Cyber_Systems_dit638/repos/cyber-car/microservices/sender /Users/olivermanzi/Desktop/PlayGround/School/Cyber_Systems_dit638/repos/cyber-car/microservices/sender/build /Users/olivermanzi/Desktop/PlayGround/School/Cyber_Systems_dit638/repos/cyber-car/microservices/sender/build /Users/olivermanzi/Desktop/PlayGround/School/Cyber_Systems_dit638/repos/cyber-car/microservices/sender/build/CMakeFiles/sender.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/sender.dir/depend
