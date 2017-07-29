//
// Created by andry on 7/28/17.
//
#define LOG_LEVEL (7)
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "chip8.h"
#include "lwlog.h"

chip8::chip8()
{
    pc = 0x200;
    opcode = 0;
    I = 0;
    sp = 0;

    std::fill(gfx.begin(), gfx.end(), 0);
    std::fill(stack.begin(), stack.end(), 0);
    std::fill(V.begin(), V.end(), 0);
    std::fill(memory.begin(), memory.end(), 0);
    srand(time(NULL));


}

void chip8::stepCycle()
{
    opcode = memory[pc] << 8 | memory[pc + 1];

    pc += 2;

    uint16_t addr = opcode & 0x0FFF;
    uint8_t byte = opcode & 0x00FF;
    uint8_t Vx = opcode & 0x0F00 >> 8;
    uint8_t Vy = opcode & 0x00F0 >> 4;
    //TODO: maybe a function pointer table
    //TODO: fprintf should be a macro or sth
    uint8_t tmp = 0;
    switch (opcode & 0xF000)
    {
        case 0x0000:
            switch (opcode & 0x000F)
            {
                case 0x0000: //CLS
                    lwlog_debug("CLS");
                    display_clear();
                    break;
                case 0x000E: //RET
                    lwlog_debug("RET");
                    sp--;
                    pc = stack[sp];
                    break;
                default:
                    lwlog_err("0x0000: Unknown opcode 0x%X", opcode);
            }
        case 0x1000: //JP addr
            lwlog_debug("JP 0x%X", addr);
            pc = addr;
            break;
        case 0x2000: //CALL addr
            lwlog_debug("CALL 0x%X", addr);
            stack[sp] = pc;
            sp++;
            pc = addr;
            break;
        case 0x3000: //SE Vx, byte
            lwlog_debug("SE V%X, %X", Vx, byte);
            if (V[Vx] == (byte))
            {
                pc += 2;
            }
            break;
        case 0x4000: //SNE Vx, byte
            lwlog_debug("SNE V%X, %X", Vx, byte);
            if (V[Vx] != (byte))
            {
                pc += 2;
            }
            break;
        case 0x5000: //SE Vx, Vy
            lwlog_debug("SNE V%X, V%X", Vx, Vy);
            if (V[Vx] == V[Vy])
            {
                pc += 2;
            }
            break;
        case 0x6000: //LD Vx, byte
            lwlog_debug("LD V%X, %X", Vx, byte);
            V[Vx] = (byte);
            break;
        case 0x7000: //ADD Vx, byte
            lwlog_debug("ADD V%X, %X", Vx, byte);
            V[Vx] += (byte);
            break;
        case 0x8000:
            switch (opcode & 0x000F)
            {
                case 0x0000: //LD Vx, Vy
                    lwlog_debug("LD V%X, V%X", Vx, Vy);
                    V[Vx] = V[Vy];
                    break;
                case 0x0001: //OR Vx, Vy
                    lwlog_debug("OR V%X, V%X", Vx, Vy);
                    V[Vx] |= V[Vy];
                    break;
                case 0x0002: //AND Vx, Vy
                    lwlog_debug("AND V%X, V%X", Vx, Vy);
                    V[Vx] &= V[Vy];
                    break;
                case 0x0003: //XOR Vx, Vy
                    lwlog_debug("XOR V%X, V%X", Vx, Vy);
                    V[Vx] ^= V[Vy];
                    break;
                case 0x0004: //ADD Vx, Vy
                    lwlog_debug("ADD V%X, V%X", Vx, Vy);
                    tmp = V[Vx];
                    V[Vx] += V[Vy];
                    if (V[Vx] < tmp)
                    {
                        V[0xF] = 1;
                    }
                    else
                    {
                        V[0xF] = 0;
                    }
                    break;
                case 0x0005: //SUB Vx, Vy
                    lwlog_debug("SUB V%X, V%X", Vx, Vy);
                    if (V[Vy] > V[Vx])
                    {
                        //VF = !borrow
                        V[0xF] = 0;
                    }
                    else
                    {
                        V[0xF] = 1;
                    }
                    V[Vx] -= V[Vy];
                    break;
                case 0x0006: //SHR Vx
                    lwlog_debug("SHR V%X", Vx);
                    V[0xF] = V[Vx] & 0x1;
                    V[Vx] >>= 1;
                    break;
                case 0x0007: //SUBN Vx, Vy
                    lwlog_debug("SUBN V%X, V%X", Vx, Vy);
                    V[Vx] = V[Vy] - V[opcode & 0x0F00];
                    if (V[Vx] > V[Vy])
                    {
                        V[0xF] = 0;
                    }
                    else
                    {
                        V[0xF] = 1;
                    }
                    break;
                case 0x000E: //SHL Vx
                    lwlog_debug("SHL V%X", Vx);
                    V[0xF] = V[Vx] & 0x8;
                    V[Vx] <<= 1;
                    break;
                default:
                    lwlog_err("0x8000: Unknown opcode 0x%X", opcode)

            }
            break;
        case 0x9000: //SNE Vx, Vy
            lwlog_debug("SNE V%X, V%X", Vx, Vy);
            if (V[Vx] != V[Vy])
            {
                pc += 2;
            }
            break;
        case 0xA000: //LD I, addr
            lwlog_debug("LD I, 0x%X", addr);
            I = addr;
            break;
        case 0xB000: //JP V0, addr
            lwlog_debug("JP V0, 0x%X", addr);
            pc = V[0] + (addr);
            break;
        case 0xC000: //RND Vx, byte
            lwlog_debug("RND V%X, %X", Vx, byte);
            V[Vx] = (uint8_t)(rand() % 256) & (byte);
            break;
        case 0xD000: //DRW Vx, Vy, nibble
            lwlog_debug("DRW V%X, V%X, %X", Vx, Vy, opcode & 0x000F);
            //TODO: Draw
            break;
        case 0xE000:
            switch (opcode & 0x00FF)
            {
                //TODO: Check this
                case 0x009E: //SKP Vx
                    lwlog_debug("SKP V%X", Vx);
                    if (key[Vx])
                    {
                        pc += 2;
                    }
                    break;
                case 0x00A1:
                    lwlog_debug("SNKP V%X", Vx);
                    if (!key[Vx])
                    {
                        pc += 2;
                    }
                    break;
                default:
                    lwlog_err("0xE000: Unknown opcode 0x%X", opcode);
            }
            break;
        default:
            lwlog_err("Unknown opcode 0x%X", opcode)
    }

    if (delay_timer > 0)
    {
        delay_timer--;
    }

    if (sound_timer > 0)
    {
        printf("Beep!\n");
        sound_timer--;
    }

    //TODO this
}

void chip8::loadRom(const char* fileName)
{
    rom = fopen(fileName, "rb");
    if (rom == NULL)
    {
        lwlog_err("File error while reading");
        fprintf(stderr, "Err: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    fseek(rom, 0, SEEK_END);
    long fSize = ftell(rom);
    fseek(rom, 0, SEEK_SET);

    fread(memory.data() + 0x200, 1, fSize, rom);
}

void inline chip8::display_clear()
{
    std::fill(gfx.begin(), gfx.end(), 0);
    drawFlag = true;
}
