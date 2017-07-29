//
// Created by andry on 7/29/17.
//

#include <array>
#include <iostream>
#include <sstream>
#include "debugger.h"


const std::array<std::string, 3> debugger::commandList =
    {
        "step",
        "print",
        "continue"
    };

debugger::debugger(chip8* chip8)
{
    _chip8 = chip8;
}

void debugger::prompt()
{
    printf("Yache-Debug > ");
    /*
    char* line;
    size_t lSize;
    getline(&line, &lSize, stdin);
    if (line[lSize - 1] == '\n')
    {
        line[lSize - 1] = '\0';
    }
    char* command = strtok(line, " ");
    */

    std::string line;
    getline(std::cin, line);
    run_command(line);
}

int debugger::run_command(std::string fullCommand)
{

    std::istringstream is(fullCommand);

    std::string command;
    std::getline(is, command, ' ');

    unsigned int index = 10000;
    for (int i = 0; i < commandList.size(); i++)
    {
        printf("%s\n", commandList[i].c_str());
        if (command == commandList[i])
        {
            index = i;
            break;
        }
    }

    switch (index)
    {
        case 0:
            step(is);
            break;
        case 1:
            print(is);
            break;
        case 2:
            continue_cmd(is);
            break;
        default:
            printf("Unknown debug instruction?\n");
    }
}

int debugger::print(std::istringstream& argv)
{
    //TODO: Proper debug print
    bool printPC = false;
    bool printSP = false;
    printf("PC: 0x%X\n", _chip8->pc);
}

int debugger::step(std::istringstream& argv)
{
    std::string arg;
    if (std::getline(argv, arg, ' '))
    {
        unsigned int stepCount = stoi(arg);
        for (int i = 0; i < stepCount; i++)
        {
            _chip8->stepCycle();
        }
    }
    else
    {
        _chip8->stepCycle();
    }
    return EXIT_SUCCESS;
}

int debugger::continue_cmd(std::istringstream& argv)
{
    printf("Welp no idea how to implement this\n");
}

