//
// Created by andry on 1/22/18.
//

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <array>
#include "gdbstub.h"
#include "log.h"


gdbstub::gdbstub(chip8 *_cpu, short int _port)
{
    cpu = _cpu;
    port = _port;
}

gdbstub::~gdbstub()
{
    shutdown(gdbSocket, SHUT_RDWR);
}

int gdbstub::init()
{
    //TODO: Error handling, check casting

    //Initializing the basic server socket
    int serverSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
    {
        ERR("Couldn't create socket");
        return -1;

    }

    //Setting the server up
    //TODO: Do this the modern way with getaddrinfo()
    sockaddr_in serverSockAddr = {};
    serverSockAddr.sin_family = AF_INET;
    serverSockAddr.sin_port = htons(port);
    serverSockAddr.sin_addr.s_addr = htons(INADDR_ANY); //listen on all addresses

    int reuse_enabled = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &reuse_enabled, sizeof(reuse_enabled)) < 0)
    {
        ERR("Couldn't set socket level");
        return -1;
    }

    //Bind socket to address
    if (bind(serverSocket, (struct sockaddr *) &serverSockAddr, sizeof(serverSockAddr)) < 0)
    {
        ERR("Couldn't bind socket");
        return -1;
    }

    //Start listening
    if (listen(serverSocket, 1) < 0)
    {
        ERR("Couldn't listen to socket");
        return -1;
    }

    LOG("Waiting for gdb connection...");
    sockaddr_in clientSockAddr;
    sockaddr *clientAddr = (sockaddr *) &clientSockAddr;
    socklen_t clientAddrLength = sizeof(clientSockAddr);

    //We'll continue communicating from this socket now on
    gdbSocket = accept(serverSocket, clientAddr, &clientAddrLength);
    if (gdbSocket < 0)
    {
        ERR("Couldn't accept GDB client");
        return -1;
    } else
    {
        LOG("Connected to GDB client");
        clientSockAddr.sin_addr.s_addr = ntohl(clientSockAddr.sin_addr.s_addr);
    }

    //We only have 1 connection, so can stop listening
    shutdown(serverSocket, SHUT_RDWR);


    while (true)
    {
        if (handlePacket() < 0)
        {
            return -1;
        }
    }
}

int gdbstub::handlePacket()
{
    std::string cmd = readCommand();

    if (cmd.length() == 0)
    {
        return -1;
    }

    switch (cmd[0])
    {
        case '+':
            return 0;
        case 'q':
            handleQuery(cmd);
            break;
        case 'H':
            sendReply("OK"); //No threads to set so we just OK
            break;
        case '?':
            //TODO: handleSignal or sth like that?
            sendReply("S05");
            break;
        case 'g':
            readRegisters();
            break;
        case 'm':
            readMemory(cmd);
            break;
        case 'p':
            readRegister(cmd);
            break;
        default:
            sendReply("");
    }
    return 0;

}

void gdbstub::handleQuery(const std::string &command)
{
    //TODO: Fix magic numbers (7, 9) as length
    if (command.compare(1, 7, "TStatus") == 0)
    {
        //Maybe enable tracepoints instead of just saying there is none?
        sendReply("TO");
    } else if (command.compare(1, 9, "Supported") == 0)
    {
        sendReply("PacketSize=800;qXfer:features:read+");
    } else if (command.compare(1, 29, "Xfer:features:read:target.xml") == 0)
    {
        sendReply(targetXML);
    } else if (command.compare(1, 8, "Attached") == 0)
    {
        sendReply("1");
    } else if (command.compare(1, 11, "fThreadInfo") == 0)
    {
        sendReply("m0");
    } else if (command.compare(1, 11, "sThreadInfo") == 0)
    {
        sendReply("l");
    } else if (command.compare(1, 1, "C") == 0)
    {
        sendReply("QC0");
    } else
    {
        sendReply("");
    }
}

void gdbstub::readRegisters()
{
    std::string reply;
    for (auto it = cpu->V.begin(); it != cpu->V.end(); it++)
    {
        reply += byteToHexString(*it);
    }
    /*
    reply += shortToHexString(cpu->stack[cpu->sp]);
    reply += shortToHexString(cpu->pc);
    reply += "xxxx""xxxx"; //Fake CPSR and LR registers
    */
    sendReply(reply);

}

