cmake_minimum_required(VERSION 3.10)
project(example_project)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find ncurses package
find_package(Curses REQUIRED)

# Find static ncurses libraries
find_library(CURSES_NCURSES_LIBRARY NAMES libncurses.a ncurses)
find_library(CURSES_TINFO_LIBRARY NAMES libtinfo.a tinfo)

add_executable(example src/example.cpp src/fzf.hpp)

# Link against static libraries
target_link_libraries(example PRIVATE -static ${CURSES_NCURSES_LIBRARY}
                                      ${CURSES_TINFO_LIBRARY})

# Include ncurses headers
target_include_directories(example PRIVATE ${CURSES_INCLUDE_DIR})
