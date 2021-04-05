cmake_minimum_required(VERSION 3.10)

# SET(CMAKE_BINARY_DIR build/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

Project(Iris)

add_executable(Iris src/main.cpp)