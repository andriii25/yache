#include <getopt.h>
#include "chip8.h"
#include "debugger.h"
#include "chip8_display.h"
#include "chip8_input.h"
#include "log.h"

int main(int argc, char** argv)
{
    //TODO OpenGL and sound stuff (imgui?)




    bool isDebug = false;
    int c;
    while ((c = getopt(argc, argv, "d")) != -1)
    {
        switch (c)
        {
            case 'd':
                isDebug = true;
                break;
            default:
                return EINVAL;
        }
    }
    if (optind == argc)
    {
        return EXIT_FAILURE;
    }
    chip8 *mChip8 = new chip8();
    mChip8->loadRom(argv[optind]);

    debugger *debug = new debugger(mChip8);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    //glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(960, 480, "Yache", nullptr, nullptr);
    if (window == nullptr)
    {
        ERR("Failed to create OpenGL context");
    }

    chip8_display *display = new chip8_display(window);
    display->init();

    chip8_input *input = new chip8_input(window);
    if (isDebug)
    {
        debug->setContinue(false);
    }

    bool shouldClose = false;
    while (!shouldClose)
    {

        if (debug->shouldContinue())
        {
            for (int i = 0; i < 6; i++)
            {
                mChip8->stepCycle();
            }
        } else
        {
            debug->prompt();
        }


        //TODO: Link this to monitor refresh rate or at least a fixed FPS
        if (mChip8->drawFlag)
        {
            display->update(mChip8->getGraphics());
            display->render();
            display->swapBuffers();
        }
        input->pollInput();

        mChip8->setKeys(input->getKeys());
        shouldClose = input->getShouldClose() || debug->shouldExit();

    }
    glfwSetWindowShouldClose(window, true);

    return 0;
}
