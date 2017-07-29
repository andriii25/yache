#pragma once

#include "chip8.h"




class debugger
{
public:
    debugger(chip8* chip8);

    void prompt();

private:
    //TODO: Make members when possible static
    chip8* _chip8;
    const static std::array<std::string, 3> commandList;
    bool shouldContinue = false;

    int run_command(std::string command);

    int print(std::istringstream& argv);
    int step(std::istringstream& argv);
    int continue_cmd(std::istringstream& argv);
};




