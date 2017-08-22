#pragma once

#include <array>

class chip8_input
{
public:
    chip8_input(GLFWwindow* glWindow);
    std::array<bool, 16> getKeys();
    void pollInput();

    bool getShouldClose();

private:
    bool shouldClose = false;
    std::array<bool, 16> keys;
    GLFWwindow* window;

};

