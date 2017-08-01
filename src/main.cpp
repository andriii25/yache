#include <getopt.h>
#include "chip8.h"
#include "debugger.h"


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
    chip8* mChip8 = new chip8();
    mChip8->loadRom(argv[optind]);

    debugger* debug = new debugger(mChip8);

    if (isDebug)
    {
        debug->setContinue(false);
    }

    while (!debug->shouldExit())
    {
        if (debug->shouldContinue())
        {
            mChip8->stepCycle();
        } else
        {
            debug->prompt();
        }
        if (mChip8->drawFlag)
        {
            //TODO: Draw graphics
        }

    }

    return 0;
}