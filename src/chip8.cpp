//
// Created by andry on 7/28/17.
//
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include "chip8.h"
#include "log.h"

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
    std::copy(fontset.begin(), fontset.end(), memory.begin() + 0x50);
    srand(time(NULL));


}

std::array<uint8_t, 64*32>& chip8::getGraphics()
{
    return gfx;
};

void chip8::stepCycle()
{
    opcode = memory[pc] << 8 | memory[pc + 1];

    pc += 2;
    drawFlag = false;

    uint16_t addr = opcode & 0x0FFF;
    uint8_t byte = opcode & 0x00FF;
    uint8_t Vx = (opcode & 0x0F00) >> 8;
    uint8_t Vy = (opcode & 0x00F0) >> 4;
    //TODO: maybe a function pointer table
    //TODO: fprintf should be a macro or sth
    uint16_t tmp = 0;
    uint8_t bcd_tmp = 0;
    switch (opcode & 0xF000)
    {
        case 0x0000:
            switch (opcode & 0x000F)
            {
                case 0x0000: //CLS
                    LOG("CLS");
                    display_clear();
                    break;
                case 0x000E: //RET
                    LOG("RET");
                    sp--;
                    pc = stack[sp];
                    break;
                default:
                    ERR("0x0000: Unknown opcode 0x%X", opcode);
            }
            break;
        case 0x1000: //JP addr
            LOG("JP 0x%X", addr);
            pc = addr;
            break;
        case 0x2000: //CALL addr
            LOG("CALL 0x%X", addr);
            stack[sp] = pc;
            sp++;
            pc = addr;
            break;
        case 0x3000: //SE Vx, byte
            LOG("SE V%X, %X", Vx, byte);
            if (V[Vx] == byte)
            {
                pc += 2;
            }
            break;
        case 0x4000: //SNE Vx, byte
            LOG("SNE V%X, %X", Vx, byte);
            if (V[Vx] != byte)
            {
                pc += 2;
            }
            break;
        case 0x5000: //SE Vx, Vy
            LOG("SNE V%X, V%X", Vx, Vy);
            if (V[Vx] == V[Vy])
            {
                pc += 2;
            }
            break;
        case 0x6000: //LD Vx, byte
            LOG("LD V%X, %X", Vx, byte);
            V[Vx] = byte;
            break;
        case 0x7000: //ADD Vx, byte
            LOG("ADD V%X, %X", Vx, byte);
            V[Vx] += byte;
            break;
        case 0x8000:
            switch (opcode & 0x000F)
            {
                case 0x0000: //LD Vx, Vy
                    LOG("LD V%X, V%X", Vx, Vy);
                    V[Vx] = V[Vy];
                    break;
                case 0x0001: //OR Vx, Vy
                    LOG("OR V%X, V%X", Vx, Vy);
                    V[Vx] |= V[Vy];
                    break;
                case 0x0002: //AND Vx, Vy
                    LOG("AND V%X, V%X", Vx, Vy);
                    V[Vx] &= V[Vy];
                    break;
                case 0x0003: //XOR Vx, Vy
                    LOG("XOR V%X, V%X", Vx, Vy);
                    V[Vx] ^= V[Vy];
                    break;
                case 0x0004: //ADD Vx, Vy
                    LOG("ADD V%X, V%X", Vx, Vy);
                    //TODO: Revise this
                    tmp = V[Vx] + V[Vy];
                    if (tmp & 0xFF00)
                    {
                        V[0xF] = 1;
                    }
                    else
                    {
                        V[0xF] = 0;
                    }
                    V[Vx] = tmp & 0x00FF;
                    break;
                case 0x0005: //SUB Vx, Vy
                    LOG("SUB V%X, V%X", Vx, Vy);
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
                case 0x0006: //SHR Vx, Vy
                    LOG("SHR V%X, V%X", Vx, Vy);
                    V[0xF] = V[Vy] & 0x1;
                    V[Vx] = V[Vy] >> 1;
                    break;
                case 0x0007: //SUBN Vx, Vy
                    LOG("SUBN V%X, V%X", Vx, Vy);
                    V[Vx] = V[Vy] - V[Vx];
                    if (V[Vx] > V[Vy])
                    {
                        V[0xF] = 0;
                    }
                    else
                    {
                        V[0xF] = 1;
                    }
                    break;
                case 0x000E: //SHL Vx, Vy
                    LOG("SHL V%X, V%X", Vx, Vy);
                    V[0xF] = (V[Vy] & 0x80) >> 7;
                    V[Vx] = V[Vy] << 1;
                    break;
                default:
                    ERR("0x8000: Unknown opcode 0x%X", opcode);

            }
            break;
        case 0x9000: //SNE Vx, Vy
            LOG("SNE V%X, V%X", Vx, Vy);
            if (V[Vx] != V[Vy])
            {
                pc += 2;
            }
            break;
        case 0xA000: //LD I, addr
            LOG("LD I, 0x%X", addr);
            I = addr;
            break;
        case 0xB000: //JP V0, addr
            LOG("JP V0, 0x%X", addr);
            pc = V[0] + (addr);
            break;
        case 0xC000: //RND Vx, byte
            LOG("RND V%X, %X", Vx, byte);
            V[Vx] = (uint8_t)(rand() % 256) & byte;
            break;
        case 0xD000: //DRW Vx, Vy, nibble
            LOG("DRW V%X, V%X, %X", Vx, Vy, opcode & 0x000F);
            for (int i = V[Vy]; i < V[Vy] + (opcode & 0x000F); i++)
            {
                int yOffset = i - V[Vy];
                uint8_t line = memory[I + yOffset];
                for (int j = V[Vx]; j < V[Vx] + 8; j++)
                {
                    int xOffset = j - V[Vx];
                    if (line & (0x80 >> xOffset))
                    {
                        if (gfx[i * 64 + j])
                        {
                            V[0xF] = 1;
                        }
                        gfx[i * 64 + j] ^= 1;
                    }

                }
            }
            drawFlag = true;
            break;
        case 0xE000:
            switch (opcode & 0x000F)
            {
                //TODO: Check this
                case 0x000E: //SKP Vx
                    LOG("SKP V%X", Vx);
                    if (key[Vx])
                    {
                        pc += 2;
                    }
                    break;
                case 0x0001:
                    LOG("SNKP V%X", Vx);
                    if (!key[Vx])
                    {
                        pc += 2;
                    }
                    break;
                default:
                    ERR("0xE000: Unknown opcode 0x%X", opcode);
            }
            break;
        case 0xF000:
            switch (opcode & 0x00FF)
            {
                case 0x0007: //LD Vx, DT
                    LOG("LD V%X, DT", Vx);
                    V[Vx] = delay_timer;
                    break;
                case 0x000A: //LD Vx, K
                    LOG("LD V%X, [key]", Vx);
                    //TODO: Input
                    break;
                case 0x0015: //LD DT, Vx
                    LOG("LD DT, V%X", Vx);
                    delay_timer = V[Vx];
                    break;
                case 0x0018: //LD ST, Vx
                    LOG("LD ST, V%X", Vx);
                    sound_timer = V[Vx];
                    break;
                case 0x001E: //ADD I, Vx
                    LOG("ADD I, V%X", Vx);
                    I += V[Vx];
                    break;
                case 0x0029: //LD F, Vx
                    LOG("LD F, V%X", Vx);
                    I = 0x50 + V[Vx] * 5;
                    break;
                case 0x0033: //LD B, Vx
                    LOG("LD B, V%X", Vx);
                    bcd_tmp = V[Vx];
                    memory[I + 2] = bcd_tmp % 10; bcd_tmp /= 10;
                    memory[I + 1] = bcd_tmp % 10; bcd_tmp /= 10;
                    memory[I] = bcd_tmp % 10;
                    break;
                case 0x0055: //LD [I], Vx
                    LOG("LD 0x%X, V%X", I, Vx);
                    for (int i = 0; i <= Vx; i++)
                    {
                        memory[I + i] = V[i];
                    }
                    I += Vx + 1;
                    break;
                case 0x0065: //LD Vx, [I]
                    LOG("LD V%X, 0x%X", Vx, I);
                    for (int i = I; i <= I + Vx; i++)
                    {
                        int vOffset = i - I;
                        V[vOffset] = memory[i];
                    }
                    I += Vx + 1;
                    break;
                default:
                    ERR("0x000F: Unknown opcode 0x%X", opcode);

            }
            break;
        default:
            ERR("Unknown opcode 0x%X", opcode);
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
        ERR("File error while reading");
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
