# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

string(
    APPEND _opts
    "$<IF:$<CXX_COMPILER_ID:MSVC>,"
        "/W4;$<$<CONFIG:RELEASE>:/O2>,"
        "-Wall;-Wextra;-Werror;"
            "$<$<CONFIG:RELEASE>:-O3>"
            "$<$<CXX_COMPILER_ID:Clang>:-stdlib=libc++>"
    ">"
)

project ("Livyonn")

#set_property(TARGET Livyonn PROPERTY CXX_STANDARD 20)

# Include sub-projects.
add_subdirectory ("src")
