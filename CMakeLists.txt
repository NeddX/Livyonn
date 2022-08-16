cmake_minimum_required(VERSION 3.0.0)
project(Livyonn VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "-O3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(liv 
main.cpp
src/Lib/src/Utils.cpp
src/Analyzer/Armavir/src/Lexer.cpp
src/Analyzer/Armavir/src/Analyzer.cpp
src/Analyzer/Armavir/src/FunctionDefinition.cpp
src/Analyzer/Armavir/src/Statement.cpp
src/Compilers/AVM/src/AVM.cpp
src/Compilers/AVM/src/Runtime.cpp
src/Compilers/AVM/src/ByteBuffer.cpp
src/Compilers/AVM/src/Registers.cpp
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
