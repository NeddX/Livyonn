cmake_minimum_required(VERSION 3.0.0)
project(Livyonn VERSION 0.1.0)

# include(CTest)
# enable_testing()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "-O3")
#set(CMAKE_CXX_FLAGS_DEBUG "-O3 -pg")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(src_dir "${PROJECT_SOURCE_DIR}/src")

# file(GLOB src_files "*.cpp")

# add_executable(liv ${src_files})

add_executable(liv 
src/main.cpp
src/Lib/src/Utils.cpp
src/Analyzers/Armavir/src/Lexer.cpp
src/Analyzers/Armavir/src/Analyzer.cpp
src/Analyzers/Armavir/src/FunctionDefinition.cpp
src/Analyzers/Armavir/src/Statement.cpp
src/Runtime/AVM/src/AVM.cpp
src/Runtime/AVM/src/Runtime.cpp
src/Runtime/AVM/src/ByteBuffer.cpp
src/Runtime/AVM/src/Registers.cpp
src/Analyzers/AVM/src/Analyzer.cpp
src/Analyzers/AVM/src/Lexer.cpp
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
