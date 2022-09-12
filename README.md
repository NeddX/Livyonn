# Livyonn
*"Sometimes you just need to create a compiler~~"*<br/>

***

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
```liv <command> <parameters>```.<br/>
```run``` to compile and execute your file.<br/><br/>
The compiler will compile depending on your file extension.<br/>

- ```.arm``` Armavir source code file.<br/>
- ```.amc``` AVM bytecode file.<br/>
- ```.aex``` AVM bytecode binary file (Not implemented yet).<br/>

Examples:<br/>
- ```liv run path/to/your/file/mycode.arm```<br/>
- ```liv run path/to/your/file/bytecode.amc```<br/>

Check out ```Test``` folder.<br/>
Basic files are fully functional meaning you can execute them fine.<br/>
Sample files are prototypes, they will not execute hence why they are prototypes.<br/>