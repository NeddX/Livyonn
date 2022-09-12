cmake_minimum_required(VERSION 3.8)

message ("MFC> Compiler 'prober'")
if (MSVC_VERSION GREATER_EQUAL "1900")
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("/std:c++20" _cpp_latest_flag_supported)
    if (_cpp_latest_flag_supported)
        add_compile_options("/std:c++20")
    endif()
endif()


# 'OR' doesn't work and im not even going to bother.
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set (CMAKE_CXX_STANDARD 23)
    set (CMAKE_CXX_STANDARD_REQUIRED ON)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set (CMAKE_CXX_STANDARD 23)
    set (CMAKE_CXX_STANDARD_REQUIRED ON)
endif()


add_executable(liv
"main.cpp"
"Lib/Utils.cpp"
"Analyzers/Armavir/Lexer.cpp"
"Analyzers/Armavir/Analyzer.cpp"
"Analyzers/Armavir/FunctionDefinition.cpp"
"Analyzers/Armavir/Statement.cpp"
"Runtime/AVM/AVM.cpp"
"Runtime/AVM/Runtime.cpp"
"Runtime/AVM/ByteBuffer.cpp"
"Runtime/AVM/Registers.cpp"
"Analyzers/AVM/Analyzer.cpp"
"Analyzers/AVM/Lexer.cpp"
"Compilers/AVM/Compiler.cpp"
"Compilers/Armavir/Compiler.cpp"
)