# Livyonn
"Sometimes you just need to create a compiler~~"

Bytecode compiled programming language written in C++, designed by me, built by me. and you best hope, for it to work.

# Pre-Alpha
You can compile very basic stuff like variable declerations, assignments, basic math and return.
I got alot to do.

# Build
Requires CMake v3.0 (Ninja) or higher with GCC installed. Just run 'build.sh' on linux and it should create builds files and compile the code. Execute the output executable inside ```build``` folder by typing ```./liv <paremeters>```.
Did not configure this project for windows but will do sometime in the future.
Tested on Manjaro Linux works and compiles just fine.

# Usage
```liv <command> <parameters>```
  
```run``` to compile and execute your source file.

The compiler will compile depending on your file extension.

```.arm``` files are Armavir source code files.

```.amc``` files are AVM bytecode files.

```.aex``` files are AVM executable files.

Examples:

```liv run path/to/your/file/mycode.arm```

```liv run path/to/your/file/bytecode.amc```

Check out ```Test``` folder.

Basic files are fully functional meaning you can execute them fine.

Sample files are prototypes, they will not execute hence why they are prototypes.  

AVM Exectuable files are not implemented yet.
