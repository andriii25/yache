cmake_minimum_required(VERSION 3.8)
project(yache)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/main.cpp
        src/chip8.cpp
        src/chip8.h
        src/debugger.cpp
        src/debugger.h
        src/log.h
        src/chip8_display.cpp
        src/chip8_display.h
        src/shader.cpp
        src/shader.h)

add_executable(yache ${SOURCE_FILES})

find_package(glfw3 3.2 REQUIRED)
find_package(OpenGL REQUIRED)
add_definitions(${OPENGL_DEFINITIONS})
find_package(X11 REQUIRED)

# TODO: Windows probably doesn't like this
set (LIBS glfw X11 Xrandr Xinerama Xi Xxf86vm Xcursor GL dl)
include_directories(
        include
        ${OPENGL_INCLUDE_DIRS}
)

add_library(GLAD "src/glad.c")
set (LIBS ${LIBS} GLAD)

target_link_libraries(${PROJECT_NAME} ${LIBS})