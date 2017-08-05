#pragma once

#define GLFW_INCLUDE_NONE

#include <cstdint>
#include <array>
#include <GLFW/glfw3.h>

#include "shader.h"

class chip8_display
{
public:
    ~chip8_display();

    void init();
    void update(const std::array<uint8_t, 64 * 32>& gfx);
    void render();
    void swapBuffers();

    bool shouldClose();

private:
    Shader* shader;
    GLFWwindow* window;

    const int WIDTH = 960;
    const int HEIGHT = 480;

    unsigned int VBO;
    unsigned int EBO;
    unsigned int VAO;
    unsigned int texture;

    std::array<float, 20> vertices =
        {
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f
        };

    std::array<unsigned int, 6> indices =
        {
            0, 1, 2,
            0, 2, 3
        };

    std::array<uint32_t, 2> colorMap =
        {
            0x000000FF,
            0xFFFFFFFF
        };
    std::array<uint32_t, 64 * 32> texData;
    //Callbacks
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id,
                                       GLenum severity, GLsizei length, const GLchar *message, const void* userParam);
    void processInput();
};

