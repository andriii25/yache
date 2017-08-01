//
// Created by andry on 7/29/17.
//

#include <array>
#include <iostream>
#include <sstream>
#include "debugger.h"
#include "log.h"


const std::array<std::string, 4> debugger::commandList =
    {
        "step",
        "print",
        "continue",
        "exit"
    };

debugger::debugger(chip8* chip8)
{
    _chip8 = chip8;
}

void debugger::prompt()
{
    printf("Yache-Debug > ");
    std::string line;
    getline(std::cin, line);
    run_command(line);
}

void debugger::setContinue(bool val)
{
    _shouldContinue = val;
}

bool debugger::shouldContinue()
{
    return _shouldContinue;
}

bool debugger::shouldExit()
{
    return _shouldExit;
}

int debugger::run_command(std::string fullCommand)
{

    std::istringstream is(fullCommand);

    std::string command;
    std::getline(is, command, ' ');

    DBG_CMD cmd;
    for (int i = 0; i < commandList.size(); i++)
    {
        if (command == commandList[i])
        {
            cmd = static_cast<DBG_CMD>(i);
            break;
        }
    }

    switch (cmd)
    {
        case DBG_CMD::STEP:
            step(is);
            break;
        case DBG_CMD::PRINT:
            print(is);
            break;
        case DBG_CMD::CONTINUE:
            continue_cmd(is);
            break;
        case DBG_CMD::EXIT:
            exit_cmd(is);
            break;
        default:
            ERR("Unknown debug instruction?");
    }
}

int debugger::print(std::istringstream& argv)
{
    //TODO: Proper debug print
    std::array<bool, 6> shouldPrint;
    std::fill(shouldPrint.begin(), shouldPrint.end(), false);

    LOG("PC: 0x%X", _chip8->pc);
    LOG("SP: 0x%X", _chip8->sp);
    for (int i = 0; i < 16; i++)
    {
        LOG("V%X: %X", i, _chip8->V[i]);
    }
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
    _shouldContinue = true;
    return EXIT_SUCCESS;
}

int debugger::exit_cmd(std::istringstream &argv)
{
    _shouldExit = true;
    return EXIT_SUCCESS;
}


