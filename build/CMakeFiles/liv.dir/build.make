# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.23

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/nedd/source/repos/Livyonn

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nedd/source/repos/Livyonn/build

# Include any dependencies generated for this target.
include CMakeFiles/liv.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/liv.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/liv.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/liv.dir/flags.make

CMakeFiles/liv.dir/main.cpp.o: CMakeFiles/liv.dir/flags.make
CMakeFiles/liv.dir/main.cpp.o: ../main.cpp
CMakeFiles/liv.dir/main.cpp.o: CMakeFiles/liv.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nedd/source/repos/Livyonn/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/liv.dir/main.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/liv.dir/main.cpp.o -MF CMakeFiles/liv.dir/main.cpp.o.d -o CMakeFiles/liv.dir/main.cpp.o -c /home/nedd/source/repos/Livyonn/main.cpp

CMakeFiles/liv.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/liv.dir/main.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nedd/source/repos/Livyonn/main.cpp > CMakeFiles/liv.dir/main.cpp.i

CMakeFiles/liv.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/liv.dir/main.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nedd/source/repos/Livyonn/main.cpp -o CMakeFiles/liv.dir/main.cpp.s

CMakeFiles/liv.dir/src/Lib/Utils/src/Utils.cpp.o: CMakeFiles/liv.dir/flags.make
CMakeFiles/liv.dir/src/Lib/Utils/src/Utils.cpp.o: ../src/Lib/Utils/src/Utils.cpp
CMakeFiles/liv.dir/src/Lib/Utils/src/Utils.cpp.o: CMakeFiles/liv.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nedd/source/repos/Livyonn/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/liv.dir/src/Lib/Utils/src/Utils.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/liv.dir/src/Lib/Utils/src/Utils.cpp.o -MF CMakeFiles/liv.dir/src/Lib/Utils/src/Utils.cpp.o.d -o CMakeFiles/liv.dir/src/Lib/Utils/src/Utils.cpp.o -c /home/nedd/source/repos/Livyonn/src/Lib/Utils/src/Utils.cpp

CMakeFiles/liv.dir/src/Lib/Utils/src/Utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/liv.dir/src/Lib/Utils/src/Utils.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nedd/source/repos/Livyonn/src/Lib/Utils/src/Utils.cpp > CMakeFiles/liv.dir/src/Lib/Utils/src/Utils.cpp.i

CMakeFiles/liv.dir/src/Lib/Utils/src/Utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/liv.dir/src/Lib/Utils/src/Utils.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nedd/source/repos/Livyonn/src/Lib/Utils/src/Utils.cpp -o CMakeFiles/liv.dir/src/Lib/Utils/src/Utils.cpp.s

CMakeFiles/liv.dir/src/Compilers/Armavir/src/Lexer.cpp.o: CMakeFiles/liv.dir/flags.make
CMakeFiles/liv.dir/src/Compilers/Armavir/src/Lexer.cpp.o: ../src/Compilers/Armavir/src/Lexer.cpp
CMakeFiles/liv.dir/src/Compilers/Armavir/src/Lexer.cpp.o: CMakeFiles/liv.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nedd/source/repos/Livyonn/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/liv.dir/src/Compilers/Armavir/src/Lexer.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/liv.dir/src/Compilers/Armavir/src/Lexer.cpp.o -MF CMakeFiles/liv.dir/src/Compilers/Armavir/src/Lexer.cpp.o.d -o CMakeFiles/liv.dir/src/Compilers/Armavir/src/Lexer.cpp.o -c /home/nedd/source/repos/Livyonn/src/Compilers/Armavir/src/Lexer.cpp

CMakeFiles/liv.dir/src/Compilers/Armavir/src/Lexer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/liv.dir/src/Compilers/Armavir/src/Lexer.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nedd/source/repos/Livyonn/src/Compilers/Armavir/src/Lexer.cpp > CMakeFiles/liv.dir/src/Compilers/Armavir/src/Lexer.cpp.i

CMakeFiles/liv.dir/src/Compilers/Armavir/src/Lexer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/liv.dir/src/Compilers/Armavir/src/Lexer.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nedd/source/repos/Livyonn/src/Compilers/Armavir/src/Lexer.cpp -o CMakeFiles/liv.dir/src/Compilers/Armavir/src/Lexer.cpp.s

CMakeFiles/liv.dir/src/Compilers/Armavir/src/Analyzer.cpp.o: CMakeFiles/liv.dir/flags.make
CMakeFiles/liv.dir/src/Compilers/Armavir/src/Analyzer.cpp.o: ../src/Compilers/Armavir/src/Analyzer.cpp
CMakeFiles/liv.dir/src/Compilers/Armavir/src/Analyzer.cpp.o: CMakeFiles/liv.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nedd/source/repos/Livyonn/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/liv.dir/src/Compilers/Armavir/src/Analyzer.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/liv.dir/src/Compilers/Armavir/src/Analyzer.cpp.o -MF CMakeFiles/liv.dir/src/Compilers/Armavir/src/Analyzer.cpp.o.d -o CMakeFiles/liv.dir/src/Compilers/Armavir/src/Analyzer.cpp.o -c /home/nedd/source/repos/Livyonn/src/Compilers/Armavir/src/Analyzer.cpp

