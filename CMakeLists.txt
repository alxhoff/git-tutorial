project(git_tutorial)
cmake_minimum_required(VERSION 3.4 FATAL_ERROR)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

include_directories(include)
include_directories(lib)

file(GLOB SOURCES "src/*.c")

add_executable(foo ${SOURCES})

file(GLOB ESPL_LIB_SRCS "lib/*.c")
add_library(ESPL_LIB ${ESPL_LIB_SRCS})
target_link_libraries(foo ESPL_LIB)
