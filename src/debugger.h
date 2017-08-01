#pragma once

#include "chip8.h"




class debugger
{
public:
    debugger(chip8* chip8);

    void prompt();

    bool shouldExit();

    void setContinue(bool val);
    bool shouldContinue();

private:
    enum DBG_CMD : unsigned int
    {
        STEP,
        PRINT,
        CONTINUE,
        EXIT

    };
    enum class PRINT_TYPE : unsigned int
    {
        PC,
        SP,
        STACK,
        REGISTER,
        SOUND_TIMER,
        DELAY_TIMER
    };
    //TODO: Make members when possible static
    chip8* _chip8;
    const static std::array<std::string, 4> commandList;
    bool _shouldContinue = true;
    bool _shouldExit = false;

    int run_command(std::string command);

    int print(std::istringstream& argv);
    int step(std::istringstream& argv);
    int continue_cmd(std::istringstream& argv);
    int exit_cmd(std::istringstream& argv);
};




