#pragma once


#include "chip8.h"

class gdbstub
{
public:
    gdbstub(chip8 *_cpu, short int _port = 22333);

    ~gdbstub();

    int init();

private:
    static const uint8_t GDB_PACKET_START = '$';
    static const uint8_t GDB_PACKET_END = '#';
    static const uint8_t GDB_PACKET_ACK = '+';
    static const uint8_t GDB_PACKET_NACK = '-';

    const std::string targetXML =
        R"foo(l<?xml version="1.0"?>
<!DOCTYPE target SYSTEM "gdb-target.dtd">
<target version="1.0">
 <feature name="org.gnu.gdb.arm.core">
  <reg name="r0" bitsize="8"/>
  <reg name="r1" bitsize="8"/>
  <reg name="r2" bitsize="8"/>
  <reg name="r3" bitsize="8"/>
  <reg name="r4" bitsize="8"/>
  <reg name="r5" bitsize="8"/>
  <reg name="r6" bitsize="8"/>
  <reg name="r7" bitsize="8"/>
  <reg name="r8" bitsize="8"/>
  <reg name="r9" bitsize="8"/>
  <reg name="r10" bitsize="8"/>
  <reg name="r11" bitsize="8"/>
  <reg name="r12" bitsize="8"/>
  <reg name="r13" bitsize="8"/>
  <reg name="r14" bitsize="8"/>
  <reg name="r15" bitsize="8"/>
  <reg name="sp" bitsize="8" type="data_ptr"/>
  <reg name="pc" bitsize="8" type="code_ptr"/>
  <reg name="lr" bitsize="16"/>
  <reg name="cpsr" bitsize="16"/>
 </feature>
</target>
)foo";

    chip8 *cpu;

    uint16_t port;
    int gdbSocket;

    int handlePacket();

    void handleQuery(const std::string &command);

    void readRegisters();

    void readRegister(const std::string &command);

    void readMemory(const std::string &command);


    uint8_t readByte();

    std::string readCommand();

    void sendByte(uint8_t byte);

    void sendReply(const std::string &message);

    uint8_t hexToNibble(uint8_t hexChar);

    std::string stringToGDBReply(const std::string &message);

    std::string byteToHexString(uint8_t byte);

    std::string shortToHexString(uint16_t shortInt);

    uint8_t calcChecksum(const std::string &message);


};



