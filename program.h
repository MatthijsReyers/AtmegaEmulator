#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <bitset> // May be removed later, just here for debug reasons.

#include <optcode.h>

// Global program vector that holds a list of all optcodes.
// -----------------------------------------------------------
std::vector<optcode> program;
int64_t programCounter;

void loadprogram(std::string &fileUrl)
{
    char optcodeBuffer[2];
    short convertBuffer;
    std::ifstream programFile;
    programFile.open(fileUrl);
    while (programFile.good())
    {
        // Read two bytes (= chars) from file.
        programFile.read(optcodeBuffer, 2);

        // Convert bytes to one short.
        convertBuffer = (((short)optcodeBuffer[0]) << 8) | (0x00ff & optcodeBuffer[1]);

        std::cout << convertBuffer << std::endl;

        // Append to program vector.
        program.push_back(optcode(convertBuffer));
    }
    printf("done loading!\n");
}

void showProgram()
{
    for (auto &code : program)
    {
        short temp = code.getBits();
        std::cout << std::bitset<16>(temp) << std::endl;
    }
}

void loadDebugProgram()
{
    program.push_back(optcode(0b1110000000000000)); // LDI r16 0
    program.push_back(optcode(0b1110000000010011)); // LDI r17 3
    program.push_back(optcode(0b0001110000000001)); // ADC r16 r7
}