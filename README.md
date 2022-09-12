# Livyonn
"Sometimes you just need to create a compiler~~"
Bytecode compiled/interpreted programming language written in C++, designed by me, built by me, and you best hope, for it to work.

# Pre-Alpha
Right now it supports only two datatypes, strings and 64bit ints. 
You can do basic mathematical expressions with ints and with strings too (only add operator is allowed of course).
Use and define variables.
Call and declare functions.
Include external files.
Print to standard out.

Check ```Test/basic.arm``` for more info.

# Build Instructions
## Windows
Requires:
- Visual Studio 2022
- Visual C++ Development Tools (including CMake and Ninja).

After installing, just open the ```CMakeLists.txt``` file in Visual Studio and build.

## Linux
Requires:
- CMake v3.0 with Ninja
- GCC v12.1.1

After installing, simply run ```chmod +x build.sh``` in your terminal and execute ```build.sh```.

# Usage
```liv <command> <parameters>```.
```run``` to compile and execute your source file.
The compiler will compile depending on your file extension.

-```.arm``` Armavir source code file.
-```.amc``` AVM bytecode file.
-```.aex``` AVM executable file (Not implemented).

Examples:
```liv run path/to/your/file/mycode.arm```.
```liv run path/to/your/file/bytecode.amc```.

Check out ```Test``` folder.
Basic files are fully functional meaning you can execute them fine.
Sample files are prototypes, they will not execute hence why they are prototypes.  