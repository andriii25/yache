cmake_minimum_required(VERSION 3.8)
project(yache)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/main.cpp src/chip8.cpp src/chip8.h src/debugger.cpp src/debugger.h src/log.h)
add_executable(yache ${SOURCE_FILES})