void gdbstub::readRegister(const std::string &command)
{
    std::string registerHex = command.substr(1);
    int registerIndex = -1;
    if (registerHex.size() == 1)
    {
        registerIndex = hexToNibble(registerHex[0]);
    } else if (registerHex.size() == 2)
    {
        registerIndex = 0;
        registerIndex |= hexToNibble(registerHex[0]) << 4;
        registerIndex |= hexToNibble(registerHex[1]);
    }
    if (registerIndex < 0x10)
    {
        sendReply(byteToHexString(cpu->V[registerIndex]));
    } else
    {
        switch (registerIndex)
        {
            case 0x10: //Stack Pointer
                sendReply(shortToHexString(cpu->stack[cpu->sp]));
                break;
            case 0x11: //Program Counter
                sendReply(shortToHexString(cpu->pc));
                break;
            default:
                sendReply("FFFF");
                return;

        }
    }
}

void gdbstub::readMemory(const std::string &command)
{
    std::string params = command.substr(1);
    int splitIndex = params.find(',');
}

std::string gdbstub::readCommand()
{
    std::string cmd = "";

    uint8_t c;

    c = readByte();
    if (c == '+')
    {
        cmd += c;
        return cmd;
    }

    if (c != GDB_PACKET_START)
    {
        return cmd;
    }

    uint8_t calcChecksum = 0;
    while ((c = readByte()) != GDB_PACKET_END)
    {
        cmd += c;
        calcChecksum += c;
    }
    LOG("recv: %s", cmd.c_str());

    uint8_t recvChecksum = 0;
    recvChecksum |= hexToNibble(readByte()) << 4;
    recvChecksum |= hexToNibble(readByte());

    if (recvChecksum != calcChecksum)
    {
        sendByte(GDB_PACKET_NACK);
        return cmd;
    }

    sendByte(GDB_PACKET_ACK);
    return cmd;
}

uint8_t gdbstub::readByte()
{
    //Is it worth using some kind of const or size instead of 1?
    uint8_t c;
    size_t sizeReceived = recv(gdbSocket, &c, 1, 0);
    if (sizeReceived != 1)
    {
        ERR("Error when receiving data, received %ld", sizeReceived);
        shutdown(gdbSocket, SHUT_RDWR);
        return 0;
    }
    return c;
}

void gdbstub::sendByte(uint8_t byte)
{
    size_t sizeSent = send(gdbSocket, &byte, 1, 0);
    LOG("send: %c", byte);
    if (sizeSent != 1)
    {
        ERR("Error while sending %c", byte);
        shutdown(gdbSocket, SHUT_RDWR);
        return;
    }
}

void gdbstub::sendReply(const std::string &message)
{

    std::string reply = stringToGDBReply(message);

    LOG("send: %s", reply.c_str());
    int offset = 0;
    int left = reply.length();

    while (left > 0)
    {
        size_t sizeSent = send(gdbSocket, (&(reply.c_str()[0]) + offset), left, 0);
        if (sizeSent < 0)
        {
            ERR("Error while sending reply");
            shutdown(gdbSocket, SHUT_RDWR);
            return;
        }
        offset += sizeSent;
        left -= sizeSent;
    }
}

uint8_t gdbstub::hexToNibble(uint8_t hexChar)
{
    if ('0' <= hexChar && hexChar <= '9')
    {
        return hexChar - '0';
    } else if ('a' <= hexChar && hexChar <= 'f')
    {
        return hexChar - 'a' + 10;
    } else if ('A' <= hexChar && hexChar <= 'F')
    {
        return hexChar - 'A' + 10;
    }

    ERR("Invalid hex char %c", hexChar);
    return 0;
}

std::string gdbstub::stringToGDBReply(const std::string &message)
{
    std::string checksum = byteToHexString(calcChecksum(message));
    std::string reply = "";
    reply += GDB_PACKET_START;
    reply += message;
    reply += GDB_PACKET_END;
    reply += checksum;
    return reply;
}

std::string gdbstub::shortToHexString(uint16_t shortInt)
{
    return byteToHexString((shortInt & 0xFF00) >> 8) + byteToHexString(shortInt & 0x00FF);
}

std::string gdbstub::byteToHexString(uint8_t byte)
{
    std::string hexString = "";
    std::array<uint8_t, 2> nibbles;
    nibbles[0] = (byte & 0xF0) >> 4;
    nibbles[1] = byte & 0x0F;

    for (int i = 0; i < nibbles.size(); i++)
    {
        if (0 <= nibbles[i] && nibbles[i] <= 9)
        {
            hexString += (char) (nibbles[i] + '0');
        } else
        {
            //No further check, as both nibbles are guaranteed to be 4 bit.
            hexString += (char) (nibbles[i] + 'A' - 10);
        }
    }
    return hexString;
}

uint8_t gdbstub::calcChecksum(const std::string &message)
{
    uint8_t checksum = 0;
    for (int i = 0; i < message.size(); i++)
    {
        checksum += message[i];
    }
    return checksum;
}



