#include <getopt.h>
#include <chrono>
#include "chip8.h"
#include "debugger.h"
#include "chip8_display.h"
#include "chip8_input.h"
#include "log.h"

void printUsage()
{
    ERR("Usage: ./yache [-df] [file]\n"
            "Example: ./yache ../roms/PONG\n");
}

int main(int argc, char** argv)
{
    //TODO OpenGL and sound stuff (imgui?)




    bool isDebug = false;
    int freq = 1000;
    int c;
    while ((c = getopt(argc, argv, "f:d")) != -1)
    {
        switch (c)
        {
            case 'd':
                isDebug = true;
                break;
            case 'f':
                freq = strtoll(optarg, nullptr, 10);
                break;
            case '?':
                if (optopt == 'f')
                {
                    ERR("Option -%c requires an argument.\n", optopt);
                } else
                {
                    ERR("Unknown option character -%c", optopt);
                }
                printUsage();
                return EINVAL;
            default:
                return EINVAL;
        }
    }
    if (optind == argc)
    {
        ERR("Too few arguments.\n");
        printUsage();
        return EXIT_FAILURE;
    }
    chip8 *cpu = new chip8();
    cpu->loadRom(argv[optind]);
    debugger *debug = new debugger(cpu);


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

    auto tick = std::chrono::milliseconds(16);
    auto current = std::chrono::high_resolution_clock::now();
    int cyclesPerFrame = freq / 60;
    int cyclesLeft = cyclesPerFrame;
    int count = 0;
    while (!shouldClose)
    {

        if (debug->shouldContinue())
        {

            if (cyclesLeft)
            {
                cpu->stepCycle();
                count++;
                cyclesLeft--;
            }

        } else
        {
            debug->prompt();
        }


        //TODO: Link this to monitor refresh rate or at least a fixed FPS
        if (cpu->drawFlag)
        {
            display->update(cpu->getGraphics());
        }
        if (current + tick < std::chrono::high_resolution_clock::now())
        {
            //printf("*****%i cycle/frame\n", count);
            count = 0;
            display->render();
            display->swapBuffers();
            current = std::chrono::high_resolution_clock::now();
            cyclesLeft = cyclesPerFrame;
        }
        input->pollInput();

        cpu->setKeys(input->getKeys());
        shouldClose = input->getShouldClose();// || debug->shouldExit();

    }
    glfwSetWindowShouldClose(window, true);

    return 0;
}
