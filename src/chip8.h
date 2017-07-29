#pragma once

#include <cstdint>
#include <array>
#include <fstream>



class chip8
{
public:
    bool drawFlag;
    chip8();
    void stepCycle();
    void loadRom(const char* fileName);
    void setKeys();

private:
    static const int MEMORY_SIZE = 4096;
    uint16_t opcode;
    std::array<uint8_t, MEMORY_SIZE> memory;

    std::array<uint8_t, 16> V;

    uint16_t I;
    uint16_t pc;

    std::array<uint8_t, 64*32> gfx;

    std::array<uint16_t, 16> stack;
    uint16_t sp;

    uint8_t delay_timer;
    uint8_t sound_timer;

    std::array<uint8_t, 16> key;
    FILE* rom;

    void display_clear();

    friend class debugger;
};

