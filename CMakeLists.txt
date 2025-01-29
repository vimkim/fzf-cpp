cmake_minimum_required(VERSION 3.10)
project(fzf_cpp_project)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(fzf_cpp src/main.cpp)

# Find ncurses package
find_package(Curses REQUIRED)

# Link against ncurses
target_link_libraries(fzf_cpp PRIVATE ${CURSES_LIBRARIES})

# Include ncurses headers
target_include_directories(fzf_cpp PRIVATE ${CURSES_INCLUDE_DIR})
