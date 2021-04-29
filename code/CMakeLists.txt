cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)

# SET(CMAKE_BINARY_DIR build/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

Project(Iris)

set(SOURCES src/main.cpp 
            src/Capture.cpp         include/Capture.h
            src/Matrix.cpp          include/Matrix.h
            src/toojpeg             include/toojpeg.h
            src/ConfigManager.cpp   include/ConfigManager.h)

add_executable(out ${SOURCES})
target_include_directories(out PRIVATE "include")