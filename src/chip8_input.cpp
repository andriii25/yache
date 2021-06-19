//
// Created by andry on 8/22/17.
//

#include <GLFW/glfw3.h>
#include "chip8_input.h"

chip8_input::chip8_input(GLFWwindow *glWindow)
{
    window = glWindow;


    // Hack to use the member function as a callback
    glfwSetWindowUserPointer(window, this);
    auto func = [](GLFWwindow* wind)
    {
        static_cast<chip8_input*>(glfwGetWindowUserPointer(wind))->setShouldClose();
    };
    glfwSetWindowCloseCallback(window, func);

}

bool chip8_input::getShouldClose()
{
    return shouldClose;
}

void chip8_input::pollInput()
{
    glfwPollEvents();
    std::fill(keys.begin(), keys.end(), false);
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        setShouldClose();
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        keys[0x1] = true;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        keys[0x2] = true;
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        keys[0x3] = true;
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        keys[0xC] = true;
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        keys[0x4] = true;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        keys[0x5] = true;
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        keys[0x6] = true;
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        keys[0xD] = true;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        keys[0x7] = true;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        keys[0x8] = true;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        keys[0x9] = true;
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        keys[0xE] = true;
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        keys[0xA] = true;
    }
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        keys[0x0] = true;
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    {
        keys[0xB] = true;
    }
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
    {
        keys[0xF] = true;
    }
}

std::array<bool, 16> chip8_input::getKeys()
{
    return keys;
}

void chip8_input::setShouldClose() {

    shouldClose = true;

}
