cmake_minimum_required(VERSION 3.10)

# SET(CMAKE_BINARY_DIR build/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

Project(Iris)

set(SOURCES src/main.cpp src/Capture.cpp include/Capture.h)
add_executable(out ${SOURCES})
target_include_directories(out PRIVATE "include")