CMakeFiles/liv.dir/src/Compilers/Armavir/src/Analyzer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/liv.dir/src/Compilers/Armavir/src/Analyzer.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nedd/source/repos/Livyonn/src/Compilers/Armavir/src/Analyzer.cpp > CMakeFiles/liv.dir/src/Compilers/Armavir/src/Analyzer.cpp.i

CMakeFiles/liv.dir/src/Compilers/Armavir/src/Analyzer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/liv.dir/src/Compilers/Armavir/src/Analyzer.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nedd/source/repos/Livyonn/src/Compilers/Armavir/src/Analyzer.cpp -o CMakeFiles/liv.dir/src/Compilers/Armavir/src/Analyzer.cpp.s

CMakeFiles/liv.dir/src/Compilers/Armavir/src/FunctionDefinition.cpp.o: CMakeFiles/liv.dir/flags.make
CMakeFiles/liv.dir/src/Compilers/Armavir/src/FunctionDefinition.cpp.o: ../src/Compilers/Armavir/src/FunctionDefinition.cpp
CMakeFiles/liv.dir/src/Compilers/Armavir/src/FunctionDefinition.cpp.o: CMakeFiles/liv.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nedd/source/repos/Livyonn/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/liv.dir/src/Compilers/Armavir/src/FunctionDefinition.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/liv.dir/src/Compilers/Armavir/src/FunctionDefinition.cpp.o -MF CMakeFiles/liv.dir/src/Compilers/Armavir/src/FunctionDefinition.cpp.o.d -o CMakeFiles/liv.dir/src/Compilers/Armavir/src/FunctionDefinition.cpp.o -c /home/nedd/source/repos/Livyonn/src/Compilers/Armavir/src/FunctionDefinition.cpp

CMakeFiles/liv.dir/src/Compilers/Armavir/src/FunctionDefinition.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/liv.dir/src/Compilers/Armavir/src/FunctionDefinition.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nedd/source/repos/Livyonn/src/Compilers/Armavir/src/FunctionDefinition.cpp > CMakeFiles/liv.dir/src/Compilers/Armavir/src/FunctionDefinition.cpp.i

CMakeFiles/liv.dir/src/Compilers/Armavir/src/FunctionDefinition.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/liv.dir/src/Compilers/Armavir/src/FunctionDefinition.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nedd/source/repos/Livyonn/src/Compilers/Armavir/src/FunctionDefinition.cpp -o CMakeFiles/liv.dir/src/Compilers/Armavir/src/FunctionDefinition.cpp.s

# Object files for target liv
liv_OBJECTS = \
"CMakeFiles/liv.dir/main.cpp.o" \
"CMakeFiles/liv.dir/src/Lib/Utils/src/Utils.cpp.o" \
"CMakeFiles/liv.dir/src/Compilers/Armavir/src/Lexer.cpp.o" \
"CMakeFiles/liv.dir/src/Compilers/Armavir/src/Analyzer.cpp.o" \
"CMakeFiles/liv.dir/src/Compilers/Armavir/src/FunctionDefinition.cpp.o"

# External object files for target liv
liv_EXTERNAL_OBJECTS =

liv: CMakeFiles/liv.dir/main.cpp.o
liv: CMakeFiles/liv.dir/src/Lib/Utils/src/Utils.cpp.o
liv: CMakeFiles/liv.dir/src/Compilers/Armavir/src/Lexer.cpp.o
liv: CMakeFiles/liv.dir/src/Compilers/Armavir/src/Analyzer.cpp.o
liv: CMakeFiles/liv.dir/src/Compilers/Armavir/src/FunctionDefinition.cpp.o
liv: CMakeFiles/liv.dir/build.make
liv: CMakeFiles/liv.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/nedd/source/repos/Livyonn/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable liv"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/liv.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/liv.dir/build: liv
.PHONY : CMakeFiles/liv.dir/build

CMakeFiles/liv.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/liv.dir/cmake_clean.cmake
.PHONY : CMakeFiles/liv.dir/clean

CMakeFiles/liv.dir/depend:
	cd /home/nedd/source/repos/Livyonn/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nedd/source/repos/Livyonn /home/nedd/source/repos/Livyonn /home/nedd/source/repos/Livyonn/build /home/nedd/source/repos/Livyonn/build /home/nedd/source/repos/Livyonn/build/CMakeFiles/liv.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/liv.dir/depend
