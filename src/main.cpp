#include <getopt.h>
#include <chrono>
#include "chip8.h"
#include "debugger.h"
#include "chip8_display.h"
#include "chip8_input.h"
#include "log.h"

void printUsage()
{
    ERR("Usage: ./yache [-dfsl] [file]\n"
            "Example: ./yache ../roms/PONG\n");
}

int main(int argc, char** argv)
{
    //TODO OpenGL and sound stuff (imgui?)




    bool isDebug = false;
    bool load_store_quirk = false;
    bool shift_quirk = false;
    int freq = 1000;

    static struct option long_options[] =
        {
            {"load-quirk", no_argument, 0, 'l'},
            {"shift-quirk", no_argument, 0, 's'},
            {"debug", no_argument, 0, 'd'},
            {"freg", required_argument, 0, 'f'},
            {0, 0,0,0 }
        };

    int c;
    int option_index = 0;
    while ((c = getopt_long(argc, argv, "f:dls", long_options, &option_index)) != -1)
    {
        switch (c)
        {
            case 'd':
                isDebug = true;
                break;
            case 'f':
                freq = strtoll(optarg, nullptr, 10);
                break;
            case 'l':
                load_store_quirk = true;
                break;
            case 's':
                shift_quirk = true;
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
    cpu->setLoadStoreQuirk(load_store_quirk);
    cpu->setShiftQuirk(shift_quirk);
    cpu->loadRom(argv[optind]);


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
    debugger *debug = new debugger(cpu);
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
