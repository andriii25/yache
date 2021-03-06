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

    std::array<uint8_t, 64*32>& getGraphics();
    void setKeys(std::array<bool, 16> key);
    void setLoadStoreQuirk(bool loadStoreQuirk);
    void setShiftQuirk(bool shiftQuirk);

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

    bool isWaitingForKey = false;
    uint8_t lastPressed = 0x10;
    uint8_t Vkey = 0x10;

    std::array<bool, 16> keys;
    FILE* rom;

    std::array<uint8_t, 80> fontset
        {
            0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
            0x20, 0x60, 0x20, 0x20, 0x70, // 1
            0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
            0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
            0x90, 0x90, 0xF0, 0x10, 0x10, // 4
            0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
            0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
            0xF0, 0x10, 0x20, 0x40, 0x40, // 7
            0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
            0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
            0xF0, 0x90, 0xF0, 0x90, 0x90, // A
            0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
            0xF0, 0x80, 0x80, 0x80, 0xF0, // C
            0xE0, 0x90, 0x90, 0x90, 0xE0, // D
            0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
            0xF0, 0x80, 0xF0, 0x80, 0x80  // F
        };

    bool load_store_quirk = false;
    bool shift_quirk = false;

    void display_clear();

    friend class debugger;
};

