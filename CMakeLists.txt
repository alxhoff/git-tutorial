project(POSIX_state_machine)
cmake_minimum_required(VERSION 3.4 FATAL_ERROR)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

include_directories(include)
include_directories(lib)

file(GLOB SOURCES "src/*.c")

add_executable(state_machine ${SOURCES})

file(GLOB STATES_LIB_SRCS "lib/*.c")
add_library(STATES_LIB STATIC ${STATES_LIB_SRCS})
target_link_libraries(state_machine STATES_LIB)

if(UNIX AND NOT APPLE)
    target_link_libraries(state_machine rt)
endif()
